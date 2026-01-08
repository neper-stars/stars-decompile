/* test_memory.c
 *
 * Unit tests for Stars! memory HB heap blocks.
 *
 * These tests assume you’re using the cross-platform HbHandleRec approach (no Win16 HGLOBAL),
 * and that the following are linked from your main code:
 *   - mphtcbAlloc[12]
 *   - rglphb[12]
 *   - LphbAlloc, LphbReAlloc, ResetHb, FreeHb
 *
 */

#include "acutest.h"

/* Pull in HB + HeapType. Prefer your project headers. */
#include "types.h"

/* Prefer your real headers; adjust if you named them differently. */
#include "globals.h"   /* mphtcbAlloc, rglphb */
#include "../memory.h" /* LphbAlloc/LphbReAlloc/ResetHb/FreeHb */

static void clear_heap_lists(void)
{
    for (int i = 0; i < (int)htCount; i++)
    {
        rglphb[i] = NULL;
    }
}

static void set_min_alloc_defaults(void)
{
    /* Make tests deterministic: give each heap type a known min size. */
    for (int i = 0; i < (int)htCount; i++)
    {
        mphtcbAlloc[i] = 0x0100; /* 256 bytes min by default for tests */
    }
}

/* ---------- Tests ---------- */

static void test_LphbAlloc_respects_min_and_inits_header(void)
{
    clear_heap_lists();
    set_min_alloc_defaults();

    mphtcbAlloc[htMsg] = 0x1000;  /* 4096 min alloc */
    HB *hb = LphbAlloc(1, htMsg); /* want = 1 + 0x10 => 0x11 < 0x1000 so clamp */

    TEST_CHECK(hb != NULL);
    TEST_CHECK(hb->ht == (uint8_t)htMsg);
    TEST_CHECK(hb->ibTop == (uint16_t)sizeof(HB));
    TEST_CHECK(hb->cbBlock == 0x1000);
    TEST_CHECK(hb->cbSlop == (uint16_t)(hb->cbBlock - (uint16_t)sizeof(HB)));
    TEST_CHECK(hb->cbFree == (uint16_t)(hb->cbBlock - (uint16_t)sizeof(HB)));

    /* inserted at head */
    TEST_CHECK(rglphb[htMsg] == hb);

    /* cleanup (avoid leaving dangling rglphb[htMsg]) */
    rglphb[htMsg] = NULL;
    FreeHb(hb);
}

static void test_LphbAlloc_no_min_clamp_when_large_enough(void)
{
    clear_heap_lists();
    set_min_alloc_defaults();

    mphtcbAlloc[htOrd] = 0x0020;       /* tiny min */
    HB *hb = LphbAlloc(0x0100, htOrd); /* want = 0x110 */

    TEST_CHECK(hb != NULL);
    TEST_CHECK(hb->cbBlock == (uint16_t)(0x0100 + (uint16_t)sizeof(HB)));
    TEST_CHECK(hb->ibTop == (uint16_t)sizeof(HB));
    TEST_CHECK(hb->cbFree == (uint16_t)(hb->cbBlock - (uint16_t)sizeof(HB)));
    TEST_CHECK(hb->cbSlop == (uint16_t)(hb->cbBlock - (uint16_t)sizeof(HB)));
    TEST_CHECK(rglphb[htOrd] == hb);

    rglphb[htOrd] = NULL;
    FreeHb(hb);
}

static void test_LphbReAlloc_grows_updates_list_and_zeroes_tail(void)
{
    clear_heap_lists();
    set_min_alloc_defaults();

    /* Growth step for this heap type */
    mphtcbAlloc[htLog] = 0x0080; /* 128 bytes grow */

    HB *hb = LphbAlloc(0x0020, htLog);
    TEST_CHECK(hb != NULL);
    TEST_CHECK(rglphb[htLog] == hb);

    uint16_t oldBlock = hb->cbBlock;

    /* scribble a recognizable pattern into the old block tail to detect new zeroing */
    uint8_t *p = (uint8_t *)hb;
    for (uint16_t i = sizeof(HB); i < oldBlock; i++)
    {
        p[i] = 0xAA;
    }

    HB *hb2 = LphbReAlloc(hb);
    TEST_CHECK(hb2 != NULL);

    /* head pointer should be updated if it moved */
    TEST_CHECK(rglphb[htLog] == hb2);

    uint16_t newBlock = hb2->cbBlock;
    TEST_CHECK(newBlock >= (uint16_t)(oldBlock + mphtcbAlloc[htLog]) || newBlock == 0xFFDC);

    /* Header fields grew by cbGrow (unless clamped to 0). */
    TEST_CHECK(hb2->ibTop == (uint16_t)sizeof(HB));

    /* Verify GMEM_ZEROINIT-like behavior: new bytes appended are zero. */
    if (newBlock > oldBlock)
    {
        uint8_t *q = (uint8_t *)hb2;
        for (uint16_t i = oldBlock; i < newBlock; i++)
        {
            TEST_CHECK(q[i] == 0);
            if (q[i] != 0)
                break;
        }
    }

    rglphb[htLog] = NULL;
    FreeHb(hb2);
}

static void test_ResetHb_resets_all_blocks_in_heap_list(void)
{
    clear_heap_lists();
    set_min_alloc_defaults();

    mphtcbAlloc[htShips] = 0x0200;

    HB *hb1 = LphbAlloc(0x0010, htShips);
    HB *hb2 = LphbAlloc(0x0010, htShips);
    TEST_CHECK(hb1 != NULL && hb2 != NULL);

    /* List order: hb2 is head, hb1 is next */
    TEST_CHECK(rglphb[htShips] == hb2);
    TEST_CHECK(hb2->lphbNext == hb1);

    /* Mutate fields so ResetHb has something to fix. */
    hb2->ibTop = 0x1234;
    hb2->cbFree = 7;
    hb2->cbSlop = 9;

    hb1->ibTop = 0x2222;
    hb1->cbFree = 11;
    hb1->cbSlop = 13;

    ResetHb(htShips);

    /* Verify both blocks reset */
    for (HB *t = rglphb[htShips]; t != NULL; t = t->lphbNext)
    {
        TEST_CHECK(t->ibTop == (uint16_t)sizeof(HB));
        TEST_CHECK(t->cbFree == (uint16_t)(t->cbBlock - (uint16_t)sizeof(HB)));
        TEST_CHECK(t->cbSlop == (uint16_t)(t->cbBlock - (uint16_t)sizeof(HB)));
    }

    /* cleanup */
    rglphb[htShips] = NULL;
    FreeHb(hb2); /* frees chain hb2->hb1 */
}

static void test_FreeHb_null_ok_and_frees_chain(void)
{
    clear_heap_lists();
    set_min_alloc_defaults();

    FreeHb(NULL); /* should be a no-op */

    mphtcbAlloc[htMisc] = 0x0100;
    HB *hb1 = LphbAlloc(0x0010, htMisc);
    HB *hb2 = LphbAlloc(0x0010, htMisc);
    TEST_CHECK(hb1 != NULL && hb2 != NULL);

    /* prevent dangling global head after free */
    rglphb[htMisc] = NULL;
    FreeHb(hb2); /* frees hb2 then hb1 */

    /* nothing strong to assert here without instrumenting the allocator,
       but at least we exercised the chain free path without crashing. */
    TEST_CHECK(1);
}

/* ---------- Test list ---------- */

TEST_LIST = {
    {"memory/LphbAlloc respects min and inits header", test_LphbAlloc_respects_min_and_inits_header},
    {"memory/LphbAlloc uses want size when >= min", test_LphbAlloc_no_min_clamp_when_large_enough},
    {"memory/LphbReAlloc grows, updates list, zeros", test_LphbReAlloc_grows_updates_list_and_zeroes_tail},
    {"memory/ResetHb resets all blocks", test_ResetHb_resets_all_blocks_in_heap_list},
    {"memory/FreeHb NULL ok and frees chain", test_FreeHb_null_ok_and_frees_chain},
    {NULL, NULL}};
