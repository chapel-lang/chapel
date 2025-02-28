#ifndef QT_BLOCKING_STRUCTS_H
#define QT_BLOCKING_STRUCTS_H

#include <stdlib.h> /* for malloc() and free() */
#include <sys/types.h>

#include "qt_mpool.h"
#include "qt_profiling.h"
#include "qt_shepherd_innards.h"

typedef enum blocking_syscalls {
  ACCEPT,
  CONNECT,
  NANOSLEEP,
  POLL,
  READ,
  PREAD,
  /*RECV,
   * RECVFROM,*/
  SELECT,
  /*SEND,
   * SENDTO,*/
  /*SIGWAIT,*/
  SLEEP,
  SYSTEM,
  USLEEP,
  WAIT4,
  WRITE,
  PWRITE,
  USER_DEFINED
} syscall_t;

typedef struct qthread_addrres_s {
  aligned_t *addr; /* ptr to the memory NOT being blocked on */
  qthread_t *waiter;
  struct qthread_addrres_s *next;
} qthread_addrres_t;

typedef struct _qt_blocking_queue_node_s {
  struct _qt_blocking_queue_node_s *next;
  qthread_t *thread;
  syscall_t op;
  uintptr_t args[5];
  ssize_t ret;
  int err;
} qt_blocking_queue_node_t;

typedef struct qthread_addrstat_s {
  QTHREAD_FASTLOCK_TYPE lock;
  qthread_addrres_t *EFQ;
  qthread_addrres_t *FEQ;
  qthread_addrres_t *FFQ;
  qthread_addrres_t *FFWQ;
  uint_fast8_t full;
  uint_fast8_t valid;
} qthread_addrstat_t;

extern qt_mpool generic_addrstat_pool;
#define ALLOC_ADDRSTAT()                                                       \
  (qthread_addrstat_t *)qt_mpool_alloc(generic_addrstat_pool)
#define FREE_ADDRSTAT(t) qt_mpool_free(generic_addrstat_pool, t)

extern qt_mpool generic_addrres_pool;

static inline qthread_addrres_t *ALLOC_ADDRRES(void) { /*{{{ */
  qthread_addrres_t *tmp =
    (qthread_addrres_t *)qt_mpool_alloc(generic_addrres_pool);

  return tmp;
} /*}}} */

static inline void FREE_ADDRRES(qthread_addrres_t *t) { /*{{{ */
  qt_mpool_free(generic_addrres_pool, t);
} /*}}} */

#endif // ifndef QT_BLOCKING_STRUCTS_H
/* vim:set expandtab: */
