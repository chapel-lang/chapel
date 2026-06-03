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

/* Copyright (c) 2009-2014 Intel Corporation. All rights reserved. */


#ifndef _IPS_PATH_REC_H_
#define _IPS_PATH_REC_H_

#include <search.h>
#if defined(PSM_VERBS)
#include <infiniband/verbs.h>
#endif
/* Default size of path record hash table */
#define DF_PATH_REC_HASH_SIZE 2047

/* Default size of path group hash table */
#define DF_PATH_GRP_HASH_SIZE 255

/* A lot of these are IBTA specific defines that are available in other header
 * files. To minimize dependencies with PSM build process they are listed
 * here. Most of this is used to implement IBTA compliance features with PSM
 * like path record query etc.
 */

enum opa_mtu {
	IBTA_MTU_256  = 1,
	IBTA_MTU_512  = 2,
	IBTA_MTU_1024 = 3,
	IBTA_MTU_2048 = 4,
	IBTA_MTU_4096 = 5,
	OPA_MTU_8192  = 6,
	OPA_MTU_10240 = 7,
	IBTA_MTU_MIN  = IBTA_MTU_256,
	IBTA_MTU_MAX  = IBTA_MTU_4096,
	OPA_MTU_MIN   = IBTA_MTU_256,
	OPA_MTU_MAX   = OPA_MTU_10240,
};

// some older distros lack some of the newer speeds, so we need to
// create our own enum comparable to ibv_rate
enum psm3_ibv_rate {
	PSM3_IBV_RATE_MAX	= 0,
	PSM3_IBV_RATE_2_5_GBPS	= 2,
	PSM3_IBV_RATE_5_GBPS	= 5,
	PSM3_IBV_RATE_10_GBPS	= 3,
	PSM3_IBV_RATE_20_GBPS	= 6,
	PSM3_IBV_RATE_30_GBPS	= 4,
	PSM3_IBV_RATE_40_GBPS	= 7,
	PSM3_IBV_RATE_60_GBPS	= 8,
	PSM3_IBV_RATE_80_GBPS	= 9,
	PSM3_IBV_RATE_120_GBPS	= 10,
	PSM3_IBV_RATE_14_GBPS	= 11,
	PSM3_IBV_RATE_56_GBPS	= 12,
	PSM3_IBV_RATE_112_GBPS	= 13,
	PSM3_IBV_RATE_168_GBPS	= 14,
	PSM3_IBV_RATE_25_GBPS	= 15,
	PSM3_IBV_RATE_100_GBPS	= 16,
	PSM3_IBV_RATE_200_GBPS	= 17,
	PSM3_IBV_RATE_300_GBPS	= 18,
	PSM3_IBV_RATE_28_GBPS	= 19,
	PSM3_IBV_RATE_50_GBPS	= 20,
	PSM3_IBV_RATE_400_GBPS	= 21,
	PSM3_IBV_RATE_600_GBPS	= 22,
	PSM3_IBV_RATE_800_GBPS	= 23,
	PSM3_IBV_RATE_1200_GBPS	= 24,
};

static inline int opa_mtu_enum_to_int(enum opa_mtu mtu)
{
	switch (mtu) {
	case IBTA_MTU_256:
		return 256;
	case IBTA_MTU_512:
		return 512;
	case IBTA_MTU_1024:
		return 1024;
	case IBTA_MTU_2048:
		return 2048;
	case IBTA_MTU_4096:
		return 4096;
	case OPA_MTU_8192:
		return 8192;
	case OPA_MTU_10240:
		return 10240;
	default:
		return -1;
	}
}

static inline enum opa_mtu opa_mtu_int_to_enum(int mtu)
{
	// the PSM mtu may be slightly less than wire MTU to allow for
	// PSM headers, so round up to nearest MTU enum
	if (mtu <= 256)
		return IBTA_MTU_256;
	else if (mtu <= 512)
		return IBTA_MTU_512;
	else if (mtu <= 1024)
		return IBTA_MTU_1024;
	else if (mtu <= 2048)
		return IBTA_MTU_2048;
	else if (mtu <= 4096)
		return IBTA_MTU_4096;
// TBD if we should allow these values on standard verbs
	else if (mtu <= 8192)
		return OPA_MTU_8192;
	else
		return OPA_MTU_10240;
}

#ifdef PSM_VERBS
static inline enum ibv_mtu ibv_mtu_int_to_enum(int mtu)
{
	// the PSM mtu may be slightly less than wire MTU to allow for
	// PSM headers, so round up to nearest MTU enum
	if (mtu <= 256)
		return IBV_MTU_256;
	else if (mtu <= 512)
		return IBV_MTU_512;
	else if (mtu <= 1024)
		return IBV_MTU_1024;
	else if (mtu <= 2048)
		return IBV_MTU_2048;
	else
		return IBV_MTU_4096;
}
#endif

/* Convert Timeout value from usec to Verbs/IB/RoCE
 * timeout_mult where usec = 4.096usec * 2^timeout_mult
 */
uint8_t psm3_timeout_usec_to_mult(uint64_t timeout_us);

/* This is same as ob_path_rec from ib_types.h. Listed here to be self
 * contained to minimize dependencies during build etc.
 */
typedef struct _ibta_path_rec {
	__be64 service_id;	/* net order */
	uint8_t dgid[16];
	uint8_t sgid[16];
	__be16 dlid;		/* net order */
	__be16 slid;		/* net order */
	__be32 hop_flow_raw;	/* net order */
	uint8_t tclass;
	uint8_t num_path;
	__be16 pkey;		/* net order */
	__be16 qos_class_sl;	/* net order */
	uint8_t mtu;		/* IBTA encoded */
	uint8_t rate;		/* IBTA encoded */
	uint8_t pkt_life;	/* IBTA encoded */
	uint8_t preference;
	uint8_t resv2[6];
} ibta_path_rec_t;

/*
 * PSM IPS path record components for endpoint.
 *
 * For Torus/non-zero LMC fabrics, pr_slid and pr_dlid may be different from
 * the "base lid" values for this connection.
 */
struct ips_proto;

/* We manually align fields and pack since O(nodes) instances and
 * union could force some undesired extra alignment bytes
 */
typedef struct ips_path_rec {
	__be16 pr_slid;
	__be16 pr_dlid;
	uint16_t pr_pkey;
	uint8_t pr_sl;
	uint8_t pr_static_rate;	// psm3_ibv_rate enum
	__be64 pr_gid_hi;	// for ethernet, has IPv4 or IPv6
	__be64 pr_gid_lo;	// addr in IPv6 style
	uint32_t pr_mtu;	/* PSM payload in bytes, <= Path's MTU */
				// TBD - could reduce to 2 bytes by storing
				// as number of dwords instead of bytes
	union {
#ifdef PSM_VERBS
		// each path_rec is shared for all remote processes on a
		// a given node.  So this is a convenient place to have
		// any shared addressing such as verbs AVs and RV connections
		struct {
			// 32b aligned at start of struct
#ifdef PSM_HAVE_RNDV_MOD
			uint8_t pr_connecting;
			uint8_t pr_pad[3]; // for alignment
			psm3_rv_conn_t pr_rv_conn;
#else
			uint8_t pr_pad[4]; // for alignment
#endif
			// address handle for UD comms
			struct ibv_ah *pr_ah;
		} PACK_SUFFIX verbs;
#endif /* PSM_VERBS */
#ifdef PSM_SOCKETS
		struct {
			// 32b aligned at start of struct
			uint8_t pr_pad[4]; // for alignment
		} sockets;
#endif /* PSM_SOCKETS */
	} PACK_SUFFIX;
} PACK_SUFFIX ips_path_rec_t;

enum psm3_ibv_rate ips_link_speed_to_enum(uint64_t link_speed);
enum psm3_ibv_rate min_rate(enum psm3_ibv_rate a, enum psm3_ibv_rate b);
psm2_error_t psm3_ips_opp_init(struct ips_proto *proto);

#endif /* _IPS_PATH_REC_H_ */
