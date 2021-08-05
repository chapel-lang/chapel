/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include <sys/poll.h>

#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "ptl_ips.h"
#include "ips_proto.h"

struct ptl_rcvthread;

static void *ips_ptl_pollintr(void *recvthreadc);
static psm2_error_t rcvthread_initstats(ptl_t *ptl);
static psm2_error_t rcvthread_initsched(struct ptl_rcvthread *rcvc);

struct ptl_rcvthread {
	const psmi_context_t *context;
	const ptl_t *ptl;
	struct ips_recvhdrq *recvq;

	pthread_t hdrq_threadid;
	uint64_t t_start_cyc;
	int pipefd[2];

	/* stats and some for scheduling */
	uint64_t pollcnt;
	uint64_t pollcnt_to;
	uint64_t pollcyc;
	uint64_t pollok;

	/* For scheduling interrupt thread */
	int timeout_period_min;
	int timeout_period_max;
	int timeout_shift;
	uint64_t pollok_last;
	uint64_t pollcnt_last;
	uint32_t last_timeout;
};

#ifdef PSM_CUDA
	/* This is a global cuda context (extern declaration in psm_user.h)
         * stored to provide hints during a cuda failure
         * due to a null cuda context.
         */
	CUcontext ctxt;
#endif

/*
 * The receive thread knows about the ptl interface, so it can muck with it
 * directly.
 */
psm2_error_t ips_ptl_rcvthread_init(ptl_t *ptl_gen, struct ips_recvhdrq *recvq)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	struct ptl_rcvthread *rcvc;

	ptl->rcvthread =
	    psmi_calloc(ptl->ep, UNDEFINED, 1, sizeof(struct ptl_rcvthread));
	if (ptl->rcvthread == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	rcvc = ptl->rcvthread;

	rcvc->recvq = recvq;
	rcvc->ptl = ptl_gen;
	rcvc->context = ptl->context;
	rcvc->t_start_cyc = get_cycles();

#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED)
		PSMI_CUDA_CALL(cuCtxGetCurrent, &ctxt);
#endif

	if (psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD) &&
	    (!psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED))){

		if ((err = rcvthread_initsched(rcvc)))
			goto fail;

		/* Create a pipe so we can synchronously terminate the thread */
		if (pipe(rcvc->pipefd) != 0) {
			err = psmi_handle_error(ptl->ep, PSM2_EP_DEVICE_FAILURE,
						"Cannot create a pipe for receive thread: %s\n",
						strerror(errno));
			goto fail;
		}

		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
		if (pthread_create(&rcvc->hdrq_threadid, NULL,
				   ips_ptl_pollintr, ptl->rcvthread)) {
			close(rcvc->pipefd[0]);
			close(rcvc->pipefd[1]);
			err = psmi_handle_error(ptl->ep, PSM2_EP_DEVICE_FAILURE,
						"Cannot start receive thread: %s\n",
						strerror(errno));
			goto fail;
		}

	}

	if ((err = rcvthread_initstats(ptl_gen)))
		goto fail;

fail:
	return err;
}

psm2_error_t ips_ptl_rcvthread_fini(ptl_t *ptl_gen)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)ptl->rcvthread;
	uint64_t t_now;
	psm2_error_t err = PSM2_OK;

	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);

	if (ptl->rcvthread == NULL)
		return err;

	psmi_stats_deregister_type(PSMI_STATSTYPE_RCVTHREAD, rcvc);
	if (rcvc->hdrq_threadid && psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED)) {
		t_now = get_cycles();

		/* Disable interrupts then kill the receive thread */
		if (psmi_context_interrupt_isenabled
		    ((psmi_context_t *) ptl->context))
			if ((err =
			     psmi_context_interrupt_set((psmi_context_t *) ptl->
							context, 0)))
				goto fail;

		/* Close the pipe so we can have the thread synchronously exit.
		   On Linux just closing the pipe does not wake up the receive
		   thread.
		 */
		if (write(rcvc->pipefd[1], (const void *)&t_now,
			  sizeof(uint64_t)) == -1 ||
		    close(rcvc->pipefd[1]) == -1) {
			_HFI_VDBG
			    ("unable to close pipe to receive thread cleanly\n");
		}
		pthread_join(rcvc->hdrq_threadid, NULL);
		psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
		rcvc->hdrq_threadid = 0;
		if (_HFI_PRDBG_ON) {
			_HFI_PRDBG_ALWAYS
				("rcvthread poll success %lld/%lld times, "
				 "thread cancelled in %.3f us\n",
				(long long)rcvc->pollok, (long long)rcvc->pollcnt,
				(double)cycles_to_nanosecs(get_cycles() - t_now) / 1e3);
		}
	}

	psmi_free(ptl->rcvthread);
	ptl->rcvthread = NULL;
fail:
	return err;
}

void ips_ptl_rcvthread_transfer_ownership(ptl_t *from_ptl_gen, ptl_t *to_ptl_gen)
{
	struct ptl_rcvthread *rcvc;

	psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
	struct ptl_ips *from_ptl = (struct ptl_ips *)from_ptl_gen;
	struct ptl_ips *to_ptl   = (struct ptl_ips *)to_ptl_gen;
	to_ptl->rcvthread = from_ptl->rcvthread;
	from_ptl->rcvthread = NULL;

	rcvc = to_ptl->rcvthread;

	rcvc->recvq = &to_ptl->recvq;
	rcvc->context = to_ptl->context;
	rcvc->ptl = to_ptl_gen;
}

psm2_error_t rcvthread_initsched(struct ptl_rcvthread *rcvc)
{
	union psmi_envvar_val env_to;
	char buf[192];
	char *rcv_freq = buf;
	int no_timeout = 0;
	int tvals[3] = { RCVTHREAD_TO_MIN_FREQ,
		RCVTHREAD_TO_MAX_FREQ,
		RCVTHREAD_TO_SHIFT
	};
	snprintf(buf, sizeof(buf) - 1, "%d:%d:%d", RCVTHREAD_TO_MIN_FREQ,
		 RCVTHREAD_TO_MAX_FREQ, RCVTHREAD_TO_SHIFT);
	buf[sizeof(buf) - 1] = '\0';

	if (!psmi_getenv("PSM3_RCVTHREAD_FREQ",
			 "Recv Thread frequency (per sec) <min_freq[:max_freq[:shift_freq]]>",
			 PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			 (union psmi_envvar_val)rcv_freq, &env_to)) {
		/* not using default values */
		int nparsed = psmi_parse_str_tuples(env_to.e_str, 3, tvals);
		int invalid = 0;

		if (nparsed < 1 || (nparsed > 0 && tvals[0] == 0) ||
		    (nparsed > 1 && tvals[1] == 0)) {
			no_timeout = 1;
		} else {
			if (nparsed > 0 && tvals[0] > 1000)
				invalid = 1;
			if (nparsed > 1
			    && (tvals[1] > 1000 || tvals[1] < tvals[0]))
				invalid = 1;
			if (nparsed > 2 && tvals[2] > 10)
				invalid = 1;
		}

		if (invalid) {
			_HFI_INFO
			    ("Overriding invalid request for RcvThread frequency"
			     " settings of %s to be <%d:%d:%d>\n", env_to.e_str,
			     RCVTHREAD_TO_MIN_FREQ, RCVTHREAD_TO_MAX_FREQ,
			     RCVTHREAD_TO_SHIFT);
			tvals[0] = RCVTHREAD_TO_MIN_FREQ;
			tvals[1] = RCVTHREAD_TO_MAX_FREQ;
			tvals[2] = RCVTHREAD_TO_SHIFT;
		}
	}

	if (no_timeout) {
		rcvc->last_timeout = -1;
		_HFI_PRDBG("PSM3_RCVTHREAD_FREQ set to only interrupt "
			   "(no timeouts)\n");
	} else {
		/* Convert freq to period in milliseconds (for poll()) */
		rcvc->timeout_period_max = 1000 / tvals[0];
		rcvc->timeout_period_min = 1000 / tvals[1];
		rcvc->timeout_shift = tvals[2];
		/* Start in the middle of min and max */
		rcvc->last_timeout = (rcvc->timeout_period_min +
				      rcvc->timeout_period_max) / 2;
		_HFI_PRDBG("PSM3_RCVTHREAD_FREQ converted to period "
			   "min=%dms,max=%dms,shift=%d\n",
			   rcvc->timeout_period_min, rcvc->timeout_period_max,
			   rcvc->timeout_shift);
	}
	return PSM2_OK;
}

static
int rcvthread_next_timeout(struct ptl_rcvthread *rcvc)
{
	uint64_t pollok_diff = rcvc->pollok - rcvc->pollok_last;

	if (pollok_diff > 0) {
		if (rcvc->last_timeout > rcvc->timeout_period_min)
			/* By default, be less aggressive, but there's a more aggressive
			 * alternative if need be */
#if 1
			rcvc->last_timeout >>= rcvc->timeout_shift;
#else
			rcvc->last_timeout = rcvc->timeout_period_min;
#endif
	} else {		/* we had less progress */
		if (rcvc->last_timeout < rcvc->timeout_period_max)
			rcvc->last_timeout <<= rcvc->timeout_shift;
	}

	rcvc->pollok_last = rcvc->pollok;
	rcvc->pollcnt_last = rcvc->pollcnt;
	return (int)rcvc->last_timeout;
}

extern int ips_in_rcvthread;

static void process_async_event(psm2_ep_t ep)
{
	struct ibv_async_event async_event;
	const char* errstr = NULL;

	if (ibv_get_async_event(ep->verbs_ep.context, &async_event)) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Receive thread ibv_get_async_event() error: %s", strerror(errno));
	}
	/* Ack the event */
	ibv_ack_async_event(&async_event);

	switch (async_event.event_type) {
	case IBV_EVENT_CQ_ERR:
		if (async_event.element.cq == ep->verbs_ep.send_cq)
			errstr = "Send CQ";
		else if (async_event.element.cq == ep->verbs_ep.recv_cq)
			errstr = "Recv CQ";
		else
			errstr = "CQ";
		break;
	case IBV_EVENT_QP_FATAL:
	case IBV_EVENT_QP_REQ_ERR:
	case IBV_EVENT_QP_ACCESS_ERR:
		if (async_event.element.qp == ep->verbs_ep.qp)
			errstr = "UD QP";
		else
			errstr = "RC QP";	// qp->context will be an ipsaddr
		break;
	case IBV_EVENT_DEVICE_FATAL:
		errstr = "NIC";
		break;
	default:
		// be silent about other events
		break;
	}
	if (errstr)
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Fatal %s Async Event: %s", errstr,
				ibv_event_type_str(async_event.event_type));
}

static void rearm_cq_event(psm2_ep_t ep)
{
	struct ibv_cq *ev_cq;
	void *ev_ctx;

	_HFI_VDBG("rcvthread got solicited event\n");
	if (ibv_get_cq_event(ep->verbs_ep.recv_comp_channel, &ev_cq, &ev_ctx)) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Receive thread ibv_get_cq_event() error: %s",
			  strerror(errno));
	}

	/* Ack the event */
	ibv_ack_cq_events(ev_cq, 1);
	psmi_assert_always(ev_cq == ep->verbs_ep.recv_cq);
	psmi_assert_always(ev_ctx == ep);
	// we only use solicited, so just reenable it
	// TBD - during shutdown events get disabled and we could check
	// psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED)
	// to make sure we still want enabled.  But given verbs events
	// are one-shots, that seems like overkill
	if (ibv_req_notify_cq(ep->verbs_ep.recv_cq, 1)) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Receive thread ibv_req_notify_cq() error: %s",
			  strerror(errno));
	}
}

// poll for async events for all rails/QPs within a given end user opened EP
static void poll_async_events(psm2_ep_t ep)
{
	struct pollfd pfd[PSMI_MAX_QPS];
	psm2_ep_t pep[PSMI_MAX_QPS];
	int num_ep = 0;
	psm2_ep_t first;
	int ret;
	int i;

	first = ep;
	do {
#ifdef RNDV_MOD
		if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)
		    && __psm2_rv_cq_overflowed(ep->verbs_ep.rv))
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "RV event ring overflow");
#endif
		pfd[num_ep].fd = ep->verbs_ep.context->async_fd;
		pfd[num_ep].events = POLLIN;
		pfd[num_ep].revents = 0;
		pep[num_ep++] = ep;
		ep = ep->mctxt_next;
	} while (ep != first);

	ret = poll(pfd, num_ep, 0);
	if_pf(ret < 0) {
		if (errno == EINTR)
			_HFI_DBG("got signal, keep polling\n");
		else
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Receive thread poll() error: %s", strerror(errno));
	} else if_pf (ret > 0) {
		for (i=0; i < num_ep; i++) {
			if (pfd[i].revents & POLLIN)
				process_async_event(pep[i]);
		}
	}
}

/*
 * Receiver thread support.
 *
 * By default, polling in the driver asks the chip to generate an interrupt on
 * every packet.  When the driver supports POLLURG we can switch the poll mode
 * to one that requests interrupts only for packets that contain an urgent bit
 * (and optionally enable interrupts for hdrq overflow events).  When poll
 * returns an event, we *try* to make progress on the receive queue but simply
 * go back to sleep if we notice that the main thread is already making
 * progress.
 */
static
void *ips_ptl_pollintr(void *rcvthreadc)
{
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)rcvthreadc;
	struct ips_recvhdrq *recvq = rcvc->recvq;
	int fd_pipe = rcvc->pipefd[0];
	psm2_ep_t ep;
	struct pollfd pfd[3];
	int ret;
	int next_timeout = rcvc->last_timeout;
	uint64_t t_cyc;
	psm2_error_t err;

#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED && ctxt != NULL)
		PSMI_CUDA_CALL(cuCtxSetCurrent, ctxt);
#endif

	PSM2_LOG_MSG("entering");
	/* No reason to have many of these, keep this as a backup in case the
	 * recvhdrq init function is misused */
	psmi_assert_always(psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED));

	/* Switch driver to a mode where it can interrupt on urgent packets */
	if (psmi_context_interrupt_set((psmi_context_t *)
				       rcvc->context, 1) == PSM2_EP_NO_RESOURCES) {
		_HFI_PRDBG
		    ("poll_type feature not present in driver, turning "
		     "off internal progress thread\n");
		return NULL;
	}

	_HFI_PRDBG("Enabled communication thread on URG packets\n");

	while (1) {
		// pfd[0] is for urgent inbound packets (NAK, urgent ACK, etc)
		// pfd[1] is for rcvthread termination
		// pfd[2] is for verbs async events (PSM_UD only)
		// on timeout (poll() returns 0), we do background process checks
		//		for non urgent inbound packets
		pfd[0].fd = rcvc->context->ep->verbs_ep.recv_comp_channel->fd;
		pfd[0].events = POLLIN;
		pfd[0].revents = 0;
		pfd[1].fd = fd_pipe;
		pfd[1].events = POLLIN;
		pfd[1].revents = 0;
		pfd[2].fd = rcvc->context->ep->verbs_ep.context->async_fd;
		pfd[2].events = POLLIN;
		pfd[2].revents = 0;

		ret = poll(pfd, 3, next_timeout);
		t_cyc = get_cycles();
		if_pf(ret < 0) {
			if (errno == EINTR)
				_HFI_DBG("got signal, keep polling\n");
			else
				psmi_handle_error(PSMI_EP_NORETURN,
						  PSM2_INTERNAL_ERR,
						  "Receive thread poll() error: %s",
						  strerror(errno));
		} else if (pfd[1].revents) {
			/* Any type of event on this fd means exit, should be POLLHUP */
			_HFI_DBG("close thread: revents=0x%x\n", pfd[1].revents);
			close(fd_pipe);
			break;
		} else {
			// we got an async event
			if (pfd[2].revents & POLLIN)
				process_async_event(rcvc->context->ep);

			// we got here due to a CQ event (as opposed to timeout)
			// consume the event and rearm, we'll poll cq below
			if (pfd[0].revents & POLLIN)
				rearm_cq_event(rcvc->context->ep);

			rcvc->pollcnt++;
			if (!PSMI_LOCK_TRY(psmi_creation_lock)) {
				if (ret == 0 || pfd[0].revents & (POLLIN | POLLERR)) {
					if (PSMI_LOCK_DISABLED) {
						// this path is not supported.  having rcvthread
						// and PSMI_PLOCK_IS_NOLOCK define not allowed.
						// TBD - would be good if we could quickly
						// check for ep->verbs_ep.recv_wc_count == 0
						//	&& nothing on CQ without doing a ibv_poll_cq
						// ibv_poll_cq(cq, 0, NULL) always returns 0, so that
						// doesn't help
						// ibv_poll_cq would consume a CQE and require a lock so
						// must call our main recv progress function below
						// maybe if we open the can on HW verbs driver we could
						// quickly check Q without polling.  Main benefit would
						// be avoiding spinlock contention with main PSM
						// thread and perhaps using the trylock style inside
						// poll_cq much like we do for WFR
						if (!ips_recvhdrq_trylock(recvq))
							continue;
						err = ips_recvhdrq_progress(recvq);
						if (err == PSM2_OK)
							rcvc->pollok++;
						else
							rcvc->pollcyc += get_cycles() - t_cyc;
						ips_recvhdrq_unlock(recvq);
					} else {

						ep = psmi_opened_endpoint;

						/* Go through all master endpoints. */
						do{
							if (!PSMI_LOCK_TRY(ep->mq->progress_lock)) {
								/* If we time out, we service shm and NIC.
								* If not, we assume to have received an urgent
								* packet and service only NIC.
								*/
								err = psmi_poll_internal(ep,
											 ret == 0 ? PSMI_TRUE : PSMI_FALSE);

								if (err == PSM2_OK)
									rcvc->pollok++;
								else
									rcvc->pollcyc += get_cycles() - t_cyc;
								PSMI_UNLOCK(ep->mq->progress_lock);
							}
							poll_async_events(ep);

							/* get next endpoint from multi endpoint list */
							ep = ep->user_ep_next;
						} while(NULL != ep);
					}
				}
				PSMI_UNLOCK(psmi_creation_lock);
			}
			if (ret == 0) { /* change timeout only on timed out poll */
				rcvc->pollcnt_to++;
				next_timeout = rcvthread_next_timeout(rcvc);
			}
		}
	}

	PSM2_LOG_MSG("leaving");
	return NULL;
}

static uint64_t rcvthread_stats_pollok(void *context)
{
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)context;
	//double ratio = 0.0;
	uint64_t ratio_u = 0;
	if (rcvc->pollcnt > 0)
		//ratio = (double)rcvc->pollok * 100.0 / rcvc->pollcnt;
		ratio_u = (uint64_t)((double)rcvc->pollok * 100.0 / rcvc->pollcnt);
	//memcpy(&ratio_u, &ratio, sizeof(uint64_t));
	return ratio_u;
}

static uint64_t rcvthread_stats_pollcyc(void *context)
{
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)context;
	/* log in milliseconds */
	return (uint64_t) ((double)cycles_to_nanosecs(rcvc->pollcyc) / 1.0e6);
}

static psm2_error_t rcvthread_initstats(ptl_t *ptl_gen)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)ptl->rcvthread;
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECLU64("intrthread_schedule_count", &rcvc->pollcnt),
		PSMI_STATS_DECL("intrthread_schedule_success_(%)",
				MPSPAWN_STATS_REDUCTION_ALL,
				rcvthread_stats_pollok, NULL),
		PSMI_STATS_DECLU64("intrthread_timeout_count", &rcvc->pollcnt_to),
		PSMI_STATS_DECL("intrthread_wasted_time_(ms)",
				MPSPAWN_STATS_REDUCTION_ALL,
				rcvthread_stats_pollcyc, NULL)
	};

	/* If we don't want a thread, make sure we still initialize the counters
	 * but set them to NaN instead */
	if (!psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED)) {
		int i;
		static uint64_t ctr_nan = MPSPAWN_NAN;
		for (i = 0; i < (int)PSMI_STATS_HOWMANY(entries); i++) {
			entries[i].getfn = NULL;
			entries[i].u.val = &ctr_nan;
		}
	}

	return psmi_stats_register_type("RcvThread_statistics",
					PSMI_STATSTYPE_RCVTHREAD,
					entries,
					PSMI_STATS_HOWMANY(entries), ptl->ep->epid, rcvc);
}
