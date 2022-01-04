#ifndef QTHREAD_LOG_BARRIER_H
#define QTHREAD_LOG_BARRIER_H

#include "macros.h"
#include "qthread.h"

Q_STARTCXX
/************************************************************/
/* Typedefs                                                 */
/************************************************************/
typedef enum {
    REGION_BARRIER,
    LOOP_BARRIER
} qt_barrier_btype;

typedef enum {
    UPLOCK,
    DOWNLOCK,
    BOTHLOCKS
} qt_barrier_dtype;

typedef struct qt_barrier_s qt_barrier_t;

/************************************************************/
/* Functions                                                */
/************************************************************/
qt_barrier_t *qt_barrier_create(size_t           max_threads,
                                qt_barrier_btype type);
void qt_barrier_enter(qt_barrier_t *b);
void qt_barrier_enter_id(qt_barrier_t *b,
                         size_t        id);
void qt_barrier_destroy(qt_barrier_t *b);
void qt_barrier_resize(qt_barrier_t *b,
                       size_t        size);

/* debugging... */
void qt_barrier_dump(qt_barrier_t    *b,
                     qt_barrier_dtype dt);

/* functions definded in qthread.c to support nested parallelism and barriers */
qt_barrier_t *qt_get_barrier(void);
void          qt_set_barrier(qt_barrier_t *);

Q_ENDCXX

#endif // ifndef QTHREAD_LOG_BARRIER_H
/* vim:set expandtab: */
