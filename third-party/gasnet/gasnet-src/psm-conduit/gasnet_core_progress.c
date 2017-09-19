/*
 * Description: psm2 progress polling code
 * Copyright (c) 2014-2015 Intel Corporation. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_core_internal.h>

#include <unistd.h>

static int64_t gasnetc_rcv_thread_rate = 1000;


GASNETI_COLD
void gasnetc_do_exit(void)
{
    {  /* ensure only one thread ever continues past this point */
        static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
        gasneti_mutex_lock(&exit_lock);
    }

    /* Try for upto 5s to obtain the lock */
    const uint64_t timeout_ns = 5 * 1000000000L;
    const gasneti_tick_t t_start = gasneti_ticks_now();
    while (gasneti_ticks_to_ns(gasneti_ticks_now() - t_start) < timeout_ns) {
      if (!GASNETC_PSM_TRYLOCK()) break;
      gasneti_sched_yield();
    }

    psm2_ep_close(gasnetc_psm_state.ep, PSM2_EP_CLOSE_GRACEFUL,
             (int64_t)(gasnetc_psm_state.exit_timeout * 1000000000L));
    psm2_finalize();

    alarm(60); /* in case bootstrapFini() hangs, for instance in a barrier */
    gasneti_bootstrapFini();
    gasneti_killmyprocess(gasnetc_psm_state.exit_code);
}

GASNETI_HOT
extern int gasnetc_AMPoll(void)
{
    int ret;
#if GASNET_PSHM
    if(GASNETC_PSM_PSHM_TRYLOCK() == 0) {
        gasneti_AMPSHMPoll(0);
        GASNETC_PSM_PSHM_UNLOCK();
    }
#endif

    ret = GASNETC_PSM_TRYLOCK();
    if(ret == 0) {
        psm2_poll(gasnetc_psm_state.ep);

        /* The branches protecting these calls are an optimization:
           if there is no progress to be done, avoid a function call. */
        if(gasnetc_psm_state.pending_ack.head.next != NULL) {
            gasnete_post_pending_ack();
        }

        if(gasnetc_psm_state.posted_reqs_length > 0) {
            gasnete_finish_mq_reqs();
        }

        if(gasnetc_psm_state.pending_mq_ops.head.next != NULL) {
            gasnete_post_pending_mq_ops();
        }

        GASNETC_PSM_UNLOCK();
    }

    if_pf (gasnetc_psm_state.should_exit && !gasnetc_psm_state.exit_in_progress) {
        gasnetc_do_exit();
    }

    return GASNET_OK;
}

GASNETI_HOT
void *gasnetc_progress_thread(void * arg)
{
    char* env_val;
    uint64_t ns_delay;

    if(gasnetc_rcv_thread_rate == 0)
        ns_delay = 0; /* No rate limit */
    else
        ns_delay = 1000000000L / gasnetc_rcv_thread_rate;

    while(1) {
        if(gasneti_nsleep(ns_delay) != 0) {
            pthread_exit(0);
        }

#if GASNET_PSHM
        if(GASNETC_PSM_PSHM_TRYLOCK() == 0) {
            gasneti_AMPSHMPoll(0);
            GASNETC_PSM_PSHM_UNLOCK();
        }
#endif

        if(gasnetc_psm_state.pending_mq_ops.head.next != NULL &&
            GASNETC_PSM_TRYLOCK() == 0) {
            /* This progress thread needs to post pending MQ ops.
               The psm2-level thread will run handlers that queue
               ops, but cannot actually post them.  The app thread
               can post them, but doing it here provides passive
               progress. */
            gasnete_post_pending_mq_ops();
            GASNETC_PSM_UNLOCK();
        }

        if(gasnetc_psm_state.pending_ack.head.next != NULL &&
            GASNETC_PSM_TRYLOCK() == 0) {
            gasnete_post_pending_ack();
            GASNETC_PSM_UNLOCK();
        }

    }

    return NULL;
}


GASNETI_COLD
int gasnetc_progress_thread_init(void)
{
    pthread_t gasnetc_pthread;
    pthread_attr_t attr;
    size_t def_stacksize = PTHREAD_STACK_MIN;
    size_t env_stack_min;
    size_t env_stack_pad;
    size_t stacksize;
    int ret;

    int enabled =
        gasneti_getenv_yesno_withdefault("GASNET_RCV_THREAD", 1);

    if(!enabled)
        return GASNET_OK;

    gasnetc_rcv_thread_rate = gasneti_getenv_int_withdefault(
            "GASNET_RCV_THREAD_RATE", gasnetc_rcv_thread_rate, 0);


    pthread_attr_init(&attr);

    pthread_attr_getstacksize(&attr, &def_stacksize);

    env_stack_min = gasneti_getenv_int_withdefault(
            "GASNET_THREAD_STACK_MIN", 0, 1);
    env_stack_pad = gasneti_getenv_int_withdefault(
            "GASNET_THREAD_STACK_PAD", 0, 1);

    stacksize = MAX(env_stack_min, env_stack_pad + def_stacksize);

    ret = pthread_attr_setstacksize(&attr, stacksize);
    if(ret) {
        char s[255] = {0};
        snprintf(s, sizeof(s),
                "Error setting progress thread stack size: %s",
                strerror(ret));
        GASNETI_RETURN_ERRR(BAD_ARG, s);
    }

    ret = pthread_create(&gasnetc_pthread, NULL,
            gasnetc_progress_thread, NULL);
    if(ret) {
        char s[255] = {0};
        snprintf(s, sizeof(s),
                "Error creating progress thread: %s",
                strerror(ret));
        GASNETI_RETURN_ERRR(RESOURCE, s);
    }

    return GASNET_OK;
}

