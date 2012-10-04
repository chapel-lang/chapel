#ifndef QT_BARRIER_H
#define QT_BARRIER_H

#include "qthread/qthread.h" /* for qthread_shepherd_id_t */

/* these two calls assume that we're using a/the global barrier */
void qt_global_barrier(void);
void qt_global_barrier_init(int size, int debug);
void qt_global_barrier_destroy(void);
#define qt_barrier(x) qt_global_barrier()

typedef enum barrierType
{
    REGION_BARRIER,
    LOOP_BARRIER
} qt_barrier_btype;

typedef enum dumpType
{
    UPLOCK,
    DOWNLOCK,
    BOTHLOCKS
} qt_barrier_dtype;

typedef struct qt_barrier_s qt_barrier_t;
/*
typedef unsigned int qthread_shepherd_id_t; // potentially included in qthread.c -- need this from
                                            // qthread.h but too early to include whole file
*/
qt_barrier_t *qt_barrier_create(int size, enum barrierType type, int debug);
void qt_barrier_enter(qt_barrier_t *b, qthread_shepherd_id_t shep);
void qt_barrier_dump(qt_barrier_t *b, enum dumpType dt);
void qt_barrier_resize(size_t size);
void qt_barrier_destroy(qt_barrier_t *b);

#endif
