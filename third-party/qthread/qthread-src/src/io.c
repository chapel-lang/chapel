#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h>       /* for uint64_t */
#include <stdio.h>                     /* for fprintf() */
#include <stdlib.h>                    /* for abort() */
#include <sys/time.h>                  /* for gettimeofday() */
#ifdef HAVE_SYS_SYSCALL_H
/* - syscall(2) */
# include <sys/syscall.h>
# include <unistd.h>
#endif
/* - accept(2) */
#include <sys/socket.h>
/* - connect(2) */
#include <sys/types.h>
#include <sys/socket.h>
/* - nanosleep(2) */
#include <time.h>
/* - poll(2) */
#include <poll.h>
/* - read(2) */
#include <sys/uio.h>
/* - select(2) */
#include <sys/select.h>
/* - wait4(2) */
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

/* Internal Headers */
#include "qt_io.h"
#include "qt_macros.h"
#include "qt_asserts.h"
#include "qthread_innards.h" /* for qthread_exec() */
#include "qt_threadqueues.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "qt_subsystems.h"

typedef struct {
    qt_blocking_queue_node_t *head;
    qt_blocking_queue_node_t *tail;
    saligned_t                length;
    pthread_mutex_t           lock;
    pthread_cond_t            notempty;
} qt_blocking_queue_t;

static qt_blocking_queue_t theQueue;
static saligned_t          io_worker_count = -1;
static saligned_t          io_worker_max   = 10;
#if !defined(UNPOOLED)
qt_mpool syscall_job_pool = NULL;
#endif
static unsigned long timeout    = 100; // in microseconds
static int           proxy_exit = 0;
TLS_DECL_INIT(qthread_t *, IO_task_struct);

static void qt_blocking_subsystem_internal_stopwork(void)
{   /*{{{*/
    proxy_exit = 1;
    MACHINE_FENCE;
    while (io_worker_count != 0) SPINLOCK_BODY();
    QTHREAD_LOCK(&theQueue.lock);
    QTHREAD_UNLOCK(&theQueue.lock);
} /*}}}*/

static void qt_blocking_subsystem_internal_freemem(void)
{   /*{{{*/
#if !defined(UNPOOLED)
    qt_mpool_destroy(syscall_job_pool);
#endif
    QTHREAD_DESTROYLOCK(&theQueue.lock);
    QTHREAD_DESTROYCOND(&theQueue.notempty);
} /*}}}*/

static void *qt_blocking_subsystem_proxy_thread(void *QUNUSED(arg))
{   /*{{{*/
    while (proxy_exit == 0) {
        if (qt_process_blocking_call()) {
            break;
        }
        COMPILER_FENCE;
    }
    qthread_debug(IO_DETAILS, "proxy_exit = %i, exiting\n", proxy_exit);
    pthread_exit(NULL);
    return 0;
} /*}}}*/

static void qt_blocking_subsystem_spawnworker(void)
{   /*{{{*/
    int       r;
    pthread_t thr;

    if ((r = pthread_create(&thr, NULL, qt_blocking_subsystem_proxy_thread, NULL)) != 0) {
        fprintf(stderr, "qt_blocking_subsystem_init: pthread_create() failed (%d)\n", r);
        perror("qt_blocking_subsystem_init spawning proxy thread");
        abort();
    }
    (void)qthread_incr(&io_worker_count, 1);
    pthread_detach(thr);
} /*}}}*/

void INTERNAL qt_blocking_subsystem_init(void)
{   /*{{{*/
#if !defined(UNPOOLED)
    syscall_job_pool = qt_mpool_create(sizeof(qt_blocking_queue_node_t));
#endif
    theQueue.head   = NULL;
    theQueue.tail   = NULL;
    io_worker_count = 0;
    io_worker_max   = qt_internal_get_env_num("MAX_IO_WORKERS", 10, 1);
    timeout         = qt_internal_get_env_num("IO_TIMEOUT", 100, 100);
    TLS_INIT(IO_task_struct);
    qassert(pthread_mutex_init(&theQueue.lock, NULL), 0);
    qassert(pthread_cond_init(&theQueue.notempty, NULL), 0);
    /* thread(s) must be stopped *before* shepherds die, to keep them from
     * trying to push orphan threads into shepherd queues */
    qthread_internal_cleanup_early(qt_blocking_subsystem_internal_stopwork);
    /* must be torn down *after* shepherds die, because live shepherd might try
     * to enqueue into my queue during shutdown */
    qthread_internal_cleanup(qt_blocking_subsystem_internal_freemem);
} /*}}}*/

int INTERNAL qt_process_blocking_call(void)
{   /*{{{*/
    qt_blocking_queue_node_t *item;

    QTHREAD_LOCK(&theQueue.lock);
    while (theQueue.head == NULL) {
        struct timeval  tv;
        struct timespec ts;
        int             ret;

        COMPILER_FENCE;
        gettimeofday(&tv, NULL);
        ts.tv_sec  = tv.tv_sec;
        ts.tv_nsec = (tv.tv_usec + timeout) * 1000;
        ret        = pthread_cond_timedwait(&theQueue.notempty, &theQueue.lock, &ts);
        switch(ret) {
            case ETIMEDOUT:
                qthread_debug(IO_BEHAVIOR, "condwait timed out\n");
                if (theQueue.head == NULL) {
                    qthread_debug(IO_BEHAVIOR, "------------------------------------- exit()\n");
#ifdef QTHREAD_DEBUG
                    unsigned ct = qthread_incr(&io_worker_count, -1);
                    qthread_debug(IO_BEHAVIOR, "worker_count post exit is %u\n", (unsigned)ct - 1);
#else
                    (void)qthread_incr(&io_worker_count, -1);
#endif
                    QTHREAD_UNLOCK(&theQueue.lock);
                    return 1;
                } else {
                    QTHREAD_UNLOCK(&theQueue.lock);
                    return 0;
                }

            case EINVAL:
                /* chances are, this is because ts is in the past */
                qthread_debug(IO_DETAILS, "condwait returned EINVAL\n");
                break;

            default:
                break;
        }
    }
    item = theQueue.head;
    assert(item != NULL);
    theQueue.head = item->next;
    if (theQueue.tail == item) {
        theQueue.tail = theQueue.head;
    }
    theQueue.length--;
    qthread_debug(IO_DETAILS, "dequeue... theQueue.head = %p, .tail = %p, item:%p, thread:%p, rdata:%p\n", theQueue.head, theQueue.tail, item, item->thread, item->thread->rdata);
    QTHREAD_UNLOCK(&theQueue.lock);
    item->next = NULL;
    /* do something with <item> */
    switch(item->op) {
        default:
            fprintf(stderr, "Unhandled syscall: %u\n", (unsigned int)item->op);
        // abort();
        case ACCEPT:
        {
            int socket;
            memcpy(&socket, &item->args[0], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_ACCEPT
            item->ret = syscall(SYS_accept,
                                socket,
                                (struct sockaddr *)item->args[1],
                                (socklen_t *)item->args[2]);
#else
            item->ret = accept(socket,
                               (struct sockaddr *)item->args[1],
                               (socklen_t *)item->args[2]);
#endif
            break;
        }
        case CONNECT:
        {
            int socket;
            memcpy(&socket, &item->args[0], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_CONNECT
            item->ret = syscall(SYS_connect,
                                socket,
                                (const struct sockaddr *)item->args[1],
                                (socklen_t)item->args[2]);
#else
            item->ret = connect(socket,
                                (const struct sockaddr *)item->args[1],
                                (socklen_t)item->args[2]);
#endif      /* if HAVE_DECL_SYS_CONNECT */
            break;
        }
        case POLL:
        {
            nfds_t nfds;
            int    timeout;
            memcpy(&nfds, &item->args[1], sizeof(nfds_t));
            memcpy(&timeout, &item->args[2], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_POLL
            item->ret = syscall(SYS_poll,
                                (struct pollfd *)item->args[0],
                                nfds,
                                timeout);
#else
            item->ret = poll((struct pollfd *)item->args[0],
                             nfds,
                             timeout);
#endif
            break;
        }
        case READ:
        {
            int fd;
            memcpy(&fd, &item->args[0], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_READ
            item->ret = syscall(SYS_read,
                                fd,
                                (void *)item->args[1],
                                (size_t)item->args[2]);
#else
            item->ret = read(fd,
                             (void *)item->args[1],
                             (size_t)item->args[2]);
#endif
            break;
        }
        case PREAD:
        {
            int   fd;
            off_t offset;
            memcpy(&fd, &item->args[0], sizeof(int));
            memcpy(&offset, &item->args[3], sizeof(off_t));
#if HAVE_SYSCALL && HAVE_DECL_SYS_PREAD
            item->ret = syscall(SYS_pread,
                                fd,
                                (void *)item->args[1],
                                (size_t)item->args[2],
                                offset);
#else
            item->ret = pread(fd,
                              (void *)item->args[1],
                              (size_t)item->args[2],
                              offset);
#endif
            break;
        }
        /* case RECV:
         * case RECVFROM: */
        case SELECT:
        {
            int nfds;
            memcpy(&nfds, &item->args[0], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_SELECT
            item->ret = syscall(SYS_select,
                                nfds,
                                (fd_set *)item->args[1],
                                (fd_set *)item->args[2],
                                (fd_set *)item->args[3],
                                (struct timeval *)item->args[4]);
#else
            item->ret = select(nfds,
                               (fd_set *)item->args[1],
                               (fd_set *)item->args[2],
                               (fd_set *)item->args[3],
                               (struct timeval *)item->args[4]);
#endif      /* if HAVE_DECL_SYS_SELECT */
            break;
        }
        /* case SEND:
         * case SENDTO: */
        /* case SIGWAIT: */
        case SYSTEM:
#if HAVE_SYSCALL && HAVE_DECL_SYS_SYSTEM
            item->ret = syscall(SYS_system,
                                (const char *)item->args[0]);
#else
            item->ret = system((const char *)item->args[0]);
#endif
            break;
        case WAIT4:
        {
            pid_t pid;
            int   options;
            memcpy(&pid, &item->args[0], sizeof(pid_t));
            memcpy(&options, &item->args[2], sizeof(int));
#if HAVE_SYSCALL && HAVE_DECL_SYS_WAIT4
            item->ret = syscall(SYS_wait4,
                                pid,
                                (int *)item->args[1],
                                options,
                                (struct rusage *)item->args[3]);
#else
            item->ret = wait4(pid,
                              (int *)item->args[1],
                              options,
                              (struct rusage *)item->args[3]);
#endif
            break;
        }
        case WRITE:
#if HAVE_SYSCALL && HAVE_DECL_SYS_WRITE
            item->ret = syscall(SYS_write,
                                (int)item->args[0],
                                (const void *)item->args[1],
                                (size_t)item->args[2]);
#else
            item->ret = write((int)item->args[0],
                              (const void *)item->args[1],
                              (size_t)item->args[2]);
#endif
        case PWRITE:
#if HAVE_SYSCALL && HAVE_DECL_SYS_PWRITE
            item->ret = syscall(SYS_pwrite,
                                (int)item->args[0],
                                (const void *)item->args[1],
                                (size_t)item->args[2],
                                (off_t)item->args[3]);
#else
            item->ret = pwrite((int)item->args[0],
                               (const void *)item->args[1],
                               (size_t)item->args[2],
                               (off_t)item->args[3]);
#endif
            break;
        case USER_DEFINED:
        {
            qt_context_t my_context;
            TLS_SET(IO_task_struct, item->thread);
            getcontext(&my_context);
            qthread_debug(IO_DETAILS, "blocking proxy context is %p (item:%p, thread:%p, rdata:%p)\n", &my_context, item, item->thread, item->thread->rdata);
            qthread_exec(item->thread, &my_context);
            qthread_debug(IO_DETAILS, "proxy back from qthread_exec (item:%p, thread:%p, rdata:%p)\n", item, item->thread, item->thread->rdata);
            // TLS_SET(IO_task_struct, NULL);
            break;
        }
    }
    /* and now, re-queue */
    qt_threadqueue_enqueue(item->thread->rdata->shepherd_ptr->ready, item->thread);
    FREE_SYSCALLJOB(item);
    return 0;
} /*}}}*/

void INTERNAL qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job)
{   /*{{{*/
    qt_blocking_queue_node_t *prev;

    qthread_debug(IO_FUNCTIONS, "entering, job = %p, thread:%p, rdata:%p\n", job, job->thread, job->thread->rdata);
    assert(job->next == NULL);
    assert(job->thread->rdata);
    QTHREAD_LOCK(&theQueue.lock);
    qthread_debug(IO_DETAILS, "1) theQueue.head = %p, .tail = %p, job = %p\n", theQueue.head, theQueue.tail, job);
    prev          = theQueue.tail;
    theQueue.tail = job;
    if (prev == NULL) {
        theQueue.head = job;
    } else {
        prev->next = job;
    }
    theQueue.length++;
    qthread_debug(IO_DETAILS, "2) theQueue.head = %p, .tail = %p, job = %p\n", theQueue.head, theQueue.tail, job);
    if (io_worker_count < theQueue.length) {
        if (io_worker_count < io_worker_max) {
            qthread_debug(IO_DETAILS, "++++++++++++++++++++ I think I oughta spawn a worker\n");
            qt_blocking_subsystem_spawnworker();
        }
    } else {
        qthread_debug(IO_DETAILS, "Queue is %u long, there are %u workers\n", (unsigned)theQueue.length, (unsigned)io_worker_count);
        QTHREAD_COND_SIGNAL(theQueue.notempty);
    }
    QTHREAD_UNLOCK(&theQueue.lock);
    qthread_debug(IO_FUNCTIONS, "exiting, job = %p\n", job);
} /*}}}*/

/* vim:set expandtab: */
