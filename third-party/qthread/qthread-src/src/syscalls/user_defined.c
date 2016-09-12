#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#ifdef HAVE_SYS_SYSCALL_H
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* Internal Headers */
#include "qt_io.h"
#include "qt_asserts.h"
#include "qt_debug.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

extern TLS_DECL(qthread_t *, IO_task_struct);

void qt_begin_blocking_action(void)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qthread_debug(IO_CALLS, "in qthreads, me=%p\n", me);
        qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();

        assert(job);
        job->next   = NULL;
        job->thread = me;
        job->op     = USER_DEFINED;

        assert(me->rdata);

        me->rdata->blockedon.io = job;
        me->thread_state        = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        /* ...and I wake up in a dedicated pthread! */
    } else {
        qthread_debug(IO_CALLS, "NOT in qthreads\n");
    }
}

void qt_end_blocking_action(void)
{
    qthread_t *me = TLS_GET(IO_task_struct);

    if ((qlib != NULL) && (me != NULL)) {
        qthread_debug(IO_CALLS, "in qthreads, me=%p\n", me);
        assert(me != NULL);
        qthread_back_to_master(me);
    } else {
        qthread_debug(IO_CALLS, "NOT in qthreads' IO subsystem\n");
    }
}

/* vim:set expandtab: */
