#include <stdint.h>

#include <sys/syscall.h> /* for SYS_accept and others */
#include <unistd.h>

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_io.h"
#include "qt_qthread_mgmt.h"
#include "qthread_innards.h" /* for qlib */

int qt_poll(struct pollfd fds[], nfds_t nfds, int timeout) {
  qthread_t *me = qthread_internal_self();
  qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
  int ret;

  assert(job);
  job->next = NULL;
  job->thread = me;
  job->op = POLL;
  job->args[0] = (uintptr_t)&(fds[0]);
  memcpy(&job->args[1], &nfds, sizeof(nfds_t));
  memcpy(&job->args[2], &timeout, sizeof(int));

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
