#include <stdint.h>

/* System Headers */
#include <sys/select.h>

#include <sys/syscall.h> /* for SYS_accept and others */
#include <unistd.h>

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_io.h"
#include "qt_qthread_mgmt.h"
#include "qthread_innards.h" /* for qlib */

int qt_select(int nfds,
              fd_set *restrict readfds,
              fd_set *restrict writefds,
              fd_set *restrict errorfds,
              struct timeval *restrict timeout) {
  qthread_t *me = qthread_internal_self();
  qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
  int ret;

  assert(job);
  job->next = NULL;
  job->thread = me;
  job->op = SELECT;
  memcpy(&job->args[0], &nfds, sizeof(int));
  job->args[1] = (uintptr_t)readfds;
  job->args[2] = (uintptr_t)writefds;
  job->args[3] = (uintptr_t)errorfds;
  job->args[4] = (uintptr_t)timeout;

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
