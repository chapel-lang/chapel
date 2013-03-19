/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_thread.c,v $
 *     $Date: 2012/05/11 21:19:14 $
 * $Revision: 1.15 $
 * Description: GASNet vapi/ibv conduit implementation, progress thread logic
 * Copyright 2012, LBNL
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

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

GASNETI_INLINE(gasnetc_testcancel)
void gasnetc_testcancel(gasnetc_progress_thread_t * const pthr_p) {
  const int save_errno = errno;
  pthread_testcancel(); /* yes, we check even if we won't call cancel ourselves */
  errno = save_errno;
  gasneti_sync_reads();
  if_pf (pthr_p->done || GASNETC_IS_EXITING()) pthread_exit(NULL);
}

static void * gasnetc_progress_thread(void *arg)
{
  gasnetc_progress_thread_t * const pthr_p  = arg;
  const gasnetc_hca_hndl_t hca_hndl         = pthr_p->hca;
  const gasnetc_cq_hndl_t cq_hndl           = pthr_p->cq;
  const gasnetc_comp_handler_t compl_hndl   = pthr_p->compl;
  void (* const fn)(gasnetc_wc_t *, void *) = pthr_p->fn;
  void * const fn_arg                       = pthr_p->fn_arg;
  const uint64_t min_us                     = pthr_p->min_us;

  my_cleanup_push(my_cleanup, fn_arg);
  my_cancel_deferred();
  my_cancel_enable();

  while (!pthr_p->done) {
    gasnetc_wc_t comp;
    int rc;

  #if GASNET_CONDUIT_VAPI
    rc = EVAPI_poll_cq_block(hca_hndl, cq_hndl, 0, &comp);
  #else
    rc = ibv_poll_cq(cq_hndl, 1, &comp);
  #endif
    if (rc == GASNETC_POLL_CQ_OK) {
      gasneti_assert((comp.opcode == GASNETC_WC_RECV) ||
		     (comp.status != GASNETC_WC_SUCCESS));
      my_cancel_disable();
      (fn)(&comp, fn_arg);
      my_cancel_enable();

      /* Throttle thread's rate */
      if_pf (min_us) {
        uint64_t prev = pthr_p->prev_time;
        if_pt (prev) {
          uint64_t elapsed = gasneti_ticks_to_us(gasneti_ticks_now() - prev);
    
          while (elapsed < min_us) {
          #if HAVE_USLEEP
            uint64_t us_delay = (min_us - elapsed);
            usleep(us_delay);
          #elif HAVE_NANOSLEEP
            uint64_t ns_delay = 1000 * (min_us - elapsed);
            struct timespec ts = { ns_delay / 1000000000L, us_delay % 1000000000L };
            nanosleep(&ts, NULL);
          #elif HAVE_NSLEEP
            uint64_t ns_delay = 1000 * (min_us - elapsed);
            struct timespec ts = { ns_delay / 1000000000L, us_delay % 1000000000L };
            nsleep(&ts, NULL);
          #else
            gasneti_yield();
          #endif
            /* {u,n,nano}sleep could have been interrupted */
            gasnetc_testcancel(pthr_p);
            elapsed = gasneti_ticks_to_us(gasneti_ticks_now() - prev);
          }
        }
        pthr_p->prev_time = gasneti_ticks_now();
      }
    } else if (rc == GASNETC_POLL_CQ_EMPTY) {
    #if GASNET_CONDUIT_VAPI
      continue; /* false wake up - loop again */
    #else
      gasnetc_cq_hndl_t the_cq;
      void *the_ctx;

      /* block for event on the empty CQ */
      rc = ibv_get_cq_event(compl_hndl, &the_cq, &the_ctx);
      if_pf (0 != rc) {
        gasnetc_testcancel(pthr_p);
        GASNETC_VAPI_CHECK(rc, "while blocked for CQ event");
        /* Not reached */
      }
      gasneti_assert(the_cq == cq_hndl);

      /* ack the event and rearm */
      ibv_ack_cq_events(cq_hndl, 1);
      rc = ibv_req_notify_cq(cq_hndl, 0);
      if_pf (0 != rc) {
        gasnetc_testcancel(pthr_p);
        GASNETC_VAPI_CHECK(rc, "while requesting CQ events");
        /* Not reached */
      }

      /* loop to poll for the new completion */
    #endif
    } else {
      gasnetc_testcancel(pthr_p);
    #if GASNET_CONDUIT_VAPI
      GASNETC_VAPI_CHECK(rc, "from EVAPI_poll_cq_block");
    #else
      GASNETC_VAPI_CHECK(rc, "from ibv_poll_cq in async thread");
    #endif
      /* Not reached */
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
    gasnett_maximize_rlimit(RLIMIT_STACK);
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
    GASNETI_TRACE_PRINTF(I, ("Stack size for progress thread(s) set to %lu%s",
                              (unsigned long)stack_sz,
                              (stack_sz == stack_dflt) ? " (default)" : ""));
  }
  gasneti_mutex_unlock(&init_lock);

  (void)pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); /* ignore failures */
  gasneti_assert_zeroret(pthread_attr_setstacksize(&attr, stack_sz));
  gasneti_assert_zeroret(pthread_create(&pthr_p->thread_id, &attr, gasnetc_progress_thread, pthr_p));
  gasneti_assert_zeroret(pthread_attr_destroy(&attr));
  GASNETI_TRACE_PRINTF(I, ("Spawned progress thread with id 0x%lx",
                           (unsigned long)(uintptr_t)(pthr_p->thread_id)));
}

extern void
gasnetc_stop_progress_thread(gasnetc_progress_thread_t *pthr_p)
{
  if (pthread_self() == pthr_p->thread_id) return; /* no suicides */
  if (pthr_p->done) return; /* no "over kill" */
  pthr_p->done = 1;
  gasneti_sync_writes();
#if GASNET_CONDUIT_VAPI
  (void) EVAPI_poll_cq_unblock(pthr_p->hca, pthr_p->cq);
#endif
#if GASNETC_THREAD_CANCEL
  (void)pthread_cancel(pthr_p->thread_id); /* ignore failure */
#endif
  GASNETI_TRACE_PRINTF(I, ("Requested termination of progress thread with id 0x%lx",
                           (unsigned long)(uintptr_t)(pthr_p->thread_id)));
}
#endif
