#ifndef QT_IO_H
#define QT_IO_H

#include "stdlib.h"                  /* malloc() and free() */

#include "qt_qthread_struct.h"

#if defined(UNPOOLED)
# define ALLOC_SYSCALLJOB (qt_blocking_queue_node_t *)malloc(sizeof(qt_blocking_queue_node_t))
# define FREE_SYSCALLJOB(s) free(s)
#else
# define ALLOC_SYSCALLJOB qt_mpool_alloc(syscall_job_pool);
# define FREE_SYSCALLJOB(j) qt_mpool_cached_free(syscall_job_pool, j);
#endif

typedef enum blocking_syscalls {
    ACCEPT,
    CONNECT,
    NANOSLEEP,
    POLL,
    READ,
    PREAD,
    /*RECV,
     * RECVFROM,*/
    SELECT,
    /*SEND,
     * SENDTO,*/
    /*SIGWAIT,*/
    SLEEP,
    SYSTEM,
    USLEEP,
    WAIT4,
    WRITE,
    PWRITE,
    USER_DEFINED
} syscall_t;

typedef struct _qt_blocking_queue_node_s {
    struct _qt_blocking_queue_node_s *next;
    qthread_t                        *thread;
    syscall_t                         op;
    uintptr_t                         args[5];
    ssize_t                           ret;
} qt_blocking_queue_node_t;

extern qt_mpool syscall_job_pool;

void qt_blocking_subsystem_init(void);
int  qt_process_blocking_call(void);
void qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job);

void qt_blocking_subsystem_begin_blocking_action(void);
void qt_blocking_subsystem_end_blocking_action(void);

#endif // ifndef QT_IO_H
/* vim:set expandtab: */
