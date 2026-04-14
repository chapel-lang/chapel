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

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "psm_mq_internal.h"
#include "ips_proto_internal.h"
#include "ips_proto_connect.h"

/* PSM/IPS uses a peer to peer connection protocol where the incoming
 * and outgoing state machines are mostly independent.
 * The protocol allows for an optional reconnect mechanism for HAL specific
 * underlying resources, such as user RC QPs in the Verbs HAL.
 *
 * The Startup protocol is in PSM/IPS driven by psm3_ips_proto_connect()
 * and protected by the MQ progress_lock with the following basic behavior:
 * (For brevity, this summary purposely leaves out some details and omits
 * details related to reconnect).
 *
 * Start overall timer.
 *
 * For all nodes to connect to:
 *   Look up epid in table
 *      MATCH.
 *         If disconnected or disconnecting
 *            return INVALID_CONNECT
 *         else if cstate_outgoing == ESTABLISHED || reconnecting
 *            return the already connected address.
 *         else
 *            cstate_outgoing := OUTGOING_WAITING
 *            req->connidx := epaddr->connidx_incoming
 *            add to list of pending connect.
 *      NO MATCH
 *         allocate epaddr and put in table
 *         cstate_outgoing := CONNECT_WAITING
 *         cstate_incoming := CONNECT_NONE
 *         connidx_outgoing := UNKNOWN
 *         req->connidx := epaddr->connidx_incoming := NEW connidx integer
 *         add to list of pending connect
 *
 * expected_connect_count = ep->total_connect_count + num_to_connect
 * while (expected_connect_count != ep->total_connect_count)
 *    check for overall timeout
 *    check for individual connection request's reply timeout, resend request
 *    progress();
 *
 * For all connection requests received (within progress loop)
 *   If uuid doesn't match, NAK the connect and skip request
 *   Look up epid in table
 *      MATCH
 *         if cstate_incoming == ESTABLISHED && ! requesting reconnect
 *            req->connidx := epaddr->connidx_incoming
 *            compose reply and send again (this is a dupe request).
 *         else if requesting reconnect
 *            reconnect details not shown in this example
 *         else
 *            cstate_incoming := ESTABLISHED
 *            epaddr->connidx_outgoing := req->connidx
 *            req->connidx := epaddr->connidx_incoming
 *      NO MATCH
 *         allocate epaddr and put in table
 *         cstate_incoming := ESTABLISHED
 *         epaddr->connidx_outgoing = req->connidx;
 *         rep->connidx := epaddr->connidx_incoming := NEW connidx integer
 *         compose connect reply and send
 *
 * For all connection replies received:
 *    If connect_result != 0
 *        process error and report connect error
 *    if cstate_incoming != OUTGOING_WAITING
 *        confirm matches prior parameters
 *    else
 *        epaddr->connidx_outgoing := rep->connidx
 *        cstate_outgoing := ESTABLISHED
 *        ep->total_connect_count ++
 *
 *
 * The reconnection protocol is optional per HAL and may be used to
 * recover from HAL issues, such as user RC QP timeout/retry-exceeded.
 * The reconnection protocol is also a peer to peer mechanism which
 * leverages the same basic connection protocol packets, but increments a
 * reconnect_count field in the request and reply packets to reflect the
 * recovery iteration.  A given connection may be reconnected multiple times,
 * and the count will wrap from 255 back to 1 (0 is only used in initial
 * connect attempts).
 *
 * Reconnection leverages the same basic outgoing connection states of
 * OUTGOING_WAITING and ESTABLISHED, which means an established connection
 * may revert to WAITING.  However, when an outbound connection is being
 * recovered, the "reconnecting" flag is set to indicate it is likely to be
 * restored to full capabilities.
 *
 * Incoming reconnection uses the same ESTABLISHED state but adds a
 * INCOMING_NEED_RECONNECT state when the local rank determines a new
 * recovery connection request from the peer is needed and expected.
 *
 * HAL routines called from this file may then recreate the resources.
 * For user RC QPs, this means the QP is recreated, the HAL fills in
 * reconnection request and reply packets with QP information, and this file
 * exchanges the packets.  Much as in initial connection, HAL routines are
 * called during the reconnection packet processing to transition the QP
 * through RTR and RTS as the outgoing state returns to ESTABLISHED.
 *
 * Upon orderly disconnect (! force), when reconnection is enabled, a
 * time wait delay occurs once both incoming and outgoing states reach
 * DISCONNECTED or NONE. This delay can allow some time for HAL resources to
 * be fully cleaned up (such as allowing user RC QPs to drain) before finally
 * freeing the ips_epaddr_t and its HAL resources.
 *
 * For HALs and HAL configurations which don't set ep->allow_reconnect, the
 * reconnect_count will remain at 0 and the state machine will never revert to
 * OUTGOING_WAITING after ESTABLISHED and never use the
 * INCOMING_NEED_RECONNECT state and never set the reconnecting flag.  Also
 * the time wait delay does not occur after disconnecting.
 *
 * some ips_epaddr_t flags:
 * reconnecting - only for datapath tests of rc_connected || reconnecting
 * waiting_in_connect - super state indicating timeout monitoring for
 *		ips_epaddr_t is under control of psm3_ips_proto_connect.
 *		When set, timer_reconnect is not used, but instead
 *		psm3_ips_proto_connect is monitoring for timeouts during its
 *		polling loop.
 * waiting_in_disconnect - super state indicating outbound ips_epaddr_t is
 *		under control of psm3_ips_proto_disconnect.  When set,
 *		inbound packet and event processing assumes the connection
 *		will soon be taken down. As a result, reconnection and
 *		inbound REQ/REP packet processing behave as if the connection
 *		is DISCONNECTED.  Also while set timer_reconnect is not used,
 *		but instead psm3_ips_proto_disconnect is monitoring for
 *		timeouts during	its polling loop.
 * set_req_params - only for debug asserts, indicates params have been
 *		set as part of processing an inbound request or reply
 *
 * timer_reconnect - a timer used for outbound reconnections for
 *		any reconnection REQ retries or total timeout.  Also used
 *		for the time wait delay after disconnection.
 */


// common CONNDBG formatting.  For the 1st REQ received or sent
// more details about the epid is output, the ipsaddr is always output
// so it is a good search key when searching logs.  epid is also
// provided and can be a good search key for a remote host.
// where available reconnect_count (rcnt) and incoming/outgoing state are
// also output

static const char* cstate_to_str(uint8_t state)
{
	switch (state) {
	case 0: 
		return "0";
	case CSTATE_NONE: 
		return "N";
	case CSTATE_ESTABLISHED:
		return "E";
	case CSTATE_OUTGOING_WAITING:
		return "W";
	case CSTATE_OUTGOING_WAITING_DISC:
		return "WD";
	case CSTATE_DISCONNECTED:
		return "D";
	case CSTATE_INCOMING_NEED_RECONNECT:
		return "R";
	default:
		// unexpected/unknown - should not happen
		return "U";
	}
}

// basic info for connection_error callback and some other errors
#define _HFI_CONNDBG_BASIC(action, ipsaddr, fmt, ...) \
	_HFI_CONNDBG("CONN rcnt=%u st=%s/%s %s: %p local %s to %s" fmt "\n", \
		(ipsaddr)->reconnect_count, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr),  \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 0), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 1), \
		##__VA_ARGS__)
#define _HFI_CONNDBG_BASIC_EPID(action, proto, remote_epid, fmt, ...) \
	_HFI_CONNDBG("CONN %s: local %s to %s" fmt "\n", \
		(action), \
		psm3_epid_fmt_internal(proto->ep->epid, 0), \
		psm3_epid_fmt_internal(remote_epid, 1), \
		##__VA_ARGS__)
// inbound REP or reconnect REQ
#define _HFI_CONNDBG_RCV(op, action, ipsaddr, req, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv %s rcnt=%u st=%s/%s%s: %p from %s (%s) to %s rcnt=%u" fmt "\n", \
		(op), (req)->reconnect_count, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 0), \
		(req)->hostname, \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 1), \
		(ipsaddr)->reconnect_count, \
		##__VA_ARGS__)
// inbound non-reconnect REQ
#define _HFI_CONNDBG_RCV_REQ(action, ipsaddr, req, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv REQ rcnt=%u st=%s/%s%s: %p from %s (%s) (%s) to %s (%s) rcnt=%u" fmt "\n", \
		(req)->reconnect_count, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 0), \
		(req)->hostname, \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 1), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 3), \
		(ipsaddr)->reconnect_count, \
		##__VA_ARGS__)
// inbound REP with no matching ipsaddr
#define _HFI_CONNDBG_RCV_REP_EPID(action, proto, remote_epid, req, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv REP rcnt=%u %s: from %s (%s) (%s) to %s" fmt "\n", \
		(req)->reconnect_count, (action), \
		psm3_epid_fmt_internal(remote_epid, 0), \
		(req)->hostname, \
		psm3_epid_fmt_addr(remote_epid, 1), \
		psm3_epid_fmt_internal(proto->ep->epid, 2), \
		##__VA_ARGS__)
// inbound DREQ
#define _HFI_CONNDBG_RCV_DREQ(action, ipsaddr, force, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv DREQ force=%u st=%s/%s%s: %p from %s to %s" fmt "\n", \
		(force), \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr),  \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 0), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 1), \
		##__VA_ARGS__)
// inbound DREP
#define _HFI_CONNDBG_RCV_DREP(action, ipsaddr, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv DREP st=%s/%s%s: %p from %s to %s" fmt "\n", \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr),  \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 0), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 1), \
		##__VA_ARGS__)
// inbound DREP with no matching ipsaddr
#define _HFI_CONNDBG_RCV_DREP_EPID(action, proto, remote_epid, fmt, ...) \
	_HFI_CONNDBG("CONN Rcv DREP%s: from %s (%s) to %s" fmt "\n", \
		(action), \
		psm3_epid_fmt_internal(remote_epid, 0), \
		psm3_epid_fmt_addr(remote_epid, 1), \
		psm3_epid_fmt_internal(proto->ep->epid, 2), \
		##__VA_ARGS__)
// outbound REQ
#define _HFI_CONNDBG_SENT_REQ(action, ipsaddr, req, fmt, ...) \
	_HFI_CONNDBG("CONN Sent REQ rcnt=%u st=%s/%s%s: %p from %s (%s) to %s (%s)" fmt "\n", \
		(req)->reconnect_count, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 0), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 1), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 3), \
		##__VA_ARGS__)
// outbound DREQ
#define _HFI_CONNDBG_SENT_DREQ(action, ipsaddr, force, fmt, ...) \
	_HFI_CONNDBG("CONN Sent DREQ force=%u st=%s/%s%s: %p from %s (%s) to %s (%s)" fmt "\n", \
		force, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 0), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 1), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 3), \
		##__VA_ARGS__)
// outbound REP
#define _HFI_CONNDBG_SENT_REP(action, ipsaddr, rep, fmt, ...) \
	_HFI_CONNDBG("CONN Sent REP rcnt=%u st=%s/%s%s: %p from %s (%s) to %s (%s)" fmt "\n", \
		(rep)->reconnect_count, \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 0), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 1), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 3), \
		##__VA_ARGS__)
// outbound DREP
#define _HFI_CONNDBG_SENT_DREP(action, ipsaddr, fmt, ...) \
	_HFI_CONNDBG("CONN Sent DREP st=%s/%s%s: %p from %s (%s) to %s (%s)" fmt "\n", \
		cstate_to_str((ipsaddr)->cstate_incoming), \
		cstate_to_str((ipsaddr)->cstate_outgoing), \
		(action), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 0), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 1), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 3), \
		##__VA_ARGS__)

#define _HFI_CONN_FATAL(err, desc, ipsaddr, fmt, ...) \
	psm3_handle_error(PSMI_EP_NORETURN, err, \
		"%s: %p from %s (%s) on %s port %u epid %s (%s)" fmt "\n", \
		(desc), (ipsaddr), \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.epid, 0), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.epid, 1), \
		(ipsaddr)->epaddr.proto->ep->dev_name, \
		(ipsaddr)->epaddr.proto->ep->portnum, \
		psm3_epid_fmt_internal((ipsaddr)->epaddr.proto->ep->epid, 2), \
		psm3_epid_fmt_addr((ipsaddr)->epaddr.proto->ep->epid, 3), \
		##__VA_ARGS__)

#define _HFI_CONN_FATAL_EPID(err, desc, proto, remote_epid, fmt, ...) \
	psm3_handle_error(PSMI_EP_NORETURN, err, \
		"%s: from %s (%s) on %s port %u epid %s (%s)" fmt "\n", \
		(desc), \
		psm3_epid_fmt_internal(remote_epid, 0), \
		psm3_epid_fmt_addr(remote_epid, 1), \
		proto->ep->dev_name, proto->ep->portnum, \
		psm3_epid_fmt_internal(proto->ep->epid, 2), \
		psm3_epid_fmt_addr(proto->ep->epid, 3), \
		##__VA_ARGS__)

#define _HFI_CONNDBG_OR_PRDBG(fmt, ...) \
	do { \
		if (_HFI_CONNDBG_ON || _HFI_PRDBG_ON) \
			_HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__); \
	} while (0)

/*
 *   Fill in a connection request:
 *      1. Set connect protocol version and PSM versions
 *      2. Set the uuid attached to current endpoint and add the job_pkey
 *         the node wishes to communicate post-connect.
 *      3. Set our mtu, rate, etc to help detect inconsistencies between peers
 */
static int
ips_proto_build_connect_message(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t opcode, uint8_t force, void *payload,
	size_t max_paylen);

static psm2_error_t
psm3_ips_proto_reconnect_timer_callback(struct psmi_timer *timer,
	uint64_t context);

/**
 * Configure flows for an ipsaddr.
 *
 * @arg ipsaddr - the ipsaddr to configure the flows for
 * @arg proto - the protocol used
 *
 * @pre proto's flags must be set
 *
 * Flows should be configured:
 * - immediately upon creation of an ipsaddr
 * - whenever a connection is established and the receiver's characteristics
 *   (e.g. mtu) become known
 */
ustatic
void
ips_ipsaddr_configure_flows(struct ips_epaddr *ipsaddr, struct ips_proto *proto)
{
	/* PIO flow uses the normal priority path, to separate low
	 * priority path for bulk sdma data packets
	 */
	psm3_ips_flow_init(&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO], proto,
		ipsaddr, PSM_TRANSFER_PIO, PSM_PROTOCOL_GO_BACK_N,
		IPS_PATH_NORMAL_PRIORITY, EP_FLOW_GO_BACK_N_PIO);

}

/*
 * Teardown any unnecessary timers that could still be active and assign NULL
 * to pointers in flow structs. We do this mainly for PIO and DMA flows.
 * TidFlow teardowns are conducted in psm3_ips_protoexp_fini()
 */
static
void
ips_flow_fini(struct ips_epaddr *ipsaddr, struct ips_proto *proto)
{
	struct ips_flow *flow;
	int i;

	for (i = 0; i < EP_NUM_FLOW_ENTRIES; i++) {
		flow = &ipsaddr->flows[i];

		/* Cancel any stale flow->timers in flight */
		psmi_timer_cancel(proto->timerq, &flow->timer_ack);
		psmi_timer_cancel(proto->timerq, &flow->timer_send);

		flow->flush = NULL;
		flow->path = NULL;
		flow->ipsaddr = NULL;
	}
}

static
psm2_epaddr_t
ips_alloc_epaddr(struct ips_proto *proto, int master, psm2_epid_t epid,
	const char *hostname, unsigned long timeout, psm2_error_t *err_out);

/* we check connect_verno and parse the epid
 * caller can easily fetch connidx and psm_verno, so don't do those here
 */
static int ips_proto_connect_hdr_parse(void *payload, uint32_t paylen,
	psm2_epid_t *epid)
{
	struct ips_connect_hdr *hdr = (struct ips_connect_hdr *)payload;

	// need enough payload to include connect_verno so can check versions
	if (paylen < offsetof(struct ips_connect_hdr, connect_verno) + sizeof(hdr->connect_verno)) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_EPID_INVALID_VERSION,
			"Connect packet too small (%u), protocol is incompatible with %x.%x",
			paylen,
			IPS_CONNECT_VER_MAJOR(IPS_CONNECT_VERNO),
			IPS_CONNECT_VER_MINOR(IPS_CONNECT_VERNO));
		return PSM2_EPID_INVALID_CONNECT;
	}

	// connect_verno mismatch should not happen. Prior to attempting
	// a connection the sender exchanged epids and if their sizes or
	// or epid formats don't match it doesn't attempt a connect.
	// So if one node has IPv6 PSM provider and the other doesn't, the
	// epid versions won't match so we won't end up sending a connect.
	// However if both use IPv4 or LID based epids, and the sender doesn't
	// check epid sizes (OpenMPI has this gap), we may get a connect
	// to/from an old PSM rev from/to a new PSM rev.
	// A problem is that the connect result is in the req, not the
	// connect_hdr, so a failed connect due to connect_verno mismatch
	// can't really be replied to with an error in req->connect_result
	// so we just exit with a fatal error here.
	// for now we are strict about major rev, if we add additional optional
	// features they can be minor revs and may need more sophisticated handling
	if (IPS_CONNECT_VER_MAJOR(hdr->connect_verno) == IPS_CONNECT_VER_MAJOR(IPS_CONNECT_VERNO)) {
		*epid = psm3_epid_pack_words(hdr->epid_w[0], hdr->epid_w[1],
						hdr->epid_w[2]);
	} else {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_EPID_INVALID_VERSION,
			"Connect protocol (%x.%x) is incompatible with %x.%x",
			IPS_CONNECT_VER_MAJOR(hdr->connect_verno),
			IPS_CONNECT_VER_MINOR(hdr->connect_verno),
			IPS_CONNECT_VER_MAJOR(IPS_CONNECT_VERNO),
			IPS_CONNECT_VER_MINOR(IPS_CONNECT_VERNO));
		return PSM2_EPID_INVALID_CONNECT;
	}
	return PSM2_OK;
}

/*
 * Given a connection request, set mtu, communication index and hdr length
 * parameters.
 * paylen here includes the ips_connect_hdr
 *
 * The most subtle parameter is the mtu.  When set as 'req->mtu', the mtu
 * is our connecting peer's declared mtu (which may not be the same as our
 * mtu).  The approach is to take the smaller of both mtus when communicating
 * with that peer.  Also, when using pio, the size can be further restricted by
 * the pio send buffer sizes (i.e. 4K fabric MTU but only 2K PIO buffers).
 */
static
psm2_error_t
ips_ipsaddr_set_req_params(struct ips_proto *proto, ips_epaddr_t *ipsaddr,
	const struct ips_connect_reqrep *req, uint32_t paylen)
{
	psm2_ep_t ep;
	psm2_epaddr_t epaddr;
	psm2_error_t err = PSM2_OK;
	int i, start, count;
	uint64_t *data;

	// this can only be called once, while much of processing is safe
	// to repeat, the HAL and multi-rail processing can only be done once
	psmi_assert(! ipsaddr->set_req_params);

	psmi_assert_always(req->mtu > 0);
	// common_mtu will be further reduced by pr_mtu to set frag_size and RC mtu
	uint32_t common_mtu = min(req->mtu, proto->epinfo.ep_mtu);
	psmi_assert_always(req->static_rate > 0);
	enum psm3_ibv_rate common_rate =
		min_rate(req->static_rate, proto->epinfo.ep_link_rate);
	int ptype, pidx;


	/*
	 * For static routes i.e. "none" path resolution update all paths to
	 * have the same profile (mtu, sl etc.).
	 *
	 * For path record queries the epr_mtu and epr_sl are setup correctly
	 * from the path itself.
	 */
	for (ptype = IPS_PATH_LOW_PRIORITY; ptype < IPS_PATH_MAX_PRIORITY; ptype++) {
		for (pidx = 0; pidx < ipsaddr->pathgrp->pg_num_paths[ptype]; pidx++) {
			if (proto->ep->path_res_type == PSM2_PATH_RES_NONE) {
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu = common_mtu;
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate = common_rate;
			} else {
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu = 
					min(common_mtu, ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu);
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate =
					min_rate(common_rate, ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate);
			}
		}
	}

	/*
	 * We've got updated mtu/path records, need to re-initialize the flows to take
	 * into account _real_ (updated) remote endpoint characteristics
	 */
	ips_ipsaddr_configure_flows(ipsaddr, proto);

	/*
	 * Save peer's info.
	 */
	ipsaddr->connidx_outgoing = req->hdr.connidx;
	ipsaddr->runid_key = req->runid_key;
	/* ipsaddr->initpsn = req->initpsn; */

	err = psm3_epid_set_hostname(psm3_epid_nid(((psm2_epaddr_t)ipsaddr)->epid), (char *)req->hostname, 0);
	if (err) {
		_HFI_ERROR("unable to save hostname: %s (%d)\n", psm3_error_get_string(err), err);
		return err;
	}

	err = psmi_hal_ips_ipsaddr_set_req_params(proto, ipsaddr, req);
	if (err) {
		// _HFI_ERROR already output w/details
		return err;
	}

	/*
	 * Check if there is other rails to setup.
	 */
	paylen -= sizeof(struct ips_connect_reqrep);
	if (paylen == 0)
		goto done;

	/*
	 * Yes, other rail's gid/epid is attached.
	 */
	if (paylen % IPS_CONNECT_RAIL_ADDR_LEN) {
		_HFI_ERROR("odd length connect packet: %u\n", paylen);
		return PSM2_INTERNAL_ERR;
	}
	count = paylen / IPS_CONNECT_RAIL_ADDR_LEN;
	if (count > PSMI_MAX_QPS) {
		_HFI_ERROR("connect packet with too many rails: %u max %u\n", count, PSMI_MAX_QPS);
		return PSM2_INTERNAL_ERR;
	}

	// only master ep's connect can specify additional rails
	psmi_assert(proto->ep->mctxt_master == proto->ep);

	/*
	 * Both side are ordered, so just search from small to big.
	 */
	start = 0;
	data = (uint64_t *) (req + 1);

	// This seeds to randomly select 1st rail to send packets on,
	// so can get away with using a subset of epid bits so we
	// just use 1 word
	struct drand48_data drand48_data;
	srand48_r((long int)(psm3_epid_hash(ipsaddr->epaddr.epid) + psm3_epid_hash(proto->ep->epid)), &drand48_data);

	/* Loop over all secondary endpoints */
	for (ep = proto->ep->mctxt_next; ep != ep->mctxt_master; ep = ep->mctxt_next) {
		for (i = start; i < count; i++) {
			uint64_t *rail_addr = &data[IPS_CONNECT_RAIL_ADDR_SIZE * i];
			psm2_epid_t rail_epid;
			psmi_subnet128_t rail_subnet;

			// 3 64b word rail_addr with 3 64b word epid
			// epid contains subnet (IPv6 subnet prefix)
			rail_epid = psm3_epid_pack_words(rail_addr[0], rail_addr[1], rail_addr[2]);
			rail_subnet = psm3_epid_subnet(rail_epid);

			// match rails by address format and full subnet
			// and associate with matching local ep
			if (psm3_subnets_match(ep->subnet, rail_subnet)) {
				/* gid (subnet) match, create the epaddr */
				epaddr = ips_alloc_epaddr(
					&((struct ptl_ips *)(ep->ptl_ips.ptl))->proto, 0,
				       	rail_epid, NULL, 5000, &err);
				if (epaddr == NULL) {
					// _HFI_ERROR already output w/details
					return err;
				}

				/* link the new epaddr as a rail within the
				 * master rail's ipsaddr rail list (mctxt_*)
				 */
				IPS_MCTXT_APPEND(ipsaddr, (ips_epaddr_t *)epaddr);

				/* Setup message control info to the same struct */
				((ips_epaddr_t *) epaddr)->msgctl = ipsaddr->msgctl;
				ipsaddr->msgctl->ipsaddr_count++;

				/* randomize the rail to start traffic */
				long int rnum;
				lrand48_r(&drand48_data, &rnum);
				if ((rnum % count) == i) {
					ipsaddr->msgctl->ipsaddr_next =	(ips_epaddr_t *)epaddr;
				}

				/* update the starting point,
				 * all previous ones are not valid anymore */
				start = i + 1;
				break;
			}
		}

	}

done:
	_HFI_CONNDBG_BASIC("set params", ipsaddr, " rate=%u mtu=%u rails=%u",
		common_rate, common_mtu,
		(unsigned)((paylen / IPS_CONNECT_RAIL_ADDR_LEN) + 1));

#ifdef PSM_DEBUG
	ipsaddr->set_req_params = 1;
#endif
	return PSM2_OK;
}

// send a CONNECT_REQUEST
// when timeout specified, will loop here until message sent, calling
// progress function per loop.
static psm2_error_t
ips_proto_send_conn_request(struct ips_proto *proto,
	struct ips_flow *flow, uint16_t *msg_queue_mask, uint64_t timeout)
{
	psm2_error_t err = PSM2_OK;
	ips_scb_t ctrlscb;

	/* msg header plus rail_addr for all rails plus checksum */
	char payload[sizeof(struct ips_connect_reqrep)
		+ IPS_CONNECT_RAIL_ADDR_LEN*PSMI_MAX_QPS
		+ PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;
	bool sent = false;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, OPCODE_CONNECT_REQUEST, 0, payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	do {
		err = psm3_ips_proto_send_ctrl_message(flow, OPCODE_CONNECT_REQUEST,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			_HFI_CONNDBG_SENT_REQ("", flow->ipsaddr,
					(struct ips_connect_reqrep *)payload, "");
			PSM2_LOG_MSG("CONN Pkt Sent: op=0x%02x from: %s to: %s with rcnt=%u",
					OPCODE_CONNECT_REQUEST,
					psm3_epid_fmt_internal(proto->ep->epid, 0),
					psm3_epid_fmt_internal(flow->ipsaddr->epaddr.epid, 1),
					flow->ipsaddr->reconnect_count);
			sent = true;
			break;
		}
		if ((err = psmi_err_only(psm3_poll_internal(proto->ep, 1, 1)))) {
			break;
		}
	} while (get_cycles() < timeout);
	if (! sent) {
		_HFI_CONNDBG_SENT_REQ(" Can't Send", flow->ipsaddr,
				(struct ips_connect_reqrep *)payload, "");
	}

	return err;
}

// send a DISCONNECT_REQUEST
// when timeout specified, will loop here until message sent, calling
// progress function per loop.  timeout only used for DISCONNECT w/o force
static psm2_error_t
ips_proto_send_disc_request(struct ips_proto *proto,
	struct ips_flow *flow, uint8_t force, uint16_t *msg_queue_mask,
	uint64_t timeout)
{
	psm2_error_t err = PSM2_OK;
	ips_scb_t ctrlscb;

	/* msg header plus checksum */
	char payload[sizeof(struct ips_connect_reqrep)
		+ PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;
	bool sent = false;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, OPCODE_DISCONNECT_REQUEST, force,
		payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	do {
		err = psm3_ips_proto_send_ctrl_message(flow, OPCODE_DISCONNECT_REQUEST,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			_HFI_CONNDBG_SENT_DREQ( "", flow->ipsaddr, force, "");
			PSM2_LOG_MSG("CONN Pkt Sent: op=0x%02x from: %s to: %s with force=%u",
					OPCODE_DISCONNECT_REQUEST,
					psm3_epid_fmt_internal(proto->ep->epid, 0),
					psm3_epid_fmt_internal(flow->ipsaddr->epaddr.epid, 1),
					force);
			sent = true;
			break;
		}
		if ((err = psmi_err_only(psm3_poll_internal(proto->ep, 1, 1)))) {
			break;
		}
	} while (get_cycles() < timeout);
	if (! sent) {
		_HFI_CONNDBG_SENT_DREQ(" Can't Send", flow->ipsaddr, force, "");
	}

	return err;
}

#define PSM_IPS_CONNECT_SEND_CTRL_RETRYS 100
// send a CONNECT_REPLY
// this has a fixed number of loops, each of which expect transfer_frame
// to check for send completions which may release send resources to permit
// this message to be queued for send
static psm2_error_t
ips_proto_send_conn_reply(struct ips_proto *proto,
	struct ips_flow *flow,
	uint16_t *msg_queue_mask)
{
	/* This will try up to 100 times until the message is sent. The code
	 * is persistent because dropping replies will lead to a lack of
	 * overall progress on the connection. We do not want
	 * to poll from here, and we cannot afford a lengthy timeout, since
	 * this is called from the receive path.
	 */
	psm2_error_t err = PSM2_OK;
	int i;
	ips_scb_t ctrlscb;
	/* msg header plus gid+epid for all rails plus checksum */
	char payload[sizeof(struct ips_connect_reqrep)
		+ IPS_CONNECT_RAIL_ADDR_LEN*PSMI_MAX_QPS
		+ PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;
	bool sent = false;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, OPCODE_CONNECT_REPLY, 0, payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	for (i = 0; i < PSM_IPS_CONNECT_SEND_CTRL_RETRYS; i++) {
		err = psm3_ips_proto_send_ctrl_message(flow, OPCODE_CONNECT_REPLY,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			_HFI_CONNDBG_SENT_REP("", flow->ipsaddr,
					(struct ips_connect_reqrep *)payload, "");
			sent = true;
			break;
		}
	}
	if (! sent) {
		_HFI_CONNDBG_SENT_REP(" Can't Send", flow->ipsaddr,
				(struct ips_connect_reqrep *)payload, "");
	}

	return err;
}

// send a DISCONNECT_REPLY
// this has a fixed number of loops, each of which expect transfer_frame
// to check for send completions which may release send resources to permit
// this message to be queued for send
static psm2_error_t
ips_proto_send_disc_reply(struct ips_proto *proto,
	struct ips_flow *flow, uint16_t *msg_queue_mask)
{
	/* This will try up to 100 times until the message is sent. The code
	 * is persistent because dropping replies will lead to a lack of
	 * overall progress on the disconnection. We do not want
	 * to poll from here, and we cannot afford a lengthy timeout, since
	 * this is called from the receive path.
	 */
	psm2_error_t err = PSM2_OK;
	int i;
	ips_scb_t ctrlscb;
	/* msg header plus checksum */
	char payload[sizeof(struct ips_connect_reqrep)
		+ PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;
	bool sent = false;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, OPCODE_DISCONNECT_REPLY, 0, payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	for (i = 0; i < PSM_IPS_CONNECT_SEND_CTRL_RETRYS; i++) {
		err = psm3_ips_proto_send_ctrl_message(flow, OPCODE_DISCONNECT_REPLY,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			_HFI_CONNDBG_SENT_DREP("", flow->ipsaddr, "");
			sent = true;
			break;
		}
	}
	if (! sent) {
		_HFI_CONNDBG_SENT_DREP(" Can't Send", flow->ipsaddr, "");
	}

	return err;
}

// we can build connect messages for master ep and secondary ep's in
// a multi-rail/multi-QP ep.
// force argument is only used for DISCONNECT_REQUEST
static int
ips_proto_build_connect_message(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t opcode, uint8_t force, void *payload,
	size_t max_paylen)
{
	struct ips_connect_hdr *hdr = (struct ips_connect_hdr *)payload;
	struct ips_connect_reqrep *req = (struct ips_connect_reqrep *)payload;
	uint32_t paylen = 0;

	psmi_assert_always(proto != NULL);

	hdr->connect_verno = IPS_CONNECT_VERNO;
	hdr->psm_verno = PSMI_VERNO;
	hdr->connidx = 0;
	hdr->epid_w[0] = psm3_epid_w0(proto->ep->epid);
	hdr->epid_w[1] = psm3_epid_w1(proto->ep->epid);
	hdr->epid_w[2] = psm3_epid_w2(proto->ep->epid);

	switch (opcode) {
	case OPCODE_CONNECT_REPLY:
	case OPCODE_CONNECT_REQUEST:
		hdr->connidx = (uint32_t) ipsaddr->connidx_incoming;
		if (opcode == OPCODE_CONNECT_REQUEST) {
			req->connect_result = PSM2_OK;
			req->runid_key = proto->runid_key;
		} else {
			req->connect_result = ipsaddr->cerror_incoming;
			req->runid_key = ipsaddr->runid_key;
		}

		req->reconnect_count = ipsaddr->reconnect_count;
		req->sl = proto->epinfo.ep_sl;
		// we keep this simple and send our local PSM payload (MTU)
		// after connection negotiation of a common_mtu, the MTU will be
		// further reduced by pr_mtu to set frag_size and RC QP mtu
		req->mtu = proto->epinfo.ep_mtu;
		req->static_rate = proto->epinfo.ep_link_rate;
		req->job_pkey = proto->epinfo.ep_pkey;
		req->wiremode = proto->ep->wiremode;

		strncpy(req->hostname, psm3_gethostname(),
			sizeof(req->hostname) - 1);
		req->hostname[sizeof(req->hostname) - 1] = '\0';
		psmi_hal_ips_proto_build_connect_message(proto, ipsaddr, opcode, req);

		paylen = sizeof(struct ips_connect_reqrep);
		psmi_assert(PSMI_EPID_LEN <= IPS_CONNECT_RAIL_ADDR_LEN);

		/* the first message our peer successfully receives
		 * must have all the information for additional rails
		 * we are requesting.  It is ok if additional messages
		 * have this information in which case peer will ignore it
		 * so we include in all reconnect_count == 0 or 1 messages
		 * since once we get to reconnect_count of 2 or more peer
		 * must have received something from us.  No issue if
		 * wrap and unnecessarily include in 256th re-connection, etc
		 */
		if (ipsaddr->reconnect_count <= 1
			&& proto->ep->mctxt_master == proto->ep) {
			/* Attach all multi-context subnetids and epids. */
			psm2_ep_t ep = proto->ep->mctxt_next;
			uint64_t *data = (uint64_t *) (req + 1);

			while (ep != proto->ep) {
				psmi_assert(PSMI_EPID_LEN <= IPS_CONNECT_RAIL_ADDR_LEN);
				// 3 64b word rail_addr with 3 64b word epid
				// epid contains full subnet
				*data = psm3_epid_w0(ep->epid);
				paylen += sizeof(uint64_t);
				data++;

				*data = psm3_epid_w1(ep->epid);
				paylen += sizeof(uint64_t);
				data++;

				*data = psm3_epid_w2(ep->epid);
				paylen += sizeof(uint64_t);
				data++;
				psmi_assert_always(paylen <= max_paylen);
				ep = ep->mctxt_next;
			}
		}

		break;

	case OPCODE_DISCONNECT_REQUEST:
		if (! force)
			hdr->connidx |= IPS_DISCONNECT_NOFORCE;
		/* FALLTHROUGH */
	case OPCODE_DISCONNECT_REPLY:
		paylen = sizeof(struct ips_connect_hdr);
		// placeholder, typically ips_connect_hdr is sufficient
		psmi_hal_ips_proto_build_connect_message(proto, ipsaddr, opcode, req);
		break;

	default:
		// obvious local code bug, called with invalid opcode
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"Unexpected outbound connection opcode 0x%x\n",
			opcode);
		break;
	}

	return paylen;
}

void
MOCKABLE(psm3_ips_flow_init)(struct ips_flow *flow, struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, psm_transfer_type_t transfer_type,
	psm_protocol_type_t protocol, ips_path_type_t path_type,
	uint32_t flow_index)
{
	psmi_assert_always(protocol < PSM_PROTOCOL_LAST);
	psmi_assert_always(flow_index < EP_FLOW_LAST);

	SLIST_NEXT(flow, next) = NULL;
	flow->path = ips_select_path(proto, path_type, ipsaddr, ipsaddr->pathgrp);

	/* Select the fragment size for this flow. Flow is the common
	 * denominator between the local endpoint, the remote endpoint,
	 * the path between those and whether it's a PIO or DMA send.
	 * Hence, it "owns" the maximum transmission unit in its frag_size
	 * member.
	 */
	/* min of local MTU and path MTU */
	flow->frag_size = min(proto->epinfo.ep_mtu, flow->path->pr_mtu);

	flow->ipsaddr = ipsaddr;
	flow->transfer = transfer_type;
	flow->protocol = protocol;
	flow->flowid = flow_index;
	flow->xmit_seq_num.psn_val = 0;
	flow->recv_seq_num.psn_val = 0;
	flow->xmit_ack_num.psn_val = 0;
	flow->flags = 0;
	flow->credits = proto->flow_credits;
	flow->max_credits = proto->flow_credits;
	flow->ack_interval = max((proto->flow_credits >> 2) - 1, 1);
	flow->ack_counter = 0;
#ifdef PSM_BYTE_FLOW_CREDITS
	flow->credit_bytes = proto->flow_credit_bytes;
	flow->ack_interval_bytes = max((proto->flow_credit_bytes >> 2) - 1, 1);
	flow->ack_counter_bytes = 0;
#endif
#ifdef PSM_DEBUG
	flow->scb_num_pending = 0;
	flow->scb_num_unacked = 0;
#endif

	psmi_timer_entry_init(&flow->timer_ack, psm3_ips_proto_timer_ack_callback, flow);
	psmi_timer_entry_init(&flow->timer_send, psm3_ips_proto_timer_send_callback, flow);

	STAILQ_INIT(&flow->scb_unacked);
	SLIST_INIT(&flow->scb_pend);

	psmi_hal_ips_flow_init(flow, proto);
	return;
}
MOCK_DEF_EPILOGUE(psm3_ips_flow_init);

static
psm2_epaddr_t
ips_alloc_epaddr(struct ips_proto *proto, int master, psm2_epid_t epid,
	const char *hostname, unsigned long timeout, psm2_error_t *err_out)
{
	psm2_error_t err = PSM2_OK;
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	ips_path_grp_t *pathgrp;
	uint16_t lid;
	psmi_gid128_t gid;

	/* The PSM/PTL-level epaddr, ips-level epaddr, and per-peer msgctl
	 * structures are collocated in memory for performance reasons -- this is
	 * why ips allocates memory for all three together.
	 *
	 * The PSM/PTL structure data is filled in upon successfully ep connect in
	 * psm3_ips_ptl_connect().
	 */
	if (master) {
		struct ips_msgctl *msgctl;

		/* Although an ips_msgtl is allocated here, it can be safely casted to
		 * both an ips_epaddr and a psm2_epaddr.  It is eventually freed as an
		 * ips_epaddr. */
		msgctl = (struct ips_msgctl *)psmi_calloc(proto->ep,
			PER_PEER_ENDPOINT, 1, sizeof(struct ips_msgctl));
		if (msgctl == NULL) {
			_HFI_ERROR("Unable to allocate primary rail epaddr: %s (%d)\n",
				psm3_error_get_string(err), err);
			*err_out = PSM2_NO_MEMORY;
			return NULL;
		}

		ipsaddr = &msgctl->master_epaddr;
		epaddr = (psm2_epaddr_t) ipsaddr;

		_HFI_CONNDBG("ips_alloc_epaddr %p for epid=%s %s\n",
			epaddr, psm3_epid_fmt_internal(epid, 0), hostname ? hostname : "unknown");
		ipsaddr->msgctl = msgctl;

		/* initialize items in ips_msgctl_t */
		msgctl->ipsaddr_next = ipsaddr;
		msgctl->mq_send_seqnum = 0;
		msgctl->mq_recv_seqnum = 0;
		msgctl->am_send_seqnum = 0;
		msgctl->am_recv_seqnum = 0;
		msgctl->ipsaddr_count = 1;
		msgctl->outoforder_count = 0;
	} else {
		epaddr = (psm2_epaddr_t)psmi_calloc(proto->ep,
			PER_PEER_ENDPOINT, 1, sizeof(struct ips_epaddr));
		if (!epaddr) {
			_HFI_ERROR("Unable to allocate epaddr: %s (%d)\n",
				psm3_error_get_string(err), err);
			*err_out = PSM2_NO_MEMORY;
			return NULL;
		}
		ipsaddr = (ips_epaddr_t *) epaddr;
	}

	epaddr->ptlctl = ((struct ptl_ips *)(proto->ptl))->ctl;
	epaddr->proto = proto;
	epaddr->epid = epid;

	/* IPS-level epaddr */
	ipsaddr->next = ipsaddr;

	ipsaddr->ctrl_msg_queued = 0;
	ipsaddr->msg_toggle = 0;

	psmi_timer_entry_init(&(ipsaddr->timer_reconnect),
		psm3_ips_proto_reconnect_timer_callback, ipsaddr);

	/* get HAL specific addressing fields initialized in ipsaddr as well as
	 * fetching lid and gid for our path record query
	 */
	psmi_hal_ips_ipsaddr_init_addressing(proto, epid, ipsaddr, &lid, &gid);

	/* Get path record for <service, slid, dlid> tuple */
	err = proto->ibta.get_path_rec(proto,
		proto->epinfo.ep_base_lid, /* __be16 */
		__cpu_to_be16(lid),
		__cpu_to_be64(gid.hi),
		__cpu_to_be64(gid.lo),
		timeout,
		&pathgrp);
	if (err != PSM2_OK) {
		_HFI_ERROR("Unable to allocate epaddr: failed to get path record: %s (%d)\n",
				psm3_error_get_string(err), err);
		goto fail;
	}
	ipsaddr->pathgrp = pathgrp;

	/* Setup high priority path index, control messages use the high
	 * priority CONTROL path.
	 */
	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE)
		ipsaddr->hpp_index = 0;
	else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_DST)
		ipsaddr->hpp_index = ipsaddr->hash %
			ipsaddr->pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY];
	else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_SRC)
		ipsaddr->hpp_index = proto->epinfo.ep_hash %
			ipsaddr->pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY];
	else			/* Base LID  */
		ipsaddr->hpp_index = 0;

	/* initialize any HAL specific per ipsaddr RC or TCP connections */
	err = psmi_hal_ips_ipsaddr_init_connections(proto, epid, ipsaddr);
	if (err) {
		// _HFI_ERROR already output w/details
		goto fail;
	}

	/*
	 * Set up the flows on this ipsaddr
	 */
	ips_ipsaddr_configure_flows(ipsaddr, proto);

	/* clear connection state. */
	ipsaddr->cstate_outgoing = CSTATE_NONE;
	ipsaddr->cstate_incoming = CSTATE_NONE;

	/* Add epaddr to PSM's epid table */
	psm3_epid_add(proto->ep, epaddr->epid, epaddr);
	psmi_assert(psm3_epid_lookup(proto->ep, epaddr->epid) == epaddr);

	*err_out = PSM2_OK;
	return epaddr;

fail:
	psmi_hal_ips_ipsaddr_free(ipsaddr, proto);
	psmi_free(epaddr);
	*err_out = err;
	return NULL;
}

static
void ips_free_epaddr(psm2_epaddr_t epaddr, struct ips_proto *proto)
{
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *) epaddr;
	ips_flow_fini(ipsaddr, proto);

	_HFI_CONNDBG_BASIC("free", ipsaddr, " idx in=%u out=%u",
		ipsaddr->connidx_incoming, ipsaddr->connidx_outgoing);
	IPS_MCTXT_REMOVE(ipsaddr);
	psmi_timer_cancel(proto->timerq, &ipsaddr->timer_reconnect);
	psmi_hal_ips_ipsaddr_free(ipsaddr, proto);
	psm3_epid_remove(epaddr->proto->ep, epaddr->epid);
	psm3_ips_epstate_del(epaddr->proto->epstate, ipsaddr->connidx_incoming);
	psmi_free(epaddr);
	return;
}

static
psm2_error_t
ips_ipsaddr_match_req_params(uint8_t opcode, const char *op,
	struct ips_proto *proto, ips_epaddr_t *ipsaddr,
	const struct ips_connect_reqrep *req, uint32_t paylen);
static
psm2_error_t
ptl_handle_connect_req(struct ips_proto *proto,
	psm2_epaddr_t epaddr, psm2_epid_t epid,
	struct ips_connect_reqrep *req, uint32_t paylen);

// handle a variety of situations where the HAL specific connection must
// be reconnected.  If triggered due to an inbound REQ, it is supplied
// if triggered due to a HAL discovered issue (such as QP error), req is NULL.
static
psm2_error_t
psm3_ips_proto_start_reconnect(struct ips_proto *proto, ips_epaddr_t *ipsaddr,
	const struct ips_connect_reqrep *req, unsigned flags)
{	
	psm2_error_t err;

	if (! ipsaddr->allow_reconnect) {
		_HFI_CONN_FATAL(PSM2_INTERNAL_ERR, 
			"reconnect not allowed/expected",
			ipsaddr, "local epid %s wiremode=%u",
			psm3_epid_fmt_internal(proto->ep->epid, 0),
			proto->ep->wiremode);
		return PSM2_INTERNAL_ERR;
	}
	if (req)
		_HFI_CONNDBG_RCV_REQ(" starting reconnect", ipsaddr, req, "");
	else
		_HFI_CONNDBG_BASIC("Err, starting reconnect", ipsaddr, "");
	proto->epaddr_stats.reconnect_started++;
	psmi_assert(proto->ep->reconnect_timeout);

	ipsaddr->reconnect_count = ips_epaddr_next_reconnect_count(ipsaddr->reconnect_count);
	// due to wrap, the max here is capped at 255, but gives a reasonable
	// indicator if we have some connections which have had lots of issues
	proto->epaddr_stats.max_reconnect_count = max(ipsaddr->reconnect_count,
			proto->epaddr_stats.max_reconnect_count);
	psmi_assert(! req || ipsaddr->reconnect_count == req->reconnect_count);

	err = psmi_hal_ips_ipsaddr_start_reconnect(proto, ipsaddr, req, flags);
	if (err) {
		// _HFI_ERROR already output w/details
		return err;
	}
	// if we have not yet attempted to establish a connection
	// can't issue a CONNECT_REQUEST here.  Peer will discover need for
	// reconnection when we attempt connect in future (with count>0) or
	// when peer discovers a similar HAL connection error
	if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED
		|| ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING)
	{
		// psm2_connect has it's own timeout handling, so don't need a
		// timer here if psm2_connect is still monitoring this ipsaddr
		if (! ipsaddr->waiting_in_connect) {
			// starting a fresh reconnect, reset timeouts
			ipsaddr->s_start = get_cycles();
			ipsaddr->delay_in_ms = 1;
			ipsaddr->s_timeout = get_cycles() +
				nanosecs_to_cycles(proto->ep->reconnect_timeout * NSEC_PER_SEC);
			if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING)
				psmi_timer_cancel(proto->timerq, &ipsaddr->timer_reconnect);
		}
		ipsaddr->cstate_outgoing = CSTATE_OUTGOING_WAITING;
		ipsaddr->reconnecting = 1;
		// we can't immediately send the REQ here because we may have been
		// called in send completion handling, so we must avoid a
		// recursive call to transfer_frame by scheduling the timer callback
		// immediately at the end of the internal progress poll
		if (! ipsaddr->waiting_in_connect) {
			psmi_timer_request(proto->timerq, &ipsaddr->timer_reconnect,
				PSMI_TIMER_PRIO_1);
		} else {
			// rare case, got connection error while still working on
			// original connection.
			// force psm2_connect to immediately issue the new REQ.
			ipsaddr->s_timeout = get_cycles();
			ipsaddr->delay_in_ms = 1;
		}
	}
	if (! req && ipsaddr->cstate_incoming == CSTATE_ESTABLISHED)
		ipsaddr->cstate_incoming = CSTATE_INCOMING_NEED_RECONNECT;
	return PSM2_OK;
}

void
psm3_ips_proto_ipsaddr_start_disconnect(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t force, int outgoing)
{
	if (outgoing) {
		psmi_assert(! ipsaddr->waiting_in_connect);
		if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
			// noop if timer not running
			psmi_timer_cancel(proto->timerq, &ipsaddr->timer_reconnect);
		}
		ipsaddr->reconnecting = 0;
	}
	psmi_hal_ips_ipsaddr_start_disconnect(proto, ipsaddr, force);
}

void
psm3_ips_proto_ipsaddr_done_disconnect(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t force, int outgoing)
{
	psmi_hal_ips_ipsaddr_done_disconnect(proto, ipsaddr, force);
	if (outgoing) {
		ipsaddr->s_timeout = 0;
	}
}

// return indicates if caller should free ipsaddr now:
// 0 - ipsaddr is now in timewait delay, caller cannot free
//	when timeout expires ipsaddr will be freed by timer callback
// 1 - ipsaddr will not have a timewait, caller may free when caller is
//	done using ipsaddr
// in some cases, caller may need to do some more things with ipsaddr
// before it can free, so we give it the chance instead of freeing here
// the act of freeing is equivalent to moving the ipsaddr to NONE/NONE
int
psm3_ips_proto_ipsaddr_finalize(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t force)
{
	// this is only valid once ipsaddr is fully disconnected
	// as reflected by DISCONNECTED state or NONE (never connected)
	psmi_assert(ipsaddr->cstate_incoming == CSTATE_NONE
			|| ipsaddr->cstate_incoming == CSTATE_DISCONNECTED);
	psmi_assert(ipsaddr->cstate_outgoing == CSTATE_NONE
			|| ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED);
	// confirm tracking flags are consistent with being fully disconnected
	psmi_assert(! ipsaddr->waiting_in_connect);
	psmi_assert(! ipsaddr->waiting_in_disconnect);
	psmi_assert(! ipsaddr->reconnecting);
	psmi_assert(! ipsaddr->s_timeout);

	ipsaddr->reconnect_count = 0;	// paranoid
	if (! force && ipsaddr->allow_reconnect) {
		// start timewait timer
		_HFI_CONNDBG_BASIC("fully disconnected, start timewait", ipsaddr, "");
		psmi_timer_request(proto->timerq, &ipsaddr->timer_reconnect,
			get_cycles() + nanosecs_to_cycles(IPS_CONNECT_TIMEWAIT));
		return 0;	// timer callback will free ipsaddr
	} else {
		// a forced disconnect runs the risk of destroying the QP
		// before it is drained.
		_HFI_CONNDBG_BASIC("fully disconnected", ipsaddr, "");
		return 1;	// caller may free ipsaddr
	}
}

// process an inbound connection packet (REQ, REP, DREQ or DREP
psm2_error_t
psm3_ips_proto_process_connect(struct ips_proto *proto, uint8_t opcode,
	struct ips_message_header *p_hdr, void *payload, uint32_t paylen)
{
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	psm2_error_t err = PSM2_OK;
	psm2_epid_t epid;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	if (PSM2_OK != ips_proto_connect_hdr_parse(payload, paylen, &epid)) {
		// we can't parse header, so we can't get an epid
		// we are stuck, must discard packet.
		// should not get here, fatal error already output
		_HFI_CONNDBG("CONN Rcv 0x%x Ignored, mismatched connect_verno: from Unknown to %s\n", \
			opcode, psm3_epid_fmt_internal(proto->ep->epid, 0));
		return PSM2_OK;
	}


	epaddr = psm3_epid_lookup(proto->ep, epid);
	ipsaddr = epaddr ? (ips_epaddr_t *) epaddr : NULL;

#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(proto->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_connrecvlost, "connrecvlost",
			"drop inbound connection packet",
			1, IPS_FAULTINJ_CONNRECVLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_connrecvlost, proto->ep,
				" %p opccode=0x%x epid=%s (%s)",
				ipsaddr, opcode,
				psm3_epid_fmt_internal(epid, 0),
				psm3_epid_fmt_addr(epid, 1)))
			return PSM2_OK;
	}
	if_pf(PSM3_FAULTINJ_ENABLED_EP(proto->ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_slowconn, "slowconn",
			"slow processing of recv connect packets",
			1, IPS_FAULTINJ_SLOWCONN);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_slowconn, proto->ep,
				" %p opccode=0x%x epid=%s (%s)",
				ipsaddr, opcode,
				psm3_epid_fmt_internal(epid, 0),
				psm3_epid_fmt_addr(epid, 1))) {
			// slow down a bit so remote retries and we see dups
			// we may each also see timeouts since total connect
			// or disconnect effort may run out of time to get all
			// started and done
			uint64_t delay = get_cycles()
				+ nanosecs_to_cycles(IPS_SLOWCONN_DELAY * NSEC_PER_MSEC);
			while (get_cycles() < delay)
				;	// just delay, caller will be in poll
		}
	}
#endif // PSM_FI 

	switch (opcode) {
	case OPCODE_CONNECT_REQUEST:
		PSM2_LOG_MSG("Got a connect from %s", psm3_epaddr_get_name(epid, 0));
		proto->epaddr_stats.connect_req_recv++;
		err = ptl_handle_connect_req(proto, epaddr, epid,
				(struct ips_connect_reqrep *)payload, paylen);
		break;

	case OPCODE_CONNECT_REPLY:
		{
			struct ips_connect_reqrep *req =
				(struct ips_connect_reqrep *)payload;

			proto->epaddr_stats.connect_rep_recv++;
			if (!ipsaddr ) {
				_HFI_CONNDBG_RCV_REP_EPID(
					" Ignored unknown epaddr", proto,
					epid, req, "");
				proto->epaddr_stats.connect_rep_unknown++;
			} else if (req->runid_key != proto->runid_key) {
				_HFI_CONNDBG_RCV("REP",
					" Ignored, mismatched runid_key",
					ipsaddr, req,
					" exp %u rcv %u",
					ipsaddr->runid_key, req->runid_key);
				proto->epaddr_stats.connect_rep_inconsistent++;
			} else if (ipsaddr->cstate_outgoing != CSTATE_OUTGOING_WAITING) {
				// possible dupe or late response to reconnect
				_HFI_CONNDBG_RCV("REP", " Ignored", ipsaddr,
					req, " outgoing state not W");
				if (req->reconnect_count)
					proto->epaddr_stats.reconnect_rep_dup++;
				else
					proto->epaddr_stats.connect_rep_dup++;
			} else if (ipsaddr->waiting_in_disconnect) {
				// ignore, started to disconnect,
				// no need to finish (re)connect.
				// Packet could be a late original connect REP
				// (possibly a late DUP) or a reconnect REP we
				// don't care about since now disconnecting.
				_HFI_CONNDBG_RCV("REP",
					" Ignored, disconnecting", ipsaddr,
					req, "");
				if (req->reconnect_count)
					proto->epaddr_stats.reconnect_rep_disc++;
				else
					proto->epaddr_stats.connect_rep_disc++;
			} else if (req->reconnect_count == ipsaddr->reconnect_count) {
				/* Reply to our request for connection (i.e. outgoing connection) */
				if (ipsaddr->reconnecting) {
					if (req->connect_result != PSM2_OK) {
						// fatal error
						_HFI_CONN_FATAL(PSM2_INTERNAL_ERR,
							"Couldn't reconnect, REP with bad result",
							ipsaddr, " result=%s (%d)",
							psm3_error_get_string(req->connect_result),
							req->connect_result);
					}
					// confirm REP consistent with
					// previous REQ or REPs from peer
					if (ips_ipsaddr_match_req_params(OPCODE_CONNECT_REPLY, "REP", proto, ipsaddr, req, paylen)) {
						// CONNDBG already output w/details
						proto->epaddr_stats.connect_rep_inconsistent++;
						break;	// ignore this REP
					}
					proto->epaddr_stats.max_reconnect_usec =
						max(proto->epaddr_stats.max_reconnect_usec,
							cycles_to_nanosecs(get_cycles() - ipsaddr->s_start) / NSEC_PER_USEC);
				}
				if (ipsaddr->cstate_incoming != CSTATE_ESTABLISHED) {
					// only do full params processing on
					// 1st REQ or REP from peer otherwise
					// just do HAL processing
					// if reconnect_count we must have
					// set HAL params (QP to RTR), then
					// gotten HAL error or gotten a
					// REQ which bumped the count
					if (ipsaddr->cstate_incoming != CSTATE_NONE || ipsaddr->reconnect_count)
						err = psmi_hal_ips_ipsaddr_set_req_params(proto, ipsaddr, req);
					else
						err = ips_ipsaddr_set_req_params(proto, ipsaddr, req, paylen);
					if (err) {
						// _HFI_ERROR already output w/details
						goto fail;
					}
				}
				_HFI_CONNDBG_RCV("REP", "", ipsaddr, req, 
					" result=%s (%d) idx in=%u out=%u flow=%p",
					psm3_error_get_string(req->connect_result),
					req->connect_result,
					ipsaddr->connidx_incoming,
					ipsaddr->connidx_outgoing,
					&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO]);
				psmi_timer_cancel(proto->timerq, &ipsaddr->timer_reconnect);
				ipsaddr->reconnecting = 0;
				err = psmi_hal_ips_ipsaddr_process_connect_reply(
							proto, ipsaddr, req);
				if (err == PSM2_EPID_RC_CONNECT_ERROR) {
					// QP got an error while in RTR which
					// we have not yet discovered, so unable
					// to move to RTS.  We treat this as if
					// the error occurred immediately after
					// moving to RTS
					_HFI_CONNDBG("[ipsaddr %p] REP - HAL reports need reconnect",
									ipsaddr);
					proto->epaddr_stats.rep_proc_err++;
					proto->epaddr_stats.connections_lost++;
					ipsaddr->cstate_outgoing = CSTATE_ESTABLISHED;
					ipsaddr->cerror_outgoing = req->connect_result;
					err = psm3_ips_proto_start_reconnect(proto, ipsaddr, NULL, 0);
					if (err) {
						// _HFI_ERROR already output w/details
						_HFI_CONNDBG("[ipsaddr %p] failed reconnect: %s (%d)",
							ipsaddr,
							psm3_error_get_string(err),
							err);
						goto fail;
					}
				} else if (err) {
					// _HFI_ERROR already output w/details
					goto fail;
				} else {
					ipsaddr->cstate_outgoing = CSTATE_ESTABLISHED;
					ipsaddr->cerror_outgoing = req->connect_result;
				}
			} else {
				// ignore - stale count
				// note that a REP with count+1 is not possible since
				// peer would have discarded a REQ with count-1
				_HFI_CONNDBG_RCV("REP",
					" Ignored Stale Reconnect", ipsaddr,
					req, "");
				if (req->reconnect_count)
					proto->epaddr_stats.reconnect_rep_stale++;
				else
					proto->epaddr_stats.connect_rep_stale++;
			}
		}
		break;

	case OPCODE_DISCONNECT_REQUEST:
		{
			ips_epaddr_t ipsaddr_f;	/* fake a ptl addr */
			int epaddr_do_free = 0;
			int force;
			struct ips_connect_hdr *hdr = (struct ips_connect_hdr *)payload;
			psmi_assert_always(paylen == sizeof(struct ips_connect_hdr));
			force = ! (hdr->connidx & IPS_DISCONNECT_NOFORCE);
			proto->num_disconnect_requests++;
			proto->epaddr_stats.disconnect_req_recv++;
			/* It's possible to get a disconnection request on a ipsaddr that
			 * we've since removed if the request is a dupe.  Instead of
			 * silently dropping the packet, we "echo" the request in the
			 * reply. */
			if (ipsaddr == NULL) {
				ips_path_grp_t *pathgrp;
				uint16_t lid;
				psmi_gid128_t gid;

				ipsaddr = &ipsaddr_f;
				memset(&ipsaddr_f, 0, sizeof(ips_epaddr_t));
				((psm2_epaddr_t) &ipsaddr_f)->epid = epid;
				((psm2_epaddr_t) &ipsaddr_f)->proto = proto;
				((psm2_epaddr_t) &ipsaddr_f)->ptlctl =
					((struct ptl_ips *)(proto->ptl))->ctl;
				ipsaddr_f.hash = psm3_epid_context(epid);
				ipsaddr_f.reconnect_count = 0;
				ipsaddr_f.cstate_outgoing = CSTATE_NONE;
				ipsaddr_f.cstate_incoming = CSTATE_NONE;

				// log needs epid, proto and reconnect_count
				// so can log before any errors in calls below
				_HFI_CONNDBG_RCV_DREQ(" unknown epaddr", ipsaddr,
					force, " hostname=%s",
					psm3_epaddr_get_name(epid, 0));
				psmi_hal_ips_ipsaddr_init_addressing(proto,
							epid, &ipsaddr_f, &lid,
							&gid);
				/* Get path record for peer */
				err = proto->ibta.get_path_rec(proto,
					proto->epinfo.ep_base_lid, /* __be16 */
					__cpu_to_be16(lid),
					__cpu_to_be64(gid.hi),
					__cpu_to_be64(gid.lo),
					3000, &pathgrp);
				if (err != PSM2_OK)
					goto fail;

				ipsaddr_f.pathgrp = pathgrp;
				/* If the send fails because of pio_busy, don't let ips queue
				 * the request on an invalid ipsaddr, just drop the reply */
				ipsaddr_f.ctrl_msg_queued = ~0;

				psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);

				psm3_ips_flow_init(&ipsaddr_f.flows[proto->msgflowid],
					proto, &ipsaddr_f, PSM_TRANSFER_PIO, PSM_PROTOCOL_GO_BACK_N,
					IPS_PATH_LOW_PRIORITY, EP_FLOW_GO_BACK_N_PIO);
				proto->epaddr_stats.disconnect_req_dup++;
			} else if (ipsaddr->cstate_incoming == CSTATE_ESTABLISHED
				|| ipsaddr->cstate_incoming == CSTATE_INCOMING_NEED_RECONNECT)
			{
				_HFI_CONNDBG_RCV_DREQ("", ipsaddr, force, "");
				psm3_ips_proto_ipsaddr_start_disconnect(proto, ipsaddr, force, 0);
				psm3_ips_proto_ipsaddr_done_disconnect(proto, ipsaddr, force, 0);
				ipsaddr->cstate_incoming = CSTATE_DISCONNECTED;
				proto->num_connected_incoming--;
				// if waiting_in_disconnect,
				// psm3_ips_proto_disconnect will finalize
				// and free when it's done with this ipsaddr
				if ( ! ipsaddr->waiting_in_disconnect
					&& (ipsaddr->cstate_outgoing == CSTATE_NONE
					|| ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED))
				{
					if (psm3_ips_proto_ipsaddr_finalize(proto, ipsaddr, force))
						epaddr_do_free = 1;
				} else {
					_HFI_CONNDBG_BASIC("incoming disconnected", ipsaddr, "");
				}
			} else {
				// possible dupe, just send DISCONNECT_REPLY
				psmi_assert(ipsaddr->cstate_incoming == CSTATE_NONE
					|| ipsaddr->cstate_incoming == CSTATE_DISCONNECTED);
				_HFI_CONNDBG_RCV_DREQ(" duplicate", ipsaddr, force, "");
				proto->epaddr_stats.disconnect_req_dup++;
			}

			psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);

			ips_proto_send_disc_reply(proto,
				&ipsaddr->flows[proto->msgflowid],
				&ipsaddr->ctrl_msg_queued);
			/* We can safely free the ipsaddr if required since disconnect
			 * messages are never enqueued so no reference to ipsaddr is kept */
			if (epaddr_do_free) {
				ips_free_epaddr(epaddr, proto);
				epaddr = NULL;
			}
		}
		break;

	case OPCODE_DISCONNECT_REPLY:
		proto->epaddr_stats.disconnect_rep_recv++;
		if (!ipsaddr) {
			_HFI_CONNDBG_RCV_DREP_EPID(" Ignored unknown epaddr", proto, epid, "");
			proto->epaddr_stats.disconnect_rep_unknown++;
			break;
		} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC) {
			_HFI_CONNDBG_RCV_DREP("", ipsaddr, "");
			ipsaddr->cstate_outgoing = CSTATE_DISCONNECTED;
			// psm3_ips_proto_disconnect() will take it from here
		} else {
			// ignore dupe reply 
			_HFI_CONNDBG_RCV_DREP(" Ignored duplicate", ipsaddr, "");
			proto->epaddr_stats.disconnect_rep_dup++;
		}
		break;

	default:
		// stray packet or invalid protocol at peer
		_HFI_CONN_FATAL_EPID(PSM2_INTERNAL_ERR,
			"Unexpected inbound connect packet",
			proto, epid, " opcode 0x%x", opcode);
	}

fail:
	return err;
}

// validate basic parameters from REQ, including epid
static
psm2_error_t
ips_validate_req_params(struct ips_proto *proto, psm2_epid_t epid,
	const struct ips_connect_reqrep *req)
{
	/* Check psm version */
	if (!psm3_verno_isinteroperable(req->hdr.psm_verno)) {
		return PSM2_EPID_INVALID_VERSION;
	} else if (psm3_epid_addr_fmt(epid) != proto->ep->addr_fmt) {
		// before connections started, sender should have confirmed
		// epid formats match for master and each rail
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_EPID_INVALID_VERSION,
				"Remote Connect (%s %s) epid address format %s (%u) is incompatible with local epid address format %s (%u)",
				req->hostname,
				psm3_epid_fmt_addr(epid, 0),
				psm3_epid_str_addr_fmt(epid),
				psm3_epid_addr_fmt(epid),
				psm3_epid_str_addr_fmt(proto->ep->epid),
				proto->ep->addr_fmt);
		return PSM2_EPID_INVALID_CONNECT;
	} else if (psm3_epid_protocol(epid) != psm3_epid_protocol(proto->ep->epid)) {
		// before connections started, sender should have confirmed
		// epid formats match for master and each rail
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_EPID_INVALID_VERSION,
				"Remote Connect (%s %s) epid protocol %s (%u) is incompatible with local epid protocol %s (%u)",
				req->hostname,
				psm3_epid_fmt_addr(epid, 0),
				psm3_epid_str_protocol(epid),
				psm3_epid_protocol(epid),
				psm3_epid_str_protocol(proto->ep->epid),
				psm3_epid_protocol(proto->ep->epid));
		return PSM2_EPID_INVALID_CONNECT;
	} else if (!(proto->flags & IPS_PROTO_FLAG_QUERY_PATH_REC) &&
			proto->epinfo.ep_pkey != psmi_hal_get_default_pkey() &&
			proto->epinfo.ep_pkey != req->job_pkey) {
		_HFI_ERROR("Remote Connection (%s %s) error: PKey mismatch (local:0x%04x, remote:0x%04x)\n",
			req->hostname, psm3_epid_fmt_addr(epid, 0),
			proto->epinfo.ep_pkey, req->job_pkey);
		return PSM2_EPID_INVALID_PKEY;
	} else if (req->sl != proto->epinfo.ep_sl) {
		_HFI_ERROR("Remote Connection (%s %s) error: Service Level mismatch (local:%d, remote:%d)\n",
			req->hostname, psm3_epid_fmt_addr(epid, 0),
			proto->epinfo.ep_sl, req->sl);
		return PSM2_EPID_INVALID_CONNECT;
	} else if (req->wiremode != proto->ep->wiremode) {
		_HFI_ERROR("Remote Connection error (%s %s): %s Wire Mode mismatch (local:%d, remote:%d)\n",
			req->hostname, psm3_epid_fmt_addr(epid, 0),
			psm3_epid_str_protocol(epid),
			proto->ep->wiremode, req->wiremode);
		return PSM2_EPID_INVALID_CONNECT;
	} else {
		return PSM2_OK;
	}
}

/*
 * Given a re-connection request or reply, confirm rank runid_key (pid),
 * mtu, communication index parameters, etc are consistent with the initial
 * connection established.
 * paylen here includes the ips_connect_hdr
 *
 * returns PSM2_OK on match
 *
 * The most subtle parameter is the mtu.  When set as 'req->mtu', the mtu
 * is our connecting peer's declared mtu (which may not be the same as our
 * mtu).  The approach is to take the smaller of both mtus when communicating
 * with that peer. Also, when using pio, the size can be further restricted by
 * the pio send buffer sizes (i.e. 4K fabric MTU but only 2K PIO buffers).
 *
 * The intial connection should have negotiated these parameters so we check
 * the new parameters given match the original connection parameters.
 *
 * This routine should only be used for inbound reconnect REQ or REP or
 * inbound duplicate initial REQ.
 */
static
psm2_error_t
ips_ipsaddr_match_req_params(uint8_t opcode, const char *op,
	struct ips_proto *proto, ips_epaddr_t *ipsaddr,
	const struct ips_connect_reqrep *req, uint32_t paylen)
{
	uint32_t common_mtu = min(req->mtu, proto->epinfo.ep_mtu);
	enum psm3_ibv_rate common_rate = min_rate(req->static_rate,
						proto->epinfo.ep_link_rate);
	int ptype, pidx;

	psmi_assert(ipsaddr->set_req_params);

	// no need to check verno and epid, the fact we found in epaddr
	// table means epid is sensible and we will confirm rank matches
	// which should imply verno is ok too.

	if ((opcode == OPCODE_CONNECT_REQUEST
			&& ipsaddr->runid_key != req->runid_key)
	    || (opcode == OPCODE_CONNECT_REPLY
			&& proto->runid_key != req->runid_key)) {
		_HFI_CONNDBG_RCV(op, " mismatched runid_key", ipsaddr, req,
			" exp %u rcv %u", ipsaddr->runid_key, req->runid_key);
		goto fail;
	}

	if (req->hostname[0]) {
		char *h = psm3_epid_lookup(PSMI_EP_HOSTNAME, psm3_epid_nid(((psm2_epaddr_t) ipsaddr)->epid));
		if (! h || strcmp(h, (char *)req->hostname)) {
			_HFI_CONNDBG_RCV(op, " mismatched hostname", ipsaddr,
				req, " exp %s rcv %s",
				h, (char *)req->hostname);
			goto fail;
		}
	}

	if (ipsaddr->connidx_outgoing != req->hdr.connidx) {
		_HFI_CONNDBG_RCV(op, " mismatched connidx", ipsaddr, req,
			" exp %u rcv %u",
			ipsaddr->connidx_outgoing, req->hdr.connidx);
		goto fail;
	}

	if (!(proto->flags & IPS_PROTO_FLAG_QUERY_PATH_REC) &&
			proto->epinfo.ep_pkey != psmi_hal_get_default_pkey() &&
			proto->epinfo.ep_pkey != req->job_pkey) {
		_HFI_CONNDBG_RCV(op, " mismatched pkey", ipsaddr, req,
			" exp 0x%x rcv 0x%x",
			ipsaddr->epaddr.proto->epinfo.ep_pkey, req->job_pkey);
		goto fail;
	}

	// the MTU and rate are negtiated per path, so a re-connect will
	// still offer the remote endpoint MTU as found in the original REQ/REP
	// but locally we should have reduced each paths MTU and rate to be <
	// the common mtu/rate.
	// We can't ensure the remote offered the same values as originally
	// but will at least discover if the pr mtu and rate we negotiated are
	// now too large
	psmi_assert_always(req->mtu > 0);
	// common_mtu will be further reduced by pr_mtu to set frag_size and RC mtu
	psmi_assert_always(req->static_rate > 0);
	for (ptype = IPS_PATH_LOW_PRIORITY; ptype < IPS_PATH_MAX_PRIORITY;
				ptype++) {
		for (pidx = 0; pidx < ipsaddr->pathgrp->pg_num_paths[ptype]; pidx++) {
			if (ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu > common_mtu) {
				_HFI_CONNDBG_RCV(op, " mismatched mtu", ipsaddr, req,
					" pr %u min %u",
					ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu,
					common_mtu);
				goto fail;
			}
			if (ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate >
					common_rate) {
				_HFI_CONNDBG_RCV(op, " mismatched rate", ipsaddr, req,
					" pr %u min %u",
					ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate,
					common_rate);
				goto fail;
			}
		}
	}

	if (req->sl != proto->epinfo.ep_sl) {
		_HFI_CONNDBG_RCV(op, " mismatched sl", ipsaddr, req,
			" exp %u rcv %u",
			ipsaddr->epaddr.proto->epinfo.ep_sl, req->sl);
		goto fail;
	}
	if (req->wiremode != proto->ep->wiremode) {
		_HFI_CONNDBG_RCV(op, " mismatched wiremode", ipsaddr, req,
			" exp %u rcv %u",
			ipsaddr->epaddr.proto->ep->wiremode, req->wiremode);
		goto fail;
	}

	// for a reconnect HAL qp_attr will all be different, can't check

	// this routine can be used for duplicate initial REQ as well as
	// reconnect.  Plus when reconnect_count <= 1,
	// ips_proto_build_connect_message includes multi-rail data even if
	// its a reconnect REQ, so we must ignore if
	// (paylen > sizeof(struct ips_connect_reqrep)) here and accept
	// packets which may have multi-rail data.
			
	return PSM2_OK;

fail:
	/* Some out of context message or from wrong rank. */
	/* May be duplicate addresses/lids in fabric or other issue */
	if (!proto->done_warning) {
		psm3_syslog(proto->ep, 1, LOG_INFO,
			"Non-fatal connection problem: Received an out-of-context "
			"connection message from host %s %s (Ignoring)",
			req->hostname,
			psm3_epid_fmt_addr(ipsaddr->epaddr.epid, 0));
		proto->done_warning = 1;
	}
	return PSM2_INTERNAL_ERR;
}

static
psm2_error_t
ptl_handle_connect_req(struct ips_proto *proto, psm2_epaddr_t epaddr,
	psm2_epid_t epid, struct ips_connect_reqrep *req, uint32_t paylen)
{
	ips_epaddr_t *ipsaddr;
	psm2_error_t err = PSM2_OK;
	int newconnect = 0;	// only used for an assert
	int alloced = 0;	// did we alloc in this func

	if (!psm3_epid_cmp_internal(epid, proto->ep->epid)) {
		// peer epid matches our own, should not happen
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_EPID_NETWORK_ERROR,
				"Network connectivity problem: Locally detected duplicate "
				"address %s on hosts %s and %s (%s port %u). (Exiting)",
				psm3_epid_fmt_nid(epid, 0),
				psm3_epaddr_get_hostname(epid, 1),
				psm3_gethostname(), proto->ep->dev_name, proto->ep->portnum);
		/* XXX no return */
		abort();
	} else if (epaddr == NULL) {	/* new ep connect before we call into connect */
		newconnect = 1;
		if ((epaddr =
			ips_alloc_epaddr(proto, 1, epid, req->hostname,
						5000, &err)) == NULL) {
			goto fail;
		}
		alloced = 1;
	}

	ipsaddr = (ips_epaddr_t *) epaddr;
	if (req->reconnect_count != ipsaddr->reconnect_count
	    && req->reconnect_count !=
		ips_epaddr_next_reconnect_count(ipsaddr->reconnect_count)) {
		// stale reconnect, discard
		proto->epaddr_stats.connect_req_stale++;
		_HFI_CONNDBG_RCV_REQ(" Stale Reconnect", ipsaddr, req, "");
		goto no_reply;
	}
	if (ipsaddr->cstate_incoming == CSTATE_DISCONNECTED) {
		// waiting for timewait, must ignore REQ for now
		_HFI_CONNDBG_RCV_REQ(" Ignored, disconnected", ipsaddr, req, "");
		proto->epaddr_stats.connect_req_disc++;
		goto no_reply;
	}
	if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC
		|| ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED
		|| ipsaddr->waiting_in_disconnect) {
		// we are disconnecting or in timewait
		// let remote timeout and discover our DISCONNECT_REQUEST
		_HFI_CONNDBG_RCV_REQ(" Ignored, disconnecting", ipsaddr, req, "");
		proto->epaddr_stats.connect_req_disc++;
		goto no_reply;
	}
	if (ipsaddr->cstate_incoming == CSTATE_INCOMING_NEED_RECONNECT) {
		// expecting reconnect.

		// confirm this REQ is consistent with previous REQs from peer
		// if not, assume this one is stale or stray
		if (ips_ipsaddr_match_req_params(OPCODE_CONNECT_REQUEST, "REQ", proto, ipsaddr, req, paylen)) {
			// CONNDBG already output w/details
			proto->epaddr_stats.reconnect_req_inconsistent++;
			goto no_reply;
		}
		if (req->reconnect_count == ipsaddr->reconnect_count) {
			// since in NEED_RECON we have already set ips params
			// and only need to set HAL params
			if (ipsaddr->cstate_outgoing != CSTATE_ESTABLISHED) {
				err = psmi_hal_ips_ipsaddr_set_req_params(proto, ipsaddr, req);
				if (err) {
					// _HFI_ERROR already output w/details
					_HFI_CONNDBG_RCV_REQ(" failed reconnect",
						ipsaddr, req, "");
					goto fatal_reconnect;
				}
			}
			proto->epaddr_stats.reconnect_req_recv++;
			_HFI_CONNDBG_RCV_REQ(" reconnect", ipsaddr, req, "");
		} else {
			err = psm3_ips_proto_start_reconnect(proto, ipsaddr, req, 0);
			if (err) {
				// _HFI_ERROR already output w/details
				_HFI_CONNDBG_RCV_REQ(" failed reconnect",
					ipsaddr, req, "");
				goto fatal_reconnect;
			}
			proto->epaddr_stats.reconnect_req_recv++;
			// CONNDBG already output
		}
		ipsaddr->cstate_incoming = CSTATE_ESTABLISHED;
		goto do_reply;
	}
	if (ipsaddr->cstate_incoming == CSTATE_ESTABLISHED) {
		if (ipsaddr->reconnect_count == req->reconnect_count) {
			// duplicate REQ

			// The statistics tabulation here is not 100% accurate
			// since if we had HAL errors before peer calls
			// psm3_ips_proto_connect the 1st REQ from peer could
			// have a reconnect_count of 1.  But that is rare
			// (we got an error before peer even connected so
			// peer never sent data to us). So we'll tabulate it
			// as the more likely case of a dup reconnect REQ, and
			// not attempt to identify the rare case where
			// reconnect_count==1 and it's a dup of the initial
			// connecton REQ from peer.

			// confirm REQ is consistent with previous REQs from
			// peer, if not, assume this one is stale or stray
			if (ips_ipsaddr_match_req_params(OPCODE_CONNECT_REQUEST, "REQ", proto, ipsaddr, req, paylen)) {
				// CONNDBG already output w/details
				if (req->reconnect_count)
					proto->epaddr_stats.reconnect_req_inconsistent++;
				else
					proto->epaddr_stats.connect_req_inconsistent++;
				goto no_reply;
			}
			_HFI_CONNDBG_RCV_REQ(" duplicate", ipsaddr, req, "");
			if (req->reconnect_count)
				proto->epaddr_stats.reconnect_req_dup++;
			else
				proto->epaddr_stats.connect_req_dup++;
			goto do_reply;
		}
		// processing for reconnect

		proto->epaddr_stats.reconnect_req_recv++;
		// confirm this REQ is consistent with previous REQs from peer
		// if not, assume this one is stale or stray
		if (ips_ipsaddr_match_req_params(OPCODE_CONNECT_REQUEST, "REQ", proto, ipsaddr, req, paylen)) {
			// CONNDBG already output w/details
			proto->epaddr_stats.reconnect_req_inconsistent++;
			goto no_reply;
		}
		err = psm3_ips_proto_start_reconnect(proto, ipsaddr, req, 0);
		if (err) {
			// _HFI_ERROR already output w/details
			_HFI_CONNDBG_RCV_REQ(" failed reconnect", ipsaddr,
					req, "");
			goto fatal_reconnect;
		}
		// CONNDBG already output
		goto do_reply;
	}

	// processing for new inbound connect
	psmi_assert_always(ipsaddr->cstate_incoming == CSTATE_NONE);
	if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
		if (req->reconnect_count) {
			_HFI_CONNDBG_RCV_REQ(" Ignored, stale reconnect",
				ipsaddr, req, "");
			proto->epaddr_stats.connect_req_stale++;
			goto no_reply;
		}
		// we could be in NONE for newconnect (ipsaddr just allocated)
		// or for a pre-allocated ipsaddr for a secondary rail
		// but we can also be left in NONE for a failed connect
		// so this assert is N/A unless we always free an ipsaddr
		// after failed connect and after disconnect/timewait as
		// opposed to just leaving it in NONE
		//psmi_assert_always(newconnect || epaddr->proto->ep !=
		//			epaddr->proto->ep->mctxt_master);
		newconnect = 1;
	}

	// caller's ips_proto_connect_hdr_parse already checked connect_verno
	// If validate fails, we will be unlikely to connect to remote
	// since their are basic options or PSM version inconsistencies.
	// By setting cerror_incoming any outgoing connect attempts will
	// also fail until we see a valid inbound REQ.
	ipsaddr->cerror_incoming = ips_validate_req_params(proto, epid, req);

	if (ipsaddr->cerror_incoming == PSM2_OK) {
		/* valid Incoming connection request */
		if (ipsaddr->cstate_outgoing != CSTATE_ESTABLISHED) {
			psmi_assert(ipsaddr->cstate_outgoing == CSTATE_NONE
				|| ipsaddr->cstate_outgoing==CSTATE_OUTGOING_WAITING);
			// we have a local HAL issue which is unlikely to
			// resolve itself.  While we could set cerror_incoming
			// and send a REP or discard the REQ, we will likely
			// remain stuck, su best to fatal error now
			err = ips_ipsaddr_set_req_params(proto, ipsaddr, req, paylen);
			if (err) {
				// _HFI_ERROR already output w/details
				_HFI_CONNDBG_RCV_REQ(" failed connect",
					ipsaddr, req, "");
				goto fatal;
			}
		}
	} else {
		// we will reply but not transition our incoming state
		// rep.connect_result != 0 will cause peer to fail connect
		_HFI_CONNDBG_RCV_REQ(" rejecting connect", ipsaddr, req,
			" result=%s (%d)",
			psm3_error_get_string(ipsaddr->cerror_incoming),
			ipsaddr->cerror_incoming);
		proto->epaddr_stats.connect_req_rej++;
		if (alloced)
			goto do_reply_and_free;
		goto do_reply;
	}

	if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
		ips_epstate_idx idx;
		psmi_assert_always(newconnect == 1);
		err = psm3_ips_epstate_add(proto->epstate, ipsaddr, &idx);
		if (err) {
			_HFI_ERROR("unable to add endpoint: %s (%d)\n",
					psm3_error_get_string(err), err);
			goto fail;
		}
		ipsaddr->connidx_incoming = idx;
	}
	_HFI_CONNDBG_RCV_REQ("", ipsaddr, req, " idx in=%u out=%u flow=%p",
		ipsaddr->connidx_incoming, ipsaddr->connidx_outgoing,
		&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO]);

	ipsaddr->cstate_incoming = CSTATE_ESTABLISHED;

	ipsaddr->runid_key = req->runid_key;

	proto->num_connected_incoming++;
	proto->epaddr_stats.max_connected_incoming =
		max(proto->epaddr_stats.max_connected_incoming,
			proto->num_connected_incoming);

do_reply:
	psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
	ips_proto_send_conn_reply(proto, &ipsaddr->flows[proto->msgflowid],
		&ipsaddr->ctrl_msg_queued);
no_reply:
fail:	// by returning an err (typically PSM2_NO_MEMORY), caller will
	// report a fatal error processing a connect or disconnect packet
	return err;

do_reply_and_free:
	psmi_assert(alloced);
	psmi_assert(err == PSM2_OK);
	psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
	ips_proto_send_conn_reply(proto, &ipsaddr->flows[proto->msgflowid],
		&ipsaddr->ctrl_msg_queued);
	ips_free_epaddr((psm2_epaddr_t)ipsaddr, proto);
	return err;

fatal_reconnect:
	_HFI_CONN_FATAL(PSM2_INTERNAL_ERR, "failed inbound reconnect",
		ipsaddr, "");
	return PSM2_INTERNAL_ERR;

fatal:
	_HFI_CONN_FATAL(PSM2_INTERNAL_ERR, "failed inbound reconnect",
		ipsaddr, "");
	return PSM2_INTERNAL_ERR;
}

// establish connections for specified array of epid's
// array_of_errors will indicate the error for each epid requested
// by array_of_epid_mask[] != 0 and array_of_epid[]
// return is worse of errors in array_of_errors[]
//	PSM2_OK - all connections established, some in array_of_errors may be
//		PSM2_EPID_ALREADY_CONNECTED or PSM2_EPID_UNREACHABLE
//		array_of_epaddr[] has ipsaddr
//	PSM2_TIMEOUT - some unable to be established due to time limit
//		array_of_epaddr[] NULL
//	PSM2_INVALID_CONNECT - some attempt to connect prior to inbound
//		disconnect or timewait complete
//		array_of_epaddr[] has ipsaddr
//	other errors - some unable to be established for other reasons
//		array_of_epaddr[] NULL
// If a mixture of timeouts and invalid connects occur, PSM2_INVALID_CONNECT
// is returned.
psm2_error_t
psm3_ips_proto_connect(struct ips_proto *proto, int numep,
	const psm2_epid_t *array_of_epid,
	const int *array_of_epid_mask, psm2_error_t *array_of_errors,
	psm2_epaddr_t *array_of_epaddr, uint64_t timeout_in)
{
	int i, n, n_first;
	psm2_error_t err = PSM2_OK;
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	ips_epstate_idx idx;
	int numep_toconnect = 0, numep_left;
	union psmi_envvar_val credits_intval;
	int connect_credits;
	uint64_t reqs_sent = 0;

	psm3_getenv("PSM3_CONNECT_CREDITS",
		"End-point connect request credits. (<=0 uses default), default is " STRINGIFY(PSM_CONN_CREDITS),
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val)PSM_CONN_CREDITS, &credits_intval);
	if (credits_intval.e_int > 0) {
		connect_credits = credits_intval.e_int;
	} else {
		connect_credits = PSM_CONN_CREDITS;
	}

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	/* All timeout values are in cycles */
	uint64_t t_start = get_cycles();
	/* Print a timeout at the warning interval */
	union psmi_envvar_val warn_intval;
	uint64_t to_warning_interval;
	uint64_t to_warning_next;

	/* Setup warning interval */
	psm3_getenv("PSM3_CONNECT_WARN_INTERVAL",
		"Period in seconds to warn if connections are not completed."
		"Default is 300 seconds, 0 to disable",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)300, &warn_intval);

	to_warning_interval = nanosecs_to_cycles(warn_intval.e_uint * NSEC_PER_SEC);
	to_warning_next = t_start + to_warning_interval;

	/* Some sanity checks */
	psmi_assert_always(array_of_epid_mask != NULL);

	/* First pass: initialize array_of_errors and array_of_epaddr */
	for (i = 0; i < numep; i++) {
		_HFI_CONNDBG("epid-connect=%s connect to epid %s: %s\n",
				array_of_epid_mask[i] ? "YES" : " NO",
				psm3_epid_fmt_internal(array_of_epid[i], 0),
				psm3_epid_fmt_addr(array_of_epid[i], 1));
		if (array_of_epid_mask[i]) {
			array_of_errors[i] = PSM2_EPID_UNKNOWN;
			array_of_epaddr[i] = NULL;
		}
	}

	/* Second pass: see what to connect and what is connectable. */
	for (i = 0, numep_toconnect = 0; i < numep; i++) {
		if (!array_of_epid_mask[i])
			continue;

		/* Can't send to epid on same NIC (eg. nid) if not loopback */
		/* never attempt to connect to self even if loopback */
		if (((0 == psm3_nid_cmp_internal(psm3_epid_nid(proto->ep->epid),
					psm3_epid_nid(array_of_epid[i])))
				&& !(proto->flags & IPS_PROTO_FLAG_LOOPBACK))
		    || !psm3_epid_cmp_internal(proto->ep->epid, array_of_epid[i])) {
			array_of_errors[i] = PSM2_EPID_UNREACHABLE;
			continue;
		}

		if (! psm3_subnets_match_epid(proto->ep->subnet, array_of_epid[i])) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				" Trying to connect from %s port %u (subnet %s) to a node (%s) on a"
				" different subnet %s\n",
				proto->ep->dev_name, proto->ep->portnum,
				psm3_subnet_epid_subset_fmt(proto->ep->subnet, 0),
				psm3_epid_fmt_addr(array_of_epid[i], 1),
				psm3_epid_fmt_subnet(array_of_epid[i], 2));
		}

		epaddr = psm3_epid_lookup(proto->ep, array_of_epid[i]);
		if (epaddr == NULL) {
			/* We're sending a connect request message before some other node
			 * has sent its connect message */
			// so we lack it's hostname, rv and qpn info
			epaddr = ips_alloc_epaddr(proto, 1, array_of_epid[i], NULL, (timeout_in / 1000000UL), &err);
			if (epaddr == NULL) {
				_HFI_ERROR("Unable to issue connect from %s to %s: unable to allocate endpoint: %s (%d)\n",
					proto->ep->dev_name, psm3_epaddr_get_name(array_of_epid[i], 0),
					psm3_error_get_string(err), err);
				goto fail;
			}
			ipsaddr = (ips_epaddr_t *) epaddr;
			err = psm3_ips_epstate_add(proto->epstate, ipsaddr, &idx);
			if (err) {
				_HFI_ERROR("Unable to issue connect from %s to %s: unable to add endpoint: %s (%d)\n",
					proto->ep->dev_name, psm3_epaddr_get_name(array_of_epid[i], 0),
					psm3_error_get_string(err), err);
				goto fail;
			}
			ipsaddr->connidx_incoming = idx;
		} else if ((((ips_epaddr_t *) epaddr)->cstate_outgoing == CSTATE_NONE)
			&& (((ips_epaddr_t *) epaddr)->cstate_incoming == CSTATE_DISCONNECTED)) /* waiting for timewait */
		{
			// we don't allow a new connect to start until
			// timewait for the inbound connection completes.
			array_of_errors[i] = PSM2_EPID_INVALID_CONNECT;
			array_of_epaddr[i] = epaddr;
			continue;
		} else if (((ips_epaddr_t *) epaddr)->cstate_outgoing == CSTATE_DISCONNECTED) { /* waiting for timewait */
			// we don't allow a new connect to start until
			// timewait for the outbound connection completes.
			// May also be waiting for inbound disconnect before
			// timewait timer actually starts
			array_of_errors[i] = PSM2_EPID_INVALID_CONNECT;
			array_of_epaddr[i] = epaddr;
			continue;
		} else if (((ips_epaddr_t *) epaddr)->cstate_outgoing != CSTATE_NONE) {	/* already connected */
			psmi_assert_always(((ips_epaddr_t *)epaddr)->cstate_outgoing == CSTATE_ESTABLISHED
				|| ((ips_epaddr_t *) epaddr)->reconnecting);
			array_of_errors[i] = PSM2_EPID_ALREADY_CONNECTED;
			array_of_epaddr[i] = epaddr;
			continue;
		} else if (((ips_epaddr_t *) epaddr)->cstate_incoming == CSTATE_NONE) {
			// we could be in NONE for a pre-allocated ipsaddr
			// for a secondary rail but we can also be left in
			// NONE for a failed connect
			// so this assert is N/A unless we always free an
			// ipsaddr after failed connect and after
			// disconnect/timewait as opposed to just leaving it
			// in NONE
			//psmi_assert_always(epaddr->proto->ep !=
			//		epaddr->proto->ep->mctxt_master);
			ipsaddr = (ips_epaddr_t *) epaddr;
			err = psm3_ips_epstate_add(proto->epstate, ipsaddr, &idx);
			if (err) {
				_HFI_ERROR("Unable to issue connect from %s to %s: unable to add endpoint: %s (%d)\n",
					proto->ep->dev_name, psm3_epaddr_get_name(array_of_epid[i], 0),
					psm3_error_get_string(err), err);
				goto fail;
			}
			ipsaddr->connidx_incoming = idx;
		} else {
			/* We've already received a connect request message from a remote
			 * peer, it's time to send our own. */
			ipsaddr = (ips_epaddr_t *) epaddr;
			/* No re-entrancy sanity check and makes sure we are not connected
			 * twice (caller's precondition) */
			psmi_assert(ipsaddr->cstate_outgoing == CSTATE_NONE);
			psmi_assert(ipsaddr->cstate_incoming != CSTATE_NONE);
		}

		ipsaddr->cstate_outgoing = CSTATE_OUTGOING_WAITING;
		ipsaddr->cerror_outgoing = PSM2_OK;
		array_of_epaddr[i] = epaddr;
		ipsaddr->s_timeout = get_cycles();
		ipsaddr->delay_in_ms = 1;
		ipsaddr->credit = 0;
		ipsaddr->waiting_in_connect = 1;
		numep_toconnect++;
	}

	/* Third pass: do the actual connect. array_of_errors[] values:
	 * PSM2_EPID_UNKNOWN: Not connected yet.
	 * PSM2_EPID_UNREACHABLE: Not to be connected.
	 * PSM2_EPID_INVALID_CONNECT: in timewait, can't connect now
	 * PSM2_EPID_ALREADY_CONNECTED: was connected prior to this func call
	 * PSM2_OK: Successfully connected (in this func call)
	 *	set as soon as observe ESTABLISHED && fully_connected
	 *	this func continues polling other ipsaddr, but not this one.
	 *	Once we get here, waiting_in_connect is cleared and we stop
	 *	checking outgoing state.  It may validly start reconnects.
	 *	If goes to ESTABLISHED and then starts a reconnect (WAITING)
	 *	quickly, we may not notice it and we will continuing polling
	 *	under the umbrella of the initial connect and retry REQ 
	 *	as needed, which is fine.
	 * Start sending connect messages at a random index between 0 and numep-1
	 */
	numep_left = numep_toconnect;
	n_first = ((uint32_t) get_cycles()) % numep;	// random index
	while (numep_left > 0) {
		for (n = 0; n < numep; n++) {
			int keep_polling = 1;
			i = (n_first + n) % numep;
			if (!array_of_epid_mask[i])
				continue;
			switch (array_of_errors[i]) {
			case PSM2_EPID_UNREACHABLE:
			case PSM2_EPID_ALREADY_CONNECTED:
			case PSM2_EPID_INVALID_CONNECT:
			case PSM2_OK:
				continue;
			default:
				// PSM3_EPID_UNKNOWN, need to work on connect
				break;
			}
			psmi_assert_always(array_of_epaddr[i] != NULL);
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
				if (ipsaddr->credit) {
					connect_credits++;
					ipsaddr->credit = 0;
				}
				switch (psmi_hal_ips_fully_connected(ipsaddr)) {
				case 1:
					/* This is not the real error code, we only set OK here
					 * so we know to stop polling for the reply. The actual
					 * error is in ipsaddr->cerror_outgoing */
					array_of_errors[i] = PSM2_OK;
					ipsaddr->s_timeout = 0;
					ipsaddr->waiting_in_connect = 0;
					numep_left--;
					continue;
					break;
				case 0:
					// fall through to "keep_polling" loop below to check timers
					break;
				default:
					/* This is not the real error code, we only set OK here
					 * so we know to stop polling for the reply. The actual
					 * error is in ipsaddr->cerror_outgoing */
					array_of_errors[i] = PSM2_OK;
					ipsaddr->s_timeout = 0;
					ipsaddr->waiting_in_connect = 0;
					numep_left--;
					if (ipsaddr->cerror_outgoing == PSM2_OK)
						ipsaddr->cerror_outgoing = PSM2_EPID_RC_CONNECT_ERROR;
					// EIO is connect error
					if (errno != EIO) {
						// _HFI_ERROR already output
						err = PSM2_INTERNAL_ERR;
						goto fail;	// serious error
					}
					continue;
					break;
				}
			}
			while (keep_polling) {
				if (!psm3_cycles_left(t_start, timeout_in)) {
					err = PSM2_TIMEOUT;
					goto err_timeout;
				}
				if (to_warning_interval
					&& get_cycles() >= to_warning_next) {
#if _HFI_DEBUGGING
					uint64_t waiting_time = 0;
					if (_HFI_INFO_ON) {
						waiting_time = cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_SEC;
					}
#endif
					const char *first_name = NULL;
					int num_waiting = 0;

					for (i = 0; i < numep; i++) {
						if (!array_of_epid_mask[i] || array_of_errors[i] != PSM2_EPID_UNKNOWN)
							continue;
						if (!first_name)
							first_name = psm3_epaddr_get_name(array_of_epid[i], 0);
						num_waiting++;
					}
					if (_HFI_INFO_ON && first_name) {
						_HFI_INFO_ALWAYS(
							"Couldn't connect to %s (and %d others). "
							"Time elapsed %02i:%02i:%02i. Still trying...\n",
   							first_name, num_waiting,
   							(int)(waiting_time / 3600),
   							(int)((waiting_time / 60) - ((waiting_time / 3600) * 60)),
							(int)(waiting_time - ((waiting_time / 60) * 60)));
					}
					to_warning_next = get_cycles() + to_warning_interval;
				}
				if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
					// just waiting for rv to be connected
					if ((err = psmi_err_only(psm3_poll_internal(proto->ep, 1, 1)))) {
						_HFI_ERROR("unable to poll: %s (%d)\n",
							psm3_error_get_string(err), err);
						goto fail;
					}
					break;	// let outer loop start another REQ
				}

				if (get_cycles() > ipsaddr->s_timeout) {
					if (!ipsaddr->credit && connect_credits) {
						ipsaddr->credit = 1;
						connect_credits--;
					}
					if (ipsaddr->credit) {
						psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
						if (PSM2_OK == ips_proto_send_conn_request(proto,
							&ipsaddr->flows[proto->msgflowid],
   							&ipsaddr->ctrl_msg_queued, 0))
						{
							reqs_sent++;
							if (ipsaddr->reconnecting) {
								// we got connection_error while still
								// waiting_in_connect. So we are actually issuing
								// the reconnect REQ now
								if (ipsaddr->delay_in_ms > 1)
									proto->epaddr_stats.reconnect_req_retry++;
								else
									proto->epaddr_stats.reconnect_req_send++;
							} else if (ipsaddr->delay_in_ms > 1)
								proto->epaddr_stats.connect_req_retry++;
							keep_polling = 0;
							ipsaddr->delay_in_ms = min(100, ipsaddr->delay_in_ms << 1);
							ipsaddr->s_timeout = get_cycles()
							       + nanosecs_to_cycles(ipsaddr->delay_in_ms * NSEC_PER_MSEC);
						}
						/* If not, send got "busy", keep trying */
					} else {
						keep_polling = 0;
					}
				}

				if ((err = psmi_err_only(psm3_poll_internal(proto->ep, 1, 1)))) {
					_HFI_ERROR("unable to poll: %s (%d)\n",
						psm3_error_get_string(err), err);
					goto fail;
				}

				if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
					connect_credits++;
					ipsaddr->credit = 0;
					switch (psmi_hal_ips_fully_connected(ipsaddr)) {
					case 1:
						/* This is not the real error code, we only set OK here
						 * so we know to stop polling for the reply. The actual
						 * error is in ipsaddr->cerror_outgoing */
						array_of_errors[i] = PSM2_OK;
						ipsaddr->s_timeout = 0;
						ipsaddr->waiting_in_connect = 0;
						numep_left--;
						break;
					case 0:
						break;
					default:
						/* This is not the real error code, we only set OK here
						 * so we know to stop polling for the reply. The actual
						 * error is in ipsaddr->cerror_outgoing */
						array_of_errors[i] = PSM2_OK;
						ipsaddr->s_timeout = 0;
						ipsaddr->waiting_in_connect = 0;
						numep_left--;
						if (ipsaddr->cerror_outgoing == PSM2_OK)
							ipsaddr->cerror_outgoing = PSM2_EPID_RC_CONNECT_ERROR;
						// EIO is connect error
						if (errno != EIO) {
							// _HFI_ERROR already output
							err = PSM2_INTERNAL_ERR;
							goto fail;	// serious error
						}
						break;
					}
					// even if ! rv_connected, let outer loop start next REQ
					break;
				}
			}
		}
	}

/* We treat internal errors (goto fail) as fatal since the internal
 * state of PSM is bad. If we want to report error to caller instead,
 * uncomment this label on the next line and then comment out the
 * fatal "fail:" label at the end of this function.
 */
//fail:	// unexpected serious error allocating or polling
err_timeout:
	/* Final pass: review array_of_errors to determine return value
	 * for overall function based on worse error of any attempted
	 * connections. For connections not completed, cleanup state
	 */
	numep_left = numep_toconnect;
	for (i = 0; i < numep; i++) {
		if (!array_of_epid_mask[i])
			continue;
		switch (array_of_errors[i]) {
		case PSM2_EPID_UNREACHABLE:	// benign
			psmi_assert(array_of_epaddr[i] == NULL);
			numep_left--;
			break;
		case PSM2_EPID_ALREADY_CONNECTED:	// benign
			psmi_assert(! ((ips_epaddr_t *) array_of_epaddr[i])->waiting_in_connect);
			numep_left--;
			break;
		case PSM2_EPID_UNKNOWN:	// timeout or other error before conn
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			array_of_errors[i] = PSM2_TIMEOUT;
			ipsaddr->waiting_in_connect = 0;
			err = psm3_error_cmp(err, PSM2_TIMEOUT);
			_HFI_CONNDBG_BASIC("Err connect timeout", ipsaddr, "");
conn_cleanup:
			if (ipsaddr->cstate_outgoing != CSTATE_NONE) {
				// we need to cleanup this ipsaddr's state
				// since it may have a connect started (or
				// even be established or reconnecting if
				// timed out just before it finished and then
				// finished too late).
				// so this is similar to a forced disconnect
				psm3_ips_proto_ipsaddr_start_disconnect(proto, ipsaddr, 1, 1);
				// since we failed to connect, unlikely DREQ
				// would get through, so don't try, also
				// avoids issues if big list of connect errors
				psm3_ips_proto_ipsaddr_done_disconnect(proto, ipsaddr, 1, 1);
				// alternative is to leave in DISCONNECTED and
				// prevent outbound connect for this ipsaddr
				ipsaddr->cstate_outgoing = CSTATE_NONE;
				_HFI_CONNDBG_BASIC("forced disconnect", ipsaddr, "");
				ipsaddr->reconnect_count = 0;
			}
			// app can attempt connect again in future
			array_of_epaddr[i] = NULL;
			if (ipsaddr->cstate_incoming == CSTATE_NONE) {
#ifdef PSM_DEBUG
				ipsaddr->set_req_params = 0;
#endif
				// we never told caller we connected so
				// can't be any outstanding app sends
				// but we may have some unflushed recvs?
				// be safe, don't free, leave in NONE/NONE
				//ips_free_epaddr(array_of_epaddr[i], proto);
			}
			break;
		case PSM2_EPID_INVALID_CONNECT:
			psmi_assert(array_of_epaddr[i] != NULL);
			err = psm3_error_cmp(err, PSM2_EPID_INVALID_CONNECT);
			break;
		case PSM2_OK:	// connected or cerror_* has error code
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			psmi_assert(! ipsaddr->waiting_in_connect);
			/* Restore the real connect error */
			array_of_errors[i] = psm3_error_cmp(ipsaddr->cerror_outgoing,
				ipsaddr->cerror_incoming);
			// normally would be in outgoing ESTABLISHED here, but
			// while looping in this routine for other peers, we
			// could have discovered an issue & started reconnect
			psmi_assert_always(ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED
				|| (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING
					&& ipsaddr->reconnecting));
			if (array_of_errors[i] != PSM2_OK) {
				// connect issue
				err = psm3_error_cmp(err, array_of_errors[i]);
				_HFI_CONNDBG_BASIC("Err failed to connect",
					ipsaddr, " err=%s (%d)", psm3_error_get_string(err), err);
				goto conn_cleanup;
			} else {
				_HFI_CONNDBG_BASIC("outgoing connected", ipsaddr, "");
				proto->num_connected_outgoing++;
				proto->epaddr_stats.max_connected_outgoing =
					max(proto->epaddr_stats.max_connected_outgoing,
						proto->num_connected_outgoing);
				psmi_assert_always(ipsaddr->pathgrp->pg_path[0][IPS_PATH_HIGH_PRIORITY]->pr_mtu > 0);
				numep_left--;
			}
			break;
		default:
			// should not happen
			_HFI_CONNDBG_BASIC_EPID("Err failed to connect",
				proto, array_of_epid[i], " unexpected err=%s (%d)",
				psm3_error_get_string(array_of_errors[i]),
				array_of_errors[i]);
			break;
		}
	}
	if (err)
		_HFI_CONNDBG_OR_PRDBG(
			"connect incomplete for %d/%d peers %s (%d) "
			"(elapsed=%d millisecs,timeout=%d millisecs,reqs=%lld)\n",
			numep_left, numep_toconnect,
			psm3_error_get_string(err), err,
			(int)(cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_MSEC),
			(int)(timeout_in / NSEC_PER_MSEC),
			(unsigned long long)reqs_sent);
	else
		_HFI_CONNDBG_OR_PRDBG(
			"connect complete for %d peers in %d millisecs (timeout=%d), reqs_sent=%lld\n",
			numep_toconnect,
			(int)(cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_MSEC),
			(int)(timeout_in / NSEC_PER_MSEC),
			(unsigned long long)reqs_sent);
	return err;

fail:	// unexpected serious error allocating or polling
	// _HFI_ERROR already output with more info
	// treat serious errors as fatal, they leave ipsaddr in
	// potentially odd states which will confuse future API calls
	psm3_handle_error(PSMI_EP_NORETURN, err, "Fatal error during connect.");
	return err;
}

static psm2_error_t
psm3_ips_proto_reconnect_timer_callback(struct psmi_timer *timer,
	uint64_t current)
{
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *)(timer->context);
	psmi_assert(ipsaddr);
	struct ips_proto *proto = ((psm2_epaddr_t) ipsaddr)->proto;

	_HFI_CONNDBG_BASIC("timer", ipsaddr, "");

	psmi_assert(ipsaddr->allow_reconnect);
	psmi_assert(! ipsaddr->waiting_in_connect);
	if (ipsaddr->waiting_in_disconnect) {
		// ignore, started to disconnect, no need to finish reconnect
		// and psm3_ips_proto_disconnect is not yet ready for timewait
	} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
		psmi_assert(ipsaddr->reconnect_count);
		psmi_assert(ipsaddr->reconnecting);
		// upper bound on total reconnect time
		// risk if app has long delays between polling
		if (get_cycles() > ipsaddr->s_timeout) {
			// fatal error
			_HFI_CONN_FATAL(PSM2_INTERNAL_ERR,
				"Couldn't reconnect, timeout", ipsaddr, "");
		}

		psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
		if ( ips_proto_send_conn_request(proto,
				&ipsaddr->flows[proto->msgflowid],
				&ipsaddr->ctrl_msg_queued, 0) == PSM2_OK) {
			if (ipsaddr->delay_in_ms > 1)
				proto->epaddr_stats.reconnect_req_retry++;
			else
				proto->epaddr_stats.reconnect_req_send++;
			ipsaddr->delay_in_ms = min(100, ipsaddr->delay_in_ms << 1);
			psmi_timer_request(proto->timerq, &ipsaddr->timer_reconnect,
				get_cycles() + nanosecs_to_cycles(ipsaddr->delay_in_ms * NSEC_PER_MSEC));
		} else {
			/* send got "busy", start short timer to reattempt */
			psmi_timer_request(proto->timerq, &ipsaddr->timer_reconnect,
				get_cycles() + nanosecs_to_cycles(IPS_CONNECT_BUSY_TIMER));
		}
	} else if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
		// ignore, should not happen
	} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC) {
		// ignore, should not happen
	} else if (ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED
		|| ipsaddr->cstate_outgoing == CSTATE_NONE) {
		// timewait expired
		_HFI_CONNDBG("timewait expired ipsaddr %p\n", ipsaddr);
		psmi_assert(ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED
			|| ipsaddr->cstate_incoming == CSTATE_DISCONNECTED);
		psmi_assert(ipsaddr->cstate_incoming == CSTATE_NONE
			|| ipsaddr->cstate_incoming == CSTATE_DISCONNECTED);
		// we assume timewait delay has allowed enough time to QP to drain
		ips_free_epaddr((psm2_epaddr_t)ipsaddr, proto);
	}
	return PSM2_OK;
}

/* Repercussions on MQ.
 *
 * If num_connected==0, everything that exists in the posted queue should
 * complete and the error must be marked epid_was_closed.
 *
 */

psm2_error_t
psm3_ips_proto_disconnect(struct ips_proto *proto, int force, int numep,
	psm2_epaddr_t array_of_epaddr[],
	const int array_of_epaddr_mask[],
	psm2_error_t array_of_errors[], uint64_t timeout_in)
{
	ips_epaddr_t *ipsaddr;
	int numep_left, numep_todisc, i, n;
	int n_first;
	int has_pending;
	uint64_t timeout;
	psm2_error_t err = PSM2_OK;
	uint64_t reqs_sent = 0;
	union psmi_envvar_val credits_intval;
	int disconnect_credits;
	uint64_t t_warning, t_start;
	union psmi_envvar_val warn_intval;
	unsigned warning_secs;

	/* In case of a forced close, we cancel whatever timers are pending
	 * on the proto so that we don't have zombie timers coming back
	 * after the internal structures of PSM2 have been destroyed
	 */
	if (force) {
		struct psmi_timer *t_cursor;
		TAILQ_FOREACH(t_cursor, &proto->timerq->timerq, timer) {
			psmi_timer_cancel(proto->timerq, t_cursor);
		}
	}

	psmi_assert_always(numep > 0);

	psm3_getenv("PSM3_DISCONNECT_CREDITS",
		"End-point disconnect request credits.",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)100, &credits_intval);

	disconnect_credits = credits_intval.e_uint;

	/* Setup warning interval */
	psm3_getenv("PSM3_DISCONNECT_WARN_INTERVAL",
		"Period in seconds to warn if disconnections are not completed."
		"Default is 300 seconds, 0 to disable.",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		(union psmi_envvar_val)300, &warn_intval);

	warning_secs = warn_intval.e_uint;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	/* First pass: see what to disconnect and what is disconnectable */
	/* mark array_of_errors[] as:
	 * PSM2_OK - nothing to do
	 * PSM2_EPID_UNKNOWN - need to perform disconnect protocol
	 */
	for (i = 0, numep_todisc = 0; i < numep; i++) {
		if (!array_of_epaddr_mask[i])
			continue;
		psmi_assert_always(array_of_epaddr[i]->ptlctl->ptl ==
				proto->ptl);
		ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
		ipsaddr->credit = 0;
		if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
			// benign, nothing to do
			array_of_errors[i] = PSM2_OK;
			continue;
		} else if (ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED) {
			// DISCONNECTED, PSM2_OK later in this func reflects
			// we did the work, so can't let app call disconnect
			// twice for same ipsaddr or will be confused later
			// in this routine
			_HFI_CONN_FATAL(PSM2_INTERNAL_ERR,
				"Disconnect not allowed when already disconnected",
				ipsaddr, "");
		} else {
			psmi_assert_always(ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED
				|| ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING);
			// this flag blocks any reconnect while we
			// run through the disconnect steps
			// and prevents inbound DREQ from freeing ipsaddr
			ipsaddr->waiting_in_disconnect = 1;
			if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING)
				psmi_timer_cancel(proto->timerq, &ipsaddr->timer_reconnect);
		}
		_HFI_CONNDBG_BASIC("disconnecting", ipsaddr, " force=%d %s",
			force, psm3_epaddr_get_hostname(ipsaddr->epaddr.epid, 1));
		// mark array entries we need to disconnect
		array_of_errors[i] = PSM2_EPID_UNKNOWN;
		numep_todisc++;
	}
	if (numep_todisc == 0)
		goto success;

	/* Wait for everyone to ack previous packets before putting */
	if (timeout_in == 0)
		timeout = ~0ULL;
	else
		timeout = get_cycles() + nanosecs_to_cycles(timeout_in);

	t_start = get_cycles();
	t_warning = t_start + nanosecs_to_cycles(warning_secs * NSEC_PER_SEC);

	n_first = ((uint32_t) get_cycles()) % numep;
	if (!force) {
		numep_left = numep_todisc;
		do {
			for (n = 0; n < numep; n++) {
				i = (n_first + n) % numep;
				if (!array_of_epaddr_mask[i] || array_of_errors[i] == PSM2_OK)
					continue;
				ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
				switch (ipsaddr->cstate_outgoing) {
				case CSTATE_DISCONNECTED:
					// we got our DREP in poll_internal
					psmi_assert(ipsaddr->waiting_in_disconnect);
					psm3_ips_proto_ipsaddr_done_disconnect(proto, ipsaddr, force, 1);
					array_of_errors[i] = PSM2_OK;
					numep_left--;
					disconnect_credits++;
					ipsaddr->credit = 0;
					_HFI_CONNDBG_BASIC("outgoing disconnected", ipsaddr, "");
					continue;
				case CSTATE_OUTGOING_WAITING_DISC:
					psmi_assert(ipsaddr->waiting_in_disconnect);
					if (ipsaddr->s_timeout > get_cycles())
						continue;
					// timeout for DREP, resend DREQ
					ipsaddr->delay_in_ms = min(100, ipsaddr->delay_in_ms << 1);
					ipsaddr->s_timeout = get_cycles()
					       + nanosecs_to_cycles(ipsaddr->delay_in_ms * NSEC_PER_MSEC);
					psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
					if (PSM2_OK == ips_proto_send_disc_request(proto,
		   					&ipsaddr->flows[proto->msgflowid],
	   						(uint8_t)force, &ipsaddr->ctrl_msg_queued,
	   						timeout))
				       	{
						proto->epaddr_stats.disconnect_req_retry++;
						reqs_sent++;
					}
					// if failed to send (queue full)
					// since we already tried for timeout
					// will fail timeout test below & ret up
					break;
				case CSTATE_OUTGOING_WAITING:
					// disconnect while reconnecting
					// we remain in this state while
					// waiting for pending acks below
					psmi_assert(ipsaddr->allow_reconnect);
					psmi_assert(ipsaddr->reconnecting);
					psmi_assert(! ipsaddr->waiting_in_connect);
					psmi_assert(ipsaddr->waiting_in_disconnect);
					/* FALLSTHROUGH */
				case CSTATE_ESTABLISHED:
					psmi_assert(ipsaddr->waiting_in_disconnect);
					/* Still pending acks, hold off for now */
					has_pending = !STAILQ_EMPTY(&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO].scb_unacked);
					if (has_pending)
						continue;
					if (!ipsaddr->credit && disconnect_credits) {
						ipsaddr->credit = 1;
						disconnect_credits--;
					}
					if (!ipsaddr->credit)
						continue;
					// issue 1st DREQ for this ipsaddr
					ipsaddr->delay_in_ms = 1;
					psm3_ips_proto_ipsaddr_start_disconnect(proto, ipsaddr, force, 1);
					ipsaddr->cstate_outgoing = CSTATE_OUTGOING_WAITING_DISC;
					ipsaddr->reconnecting = 0;
					ipsaddr->s_timeout = get_cycles() + nanosecs_to_cycles(NSEC_PER_MSEC);
					psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
					if (PSM2_OK == ips_proto_send_disc_request(proto,
		   					&ipsaddr->flows[proto->msgflowid],
	   						(uint8_t)force, &ipsaddr->ctrl_msg_queued,
	   						timeout))
				       	{
						reqs_sent++;
					}
					// if failed to send (queue full)
					// since we already tried for timeout
					// will fail timeout test below & ret up
					break;
				default:
					_HFI_CONN_FATAL(PSM2_INTERNAL_ERR, "Unhandled/unknown close state",
						ipsaddr, " state=%s", cstate_to_str(ipsaddr->cstate_outgoing));
					break;
				}
			}
			if (numep_left == 0)
				break;

			if ((err = psmi_err_only(psm3_poll_internal(proto->ep, 1, 1))))
				goto fail;

			if (warning_secs && get_cycles() > t_warning) {
				_HFI_INFO("graceful close in progress for %d/%d peers "
				     "(elapsed=%d millisecs,timeout=%d millisecs,reqs=%lld)\n",
				     numep_left, numep_todisc,
				     (int)(cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_MSEC),
				     (int)(timeout_in / NSEC_PER_MSEC),
				     (unsigned long long)reqs_sent);
				t_warning = get_cycles() + nanosecs_to_cycles(warning_secs * NSEC_PER_SEC);
			}
		} while (timeout > get_cycles());

		if (numep_left > 0) {
			err = PSM2_TIMEOUT;
			for (i = 0; i < numep; i++) {
				if (!array_of_epaddr_mask[i])
					continue;
				if (array_of_errors[i] == PSM2_EPID_UNKNOWN) {
					array_of_errors[i] = PSM2_TIMEOUT;
					_HFI_CONNDBG_BASIC("disc timeout", (ips_epaddr_t *)array_of_epaddr[i], " index %d", i);
					// code below will do a forced
					// disconnect without sending a DREQ
				}
			}
			_HFI_CONNDBG_OR_PRDBG(
				"graceful close incomplete for %d/%d peers %s (%d) "
				"(elapsed=%d millisecs,timeout=%d millisecs,reqs=%lld)\n",
				numep_left, numep_todisc,
				psm3_error_get_string(err), err,
				(int)(cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_MSEC),
				(int)(timeout_in / NSEC_PER_MSEC),
				(unsigned long long)reqs_sent);
		} else {
			_HFI_CONNDBG_OR_PRDBG(
				"graceful close complete for %d peers in %d millisecs (timeout=%d), reqs_sent=%lld\n",
				numep_todisc,
				(int)(cycles_to_nanosecs(get_cycles() - t_start) / NSEC_PER_MSEC),
				(int)(timeout_in / NSEC_PER_MSEC),
				(unsigned long long)reqs_sent);
		}
	} else {
		psmi_assert_always(proto->msgflowid < EP_NUM_FLOW_ENTRIES);
		for (n = 0; n < numep; n++) {
			i = (n_first + n) % numep;
			if (!array_of_epaddr_mask[i] || array_of_errors[i] == PSM2_OK)
				continue;
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
				psmi_assert(ipsaddr->allow_reconnect);
				psmi_assert(ipsaddr->reconnecting);
			} else {
				psmi_assert_always(ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED);
				psmi_assert(! ipsaddr->reconnecting);
			}
			// do our best to send DREQ, but if queue full
			// just skip it.  No wait for DREP.  No retries.
			psm3_ips_proto_ipsaddr_start_disconnect(proto, ipsaddr, force, 1);
			ips_proto_send_disc_request(proto,
				&ipsaddr->flows[proto->msgflowid],
				(uint8_t)force, &ipsaddr->ctrl_msg_queued, 0);
			/* Force state to DISCONNECTED */
			ipsaddr->cstate_outgoing = CSTATE_DISCONNECTED;
			ipsaddr->reconnecting = 0;
			psm3_ips_proto_ipsaddr_done_disconnect(proto, ipsaddr, force, 1);
			array_of_errors[i] = PSM2_OK;
			_HFI_CONNDBG_BASIC("outgoing disconnected", ipsaddr, "");
		}
		_HFI_CONNDBG_OR_PRDBG("non-graceful close complete for %d peers\n", numep);
	}

	// if array_of_errors is now PSM2_OK and we are not outgoing NONE
	// then we did work here to disconnect and can complete cleanup
	// of ipsaddr.  If we had a timeout, err is already PSM2_TIMEOUT
	// and so are the problematic array_of_errors[] entries
	for (i = 0; i < numep; i++) {
		if (!array_of_epaddr_mask[i])
			continue;
		ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
		if (ipsaddr->cstate_outgoing == CSTATE_NONE)
			continue;
		psmi_assert(ipsaddr->waiting_in_disconnect);
		if (array_of_errors[i] != PSM2_OK) {
			psmi_assert(array_of_errors[i] == PSM2_TIMEOUT);
			// force to DISCONNECTED
			// depending on how far we got, ipsaddr could still
			// be ESTABLISHED, WAITING (reconnecting),
			// or WAITING_DISC. Can even be DISCONNECTED
			// if we timed out before upper loop observed it
			// reached DISCONNECTED
			if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
				psmi_assert(ipsaddr->allow_reconnect);
				psmi_assert(ipsaddr->reconnecting);
			} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC) {
				psmi_assert(! ipsaddr->reconnecting);
			} else if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
				psmi_assert(! ipsaddr->reconnecting);
			}
			if (ipsaddr->cstate_outgoing != CSTATE_DISCONNECTED) {
				if (ipsaddr->cstate_outgoing != CSTATE_OUTGOING_WAITING_DISC)
					psm3_ips_proto_ipsaddr_start_disconnect(proto, ipsaddr, force, 1);
				ipsaddr->cstate_outgoing = CSTATE_DISCONNECTED;
				ipsaddr->reconnecting = 0;
				_HFI_CONNDBG_BASIC("Err outgoing disconnect timeout, forced disconnect",
					ipsaddr, "");
			} else {
				// got DREP just as gave up
				_HFI_CONNDBG_BASIC("outgoing disconnected", ipsaddr, "");
			}
			psm3_ips_proto_ipsaddr_done_disconnect(proto, ipsaddr, force, 1);
		}
		psmi_assert(! ipsaddr->s_timeout);
		psmi_assert_always(ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED);
		ipsaddr->waiting_in_disconnect = 0;
		proto->num_connected_outgoing--;
		if (ipsaddr->cstate_incoming == CSTATE_NONE
			|| ipsaddr->cstate_incoming == CSTATE_DISCONNECTED)
		{
			// remote disconnect also done, final stages
			if (psm3_ips_proto_ipsaddr_finalize(proto, ipsaddr, force))
				ips_free_epaddr(array_of_epaddr[i], proto);
		} else {
			// incoming not yet DISCONNECTED
			// leave in outgoing DISCONNECTED
			// when incoming disconnects we'll delay for timewait
			// and then free ipsaddr
		}
		// caller should not access this ipsaddr anymore
		array_of_epaddr[i] = NULL;
	}

fail:
success:
	return err;
}

// we observed a HAL specific issue with the connection
// such as a user space RC QP error
// this can occur anytime, including while we are still waiting in
// psm3_ips_proto_connect or psm3_ips_proto_disconnect
// op, err_str and err_num are only used in error message logging
// and may be any HAL specific meaningful information
// returns:
//	PSM2_OK - HAL start_reconnect successfully called
//		current rc_qp may have been freed
//	PSM2_OK_NO_PROGRESS - nothing done due to state, caller may want
//		to free HAL resources (RC QP) related to connection if it can
//	PSM2_INTERNAL_ERROR - failed to start reconnect, fatal
//		should never actually be returned since we fatally exit here
psm2_error_t
psm3_ips_proto_connection_error(ips_epaddr_t *ipsaddr, const char *op,
	const char *err_str, int err_num, unsigned flags)
{
	struct ips_proto *proto = ipsaddr->epaddr.proto;
	if (ipsaddr->cstate_incoming == CSTATE_DISCONNECTED
		|| (ipsaddr->cstate_outgoing == CSTATE_DISCONNECTED
			|| ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC
			|| ipsaddr->waiting_in_disconnect))
		// ignore, connection being taken down
		return PSM2_OK_NO_PROGRESS;

	// handled DISC above, these are only other valid incoming states
	psmi_assert(ipsaddr->cstate_incoming == CSTATE_NONE
		|| ipsaddr->cstate_incoming == CSTATE_ESTABLISHED
		|| ipsaddr->cstate_incoming == CSTATE_INCOMING_NEED_RECONNECT);

	proto->epaddr_stats.connections_lost++;

	if (! ipsaddr->allow_reconnect) {
		_HFI_CONN_FATAL(PSM2_INTERNAL_ERR, "Fatal connection error", ipsaddr,
			" for %s: %s (%d)", op, err_str, err_num);
		return PSM2_INTERNAL_ERR;
	}
	if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
		psmi_assert(! ipsaddr->reconnecting);
		if (psm3_ips_proto_start_reconnect(proto, ipsaddr, NULL, flags)) {
			// _HFI_ERROR already output w/details
			_HFI_CONNDBG_BASIC("Err failed reconnect", ipsaddr, "");
			goto fatal;
		}
		// incoming already state moved to NEED_RECONNECT as needed
		// CONNDBG already output,
	} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
		psmi_assert(ipsaddr->reconnecting || ipsaddr->waiting_in_connect);
		// our outstanding CONNECT_REQUEST is now stale
		// (wrong QP, count, etc)
		if (psm3_ips_proto_start_reconnect(proto, ipsaddr, NULL, flags)) {
			// _HFI_ERROR already output w/details
			_HFI_CONNDBG_BASIC("Err failed reconnect", ipsaddr, "");
			goto fatal;
		}
		// incoming already state moved to NEED_RECONNECT as needed
		// CONNDBG already output
	} else if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
		// we aren't connected, so we can update our HAL resources
		// but due to our outgoing state no CONNECT_REQUEST will be
		// issued here
		if (psm3_ips_proto_start_reconnect(proto, ipsaddr, NULL, flags)) {
			// _HFI_ERROR already output w/details
			_HFI_CONNDBG_BASIC("Err failed reconnect", ipsaddr, "");
			goto fatal;
		}
		// CONNDBG already output
		// incoming already state moved to NEED_RECONNECT as needed
	} else {
		// covered all the valid outgoing states above, can't get here
		psmi_assert(0);
	}
	return PSM2_OK;

fatal:
	_HFI_CONN_FATAL(PSM2_INTERNAL_ERR,
		"Unable to start reconnect", ipsaddr, " for failed  %s: %s (%d)",
		op, err_str, err_num);
	return PSM2_INTERNAL_ERR;
}
