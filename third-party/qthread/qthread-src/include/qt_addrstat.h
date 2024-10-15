#ifndef QT_ADDRSTAT_H
#define QT_ADDRSTAT_H

#include "qt_blocking_structs.h"

/* This allocates a new, initialized addrstat structure, which is used for
 * keeping track of the FEB status of an address. It expects a shepherd pointer
 * to use to find the right memory pool to use. */
static inline qthread_addrstat_t *qthread_addrstat_new(void) { /*{{{ */
  qthread_addrstat_t *ret = ALLOC_ADDRSTAT();
  QTHREAD_FASTLOCK_INIT_PTR(&ret->lock);
  QTHREAD_FASTLOCK_LOCK(&ret->lock);
  ret->full = 1;
  ret->valid = 1;
  ret->EFQ = NULL;
  ret->FEQ = NULL;
  ret->FFQ = NULL;
  ret->FFWQ = NULL;
  QTHREAD_FASTLOCK_UNLOCK(&ret->lock);

  return ret;
} /*}}} */

/* this function is for maintenance of the FEB hashtables. SHOULD only be
 * necessary for things left over when qthread_finalize is called */
static void qthread_addrstat_delete(void *m_void) { /*{{{ */
  qthread_addrstat_t *m = (qthread_addrstat_t *)m_void;
  QTHREAD_FASTLOCK_DESTROY(m->lock);
  FREE_ADDRSTAT(m);
} /*}}} */

#endif // ifndef QT_ADDRSTAT_H
/* vim:set expandtab: */
