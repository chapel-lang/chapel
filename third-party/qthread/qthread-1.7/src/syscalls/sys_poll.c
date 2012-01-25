#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <poll.h>

#include <unistd.h>
#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */

int poll(struct pollfd fds[],
         nfds_t        nfds,
         int           timeout)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB;
        int                       ret;

        assert(job);
        job->next   = NULL;
        job->thread  = me;
        job->op      = POLL;
        job->args[0] = (uintptr_t)&(fds[0]);
        memcpy(&job->args[1], &nfds, sizeof(nfds_t));
        memcpy(&job->args[2], &timeout, sizeof(int));

        assert(me->rdata);

        me->rdata->blockedon = (struct qthread_lock_s *)job;
        me->thread_state     = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        ret = job->ret;
        qt_mpool_free(syscall_job_pool, job);
        return ret;
    } else {
        return syscall(SYS_poll, fds, nfds, timeout);
    }
}

/* vim:set expandtab: */
