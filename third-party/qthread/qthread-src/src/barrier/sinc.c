#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <stdio.h>

/* System Compatibility Header */
#include "qthread-int.h"

/* Installed Headers */
#include "qthread/qthread.h"
#include "qthread/barrier.h"
#include "qthread/sinc.h"

/* Internal Headers */
#include "qt_atomics.h"
#include "qt_mpool.h"
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qt_asserts.h"

/* The Datatype */
struct qt_barrier_s {
    uint64_t   count;
    qt_sinc_t *sinc_1;
    qt_sinc_t *sinc_2;
    qt_sinc_t *sinc_3;
};

static qt_barrier_t * global_barrier = NULL;

void INTERNAL qt_barrier_internal_init(void)
{ }

qt_barrier_t API_FUNC *qt_barrier_create(size_t           size,
                                         qt_barrier_btype Q_UNUSED(type))
{
    qt_barrier_t *barrier = MALLOC(sizeof(qt_barrier_t));

    barrier->count  = size;
    barrier->sinc_1 = qt_sinc_create(0, NULL, NULL, size);
    barrier->sinc_2 = qt_sinc_create(0, NULL, NULL, size);
    barrier->sinc_3 = qt_sinc_create(0, NULL, NULL, size);

    return barrier;
}

void API_FUNC qt_barrier_destroy(qt_barrier_t *restrict barrier)
{
    if (barrier->sinc_1) { qt_sinc_destroy(barrier->sinc_1); }
    barrier->sinc_1 = NULL;
    if (barrier->sinc_2) { qt_sinc_destroy(barrier->sinc_2); }
    barrier->sinc_2 = NULL;
    if (barrier->sinc_3) { qt_sinc_destroy(barrier->sinc_3); }
    barrier->sinc_3 = NULL;
}

void API_FUNC qt_barrier_resize(qt_barrier_t *restrict barrier,
                                size_t                 new_size)
{
    int diff = new_size - barrier->count;

    barrier->count = new_size;

    // modify each internal sinc to countdown form new limit
    qt_sinc_reset(barrier->sinc_1, diff);
    qt_sinc_reset(barrier->sinc_2, diff);
    qt_sinc_reset(barrier->sinc_3, diff);
}

void API_FUNC qt_barrier_enter_id(qt_barrier_t *barrier,
                                  size_t        id)
{
    qt_sinc_submit(barrier->sinc_1, NULL);
    qt_sinc_wait(barrier->sinc_1, NULL);
    if (id == 0) {
        qt_sinc_reset(barrier->sinc_3, barrier->count); // should be only 1 reset not all
    }
    qt_sinc_submit(barrier->sinc_2, NULL);
    qt_sinc_wait(barrier->sinc_2, NULL);
    if (id == 0) {
        qt_sinc_reset(barrier->sinc_1, barrier->count); // should be only 1 reset not all
    }
    qt_sinc_submit(barrier->sinc_3, NULL);
    qt_sinc_wait(barrier->sinc_3, NULL);
    if (id == 0) {
        qt_sinc_reset(barrier->sinc_2, barrier->count); // should be only 1 reset not all
    }
}

// feels like care with the reset should allow this to be done with only 2 not 3 sincs

void API_FUNC qt_barrier_enter(qt_barrier_t *barrier)
{
    qt_sinc_submit(barrier->sinc_1, NULL);
    qt_sinc_wait(barrier->sinc_1, NULL);
    qt_sinc_reset(barrier->sinc_3, barrier->count); // should be only 1 reset not all
    qt_sinc_submit(barrier->sinc_2, NULL);
    qt_sinc_wait(barrier->sinc_2, NULL);
    qt_sinc_reset(barrier->sinc_1, barrier->count); // should be only 1 reset not all
    qt_sinc_submit(barrier->sinc_3, NULL);
    qt_sinc_wait(barrier->sinc_3, NULL);
    qt_sinc_reset(barrier->sinc_2, barrier->count); // should be only 1 reset not all
}

void API_FUNC qt_barrier_dump(qt_barrier_t    *b,
                              qt_barrier_dtype dt)
{}



void qt_global_barrier(void)
{
    assert(global_barrier);
    qt_barrier_enter(global_barrier);
}

void qt_global_barrier_init(size_t size,
                                     int    debug)
{
    if (global_barrier == NULL) {
        global_barrier = qt_barrier_create(size, 0);
        assert(global_barrier);
    }
}

void qt_global_barrier_destroy(void)
{
    if (global_barrier) {
        qt_barrier_destroy(global_barrier);
        global_barrier = NULL;
    }
}

void qt_global_barrier_resize(size_t size)
{
    qt_barrier_resize(global_barrier, size);
}

/* vim:set expandtab: */
