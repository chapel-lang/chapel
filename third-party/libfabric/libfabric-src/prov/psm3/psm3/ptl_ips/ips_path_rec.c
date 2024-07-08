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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"

/* These functions hand build path records based on local link information
 * and basic destination addressing available to our callers from the EPID
 */

/*
 * These are the default values used in parsing the environment
 * variable PSM3_PATH_NO_LMC_RANGE, which can be used to exclude
 * a range of message sizes from the LMC LID assignments used to
 * implement dispersive routing.
 *
 * This value is 2^32 - 1.
 */
#define DEF_LIMITS_STRING "4294967295:4294967295"
#define DEF_LIMITS_VALUE 4294967295


// unfortunately ibv_rate_to_mult and mult_to_ibv_rate have a bug as they
// omit 100g rate and some others, so we create our own

#define PSM3_GIGABIT 1000000000ULL
// link speed input in units of bytes/sec
// we take a conservative approach since some VMs might report
// atypical speeds due to link throttling.  For such atypical speeds
// we chose to round up
enum psm3_ibv_rate ips_link_speed_to_enum(uint64_t link_speed)
{
	if (link_speed <= 2500000000)
		return PSM3_IBV_RATE_2_5_GBPS;
	else if (link_speed <= 5*PSM3_GIGABIT)
		return PSM3_IBV_RATE_5_GBPS;
	else if (link_speed <= 10*PSM3_GIGABIT)
		return PSM3_IBV_RATE_10_GBPS;
	else if (link_speed <= 14*PSM3_GIGABIT)
		return PSM3_IBV_RATE_14_GBPS;
	else if (link_speed <= 20*PSM3_GIGABIT)
		return PSM3_IBV_RATE_20_GBPS;
	else if (link_speed <= 25*PSM3_GIGABIT)
		return PSM3_IBV_RATE_25_GBPS;
	else if (link_speed <= 28*PSM3_GIGABIT)
		return PSM3_IBV_RATE_28_GBPS;
	else if (link_speed <= 30*PSM3_GIGABIT)
		return PSM3_IBV_RATE_30_GBPS;
	else if (link_speed <= 40*PSM3_GIGABIT)
		return PSM3_IBV_RATE_40_GBPS;
	else if (link_speed <= 50*PSM3_GIGABIT)
		return PSM3_IBV_RATE_50_GBPS;
	else if (link_speed <= 56*PSM3_GIGABIT)
		return PSM3_IBV_RATE_56_GBPS;
	else if (link_speed <= 60*PSM3_GIGABIT)
		return PSM3_IBV_RATE_60_GBPS;
	else if (link_speed <= 80*PSM3_GIGABIT)
		return PSM3_IBV_RATE_80_GBPS;
	else if (link_speed <= 100*PSM3_GIGABIT)
		return PSM3_IBV_RATE_100_GBPS;
	else if (link_speed <= 112*PSM3_GIGABIT)
		return PSM3_IBV_RATE_112_GBPS;
	else if (link_speed <= 120*PSM3_GIGABIT)
		return PSM3_IBV_RATE_120_GBPS;
	else if (link_speed <= 168*PSM3_GIGABIT)
		return PSM3_IBV_RATE_168_GBPS;
	else if (link_speed <= 200*PSM3_GIGABIT)
		return PSM3_IBV_RATE_200_GBPS;
	else if (link_speed <= 300*PSM3_GIGABIT)
		return PSM3_IBV_RATE_300_GBPS;
	else if (link_speed <= 400*PSM3_GIGABIT)
		return PSM3_IBV_RATE_400_GBPS;
	else if (link_speed <= 600*PSM3_GIGABIT)
		return PSM3_IBV_RATE_600_GBPS;
	else if (link_speed <= 800*PSM3_GIGABIT)
		return PSM3_IBV_RATE_800_GBPS;
	else
		return PSM3_IBV_RATE_1200_GBPS;
}

static uint64_t ips_enum_to_link_speed(enum psm3_ibv_rate rate)
{
	switch (rate) {
	case PSM3_IBV_RATE_2_5_GBPS:	return  2500000000;
	case PSM3_IBV_RATE_5_GBPS:	return  5*PSM3_GIGABIT;
	case PSM3_IBV_RATE_10_GBPS:	return  10*PSM3_GIGABIT;
	case PSM3_IBV_RATE_20_GBPS:	return  20*PSM3_GIGABIT;
	case PSM3_IBV_RATE_30_GBPS:	return  30*PSM3_GIGABIT;
	case PSM3_IBV_RATE_40_GBPS:	return  40*PSM3_GIGABIT;
	case PSM3_IBV_RATE_60_GBPS:	return  60*PSM3_GIGABIT;
	case PSM3_IBV_RATE_80_GBPS:	return  80*PSM3_GIGABIT;
	case PSM3_IBV_RATE_120_GBPS:	return 120*PSM3_GIGABIT;
	case PSM3_IBV_RATE_14_GBPS:	return  14*PSM3_GIGABIT;
	case PSM3_IBV_RATE_56_GBPS:	return  56*PSM3_GIGABIT;
	case PSM3_IBV_RATE_112_GBPS:	return 112*PSM3_GIGABIT;
	case PSM3_IBV_RATE_168_GBPS:	return 168*PSM3_GIGABIT;
	case PSM3_IBV_RATE_25_GBPS:	return  25*PSM3_GIGABIT;
	case PSM3_IBV_RATE_100_GBPS:	return 100*PSM3_GIGABIT;
	case PSM3_IBV_RATE_200_GBPS:	return 200*PSM3_GIGABIT;
	case PSM3_IBV_RATE_300_GBPS:	return 300*PSM3_GIGABIT;
	case PSM3_IBV_RATE_28_GBPS:	return  28*PSM3_GIGABIT;
	case PSM3_IBV_RATE_50_GBPS:	return  50*PSM3_GIGABIT;
	case PSM3_IBV_RATE_400_GBPS:	return 400*PSM3_GIGABIT;
	case PSM3_IBV_RATE_600_GBPS:	return 600*PSM3_GIGABIT;
	case PSM3_IBV_RATE_800_GBPS:	return 800*PSM3_GIGABIT;
	case PSM3_IBV_RATE_1200_GBPS:	return 1200*PSM3_GIGABIT;
	default:			return 100*PSM3_GIGABIT;
	}
}
#undef PSM3_GIGABIT

enum psm3_ibv_rate min_rate(enum psm3_ibv_rate a, enum psm3_ibv_rate b)
{
	// unfortunately the ibv_rate enum is not sorted by link rate
	// so we must convert to link_speed to compare then convert back
	return ips_link_speed_to_enum(min(ips_enum_to_link_speed(a),
                                 ips_enum_to_link_speed(b)));
}

/* Convert Timeout value from usec to
 * timeout_mult where usec = 4.096usec * 2^timeout_mult
 */
uint8_t psm3_timeout_usec_to_mult(uint64_t timeout_us)
{
	/* all values are rounded up, comments reflect exact value */
	if (timeout_us <= 4)
		return 0;	/* 4.096 us */
	else if (timeout_us <= 8)
		return 1;	/* 8.192 us */
	else if (timeout_us <= 16)
		return 2;	/* 16.384 us */
	else if (timeout_us <= 32)
		return 3;	/* 32.768 us */
	else if (timeout_us <= 65)
		return 4;	/* 65.536 us */
	else if (timeout_us <= 131)
		return 5;	/* 131.072 us */
	else if (timeout_us <= 262)
		return 6;	/* 262.144 us */
	else if (timeout_us <= 524)
		return 7;	/* 524.288 us */
	else if (timeout_us <= 1048)
		return 8;	/* 1048.576 us */
	else if (timeout_us <= 2097)
		return 9;	/* 2.097 ms */
	else if (timeout_us <= 4194)
		return 10;	/* 4.197 ms */
	else if (timeout_us <= 8388)
		return 11;	/* 8.388 ms */
	else if (timeout_us <= 16777)
		return 12;	/* 16.777 ms */
	else if (timeout_us <= 33554)
		return 13;	/* 33.554 ms */
	else if (timeout_us <= 67108)
		return 14;	/* 67.1 ms */
	else if (timeout_us <= 134217)
		return 15;	/* 134.2 ms */
	else if (timeout_us <= 268435)
		return 16;	/* 268.4 ms */
	else if (timeout_us <= 536870)
		return 17;	/* 536.8 ms */
	else if (timeout_us <= 1073741)
		return 18;/* 1.073 s */
	else if (timeout_us <= 2147483)
		return 19;/* 2.148 s */
	else if (timeout_us <= 4294967)
		return 20;/* 4.294 s */
	else if (timeout_us <= 8589934)
		return 21;/* 8.589 s */
	else if (timeout_us <= 17179869)
		return 22;/* 17.179 s */
	else if (timeout_us <= 34359738)
		return 23;/* 34.359 s */
	else if (timeout_us <= 68719476)
		return 24;/* 68.719 s */
	else if (timeout_us <= 137438953ll)
		return 25;/* 2.2 minutes */
	else if (timeout_us <= 274877906ll)
		return 26; /* 4.5 minutes */
	else if (timeout_us <= 549755813ll)
		return 27; /* 9 minutes */
	else if (timeout_us <= 1099511628ll)
		return 28;	/* 18 minutes */
	else if (timeout_us <= 2199023256ll)
		return 29;	/* 0.6 hr */
	else if (timeout_us <= 4398046511ll)
		return 30;	/* 1.2 hr	 */
	else
		return 31;	/* 2.4 hr */
}

static psm2_error_t
ips_none_get_path_rec(struct ips_proto *proto,
		      __be16 slid, __be16 dlid,
		      __be64 gid_hi, __be64 gid_lo,
		      unsigned long timeout, ips_path_rec_t **ppath_rec)
{
	psm2_error_t err = PSM2_OK;
	ips_path_rec_t *path_rec;
	ENTRY elid = {}, *epath = NULL;
	char eplid[128];

	/* Query the path record cache */
	/* the eplid is simply an exact match search key, we don't worry
	 * about slid, dlid and gid being big endian.  In fact on little
	 * endian CPU, this will put low bits earlier in string and cause
	 * quicker discovery of differences when doing strcmp to sort/search
	 */
	// TBD - slid same until have dispersive LMC-like, could just use dest
	snprintf(eplid, sizeof(eplid), "%x_%"PRIx64"_%"PRIx64"_%x", slid, (uint64_t)gid_lo, (uint64_t)gid_hi, dlid);
	elid.key = eplid;
	hsearch_r(elid, FIND, &epath, &proto->ips_path_rec_hash);

	if (!epath) {
		elid.key =
		    psmi_calloc(proto->ep, UNDEFINED, 1, strlen(eplid) + 1);
		path_rec = (ips_path_rec_t *)
		    psmi_calloc(proto->ep, UNDEFINED, 1,
				sizeof(ips_path_rec_t));
		if (!elid.key || !path_rec) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}

		/* Create path record */
		path_rec->pr_slid = slid;	/* __be16 */
		path_rec->pr_dlid = dlid;	/* __be16 */
		path_rec->pr_mtu = proto->epinfo.ep_mtu;
		path_rec->pr_pkey = proto->epinfo.ep_pkey;
		path_rec->pr_sl = proto->epinfo.ep_sl;
		path_rec->pr_gid_hi = gid_hi;	/* __be64 */
		path_rec->pr_gid_lo = gid_lo;	/* __be64 */
		path_rec->pr_static_rate = proto->epinfo.ep_link_rate;

		if (path_rec->pr_sl > PSMI_SL_MAX) {
			err =  PSM2_INTERNAL_ERR;
			goto fail;
		}
		err = psmi_hal_ips_path_rec_init(proto, path_rec, NULL);
		if (err != PSM2_OK)
			goto fail;

		/* Add path record into cache */
		strcpy(elid.key, eplid);
		elid.data = (void *)path_rec;
		hsearch_r(elid, ENTER, &epath, &proto->ips_path_rec_hash);
	} else
		path_rec = (ips_path_rec_t *) epath->data;

	/* Return IPS path record */
	*ppath_rec = path_rec;

	return err;

fail:
	if (elid.key)
		psmi_free(elid.key);
	if (path_rec)
		psmi_free(path_rec);
	return err;
}

static psm2_error_t
ips_none_path_rec(struct ips_proto *proto,
		  __be16 slid, __be16 dlid,
		  __be64 gid_hi, __be64 gid_lo,
		  unsigned long timeout, ips_path_grp_t **ppathgrp)
{
	psm2_error_t err = PSM2_OK;
	uint16_t pidx, num_path = (1 << proto->epinfo.ep_lmc);
	__be16 path_slid, path_dlid;
	ips_path_rec_t *path;
	ips_path_grp_t *pathgrp;
	ENTRY elid = {}, *epath = NULL;
	char eplid[128];

	/* For the "none" path record resolution all paths are assumed to be
	 * of equal priority however since we want to isolate all control
	 * traffic (acks, naks) to a separate path for non zero LMC subnets
	 * the "first path" between a pair of endpoints is always the "higher"
	 * priority paths. The rest of the paths are the normal (and low
	 * priority) paths.
	 */

	/* Query the path record cache */
	/* the eplid is simply an exact match search key, we don't worry
	 * about slid, dlid and gid being big endian.  In fact on little
	 * endian CPU, this will put low bits earlier in string and cause
	 * quicker discovery of differences when doing strcmp to sort/search
	 */
	// TBD - slid same until have dispersive LMC-like, could just use dest
	snprintf(eplid, sizeof(eplid), "%x_%"PRIx64"_%"PRIx64"_%x", slid, (uint64_t)gid_lo, (uint64_t)gid_hi, dlid);
	elid.key = eplid;
	hsearch_r(elid, FIND, &epath, &proto->ips_path_grp_hash);

	if (epath) {		/* Find path group in cache */
		*ppathgrp = (ips_path_grp_t *) epath->data;
		return err;
	}

	/* If base lids are only used then reset num_path to 1 */
	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_BASE)
		num_path = 1;

	/* Allocate a new pathgroup */
	elid.key = psmi_calloc(proto->ep, UNDEFINED, 1, strlen(eplid) + 1);
	pathgrp = (ips_path_grp_t *)
	    psmi_calloc(proto->ep, UNDEFINED, 1, sizeof(ips_path_grp_t) +
			num_path * IPS_PATH_MAX_PRIORITY *
			sizeof(ips_path_rec_t *));
	if (!elid.key || !pathgrp) {
		if (elid.key)
			psmi_free(elid.key);
		if (pathgrp)
			psmi_free(pathgrp);
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	/*
	 * dlid is the peer base lid.
	 * slid is the base lid for the local end point.
	 * Store in network byte order.
	 */
	pathgrp->pg_base_dlid = dlid;
	pathgrp->pg_base_slid = slid;

	if (num_path > 1) {
		/* One control path and (num_path - 1) norm and low priority paths */
		pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY] = 1;
		pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY] = num_path - 1;
		pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY] = num_path - 1;
	} else
	{
		/* LMC of 0. Use the same path for all priorities */
		pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY] = 1;
		pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY] = 1;
		pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY] = 1;
	}

	/* For "none" path record we just setup 2^lmc paths. To get better load
	 * balance
	 */
	for (pidx = 0; pidx < num_path; pidx++) {
		path_slid = __cpu_to_be16(__be16_to_cpu(slid) + pidx);
		path_dlid = __cpu_to_be16(__be16_to_cpu(dlid) + pidx);

		err =
		    ips_none_get_path_rec(proto, path_slid, path_dlid,
					  gid_hi, gid_lo,
					  timeout, &path);
		if (err != PSM2_OK) {
			psmi_free(elid.key);
			psmi_free(pathgrp);
			goto fail;
		}

		if (num_path > 1) {
			if (pidx == 0) {
				/* First path is always the high priority path */
				pathgrp->pg_path[0][IPS_PATH_HIGH_PRIORITY] =
				    path;
			} else {
				pathgrp->pg_path[pidx -
						 1][IPS_PATH_NORMAL_PRIORITY] =
				    path;
				pathgrp->pg_path[pidx -
						 1][IPS_PATH_LOW_PRIORITY] =
				    path;
			}
		} else
		{
			pathgrp->pg_path[0][IPS_PATH_HIGH_PRIORITY] = path;
			pathgrp->pg_path[0][IPS_PATH_NORMAL_PRIORITY] = path;
			pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY] = path;
		}
                PSM2_LOG_MSG("path %p slid %hu dlid %hu gid %0x"PRIx64":%"PRIx64"\n",
                              path,
			      __be16_to_cpu(path->pr_slid),
			      __be16_to_cpu(path->pr_dlid),
			      __be64_to_cpu(path->pr_gid_hi),
			      __be64_to_cpu(path->pr_gid_lo));

	}

	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE) {
		pathgrp->pg_next_path[IPS_PATH_NORMAL_PRIORITY] =
		    proto->epinfo.ep_hash %
		    pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY];
		pathgrp->pg_next_path[IPS_PATH_LOW_PRIORITY] =
		    proto->epinfo.ep_hash %
		    pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY];
	}

	/* Add path record into cache */
	strcpy(elid.key, eplid);
	elid.data = (void *)pathgrp;
	hsearch_r(elid, ENTER, &epath, &proto->ips_path_grp_hash);

	*ppathgrp = pathgrp;

fail:
	if (err != PSM2_OK)
		_HFI_CONNDBG
		    ("Unable to get path record for %s port %u LID %x <---> DLID %x.\n",
		     proto->ep->dev_name, proto->ep->portnum, slid, dlid);
	return err;
}

/* parse error check timeouts for PSM3_ERRCHK_TIMEOUT or PSM3_ERRCHK_TIMEOUT_US
 * format is min:max:factor
 * all must be non-zero, min must be <= max
 * Either field can be omitted in which case default (input tvals) is used
 * for given field.
 * 0 - successfully parsed, tvals updated
 * -1 - str empty, tvals unchanged
 * -2 - syntax error, tvals may have been changed
 */
static int parse_errchk_timeout(const char *str,
			size_t errstr_size, char errstr[],
			int tvals[3])
{
	psmi_assert(tvals);
	int ret = psm3_parse_str_tuples(str, 3, tvals);
	if (ret < 0)
		return ret;
	if (tvals[0] < 0 || tvals[1] < 0 || tvals[2] < 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Negative values not allowed");
		return -2;
	}
	if (tvals[0] == 0 || tvals[1] == 0 || tvals[2] == 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Zero values not allowed");
		return -2;
	}
	if (tvals[0] > tvals[1]) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " min (%d) must be <= max (%d)", tvals[0], tvals[1]);
		return -2;
	}
	return 0;
}

static int parse_check_errchk_timeout(int type,
			const union psmi_envvar_val val, void *ptr,
			size_t errstr_size, char errstr[])
{
	// parser will set tvals to result, use a copy to protect input of defaults
	int tvals[3] = { ((int*)ptr)[0], ((int*)ptr)[1], ((int*)ptr)[2] };
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR_TUPLES);
	return parse_errchk_timeout(val.e_str, errstr_size, errstr, tvals);
}

static psm2_error_t ips_none_path_rec_init(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	/* Obtain the SL and PKEY to use from the environment (PSM3_NIC_SL & PSM_KEY) */
	proto->epinfo.ep_sl = proto->ep->out_sl;
	proto->epinfo.ep_pkey = (uint16_t) proto->ep->network_pkey;

	/*
	 * Parse the err_chk settings from the environment.
	 * <min_timeout>:<max_timeout>:<timeout_factor>
	 */
	{
		union psmi_envvar_val env_to;
		int tvals[3] = {
			IPS_PROTO_ERRCHK_MS_MIN_DEFAULT,
			IPS_PROTO_ERRCHK_MS_MAX_DEFAULT,
			IPS_PROTO_ERRCHK_FACTOR_DEFAULT
		};

		(void)psm3_getenv_range("PSM3_ERRCHK_TIMEOUT",
				 "Errchk timeouts in milliseconds <min:max:factor>",
				 "Specified as min:max:factor where min and max is the range of timeouts\nand factor is the multiplier for growing timeout",
				 PSMI_ENVVAR_LEVEL_USER,
				 PSMI_ENVVAR_TYPE_STR_TUPLES,
				 (union psmi_envvar_val)PSM_TID_TIMEOUT_DEFAULT,
				 (union psmi_envvar_val)NULL,
				 (union psmi_envvar_val)NULL,
				 parse_check_errchk_timeout, tvals, &env_to);
		if (parse_errchk_timeout(env_to.e_str, 0, NULL, tvals) < 0) {
			// already checked, shouldn't get parse errors nor empty strings
			psmi_assert(0);
		}

		proto->epinfo.ep_timeout_ack = ms_2_cycles(tvals[0]);
		proto->epinfo.ep_timeout_ack_max = ms_2_cycles(tvals[1]);
		proto->epinfo.ep_timeout_ack_factor = tvals[2];

#ifdef PSM_FI
		/* when doing Fault Injection to test send DMA completion races
		 * it can be useful to set very aggressive timeouts such that
		 * ack_timeout fires before send DMA is locally completed.
		 * This allows values in units of microseconds and will override
		 * any values specified in PSM3_ERRCHK_TIMEOUT
		 */
		int us_tvals[3] = {
			IPS_PROTO_ERRCHK_MS_MIN_DEFAULT*1000,
			IPS_PROTO_ERRCHK_MS_MAX_DEFAULT*1000,
			IPS_PROTO_ERRCHK_FACTOR_DEFAULT
		};
		if (1 > psm3_getenv_range("PSM3_ERRCHK_TIMEOUT_US",
				 "Errchk timeouts in microseconds <min:max:factor>",
				 "Specified as min:max:factor where min and max is the range of timeouts\nand factor is the multiplier for growing timeout",
				 PSMI_ENVVAR_LEVEL_USER,
				 PSMI_ENVVAR_TYPE_STR_TUPLES,
				 (union psmi_envvar_val)PSM_TID_TIMEOUT_DEFAULT_US,
				 (union psmi_envvar_val)NULL,
				 (union psmi_envvar_val)NULL,
				 parse_check_errchk_timeout, us_tvals, &env_to)) {
			// value specified (perhaps bad input), use
			// what was returned (will be default if bad input)
			if (parse_errchk_timeout(env_to.e_str, 0, NULL, us_tvals) < 0) {
				// already checked, shouldn't get parse errors nor empty strings
				psmi_assert(0);
			}
			proto->epinfo.ep_timeout_ack = us_2_cycles(us_tvals[0]);
			proto->epinfo.ep_timeout_ack_max = us_2_cycles(us_tvals[1]);
			proto->epinfo.ep_timeout_ack_factor = us_tvals[2];
		}
#endif /* PSM_FI */
	}

	proto->ibta.get_path_rec = ips_none_path_rec;
	proto->ibta.fini = NULL;


	return err;
}

psm2_error_t
MOCKABLE(psm3_ips_ibta_init)(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;
	union psmi_envvar_val path_disable_lmc_interval;

	if ((err = psmi_hal_ips_ibta_init(proto)) != PSM2_OK)
		goto fail;

	/* Initialize path record/group hash table */

	{
		uint32_t lmc_disable_low, lmc_disable_high;
		int sscanf_ret;

		/* The default disable_low and disable_low values
		 * are 2^32 - 1, the maximum allowable message size.
		 * So by default all messages should be smaller than the
		 * lower limit, and so will not have LMC dispersive
		 * routing disabled.
		 *
		 * Add to this, these limits are applied only to SDMA
		 * and PIO message, NOT TID messages.  So this size
		 * bigger than any PIO size.
		 */
		psm3_getenv("PSM3_PATH_NO_LMC_RANGE",
		            "Disable LMC route dispersion within this range, "
		             "low_value:high_value\n",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
			    (union psmi_envvar_val)DEF_LIMITS_STRING,
			    &path_disable_lmc_interval);

		sscanf_ret = sscanf(path_disable_lmc_interval.e_str, "%u:%u",
		       		   &lmc_disable_low, &lmc_disable_high);

		/*
		 * It's "invalid" for the low end of the range to be
		 * larger than the hig end of the range, so revert
		 * to the "maximum message size" (2^32 - 1).
		 */
		if ((sscanf_ret != 2) || (lmc_disable_low > lmc_disable_high)) {
			lmc_disable_low = lmc_disable_high = DEF_LIMITS_VALUE;
		}

		PSM2_LOG_MSG("PSM3_PATH_NO_LMC_RANGE: "
			     "lmc_disable_low %u lmc_disable_high %u\n",
			     lmc_disable_low, lmc_disable_high);

		/*
		 * These specify the range of message sizes in bytes, of
		 * the messages to disable LMC dynamic LID assignment.
		 */
		proto->ips_lmc_disable_low = lmc_disable_low;
		proto->ips_lmc_disable_high = lmc_disable_high;
	}

	hcreate_r(DF_PATH_REC_HASH_SIZE, &proto->ips_path_rec_hash);
	hcreate_r(DF_PATH_GRP_HASH_SIZE, &proto->ips_path_grp_hash);

	/* setup initial link state */
	if ((err = psmi_hal_ips_proto_update_linkinfo(proto)) != PSM2_OK)
		goto fail;

	/* Setup the appropriate query interface for the endpoint */
	switch (proto->ep->path_res_type) {
	case PSM2_PATH_RES_OPP:
		err = psm3_ips_opp_init(proto);
		if (err != PSM2_OK)
			_HFI_ERROR
			    ("Unable to use OFED Plus Plus for path record queries.\n");
		break;
	case PSM2_PATH_RES_UMAD:
		_HFI_ERROR
		    ("Path record queries using UMAD is not supported in PSM version %d.%dx\n",
		     PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR);
		err = PSM2_EPID_PATH_RESOLUTION;
		break;
	case PSM2_PATH_RES_NONE:
	default:
		err = ips_none_path_rec_init(proto);
	}

fail:
	return err;
}
MOCK_DEF_EPILOGUE(psm3_ips_ibta_init);

psm2_error_t psm3_ips_ibta_fini(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	if (proto->ibta.fini)
		err = proto->ibta.fini(proto);

	/* Destroy the path record/group hash */
	hdestroy_r(&proto->ips_path_rec_hash);
	hdestroy_r(&proto->ips_path_grp_hash);

	return err;
}
