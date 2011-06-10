#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <unistd.h>

#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */

ssize_t write(int         filedes,
              const void *buf,
              size_t      nbyte)
{
    qthread_t *me;

    if ((qlib != NULL) && ((me = qthread_internal_self()) != NULL)) {
        qt_blocking_queue_node_t *job = qt_mpool_alloc(syscall_job_pool);
        ssize_t                   ret;

        assert(job);
        job->thread = me;
        job->op     = WRITE;
        memcpy(&job->args[0], &filedes, sizeof(int));
        job->args[1] = (uintptr_t)buf;
        memcpy(&job->args[2], &nbyte, sizeof(size_t));

        assert(me->rdata);

        me->rdata->blockedon = (struct qthread_lock_s *)job;
        me->thread_state     = QTHREAD_STATE_SYSCALL;
        qthread_back_to_master(me);
        ret = job->ret;
        qt_mpool_free(syscall_job_pool, job);
        return ret;
    } else {
        return syscall(SYS_write, filedes, buf, nbyte);
    }
}

/* vim:set expandtab: */
