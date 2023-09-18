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
	psm2_ep_t ep;
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

#ifdef PSM_DSA
// we only create one per process, can save here for read/compare only
pthread_t psm3_rcv_threadid;
#endif

#ifdef PSM_CUDA
	/* This is a global cuda context (extern declaration in psm_user.h)
         * stored to provide hints during a cuda failure
         * due to a null cuda context.
         */
	CUcontext cu_ctxt;
#endif

/*
 * The receive thread knows about the ptl interface, so it can muck with it
 * directly.
 */
psm2_error_t psm3_ips_ptl_rcvthread_init(ptl_t *ptl_gen, struct ips_recvhdrq *recvq)
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

	rcvc->ep = ptl->ep;
	rcvc->recvq = recvq;
	rcvc->ptl = ptl_gen;
	rcvc->t_start_cyc = get_cycles();

#ifdef PSM_CUDA
	if (PSMI_IS_GPU_ENABLED)
		PSMI_CUDA_CALL(cuCtxGetCurrent, &cu_ctxt);
#endif

	if (psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD) &&
	    (!psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED))){

		if ((err = rcvthread_initsched(rcvc)))
			goto fail;

		/* Create a pipe so we can synchronously terminate the thread */
		if (pipe(rcvc->pipefd) != 0) {
			err = psm3_handle_error(ptl->ep, PSM2_EP_DEVICE_FAILURE,
						"Cannot create a pipe for receive thread: %s\n",
						strerror(errno));
			goto fail;
		}

		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
		if (pthread_create(&rcvc->hdrq_threadid, NULL,
				   ips_ptl_pollintr, ptl->rcvthread)) {
			close(rcvc->pipefd[0]);
			close(rcvc->pipefd[1]);
			err = psm3_handle_error(ptl->ep, PSM2_EP_DEVICE_FAILURE,
						"Cannot start receive thread: %s\n",
						strerror(errno));
			goto fail;
		}
#ifdef PSM_DSA
		psm3_rcv_threadid = rcvc->hdrq_threadid;
#endif
		if ((err = rcvthread_initstats(ptl_gen)))
			goto fail;
	}


fail:
	return err;
}

psm2_error_t psm3_ips_ptl_rcvthread_fini(ptl_t *ptl_gen)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)ptl->rcvthread;
	uint64_t t_now;
	psm2_error_t err = PSM2_OK;

	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);

	if (ptl->rcvthread == NULL)
		return err;

	psm3_stats_deregister_type(PSMI_STATSTYPE_RCVTHREAD, rcvc);
	if (rcvc->hdrq_threadid && psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED)) {
		t_now = get_cycles();

		/* Disable interrupts then kill the receive thread */
		if (psm3_context_interrupt_isenabled(ptl->ep))
			if ((err =
			     psm3_context_interrupt_set(ptl->ep, 0)))
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
		_HFI_PRDBG("rcvthread poll success %lld/%lld times, "
				 "thread cancelled in %.3f us\n",
				(long long)rcvc->pollok, (long long)rcvc->pollcnt,
				(double)cycles_to_nanosecs(get_cycles() - t_now) / 1e3);
	}

	psmi_free(ptl->rcvthread);
	ptl->rcvthread = NULL;
fail:
	return err;
}

/* This is never actually called since we track rcvthread_is_enabled per HAL
 * but if it was called it might have a bug since we don't
 * psm3_context_interrupt_set for the new owning ep, nor do we unset
 * interrupt for the old owning ep.
 */
void psm3_ips_ptl_rcvthread_transfer_ownership(ptl_t *from_ptl_gen, ptl_t *to_ptl_gen)
{
	struct ptl_rcvthread *rcvc;

	psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
	struct ptl_ips *from_ptl = (struct ptl_ips *)from_ptl_gen;
	struct ptl_ips *to_ptl   = (struct ptl_ips *)to_ptl_gen;
	to_ptl->rcvthread = from_ptl->rcvthread;
	from_ptl->rcvthread = NULL;

	rcvc = to_ptl->rcvthread;

	rcvc->recvq = &to_ptl->recvq;
	rcvc->ep = to_ptl->ep;
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

	if (!psm3_getenv("PSM3_RCVTHREAD_FREQ",
			 "Recv Thread frequency (per sec) <min_freq[:max_freq[:shift_freq]]>",
			 PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			 (union psmi_envvar_val)rcv_freq, &env_to)) {
		/* not using default values */
		int nparsed = psm3_parse_str_tuples(env_to.e_str, 3, tvals);
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
	int fd_pipe = rcvc->pipefd[0];
	int next_timeout = rcvc->last_timeout;
	psm2_error_t err;

#ifdef PSM_CUDA
	if (PSMI_IS_GPU_ENABLED && cu_ctxt != NULL)
		PSMI_CUDA_CALL(cuCtxSetCurrent, cu_ctxt);
#endif

	PSM2_LOG_MSG("entering");
	/* No reason to have many of these, keep this as a backup in case the
	 * recvhdrq init function is misused */
	psmi_assert_always(psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED));

	/* Switch driver to a mode where it can interrupt on urgent packets */
	if (psm3_context_interrupt_set(rcvc->ep, 1) == PSM2_EP_NO_RESOURCES) {
		_HFI_PRDBG
		    ("poll_type feature not present in driver, turning "
		     "off internal progress thread\n");
		return NULL;
	}

	_HFI_PRDBG("Enabled communication thread on URG packets\n");

	while (1) {
		err = psmi_hal_ips_ptl_pollintr(rcvc->ep,
					rcvc->recvq, fd_pipe, next_timeout,
					&rcvc->pollok, &rcvc->pollcyc);
		if (err == PSM2_IS_FINALIZED)
			break;
		if (err == PSM2_OK_NO_PROGRESS)
			continue;
		rcvc->pollcnt++;
		if (err == PSM2_TIMEOUT) {
			/* change timeout only on timed out poll */
			rcvc->pollcnt_to++;
			next_timeout = rcvthread_next_timeout(rcvc);
		} else if (err != PSM2_OK) {
			psm3_handle_error(PSMI_EP_NORETURN,
                                                  PSM2_INTERNAL_ERR,
                                                  "Unexpected Receive thread HAL poll() error: %s",
                                                  psm3_error_get_string(err));
			break;

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
		for (i = 0; i < (int)PSMI_HOWMANY(entries); i++) {
			entries[i].getfn = NULL;
			entries[i].u.val = &ctr_nan;
		}
	}

	// one rcvThread per process, so omit id (ptl->ep->epid) and
	// info (ptl->ep->dev_name)
	return psm3_stats_register_type("RcvThread_statistics",
					PSMI_STATSTYPE_RCVTHREAD,
					entries,
					PSMI_HOWMANY(entries), NULL, rcvc, NULL);
}
