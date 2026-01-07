# Stars! AI Systems

## Overview

Stars! includes 6 AI "races" with different strategic personalities. The AI system is implemented across multiple source files:

- `ai.c` - Main AI controller, `DoAiTurn()` entry point
- `ai2.c` - Strategy layer
- `ai3.c` - Tactical decisions
- `ai4.c` - Support functions
- `aiutil.c` - AI utility functions
- `aiu.c` - Additional AI utilities

## AI Races

From the release notes and game documentation:

| ID | Name | Personality |
|----|------|-------------|
| 0 | Random | Randomly selected |
| 1 | Robotoids | Aggressive, expansion-focused |
| 2 | Turindrones | Balanced playstyle |
| 3 | Automitrons | Technology-focused |
| 4 | Rototills | Resource/economy focused |
| 5 | Cybertrons | Military/combat focused |
| 6 | Macinti | Defensive playstyle |

## Skill Levels

| Level | Name | Description |
|-------|------|-------------|
| 0 | Random | Randomly selected |
| 1 | Easy | Reduced effectiveness |
| 2 | Standard | Normal AI behavior |
| 3 | Tough | Enhanced decision making |
| 4 | Expert | Full AI capabilities |

## AI Data Structures

### Global AI State

```c
int16_t fAi;              // AI mode active flag
uint8_t *vlpbAiPlanet;    // AI planet analysis data
uint8_t *vlpbAiData;      // General AI state data
PLANET **vrglpplAi;       // AI planet list
int16_t vclpplAi;         // Count of AI planets
uint8_t vrgcAiParts[45];  // AI component preferences
```

### AIHIST - AI History

The AI tracks historical data for decision making:

```c
typedef struct _aihist {
    // Historical records of player actions
    // Used for threat assessment and diplomacy
} AIHIST;
```

### AISTARBASE - AI Starbase Management

```c
typedef struct _aistarbase {
    int16_t idPlanet;     // Planet with starbase
    int16_t cFreighter;   // Freighter count
    int16_t rgflid[8];    // Associated fleet IDs
} AISTARBASE;
```

### AIPART - AI Part Selection

```c
typedef struct _aipart {
    uint16_t ibit : 4;    // Capability bit
    uint16_t iItem : 5;   // Part item index
    uint16_t cItem : 4;   // Quantity to use
    uint16_t fRandom : 3; // Randomization factor
} AIPART;
```

### CYBERINFO - Cybertron-Specific AI Data

```c
typedef struct _cyberinfo {
    uint16_t iLstPktDir : 3;    // Last packet direction
    uint16_t fBltColony : 1;    // Built colony ship
    uint16_t fLaunchedPkt : 1;  // Launched packet
    uint16_t iPktTarget : 2;    // Packet target priority
    uint16_t fNeedScanPkt : 1;  // Need scanner packet
} CYBERINFO;
```

### CYBERINFOTEMP - Temporary AI State

```c
typedef struct _cyberinfotemp {
    uint16_t fIdleColonizers : 1;  // Has idle colony ships
    uint16_t cIdleFreighters : 2;  // Count of idle freighters
    uint16_t cFreightersDst : 2;   // Freighters at destination
    uint16_t fNeedDefenders : 1;   // Needs defensive ships
    uint16_t fDefended : 1;        // Planet is defended
    uint16_t fUnderAttack : 1;     // Currently under attack
    uint16_t fNeedsMin1 : 1;       // Needs mineral type 1
    uint16_t fNeedsMin2 : 1;       // Needs mineral type 2
    uint16_t fNeedsMin3 : 1;       // Needs mineral type 3
} CYBERINFOTEMP;
```

## AI Decision Flow

### Turn Processing

```
DoAiTurn(iPlayer, wMdPlr)
    │
    ├── Analyze current state
    │   ├── Planet assessment
    │   ├── Fleet status
    │   └── Threat evaluation
    │
    ├── Strategic decisions
    │   ├── Research priorities
    │   ├── Expansion targets
    │   └── Diplomacy updates
    │
    ├── Production decisions
    │   ├── Ship designs needed
    │   ├── Infrastructure
    │   └── Queue management
    │
    └── Fleet orders
        ├── Colonization missions
        ├── Military operations
        └── Resource transport
```

## AI Capabilities by Module

### ai.c - Main Controller
- `DoAiTurn()` - Entry point for AI turn processing
- State initialization
- Turn phase coordination

### ai2.c - Strategy Layer
- Long-term planning
- Research allocation
- Expansion priorities
- Resource management

### ai3.c - Tactical Decisions
- Ship design creation
- Fleet composition
- Target selection
- Battle plan assignment

### ai4.c - Support Functions
- Planet evaluation scoring
- Distance calculations
- Threat assessment

### aiutil.c - Utility Functions
- `AiEvaluatePlanet()` - Score planet value
- `AiSelectTarget()` - Choose attack targets
- `AiBuildShipDesign()` - Create ship designs

## AI Global Arrays

### vrgAiArmadaPotency

```c
uint8_t vrgAiArmadaPotency[4];
```

Military strength thresholds for fleet grouping.

### vrgAiCyberArmadaPotency

```c
uint8_t vrgAiCyberArmadaPotency[4];
```

Cybertron-specific military thresholds.

### rgTechBattle / rgTechTrader

```c
uint8_t rgTechBattle[6];   // Tech priorities for combat
uint8_t rgTechTrader[13];  // Tech priorities for trading
```

## Known AI Bugs (Fixed in 2.60j)

From the release notes:

1. **Too Many Remote Miners**
   > "Computer players no longer build fleets with too many remote miners"

   The AI had no upper limit on mining fleet size.

2. **Cybertron Gift Bug**
   > "Cybertron AIs no longer accidentally give other players colonists"

   Cargo transfer logic was inverted.

3. **Inactive Colonization**
   > "Inactive players no longer try colonizing with non-colonizer ships"

   Ship capability check was missing.

4. **Genesis Device in No Random**
   > "Fixed a bug that inadvertently allowed AIs to build the Genesis Device in No Random Events games"

   Genesis Device should only appear with random events enabled.

## AI Personality Traits (Inferred)

Based on naming and common 4X AI patterns:

### Robotoids
- Early aggression
- Rapid expansion
- Lower tech investment

### Turindrones
- Balanced approach
- Adaptive strategies
- Diplomatic

### Automitrons
- Research priority
- Late-game power
- Defensive early

### Rototills
- Economy focus
- High production
- Turtle strategy

### Cybertrons
- Military buildup
- Fleet-focused
- Aggressive expansion

### Macinti (Maid)
- Defensive posture
- Starbase emphasis
- Counter-attacks

## AI Constants

```c
#define AI_MAX_PLAYERS 16
#define AI_MAX_FLEETS 512
#define AI_MAX_PLANETS 1000
#define AI_DESIGN_SLOTS 16
```

## Integration with Game Logic

The AI system interfaces with:

1. **Production** (`produce.c`)
   - Queue management
   - Build priorities

2. **Research** (`research.c`)
   - Tech allocation
   - Field priorities

3. **Fleet Management** (`ship.c`, `ship2.c`)
   - Design creation
   - Fleet orders

4. **Battle Plans** (`battle.c`)
   - Tactic selection
   - Target priorities
