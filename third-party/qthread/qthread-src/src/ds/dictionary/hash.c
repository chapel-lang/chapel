#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* Qthreads Headers */
#include <qthread/qthread.h>
#include <qthread/hash.h>

/* Internal Headers */
#include "qt_visibility.h"

/* these functions are based on http://burtleburtle.net/bob/hash/evahash.html */
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
uint64_t API_FUNC qt_hash64(uint64_t key)
{   /*{{{*/
    const union {
        uint64_t key;
        uint8_t  b[sizeof(uint64_t)];
    } k = {
        key
    };

#if (SIZEOF_VOIDP == 8) /* i.e. a 64-bit machine */
    uint64_t a, b, c;

    a = b = 0x9e3779b97f4a7c13LL;   // the golden ratio
    c = 0xdeadbeefcafebabeULL + sizeof(uint64_t);

    a += ((uint64_t)k.b[7]) << 56;
    a += ((uint64_t)k.b[6]) << 48;
    a += ((uint64_t)k.b[5]) << 40;
    a += ((uint64_t)k.b[4]) << 32;
    a += k.b[3] << 24;
    a += k.b[2] << 16;
    a += k.b[1] << 8;
    a += k.b[0];

    a = a - b;  a = a - c;  a = a ^ (c >> 43);
    b = b - c;  b = b - a;  b = b ^ (a << 9);
    c = c - a;  c = c - b;  c = c ^ (b >> 8);
    a = a - b;  a = a - c;  a = a ^ (c >> 38);
    b = b - c;  b = b - a;  b = b ^ (a << 23);
    c = c - a;  c = c - b;  c = c ^ (b >> 5);
    a = a - b;  a = a - c;  a = a ^ (c >> 35);
    b = b - c;  b = b - a;  b = b ^ (a << 49);
    c = c - a;  c = c - b;  c = c ^ (b >> 11);
    a = a - b;  a = a - c;  a = a ^ (c >> 12);
    b = b - c;  b = b - a;  b = b ^ (a << 18);
    c = c - a;  c = c - b;  c = c ^ (b >> 22);
    return c;

#else /* i.e. a 32-bit machine */
    uint32_t a, b, c;

    a = b = 0x9e3779b9;   // golden ratio
    c = 0xdeadbeef + sizeof(uint64_t);

    b += k.b[7] << 24;
    b += k.b[6] << 16;
    b += k.b[5] << 8;
    b += k.b[4];
    a += k.b[3] << 24;
    a += k.b[2] << 16;
    a += k.b[1] << 8;
    a += k.b[0];

    c ^= b;
    c -= rot(b, 14);
    a ^= c;
    a -= rot(c, 11);
    b ^= a;
    b -= rot(a, 25);
    c ^= b;
    c -= rot(b, 16);
    a ^= c;
    a -= rot(c, 4);
    b ^= a;
    b -= rot(a, 14);
    c ^= b;
    c -= rot(b, 24);
    return ((uint64_t)c + (((uint64_t)b) << 32));
#endif /* if (SIZEOF_VOIDP == 8) */
} /*}}}*/

#if (QTHREAD_SIZEOF_ALIGNED_T == 4)
# define mix(a, b, c) do {                \
        a -= b;  a -= c;  a ^= (c >> 13); \
        b -= c;  b -= a;  b ^= (a << 8);  \
        c -= a;  c -= b;  c ^= (b >> 13); \
        a -= b;  a -= c;  a ^= (c >> 12); \
        b -= c;  b -= a;  b ^= (a << 16); \
        c -= a;  c -= b;  c ^= (b >> 5);  \
        a -= b;  a -= c;  a ^= (c >> 3);  \
        b -= c;  b -= a;  b ^= (a << 10); \
        c -= a;  c -= b;  c ^= (b >> 15); \
} while(0)
#else /* QTHREAD_SIZEOF_ALIGNED_T == 8 */
# define mix(a, b, c) do {              \
        a -= b; a -= c; a ^= (c >> 43); \
        b -= c; b -= a; b ^= (a << 9);  \
        c -= a; c -= b; c ^= (b >> 8);  \
        a -= b; a -= c; a ^= (c >> 38); \
        b -= c; b -= a; b ^= (a << 23); \
        c -= a; c -= b; c ^= (b >> 5);  \
        a -= b; a -= c; a ^= (c >> 35); \
        b -= c; b -= a; b ^= (a << 49); \
        c -= a; c -= b; c ^= (b >> 11); \
        a -= b; a -= c; a ^= (c >> 12); \
        b -= c; b -= a; b ^= (a << 18); \
        c -= a; c -= b; c ^= (b >> 22); \
} while(0)
#endif /* define mix() */

aligned_t API_FUNC qt_hash_bytes(void     *key_ptr,
                                 size_t    bytes,
                                 aligned_t state)
{
    aligned_t a, b, c;   /* internal state */
    size_t             len = bytes;
    const uint8_t     *k   = key_ptr;

    /* set up the internal state */
    a = b = GOLDEN_RATIO;
    c = state;

    /* handle most of the key */
    while (len >= 12) {
        a += k[0] + ((aligned_t)k[1] << 8) + ((aligned_t)k[2] << 16) + ((aligned_t)k[3] << 24);
        b += k[4] + ((aligned_t)k[5] << 8) + ((aligned_t)k[6] << 16) + ((aligned_t)k[7] << 24);
        c += k[8] + ((aligned_t)k[9] << 8) + ((aligned_t)k[10] << 16) + ((aligned_t)k[11] << 24);
        mix(a, b, c);
        k   += 12;
        len -= 12;
    }

    /* handle the last 11 bytes */
    c += bytes;
    switch(len) {
        case 11: c += ((aligned_t)k[10] << 24);
        case 10: c += ((aligned_t)k[9] << 16);
        case 9:  c += ((aligned_t)k[8] << 16);
        /* the first byte of c is reserved for the length */
        case 8: b += ((aligned_t)k[7] << 24);
        case 7: b += ((aligned_t)k[6] << 16);
        case 6: b += ((aligned_t)k[5] << 8);
        case 5: b += k[4];
        case 4: a += ((aligned_t)k[3] << 24);
        case 3: a += ((aligned_t)k[2] << 16);
        case 2: a += ((aligned_t)k[1] << 8);
        case 1: a += k[0];
            /* case 0: nothing left to add */
    }
    mix(a, b, c);
    return c;
}

/* vim:set expandtab: */
