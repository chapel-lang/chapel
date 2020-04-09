#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qtimer.h>
#include "qt_gcd.h"
#include "argparsing.h"

static inline size_t shift_gcd(
    size_t a,
    size_t b)
{
    size_t k = 0;

    if (a == 0) {return b; }
    if (b == 0) {return a; }
    while ((a & 1) == 0 && (b & 1) == 0) {      /* while they're both even */
        a >>= 1;                       /* divide by 2 */
        b >>= 1;                       /* divide by 2 */
        k++;                           /* power of 2 to the result */
    }
    /* now, one or the other is odd */
    do {
        if ((a & 1) == 0) {
            a >>= 1;
        } else if ((b & 1) == 0) {
            b >>= 1;
        } else if (a >= b) {           /* both odd */
            a = (a - b) >> 1;
        } else {
            b = (b - a) >> 1;
        }
    } while (a > 0);
    return b << k;
}

static inline size_t mod_gcd(
    size_t a,
    size_t b)
{
    while (1) {
        if (a == 0) {return b; }
        b %= a;
        if (b == 0) {return a; }
        a %= b;
    }
}

size_t BIGNUM = 1000000;

int main(
    int argc,
    char *argv[])
{
    struct pair {
        size_t a, b;
    };
    struct pair *bigset = malloc(sizeof(struct pair) * BIGNUM);
    size_t *answer1 = malloc(sizeof(size_t) * BIGNUM);
    size_t *answer2 = malloc(sizeof(size_t) * BIGNUM);
    size_t i;
    qtimer_t mod_timer = qtimer_create();
    qtimer_t shift_timer = qtimer_create();
    CHECK_VERBOSE();
    NUMARG(BIGNUM, "BIGNUM");
    for (i = 0; i < BIGNUM; i++) {
        bigset[i].a = random();
        bigset[i].b = 4096;
    }
    for (i = 0; i < BIGNUM; i++) {
        answer1[i] = qt_gcd(bigset[i].a, bigset[i].b);
    }
    for (i = 0; i < BIGNUM; i++) {
        answer2[i] = mod_gcd(bigset[i].a, bigset[i].b);
    }
    qtimer_start(mod_timer);
    for (i = 0; i < BIGNUM; i++) {
        answer2[i] = mod_gcd(bigset[i].a, bigset[i].b);
    }
    qtimer_stop(mod_timer);
    for (i = 0; i < BIGNUM; i++) {
        if (answer1[i] != answer2[i]) {
            fprintf(stderr, "ERROR! %lu\n", (unsigned long)i);
        }
    }
    for (i = 0; i < BIGNUM; i++) {
        answer2[i] = shift_gcd(bigset[i].a, bigset[i].b);
    }
    qtimer_start(shift_timer);
    for (i = 0; i < BIGNUM; i++) {
        answer2[i] = shift_gcd(bigset[i].a, bigset[i].b);
    }
    qtimer_stop(shift_timer);
    for (i = 0; i < BIGNUM; i++) {
        if (answer1[i] != answer2[i]) {
            fprintf(stderr, "ERROR! %lu\n", (unsigned long)i);
        }
    }
    printf("  mod gcd secs: %f\n", qtimer_secs(mod_timer));
    printf("shift gcd secs: %f\n", qtimer_secs(shift_timer));
#ifdef QTHREAD_SHIFT_GCD
    assert(qtimer_secs(shift_timer) < qtimer_secs(mod_timer));
#else
    assert(qtimer_secs(shift_timer) > qtimer_secs(mod_timer));
#endif
    qtimer_destroy(mod_timer);
    qtimer_destroy(shift_timer);
    free(bigset);
    free(answer1);
    free(answer2);
}

/* vim:set expandtab */