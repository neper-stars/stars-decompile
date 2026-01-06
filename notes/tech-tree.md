# Stars! Technology System

## Technology Fields

Stars! has 6 technology fields, each unlocking different components and capabilities:

| Index | Field | Abbreviation | Focus |
|-------|-------|--------------|-------|
| 0 | Energy | Ene | Shields, beam weapons |
| 1 | Weapons | Wep | Bombs, torpedoes |
| 2 | Propulsion | Prop | Engines, stargates |
| 3 | Construction | Con | Armor, hulls |
| 4 | Electronics | Ele | Scanners, computers |
| 5 | Biotechnology | Bio | Terraforming, some armor |

## Tech Requirements in Code

Components store tech requirements in a 6-byte array (`rgTech[6]`):

```c
typedef struct _engine {
    int16_t id;
    char rgTech[6];  // [Ene, Wep, Prop, Con, Ele, Bio]
    char szName[32];
    // ...
} ENGINE;
```

Example: Trans-Galactic Drive requires Propulsion 9:
```c
.rgTech = {0, 0, 9, 0, 0, 0}
```

Example: Mega Poly Shell requires multiple techs:
```c
.rgTech = {14, 0, 0, 14, 14, 6}  // Ene 14, Con 14, Ele 14, Bio 6
```

## Research Mechanics

### Resources to Tech

Research allocation is done as percentage of total resources. The conversion uses:

```c
int16_t rgTech[6];       // Current tech levels
int16_t rgResSpent[6];   // Resources invested per field
int16_t rgTechTarget;    // Target field for research
```

### Tech Cost Scaling

Tech costs increase exponentially. Base formula (approximate):
```
Cost(level) = BaseResources * level^2 * RaceFactor
```

Where `RaceFactor` depends on racial traits.

### Slow Tech Universe

Games can enable "Slow Tech Advance" option:
```c
game.fSlowTech : 1;  // If set, tech costs doubled
```

## Racial Tech Modifiers

Certain Primary Racial Traits (PRT) affect tech:

| PRT | Effect |
|-----|--------|
| Hyper Expansion (HE) | Reduced tech costs |
| Super Stealth (SS) | Electronics bonus |
| Warmonger (WM) | Weapons bonus |
| Interstellar Traveler (IT) | Propulsion bonus |
| Space Demolition (SD) | Mining/bomb bonus |
| Packet Physics (PP) | Construction bonus |
| Inner Strength (IS) | Biotechnology bonus |
| Claim Adjuster (CA) | Terraforming bonus |
| Jack of All Trades (JOAT) | Balanced start |
| Alternate Reality (AR) | Unique tech path |

## Lesser Racial Traits (LRT)

LRTs that affect research:

| LRT | Effect |
|-----|--------|
| Improved Fuel Efficiency (IFE) | Propulsion start bonus |
| Total Terraforming (TT) | Biotech requirement |
| Advanced Remote Mining (ARM) | Mining component access |
| Improved Starbases (ISB) | Construction bonus |
| Generalized Research (GR) | Spillover tech gains |
| Ultimate Recycling (UR) | Tech from scrapping |
| No Ram Scoop Engines (NRSE) | No ramscoop access |
| Cheap Engines (CE) | Engine cost reduction |
| Only Basic Remote Mining (OBRM) | Limited mining tech |
| No Advanced Scanners (NAS) | Limited scanner tech |
| Low Starting Population (LSP) | - |
| Bleeding Edge Tech (BET) | New tech starts at 0% |
| Regenerating Shields (RS) | Shield regeneration |

## Technology Unlocks by Field

### Energy (Ene)

| Level | Unlocks |
|-------|---------|
| 0 | Mole-skin Shield, Laser |
| 3 | Cow-hide Shield |
| 5 | X-Ray Laser |
| 6 | Wolverine Diffuse Shield |
| 7 | Shadow Shield, Croby Sharmor |
| 10 | Bear Neutrino Barrier |
| 12 | Langston Shell |
| 14 | Gorilla Delagator |
| 18 | Elephant Hide Fortress |
| 22 | Complete Phase Shield |

### Weapons (Wep)

| Level | Unlocks |
|-------|---------|
| 2 | Lady Finger Bomb |
| 5 | Black Cat Bomb, Smart Bomb |
| 8 | M-70 Bomb |
| 10 | Neutron Bomb |
| 11 | M-80 Bomb |
| 12 | All torpedoes unlocked progressively |
| 14 | Cherry Bomb |
| 15 | Enriched Neutron Bomb |
| 22 | Peerless Bomb |
| 24 | Annihilator Bomb |

### Propulsion (Prop)

| Level | Unlocks |
|-------|---------|
| 0 | Quick Jump 5 |
| 2 | Fuel Mizer |
| 3 | Long Hump 6 |
| 5 | Daddy Long Legs 7 |
| 6 | Radiating Hydro-Ram Scoop |
| 7 | Alpha Drive 8 |
| 8 | Sub-Galactic Fuel Scoop |
| 9 | Trans-Galactic Drive, TG Fuel Scoop |
| 11 | Interspace-10 |
| 12 | Trans-Galactic Super Scoop |
| 13 | Enigma Pulsar |
| 16 | Trans-Galactic Mizer Scoop |
| 20 | Galaxy Scoop |
| 23 | Trans-Star 10 |

### Construction (Con)

| Level | Unlocks |
|-------|---------|
| 0 | Tritanium Armor |
| 2 | Robo-Mini-Miner |
| 3 | Crobmnium Armor |
| 4 | Robo-Miner |
| 6 | Strobnium Armor |
| 7 | Robo-Maxi-Miner |
| 9 | Kelarium Armor |
| 10 | Fielded Kelarium, Depleted Neutronium |
| 12 | Neutronium Armor, Robo-Super-Miner |
| 14 | Mega Poly Shell |
| 15 | Robo-Ultra-Miner |
| 16 | Valanium Armor |
| 24 | Superlatanium Armor |

### Electronics (Ele)

| Level | Unlocks |
|-------|---------|
| 0 | Bat Scanner |
| 1 | Rhino Scanner |
| 3 | Depleted Neutronium |
| 4 | Mole Scanner |
| 5 | Possum Scanner |
| 6 | Ferret Scanner |
| 7 | Chameleon Scanner |
| 8 | Gazelle Scanner |
| 10 | Dolphin Scanner |
| 11 | Cheetah Scanner |
| 14 | Eagle Eye Scanner |
| 16 | Elephant Scanner |
| 24 | Peerless Scanner |

### Biotechnology (Bio)

| Level | Unlocks |
|-------|---------|
| 0 | - |
| 3 | Total Terraform 5 |
| 4 | Carbonic Armor |
| 6 | Total Terraform 7, Orbital Adjuster |
| 7 | Organic Armor |
| 9 | Total Terraform 10 |
| 10 | RNA Scanner |
| 12 | Hush-a-Boom |
| 13 | Total Terraform 15 |
| 17 | Total Terraform 20 |
| 22 | Total Terraform 25 |
| 25 | Total Terraform 30 |

## Special Components

### Genesis Device

The ultimate planetary weapon requires extreme multi-disciplinary research:

```c
.rgTech = {20, 10, 10, 20, 10, 20}  // Energy 20, Weapons 10, Prop 10, Con 20, Ele 10, Bio 20
```

Only available in games with random events enabled.

### Mystery Trader Exclusive

Some components only appear via Mystery Trader events and cannot be researched:
- Multi Function Pod
- Jump Gate (also researchable at very high tech)
- Alien Miner

## Future Tech

After reaching level 26 in any field, players can continue researching for incremental bonuses. The game tracks this separately:

```c
int16_t cFutureTech;      // Count of future tech levels
HS rghsFutureTech[8];     // Future tech slot storage
```

Future tech provides:
- 10% bonus to all components in that field per level
- Exponentially increasing costs
