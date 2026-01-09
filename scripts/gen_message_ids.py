#!/usr/bin/env python3
"""
gen_message_ids.py

Usage:
  python3 gen_message_ids.py input.txt > message_ids.h

Where input.txt contains the C snippet with:
  const char *const aMSGUncompressed[] = { ... };
  const char *const aTUTUncompressed[] = { ... };

It will emit:
  typedef enum MessageId { ... } MessageId;
"""

from __future__ import annotations
import re
import sys
from typing import Dict, List, Tuple


# --- Optional manual naming overrides (index -> name) ---
# Put only the ones you really care about.
OVERRIDES_MSG: Dict[int, str] = {
    # 0: "midColonistsDroppedMassacred",
}
OVERRIDES_TUT: Dict[int, str] = {
    # 0: "midTutWelcome",
}

# If you want a single enum with unique values across both arrays,
# keep TUT_BASE at a non-overlapping range. If you prefer both to be
# in the same numeric space starting at 0, set TUT_BASE = 0.
TUT_BASE = 0x1000

# --- Heuristic name generation helpers ---

STOPWORDS = {
    "the", "a", "an", "and", "or", "of", "to", "in", "on", "at", "as", "is", "are",
    "was", "were", "be", "been", "being", "you", "your", "yours", "their", "they",
    "it", "its", "this", "that", "these", "those", "for", "with", "by", "from",
    "now", "then", "than", "but", "not", "no", "only", "all", "some", "more",
    "less", "into", "out", "up", "down", "over", "under", "again", "next",
}

# Replace Stars! format escapes like \p \s \v00 etc with nothing
ESCAPE_RE = re.compile(r"\\[A-Za-z](?:\d\d)?")  # \p \v00 \i etc
HEXSTAR_RE = re.compile(r"\\X")                # \X
SPLIT_RE = re.compile(r"[^A-Za-z0-9]+")

def title_case_token(tok: str) -> str:
    if not tok:
        return ""
    if tok.isdigit():
        return tok
    return tok[0].upper() + tok[1:].lower()

def make_identifier(prefix: str, s: str, idx: int) -> str:
    # Strip escapes and punctuation
    s2 = ESCAPE_RE.sub(" ", s)
    s2 = HEXSTAR_RE.sub(" ", s2)
    s2 = s2.replace("Stars!", "Stars")
    s2 = s2.replace("'", "")
    s2 = s2.replace("...", " ")
    s2 = s2.strip()

    # Common tutorial filler lines are just " " or empty
    if not s2 or s2.strip() == "":
        return f"{prefix}{idx:04d}Blank"

    # Pull words
    parts = [p for p in SPLIT_RE.split(s2) if p]
    # Remove stopwords, keep first ~7 meaningful tokens
    kept: List[str] = []
    for p in parts:
        pl = p.lower()
        if pl in STOPWORDS:
            continue
        # avoid super-generic tutorial verbs dominating every line
        if prefix.startswith("midTut") and pl in {"read", "goto", "click", "press", "select", "hit"}:
            continue
        kept.append(p)
        if len(kept) >= 7:
            break

    if not kept:
        # fallback to first few tokens regardless
        kept = parts[:5] if parts else [f"Msg{idx:04d}"]

    # Make PascalCase
    core = "".join(title_case_token(k) for k in kept)
    # Keep it from getting ridiculous
    if len(core) > 48:
        core = core[:48]

    # Ensure starts with alpha
    if core and core[0].isdigit():
        core = "N" + core

    return f"{prefix}{core}"

def dedupe(names: List[str]) -> List[str]:
    seen: Dict[str, int] = {}
    out: List[str] = []
    for n in names:
        if n not in seen:
            seen[n] = 1
            out.append(n)
        else:
            seen[n] += 1
            out.append(f"{n}{seen[n]}")
    return out


# --- Parsing ---

# Matches lines like:
#   /*   10 (0x000a) */ "Your colonists are now in control of \p.",
LINE_RE = re.compile(
    r"""/\*\s*(\d+)\s*\(0x[0-9a-fA-F]+\)\s*\*/\s*"((?:[^"\\]|\\.)*)"\s*,?"""
)

def parse_block(text: str, block_name: str) -> List[Tuple[int, str]]:
    # Find the block braces for the given array name
    m = re.search(rf"\b{re.escape(block_name)}\b\s*\[\]\s*=\s*\{{", text)
    if not m:
        return []
    start = m.end()
    # naive brace matching from start
    depth = 1
    i = start
    while i < len(text) and depth > 0:
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
        i += 1
    block = text[start:i]

    items: List[Tuple[int, str]] = []
    for lm in LINE_RE.finditer(block):
        idx = int(lm.group(1))
        raw = lm.group(2)
        # unescape \" and \\n style escapes minimally (keep \p etc already handled later)
        raw = raw.replace(r"\"", '"')
        raw = raw.replace(r"\\", "\\")
        items.append((idx, raw))
    return items


def main() -> int:
    data = sys.stdin.read()

    str_items = parse_block(data, "aSTRUncompressed")
    msg_items = parse_block(data, "aMSGUncompressed")
    tut_items = parse_block(data, "aTUTUncompressed")

    if not msg_items and not tut_items and not str_items:
        print("/* ERROR: no aMSGUncompressed[] or aTUTUncompressed[] blocks found on stdin */")
        return 2

    # --- Build enum entries ---
    idm_entries: List[Tuple[str, int]] = []
    ids_entries: List[Tuple[str, int]] = []
    idt_entries: List[Tuple[str, int]] = []

    if str_items:
        # Ensure sorted by index
        str_items.sort(key=lambda t: t[0])
        names = []
        for idx, s in str_items:
            name = OVERRIDES_MSG.get(idx) or make_identifier("ids", s, idx)
            names.append(name)
        names = dedupe(names)
        for (idx, _), name in zip(str_items, names):
            ids_entries.append((name, idx))

    if msg_items:
        # Ensure sorted by index
        msg_items.sort(key=lambda t: t[0])
        names = []
        for idx, s in msg_items:
            name = OVERRIDES_MSG.get(idx) or make_identifier("idm", s, idx)
            names.append(name)
        names = dedupe(names)
        for (idx, _), name in zip(msg_items, names):
            idm_entries.append((name, idx))

    if tut_items:
        tut_items.sort(key=lambda t: t[0])
        names = []
        for idx, s in tut_items:
            name = OVERRIDES_TUT.get(idx) or make_identifier("idt", s, idx)
            names.append(name)
        names = dedupe(names)
        for (idx, _), name in zip(tut_items, names):
            idt_entries.append((name, TUT_BASE + idx))

    # --- Emit ---
    print("typedef enum StringId")
    print("{")
    for name, val in ids_entries:
        print(f"    {name} = 0x{val:04x},")
    print("} StringId;")

    print("typedef enum MessageId")
    print("{")
    for name, val in idm_entries:
        print(f"    {name} = 0x{val:04x},")
    print("} MessageId;")

    print("typedef enum TutorId")
    print("{")
    for name, val in idt_entries:
        print(f"    {name} = 0x{val:04x},")
    print("} TutorId;")

    # Small sanity note
    if str_items:
        print(f"\n/* STR count: {len(str_items)} (0x0000..0x{(len(str_items)-1):04x}) */")
    if msg_items:
        print(f"/* MSG count: {len(msg_items)} (0x0000..0x{(len(msg_items)-1):04x}) */")
    if tut_items:
        print(f"/* TUT count: {len(tut_items)} (base=0x{TUT_BASE:04x}, last=0x{(TUT_BASE+len(tut_items)-1):04x}) */")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
