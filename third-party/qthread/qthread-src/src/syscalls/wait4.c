#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

/* System Headers */
#ifdef HAVE_SYS_SYSCALL_H
#include <sys/syscall.h> /* for SYS_accept and others */
#include <unistd.h>
#endif

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_io.h"
#include "qt_qthread_mgmt.h"
#include "qthread_innards.h" /* for qlib */

pid_t qt_wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage) {
  qthread_t *me = qthread_internal_self();
  qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
  pid_t ret;

  assert(job);
  job->next = NULL;
  job->thread = me;
  job->op = WAIT4;
  memcpy(&job->args[0], &pid, sizeof(pid_t));
  job->args[1] = (uintptr_t)stat_loc;
  memcpy(&job->args[2], &options, sizeof(int));
  job->args[3] = (uintptr_t)rusage;

  assert(me->rdata);

  me->rdata->blockedon.io = job;
  atomic_store_explicit(
    &me->thread_state, QTHREAD_STATE_SYSCALL, memory_order_relaxed);
  qthread_back_to_master(me);
  ret = job->ret;
  errno = job->err;
  FREE_SYSCALLJOB(job);
  return ret;
}

/* vim:set expandtab: */
