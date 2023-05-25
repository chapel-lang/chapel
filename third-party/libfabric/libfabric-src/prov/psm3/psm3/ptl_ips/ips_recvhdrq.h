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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#ifndef _IPS_RECVHDRQ_H
#define _IPS_RECVHDRQ_H

#include "psm_user.h"
#include "ips_proto_params.h"
#include "ips_proto_header.h"

struct ips_recvhdrq;
struct ips_recvhdrq_state;
struct ips_epstate;

/* process current packet, continue on next packet */
#define IPS_RECVHDRQ_CONTINUE   0
/* process current packet, break and return to caller */
#define IPS_RECVHDRQ_BREAK      1
/* keep current packet, revisit the same packet next time */
#define IPS_RECVHDRQ_REVISIT	2
/* no progress, such as data not ready */
#define IPS_RECVHDRQ_NO_PROGRESS 3
/* internal error */
#define IPS_RECVHDRQ_ERROR	4


struct ips_recvhdrq_event {
	struct ips_proto *proto;
	const struct ips_recvhdrq *recvq;	/* where message received */
	struct ips_message_header *p_hdr;	/* protocol header in rcv_hdr */
	struct ips_epaddr *ipsaddr;	/* peer ipsaddr, if available */
	// we point to the payload part of our recv buffer
	uint8_t *payload;
	uint32_t payload_size;
};

struct ips_recvhdrq_callbacks {
	int (*callback_packet_unknown) (const struct ips_recvhdrq_event *);
};

/*
 * Structure containing state for recvhdrq reading. This is logically
 * part of ips_recvhdrq but needs to be separated out for context
 * sharing so that it can be put in a shared memory page and hence
 * be available to all processes sharing the context. Generally, do not
 * put pointers in here since the address map of each process can be
 * different.
 */
#define NO_EAGER_UPDATE ~0U
struct ips_recvhdrq_state {
};

/*
 * Structure to read from recvhdrq
 */
struct ips_recvhdrq {
	struct ips_proto *proto;
	/* Header queue handling */
	pthread_spinlock_t hdrq_lock;	/* Lock for thread-safe polling */
	/* Lookup endpoints epid -> ptladdr (rank)) */
	const struct ips_epstate *epstate;

	/* Callbacks to handle recvq events */
	struct ips_recvhdrq_callbacks recvq_callbacks;

	/* List of flows with pending acks for receive queue */
	SLIST_HEAD(pending_flows, ips_flow) pending_acks;

};

PSMI_INLINE(
void *
ips_recvhdrq_event_payload(const struct ips_recvhdrq_event *rcv_ev))
{
	psmi_assert(rcv_ev);
	return rcv_ev->payload;
}

PSMI_INLINE(
uint32_t
ips_recvhdrq_event_paylen(const struct ips_recvhdrq_event *rcv_ev))
{
	psmi_assert(rcv_ev);
	return rcv_ev->payload_size;
}

PSMI_INLINE(int ips_recvhdrq_trylock(struct ips_recvhdrq *recvq))
{
	int ret = pthread_spin_trylock(&recvq->hdrq_lock);
	return !ret;
}

PSMI_INLINE(int ips_recvhdrq_lock(struct ips_recvhdrq *recvq))
{
	int ret = pthread_spin_lock(&recvq->hdrq_lock);
	return !ret;
}

PSMI_INLINE(int ips_recvhdrq_unlock(struct ips_recvhdrq *recvq))
{
	int ret = pthread_spin_unlock(&recvq->hdrq_lock);
	return !ret;
}

#endif /* _IPS_RECVHDRQ_H */
