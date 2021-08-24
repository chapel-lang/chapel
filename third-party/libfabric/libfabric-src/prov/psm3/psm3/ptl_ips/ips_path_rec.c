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





static psm2_error_t
ips_none_get_path_rec(struct ips_proto *proto,
		      uint16_t slid, uint16_t dlid,
		      uint16_t ip_hi,
		      unsigned long timeout, ips_path_rec_t **ppath_rec)
{
	psm2_error_t err = PSM2_OK;
	ips_path_rec_t *path_rec;
	ENTRY elid, *epath = NULL;
	char eplid[128];

	/* Query the path record cache */
	// TBD - slid same until have dispersive LMC-like, could just use dest
	snprintf(eplid, sizeof(eplid), "%x_%x%04x", slid, ip_hi, dlid);
	elid.key = eplid;
	hsearch_r(elid, FIND, &epath, &proto->ips_path_rec_hash);

	if (!epath) {
		elid.key =
		    psmi_calloc(proto->ep, UNDEFINED, 1, strlen(eplid) + 1);
		path_rec = (ips_path_rec_t *)
		    psmi_calloc(proto->ep, UNDEFINED, 1,
				sizeof(ips_path_rec_t));
		if (!elid.key || !path_rec) {
			if (elid.key)
				psmi_free(elid.key);
			if (path_rec)
				psmi_free(path_rec);
			return PSM2_NO_MEMORY;
		}

		/* Create path record */
		path_rec->pr_slid = slid;
		path_rec->pr_dlid = dlid;
		path_rec->pr_mtu = proto->epinfo.ep_mtu;
		path_rec->pr_pkey = proto->epinfo.ep_pkey;
		path_rec->pr_sl = proto->epinfo.ep_sl;
		path_rec->pr_ip_hi = ip_hi;
		path_rec->pr_static_rate = proto->epinfo.ep_link_rate;


		/* Setup CCA parameters for path */
		if (path_rec->pr_sl > PSMI_SL_MAX) {
			psmi_free(elid.key);
			psmi_free(path_rec);
			return PSM2_INTERNAL_ERR;
		}
		err = ips_make_ah(proto->ep, path_rec);
		if (err != PSM2_OK) {
			psmi_free(elid.key);
			psmi_free(path_rec);
			return err;
		}

		/* Add path record into cache */
		strcpy(elid.key, eplid);
		elid.data = (void *)path_rec;
		hsearch_r(elid, ENTER, &epath, &proto->ips_path_rec_hash);
	} else
		path_rec = (ips_path_rec_t *) epath->data;

	/* Return IPS path record */
	*ppath_rec = path_rec;

	return err;
}

// This works for UD address vectors as well as the ah_attr in an RC QP attrs
psm2_error_t ips_path_rec_to_ah_attr(psm2_ep_t ep,
				const ips_path_rec_t *path_rec, struct ibv_ah_attr *ah_attr)
{
	memset(ah_attr, 0, sizeof *ah_attr);

	// we keep PR in network byte order
	// ah_attr is in CPU byte order except for GIDs which are always
	// in network byte order
	ah_attr->sl = path_rec->pr_sl;
	ah_attr->port_num = ep->portnum;
	ah_attr->static_rate = path_rec->pr_static_rate;
	// for OPA/IB we use dlid and is_global=0, for eth use dgid and is_global=1
	if (ep->verbs_ep.link_layer != IBV_LINK_LAYER_ETHERNET) {
		// OPA or IB
			// NIC/HCA/HFI will only look at low "LMC" worth of bits
		ah_attr->src_path_bits = __be16_to_cpu(path_rec->pr_slid);
		ah_attr->dlid = __be16_to_cpu(path_rec->pr_dlid);
		ah_attr->is_global  = 0;
		_HFI_UDDBG("creating AH with DLID %u\n", ah_attr->dlid);
	} else {
		ah_attr->src_path_bits = 0;
		ah_attr->dlid = 1;	// not used on ethernet, make non-zero
		ah_attr->is_global  = 1;
		ah_attr->grh.dgid = ep->verbs_ep.lgid;
		ah_attr->grh.dgid.raw[12] =  (uint8_t)(__be16_to_cpu(path_rec->pr_ip_hi)>>8);
		ah_attr->grh.dgid.raw[13] =  (uint8_t)(__be16_to_cpu(path_rec->pr_ip_hi));
		ah_attr->grh.dgid.raw[14] =  (uint8_t)(__be16_to_cpu(path_rec->pr_dlid)>>8);
		ah_attr->grh.dgid.raw[15] =  (uint8_t)(__be16_to_cpu(path_rec->pr_dlid));
		ah_attr->grh.sgid_index = ep->verbs_ep.lgid_index;
		ah_attr->grh.hop_limit = 0xFF;
		ah_attr->grh.traffic_class = 0;
		if (_HFI_UDDBG_ON) {
			char buf[80];
			_HFI_UDDBG("creating AH with DGID: %s\n",
				__psm2_dump_gid(&ah_attr->grh.dgid, buf, sizeof(buf)));
		}
	}
	return PSM2_OK;
}

psm2_error_t ips_make_ah(psm2_ep_t ep, ips_path_rec_t *path_rec)
{
	struct ibv_ah_attr ah_attr;

	if (path_rec->ah) {
		_HFI_UDDBG("make_ah called second time on given path_rec, skipping\n");
		return PSM2_OK;
	}
	if (PSM2_OK != ips_path_rec_to_ah_attr(ep, path_rec, &ah_attr)) {
		_HFI_ERROR( "Unable to convert path_rec to AH\n");
		return PSM2_INTERNAL_ERR;
	}
	path_rec->ah = ibv_create_ah(ep->verbs_ep.pd, &ah_attr);
	if (! path_rec->ah) {
		int save_errno = errno;
		_HFI_ERROR( "Unable to create AH: %s (%d)\n", strerror(save_errno), save_errno);
		if (save_errno == ETIMEDOUT)
			return PSM2_EPID_PATH_RESOLUTION;
		else
			return PSM2_INTERNAL_ERR;
	}
	_HFI_UDDBG("created AH %p\n", path_rec->ah);
	// PSM doesn't free path_rec structures on shutdown, so this will
	// simply leak and be cleaned up by the kernel close when we shutdown
	return PSM2_OK;
}

#ifdef RNDV_MOD
void ips_path_rec_to_ib_user_path_rec(psm2_ep_t ep,
		const ips_path_rec_t *path_rec, union ibv_gid *dgid,
		struct ib_user_path_rec *path)
{
	memset(path, 0, sizeof(*path));
	memcpy(&path->sgid, &ep->verbs_ep.lgid, sizeof(path->sgid));
	memcpy(&path->dgid, dgid, sizeof(path->dgid));
	path->slid = path_rec->pr_slid; /* __be16 */
	if (ep->verbs_ep.link_layer != IBV_LINK_LAYER_ETHERNET)
		path->dlid = path_rec->pr_dlid; /* __be16 */
	else
		path->dlid = __cpu_to_be16(1);
	//path->raw_traffic
	//path->flow_label
	path->reversible = 1;
	path->mtu = opa_mtu_int_to_enum(path_rec->pr_mtu);
	path->pkey = __cpu_to_be16(path_rec->pr_pkey); /* __be16 */
	path->hop_limit = (ep->verbs_ep.link_layer == IBV_LINK_LAYER_ETHERNET)
						?0xFF:0;	// indicates if need GRH
	//path->traffic_class
	path->numb_path = 1;
	path->sl = path_rec->pr_sl;
	path->mtu_selector = 2;  /* Exactly the given MTU */
	path->rate_selector = 2; /* Exactly the given rate */
	// ips_path_rec.pr_static_rate is negotiated in PSM REQ/REP
	// then also use negotiated rate in user RC QP, ah_attr above and here
	path->rate = path_rec->pr_static_rate;
	path->packet_life_time_selector = 2; /* Exactly the given LT */
	// the value supplied here will be increased by the CM based on ack_delay
	// typically ack_delay will be small compared to packet_life_time
	// in which case the CM wil end up using packet_life_time+1 as the timeout
	// so we pass timeout-1 here so final timeout is usually what was requested
	path->packet_life_time = ep->hfi_qp_timeout - 1;
	//path->preferences
}
#endif // RNDV_MOD

static psm2_error_t
ips_none_path_rec(struct ips_proto *proto,
		  uint16_t slid, uint16_t dlid,
		  uint16_t ip_hi,
		  unsigned long timeout, ips_path_grp_t **ppathgrp)
{
	psm2_error_t err = PSM2_OK;
	uint16_t pidx, num_path = (1 << proto->epinfo.ep_lmc);
	uint16_t path_slid, path_dlid;
	ips_path_rec_t *path;
	ips_path_grp_t *pathgrp;
	ENTRY elid, *epath = NULL;
	char eplid[128];

	num_path = 1;	// don't yet have multi-path dispersive routing
					// maybe we use env to derrive multiple sequential IP
					// addresses, sort of like an LMC concept
					// or use ECMP or other mechanism

	/* For the "none" path record resolution all paths are assumed to be
	 * of equal priority however since we want to isolate all control
	 * traffic (acks, naks) to a separate path for non zero LMC subnets
	 * the "first path" between a pair of endpoints is always the "higher"
	 * priority paths. The rest of the paths are the normal (and low
	 * priority) paths.
	 */

	/* Query the path record cache */
	// TBD - slid same until have dispersive LMC-like, could just use dest
	snprintf(eplid, sizeof(eplid), "%x_%x%04x", slid, ip_hi, dlid);
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
	} else {
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
					  ip_hi,
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
		} else {
			pathgrp->pg_path[0][IPS_PATH_HIGH_PRIORITY] = path;
			pathgrp->pg_path[0][IPS_PATH_NORMAL_PRIORITY] = path;
			pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY] = path;
		}
                PSM2_LOG_MSG("path %p slid %hu dlid %hu ip_hi %hu\n",
                              path,
			      __be16_to_cpu(path->pr_slid),
			      __be16_to_cpu(path->pr_dlid),
			      __be16_to_cpu(path->pr_ip_hi));

	}

	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE) {
		pathgrp->pg_next_path[IPS_PATH_NORMAL_PRIORITY] =
		    proto->epinfo.EP_HASH %
		    pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY];
		pathgrp->pg_next_path[IPS_PATH_LOW_PRIORITY] =
		    proto->epinfo.EP_HASH %
		    pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY];
	}

	/* Add path record into cache */
	strcpy(elid.key, eplid);
	elid.data = (void *)pathgrp;
	hsearch_r(elid, ENTER, &epath, &proto->ips_path_grp_hash);

	*ppathgrp = pathgrp;

fail:
	if (err != PSM2_OK)
		_HFI_PRDBG
		    ("Unable to get path record for LID %x <---> DLID %x.\n",
		     slid, dlid);
	return err;
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
		char *errchk_to = PSM_TID_TIMEOUT_DEFAULT;
		int tvals[3] = {
			IPS_PROTO_ERRCHK_MS_MIN_DEFAULT,
			IPS_PROTO_ERRCHK_MS_MAX_DEFAULT,
			IPS_PROTO_ERRCHK_FACTOR_DEFAULT
		};

		if (!psmi_getenv("PSM3_ERRCHK_TIMEOUT",
				 "Errchk timeouts in mS <min:max:factor>",
				 PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
				 (union psmi_envvar_val)errchk_to, &env_to)) {
			/* Not using default values, parse what we can */
			errchk_to = env_to.e_str;
			psmi_parse_str_tuples(errchk_to, 3, tvals);
			/* Adjust for max smaller than min, things would break */
			if (tvals[1] < tvals[0])
				tvals[1] = tvals[0];
		}

		proto->epinfo.ep_timeout_ack = ms_2_cycles(tvals[0]);
		proto->epinfo.ep_timeout_ack_max = ms_2_cycles(tvals[1]);
		proto->epinfo.ep_timeout_ack_factor = tvals[2];
	}

	proto->ibta.get_path_rec = ips_none_path_rec;
	proto->ibta.fini = NULL;


	return err;
}


/* On link up/down we need to update some state */
psm2_error_t ips_ibta_link_updown_event(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	/* Get base lid, lmc and rate as these may have changed if the link bounced */
	proto->epinfo.ep_base_lid =
	    __cpu_to_be16((uint16_t) psm2_epid_nid(proto->ep->context.epid));

	proto->epinfo.ep_lmc = 0; // No LMC for UD
	proto->epinfo.ep_link_rate = proto->ep->verbs_ep.active_rate;
	return err;
}

psm2_error_t
MOCKABLE(ips_ibta_init)(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;
	union psmi_envvar_val path_disable_lmc_interval;

	proto->flags |= IPS_PROTO_FLAG_PPOLICY_ADAPTIVE;

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
		psmi_getenv("PSM3_PATH_NO_LMC_RANGE",
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

	/* On startup treat it as a link up/down event to setup state . */
	if ((err = ips_ibta_link_updown_event(proto)) != PSM2_OK)
		goto fail;

	/* Setup the appropriate query interface for the endpoint */
	switch (proto->ep->path_res_type) {
	case PSM2_PATH_RES_OPP:
		err = ips_opp_init(proto);
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
MOCK_DEF_EPILOGUE(ips_ibta_init);

psm2_error_t ips_ibta_fini(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	if (proto->ibta.fini)
		err = proto->ibta.fini(proto);

	/* Destroy the path record/group hash */
	hdestroy_r(&proto->ips_path_rec_hash);
	hdestroy_r(&proto->ips_path_grp_hash);

	return err;
}
