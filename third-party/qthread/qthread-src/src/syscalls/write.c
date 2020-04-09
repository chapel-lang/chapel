#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

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

ssize_t qt_write(int         filedes,
                 const void *buf,
                 size_t      nbyte)
{
    qthread_t                *me  = qthread_internal_self();
    qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
    ssize_t                   ret;

    assert(job);
    job->next   = NULL;
    job->thread = me;
    job->op     = WRITE;
    memcpy(&job->args[0], &filedes, sizeof(int));
    job->args[1] = (uintptr_t)buf;
    memcpy(&job->args[2], &nbyte, sizeof(size_t));

    assert(me->rdata);

    me->rdata->blockedon.io = job;
    me->thread_state        = QTHREAD_STATE_SYSCALL;
    qthread_back_to_master(me);
    ret = job->ret;
    FREE_SYSCALLJOB(job);
    return ret;
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_WRITE
ssize_t write(int         filedes,
              const void *buf,
              size_t      nbyte)
{
    if ((qlib != NULL) && (qthread_internal_self() != NULL)) {
        return qt_write(filedes, buf, nbyte);
    } else {
        return syscall(SYS_write, filedes, buf, nbyte);
    }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_WRITE */

/* vim:set expandtab: */
