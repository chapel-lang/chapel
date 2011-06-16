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

typedef struct {
    qt_blocking_queue_node_t *head;
    qt_blocking_queue_node_t *tail;
    pthread_mutex_t           lock;
    pthread_cond_t            notempty;
} qt_blocking_queue_t;

static qt_blocking_queue_t theQueue;
qt_mpool                   syscall_job_pool = NULL;
static pthread_t           proxy_thread;
static volatile int        proxy_exit = 0;

static void qt_blocking_subsystem_internal_teardown(void)
{   /*{{{*/
    proxy_exit = 1;
    pthread_join(proxy_thread, NULL);
    qt_mpool_destroy(syscall_job_pool);
    QTHREAD_DESTROYLOCK(&theQueue.lock);
    QTHREAD_DESTROYCOND(&theQueue.notempty);
} /*}}}*/

static void *qt_blocking_subsystem_proxy_thread(void *arg)
{   /*{{{*/
    while (proxy_exit == 0) {
        qt_process_blocking_calls();
    }
    pthread_exit(NULL);
    return 0;
} /*}}}*/

void INTERNAL qt_blocking_subsystem_init(void)
{   /*{{{*/
    syscall_job_pool = qt_mpool_create(sizeof(qt_blocking_queue_node_t));
    theQueue.head    = NULL;
    theQueue.tail    = NULL;
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
    qthread_internal_cleanup_early(qt_blocking_subsystem_internal_teardown);
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
    QTHREAD_UNLOCK(&theQueue.lock);
    item->next = NULL;
    /* do something with <item> */
    switch(item->op) {
        default:
            fprintf(stderr, "Unhandled syscall: %u\n", (unsigned int)item->op);
            abort();
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
#endif
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
#endif
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
#endif /* if HAVE_DECL_SYS_PREAD */
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
#endif /* if HAVE_DECL_SYS_SELECT */
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
#endif
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
            qthread_debug(THREAD_DETAILS, "blocking proxy context is %p\n", &my_context);
            qthread_exec(item->thread, &my_context);
            qthread_debug(THREAD_DETAILS, "proxy back from qthread_exec\n");
            qt_mpool_free(syscall_job_pool, item);
            break;
        }
    }
    /* and now, re-queue */
    qt_threadqueue_enqueue(item->thread->rdata->shepherd_ptr->ready, item->thread, item->thread->rdata->shepherd_ptr);
} /*}}}*/

void INTERNAL qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job)
{   /*{{{*/
    qt_blocking_queue_node_t *prev;

    QTHREAD_LOCK(&theQueue.lock);
    prev          = theQueue.tail;
    theQueue.tail = job;
    if (prev == NULL) {
        theQueue.head = job;
    } else {
        prev->next = job;
    }
    QTHREAD_SIGNAL(&theQueue.notempty);
    QTHREAD_UNLOCK(&theQueue.lock);
} /*}}}*/

/* vim:set expandtab: */
