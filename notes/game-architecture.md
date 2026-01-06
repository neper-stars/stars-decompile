# Stars! Game Architecture

## High-Level System Overview

Stars! is organized into several interconnected subsystems:

```
+-------------------+     +-------------------+     +-------------------+
|    UI Layer       |     |   Game Logic      |     |   AI System       |
|  (mdi.c, popup.c, |<--->|  (turn.c, turn2.c,|<--->|  (ai.c, ai2.c,    |
|   scan.c, tb.c)   |     |   turn3.c)        |     |   ai3.c, ai4.c)   |
+-------------------+     +-------------------+     +-------------------+
         |                        |                        |
         v                        v                        v
+-------------------+     +-------------------+     +-------------------+
|   Display/Render  |     |   Core Systems    |     |  AI Utilities     |
|  (report.c, vcr.c)|     | (planet.c, ship.c,|     |  (aiutil.c,       |
|                   |     |  battle.c, mine.c)|     |   aiu.c)          |
+-------------------+     +-------------------+     +-------------------+
                                  |
                                  v
                    +-------------------+
                    |   Data Layer      |
                    | (save.c, file.c,  |
                    |  io.c, log.c)     |
                    +-------------------+
```

## Source File Organization

### Core Game Systems

| File | Purpose |
|------|---------|
| `stars.c` | Main entry point, WinMain, message loop |
| `init.c` | Game initialization, startup procedures |
| `turn.c` | Turn processing phase 1 |
| `turn2.c` | Turn processing phase 2 |
| `turn3.c` | Turn processing phase 3 |
| `create.c` | New game creation, universe generation |

### Entity Management

| File | Purpose |
|------|---------|
| `planet.c` | Planet logic, colonization, production |
| `ship.c` | Ship management, fleet operations |
| `ship2.c` | Additional ship functions |
| `thing.c` | Generic "things" (minefields, packets, wormholes) |
| `mine.c` | Mining operations, minefield mechanics |

### Combat System

| File | Purpose |
|------|---------|
| `battle.c` | Battle resolution, damage calculations |
| `vcr.c` | Battle VCR (Video Combat Recorder) playback |

### AI System

| File | Purpose |
|------|---------|
| `ai.c` | Main AI controller, DoAiTurn() |
| `ai2.c` | AI strategy layer |
| `ai3.c` | AI tactical decisions |
| `ai4.c` | AI support functions |
| `aiutil.c` | AI utility functions |
| `aiu.c` | Additional AI utilities |

### User Interface

| File | Purpose |
|------|---------|
| `mdi.c` | MDI (Multiple Document Interface) window management |
| `scan.c` | Scanner/map window |
| `popup.c` | Pop-up windows, detail views |
| `msg.c` | Message system, player communications |
| `report.c` | Report generation and display |
| `tb.c` | Toolbar handling |

### Supporting Systems

| File | Purpose |
|------|---------|
| `race.c` | Race definitions and traits |
| `produce.c` | Production queue management |
| `research.c` | Technology research system |
| `save.c` | Save/load game state |
| `file.c` | File I/O operations |
| `log.c` | Turn log (x-file) handling |
| `util.c` | General utilities |
| `utilgen.c` | Generic utilities |
| `strings.c` | String handling |
| `memory.c` | Memory management |

### Data Definitions

| File | Purpose |
|------|---------|
| `types.h` | All struct/type definitions (~2,800 lines) |
| `globals.h/c` | Global variable declarations |
| `parts.h/c` | Ship component definitions (engines, weapons, etc.) |

## Turn Processing Flow

The turn generation process occurs in three phases:

### Phase 1 (turn.c)
1. Fleet movement orders processed
2. Waypoint tasks executed
3. Cargo transfers
4. Colonization attempts
5. Fleet merges/splits

### Phase 2 (turn2.c)
1. Battle resolution
2. Bombing runs
3. Minefield interactions
4. Remote mining
5. Population growth

### Phase 3 (turn3.c)
1. Production completion
2. Research advancement
3. Resource allocation
4. End-of-year cleanup
5. Victory condition checks

## Memory Layout

The game uses a segmented memory model typical of 16-bit Windows:

- **CODE segments** (Code1-Code36): Executable code, ~36 segments
- **DATA segment** (Data37): Global variables and initialized data
- **BSS segment**: Uninitialized data
- **c_common segment**: Common/shared data across modules
- **Resource segments** (Rsrc0-Rsrc11): Bitmaps, strings, dialogs

## Key Global Variables

```c
GAME game;           // Current game settings
GDATA gd;            // Runtime game data/flags
PLANET *lpPlanets;   // Array of all planets
FLEET **rglpfl;      // Array of fleet pointers
SHDEF *rglpshdef[1]; // Ship design definitions
int16_t idPlayer;    // Current player ID
PLAYER rgplr[16];    // All player data (max 16 players)
```

## Event-Driven Architecture

Stars! uses Windows message-based event handling:

1. **WndProc callbacks** for each window type
2. **Timer messages** for animations (VCR) and auto-turn
3. **Custom messages** for inter-window communication
4. **Dialog procedures** for modal interactions
