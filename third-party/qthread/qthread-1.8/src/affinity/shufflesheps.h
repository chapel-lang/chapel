#ifndef QTHREAD_SRC_SHUFFLE_SHEPS_H
#define QTHREAD_SRC_SHUFFLE_SHEPS_H

#include <stdlib.h> // for rand()

static void shuffle_sheps(qthread_shepherd_id_t*s, size_t len)
{/*{{{*/
    for (size_t i = 0; i < len; ++i) {
        qthread_shepherd_id_t tmp;
        size_t j = rand() % len;
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }
}/*}}}*/

#endif
