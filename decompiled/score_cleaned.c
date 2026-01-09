/*
 * Player Score Calculation - Stars! 2.60j RC3
 * Cleaned up from decompiled code (score.c)
 *
 * This file contains the score calculation algorithm used by Stars!
 * The score is displayed in the player score window and determines rankings.
 *
 * SCORE STRUCTURE (20 bytes):
 *   Offset  Size  Field
 *   0-3     4     lScore      - Total score points
 *   4-5     2     cPlanet     - Number of planets owned
 *   6-7     2     cStarbase   - Number of starbases
 *   8-11    4     cResources  - Total resources available
 *   12-13   2     cTechLevels - Sum of all tech levels
 *   14-19   6     rgcsh[3]    - Ship counts by type (packed)
 *                              [0] = Unarmed ships
 *                              [1] = Escort ships (power < 2000)
 *                              [2] = Capital ships (power >= 2000)
 */

#include <stdint.h>
#include <string.h>

/* Ship type classification based on computed power */
typedef enum {
    SHIP_TYPE_DELETED = -1,  /* Design slot is empty/deleted */
    SHIP_TYPE_UNARMED = 0,   /* No weapons (power = 0) */
    SHIP_TYPE_ESCORT  = 1,   /* Armed but power < 2000 */
    SHIP_TYPE_CAPITAL = 2    /* Capital ship (power >= 2000) */
} ShipType;

/* Race Primary Racial Trait (PRT) indices */
#define PRT_HYPER_EXPANSION     0   /* HE - gets 50% max pop penalty */
#define PRT_ALTERNATE_REALITY   8   /* AR - uses energy tech for resources */
#define PRT_JOAT                9   /* JoaT - gets 20% max pop bonus */

/*
 * CalcPlayerScore - Calculate total score for a player
 *
 * @param iPlr    Player index (0-15)
 * @param pscore  Optional output: SCORE structure to fill
 * @return        Total score (low 16 bits)
 *
 * Score Formula Components:
 * 1. Planets: min(population/1000, 6) points per planet
 * 2. Resources: resources / 30 points
 * 3. Starbases: 3 points each
 * 4. Tech Levels (varies by level):
 *    - Levels 1-3:  +1 point per level
 *    - Levels 4-6:  +2*level - 3 points (5,7,9)
 *    - Levels 7-9:  +3*level - 9 points (12,15,18)
 *    - Levels 10+:  +4*level - 18 points (22,26,30,...)
 * 5. Ships:
 *    - Unarmed: count/2 points (capped at planet count)
 *    - Capital: bonus based on ratio to total armed ships
 *
 * Address: 1038:58a6
 */
int16_t CalcPlayerScore(int16_t iPlr, SCORE *pscore)
{
    SCORE score;
    int16_t shipType[16];      /* Classification for each ship design */
    int32_t shipCounts[3];     /* [0]=unarmed, [1]=escort, [2]=capital */
    int32_t totalScore;
    int16_t i;

    memset(&score, 0, sizeof(SCORE));

    /* ========================================
     * PHASE 1: Count planets, starbases, resources
     * ======================================== */

    PLANET *planet = lpPlanets;
    PLANET *planetEnd = lpPlanets + cPlanet;

    while (planet < planetEnd) {
        if (planet->iPlayer == iPlr) {
            score.cPlanet++;

            /* Population contribution: min(pop/1000, 6) points */
            int32_t popScore = planet->population / 1000;
            if (popScore > 6) {
                popScore = 6;
            }
            score.lScore += popScore;

            /* Check for starbase */
            if (planet->flags3 & FLAG_HAS_STARBASE) {
                HULDEF *hull = LphuldefFromId(
                    rgplrDesigns[iPlr][(planet->sbDesignId & 0x0F)].hullId
                );
                /* Only count if hull has dock capacity (is a real starbase) */
                if (hull->dockCapacity != 0) {
                    score.cStarbase++;
                }
            }

            /* Add resources from this planet */
            score.cResources += CResourcesAtPlanet(planet, iPlr);
        }
        planet++;
    }

    /* Resource contribution: resources / 30 */
    totalScore = score.lScore + (score.cResources / 30) + (score.cStarbase * 3);
    score.lScore = totalScore;

    /* ========================================
     * PHASE 2: Tech level contribution
     * ======================================== */

    /* Skip tech scoring if player is dead (flag bit 0 set) */
    if ((rgplr[iPlr].wFlags & 0x0001) == 0) {
        for (i = 0; i < 6; i++) {
            int16_t techLevel = rgplr[iPlr].rgTech[i];
            score.cTechLevels += techLevel;

            /* Tech level score contribution (per field):
             * Level 1-3:  +level points (1,2,3)
             * Level 4-6:  +2*level-3 points (5,7,9)
             * Level 7-9:  +3*level-9 points (12,15,18)
             * Level 10+:  +4*level-18 points (22,26,30,34,...)
             */
            int16_t techScore;
            if (techLevel < 4) {
                techScore = techLevel;
            } else if (techLevel < 7) {
                techScore = techLevel * 2 - 3;
            } else if (techLevel < 10) {
                techScore = techLevel * 3 - 9;
            } else {
                techScore = techLevel * 4 - 18;
            }
            totalScore += techScore;
        }
        score.lScore = totalScore;
    }

    /* ========================================
     * PHASE 3: Classify ship designs by power
     * ======================================== */

    for (i = 0; i < 16; i++) {
        SHDEF *design = &rgplrDesigns[iPlr][i];

        /* Check if design slot is deleted/empty */
        if (design->flags & FLAG_DESIGN_DELETED) {
            shipType[i] = SHIP_TYPE_DELETED;
            continue;
        }

        /* Compute combat power of this design */
        int32_t power = LComputePower(design);

        if (power == 0) {
            shipType[i] = SHIP_TYPE_UNARMED;
        } else if (power < 2000) {
            shipType[i] = SHIP_TYPE_ESCORT;
        } else {
            shipType[i] = SHIP_TYPE_CAPITAL;
        }
    }

    /* ========================================
     * PHASE 4: Count ships by type across all fleets
     * ======================================== */

    shipCounts[0] = 0;  /* Unarmed */
    shipCounts[1] = 0;  /* Escort */
    shipCounts[2] = 0;  /* Capital */

    for (int16_t ifl = 0; ifl < cFleet; ifl++) {
        FLEET *fleet = rglpfl[ifl];
        if (fleet == NULL) break;

        /* Only count our fleets that aren't in transit */
        if (fleet->iPlayer != iPlr) continue;
        if (fleet->flags & FLAG_FLEET_IN_TRANSIT) continue;

        /* Count ships of each design type */
        for (i = 0; i < 16; i++) {
            if (fleet->rgcShips[i] > 0 && shipType[i] != SHIP_TYPE_DELETED) {
                shipCounts[shipType[i]] += fleet->rgcShips[i];
            }
        }
    }

    /* ========================================
     * PHASE 5: Calculate ship score contribution
     * ======================================== */

    /* Unarmed ships: count/2 points, but capped at planet count */
    int32_t unarmedCount = shipCounts[0];
    if (unarmedCount > score.cPlanet) {
        unarmedCount = score.cPlanet;
    }
    totalScore = score.lScore + (unarmedCount / 2);
    score.lScore = totalScore;

    /* Capital ships bonus: if we have capital ships, add bonus based on
     * proportion of capital ships to total armed ships */
    int32_t capitalCount = shipCounts[2];
    if (capitalCount > 0) {
        int32_t totalArmed = score.cPlanet + capitalCount;
        /* Bonus = (cPlanet * cPlanet * capitalCount) / totalArmed */
        int32_t bonus = ((int32_t)score.cPlanet * score.cPlanet * capitalCount) / totalArmed;
        totalScore += bonus;
        score.lScore = totalScore;
    }

    /* Pack ship counts into score structure */
    for (i = 0; i < 3; i++) {
        score.rgcsh[i] = WPackLong(shipCounts[i]);
    }

    /* Copy score to output if requested */
    if (pscore != NULL) {
        memcpy(pscore, &score, sizeof(SCORE));
    }

    return (int16_t)score.lScore;
}


/*
 * LComputePower - Calculate combat power rating for a ship design
 *
 * @param lpshdef  Pointer to ship design
 * @return         Power rating (0 = unarmed, <2000 = escort, >=2000 = capital)
 *
 * Power is computed from:
 * - Beam weapons: damage * count * (range+3) / 4, halved if gattling
 * - Torpedoes: damage * count * (range-2) / 2
 * - Bombs: (normalDamage + smartDamage) * count * 2
 * - Capacitors: multiply beam power by (100 + capacitorBonus)%
 * - Speed bonus: beamPower * (speed - 4) / 10
 *
 * Address: 1038:0b32
 */
int32_t LComputePower(SHDEF *lpshdef)
{
    int32_t beamPower = 0;
    int32_t torpPower = 0;
    int32_t bombPower = 0;
    int32_t capacitorMultiplier = 1000;  /* 100.0% in fixed point */

    /* Iterate through all hull slots */
    for (int16_t slot = 0; slot < lpshdef->cSlots; slot++) {
        HARDWARESPEC *hs = &lpshdef->rghs[slot];
        int16_t count = hs->count;

        if (count == 0) continue;

        PART part;
        if (!FLookupPart(hs, &part)) continue;

        int16_t category = hs->category;

        switch (category) {
            case PART_CATEGORY_BEAM:  /* 0x10 */
            {
                int16_t range = part.range;
                int16_t damage = part.damage;
                /* Power = damage * count * (range + 3) / 4 */
                int32_t power = (int32_t)damage * count * (range + 3) / 4;
                /* Gattling guns (sapper flag) do 1/3 power */
                if (part.flags & FLAG_PART_GATTLING) {
                    power /= 3;
                }
                beamPower += power;
                break;
            }

            case PART_CATEGORY_TORPEDO:  /* 0x20 */
            {
                int16_t range = part.range;
                int16_t damage = part.damage;
                /* Power = damage * count * (range - 2) / 2 */
                int32_t power = (int32_t)damage * count * (range - 2) / 2;
                torpPower += power;
                break;
            }

            case PART_CATEGORY_BOMB:  /* 0x40 */
            {
                int16_t normalDmg = part.damage;
                int16_t smartDmg = part.damage2;
                /* Power = (normalDmg + smartDmg) * count * 2 */
                bombPower += (normalDmg + smartDmg) * count * 2;
                break;
            }

            case PART_CATEGORY_ELECTRICAL:  /* 0x800 */
            {
                /* Check for capacitors (part IDs 0x0C and 0x0D) */
                int16_t partId = hs->partId & 0xFF;
                if (partId == 0x0C || partId == 0x0D) {
                    /* Each capacitor multiplies by (100 + bonus)% */
                    int16_t bonus = part.capacitorBonus;
                    for (int16_t j = 0; j < count; j++) {
                        capacitorMultiplier = capacitorMultiplier * (100 + bonus) / 100;
                    }
                }
                break;
            }
        }
    }

    /* Apply capacitor multiplier to beam weapons */
    if (capacitorMultiplier != 1000) {
        capacitorMultiplier /= 10;  /* Convert to percentage */
        if (capacitorMultiplier > 255) {
            capacitorMultiplier = 255;  /* Cap at 255% */
        }
        beamPower = beamPower * capacitorMultiplier / 100;
    }

    /* Speed bonus for beam weapons */
    int16_t speed = SpdOfShip(NULL, 0, NULL, 0, lpshdef);
    int32_t speedBonus = beamPower * (speed - 4) / 10;

    /* Total power = bombs + beams + speedBonus + torps */
    return bombPower + beamPower + speedBonus + torpPower;
}


/*
 * CResourcesAtPlanet - Calculate resources available at a planet
 *
 * @param lppl  Planet pointer
 * @param iplr  Player index
 * @return      Resources available this turn
 *
 * For normal races:
 *   resources = population/colonistsPerResource + factories*factoryOutput/10
 *
 * For Alternate Reality (AR) race:
 *   resources = sqrt(energyTech * population / colonistsPerResource)
 *
 * If population exceeds max, effective population is reduced.
 *
 * Address: 1048:788e
 */
int16_t CResourcesAtPlanet(PLANET *lppl, int16_t iplr)
{
    if (lppl->population == 0) {
        return 0;
    }

    int16_t colonistsPerResource = GetRaceStat(&rgplr[iplr], STAT_COLONISTS_PER_RESOURCE);
    int32_t population = lppl->population;

    /* If over max population, reduce effective population */
    int32_t maxPop = CalcPlanetMaxPop(lppl->id, iplr);
    if (population > maxPop) {
        /* Effective pop = maxPop + (overflow / 2), capped at some limit */
        int32_t overflow = population - maxPop;
        population = maxPop + (overflow / 2);
        /* Additional cap check omitted for clarity */
    }

    int16_t prt = GetRaceStat(&rgplr[iplr], STAT_PRT);
    int16_t resources;

    if (prt == PRT_ALTERNATE_REALITY) {
        /* AR race: resources based on energy tech and population */
        int16_t energyTech = rgplr[iplr].rgTech[TECH_ENERGY];
        if (energyTech < 1) energyTech = 1;

        /* resources = sqrt(energyTech * population / colonistsPerResource) */
        double temp = (double)energyTech * (double)population / (double)colonistsPerResource;
        resources = (int16_t)sqrt(temp);
    } else {
        /* Normal race: population contribution + factory contribution */
        int32_t popResources = population / colonistsPerResource;

        int16_t factories = CMaxOperableFactories(lppl, iplr, 0);
        int16_t factoryOutput = GetRaceStat(&rgplr[iplr], STAT_FACTORY_OUTPUT);
        int32_t factResources = ((int32_t)factories * factoryOutput + 9) / 10;

        resources = (int16_t)(popResources + factResources);
    }

    /* Minimum 1 resource if planet is populated */
    if (resources == 0) {
        resources = 1;
    }

    return resources;
}


/*
 * CalcPlanetMaxPop - Calculate maximum population for a planet
 *
 * @param idpl  Planet ID
 * @param iplr  Player index
 * @return      Maximum population in colonists
 *
 * For normal races:
 *   maxPop = habitability% * 100 (e.g., 100% hab = 10,000 colonists base)
 *   Modified by PRT: HE = 50% penalty, JoaT = 20% bonus
 *   Modified by LRT: OBRM = 10% bonus
 *
 * For Alternate Reality (AR) race:
 *   maxPop = orbital capacity based on starbase size
 *
 * Address: 1048:7096
 */
int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr)
{
    PLANET pl;
    FLookupPlanet(idpl, &pl);

    int16_t prt = GetRaceStat(&rgplr[iplr], STAT_PRT);
    int32_t maxPop;

    if (prt == PRT_ALTERNATE_REALITY) {
        /* AR race: population lives in orbital facilities */
        if (pl.iPlayer != iplr || !(pl.flags3 & FLAG_HAS_STARBASE)) {
            return 0;  /* Need our own starbase */
        }
        /* Look up orbital capacity from starbase hull */
        int16_t hullId = rgplrDesigns[iplr][pl.sbDesignId & 0x0F].hullId;
        maxPop = rgOrbitalCapacity[(hullId - 0x20) * 4];
    } else {
        /* Normal race: based on planet habitability */
        int16_t habitability = PctPlanetDesirability(&pl, iplr);

        if (habitability < 5) {
            maxPop = 500;  /* Minimum for marginal planets */
        } else {
            maxPop = (int32_t)habitability * 100;
        }

        /* Apply PRT modifiers */
        if (prt == PRT_HYPER_EXPANSION) {
            maxPop -= maxPop / 2;  /* HE: 50% penalty */
        } else if (prt == PRT_JOAT) {
            maxPop += maxPop / 5;  /* JoaT: 20% bonus */
        }
    }

    /* Apply OBRM (Only Basic Remote Mining) LRT bonus */
    if (GetRaceGrbit(&rgplr[iplr], GRBIT_OBRM)) {
        maxPop += maxPop / 10;  /* 10% bonus */
    }

    return maxPop;
}


/*
 * PctPlanetDesirability - Calculate habitability percentage for a planet
 *
 * @param lppl  Planet pointer
 * @param iPlr  Player index
 * @return      Habitability percentage (-45 to 100)
 *              Negative values indicate hostile environment
 *
 * Calculates how suitable a planet is based on gravity, temperature,
 * and radiation compared to the race's preferred and tolerable ranges.
 *
 * Address: 1048:6e1e
 */
int16_t PctPlanetDesirability(PLANET *lppl, int16_t iPlr)
{
    int32_t positiveScore = 0;
    int32_t negativeScore = 0;
    int32_t modifier = 10000;  /* 100.00% in fixed point */

    /* Check each environment factor: gravity, temperature, radiation */
    for (int16_t i = 0; i < 3; i++) {
        int16_t planetValue = lppl->rgEnv[i];
        int16_t idealValue = rgplr[iPlr].rgEnvIdeal[i];
        int16_t minTolerable = rgplr[iPlr].rgEnvMin[i];
        int16_t maxTolerable = rgplr[iPlr].rgEnvMax[i];

        /* Check for immunity (negative tolerance = immune to this factor) */
        if (maxTolerable < 0) {
            positiveScore += 10000;  /* Full score for immune factors */
            continue;
        }

        /* Check if outside tolerable range */
        if (planetValue < minTolerable || planetValue > maxTolerable) {
            /* Calculate how far outside range (capped at 15) */
            int16_t distance;
            if (planetValue < minTolerable) {
                distance = minTolerable - planetValue;
            } else {
                distance = planetValue - maxTolerable;
            }
            if (distance > 15) distance = 15;

            negativeScore += distance;
        } else {
            /* Inside tolerable range - calculate comfort score */
            int16_t distFromIdeal = abs(planetValue - idealValue);
            int16_t rangeHalf;
            int16_t penalty;

            if (planetValue < idealValue) {
                rangeHalf = idealValue - minTolerable;
                penalty = (idealValue - planetValue) * 2 - rangeHalf;
            } else {
                rangeHalf = maxTolerable - idealValue;
                penalty = (planetValue - idealValue) * 2 - rangeHalf;
            }

            int16_t pctFromIdeal = (distFromIdeal * 100) / rangeHalf;
            int32_t comfortScore = (int32_t)(100 - pctFromIdeal) * (100 - pctFromIdeal);
            positiveScore += comfortScore;

            /* Apply penalty modifier if outside sweet spot */
            if (penalty > 0) {
                modifier = modifier * (rangeHalf * 2 - penalty) / (rangeHalf * 2);
            }
        }
    }

    /* Calculate final habitability */
    if (negativeScore == 0) {
        /* Positive habitability: sqrt(average comfort) * modifier */
        double avgComfort = (double)positiveScore / 3.0;
        int32_t baseHab = (int32_t)sqrt(avgComfort);
        return (int16_t)(baseHab * modifier / 10000);
    } else {
        /* Negative habitability */
        return -(int16_t)negativeScore;
    }
}
