/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core_thread.c $
 * Description: GASNet ibv conduit implementation, progress thread logic
 * Copyright 2012, LBNL
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* If too many problems, one can disable here. */
#ifndef GASNETC_THREAD_CANCEL
#define GASNETC_THREAD_CANCEL 1
#endif

#if !GASNETI_CONDUIT_THREADS

/* Protect against compiler warnings about "empty compilation unit" */
int gasnetc_thread_dummy = 1;

#else

#if GASNETC_DEBUG_PTHR
  static void my_cleanup(void *arg) {
    gasnetc_progress_thread_t * const pthr_p = arg;
    fprintf(stderr, "@%d> thread w/ fn_arg=%p terminated\n", gasneti_mynode, pthr_p->fn_arg);
  }
  #define my_cleanup_push pthread_cleanup_push
  #define my_cleanup_pop  pthread_cleanup_pop
#else
  #define my_cleanup_push(f,a)  ((void)0)
  #define my_cleanup_pop(e)     ((void)0)
#endif

#if GASNETC_THREAD_CANCEL && defined(PTHREAD_CANCEL_ENABLE)
  #define my_cancel_enable()  (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)
  #define my_cancel_disable() (void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)
#else
  #define my_cancel_enable()  ((void)0)
  #define my_cancel_disable() ((void)0)
#endif

#if GASNETC_THREAD_CANCEL && defined(PTHREAD_CANCEL_DEFERRED)
  #define my_cancel_deferred() (void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL)
#else
  #define my_cancel_deferred() ((void)0)
#endif

static void * gasnetc_progress_thread(void *arg)
{
  gasnetc_progress_thread_t * const pthr_p  = arg;
  struct ibv_cq * const cq_hndl             = pthr_p->cq;
  struct ibv_comp_channel * const compl_hndl= pthr_p->compl;
  void (* const fn)(struct ibv_wc *, void *)= pthr_p->fn;
  void * const fn_arg                       = pthr_p->fn_arg;
  const uint64_t min_ns                     = pthr_p->min_ns;
  int fd = compl_hndl->fd;
  fd_set readfds;

  /* Setup completion channel for non-blocking access.
   * This way pthread_cancel() never needs to interrupt ibv calls.
   */
  if (0 > fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK)) {
    gasneti_fatalerror("Could not set ibv completion channel to non-blocking mode");
  }
  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);

  my_cleanup_push(my_cleanup, arg);
  my_cancel_deferred();
  my_cancel_disable();

  while (!pthr_p->done) {
    struct ibv_wc comp;
    int rc;

    rc = ibv_poll_cq(cq_hndl, 1, &comp);
    if (rc == 1) {
      gasneti_assert((comp.opcode == IBV_WC_RECV) ||
		     (comp.status != IBV_WC_SUCCESS));
      (fn)(&comp, fn_arg);

      /* Throttle thread's rate */
      if_pf (min_ns) {
        uint64_t prev = pthr_p->prev_time;
        if_pt (prev) {
          uint64_t elapsed = gasneti_ticks_to_ns(gasneti_ticks_now() - prev);
    
          my_cancel_enable();
          while (elapsed < min_ns) {
            gasneti_nsleep(min_ns - elapsed);
            elapsed = gasneti_ticks_to_ns(gasneti_ticks_now() - prev);
          }
          pthread_testcancel();
          my_cancel_disable();
        }
        pthr_p->prev_time = gasneti_ticks_now();
      }
    } else if (rc == 0) {
      struct ibv_cq * the_cq;
      void *the_ctx;
      int rc;

      /* block for event on the empty CQ */
      my_cancel_enable();
      FD_SET(fd, &readfds); /* should never *not* be set */
      do {
        rc = select(fd+1, &readfds, NULL, NULL, NULL);
        if_pf (pthr_p->done) pthread_exit(NULL);
      } while ((rc < 0) && (errno == EINTR));
      my_cancel_disable();

      /* get the cq event, ack it, and rearm */
      rc = ibv_get_cq_event(compl_hndl, &the_cq, &the_ctx);
      GASNETC_IBV_CHECK(rc, "from ibv_get_cq_event");
      gasneti_assert(the_cq == cq_hndl);
      ibv_ack_cq_events(cq_hndl, 1);
      rc = ibv_req_notify_cq(cq_hndl, 0);
      GASNETC_IBV_CHECK(rc, "from ibv_req_notify_cq");

      /* loop to poll for the new completion */
    } else {
      GASNETC_IBV_CHECK(rc, "from ibv_poll_cq in async thread");
    }
  }

  my_cleanup_pop(1);
  return NULL;
}

extern void
gasnetc_spawn_progress_thread(gasnetc_progress_thread_t *pthr_p)
{
  static gasneti_mutex_t init_lock = GASNETI_MUTEX_INITIALIZER;
  static size_t stack_sz = 0;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  gasneti_mutex_lock(&init_lock);
  if (!stack_sz) {
    /* Code to determine proper stack size */
    /* TODO: move this logic to gasnet_internal.c for use in other conduits */
    const size_t stack_min = gasneti_getenv_int_withdefault("GASNET_THREAD_STACK_MIN", 0, 1);
    const size_t stack_pad = gasneti_getenv_int_withdefault("GASNET_THREAD_STACK_PAD", 0, 1);
    size_t stack_dflt;
  #ifdef RLIMIT_STACK
    gasnett_maximize_rlimit(RLIMIT_STACK, "RLIMIT_STACK");
  #endif
    gasneti_assert_zeroret(pthread_attr_getstacksize(&attr, &stack_dflt));
    stack_sz = MAX(stack_min, stack_dflt + stack_pad);
    stack_sz = GASNETI_PAGE_ALIGNUP(stack_sz);
    if (0 != pthread_attr_setstacksize(&attr, stack_sz)) {
      /* Binary search between default (lo) and requested (hi) */
      size_t lo = stack_dflt;
      size_t hi = stack_sz;
    
      while (lo != (stack_sz = lo + GASNETI_PAGE_ALIGNUP((hi - lo) >> 1))) {
        int rc = pthread_attr_setstacksize(&attr, stack_sz);
        if (rc == 0) {
          lo = stack_sz;
        } else if (rc == EINVAL){
          hi = stack_sz;
        } else {
          /* Unknown error: silently give up and use the default */
          stack_sz = stack_dflt;
          break;
        }
      }
    }
    GASNETI_TRACE_PRINTF(I, ("Stack size for progress thread(s) set to %"PRIuPTR"%s",
                              (uintptr_t)stack_sz,
                              (stack_sz == stack_dflt) ? " (default)" : ""));
  }
  gasneti_mutex_unlock(&init_lock);

  (void)pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); /* ignore failures */
  if (stack_sz) gasneti_assert_zeroret(pthread_attr_setstacksize(&attr, stack_sz));
  gasneti_assert_zeroret(pthread_create(&pthr_p->thread_id, &attr, gasnetc_progress_thread, pthr_p));
  gasneti_assert_zeroret(pthread_attr_destroy(&attr));
  GASNETI_TRACE_PRINTF(I, ("Spawned progress thread with id 0x%"PRIxPTR,
                           (uintptr_t)(pthr_p->thread_id)));
}

extern void
gasnetc_stop_progress_thread(gasnetc_progress_thread_t *pthr_p, int block)
{
  pthread_t tid = pthr_p->thread_id;
  if (pthread_self() == tid) return; /* no suicides */
  if (pthr_p->done) return; /* no "over kill" */
  pthr_p->done = 1;
  gasneti_sync_writes();
#if GASNETC_THREAD_CANCEL
  (void)pthread_cancel(tid); /* ignore failure */
#endif
  GASNETI_TRACE_PRINTF(I, ("Requested termination of progress thread with id 0x%"PRIxPTR,
                           (uintptr_t)(tid)));
  if (block) {
    (void)pthread_join(tid, NULL);
    GASNETI_TRACE_PRINTF(I, ("Joined progress thread with id 0x%"PRIxPTR,
                             (uintptr_t)(tid)));
  } else {
    (void)pthread_detach(tid);
  }
}
#endif
