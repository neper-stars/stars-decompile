#include "acutest.h"

#include <stdint.h>
#include <string.h>

#include "globals.h"
#include "types.h"
#include "planet.h" /* PctPlanetDesirability */

typedef struct HabCase
{
    const char *name;

    /* player env model */
    int8_t pref[3]; /* optimal (typically 50) */
    int8_t minv[3]; /* typically 15 */
    int8_t maxv[3]; /* typically 85; <0 means immune axis */

    /* planet env (Stars: 1..100) */
    int8_t env[3];

    /* expected result:
       - if expect_exact: want is exact
       - else: want is ignored and we check bounds/relations */
    int16_t want;
    int8_t expect_exact;

    /* optional: bounds check for non-exact cases */
    int16_t min_want;
    int16_t max_want;
} HabCase;

static void apply_player0(const HabCase *tc, PLAYER *old_out)
{
    *old_out = rgplr[0];

    PLAYER pr = rgplr[0];
    for (int i = 0; i < 3; i++)
    {
        pr.rgEnvVar[i] = tc->pref[i];
        pr.rgEnvVarMin[i] = tc->minv[i];
        pr.rgEnvVarMax[i] = tc->maxv[i];
    }
    rgplr[0] = pr;
}

static PLANET make_planet(const HabCase *tc)
{
    PLANET pl;
    memset(&pl, 0, sizeof(pl));
    for (int i = 0; i < 3; i++)
    {
        pl.rgEnvVar[i] = tc->env[i];
    }
    return pl;
}

static void test_PctPlanetDesirability_table_stars_defaults(void)
{
    /* Stars default race window: 15..85, pref=50. */
    static const HabCase cases[] = {
        {.name = "ideal (50,50,50) in 15..85 => 100",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {85, 85, 85},
         .env = {50, 50, 50},
         .want = 100,
         .expect_exact = 1},
        {.name = "just inside min edge (15,50,50) => positive",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {85, 85, 85},
         .env = {15, 50, 50},
         .expect_exact = 0,
         .min_want = 1,
         .max_want = 99},
        {.name = "below min (1,50,50) => negative penalty (15-1=14)",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {85, 85, 85},
         .env = {1, 50, 50},
         .want = -14,
         .expect_exact = 1},
        {.name = "above max (100,50,50) => negative penalty (100-85=15, capped 15)",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {85, 85, 85},
         .env = {100, 50, 50},
         .want = -15,
         .expect_exact = 1},
        {.name = "two axes out of range sum penalties (1,100,50) => -(14 + 15) = -29",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {85, 85, 85},
         .env = {1, 100, 50},
         .want = -29,
         .expect_exact = 1},
        {.name = "immune axis: max<0 => treated as perfect contribution; other axes ideal => 100",
         .pref = {50, 50, 50},
         .minv = {15, 15, 15},
         .maxv = {-1, 85, 85}, /* axis0 immune */
         .env = {1, 50, 50},   /* would be out-of-range, but immune */
         .want = 100,
         .expect_exact = 1},
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        const HabCase *tc = &cases[i];

        /* sanity: keep tests Stars-faithful unless intentionally testing bad data */
        for (int k = 0; k < 3; k++)
        {
            TEST_CHECK_(tc->env[k] >= 1 && tc->env[k] <= 100,
                        "case[%zu] %s: env[%d]=%d must be 1..100 for Stars data",
                        i, tc->name, k, (int)tc->env[k]);
        }

        PLAYER old;
        apply_player0(tc, &old);

        PLANET pl = make_planet(tc);
        int16_t got = PctPlanetDesirability(&pl, 0);

        if (tc->expect_exact)
        {
            TEST_CHECK_(got == tc->want,
                        "case[%zu] %s: got=%d want=%d env={%d,%d,%d}",
                        i, tc->name, (int)got, (int)tc->want,
                        (int)tc->env[0], (int)tc->env[1], (int)tc->env[2]);
        }
        else
        {
            TEST_CHECK_(got >= tc->min_want && got <= tc->max_want,
                        "case[%zu] %s: got=%d expected in [%d..%d] env={%d,%d,%d}",
                        i, tc->name, (int)got, (int)tc->min_want, (int)tc->max_want,
                        (int)tc->env[0], (int)tc->env[1], (int)tc->env[2]);
        }

        rgplr[0] = old;
    }
}

TEST_LIST = {
    {"PctPlanetDesirability table (Stars defaults)", test_PctPlanetDesirability_table_stars_defaults},
    {NULL, NULL}};
