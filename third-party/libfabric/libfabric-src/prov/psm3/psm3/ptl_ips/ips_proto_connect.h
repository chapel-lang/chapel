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

#ifndef _IPS_PROTO_CONNECT_H
#define _IPS_PROTO_CONNECT_H

/* IPS connection management (connect and disconnect)  packet formats */

/* These packet formats are payloads within the basic PSM IPS header
 * as defined in ips_proto_header.h
 */

/*
 * define connection version.
 * version kept in 2 nibbles in this format: 0xMMmm MM=major, mm=minor version
 * Note that a UD or UDP connect can't reach a STL100 PSM recv context.
 * We don't worry about UDP vs UD since can't reach eachother either.
 */
#define IPS_CONNECT_VERNO	  0x0201 // 2.1
// 1.0 was STL100 PSM
//   epid of 8 bytes (1 word)
// 2.0 moved to epid_size of 24 bytes (3 words).  Cannot interop with 1.x
// 2.1 added:
//   - optional HAL reconnect with new reconnect_count (for user RC QP)
//   - connidx high bit in connect_hdr indiates disconnect timewait (! force)
//   when 2.1 connects with 2.0, the mismatch disables the reconnect and
//   timewait features in the 2.1 peer.
#define IPS_CONNECT_VER_MAJOR(verno) (((verno) & 0xff00) >> 8)
#define IPS_CONNECT_VER_MINOR(verno) ((verno) & 0x00ff)

// does connection version allow HAL reconnect
#define IPS_CONNECT_VER_ALLOW_RECONNECT(verno) \
	(IPS_CONNECT_VER_MAJOR(verno) > 2 \
	|| (IPS_CONNECT_VER_MAJOR(verno) == 2 \
		&& IPS_CONNECT_VER_MINOR(verno) >= 1))

// when sending CONNECT_REQUEST outside of psm2_connect, how long to
// wait if output queue full/busy before reattempt.  Kept short.
#define IPS_CONNECT_BUSY_TIMER (20 * NSEC_PER_USEC) // in nanoseconds

// how long to stay in DISCONNECTED state for timewait before free ipsaddr
// and allow a fresh connect.  A fixed timer is used since
// ep->reconnect_timeout could be quite large.  Since this is 1/2 second
// it is always less than reconnect_timeout, so remote retries
// should be able to complete a connect after disconnect even if disconnect
//  must time wait (provided both sides disconnect so time wait starts).
#define IPS_CONNECT_TIMEWAIT (500 * NSEC_PER_MSEC)

// prior to attempting connections, the address formats of each rail are checked
// on both sides and must exactly match.  Unfortunately, the epid's exchanged
// in the runtime must all be of the same size so we can't interop between
// old and new PSM due OFI using a single psmx3_ep_name structure size and
// PSM2 API using a single psm2_epid_t structure size.
//
// The epid address formats can be different per rail.

// The information in the 1st part of the header dictates the format for the
// rest of the header and for the main connect payload.
// The connect_verno dictates the hdr format and multi-rail format.
// When connect_verno major version is equal, connections are allowed and
// psm_verno may indicate any differences in the remaining PSM wire protocols.
// in CONNECT_REQUEST/REPLY, connidx is the senders index into
//   ips_epaddr table (plus a base offset randomized at job start).  The index
//   is limited in size by IPS_EPSTATE_CONNIDX_MAX.
// in DISCONNECT_REQUEST/REPLY connidx is not used. For DISCON_REQ it holds
//   just the IPS_DISCONNECT_NOFORCE bit.  Note in 2.0 connect protocol the
//   connidx held the index, however in 2.0 builds limited it to 26 bits so
//   the high bit can safely be used for NOFORCE even when 2.0 & 2.1 interop.
//   For further safety, when 2.1 interops with 2.0 it disables 2.1 timewait
//   and effectively ignores connidx in incoming disconnect packets.
struct ips_connect_hdr {
	uint16_t connect_verno;	/* should be ver IPS_CONNECT_VERNO */
	uint16_t psm_verno;	/* should be 2.0 */
	uint32_t connidx;	/* ignore if 0xffffffff */
#define IPS_DISCONNECT_NOFORCE 0x80000000	// discon w/o force (timewait)
	uint64_t epid_w[3];	/* epid of connector process */
} PACK_SUFFIX;

// payload of CONNECT_REQUEST/REPLY (including header above)
// In 2.1 and later connect protocol, the reconnect_count is used to indicate
// HAL reconnection count in REQ/REP (user RC QP reconnect for verbs).
// Only the initial connect is 0, while reconnects increment reconnect_count
// and wrap after 255 back to 1.
// The protocol is designed such that peers will have a value within 1 of
// eachother and when an incoming REQ/REP is 1 beyond the local value (in
// ips_epaddr) the receiver knows peer is performing or expecting a reconnect.
struct ips_connect_reqrep {
	struct ips_connect_hdr hdr;
	uint8_t connect_result;	/* error code */
	uint8_t reconnect_count; /* incrementing count */
		
	uint16_t job_pkey;	/* partition key for verification */
	uint32_t mtu;		/* max PSM payload in bytes */

	uint32_t runid_key;	/* one-time stamp connect key (PID) */
	uint32_t initpsn;	/* initial psn for flow */

	char hostname[128];	/* sender's hostname string */
	// sl is 16 bits in many PSM fields, but only 4 bits in IB PathRecord
	// and only 8 bits in ips_path_rec.pr_sl. PSMI_SL_MAX is 31
	// Ethernet TClass is 8 bits, so at least 1 unused byte here for
	// future expansion by reducing sl to 8 bits.
	uint16_t sl;		/* service level for matching */
	uint8_t static_rate;	// ibv_rate enum, ignored for OPA
	uint8_t wiremode;	// sub-mode within epid_protocol
				// for RoCE and IB this is RDMA Mode
				// (eg. rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK)
				// Other protocols currently have only 1 mode
				// (note UDP vs TCP are unique EPID protocols)
	uint8_t pad[4];
	union {
		uint8_t hal_pad[64];// all HAL specific structs <= 64 total
#ifdef PSM_VERBS
		// fields below added as part of IPS_CONNECT_VERNO 1.0
		// used for rndv and user space RC QP connection
		// (CONNECT_REQUEST/REPLY)
		struct {
			// if evolve this structure, add a version field here
			uint8_t reserved[16];	// 64b aligned
			// fields below can be zero depending on rdmamode

			union {
				struct {
					// For rndv module connection establishment, PSM3_RDMA=1
					// zero if no rndv mod RDMA
					union ibv_gid gid; // sender's gid
					uint32_t rv_index; // senders process index
					uint32_t resv;	// alignment
				} rv;
				struct {
#ifdef USE_RDMA_READ
					// For PSM3_RDMA=3 only
					uint64_t recv_addr;
					uint32_t recv_rkey;
					uint8_t resv[12];
#else
					// reserved for future use
					// When need, we can potentially use the fields here to
					// delivery data during connection establishment
					uint8_t resv[24];
#endif
				} urc; // user space RC
			};

			// For user space RC QP connection establishment
			// only set for USE_RC with PSM3_RDMA=2 or 3
			// zero if no user space RC QPs
			struct psm_rc_qp_attr qp_attr;
		} verbs PACK_SUFFIX;
#endif /* PSM_VERBS */
#ifdef PSM_SOCKETS
		//struct {
		//} sockets PACK_SUFFIX;
#endif /* PSM_SOCKETS */
	} PACK_SUFFIX;
	// Beware, with PSMI_MAX_QPS (32) rails this
	// payload is 248 (connect_hdr) + 768 bytes (32*24) + 56 byte PSM header
	// (+ opt 8B CRC for debug) = 1080
	// To allow RoCE UD on 1500 byte MTU ethernet links
	// we must limit our total UD payload to 1024 bytes including PSM header
	// which implies such a config can only have 29 rails (30 w/o debug CRC)
	//
#define IPS_CONNECT_RAIL_ADDR_SIZE 3 // size per rail in 64b words
#define IPS_CONNECT_RAIL_ADDR_LEN (3*sizeof(uint64_t)) // length in bytes
	// For a multi-rail and/or multi-QP run, Up to PSMI_MAX_QPS of rail_addr
	// follow (24 bytes per rail).
	//		3 word epid format - has full IB/OPA/IPv4/IPv6 subnet
	// if we run out of space in a future IPS_CONNECT_VERNO we could
	// probably compact the IPv6 epid into 20 bytes per rail but leave at
	// 24 bytes in connect_hdr for good field alignment
} PACK_SUFFIX;

#endif /* _IPS_PROTO_CONNECT_H */
