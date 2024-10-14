#ifndef QT_IO_H
#define QT_IO_H

/* System Headers */
#include "stdlib.h" /* malloc() and free() */

/* Public Headers */
#include "qthread/io.h"

/* Internal Headers */
#include "qt_blocking_structs.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"

#define ALLOC_SYSCALLJOB() qt_mpool_alloc(syscall_job_pool);
#define FREE_SYSCALLJOB(j) qt_mpool_free(syscall_job_pool, j);

extern qt_mpool syscall_job_pool;

void qt_blocking_subsystem_init(void);
int qt_process_blocking_call(void);
void qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job);

static inline int qt_blockable(void) {
  qthread_t *t = qthread_internal_self();

  if ((t != NULL) &&
      atomic_load_explicit(&t->flags, memory_order_relaxed) & QTHREAD_SIMPLE) {
    t = NULL;
  }
  return (t != NULL);
}
#endif // ifndef QT_IO_H
/* vim:set expandtab: */
