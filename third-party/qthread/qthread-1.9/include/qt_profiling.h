#ifndef QT_PROFILING_H
#define QT_PROFILING_H

#ifdef QTHREAD_FEB_PROFILING
# include "qthread/qtimer.h"
# define QTHREAD_ACCUM_MAX(a, b) do { if ((a) < (b)) { a = b; } } while (0)
# define QTHREAD_WAIT_TIMER_DECLARATION qtimer_t wait_timer = qtimer_create();
# define QTHREAD_WAIT_TIMER_START() qtimer_start(wait_timer)
# define QTHREAD_WAIT_TIMER_STOP(ME, TYPE)                         \
    do { double secs;                                              \
         qtimer_stop(wait_timer);                                  \
         secs = qtimer_secs(wait_timer);                           \
         if ((ME)->rdata->shepherd_ptr->TYPE ## _maxtime < secs) { \
             (ME)->rdata->shepherd_ptr->TYPE ## _maxtime = secs; } \
         (ME)->rdata->shepherd_ptr->TYPE ## _time += secs;         \
         (ME)->rdata->shepherd_ptr->TYPE ## _count++;              \
         qtimer_destroy(wait_timer); } while(0)
# define QTHREAD_FEB_TIMER_DECLARATION(TYPE) qtimer_t TYPE ## _timer = qtimer_create();
# define QTHREAD_FEB_TIMER_START(TYPE)       qtimer_start(TYPE ## _timer)
# define QTHREAD_FEB_TIMER_STOP(TYPE, ME)                                        \
    do { double secs;                                                             \
         qtimer_stop(TYPE ## _timer);                                             \
         secs = qtimer_secs(TYPE ## _timer);                                      \
         if ((ME)->rdata->shepherd_ptr->TYPE ## _maxtime < secs) {                \
             (ME)->rdata->shepherd_ptr->TYPE ## _maxtime = secs; }                \
         (ME)->rdata->shepherd_ptr->TYPE ## _time += qtimer_secs(TYPE ## _timer); \
         (ME)->rdata->shepherd_ptr->TYPE ## _count++;                             \
         qtimer_destroy(TYPE ## _timer); } while(0)
# define QTHREAD_HOLD_TIMER_INIT(LOCKSTRUCT_P)  (LOCKSTRUCT_P)->hold_timer = qtimer_create()
# define QTHREAD_HOLD_TIMER_START(LOCKSTRUCT_P) qtimer_start((LOCKSTRUCT_P)->hold_timer)
# define QTHREAD_HOLD_TIMER_STOP(LOCKSTRUCT_P, SHEP)     \
    do { double secs;                                    \
         qtimer_stop((LOCKSTRUCT_P)->hold_timer);        \
         secs = qtimer_secs((LOCKSTRUCT_P)->hold_timer); \
         if ((SHEP)->hold_maxtime < secs) {              \
             (SHEP)->hold_maxtime = secs; }              \
         (SHEP)->hold_time += secs; } while(0)
# define QTHREAD_HOLD_TIMER_DESTROY(LOCKSTRUCT_P) qtimer_destroy((LOCKSTRUCT_P)->hold_timer)
# define QTHREAD_EMPTY_TIMER_INIT(LOCKSTRUCT_P)   (LOCKSTRUCT_P)->empty_timer = qtimer_create()
# define QTHREAD_EMPTY_TIMER_START(LOCKSTRUCT_P)  qtimer_start((LOCKSTRUCT_P)->empty_timer)
# define QTHREAD_EMPTY_TIMER_STOP(LOCKSTRUCT_P)           \
    do { qthread_shepherd_t *ret;                         \
         double              secs;                        \
         qtimer_stop((LOCKSTRUCT_P)->empty_timer);        \
         ret = qthread_internal_getshep();                \
         assert(ret != NULL);                             \
         secs = qtimer_secs((LOCKSTRUCT_P)->empty_timer); \
         if (ret->empty_maxtime < secs) {                 \
             ret->empty_maxtime = secs; }                 \
         ret->empty_time += secs;                         \
         ret->empty_count++; } while (0)
# define QTHREAD_FEB_UNIQUERECORD(TYPE, ADDR, ME)    qt_hash_put((ME)->rdata->shepherd_ptr->unique ## TYPE ## addrs, (void *)(ADDR), (void *)(ADDR))
# define QTHREAD_FEB_UNIQUERECORD2(TYPE, ADDR, SHEP) qt_hash_put((SHEP)->unique ## TYPE ## addrs, (void *)(ADDR), (void *)(ADDR))
static QINLINE void qthread_unique_collect(const qt_key_t key,
                                           void          *value,
                                           void          *id)
{   /*{{{*/
    qt_hash_put_locked((qt_hash)id, key, value);
} /*}}}*/

#else // ifdef QTHREAD_FEB_PROFILING
# define QTHREAD_WAIT_TIMER_DECLARATION
# define QTHREAD_WAIT_TIMER_START()        do { } while(0)
# define QTHREAD_WAIT_TIMER_STOP(ME, TYPE) do { } while(0)
# define QTHREAD_FEB_TIMER_DECLARATION(TYPE)
# define QTHREAD_FEB_TIMER_START(TYPE)               do { } while(0)
# define QTHREAD_FEB_TIMER_STOP(TYPE, ME)            do { } while(0)
# define QTHREAD_HOLD_TIMER_INIT(LOCKSTRUCT_P)        do { } while(0)
# define QTHREAD_HOLD_TIMER_START(LOCKSTRUCT_P)       do { } while(0)
# define QTHREAD_HOLD_TIMER_STOP(LOCKSTRUCT_P, SHEP)  do { } while(0)
# define QTHREAD_HOLD_TIMER_DESTROY(LOCKSTRUCT_P)     do { } while(0)
# define QTHREAD_EMPTY_TIMER_INIT(LOCKSTRUCT_P)       do { } while(0)
# define QTHREAD_EMPTY_TIMER_START(LOCKSTRUCT_P)      do { } while(0)
# define QTHREAD_EMPTY_TIMER_STOP(LOCKSTRUCT_P)       do { } while(0)
# define QTHREAD_FEB_UNIQUERECORD(TYPE, ADDR, ME)    do { } while(0)
# define QTHREAD_FEB_UNIQUERECORD2(TYPE, ADDR, SHEP) do { } while(0)
#endif // ifdef QTHREAD_FEB_PROFILING

#ifdef QTHREAD_COUNT_THREADS
# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) qthread_internal_incr(&TYPE ## _stripes[(BIN)], &TYPE ## _stripes_locks[(BIN)], 1)
#else
# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) do { } while(0)
#endif

#endif // ifndef QT_PROFILING_H
/* vim:set expandtab: */
