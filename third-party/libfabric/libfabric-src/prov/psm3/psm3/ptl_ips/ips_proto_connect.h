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
 * define connection version. this is the basic version, optimized
 * version will be added later for scalability.
 * version kept in 2 nibbles in this format: 0xMMmm MM=major, mm=minor version
 */
// a litle paranod as a UD or UDP connect can't reach a STL100 PSM recv context
// but we don't worry about UDP vs UD since can't reach eachother either
#define IPS_CONNECT_VERNO	  0x0200 // 2.0 - epid_size of 24 bytes (3 word)
#define IPS_CONNECT_VER_MAJOR(verno) (((verno) & 0xff00) >> 8)
#define IPS_CONNECT_VER_MINOR(verno) ((verno) & 0x00ff)

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
struct ips_connect_hdr {
	uint16_t connect_verno;	/* should be ver IPS_CONNECT_VERNO */
	uint16_t psm_verno;	/* should be 2.0 */
	uint32_t connidx;	/* ignore if 0xffffffff */
	uint64_t epid_w[3];	/* epid of connector process */
} PACK_SUFFIX;

// payload of CONNECT_REQUEST/REPLY (including header above)
struct ips_connect_reqrep {
	struct ips_connect_hdr hdr;
	uint16_t connect_result;	/* error code */
	uint16_t job_pkey;	/* partition key for verification */
	uint32_t mtu;		/* max PSM payload in bytes */

	uint32_t runid_key;	/* one-time stamp connect key */
	uint32_t initpsn;	/* initial psn for flow */

	char hostname[128];	/* sender's hostname string */
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

			// TBD - we could combine the RDMA=1 and RDMA=2,3
			// sets of fields below into a union and save space
			// or make room for more reserved space

			// For rndv module connection establishment, PSM3_RDMA=1
			// zero if no rndv mod RDMA
			union ibv_gid gid; // sender's gid
			uint32_t rv_index; // senders process index
			uint32_t resv;	// alignment

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
