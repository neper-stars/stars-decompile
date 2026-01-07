#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""extract_stars_messages.py

Extract and decode Stars! (Win16 NE) nibble-compressed string catalogs.

Stars26jrc3.exe contains multiple independently compressed catalogs:
  - STR (general UI strings)
  - MSG (message templates)
  - TUT (tutor2 strings)
  - PN  (planet name pieces)

Each catalog uses the same layout in its segment:
  - rg*LookupTable     : byte lookup table (alphabet)
  - ai*ChunkOffset     : uint16 offsets (one per 64-ID chunk), relative to a*Cmpr base
  - ac*                : uint8 nibble length for each ID (how many nibbles to read)
  - a*Cmpr             : compressed nibble stream blob

This script locates the globals via nb09_ghidra_globals.json (your dumper output)
and reads the correct NE segment from the .exe.

C output example (for strings_uncompressed.c):

  const char *const aSTRUncompressed[] = {
      /*    0 (0x0000) */ "...",
      ...
  };

Usage:
  python3 extract_stars_messages.py \
    --exe stars26jrc3.exe \
    --globals nb09_ghidra_globals.json \
    --set STR --emit-c --c-array aSTRUncompressed \
    --out strings_uncompressed.c

  python3 extract_stars_messages.py --exe stars26jrc3.exe --globals nb09_ghidra_globals.json --all --emit-c --out -
"""

from __future__ import annotations

import argparse
import json
import os
import re
import struct
import sys
from dataclasses import dataclass
from typing import Dict, Iterable, List, Optional, Tuple


# 64 IDs per chunk (fixed in Stars!)
CSTR_CHUNK_BITS = 6
CSTR_CHUNK_SIZE = 1 << CSTR_CHUNK_BITS


# ---------------------------
# Minimal NE parser utilities
# ---------------------------


def _read_u16_le(b: bytes, off: int) -> int:
    return struct.unpack_from('<H', b, off)[0]


def _read_u32_le(b: bytes, off: int) -> int:
    return struct.unpack_from('<I', b, off)[0]


def load_ne_segment_bytes(path_exe: str, seg_index_1based: int) -> bytes:
    """Load raw bytes of an NE segment by 1-based index."""
    with open(path_exe, 'rb') as f:
        data = f.read()

    if data[:2] != b'MZ':
        raise RuntimeError('Not an MZ executable')
    if len(data) < 0x40:
        raise RuntimeError('File too small to be a valid MZ/NE')

    ne_hdr = _read_u32_le(data, 0x3C)
    if data[ne_hdr:ne_hdr + 2] != b'NE':
        raise RuntimeError(f'Not an NE executable (magic={data[ne_hdr:ne_hdr+2]!r})')

    # NE header fields we need
    ofs_seg_table = _read_u16_le(data, ne_hdr + 0x22)   # from NE header base
    num_segments = _read_u16_le(data, ne_hdr + 0x1C)
    align_shift_cnt = data[ne_hdr + 0x32]

    if seg_index_1based < 1 or seg_index_1based > num_segments:
        raise RuntimeError(f'Segment index {seg_index_1based} out of range 1..{num_segments}')

    segtbl_off = ne_hdr + ofs_seg_table
    entry_off = segtbl_off + (seg_index_1based - 1) * 8

    seg_off_units = _read_u16_le(data, entry_off + 0)
    seg_len_bytes = _read_u16_le(data, entry_off + 2)

    unit_size = 1 << align_shift_cnt
    file_off = seg_off_units * unit_size
    if seg_len_bytes == 0:
        seg_len_bytes = 0x10000

    if file_off + seg_len_bytes > len(data):
        raise RuntimeError('NE segment points past end of file')

    return data[file_off:file_off + seg_len_bytes]


# ---------------------------
# Globals JSON parsing
# ---------------------------


_RE_ARR = re.compile(r'\[(\d+)\]')


def _parse_array_len(c_type: str) -> Optional[int]:
    m = _RE_ARR.search(c_type or '')
    if not m:
        return None
    return int(m.group(1))


def _sizeof_elem(c_type: str) -> int:
    """Very small 'sizeof' for the element type, enough for our tables."""
    t = (c_type or '').split('[', 1)[0].strip()
    if t in ('char', 'uint8_t', 'int8_t'):
        return 1
    if t in ('uint16_t', 'int16_t', 'short', 'unsigned short'):
        return 2
    if t in ('uint32_t', 'int32_t', 'long', 'unsigned long'):
        return 4
    raise RuntimeError(f"Unsupported element type for sizing: '{t}' (from '{c_type}')")


@dataclass(frozen=True)
class GlobalRec:
    name: str
    seg: int
    off: int
    size_bytes: int


def build_globals_index(globals_json_path: str) -> Dict[str, GlobalRec]:
    j = json.load(open(globals_json_path, 'r', encoding='utf-8'))
    out: Dict[str, GlobalRec] = {}

    for rec in j.get('globals', []):
        name = rec.get('name')
        cv = rec.get('cv') or {}
        types = rec.get('types') or {}

        if not name or 'seg' not in cv or 'off' not in cv:
            continue

        c_type = types.get('c_type', '')
        n = _parse_array_len(c_type)
        if n is None:
            continue

        # We only need primitive arrays for the compressed catalogs.
        # The globals json includes many struct arrays (e.g., ENGINE[...]) that
        # we intentionally skip here.
        try:
            size = n * _sizeof_elem(c_type)
        except Exception:
            continue

        out[name] = GlobalRec(name=name, seg=int(cv['seg']), off=int(cv['off']), size_bytes=size)

    return out


def read_global(seg_bytes: bytes, g: GlobalRec) -> bytes:
    end = g.off + g.size_bytes
    if end > len(seg_bytes):
        raise RuntimeError(f"Global {g.name} overruns segment: off=0x{g.off:x} size=0x{g.size_bytes:x} seglen=0x{len(seg_bytes):x}")
    return seg_bytes[g.off:end]


# ---------------------------
# Catalog specs
# ---------------------------


@dataclass(frozen=True)
class CatalogSpec:
    key: str
    cmpr: str
    ac: str
    chunk: str
    lookup: str


CATALOGS: Dict[str, CatalogSpec] = {
    'STR': CatalogSpec('STR', cmpr='aSTRCmpr', ac='acSTR', chunk='aiSTRChunkOffset', lookup='rgSTRLookupTable'),
    'MSG': CatalogSpec('MSG', cmpr='aMSGCmpr', ac='acMSG', chunk='aiMSGChunkOffset', lookup='rgMSGLookupTable'),
    'TUT': CatalogSpec('TUT', cmpr='aTUTCmpr', ac='acTUT', chunk='aiTUTChunkOffset', lookup='rgTUTLookupTable'),
    'PN':  CatalogSpec('PN',  cmpr='aPNCmpr',  ac='acPN',  chunk='aiPNChunkOffset',  lookup='rgPNLookupTable'),
}


# ---------------------------
# Nibble decode
# ---------------------------


def decode_strings(*, lookup_table: bytes, chunk_offsets_u16: List[int], nibble_lens: bytes, cmpr_blob: bytes) -> List[str]:
    """Decode all strings in a catalog."""
    total_ids = len(nibble_lens)
    if total_ids == 0:
        return []

    strings: List[str] = []

    for ids in range(total_ids):
        i_chunk = ids >> CSTR_CHUNK_BITS
        i_off = ids & (CSTR_CHUNK_SIZE - 1)

        if i_chunk >= len(chunk_offsets_u16):
            strings.append('')
            continue

        base_len_idx = i_chunk << CSTR_CHUNK_BITS
        nib_index = sum(nibble_lens[base_len_idx: base_len_idx + i_off])

        cmpr_off = chunk_offsets_u16[i_chunk] + (nib_index >> 1)
        if cmpr_off >= len(cmpr_blob):
            strings.append('')
            continue

        rem_nibbles = nibble_lens[base_len_idx + i_off]
        use_high = ((nib_index & 1) == 0)
        p = cmpr_off
        build = 0
        out_chars: List[str] = []

        while rem_nibbles > 0:
            rem_nibbles -= 1
            byte_val = cmpr_blob[p]
            nib = (byte_val >> 4) if use_high else (byte_val & 0x0F)
            if not use_high:
                p += 1
            use_high = not use_high

            build += nib
            if nib != 0x0F:
                if build >= len(lookup_table):
                    out_chars.append('?')
                else:
                    out_chars.append(chr(lookup_table[build]))
                build = 0

        strings.append(''.join(out_chars))

    return strings


def load_catalog_from_exe(*, exe_path: str, globals_index: Dict[str, GlobalRec], spec: CatalogSpec) -> List[str]:
    needed = [spec.cmpr, spec.ac, spec.chunk, spec.lookup]
    missing = [n for n in needed if n not in globals_index]
    if missing:
        raise RuntimeError(f"Missing globals in json for {spec.key}: {', '.join(missing)}")

    g_cmpr = globals_index[spec.cmpr]
    g_ac = globals_index[spec.ac]
    g_chunk = globals_index[spec.chunk]
    g_lookup = globals_index[spec.lookup]

    seg = g_cmpr.seg
    if not (g_ac.seg == seg and g_chunk.seg == seg and g_lookup.seg == seg):
        raise RuntimeError(
            f"Catalog {spec.key} globals are not in one segment: cmpr={g_cmpr.seg}, ac={g_ac.seg}, chunk={g_chunk.seg}, lookup={g_lookup.seg}"
        )

    seg_bytes = load_ne_segment_bytes(exe_path, seg)

    lookup = read_global(seg_bytes, g_lookup)
    nibble_lens = read_global(seg_bytes, g_ac)

    chunk_raw = read_global(seg_bytes, g_chunk)
    if len(chunk_raw) % 2 != 0:
        raise RuntimeError(f"{g_chunk.name} size is not word-aligned (size={len(chunk_raw)})")
    chunk_offsets = list(struct.unpack('<' + 'H' * (len(chunk_raw) // 2), chunk_raw))

    cmpr_blob = read_global(seg_bytes, g_cmpr)

    return decode_strings(lookup_table=lookup, chunk_offsets_u16=chunk_offsets, nibble_lens=nibble_lens, cmpr_blob=cmpr_blob)


# ---------------------------
# C emission
# ---------------------------


def c_escape(s: str) -> str:
    """Escape to a valid C string literal content (without surrounding quotes)."""
    out: List[str] = []
    for ch in s:
        o = ord(ch)
        if ch == '\\':
            out.append('\\\\')
        elif ch == '"':
            out.append('\\"')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\r':
            out.append('\\r')
        elif ch == '\t':
            out.append('\\t')
        elif 32 <= o <= 126:
            out.append(ch)
        else:
            out.append(f"\\x{o:02x}")
    return ''.join(out)


def emit_c_array(strings: List[str], array_name: str) -> str:
    lines: List[str] = []
    lines.append(f"const char *const {array_name}[] = {{")
    for i, s in enumerate(strings):
        lines.append(f"    /* {i:4d} (0x{i:04x}) */ \"{c_escape(s)}\",")
    lines.append("};")
    lines.append("")
    return "\n".join(lines)


# ---------------------------
# CLI
# ---------------------------


def main() -> int:
    ap = argparse.ArgumentParser(description='Decode Stars! nibble-compressed string catalogs from a Win16 NE .exe.')
    ap.add_argument('--exe', required=True, help='Path to stars26jrc3.exe (Win16 NE).')
    ap.add_argument('--globals', required=True, help='Path to nb09_ghidra_globals.json.')

    which = ap.add_mutually_exclusive_group(required=True)
    which.add_argument('--set', choices=sorted(CATALOGS.keys()), help='Catalog to decode (STR/MSG/TUT/PN).')
    which.add_argument('--all', action='store_true', help='Decode all known catalogs.')

    ap.add_argument('--emit-c', action='store_true', help='Emit C code (const char *const ...[]).')
    ap.add_argument('--c-array', default=None, help='C array name when using --set (default: a<SET>Uncompressed).')
    ap.add_argument('--out', default='-', help='Output path (default: stdout).')

    args = ap.parse_args()

    if not os.path.exists(args.exe):
        raise SystemExit(f"exe not found: {args.exe}")
    if not os.path.exists(args.globals):
        raise SystemExit(f"globals json not found: {args.globals}")

    globals_index = build_globals_index(args.globals)

    specs: List[CatalogSpec]
    if args.all:
        specs = [CATALOGS[k] for k in ('STR', 'MSG', 'TUT', 'PN') if k in CATALOGS]
    else:
        specs = [CATALOGS[args.set]]

    out_f = sys.stdout if args.out == '-' else open(args.out, 'w', encoding='utf-8', newline='\n')
    try:
        for spec in specs:
            strings = load_catalog_from_exe(exe_path=args.exe, globals_index=globals_index, spec=spec)

            if args.emit_c:
                arr_name = args.c_array if args.c_array else f"a{spec.key}Uncompressed"
                out_f.write(emit_c_array(strings, arr_name))
            else:
                # simple text dump
                out_f.write(f"# {spec.key} ({len(strings)} strings)\n")
                for i, s in enumerate(strings):
                    out_f.write(f"{i:04d} {s}\n")
                out_f.write("\n")

        return 0
    finally:
        if out_f is not sys.stdout:
            out_f.close()


if __name__ == '__main__':
    raise SystemExit(main())
