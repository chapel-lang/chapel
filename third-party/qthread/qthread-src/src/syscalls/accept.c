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

int qt_accept(int                       socket,
              struct sockaddr *restrict address,
              socklen_t *restrict       address_len)
{
    qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
    int                       ret;
    qthread_t                *me = qthread_internal_self();

    assert(job);
    job->next   = NULL;
    job->thread = me;
    job->op     = ACCEPT;
    memcpy(&job->args[0], &socket, sizeof(int));
    job->args[1] = (uintptr_t)address;
    job->args[2] = (uintptr_t)address_len;

    assert(me->rdata);
    me->rdata->blockedon.io = job;
    me->thread_state     = QTHREAD_STATE_SYSCALL;
    qthread_back_to_master(me);
    ret = job->ret;
    FREE_SYSCALLJOB(job);
    return ret;
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_ACCEPT
int accept(int                       socket,
           struct sockaddr *restrict address,
           socklen_t *restrict       address_len)
{
    if (qt_blockable()) {
        return qt_accept(socket, address, address_len);
    } else {
        return syscall(SYS_accept, socket, address, address_len);
    }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_ACCEPT */

/* vim:set expandtab: */
