#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <stdlib.h>

#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */

int system(const char *command)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qt_blocking_queue_node_t *job = qt_mpool_alloc(syscall_job_pool);
        int                       ret;

        assert(job);
        job->thread = me;
        job->op     = SYSTEM;
        job->args[0] = (uintptr_t)command;

        assert(me->rdata);

        me->rdata->blockedon = (struct qthread_lock_s *)job;
        me->thread_state     = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        ret = job->ret;
        qt_mpool_free(syscall_job_pool, job);
        return ret;
    } else {
        return syscall(SYS_system, command);
    }
}

/* vim:set expandtab: */
