#ifdef PSM_SOCKETS
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

/* This file implements the HAL specific code for PSM PTL for ips */
#include "psm_user.h"
#include "psm2_hal.h"
#include "ptl_ips.h"
#include "psm_mq_internal.h"
#include "sockets_hal.h"

psm2_error_t psm3_sockets_ips_ptl_poll(ptl_t *ptl_gen, int _ignored, bool force);

// initialize HAL specific parts of ptl_ips
// This is called after most of the generic aspects have been initialized
// so we can use ptl->ep, ptl->ctl, etc as needed
// However it is called prior to ips_proto_init.  ips_proto_init requires some
// ips_ptl items such as ptl->spioc
psm2_error_t psm3_sockets_ips_ptl_init_pre_proto_init(struct ptl_ips *ptl)
{
	ptl->ctl->ep_poll = psm3_sockets_ips_ptl_poll;	
	return PSM2_OK;
}

// initialize HAL specific parts of ptl_ips
// This is called after after ips_proto_init and after most of the generic
// aspects of ips_ptl have been initialized
// so we can use ptl->ep and ptl->proto as needed
psm2_error_t psm3_sockets_ips_ptl_init_post_proto_init(struct ptl_ips *ptl)
{
	struct ips_recvhdrq_callbacks recvq_callbacks;
	recvq_callbacks.callback_packet_unknown = psm3_sockets_ips_ptl_process_unknown;
	/*
	 * Hardware receive hdr/egr queue, services incoming packets and issues
	 * callbacks for protocol handling in proto_recv.  It uses the epstate
	 * interface to determine if a packet is known or unknown.
	 */
	return psm3_sockets_recvhdrq_init(&ptl->epstate,
		&ptl->proto, &recvq_callbacks, &ptl->recvq);
}

// finalize HAL specific parts of ptl_ips
// This is called before the generic aspects have been finalized
// but after ips_proto has been finalized
// so we can use ptl->ep as needed
psm2_error_t psm3_sockets_ips_ptl_fini(struct ptl_ips *ptl)
{
	return PSM2_OK;
}

psm2_error_t psm3_sockets_ips_ptl_poll(ptl_t *ptl_gen, int _ignored, bool force)
{
	struct ptl_ips *ptl = (struct ptl_ips *)ptl_gen;
	const uint64_t current_count = get_cycles();
	const int do_lock = PSMI_LOCK_DISABLED &&
		psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED);
	psm2_error_t err = PSM2_OK_NO_PROGRESS;
	psm2_error_t err2;

	if (do_lock && !ips_recvhdrq_trylock(&ptl->recvq))
		return err;
#ifdef USE_UDP
	// This is a trade-off.  We could avoid this if-test if we limit
	// all sockets NICs in a job to the same PSM3_SOCKETS setting and
	// then we could set our HAL function table to a TCP or UDP
	// specific ips_ptl_poll function based on PSM3_SOCKETS at the time
	// of HAL initialize.  If built with in-line HAL functions
	// would need to save off these few functions (or adjust psm2_hal.h
	// to have the function table entries) and then use them in our
	// sockets_inline_i.h when built with 1 HAL.
	// It is assumed that for sockets message rate, the extra if will not
	// make a noticible difference, so this approach is chosen as easier to
	// understand and maintain.
	if_pf (ptl->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_UDP)
		err = psm3_sockets_udp_recvhdrq_progress(&ptl->recvq, force);
	else
#endif /* USE_UDP */
		err = psm3_sockets_tcp_recvhdrq_progress(&ptl->recvq, force);
	if (do_lock)
		ips_recvhdrq_unlock(&ptl->recvq);
	if_pf(err > PSM2_OK_NO_PROGRESS)
		return err;
	err2 = psmi_timer_process_if_expired(&(ptl->timerq), current_count);
	if (err2 != PSM2_OK_NO_PROGRESS)
		return err2;
	else {
		return err;
	}
}

int psm3_sockets_ips_ptl_process_unknown(const struct ips_recvhdrq_event *rcv_ev)
{
	int opcode;

	if (0 == psm3_ips_proto_process_unknown(rcv_ev, &opcode))
		return IPS_RECVHDRQ_CONTINUE;

	// truely an unknown remote node, psm3_ips_proto_process_unknown already
	// did generic output and debug packet dumps
	// now output the final HAL specific error message
	psm3_handle_error(PSMI_EP_LOGEVENT, PSM2_EPID_NETWORK_ERROR,
			 "Received message(s) opcode=%x from an unknown process", opcode);

	/* Always skip this packet unless the above call was a noreturn call */
	return IPS_RECVHDRQ_CONTINUE;
}
#endif /* PSM_SOCKETS */
