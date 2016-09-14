#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <sys/select.h>

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_io.h"
#include "qt_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

int qt_select(int                      nfds,
              fd_set *restrict         readfds,
              fd_set *restrict         writefds,
              fd_set *restrict         errorfds,
              struct timeval *restrict timeout)
{
    qthread_t                *me  = qthread_internal_self();
    qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
    int                       ret;

    assert(job);
    job->next   = NULL;
    job->thread = me;
    job->op     = SELECT;
    memcpy(&job->args[0], &nfds, sizeof(int));
    job->args[1] = (uintptr_t)readfds;
    job->args[2] = (uintptr_t)writefds;
    job->args[3] = (uintptr_t)errorfds;
    job->args[4] = (uintptr_t)timeout;

    assert(me->rdata);

    me->rdata->blockedon.io = job;
    me->thread_state        = QTHREAD_STATE_SYSCALL;
    qthread_back_to_master(me);
    ret = job->ret;
    FREE_SYSCALLJOB(job);
    return ret;
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_SELECT
int select(int                      nfds,
           fd_set *restrict         readfds,
           fd_set *restrict         writefds,
           fd_set *restrict         errorfds,
           struct timeval *restrict timeout)
{
    if (qt_blockable()) {
        return qt_select(nfds, readfds, writefds, errorfds, timeout);
    } else {
        return syscall(SYS_select, nfds, readfds, writefds, errorfds, timeout);
    }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_SELECT */

/* vim:set expandtab: */
