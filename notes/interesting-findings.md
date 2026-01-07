# Stars! Interesting Findings

## Historical Context

Stars! 2.60j RC3 was released April 26, 2000. This version includes CodeView NB09 debug symbols, likely because it was a Release Candidate intended for testing. The final shipping version would have had these symbols stripped.

## Notable Bug Fixes Revealed in Release Notes

The release notes (`target/26JFIN.txt`) reveal many interesting implementation details:

### Memory Corruption Issues

> "The memory corruption happening in the game creation has been fixed, ships now always receive the right parts."

This suggests the ship design system had buffer overrun issues during game creation.

### The Radiating Engine Bug (Fixed in RC3)

> "Colonists no longer die from non-existent engine radiation"

The Radiating Hydro-Ram Scoop engine is supposed to kill colonists. But there was a bug where OTHER engines were being flagged as radiating. The `grfAbilities` field bit 2 indicates radiation - this was being checked incorrectly.

### Cargo Disappearance

> "The problem where cargo disappear has been fixed"

Multiple references to cargo/minerals vanishing:
- "Mineral transfer no longer loses minerals when transferring more than a ship can handle and then moving back the excess"
- Earlier version: cargo would be lost during complex transfer operations

### Overflow Bugs

> "Fixed overflow bug with >21 Superlatanium armors on a Space Dock with Regenerating Shields"

This reveals:
- Armor is stored in some fixed-precision format
- 21+ Superlatanium (1500 dp each = 31,500+ dp) exceeded a 16-bit limit
- Combined with shield regeneration, this caused integer overflow

> "Ship scanner ranges are no longer restricted to a max of 2047 light years"

Scanner range was stored in an 11-bit field (2^11 = 2048). This was expanded.

### AI Bugs

> "The Maid AI is much less brain dead now"

The AI players have names:
- Robotoids, Turindrones, Automitrons, Rototills, Cybertrons, Macinti

> "Cybertron AIs no longer accidentally give other players colonists"

Gift logic bug in AI cargo transfer code.

> "Computer players no longer build fleets with too many remote miners"

AI ship design/production logic had no upper bound check.

## Code Architecture Observations

### Naming Conventions

The code uses Hungarian notation extensively (common for Windows 3.x era):

| Prefix | Meaning |
|--------|---------|
| `c` | Count |
| `i` | Index |
| `f` | Flag/Boolean |
| `lp` | Long pointer |
| `rg` | Array ("range") |
| `sz` | Null-terminated string |
| `gr` | Group/bitmask |
| `cb` | Count of bytes |
| `dw` | Double word |
| `w` | Word |
| `h` | Handle |
| `hwnd` | Window handle |

### Memory Segments

The game uses the Win16 segmented memory model:
- 36 CODE segments (functions distributed across segments)
- 1 DATA segment for globals
- `c_common` segment for cross-module shared data
- Resource segments for bitmaps, strings, dialogs

### The "PCT" Mystery

Many string variables are named like `PCTD`, `PCTLD`, `PCTDPCTPCT`. These are format strings:
- `PCT` = `%` (percent sign in printf)
- `PCTD` = `%d` (decimal)
- `PCTLD` = `%ld` (long decimal)
- `PCTDPCTPCT` = `%d%%` (decimal followed by literal %)

### Random Number Generation

```c
int32_t lRandSeed1;
int32_t lRandSeed2;
int16_t cRandStack;
int32_t rglRandStack[4][2];  // Stack for push/pop of RNG state
```

The game uses dual-seed RNG with a stack for saving/restoring state. This is important for deterministic replay (Battle VCR).

### Salt/Security

```c
int32_t lSaltCur;
int32_t lSaltLast;
```

"Salt" values are used for:
- File integrity checking
- Preventing save file tampering
- Race file validation

The release notes mention: "Improved checking for hacked race files"

## Interesting Game Mechanics

### Ship Mass and Initiative

From the battle system, ship mass affects initiative:
- Lighter ships move first
- Battle computers add to initiative
- Some weapons have innate initiative bonuses

### Minefield Math

Three minefield types with different mechanics:
- **Normal**: 0.3% hit chance per LY, 100 damage
- **Heavy**: 1.0% hit chance per LY, 500 damage (fleet damage, not per-ship)
- **Speed**: 3.5% × (warp²) hit chance, 128 damage

Speed mines become exponentially dangerous at high warp:
- Warp 9: 3.5% × 81 = ~280% (guaranteed hit per LY!)
- This is why ramscoop ships traveling at warp 10 are risky

### Terraforming

Environment variables stored as signed bytes (-50 to +50):
- Gravity, Temperature, Radiation
- Original values preserved for reversal
- Retro Bomb undoes terraforming

### The Genesis Device

Special planetary device (id=15 in rgplanetary):
- Requires ALL tech at 10+ except Biotechnology at 20
- Costs 5000 resources
- Zero mineral cost (rare!)
- Can only appear in games with random events

## File Format Details

### Magic Number

Files start with `J3D1` (ASCII), likely referencing:
- **J3D**: "Jeff" × 2 + "D" (developers Jeff Johnson, Jeff McBride)
- Or possibly "Johnson 3D" (3D being marketing speak)

### Record Types

The `HDR` structure shows a tagged record format:
```c
uint16_t cb : 10;  // Record size (max 1023 bytes)
uint16_t rt : 6;   // Record type (max 63 types)
```

This is a TLV (Type-Length-Value) format common in file formats.

## Platform-Specific Quirks

### Win16 Handles

All handles are `uint16_t`:
- Window handles (`hwnd`)
- Instance handles (`hInst`)
- Brush handles (`hbr`)
- Icon handles (`hicon`)

### Far Pointers

Some types hint at far pointer usage:
- `char * far` for strings
- Segment:offset addressing

### Timer Resolution

```c
uint32_t ctickLast;
uint32_t vtickTooltipLast;
```

Windows 3.x tick resolution was ~55ms (18.2 ticks/second), explaining why animations could be choppy.

## Potential Exploits (Historical)

The release notes document several fixed exploits:

1. **Space Dock Capacity**: "Fixed loophole that allowed space docks to build ships beyond their capacity"
2. **Illegal Ship Designs**: "Closed illegal ship design loophole. Illegal components are now deleted"
3. **Gift Ships**: "Enemies are now prevented from giving you fleets" (used to crash recipient?)
4. **Fleet Merging**: "Fixed problems associated with merging large numbers of fleets"

## Debug Symbol Statistics

From the extraction scripts:
- 612 global variables
- 1,029 procedures (functions)
- 431 labels (jump targets within functions)
- ~100 struct/type definitions

The debug symbols provide:
- Full function signatures
- Local variable names and types
- Source file groupings
- Line number information (partially)
