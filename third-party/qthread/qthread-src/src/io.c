/* System Headers */
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>    /* for fprintf() */
#include <stdlib.h>   /* for abort() */
#include <sys/time.h> /* for gettimeofday() */
/* - syscall(2) */
#include <sys/syscall.h>
#include <unistd.h>
/* - accept(2) */
#include <sys/socket.h>
/* - connect(2) */
#include <sys/socket.h>
#include <sys/types.h>
/* - nanosleep(2) */
#include <time.h>
/* - poll(2) */
#include <poll.h>
/* - read(2) */
#include <sys/uio.h>
/* - select(2) */
#include <sys/select.h>
/* - wait4(2) */
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_envariables.h"
#include "qt_io.h"
#include "qt_macros.h"
#include "qt_subsystems.h"
#include "qt_threadqueues.h"
#include "qthread_innards.h" /* for qthread_exec() */

#define NSEC_PER_SEC 1000000000
#define NSEC_PER_USEC 1000
#define DEFAULT_TIMEOUT 1000

typedef struct {
  qt_blocking_queue_node_t *head;
  qt_blocking_queue_node_t *tail;
  saligned_t length;
  pthread_mutex_t lock;
  pthread_cond_t notempty;
} qt_blocking_queue_t;

static qt_blocking_queue_t theQueue;
static saligned_t _Atomic io_worker_count = -1;
static saligned_t io_worker_max = 10;
qt_mpool syscall_job_pool = NULL;
static unsigned long timeout = DEFAULT_TIMEOUT; // in microseconds
static int _Atomic proxy_exit = 0;
TLS_DECL_INIT(qthread_t *, IO_task_struct);

static void qt_blocking_subsystem_internal_stopwork(void) {
  atomic_store_explicit(&proxy_exit, 1, memory_order_relaxed);
  MACHINE_FENCE;
  while (atomic_load_explicit(&io_worker_count, memory_order_relaxed))
    SPINLOCK_BODY();
  QTHREAD_LOCK(&theQueue.lock);
  QTHREAD_UNLOCK(&theQueue.lock);
}

static void qt_blocking_subsystem_internal_freemem(void) {
  qt_mpool_destroy(syscall_job_pool);
  QTHREAD_DESTROYLOCK(&theQueue.lock);
  QTHREAD_DESTROYCOND(&theQueue.notempty);
}

static void *qt_blocking_subsystem_proxy_thread(void *Q_UNUSED(arg)) {
  while (!atomic_load_explicit(&proxy_exit, memory_order_relaxed)) {
    if (qt_process_blocking_call()) { break; }
  }
  atomic_fetch_sub_explicit(&io_worker_count, 1, memory_order_relaxed);
  pthread_exit(NULL);
  return 0;
}

static void qt_blocking_subsystem_spawnworker(void) {
  int r;
  pthread_t thr;

  if ((r = pthread_create(
         &thr, NULL, qt_blocking_subsystem_proxy_thread, NULL)) != 0) {
    fprintf(
      stderr, "qt_blocking_subsystem_init: pthread_create() failed (%d)\n", r);
    perror("qt_blocking_subsystem_init spawning proxy thread");
    abort();
  }
  atomic_fetch_add_explicit(&io_worker_count, 1, memory_order_relaxed);
  pthread_detach(thr);
}

void INTERNAL qt_blocking_subsystem_init(void) {
  syscall_job_pool = qt_mpool_create(sizeof(qt_blocking_queue_node_t));
  theQueue.head = NULL;
  theQueue.tail = NULL;
  atomic_store_explicit(&io_worker_count, 0, memory_order_relaxed);
  io_worker_max = qt_internal_get_env_num("MAX_IO_WORKERS", 10, 1);
  timeout =
    qt_internal_get_env_num("IO_TIMEOUT", DEFAULT_TIMEOUT, DEFAULT_TIMEOUT);
  TLS_INIT(IO_task_struct);
  qassert(pthread_mutex_init(&theQueue.lock, NULL), 0);
  qassert(pthread_cond_init(&theQueue.notempty, NULL), 0);
  /* thread(s) must be stopped *before* shepherds die, to keep them from
   * trying to push orphan threads into shepherd queues */
  qthread_internal_cleanup_early(qt_blocking_subsystem_internal_stopwork);
  /* must be torn down *after* shepherds die, because live shepherd might try
   * to enqueue into my queue during shutdown */
  qthread_internal_cleanup(qt_blocking_subsystem_internal_freemem);
}

int INTERNAL qt_process_blocking_call(void) {
  qt_blocking_queue_node_t *item;

  QTHREAD_LOCK(&theQueue.lock);
  while (theQueue.head == NULL) {
    struct timespec ts;
    int ret;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    time_t nsec = ts.tv_nsec + timeout * NSEC_PER_USEC;
    if (nsec < NSEC_PER_SEC) {
      ts.tv_nsec = nsec;
    } else {
      ts.tv_sec += nsec / NSEC_PER_SEC;
      ts.tv_nsec = nsec % NSEC_PER_SEC;
    }
    while (1) {
      ret = pthread_cond_timedwait(&theQueue.notempty, &theQueue.lock, &ts);
      // Check that time actually elapsed and that it's not a spurious wakeup.
      struct timespec ts2;
      clock_gettime(CLOCK_MONOTONIC, &ts2);
      if (ts.tv_sec <= ts2.tv_sec && ts.tv_nsec <= ts2.tv_nsec) { break; }
    }
    switch (ret) {
      case ETIMEDOUT:
        if (theQueue.head == NULL) {
          QTHREAD_UNLOCK(&theQueue.lock);
          return 1;
        } else {
          QTHREAD_UNLOCK(&theQueue.lock);
          return 0;
        }

      case EINVAL:
        /* chances are, this is because ts is in the past */
        break;

      default: break;
    }
  }
  item = theQueue.head;
  assert(item != NULL);
  theQueue.head = item->next;
  if (theQueue.tail == item) { theQueue.tail = theQueue.head; }
  theQueue.length--;
  QTHREAD_UNLOCK(&theQueue.lock);
  item->next = NULL;
  /* do something with <item> */
  switch (item->op) {
    default: fprintf(stderr, "Unhandled syscall: %u\n", (unsigned int)item->op);
    // abort();
    case ACCEPT: {
      int socket;
      memcpy(&socket, &item->args[0], sizeof(int));
      item->ret = accept(
        socket, (struct sockaddr *)item->args[1], (socklen_t *)item->args[2]);
      break;
    }
    case CONNECT: {
      int socket;
      memcpy(&socket, &item->args[0], sizeof(int));
      item->ret = connect(socket,
                          (const struct sockaddr *)item->args[1],
                          (socklen_t)item->args[2]);
      break;
    }
    case POLL: {
      nfds_t nfds;
      int timeout;
      memcpy(&nfds, &item->args[1], sizeof(nfds_t));
      memcpy(&timeout, &item->args[2], sizeof(int));
      item->ret = poll((struct pollfd *)item->args[0], nfds, timeout);
      break;
    }
    case READ: {
      int fd;
      memcpy(&fd, &item->args[0], sizeof(int));
      item->ret = read(fd, (void *)item->args[1], (size_t)item->args[2]);
      break;
    }
    case PREAD: {
      int fd;
      off_t offset;
      memcpy(&fd, &item->args[0], sizeof(int));
      memcpy(&offset, &item->args[3], sizeof(off_t));
      item->ret =
        pread(fd, (void *)item->args[1], (size_t)item->args[2], offset);
      break;
    }
    /* case RECV:
     * case RECVFROM: */
    case SELECT: {
      int nfds;
      memcpy(&nfds, &item->args[0], sizeof(int));
      item->ret = select(nfds,
                         (fd_set *)item->args[1],
                         (fd_set *)item->args[2],
                         (fd_set *)item->args[3],
                         (struct timeval *)item->args[4]);
      break;
    }
    /* case SEND:
     * case SENDTO: */
    /* case SIGWAIT: */
    case SYSTEM: item->ret = system((char const *)item->args[0]); break;
    case WAIT4: {
      pid_t pid;
      int options;
      memcpy(&pid, &item->args[0], sizeof(pid_t));
      memcpy(&options, &item->args[2], sizeof(int));
      item->ret = wait4(
        pid, (int *)item->args[1], options, (struct rusage *)item->args[3]);
      break;
    }
    case WRITE:
      item->ret = write(
        (int)item->args[0], (void const *)item->args[1], (size_t)item->args[2]);
    case PWRITE:
      item->ret = pwrite((int)item->args[0],
                         (void const *)item->args[1],
                         (size_t)item->args[2],
                         (off_t)item->args[3]);
      break;
    case USER_DEFINED: {
      qt_context_t my_context;
      TLS_SET(IO_task_struct, item->thread);
      getcontext(&my_context);
      qthread_exec(item->thread, &my_context);
      // TLS_SET(IO_task_struct, NULL);
      break;
    }
  }
  /* preserve errno in item */
  item->err = errno;
  /* and now, re-queue */
  qt_threadqueue_enqueue(item->thread->rdata->shepherd_ptr->ready,
                         item->thread);
  FREE_SYSCALLJOB(item);
  return 0;
}

void INTERNAL qt_blocking_subsystem_enqueue(qt_blocking_queue_node_t *job) {
  qt_blocking_queue_node_t *prev;

  assert(job->next == NULL);
  assert(job->thread->rdata);
  QTHREAD_LOCK(&theQueue.lock);
  prev = theQueue.tail;
  theQueue.tail = job;
  if (prev == NULL) {
    theQueue.head = job;
  } else {
    prev->next = job;
  }
  theQueue.length++;
  if (atomic_load_explicit(&io_worker_count, memory_order_relaxed) <
      theQueue.length) {
    if (atomic_load_explicit(&io_worker_count, memory_order_relaxed) <
        io_worker_max) {
      qt_blocking_subsystem_spawnworker();
    }
  } else {
    QTHREAD_COND_SIGNAL(theQueue.notempty);
  }
  QTHREAD_UNLOCK(&theQueue.lock);
}

/* vim:set expandtab: */
