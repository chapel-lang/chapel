#include <stdint.h>

/* System Headers */
#include <sys/syscall.h> /* for SYS_accept and others */
#include <unistd.h>

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_io.h"
#include "qt_qthread_mgmt.h"
#include "qthread_innards.h" /* for qlib */

int qt_accept(int socket,
              struct sockaddr *restrict address,
              socklen_t *restrict address_len) {
  qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
  int ret;
  qthread_t *me = qthread_internal_self();

  assert(job);
  job->next = NULL;
  job->thread = me;
  job->op = ACCEPT;
  memcpy(&job->args[0], &socket, sizeof(int));
  job->args[1] = (uintptr_t)address;
  job->args[2] = (uintptr_t)address_len;

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
