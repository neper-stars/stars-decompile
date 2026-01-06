# Stars! Ship Components

## Overview

Ship components in Stars! are organized into categories, each with specific properties. All components have:
- **Tech Requirements**: 6 fields (Energy, Weapons, Propulsion, Construction, Electronics, Biotech)
- **Mass**: Weight in kT
- **Resource Cost**: Resources needed to build
- **Mineral Costs**: Ironium, Boranium, Germanium requirements

## Engines (16 types)

Engines determine warp speed capability and fuel consumption. Fuel usage is stored per-warp in `rgcFuelUsed[12]`.

| ID | Name | Mass | Tech (Prop) | Special |
|----|------|------|-------------|---------|
| 1 | Settler's Delight | 2 | 0 | Free warp 6, IFE only |
| 2 | Quick Jump 5 | 4 | 0 | Basic starter engine |
| 3 | Fuel Mizer | 6 | 2 | Efficient, free warp 5 |
| 4 | Long Hump 6 | 9 | 3 | Good mid-game engine |
| 5 | Daddy Long Legs 7 | 13 | 5 | Efficient at warp 7 |
| 6 | Alpha Drive 8 | 17 | 7 | Strong warp 8 |
| 7 | Trans-Galactic Drive | 25 | 9 | Best non-scoop engine |
| 8 | Interspace-10 | 25 | 11 | Warp 10 capable, overgateable |
| 9 | Enigma Pulsar | 20 | 13 | Battle speed 9, rare |
| 10 | Trans-Star 10 | 5 | 23 | Ultimate engine |
| 11 | Radiating Hydro-Ram Scoop | 10 | 6 | RADIATING - kills colonists! |
| 12 | Sub-Galactic Fuel Scoop | 20 | 8 | Free warp 6 |
| 13 | Trans-Galactic Fuel Scoop | 19 | 9 | Free warp 7 |
| 14 | Trans-Galactic Super Scoop | 18 | 12 | Free warp 8 |
| 15 | Trans-Galactic Mizer Scoop | 11 | 16 | Free warp 9 |
| 16 | Galaxy Scoop | 8 | 20 | Free warp 10! |

### Engine Abilities (grfAbilities bitmask)
- Bit 0: IFE bonus (Settler's Delight, Fuel Mizer)
- Bit 1: Ram scoop (free fuel above threshold)
- Bit 2: Radiating (damages colonists)
- Bit 3: Overgateable (can use stargates at warp 10)

## Armor (12 types)

Armor provides damage points (dp) of protection.

| ID | Name | Mass | dp | Tech (Con) |
|----|------|------|-----|------------|
| 1 | Tritanium | 60 | 50 | 0 |
| 2 | Crobmnium | 56 | 75 | 3 |
| 3 | Carbonic Armor | 25 | 100 | Bio 4 |
| 4 | Strobnium | 54 | 120 | 6 |
| 5 | Organic Armor | 15 | 175 | Bio 7 |
| 6 | Kelarium | 50 | 180 | 9 |
| 7 | Fielded Kelarium | 50 | 175 | Con 10, Ene 4 |
| 8 | Depleted Neutronium | 50 | 200 | Con 10, Ele 3 |
| 9 | Neutronium | 45 | 275 | 12 |
| 10 | Mega Poly Shell | 20 | 400 | Multi-tech |
| 11 | Valanium | 40 | 500 | 16 |
| 12 | Superlatanium | 30 | 1500 | 24 |

## Shields (10 types)

| ID | Name | Mass | dp | Tech (Ene) |
|----|------|------|-----|------------|
| 1 | Mole-skin Shield | 1 | 25 | 0 |
| 2 | Cow-hide Shield | 1 | 40 | 3 |
| 3 | Wolverine Diffuse Shield | 1 | 60 | 6 |
| 4 | Croby Sharmor | 10 | 60 | Ene 7, Con 4 |
| 5 | Shadow Shield | 2 | 75 | Ene 7, Ele 3 |
| 6 | Bear Neutrino Barrier | 1 | 100 | 10 |
| 7 | Langston Shell | 10 | 125 | Multi-tech |
| 8 | Gorilla Delagator | 1 | 175 | 14 |
| 9 | Elephant Hide Fortress | 1 | 300 | 18 |
| 10 | Complete Phase Shield | 1 | 500 | 22 |

## Scanners (16 types)

`dRange` is the normal scanning range in light-years.

| ID | Name | Range | Mass | Abilities |
|----|------|-------|------|-----------|
| 1 | Bat Scanner | 0 | 2 | Basic |
| 2 | Rhino Scanner | 50 | 5 | - |
| 3 | Mole Scanner | 100 | 2 | - |
| 4 | DNA Scanner | 125 | 2 | Bio tech |
| 5 | Possum Scanner | 150 | 3 | - |
| 6 | Pick Pocket Scanner | 80 | 15 | Can steal cargo |
| 7 | Chameleon Scanner | 160 | 6 | + cloaking |
| 8 | Ferret Scanner | 185 | 2 | Penetrating |
| 9 | Dolphin Scanner | 220 | 4 | Penetrating |
| 10 | Gazelle Scanner | 225 | 5 | - |
| 11 | RNA Scanner | 230 | 2 | Bio tech |
| 12 | Cheetah Scanner | 275 | 4 | - |
| 13 | Elephant Scanner | 300 | 6 | Penetrating |
| 14 | Eagle Eye Scanner | 335 | 3 | - |
| 15 | Robber Baron Scanner | 220 | 20 | Can steal cargo |
| 16 | Peerless Scanner | 500 | 4 | Best range |

### Scanner Abilities (grfAbilities)
- Bit 0: Penetrating scanner (sees through cloaking)
- Bit 1: Penetrating (alternate flag)
- Bit 2: Can steal cargo (Pick Pocket, Robber Baron)

## Beam Weapons

Beam weapons fire in combat, have a range, and deal damage.

| Name | Damage | Range | Init | Special |
|------|--------|-------|------|---------|
| Laser | 10 | 1 | 9 | - |
| X-Ray Laser | 16 | 1 | 9 | - |
| Mini Gun | 13 | 2 | 12 | Gatling |
| Yakimora Light Phaser | 26 | 1 | 9 | - |
| Blackjack | 90 | 0 | 10 | Point-blank |
| Phaser Bazooka | 26 | 2 | 7 | - |
| Pulsed Sapper | 82 | 3 | 14 | Sapper |
| Colloidal Phaser | 26 | 3 | 5 | - |
| Gatling Gun | 31 | 2 | 13 | Gatling |
| Mini Blaster | 66 | 1 | 9 | - |
| Bludgeon | 231 | 0 | 10 | Point-blank |
| Disruptor | 169 | 2 | 8 | - |
| Multi Function Phaser | 40 | 3 | 6 | Torpedo too |
| Mega Disruptor | 169 | 2 | 8 | - |
| Big Mutha Cannon | 400 | 0 | 7 | Massive |
| Streaming Pulverizer | 169 | 2 | 8 | - |
| Anti-Matter Pulverizer | 220 | 2 | 6 | - |

## Torpedoes

Torpedoes have accuracy ratings and initiative.

| Name | Damage | Range | Init | Accuracy |
|------|--------|-------|------|----------|
| Alpha Torpedo | 5 | 4 | 0 | 35% |
| Beta Torpedo | 12 | 4 | 1 | 45% |
| Delta Torpedo | 26 | 4 | 1 | 60% |
| Epsilon Torpedo | 48 | 5 | 2 | 65% |
| Rho Torpedo | 90 | 5 | 2 | 75% |
| Upsilon Torpedo | 169 | 5 | 3 | 75% |
| Omega Torpedo | 316 | 5 | 4 | 80% |
| Jihad Missile | 85 | 5 | 0 | 20% |
| Juggernaut Missile | 150 | 5 | 1 | 20% |
| Doomsday Missile | 280 | 6 | 2 | 25% |
| Armageddon Missile | 525 | 6 | 3 | 30% |

## Bombs

Bombs deal damage to colonists and installations.

| Name | Pop Kill | Bldg Kill | Tech |
|------|----------|-----------|------|
| Lady Finger Bomb | 6 | 2 | Wep 2 |
| Black Cat Bomb | 9 | 4 | Wep 5 |
| M-70 Bomb | 12 | 6 | Wep 8 |
| M-80 Bomb | 17 | 7 | Wep 11 |
| Cherry Bomb | 25 | 10 | Wep 14 |
| LBU-17 Bomb | 2 | 16 | Structure bomb |
| LBU-32 Bomb | 3 | 28 | Structure bomb |
| LBU-74 Bomb | 4 | 45 | Structure bomb |
| Hush-a-Boom | 30 | 2 | Stealthy |
| Retro Bomb | 0 | 0 | De-terraforms! |
| Smart Bomb | 13 | 0 | Pop only |
| Neutron Bomb | 22 | 0 | Pop only |
| Enriched Neutron Bomb | 35 | 0 | Pop only |
| Peerless Bomb | 50 | 0 | Pop only |
| Annihilator Bomb | 70 | 0 | Pop only |

## Special Equipment

### Cloaking Devices
- Transport Cloaking (300% cloak for transports only)
- Stealth Cloak (70%)
- Super-Stealth Cloak (140%)
- Ultra-Stealth Cloak (540%)

### Battle Computers
- Battle Computer (+20% accuracy)
- Battle Super Computer (+30%)
- Battle Nexus (+50%)

### Jammers (reduce enemy accuracy)
- Jammer 10/20/30/50

### Other
- Energy Capacitor (+10% beam damage)
- Flux Capacitor (+20% beam damage)
- Energy Dampener (reduces enemy capacitor bonus)
- Tachyon Detector (sees cloaked ships)
- Anti-matter Generator (generates 50 fuel/year)

## Mechanical Equipment

- Colonization Module (required for colonizing)
- Orbital Construction Module (build starbases anywhere)
- Cargo Pod (50 kT cargo)
- Super Cargo Pod (100 kT cargo)
- Multi Cargo Pod (250 kT cargo)
- Fuel Tank (250 mg fuel)
- Super Fuel Tank (500 mg fuel)
- Maneuvering Jet (+1 battle speed)
- Overthruster (+2 battle speed)
- Jump Gate (instant travel in battle)
- Beam Deflector (reduces beam damage)

## Mining Equipment

Remote miners extract minerals from uninhabited planets.

| Name | Rate | Mass | Tech |
|------|------|------|------|
| Robo-Midget Miner | 5 | 80 | 0 |
| Robo-Mini-Miner | 4 | 240 | Low |
| Robo-Miner | 12 | 240 | Con 4 |
| Robo-Maxi-Miner | 18 | 240 | Con 7 |
| Robo-Super-Miner | 27 | 240 | Con 12 |
| Robo-Ultra-Miner | 25 | 80 | Con 15 |
| Alien Miner | 10 | 20 | Multi |
| Orbital Adjuster | 0 | 80 | Bio 6 (terraforms!) |

## Mine Layers

Lay space mines to damage enemy ships.

| Name | Mines/Year | Type |
|------|------------|------|
| Mine Dispenser 40 | 40 | Normal |
| Mine Dispenser 50 | 50 | Normal |
| Mine Dispenser 80 | 80 | Normal |
| Mine Dispenser 130 | 130 | Normal |
| Heavy Dispenser 50 | 50 | Heavy |
| Heavy Dispenser 110 | 110 | Heavy |
| Heavy Dispenser 200 | 200 | Heavy |
| Speed Trap 20 | 20 | Speed |
| Speed Trap 30 | 30 | Speed |
| Speed Trap 50 | 50 | Speed |

### Mine Types
- **Normal**: 0.3% chance to hit per LY, 100 dp damage
- **Heavy**: 1.0% chance to hit per LY, 500 dp damage (affects fleets not individual ships)
- **Speed**: 3.5% chance to hit per warp squared, 128 dp damage (faster = more danger)
