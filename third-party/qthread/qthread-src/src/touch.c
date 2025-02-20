/* Internal Headers */
#include "qt_qthread_mgmt.h" /* for qthread_internal_self() */
#include "qt_shepherd_innards.h"
#include "qt_threadqueues.h"
#include "qt_touch.h"

void qthread_run_needed_task(syncvar_t *value) {
  qthread_shepherd_t *shep = qthread_internal_getshep();
  qthread_t *orig_t = qthread_internal_self();

  if (qt_threadqueue_dequeue_specific(shep->ready, value)) {
    // switch to task and run -- else missing and return
    qthread_back_to_master(orig_t);
  }
}

/* vim:set expandtab: */
