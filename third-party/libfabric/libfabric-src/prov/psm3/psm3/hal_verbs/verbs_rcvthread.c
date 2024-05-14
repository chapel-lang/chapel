#ifdef PSM_VERBS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2021 Intel Corporation.

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

  Copyright(c) 2021 Intel Corporation.

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

/* Copyright (c) 2003-2021 Intel Corporation. All rights reserved. */

#include <sys/poll.h>

#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "ptl_ips.h"
#include "ips_proto.h"
#include "verbs_hal.h"

static void psm3_verbs_process_async_event(psm2_ep_t ep)
{
	struct ibv_async_event async_event;
	const char* errstr = NULL;

	if (ibv_get_async_event(ep->verbs_ep.context, &async_event)) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Receive thread ibv_get_async_event() error on %s port %u: %s", ep->dev_name, ep->portnum, strerror(errno));
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
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Fatal %s Async Event on %s port %u: %s", errstr, ep->dev_name, ep->portnum,
				ibv_event_type_str(async_event.event_type));
}

static void psm3_verbs_rearm_cq_event(psm2_ep_t ep)
{
	struct ibv_cq *ev_cq;
	void *ev_ctx;

	_HFI_VDBG("rcvthread got solicited event\n");
	if (ibv_get_cq_event(ep->verbs_ep.recv_comp_channel, &ev_cq, &ev_ctx)) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Receive thread ibv_get_cq_event() error on %s port %u: %s",
			  ep->dev_name, ep->portnum, strerror(errno));
	}

	/* Ack the event */
	ibv_ack_cq_events(ev_cq, 1);
	psmi_assert_always(ev_cq == ep->verbs_ep.recv_cq);
	psmi_assert_always(ev_ctx == ep);
	// we only use solicited, so just reenable it
	// TBD - during shutdown events get disabled and we could check
	// psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED)
	// to make sure we still want enabled.  But given these are only
	// for PSM urgent protocol packets, that seems like overkill
	if (ibv_req_notify_cq(ep->verbs_ep.recv_cq, 1)) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			  "Receive thread ibv_req_notify_cq() error on %s port %u: %s",
			  ep->dev_name, ep->portnum, strerror(errno));
	}
}

// poll for async events for all rails/QPs within a given end user opened EP
static void psm3_verbs_poll_async_events(psm2_ep_t ep)
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
		    && psm3_rv_cq_overflowed(ep->rv))
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "RV event ring overflow for %s port %u", ep->dev_name, ep->portnum);
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
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Receive thread poll() error: %s", strerror(errno));
	} else if_pf (ret > 0) {
		for (i=0; i < num_ep; i++) {
			if (pfd[i].revents & POLLIN)
				psm3_verbs_process_async_event(pep[i]);
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
 *
 * returns:
 * 	PSM2_IS_FINALIZED - fd_pipe was closed, caller can exit rcvthread
 * 	PSM2_NO_PROGRESS - got an EINTR, need to be called again with same
 * 			next_timeout value
 * 	PSM2_TIMEOUT - poll waited full timeout, no events
 * 			caller will check *pollok to determine if work was found to do
 * 	PSM2_OK - poll found an event and processed it
 * 	PSM2_INTERNAL_ERR - unexpected error attempting poll()
 * updates counters: pollok (poll's which made progress), pollcyc (time spent
 * 	polling without finding any events), pollintr (polls woken before timeout)
 */
psm2_error_t psm3_verbs_ips_ptl_pollintr(psm2_ep_t ep,
		struct ips_recvhdrq *recvq, int fd_pipe, int next_timeout,
		uint64_t *pollok, uint64_t *pollcyc, uint64_t *pollintr)
{
	struct pollfd pfd[3];
	int ret;
	uint64_t t_cyc;
	psm2_error_t err;
	uint64_t save_pollok = *pollok;

again:
	// pfd[0] is for urgent inbound packets (NAK, urgent ACK, etc)
	// pfd[1] is for rcvthread termination
	// pfd[2] is for verbs async events
	// on timeout (poll() returns 0), we do background process checks
	//		for non urgent inbound packets
	pfd[0].fd = ep->verbs_ep.recv_comp_channel->fd;
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	pfd[1].fd = fd_pipe;
	pfd[1].events = POLLIN;
	pfd[1].revents = 0;
	pfd[2].fd = ep->verbs_ep.context->async_fd;
	pfd[2].events = POLLIN;
	pfd[2].revents = 0;

	ret = poll(pfd, 3, next_timeout);
	t_cyc = get_cycles();
	if_pf(ret < 0) {
		if (errno == EINTR) {
			_HFI_DBG("got signal, keep polling\n");
			return PSM2_OK_NO_PROGRESS;
		} else {
			psm3_handle_error(PSMI_EP_NORETURN,
					  PSM2_INTERNAL_ERR,
					  "Receive thread poll() error: %s",
					  strerror(errno));
			return PSM2_INTERNAL_ERR;
		}
	} else if (pfd[1].revents) {
		/* Any type of event on this fd means exit, should be POLLHUP */
		_HFI_DBG("close thread: revents=0x%x\n", pfd[1].revents);
		close(fd_pipe);
		return PSM2_IS_FINALIZED;
	} else {
		(*pollintr) += ret;
		// we got an async event
		if (pfd[2].revents & POLLIN)
			psm3_verbs_process_async_event(ep);

		// we got here due to a CQ event (as opposed to timeout)
		// consume the event and rearm, we'll poll cq below
		if (pfd[0].revents & POLLIN)
			psm3_verbs_rearm_cq_event(ep);
		// The LOCK_TRY avoids a deadlock when ep destruction has
		// creation_lock, writes fd_pipe and needs to wait for this
		// thread to exit.  For psm3_wait() we must process the event
		// while here and re-establish the poll_type so get future interrupts.
		// So if can't get creation_lock, poll() again with short timeout
		// to catch EP and progress thread destruction so we can do the
		// progress polling and restablish poll_type if not being shutdown.
		// when competing with psm3_wait creation_lock this can add some delay
		// but hopeflly that is rare.
		if (PSMI_LOCK_TRY(psm3_creation_lock)) {
			next_timeout = 1;
			goto again;
		}
		// must have creation_lock before check WAITING and must reestablish
		// poll_type before we drain the CQ so we don't miss any CQ events
		if (psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_WAITING))
			psm3_verbs_poll_type(PSMI_HAL_POLL_TYPE_ANYRCV, ep);

		{
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
						return PSM2_OK;
					err = psm3_verbs_recvhdrq_progress(recvq);
					if (err == PSM2_OK)
						(*pollok)++;
					else
						(*pollcyc) += get_cycles() - t_cyc;
					ips_recvhdrq_unlock(recvq);
				} else {

					ep = psm3_opened_endpoint;

					/* Go through all master endpoints. */
					do{
						if (!PSMI_LOCK_TRY(ep->mq->progress_lock)) {
							/* If we time out, we service shm and NIC.
							* If not, we assume to have received an urgent
							* packet and service only NIC.
							*/
							err = psm3_poll_internal(ep,
										 ret == 0 ? PSMI_TRUE : PSMI_FALSE, 0);
							if (err == PSM2_OK)
								(*pollok)++;
							else
								(*pollcyc) += get_cycles() - t_cyc;
							PSMI_UNLOCK(ep->mq->progress_lock);
						}
						psm3_verbs_poll_async_events(ep);

						/* get next endpoint from multi endpoint list */
						ep = ep->user_ep_next;
					} while(NULL != ep);
				}
			}
			if (psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_WAITING)
				&& save_pollok != *pollok)
				psm3_wake(psm3_opened_endpoint);	// made some progress
			PSMI_UNLOCK(psm3_creation_lock);
		}
		if (ret == 0)
			/* timed out poll */
			return PSM2_TIMEOUT;
		else
			/* found work to do */
			return PSM2_OK;
	}
}
#endif /* PSM_VERBS */
