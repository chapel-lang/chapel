#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */

void qt_blocking_subsystem_begin_blocking_action(void)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qt_blocking_queue_node_t *job = qt_mpool_alloc(syscall_job_pool);

        assert(job);
        job->thread = me;
        job->op     = USER_DEFINED;

        assert(me->rdata);

        me->rdata->blockedon = (struct qthread_lock_s *)job;
        me->thread_state     = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        /* ...and I wake up in a dedicated pthread! */
    }
}

void qt_blocking_subsystem_end_blocking_action(void)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        me->thread_state = QTHREAD_STATE_RUNNING;
        qthread_back_to_master(me);
    }
}

/* vim:set expandtab: */
