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

/* This file implements the PSM PTL for ips */
#include "psm_user.h"
#include "psm2_hal.h"
#include "ptl_ips.h"
#include "psm_mq_internal.h"

static size_t ips_ptl_sizeof(void)
{
	return sizeof(struct ptl_ips);
}

#if 0	// unused code, specific to QLogic MPI
static
int ips_ptl_epaddr_stats_num(void)
{
	return sizeof(struct ips_proto_epaddr_stats) / sizeof(uint64_t);
}

static
int ips_ptl_epaddr_stats_init(char **desc, uint16_t *flags)
{
	int num_stats =
	    sizeof(struct ips_proto_epaddr_stats) / sizeof(uint64_t);
	int i;
	int j=0;

	/* All stats are uint64_t */
	for (i = 0; i < num_stats; i++)
		flags[i] = MPSPAWN_STATS_REDUCTION_ALL |
		    MPSPAWN_STATS_SKIP_IF_ZERO;

	desc[j++] = "errchecks send";
	desc[j++] = "errchecks recv";
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RNDV_MOD
	desc[j++] = "err_chk_rdma send"
	desc[j++] = "err_chk_rdma recv"
#endif
#endif
	desc[j++] = "nak send";
	desc[j++] = "nak recv";
	desc[j++] = "connect req send";
	desc[j++] = "connect req recv";
	desc[j++] = "connect rep send";
	desc[j++] = "connect rep recv";
	desc[j++] = "disconnect req send";
	desc[j++] = "disconnect req recv";
	desc[j++] = "disconnect rep send";
	desc[j++] = "disconnect rep recv";
	desc[j++] = "tid grants send";
	desc[j++] = "tid grants recv";
	desc[j++] = "send rexmit";
#if defined(PSM_VERBS)
#ifdef PSM_HAVE_RNDV_MOD
	desc[j++] = "rdma rexmit";
#endif
#endif
	desc[j++] = "congestion packets";

	psmi_assert(num_stats == j);
	return num_stats;
}

int ips_ptl_epaddr_stats_get(psm2_epaddr_t epaddr, uint64_t *stats_o)
{
	int i, num_stats =
	    sizeof(struct ips_proto_epaddr_stats) / sizeof(uint64_t);
	uint64_t *stats_i = (uint64_t *) &epaddr->proto->epaddr_stats;

	for (i = 0; i < num_stats; i++)
		stats_o[i] = stats_i[i];

	return num_stats;
}
#endif // 0	// unused code, specific to QLogic MPI

/* periodic (PSMI_CONTEXT_STATUS_CHECK_INTERVAL_MSECS (250ms)) check for
 * health of HW and network link.
 */
static
psm2_error_t
psm3_context_check_status_callback(struct psmi_timer *t, uint64_t current)
{
	struct ptl_ips *ptl = (struct ptl_ips *)t->context;
	const uint64_t current_count = get_cycles();
	psm2_error_t err;

	err = psmi_hal_context_check_status(ptl);
	psm3_timer_request_always(&ptl->timerq, &ptl->status_timer,
				  current_count + ptl->status_cyc_timeout);

	return err;
}

static
psm2_error_t ips_ptl_init(const psm2_ep_t ep, ptl_t *ptl_gen, ptl_ctl_t *ctl)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	uint32_t num_of_send_bufs = ep->hfi_num_sendbufs;
	uint32_t num_of_send_desc = ep->hfi_num_descriptors;
	uint32_t imm_size = ep->hfi_imm_size;
	const uint64_t current_count = get_cycles();

	/* Preconditions */
	psmi_assert_always(ep != NULL);
	psmi_assert_always(ep->epaddr != NULL);
	psmi_assert_always(!psm3_epid_zero_internal(ep->epid));
	psmi_assert_always(ep->hfi_num_sendbufs > 0);

	memset(ptl, 0, sizeof(struct ptl_ips));

	ptl->ep = ep;		/* back pointer */
	ptl->epid = ep->epid;	/* cache epid */
	ptl->epaddr = ep->epaddr;	/* cache a copy */
	ptl->ctl = ctl;

	memset(ctl, 0, sizeof(*ctl));
	/* Fill in the control structure */
	ctl->ep = ep;
	ctl->ptl = ptl_gen;
	// ctl->ep_poll initialized in psmi_hal_ips_ptl_init_pre_proto_init
	ctl->ep_connect = psm3_ips_ptl_connect;
	ctl->ep_disconnect = psm3_ips_ptl_disconnect;
	ctl->mq_send = psm3_ips_proto_mq_send;
	ctl->mq_isend = psm3_ips_proto_mq_isend;

	ctl->am_get_parameters = psm3_ips_am_get_parameters;

	ctl->am_short_request = psm3_ips_am_short_request;
	ctl->am_short_reply = psm3_ips_am_short_reply;

#if 0	// unused code, specific to QLogic MPI
	ctl->epaddr_stats_num = ips_ptl_epaddr_stats_num;
	ctl->epaddr_stats_init = ips_ptl_epaddr_stats_init;
	ctl->epaddr_stats_get = ips_ptl_epaddr_stats_get;
#endif

	/*
	 * Runtime flags in 'ptl' are different from runtime flags in 'context'.
	 * In 'context', runtime flags reflect what the driver is capable of.
	 * In 'ptl', runtime flags reflect the features we can or want to use in
	 *           the driver's supported runtime flags.
	 */

	/*
	 * This timer is to be used to check the context's status at every
	 * PSMI_CONTEXT_STATUS_CHECK_INTERVAL_MSECS.  This is useful to detect when
	 * the link transitions from the DOWN state to the UP state.  We can thus
	 * stop aggregating link failure messages once we detect that the link is
	 * up.
	 */
	psmi_timer_entry_init(&ptl->status_timer,
			      psm3_context_check_status_callback, ptl);

	/* cache the context's status timeout in cycles */
	ptl->status_cyc_timeout =
	    ms_2_cycles(PSMI_CONTEXT_STATUS_CHECK_INTERVAL_MSECS);

	/*
	 * Retransmissions and pending operations are kept in a timer structure
	 * (queue).  The timerq is shared to various internal IPS interfaces so
	 * that they too may schedule events on the timer queue.  The timerq is
	 * drained in the progress function.
	 */
	if ((err = psm3_timer_init(&ptl->timerq)))
		goto fail;

	/* start the context's status timer */
	psm3_timer_request_always(&ptl->timerq, &ptl->status_timer,
				  current_count + ptl->status_cyc_timeout);

	/*
	 * Epstate maps endpoint ids (epid integers) to ipsaddr (structs). Mappings
	 * are added/removed by the connect portion of the ips protocol and lookup
	 * is made by the receive queue processing component.
	 */
	if ((err = psm3_ips_epstate_init(&ptl->epstate, ep)))
		goto fail;

	if ((err = psmi_hal_ips_ptl_init_pre_proto_init(ptl)))
		goto fail;

	/*
	 * Actual ips protocol handling.
	 */
	if ((err =
	     psm3_ips_proto_init(ep, ptl_gen, num_of_send_bufs, num_of_send_desc,
			    imm_size, &ptl->timerq, &ptl->epstate, ptl->spioc,
			    &ptl->proto)))
		goto fail;

	if ((err = psmi_hal_ips_ptl_init_post_proto_init(ptl)))
		goto fail;

	/*
	 * ips_ptl threads, always initialized but not necessary creates a
	 * pthread.
	 */
	if ((err = psm3_ips_ptl_rcvthread_init(ptl_gen, &ptl->recvq)))
		goto fail;
fail:
	return err;
}

static psm2_error_t ips_ptl_fini(ptl_t *ptl_gen, int force, uint64_t timeout_in)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	psm2_error_t err = PSM2_OK;

	/* Finalize the protocol before stopping the threads because
	 * connect/disconnect packets use interrupts and the kernel doesn't
	 * like to have no pollers waiting */
	if ((err = psm3_ips_proto_disconnect_all(&ptl->proto, force, timeout_in)))
		goto fail;

	/* stop the threads.  They may be accessing fields in proto such as
	 * proto->mr_cache, or ep linked lists.  In practice the threads are
	 * already mostly blocked since our caller has psm3_creation_lock */
	if ((err = psm3_ips_ptl_rcvthread_fini(ptl_gen)))
		goto fail;

	/* now free the proto resources */
	if ((err = psm3_ips_proto_fini(&ptl->proto)))
		goto fail;

	if ((err = psm3_ips_epstate_fini(&ptl->epstate)))
		goto fail;

	if ((err = psmi_hal_ips_ptl_fini(ptl)))
		goto fail;

	if ((err = psm3_timer_fini(&ptl->timerq)))
		goto fail;

fail:
	return err;
}

static
psm2_error_t
ips_ptl_optctl(const void *core_obj, int optname,
	       void *optval, uint64_t *optlen, int get)
{
	psm2_error_t err = PSM2_OK;

	switch (optname) {
	case PSM2_IB_OPT_EP_SL:
		{
			/* Core object is psm2_epaddr */
			psm2_epaddr_t epaddr = (psm2_epaddr_t) core_obj;
			ips_epaddr_t *ipsaddr = (ips_epaddr_t *) epaddr;

			/* If endpoint does not use IB ignore for set, complain for get */
			if (epaddr->ptlctl->ep_connect != psm3_ips_ptl_connect) {
				if (get)
					err =
					    psm3_handle_error(PSMI_EP_LOGEVENT,
							      PSM2_PARAM_ERR,
							      "Invalid EP transport");
				goto exit_fn;
			}

			/* Sanity check option length */
			if (*optlen < sizeof(uint8_t)) {
				err =
				    psm3_handle_error(PSMI_EP_LOGEVENT,
						      PSM2_PARAM_ERR,
						      "Option value length error");
				*optlen = sizeof(unsigned);
				goto exit_fn;
			}

			if (get) {
				/* Get returns the SL for the PIO flow */
				*((uint8_t *) optval) =
				    (uint8_t) ipsaddr->
				    flows[EP_FLOW_GO_BACK_N_PIO].path->pr_sl;
			} else {
				uint16_t new_sl;

				/* Sanity check if SL is within range */
				new_sl = (uint16_t) *(uint8_t *) optval;
				if (new_sl > PSMI_SL_MAX) {
					err =
					    psm3_handle_error(PSMI_EP_LOGEVENT,
						      PSM2_PARAM_ERR,
						      "Invalid SL value %u. %d<= SL <=%d.",
						      new_sl, PSMI_SL_MIN, PSMI_SL_MAX);
					goto exit_fn;
				}

				/* Set new SL for all flows */
				ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO].path->
				    pr_sl = new_sl;
			}
		}
		break;
	case PSM2_IB_OPT_DF_SL:
		{
			/* Set default SL to be used by an endpoint for all communication */
			/* Core object is psm2_epaddr */
			psm2_ep_t ep = (psm2_ep_t) core_obj;

			/* Make sure ep is specified */
			if (!ep) {
				err =
				    psm3_handle_error(PSMI_EP_LOGEVENT,
						      PSM2_PARAM_ERR,
						      "Invalid PSM Endpoint");
				goto exit_fn;
			}

			/* Sanity check option length */
			if (*optlen < sizeof(uint8_t)) {
				err =
				    psm3_handle_error(PSMI_EP_LOGEVENT,
						      PSM2_PARAM_ERR,
						      "Option value length error");
				*optlen = sizeof(uint8_t);
				goto exit_fn;
			}

			if (get) {
				*((uint8_t *) optval) =
					((struct ptl_ips *)(ep->ptl_ips.ptl))->proto.epinfo.ep_sl;
			} else {
				uint16_t new_sl;

				/* Sanity check if SL is within range */
				new_sl = (uint16_t) *(uint8_t *) optval;
				if (new_sl > PSMI_SL_MAX) {
					err =
					    psm3_handle_error(PSMI_EP_LOGEVENT,
						      PSM2_PARAM_ERR,
						      "Invalid SL value %u. %d<= SL <=%d.",
						      new_sl, PSMI_SL_MIN, PSMI_SL_MAX);
					goto exit_fn;
				}

				((struct ptl_ips *)(ep->ptl_ips.ptl))->proto.epinfo.ep_sl =
				    (uint8_t) new_sl;
			}
		}
		break;
	default:
		err =
		    psm3_handle_error(NULL, PSM2_PARAM_ERR,
				      "Unknown PSM3_IB option %u.", optname);
	}

exit_fn:
	return err;
}

static
psm2_error_t
ips_ptl_setopt(const void *component_obj, int optname,
	       const void *optval, uint64_t optlen)
{
	return ips_ptl_optctl(component_obj, optname, (void *)optval, &optlen,
			      0);
}

static
psm2_error_t
ips_ptl_getopt(const void *component_obj, int optname,
	       void *optval, uint64_t *optlen)
{
	return ips_ptl_optctl(component_obj, optname, optval, optlen, 1);
}

static
uint32_t
ips_ptl_rcvthread_is_enabled(const ptl_t *ptl)
{
	return psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
}

psm2_error_t
psm3_ips_ptl_connect(ptl_t *ptl_gen, int numep, const psm2_epid_t *array_of_epid,
		const int *array_of_epid_mask, psm2_error_t *array_of_errors,
		psm2_epaddr_t *array_of_epaddr, uint64_t timeout_in)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	psm2_error_t err;
	psm2_ep_t ep;
	psm2_epid_t *epid_array = NULL;
	psm2_error_t *error_array = NULL;
	psm2_epaddr_t *epaddr_array = NULL;
	ips_epaddr_t *ipsaddr_master, *ipsaddr;
	int *mask_array = NULL;
	int i;

	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);
	err = psm3_ips_proto_connect(&ptl->proto, numep, array_of_epid,
				array_of_epid_mask, array_of_errors,
				array_of_epaddr, timeout_in);
	if (err)
		return err;

	psmi_assert_always(ptl->ep->mctxt_master == ptl->ep);
	if (ptl->ep->mctxt_next == ptl->ep)
		return err;

	/* make the additional multi-context connections. */
	epid_array = (psm2_epid_t *)
	    psmi_malloc(ptl->ep, UNDEFINED, sizeof(psm2_epid_t) * numep);
	mask_array = (int *)
	    psmi_malloc(ptl->ep, UNDEFINED, sizeof(int) * numep);
	error_array = (psm2_error_t *)
	    psmi_malloc(ptl->ep, UNDEFINED, sizeof(psm2_error_t) * numep);
	epaddr_array = (psm2_epaddr_t *)
	    psmi_malloc(ptl->ep, UNDEFINED, sizeof(psm2_epaddr_t) * numep);
	if (!epid_array || !mask_array || !error_array || !epaddr_array) {
		goto fail;
	}

	ep = ptl->ep->mctxt_next;
	while (ep != ep->mctxt_master) {

		/* Setup the mask array and epid array. */
		for (i = 0; i < numep; i++) {
			if (array_of_epid_mask[i]
			    && array_of_errors[i] == PSM2_OK) {
				ipsaddr_master =
				    (ips_epaddr_t *) array_of_epaddr[i];
				ipsaddr = ipsaddr_master->next;
				mask_array[i] = 0;
				while (ipsaddr != ipsaddr_master) {
					if (((psm2_epaddr_t) ipsaddr)->proto->
					    ep == ep) {
						mask_array[i] = 1;
						epid_array[i] =
						    ((psm2_epaddr_t) ipsaddr)->
						    epid;
						break;
					}
					ipsaddr = ipsaddr->next;
				}
			} else {
				mask_array[i] = 0;
			}
		}

		/* Make the real protocol connections. */
		err =
			psm3_ips_proto_connect(&((struct ptl_ips *)(ep->ptl_ips.ptl))->proto,
					  numep, epid_array, mask_array, error_array,
					  epaddr_array, timeout_in);
		if (err)
			goto fail;

		ep = ep->mctxt_next;
	}

fail:
	if (epid_array)
		psmi_free(epid_array);
	if (mask_array)
		psmi_free(mask_array);
	if (error_array)
		psmi_free(error_array);
	if (epaddr_array)
		psmi_free(epaddr_array);

	return err;
}

psm2_error_t
psm3_ips_ptl_disconnect(ptl_t *ptl_gen, int force, int numep,
		   psm2_epaddr_t array_of_epaddr[],
		   const int array_of_epaddr_mask[],
		   psm2_error_t array_of_errors[], uint64_t timeout_in)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	int *array_of_epaddr_mask_internal, i;
	psm2_error_t err;

	/*
	 * Copy true values from array_of_epaddr_mask, provided that their
	 * respective epaddr is an ips one.
	 * Newly created mask will be used for the protocol disconnect call
	 * instead.
	 */
	PSMI_LOCK_ASSERT(ptl->ep->mq->progress_lock);
	array_of_epaddr_mask_internal = psmi_calloc(ptl->ep, UNDEFINED,
						    sizeof(int), numep);
	if (!array_of_epaddr_mask_internal)
		return PSM2_NO_MEMORY;

	for (i = 0; i < numep; ++i) {
		if (array_of_epaddr_mask[i] && array_of_epaddr[i]
		    && array_of_epaddr[i]->ptlctl->ptl == ptl_gen) {
			array_of_epaddr_mask_internal[i] = 1;
		}
	}

	err = psm3_ips_proto_disconnect(&ptl->proto, force, numep, array_of_epaddr,
				   array_of_epaddr_mask_internal,
				   array_of_errors, timeout_in);

	psmi_free(array_of_epaddr_mask_internal);
	return err;
}

/* Only symbol we expose out of here */
struct ptl_ctl_init
psm3_ptl_ips = {
	ips_ptl_sizeof, ips_ptl_init, ips_ptl_fini, ips_ptl_setopt,
	    ips_ptl_getopt
};

struct ptl_ctl_rcvthread
psm3_ptl_ips_rcvthread = {
	ips_ptl_rcvthread_is_enabled,
	psm3_ips_ptl_rcvthread_transfer_ownership,
};
