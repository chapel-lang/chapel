#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <qthread/qtimer.h>
#include "qthread_asserts.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#include <qthread/qthread-int.h> /* for uint64_t */

struct qtimer_s {
    struct timespec start, stop;
};

void qtimer_start(qtimer_t q)
{
    assert(q);
    qassert(clock_gettime(CLOCK_MONOTONIC, &(q->start)), 0);
}

/* this function based on http://burtleburtle.net/bob/hash/evahash.html */
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
static uint64_t qt_hashword(uint64_t key)
{   /*{{{*/
    uint32_t a, b, c;

    const union {
        uint64_t key;
        uint8_t  b[sizeof(uint64_t)];
    } k = {
        key
    };

    a  = b = c = 0xdeadbeef + sizeof(uint64_t);
    c += 47;

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
    return ((uint64_t)c + ((uint64_t)b << 32));
} /*}}}*/

unsigned long qtimer_fastrand(void)
{
    struct timespec s;

    qassert(clock_gettime(CLOCK_MONOTONIC, &(s)), 0);
    return qt_hashword((uint64_t)(s.tv_nsec));
}

void qtimer_stop(qtimer_t q)
{
    assert(q);
    qassert(clock_gettime(CLOCK_MONOTONIC, &(q->stop)), 0);
}

double qtimer_secs(qtimer_t q)
{
    assert(q);
    return (q->stop.tv_sec + q->stop.tv_nsec * 1e-9) - (q->start.tv_sec + q->start.tv_nsec * 1e-9);
}

qtimer_t qtimer_create()
{
    qtimer_t ret = calloc(1, sizeof(struct qtimer_s));

    assert(ret);
    return ret;
}

void qtimer_destroy(qtimer_t q)
{
    assert(q);
    free(q);
}

/* vim:set expandtab: */
