#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h>       /* for uint64_t */
#include <stdio.h>                     /* for fprintf() */
#include <stdlib.h>                    /* for abort() */
#include <sys/time.h>                  /* for gettimeofday() */
/* - syscall(2) */
#include <sys/syscall.h>
#include <unistd.h>
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

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"

typedef struct {
    qt_blocking_queue_node_t *head;
    qt_blocking_queue_node_t *tail;
    pthread_mutex_t           lock;
    pthread_cond_t            notempty;
} qt_blocking_queue_t;

static qt_blocking_queue_t theQueue;
#if !defined(UNPOOLED)
qt_mpool syscall_job_pool = NULL;
#endif
static pthread_t     proxy_thread;
static volatile int  proxy_exit = 0;
pthread_key_t        IO_task_struct;
extern pthread_key_t shepherd_structs;

static void qt_blocking_subsystem_internal_stopwork(void)
{   /*{{{*/
    proxy_exit = 1;
    pthread_join(proxy_thread, NULL);
} /*}}}*/

static void qt_blocking_subsystem_internal_freemem(void)
{   /*{{{*/
#if !defined(UNPOOLED)
    qt_mpool_destroy(syscall_job_pool);
#endif
    QTHREAD_DESTROYLOCK(&theQueue.lock);
    QTHREAD_DESTROYCOND(&theQueue.notempty);
} /*}}}*/

static void *qt_blocking_subsystem_proxy_thread(void *arg)
{   /*{{{*/
    pthread_setspecific(shepherd_structs, (void *)1);
    while (proxy_exit == 0) {
        pthread_setspecific(IO_task_struct, NULL);
        qt_process_blocking_calls();
    }
    qthread_debug(IO_DETAILS, "proxy_exit = %i, exiting\n", proxy_exit);
    pthread_exit(NULL);
    return 0;
} /*}}}*/

void INTERNAL qt_blocking_subsystem_init(void)
{   /*{{{*/
#if !defined(UNPOOLED)
    syscall_job_pool = qt_mpool_create(sizeof(qt_blocking_queue_node_t));
#endif
    theQueue.head = NULL;
    theQueue.tail = NULL;
    qassert(pthread_key_create(&IO_task_struct, NULL), 0);
    qassert(pthread_mutex_init(&theQueue.lock, NULL), 0);
    qassert(pthread_cond_init(&theQueue.notempty, NULL), 0);
    {
        int r;
        if ((r = pthread_create(&proxy_thread, NULL, qt_blocking_subsystem_proxy_thread, NULL)) != 0) {
            fprintf(stderr, "qt_blocking_subsystem_init: pthread_create() failed (%d)\n", r);
            perror("qt_blocking_subsystem_init spawning proxy thread");
            abort();
        }
    }
    /* thread(s) must be stopped *before* shepherds die, to keep them from
     * trying to push orphan threads into shepherd queues */
    qthread_internal_cleanup_early(qt_blocking_subsystem_internal_stopwork);
    /* must be torn down *after* shepherds die, because live shepherd might try
     * to enqueue into my queue during shutdown */
    qthread_internal_cleanup(qt_blocking_subsystem_internal_freemem);
} /*}}}*/

void INTERNAL qt_process_blocking_calls(void)
{   /*{{{*/
    qt_blocking_queue_node_t *item;

    QTHREAD_LOCK(&theQueue.lock);
    while (theQueue.head == NULL) {
        struct timeval  tv;
        struct timespec ts;
        int             ret;

        gettimeofday(&tv, NULL);
        ts.tv_sec  = tv.tv_sec;
        ts.tv_nsec = (tv.tv_usec + 100) * 1000;
        ret        = pthread_cond_timedwait(&theQueue.notempty, &theQueue.lock, &ts);
        switch(ret) {
            case ETIMEDOUT:
                // qthread_debug(IO_DETAILS, "condwait timed out\n");
                QTHREAD_UNLOCK(&theQueue.lock);
                return;

            case EINVAL:
                /* chances are, this is because ts is in the past */
                QTHREAD_UNLOCK(&theQueue.lock);
                return;

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
    qthread_debug(IO_DETAILS, "dequeue... theQueue.head = %p, .tail = %p, item = %p\n", theQueue.head, theQueue.tail, item);
    QTHREAD_UNLOCK(&theQueue.lock);
    item->next = NULL;
    /* do something with <item> */
    qassert(pthread_setspecific(IO_task_struct, item->thread), 0);
    switch(item->op) {
        default:
            fprintf(stderr, "Unhandled syscall: %u\n", (unsigned int)item->op);
            // abort();
#if HAVE_DECL_SYS_ACCEPT
        case ACCEPT:
        {
            int socket;
            memcpy(&socket, &item->args[0], sizeof(int));
            item->ret = syscall(SYS_accept,
                                socket,
                                (struct sockaddr *)item->args[1],
                                (socklen_t *)item->args[2]);
            break;
        }
#endif  /* if HAVE_DECL_SYS_ACCEPT */
#if HAVE_DECL_SYS_CONNECT
        case CONNECT:
        {
            int socket;
            memcpy(&socket, &item->args[0], sizeof(int));
            item->ret = syscall(SYS_connect,
                                socket,
                                (const struct sockaddr *)item->args[1],
                                (socklen_t)item->args[2]);
            break;
        }
#endif  /* if HAVE_DECL_SYS_CONNECT */
        case POLL:
        {
            nfds_t nfds;
            int    timeout;
            memcpy(&nfds, &item->args[1], sizeof(nfds_t));
            memcpy(&timeout, &item->args[2], sizeof(int));
            item->ret = syscall(SYS_poll,
                                (struct pollfd *)item->args[0],
                                nfds,
                                timeout);
            break;
        }
        case READ:
        {
            int fd;
            memcpy(&fd, &item->args[0], sizeof(int));
            item->ret = syscall(SYS_read,
                                fd,
                                (void *)item->args[1],
                                (size_t)item->args[2]);
            break;
        }
#if HAVE_DECL_SYS_PREAD
        case PREAD:
        {
            int   fd;
            off_t offset;
            memcpy(&fd, &item->args[0], sizeof(int));
            memcpy(&offset, &item->args[3], sizeof(off_t));
            item->ret = syscall(SYS_pread,
                                fd,
                                (void *)item->args[1],
                                (size_t)item->args[2],
                                offset);
            break;
        }
#endif  /* if HAVE_DECL_SYS_PREAD */
        /* case RECV:
         * case RECVFROM: */
#if HAVE_DECL_SYS_SELECT
        case SELECT:
        {
            int nfds;
            memcpy(&nfds, &item->args[0], sizeof(int));
            item->ret = syscall(SYS_select,
                                nfds,
                                (fd_set *)item->args[1],
                                (fd_set *)item->args[2],
                                (fd_set *)item->args[3],
                                (struct timeval *)item->args[4]);
            break;
        }
#endif  /* if HAVE_DECL_SYS_SELECT */
            /* case SEND:
             * case SENDTO: */
            /* case SIGWAIT: */
#if HAVE_DECL_SYS_SYSTEM
        case SYSTEM:
            item->ret = syscall(SYS_system,
                                (const char *)item->args[0]);
            break;
#endif
#if HAVE_DECL_SYS_WAIT4
        case WAIT4:
        {
            pid_t pid;
            int   options;
            memcpy(&pid, &item->args[0], sizeof(pid_t));
            memcpy(&options, &item->args[2], sizeof(int));
            item->ret = syscall(SYS_wait4,
                                pid,
                                (int *)item->args[1],
                                options,
                                (struct rusage *)item->args[3]);
            break;
        }
#endif  /* if HAVE_DECL_SYS_WAIT4 */
        case WRITE:
            item->ret = syscall(SYS_write,
                                (int)item->args[0],
                                (const void *)item->args[1],
                                (size_t)item->args[2],
                                (off_t)item->args[3]);
        case USER_DEFINED:
        {
            qt_context_t my_context;
            getcontext(&my_context);
            qthread_debug(IO_DETAILS, "blocking proxy context is %p\n", &my_context);
            qthread_exec(item->thread, &my_context);
            qthread_debug(IO_DETAILS, "proxy back from qthread_exec\n");
            FREE_SYSCALLJOB(item);
            break;
        }
    }
    /* and now, re-queue */
    qt_threadqueue_enqueue(item->thread->rdata->shepherd_ptr->ready, item->thread, item->thread->rdata->shepherd_ptr);
} /*}}}*/

void INTERNAL qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job)
{   /*{{{*/
    qt_blocking_queue_node_t *prev;

    qthread_debug(IO_FUNCTIONS, "entering, job = %p\n", job);
    assert(job->next == NULL);
    QTHREAD_LOCK(&theQueue.lock);
    qthread_debug(IO_DETAILS, "1) theQueue.head = %p, .tail = %p, job = %p\n", theQueue.head, theQueue.tail, job);
    prev          = theQueue.tail;
    theQueue.tail = job;
    if (prev == NULL) {
        theQueue.head = job;
    } else {
        prev->next = job;
    }
    qthread_debug(IO_DETAILS, "2) theQueue.head = %p, .tail = %p, job = %p\n", theQueue.head, theQueue.tail, job);
    QTHREAD_SIGNAL(&theQueue.notempty);
    QTHREAD_UNLOCK(&theQueue.lock);
    qthread_debug(IO_FUNCTIONS, "exiting, job = %p\n", job);
} /*}}}*/

/* vim:set expandtab: */
