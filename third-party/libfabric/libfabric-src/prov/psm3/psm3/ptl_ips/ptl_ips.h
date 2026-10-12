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

#ifndef _IPS_PTL_H
#define _IPS_PTL_H

#include "psm_user.h"

#include "ips_proto.h"

/*
 * PTL at the ips level (for NIC)
 *
 * This PTL structure glues all the ips components together.
 *
 * * ips timer, shared by various components, allows each component to
 *   schedule time-based expiration callbacks on the timerq.
 * * HW receive queue
 * * send control block to handle eager messages
 * * instantiation of the ips protocol
 * * endpoint state, to map endpoint indexes into structures
 *
 *   Receive-side
 *
 *          ----[   proto    ]
 *         /       ^      ^
 *        |        |      |
 *        |     packet  packet
 *        |	known   unknown
 *   add_endpt      \ /
 *        |          |
 *        `----> [epstate]
 *                   ^
 *                   |
 *               lookup_endpt
 *                   |
 *                [recvq]
 *                   |
 *                 poll
 *
 */
/* Updates to this struct must be reflected in PTL_IPS_SIZE in ptl_fwd.h */
/* IPS knows it functions as a PTL whenever ptl->ep is non-NULL */
struct ptl_ips {
	psm2_ep_t ep;		/* back ptr */
	psm2_epid_t epid;	/* cached from ep */
	psm2_epaddr_t epaddr;	/* cached from ep */
	ips_epaddr_t *ipsaddr;	/* cached from epaddr */
	ptl_ctl_t *ctl;		/* cached from init */

	void *spioc;	        /* PIO send control (opaque ptr) */
	struct ips_proto proto;	/* protocol instance: timerq, epstate, spio */

	struct psmi_timer_ctrl timerq;
	struct ips_epstate epstate;	/* map incoming packets */
	struct ips_recvhdrq_state recvq_state;
	struct ips_recvhdrq recvq;	/* HW recvq: epstate, proto */

	/* timer to check the context's status */
	struct psmi_timer status_timer;

	/* context's status check timeout in cycles -- cached */
	uint64_t status_cyc_timeout;
	/* Rcv thread context */
	struct ptl_rcvthread *rcvthread;
}
#ifndef PACK_STRUCT_STL
#define PACK_STRUCT_STL /* nothing */
#endif
 __attribute__ ((PACK_STRUCT_STL aligned(16)));

/*
 * Connect/disconnect are wrappers around psm proto's connect/disconnect,
 * mostly to abstract away PSM-specific stuff from ips internal structures
 */
psm2_error_t psm3_ips_ptl_connect(ptl_t *ptl, int numep,
			    const psm2_epid_t *array_of_epid,
			    const int *array_of_epid_mask,
			    psm2_error_t *array_of_errors,
			    psm2_epaddr_t *array_of_epaddr,
			    uint64_t timeout_in);

psm2_error_t psm3_ips_ptl_disconnect(ptl_t *ptl, int force, int numep,
			       psm2_epaddr_t array_of_epaddr[],
			       const int array_of_epaddr_mask[],
			       psm2_error_t array_of_errors[],
			       uint64_t timeout_in);

/*
 * Support for receive thread
 */
psm2_error_t psm3_ips_ptl_rcvthread_init(ptl_t *ptl, struct ips_recvhdrq *recvq);
psm2_error_t psm3_ips_ptl_rcvthread_fini(ptl_t *ptl);
void psm3_ips_ptl_rcvthread_transfer_ownership(ptl_t *from_ptl, ptl_t *to_ptl);

#endif /* _IPS_PTL_H */
