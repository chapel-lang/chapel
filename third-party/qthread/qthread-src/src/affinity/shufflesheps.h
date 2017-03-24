#ifndef QTHREAD_SRC_SHUFFLE_SHEPS_H
#define QTHREAD_SRC_SHUFFLE_SHEPS_H

#include <stdlib.h> // for rand()

#include "qt_debug.h"

static void shuffle_sheps(qthread_shepherd_id_t *s,
                          size_t                 len)
{   /*{{{*/
    for (size_t i = 0; i < len; ++i) {
        qthread_shepherd_id_t tmp;
        size_t                j = rand() % len;
        tmp  = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }
} /*}}}*/

static void sort_sheps(unsigned int *restrict          dists,
                       qthread_shepherd_id_t *restrict s,
                       size_t                          num)
{   /*{{{*/
    assert(num > 1);
    size_t                 s_max    = num - 1;
    qthread_shepherd_id_t *tmp      = qt_calloc(sizeof(qthread_shepherd_id_t), (num - 1));
    size_t                 tmp_base = 0;
    size_t                 tmp_max  = 0;

    qthread_debug(AFFINITY_FUNCTIONS, "dists:%p, s:%p, num:%u .. s_max:%u\n", dists, s, (unsigned int)num, s_max);
    while (s_max > 0) {
        size_t       i, k;
        unsigned int mindist = UINT_MAX;
        for (i = 0; i < s_max; ++i) { // find the minimum distance of the remaining sheps
            if (mindist > dists[s[i]]) {
                mindist = dists[s[i]];
            }
        }
        for (i = 0; i < s_max; ++i) { // copy all minimum-distance sheps to tmp
            if (dists[s[i]] == mindist) {
                tmp[tmp_max++] = s[i];
            }
        }
        if (tmp_max - tmp_base > 1) {
            shuffle_sheps(tmp + tmp_base, tmp_max - tmp_base);
        }
        tmp_base = tmp_max;
        for (i = k = 0; i < s_max; ++i) { // compress s
            assert(dists[s[i]] >= mindist);
            if (dists[s[i]] > mindist) {
                s[k++] = s[i];
            }
        }
        s_max = k;
    }
#if 0
    printf("\torig:");
    for (size_t i = 0; i < num - 1; ++i) {
        printf(" %u[%u]", s[i], dists[s[i]]);
    }
    printf("\n\tshuf:");
    for (size_t i = 0; i < num - 1; ++i) {
        printf(" %u[%u]", tmp[i], dists[tmp[i]]);
    }
    printf("\n");
#endif
    memcpy(s, tmp, sizeof(qthread_shepherd_id_t) * (num - 1));
    qt_free(tmp);
} /*}}}*/
#endif // ifndef QTHREAD_SRC_SHUFFLE_SHEPS_H
/* vim:set expandtab: */
