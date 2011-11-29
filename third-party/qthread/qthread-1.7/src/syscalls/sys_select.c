#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <sys/select.h>

#include <unistd.h>
#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */

int select(int                      nfds,
           fd_set *restrict         readfds,
           fd_set *restrict         writefds,
           fd_set *restrict         errorfds,
           struct timeval *restrict timeout)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB;
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

        me->rdata->blockedon = (struct qthread_lock_s *)job;
        me->thread_state     = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        ret = job->ret;
        qt_mpool_free(syscall_job_pool, job);
        return ret;
    } else {
        return syscall(SYS_select, nfds, readfds, writefds, errorfds, timeout);
    }
}

/* vim:set expandtab: */
