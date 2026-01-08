#ifndef PACK1_H_
#define PACK1_H_

#include <stddef.h>
#include <stdint.h>

/* Force 1-byte packing (matches typical Win16 “pack 1” layouts) */
#if defined(_MSC_VER)
#define PACK_PUSH_1 __pragma(pack(push, 1))
#define PACK_POP __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
#define PACK_PUSH_1
#define PACK_POP
#else
#define PACK_PUSH_1
#define PACK_POP
#endif

#if __STDC_VERSION__ >= 201112L

#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

#else
/* Pre-C11 fallback */
#define STATIC_ASSERT_CONCAT_(a, b) a##b
#define STATIC_ASSERT_CONCAT(a, b) STATIC_ASSERT_CONCAT_(a, b)
#define STATIC_ASSERT(cond, msg) \
    typedef char STATIC_ASSERT_CONCAT(static_assertion_, __LINE__)[(cond) ? 1 : -1]
#endif

#define ASSERT_SIZE(T, sz) \
    STATIC_ASSERT(sizeof(T) == (sz), "sizeof_" #T "_mismatch")

#define ASSERT_OFFSETOF(T, field, off) \
    STATIC_ASSERT(offsetof(T, field) == (off), "offsetof_" #T "_" #field "_mismatch")

#endif
