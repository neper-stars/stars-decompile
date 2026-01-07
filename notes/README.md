# Stars! 2.60j Decompilation Notes

This directory contains documentation and research notes for the Stars! 2.60j RC3 decompilation project.

## Overview

Stars! is a turn-based 4X (eXplore, eXpand, eXploit, eXterminate) space strategy game originally released in 1995 by Jeff Johnson and Jeff McBride. Version 2.60j RC3 was released around April 26, 2000 and is significant because it includes CodeView NB09 debug symbols, making reverse engineering significantly easier.

## Documentation Files

- `game-architecture.md` - High-level overview of game systems and data flow
- `data-structures.md` - Detailed documentation of key data structures
- `ship-components.md` - Ship parts, weapons, and their statistics
- `game-mechanics.md` - Core game mechanics and formulas
- `tech-tree.md` - Technology fields and requirements
- `ai-systems.md` - Computer player behavior and strategies
- `file-formats.md` - Save file and turn file formats
- `interesting-findings.md` - Bugs, quirks, and notable discoveries

## Project Status

- **Total Source Files**: 83 C/H file pairs
- **Extracted Functions**: 1,029+ with full signatures
- **Extracted Globals**: 612 variables
- **Functions Implemented**: ~5% (mostly data tables in parts.c)

## Tools Used

- **Ghidra** (Pascal-aware version) - Primary disassembler
- **Python 3** - Symbol extraction scripts
- **CodeView NB09** - Debug symbol format from the executable
