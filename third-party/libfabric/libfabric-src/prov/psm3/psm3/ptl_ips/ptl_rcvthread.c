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
	uint64_t pollintr;

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

// for psm3_wait and psm3_wake
static pthread_mutex_t     wait_mutex;
static pthread_cond_t      wait_condvar;
static int wait_signalled;

// statistics for psm3_wait and psm3_wake
static uint64_t wait_count;
static uint64_t wait_err_count;
static uint64_t wait_nosleep_avail_count;
static uint64_t wait_nosleep_signalled_count;
static uint64_t wait_sleep_til_signal_count;
static uint64_t wait_sleep_timeout_count;
static uint64_t wait_sleep_signal_count;
static uint64_t wakeup_count;


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

		pthread_cond_init(&wait_condvar, NULL);
		pthread_mutex_init(&wait_mutex, NULL);
		wait_signalled = 0;

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

	PSMI_LOCK_ASSERT(psm3_creation_lock)
	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);

	if (ptl->rcvthread == NULL)
		return err;

	psm3_stats_deregister_type(PSMI_STATSTYPE_RCVTHREAD, rcvc);
	if (rcvc->hdrq_threadid && psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED)) {
		t_now = get_cycles();

		/* Disable interrupts then kill the receive thread */
		psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_WAITING);
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

		pthread_cond_destroy(&wait_condvar);
		pthread_mutex_destroy(&wait_mutex);
		wait_signalled = 0;
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

/* parse recv thread frequency for PSM3_RCVTHREAD_FREQ"
 * format is min_freq[:max_freq[:shift_freq]]",
 * Either field can be omitted in which case default (input tvals) is used
 * for given field.
 * 0 - successfully parsed, tvals updated
 * -1 - str empty, tvals unchanged
 * -2 - syntax error, tvals may have been changed
 */
static int parse_rcvthread_freq(const char *str,
			size_t errstr_size, char errstr[],
			int tvals[3])
{
	psmi_assert(tvals);
	int ret = psm3_parse_str_tuples(str, 3, tvals);
	if (ret < 0)
		return ret;
	if (tvals[0] == 0 || tvals[1] == 0) {
		// disables receiver thread, no other checks needed
		return 0;
	}
	if (tvals[0] < 0 || tvals[0] > 1000) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " min_freq must be 0 to 1000");
		return -2;
	}
	if (tvals[1] < 0 || tvals[1] > 1000) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " max_freq must be 0 to 1000");
		return -2;
	}
	if (tvals[0] > tvals[1]) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " min_freq (%d) must be <= max_freq (%d)", tvals[0], tvals[1]);
		return -2;
	}
	if (tvals[2] < 0 || tvals[2] > 10) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " shift_freq must be 0 to 10");
		return -2;
	}
	return 0;
}

static int parse_check_rcvthread_freq(int type,
				const union psmi_envvar_val val, void *ptr,
				size_t errstr_size, char errstr[])
{
	// parser will set tvals to result, use a copy to protect input of defaults
	int tvals[3] = { ((int*)ptr)[0], ((int*)ptr)[1], ((int*)ptr)[2] };
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR_TUPLES);
	return parse_rcvthread_freq(val.e_str, errstr_size, errstr, tvals);
}


psm2_error_t rcvthread_initsched(struct ptl_rcvthread *rcvc)
{
	union psmi_envvar_val env_to;
	char rcv_freq[192];
	int tvals[3] = { RCVTHREAD_TO_MIN_FREQ,
		RCVTHREAD_TO_MAX_FREQ,
		RCVTHREAD_TO_SHIFT
	};
	snprintf(rcv_freq, sizeof(rcv_freq) - 1, "%d:%d:%d",
		 RCVTHREAD_TO_MIN_FREQ, RCVTHREAD_TO_MAX_FREQ,
		 RCVTHREAD_TO_SHIFT);

	(void)psm3_getenv_range("PSM3_RCVTHREAD_FREQ",
			 "Recv Thread frequency (per sec) <min_freq[:max_freq[:shift_freq]]>",
			 "Specified as min_freq[:max_freq[:shift_freq]]\nwhere min_freq and max_freq are polls per second\n(0 disables receiver thread)\nand 2^shift_freq is amount to multiply or divide frequency by",
			 PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR_TUPLES,
			 (union psmi_envvar_val)rcv_freq,
			 (union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
			 parse_check_rcvthread_freq, tvals, &env_to);
	if (parse_rcvthread_freq(env_to.e_str, 0, NULL, tvals) < 0) {
		// already checked, shouldn't get parse errors nor empty strings
		psmi_assert(0);
	}
	if (tvals[0] == 0 || tvals[1] == 0) {
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

// timeout is in milliseconds, <0 means infinite timeout
// returns:
// 	PSM2_OK - wait completed and some progress may have been made
// 	PSM2_TIMEOUT - wait timeout exceeded with no progress made
//	PSM2_INTERNAL_ERR - wait mode not allowed for given HAL
//	PSM2_PARAM_ERR - not allowed for use with current PSM settings/modes
// For now, progress includes both actual packet receipt as well as
// protocol timers, some of which are just status checks.
//
// For now psm3_wait does not allow multi-EP, multi-rail/QP/addr
// due to psm3_wake() gap.  Properly doing multi-EP probably also
// means migrating to a singleton model and properly fixing/removing the
// transfer_ownership of rcvc when EPs are destroyed so can ensure
// poll_type properly maintained on all affected EPs.
psm2_error_t psm3_wait(int timeout)
{
	psm2_ep_t ep;
	psm2_error_t ret = PSM2_OK;

	_HFI_VDBG("Wait for event. timeout=%d\n", timeout);
	// TBD - while psm3_wait is active, we would like a quick poll() timeout
	// because it is our only checking for PSM protocol timeouts.  However
	// poll() has probably already started, so too late to change it now.
	// Maybe when an app is using fi_wait or has selected FI_PSM3_YIELD_MODE
	// we should automatically reduce the range for rcvThread timeouts.
	// However such apps also want to compromise performance to reduce
	// CPU, so for now we leave timeouts as is and will need to do some
	// benchmarking w/various PSM3_RCVTHREAD_FREQ settings when use fi_wait
	PSMI_LOCK(psm3_creation_lock);
	wait_count++;
	// if more than 1 master EP -> error for now
	if (psm3_opened_endpoint_count != 1) {
		_HFI_DBG("wait inval: more than 1 EP\n");
		goto inval;
	}
	ep = psm3_opened_endpoint;
	// if any secondary non-master EPs (multi-rail/QP/addr) -> error for now
	if (ep->mctxt_next != ep) {
		_HFI_DBG("wait inval: more than 1 Rail/QP/addr\n");
		goto inval;
	}
	// if no rcv thread -> error
	if (!psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED)) {
		_HFI_DBG("wait inval: no rcvThread\n");
		goto inval;
	}

	// TBD - similar test in fi_wait_open or when PSM3 YIELD_MODE env parsed
	// want warning if FI_PSM3_YIELD_MODE without NIC loopback fully enabled
	// such as without ips enabled, or with shm enabled instead of NIC_LOOPBACK
	if (! psm3_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		_HFI_VDBG("wait warning: PSM3_DEVICES nic not enabled\n");
	} else if (! (((struct ptl_ips*)ep->ptl_ips.ptl)->proto.flags
					 & IPS_PROTO_FLAG_LOOPBACK)) {
		_HFI_VDBG("wait warning: PSM3_NIC_LOOPBACK not enabled\n");
	}
	if (psm3_ep_device_is_enabled(ep, PTL_DEVID_AMSH)) {
		_HFI_VDBG("wait warning: PSM3_DEVICES shm enabled\n");
	}

	// if allow multiple EPs or secondary EPs would need to do this for every EP
	// and may need a way for fi_wait to indicate which EPs it wants to wait on
	// and have per EP flags for wait mode for use in psmi_hal_ips_ptl_pollintr
	// enable events
	if (psmi_hal_poll_type(PSMI_HAL_POLL_TYPE_ANYRCV, ep) < 0) {
		_HFI_DBG("wait inval: unable to set poll_type\n");
		ret = PSM2_INTERNAL_ERR;
		goto unlock;
	}

	// this is a one shot, will be cleared when wakeup
	psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_WAITING);

	PSMI_UNLOCK(psm3_creation_lock);

	// something could have arrived before we set WAITING, check for it
	// peek will detect if anything done or anything newly arrived
	// Note psm3_poll insufficient as only checks for new arrivals
	{
		psm2_mq_req_t req;
		if (psm3_mq_ipeek2(ep->mq, &req, NULL) == PSM2_OK) {
			wait_nosleep_avail_count++;
			_HFI_VDBG("found work already avail, no sleep\n");
			return PSM2_OK;
		}
	}

	// for now simple flag, but in future if have multi-ep
	// each ep will register with rcvthread to get its fd's properly added
	// TBD how to handle multiple threads calling fi_wait, would need
	// at least 1 bit in event flag per thread so all can be truely woken
	// or maybe treat like a counting event and each gets a count?
	// but some might not be waiting?
	pthread_mutex_lock(&wait_mutex);
	if (wait_signalled) {
		// autoclear
		wait_signalled = 0;
		wait_nosleep_signalled_count++;
		_HFI_VDBG("found already signaled, no sleep\n");
	} else if (timeout < 0) {	// infinite timeout
		// Wait for condition variable to be signaled or broadcast.
		pthread_cond_wait(&wait_condvar, &wait_mutex);
		wait_signalled = 0;
		wait_sleep_til_signal_count++;
		_HFI_VDBG("slept, infinite timeout\n");
	} else {
		struct timespec wait_time;
		clock_gettime(CLOCK_MONOTONIC, &wait_time);	// current time
		wait_time.tv_sec += timeout / 1000;
		wait_time.tv_nsec += (timeout % 1000) * 1000;
		if (wait_time.tv_nsec > 1000000000) { // handle carry from nsec to sec
			wait_time.tv_sec++; 
			wait_time.tv_nsec -= 1000000000;
		}
		if (0 > pthread_cond_timedwait(&wait_condvar, &wait_mutex, &wait_time)) {
			_HFI_VDBG("slept, timeout\n");
			psmi_assert(errno == ETIMEDOUT);
			ret = PSM2_TIMEOUT;
			wait_sleep_timeout_count++;
		} else {
			_HFI_VDBG("slept, signalled before timeout. signaled=%d\n", wait_signalled);
			wait_signalled = 0;
			wait_sleep_signal_count++;
		}
	}
	pthread_mutex_unlock( &wait_mutex );
	// TBD if ret == PSM2_OK we could use ipeek to see if any real progress
	// was made and loop back to start to wait again if not.  For now we
	// leave that to our caller
	return ret;

inval:
	ret = PSM2_PARAM_ERR;
unlock:
	wait_err_count++;
	_HFI_DBG("wait inval: err=%d\n", ret);
	PSMI_UNLOCK(psm3_creation_lock);
	return ret;
}

// wakeup on behalf of an event on given master ep
// called for event on master ep or any of its secondary eps
// wakes all threads in psm3_wait for given EP (may have to wake all
// because may not be able to associate an fi_wait call to an ep)
void psm3_wake(psm2_ep_t ep)
{
	_HFI_VDBG("wakeup\n");
	PSMI_LOCK_ASSERT(psm3_creation_lock);
	psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_WAITING);
	pthread_mutex_lock( &wait_mutex );
	wakeup_count++;
	wait_signalled = 1;
	pthread_cond_signal(&wait_condvar );
	//pthread_cond_broadcast(&wait_condvar );
	pthread_mutex_unlock( &wait_mutex );
}

static
int rcvthread_next_timeout(struct ptl_rcvthread *rcvc)
{
	uint64_t pollok_diff = rcvc->pollok - rcvc->pollok_last;

	if (pollok_diff > 0) {
		// timed out, but found work to do, so decrease timeout
		if (rcvc->last_timeout > rcvc->timeout_period_min)
			/* By default, be less aggressive, but there's a more aggressive
			 * alternative if need be */
#if 1
			rcvc->last_timeout >>= rcvc->timeout_shift;
#else
			rcvc->last_timeout = rcvc->timeout_period_min;
#endif
	} else {		/* timed out and found no work to do, grow timeout */
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
					&rcvc->pollok, &rcvc->pollcyc, &rcvc->pollintr);
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

static uint64_t rcvthread_stats_timeout(void *context)
{
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)context;
	return rcvc->last_timeout;
}

static psm2_error_t rcvthread_initstats(ptl_t *ptl_gen)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	struct ptl_rcvthread *rcvc = (struct ptl_rcvthread *)ptl->rcvthread;
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECLU64("intrthread_schedule_count",
				"number of rcvthread polling calls",
				&rcvc->pollcnt),
		PSMI_STATS_DECLU64("intrthread_schedule_intr",
				"number of rcvthread polling calls woken before timeout",
				&rcvc->pollintr),
		PSMI_STATS_DECLU64("intrthread_schedule_success",
				"number of rcvthread polling calls found work",
				&rcvc->pollok),
		PSMI_STATS_DECL("intrthread_schedule_success_(%)",
				"percent rcvthread polling calls found work",
				MPSPAWN_STATS_REDUCTION_ALL,
				rcvthread_stats_pollok, NULL),
		PSMI_STATS_DECLU64("intrthread_timeout_count",
				"number of rcvthread polling calls timedout with no work found",
				&rcvc->pollcnt_to),
		PSMI_STATS_DECL("intrthread_wasted_time_(ms)",
				"total rcvthread polling time with no work found",
				MPSPAWN_STATS_REDUCTION_ALL,
				rcvthread_stats_pollcyc, NULL),
		PSMI_STATS_DECL("intrthread_timeout_(ms)",
				"current rcvthread timeout in milliseconds",
				MPSPAWN_STATS_REDUCTION_ALL,
				rcvthread_stats_timeout, NULL),
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Statistics for fi_wait and corresponding "
			"wakeup events "),
		PSMI_STATS_DECLU64("wait_count",
				"number of wait calls",
				&wait_count),
		PSMI_STATS_DECLU64("wait_err_count",
				"number of failed wait calls (invalid preconditions)",
				&wait_err_count),
		PSMI_STATS_DECLU64("wait_nosleep_avail",
				"number of wait calls which found events already available",
				&wait_nosleep_avail_count),
		PSMI_STATS_DECLU64("wait_nosleep_signalled",
				"number of wait calls which found wakeup signalled already",
				&wait_nosleep_signalled_count),
		PSMI_STATS_DECLU64("wait_sleep_til_signal",
				"number of wait calls with infinite timeout which waited til signalled",
				&wait_sleep_til_signal_count),
		PSMI_STATS_DECLU64("wait_sleep_timeout",
				"number of wait calls with timeout which waited til timeout",
				&wait_sleep_timeout_count),
		PSMI_STATS_DECLU64("wait_sleep_signal",
				"number of wait calls with timeout which got signal before timeout",
				&wait_sleep_signal_count),
		PSMI_STATS_DECLU64("wakeup_count",
				"number of wakeup events",
				&wakeup_count),
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
		"Recv Progress Thread Statistics for the process.\n"
		"A progress is used to periodically check the NIC and ensure "
		"comms progress even if the applicaton only calls "
		"libfabric infrequently.",
					PSMI_STATSTYPE_RCVTHREAD,
					entries,
					PSMI_HOWMANY(entries), NULL, rcvc, NULL);
}
