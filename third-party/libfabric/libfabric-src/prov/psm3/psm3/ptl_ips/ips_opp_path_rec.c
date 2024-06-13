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
#include <dlfcn.h>

/* These functions interact with the OPP (Ofed Plus Plus) SA replica to
 * scalably query IBTA Path Records for IB verbs and native OPA
 * This is N/A to RoCE and UDP/TCP
 */

static psm2_error_t
ips_opp_get_path_rec(ips_path_type_t type, struct ips_proto *proto,
		     __be16 slid, __be16 dlid,
		     ips_path_rec_t **ppath_rec)
{
	psm2_error_t err = PSM2_OK;
	ibta_path_rec_t query, opp_response;
#ifdef _HFI_DEBUGGING
	int opp_response_set = 0;
#endif
	ips_path_rec_t *path_rec;
	int opp_err;
	ENTRY elid = {}, *epath = NULL;
	char eplid[128];
	uint64_t timeout_ack_ms;

	/* Query path record query cache first */
	bzero(&query, sizeof(query));
	bzero(eplid, sizeof(eplid));

	/* Bulk service ID is control service id + 1 */
	switch (type) {
	case IPS_PATH_LOW_PRIORITY:
		query.service_id =
		    __cpu_to_be64(proto->ep->service_id + DATA_VFABRIC_OFFSET);
		break;
	case IPS_PATH_NORMAL_PRIORITY:
	case IPS_PATH_HIGH_PRIORITY:
	default:
		query.service_id = __cpu_to_be64(proto->ep->service_id);
	}

	query.slid = slid;
	query.dlid = dlid;

	/* the eplid is simply an exact match search key, we don't worry
	 * about slid, dlid and gid being big endian.  In fact on little
	 * endian CPU, this will put low bits earlier in string and cause
	 * quicker discovery of differences when doing strcmp to sort/search
	 */
	snprintf(eplid, sizeof(eplid), "%s_%x_%x",
		 (type == IPS_PATH_LOW_PRIORITY) ? "LOW" : "HIGH",
		 query.slid, query.dlid);
	elid.key = eplid;
	hsearch_r(elid, FIND, &epath, &proto->ips_path_rec_hash);

	if (!epath) {		/* Unable to find path record in cache */
		elid.key =
		    psmi_calloc(proto->ep, UNDEFINED, 1, strlen(eplid) + 1);
		path_rec = (ips_path_rec_t *)
		    psmi_calloc(proto->ep, UNDEFINED, 1,
				sizeof(ips_path_rec_t));
		if (!elid.key || !path_rec) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}

		/* Get path record between local LID and remote */
		opp_err =
		    proto->opp_fn.op_path_get_path_by_rec(proto->opp_ctxt,
							  &query,
							  &opp_response);
		if (opp_err) {
			err = PSM2_EPID_PATH_RESOLUTION;
			goto fail;
		}
#ifdef _HFI_DEBUGGING
		opp_response_set = 1;
#endif
		/* Create path record */
		path_rec->pr_slid = opp_response.slid;
		path_rec->pr_dlid = opp_response.dlid;
			/* HAL may recompute pr_mtu */
		path_rec->pr_mtu =
		    min(opa_mtu_enum_to_int(opp_response.mtu & 0x3f),
			proto->epinfo.ep_mtu);
		path_rec->pr_pkey = ntohs(opp_response.pkey);
		path_rec->pr_sl = ntohs(opp_response.qos_class_sl);
		path_rec->pr_static_rate = opp_response.rate & 0x3f;
		/* this function is N/A to RoCE.
		 * We don't support routing for IB/OPA so set gid to 0
		 * so verbs HAL can easily distinguish RoCE (gid!=0) vs IB/OPA.
		 */
		path_rec->pr_gid_hi = 0;
		path_rec->pr_gid_lo = 0;

		if (path_rec->pr_sl > PSMI_SL_MAX) {
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}

		/* Compute max timeout based on pkt life time for path */
		timeout_ack_ms =
		    ((4096UL * (1UL << (opp_response.pkt_life & 0x3f))) /
		     1000000UL);
		timeout_ack_ms =
		    ms_2_cycles(IPS_PROTO_ERRCHK_MS_MIN_DEFAULT +
				timeout_ack_ms);
		if (proto->epinfo.ep_timeout_ack_max < timeout_ack_ms)
			proto->epinfo.ep_timeout_ack_max = timeout_ack_ms;

		err = psmi_hal_ips_path_rec_init(proto, path_rec, &opp_response);
		if (err != PSM2_OK)
			goto fail;

		/* Add path record into cache */
		strcpy(elid.key, eplid);
		elid.data = (void *)path_rec;
		hsearch_r(elid, ENTER, &epath, &proto->ips_path_rec_hash);
	} else			/* Path record found in cache */
		path_rec = (ips_path_rec_t *) epath->data;

#ifdef _HFI_DEBUGGING
	/* Dump path record stats */
	_HFI_CONNDBG("Path Record ServiceID: %" PRIx64 " %x -----> %x\n",
		   (uint64_t) __be64_to_cpu(query.service_id),
		   __be16_to_cpu(slid), __be16_to_cpu(dlid));
	if (opp_response_set)
	{
		_HFI_CONNDBG("MTU: %x, %x\n", (opp_response.mtu & 0x3f),
			   path_rec->pr_mtu);
		_HFI_CONNDBG("PKEY: 0x%04x\n", ntohs(opp_response.pkey));
		_HFI_CONNDBG("SL: 0x%04x\n", ntohs(opp_response.qos_class_sl));
		_HFI_CONNDBG("Rate: %x\n", (opp_response.rate & 0x3f));
	}
	_HFI_CONNDBG("Timeout Init.: 0x%" PRIx64 " Max: 0x%" PRIx64 "\n",
		   proto->epinfo.ep_timeout_ack,
		   proto->epinfo.ep_timeout_ack_max);
#endif
	/* Return the IPS path record */
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
ips_opp_path_rec(struct ips_proto *proto,
		 __be16 slid, __be16 dlid,
		 __be64 gid_hi, __be64 gid_lo,// unused here, but must match API signature
		 unsigned long timeout, ips_path_grp_t **ppathgrp)
{
	psm2_error_t err = PSM2_OK;
	uint16_t pidx, cpath, num_path = (1 << proto->epinfo.ep_lmc);
	ips_path_type_t path_type = IPS_PATH_NORMAL_PRIORITY;
	ips_path_rec_t *path;
	ips_path_grp_t *pathgrp;
	uint16_t path_slid, path_dlid;
	ENTRY elid = {}, *epath = NULL;
	char eplid[128];

	/*
	 * High Priority Path
	 * ------------------
	 *
	 * Uses the "base" Service ID. For now there exists only 1 high priority
	 * path between nodes even for non zero LMC fabrics.
	 *
	 * Normal/Low Priority Paths
	 * -------------------------
	 *
	 * Currently these paths are the same i.e. they are queried for the same
	 * Service ID/vFabric which is the Base Service ID for High Priority + 1.
	 *
	 * Use case Scenarios
	 * ------------------
	 *
	 * Since with vFabrics we have the capability to define different QoS
	 * parameters per vFabric it is envisioned that the IPS_PATH_HIGH_PRIORITY is
	 * setup in a separate vFabric for high priority traffic. The NORMAL paths
	 * are setup in a separate vFabric optimized for high bandwidth. This allows
	 * us to potentially have control traffic (RTS, CTS etc.) not be bottlenecked
	 * by bulk transfer data. All control messages (ACKs,NAKs, TID_GRANT etc.)
	 * also use the high priority control vFabric.
	 *
	 * NOTE: In order to distinguish between the different vFabrics the user
	 * specifies the service ID to use via mpirun (or environment variable).
	 * This is the service ID for the high priority control traffic. The bulk
	 * data vFabric is identified by service ID + 1. So for each MPI application
	 * one should specify two service IDs for the high priority and bulk data.
	 * Both these service IDs can be placed in the same vFabric which can be
	 * configured for high priority or bandwidth traffic giving us the default
	 * behavior upto Infinhfi 2.5 release.
	 *
	 * NOTE: All of the above would have really helped if the S20 silicon could
	 * correctly support IBTA QoS features. Due to S20 design we can only have
	 * high priority VLarb table (low priority VLarb table results in round
	 * robin arbitration ignoring the weights!). But if this is fixed in a
	 * subsequent chip respin then this may potentially help our scalability
	 * on large fabrics.
	 *
	 * Mesh/Torus and DOR routed networks
	 * ----------------------------------
	 *
	 * In a mesh/torus fabric we always have a non zero LMC (at least 1 can be
	 * more). We would like to take advantage of dispersive routing on these
	 * fabrics as well to obtain better "worst case/congested" bandwidth. For
	 * these networks currently the base LIDs are used for UPDN routing which
	 * is suboptimal on these networks. Higher order LIDs (+1 .. +N) use DOR
	 * routing (Dimension Ordered Routing) to avoid deadlocks and provide
	 * higher performance. If a fabric is disrupted then only the base UPDN
	 * routing is available. PSM should continue to operate in this environment
	 * albeit with degraded performance. In disrupted fabric the OPP path
	 * record queries may fail for some DOR routed LIDs i.e. no path exists
	 * PSM should hence ignore path record failures as they indicate a disrupted
	 * fabric and only use valid paths that are returned from the replica. This
	 * will degenerate to only using the UPDN paths on disrupted fabrics and DOR
	 * routes only for fully configured fabrics. Note: For a clean fabric the
	 * base LIDs that are configured for UPDN route will not exist in the replica
	 * as DOR routes are preferred. Hence we will only dispersively route across
	 * the DOR routes only using the UPDN route for disrupted fabrics.
	 *
	 * AS LONG AS ONE PATH EXISTS (for each of the priorities) COMMUNICATION CAN
	 * TAKE PLACE.
	 */

	/* Check if this path grp is already in hash table */
	/* the eplid is simply an exact match search key, we don't worry
	 * about slid, dlid and gid being big endian.  In fact on little
	 * endian CPU, this will put low bits earlier in string and cause
	 * quicker discovery of differences when doing strcmp to sort/search
	 */
	snprintf(eplid, sizeof(eplid), "%x_%x", slid, dlid);
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
	 * Store here in network byte order.
	 */
	pathgrp->pg_base_dlid = dlid;
	pathgrp->pg_base_slid = slid;

	pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY] =
	    pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY] =
	    pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY] = 0;

	/* For now there is always only one high priority path between nodes. */
	for (pidx = 0, cpath = 0; pidx < num_path && cpath == 0; pidx++) {
		path_slid = __cpu_to_be16(__be16_to_cpu(slid) + pidx);
		path_dlid = __cpu_to_be16(__be16_to_cpu(dlid) + pidx);

		err = ips_opp_get_path_rec(IPS_PATH_HIGH_PRIORITY, proto,
					   path_slid, path_dlid,
					   &path);

		if (err == PSM2_OK) {	/* Valid high priority path found */
			/* Resolved high priority path successfully */
			pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY]++;
			pathgrp->pg_path[cpath][IPS_PATH_HIGH_PRIORITY] = path;

			/* Increment current path index */
			cpath++;
		}

		PSM2_LOG_MSG("path %p slid %hu dlid %hu\n",
			      path,
			      __be16_to_cpu(path->pr_slid),
			      __be16_to_cpu(path->pr_dlid));
	}

	/* Make sure we have atleast 1 high priority path */
	if (pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY] == 0) {
		psmi_free(elid.key);
		psmi_free(pathgrp);
		err = psm3_handle_error(NULL, PSM2_EPID_PATH_RESOLUTION,
					"OFED Plus path lookup failed. Unable to resolve high priority network path for LID 0x%x <---> 0x%x. Is the SM running or service ID %"
					PRIx64 " defined?", ntohs(slid),
					ntohs(dlid),
					(uint64_t) proto->ep->service_id);
		goto fail;
	}



	/* Next setup the bulk paths. If the subnet administrator has misconfigured
	 * or rather not configured two separate service IDs we place the bulk
	 * paths in the same vFabric as the control paths.
	 */

	path_type = IPS_PATH_NORMAL_PRIORITY;
	for (pidx = 0, cpath = 0; pidx < num_path; pidx++) {
		path_slid = __cpu_to_be16(__be16_to_cpu(slid) + pidx);
		path_dlid = __cpu_to_be16(__be16_to_cpu(dlid) + pidx);

retry_normal_path_res:
		err = ips_opp_get_path_rec(path_type, proto,
					   path_slid, path_dlid,
					   &path);
		if (err != PSM2_OK) {
			if (path_type == IPS_PATH_NORMAL_PRIORITY) {
				/* Subnet may only be configured for one service ID/vFabric. Default
				 * to using the control vFabric/service ID for bulk data as well.
				 */
				path_type = IPS_PATH_HIGH_PRIORITY;
				goto retry_normal_path_res;
			}

			/* Unable to resolve path for <path_slid, path_dline>. This is possible
			 * for disrupted fabrics using DOR routing so continue to acquire paths
			 */
			err = PSM2_OK;
			continue;
		}

		/* Valid path. */
		pathgrp->pg_path[cpath][IPS_PATH_NORMAL_PRIORITY] = path;
		pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY]++;
		cpath++;
	}

	/* Make sure we have at least have a single bulk data transfer path */
	if (pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY] == 0) {
		psmi_free(elid.key);
		psmi_free(pathgrp);
		err = psm3_handle_error(NULL, PSM2_EPID_PATH_RESOLUTION,
					"OFED Plus path lookup failed. Unable to resolve normal priority network path for LID 0x%x <---> 0x%x. Is the SM running or service ID %"
					PRIx64 " defined?", ntohs(slid),
					ntohs(dlid),
					(uint64_t) proto->ep->service_id);
		goto fail;
	}

	path_type = IPS_PATH_LOW_PRIORITY;
	for (pidx = 0, cpath = 0; pidx < num_path; pidx++) {
		path_slid = __cpu_to_be16(__be16_to_cpu(slid) + pidx);
		path_dlid = __cpu_to_be16(__be16_to_cpu(dlid) + pidx);

retry_low_path_res:
		err = ips_opp_get_path_rec(path_type, proto,
					   path_slid, path_dlid,
					   &path);
		if (err != PSM2_OK) {
			if (path_type == IPS_PATH_LOW_PRIORITY) {
				/* Subnet may only be configured for one service ID/vFabric. Default
				 * to using the control vFabric/service ID for bulk data as well.
				 */
				path_type = IPS_PATH_HIGH_PRIORITY;
				goto retry_low_path_res;
			}

			/* Unable to resolve path for <path_slid, path_dline>. This is possible
			 * for disrupted fabrics using DOR routing so continue to acquire paths
			 */
			err = PSM2_OK;
			continue;
		}

		/* Valid path. */
		pathgrp->pg_path[cpath][IPS_PATH_LOW_PRIORITY] = path;
		pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY]++;
		cpath++;
	}

	/* Make sure we have at least have a single bulk data transfer path */
	if (pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY] == 0) {
		psmi_free(elid.key);
		psmi_free(pathgrp);
		err = psm3_handle_error(NULL, PSM2_EPID_PATH_RESOLUTION,
					"OFED Plus path lookup failed. Unable to resolve low priority network path for LID 0x%x <---> 0x%x. Is the SM running or service ID %"
					PRIx64 " defined?", ntohs(slid),
					ntohs(dlid),
					(uint64_t) proto->ep->service_id);
		goto fail;
	}

	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE) {
		pathgrp->pg_next_path[IPS_PATH_NORMAL_PRIORITY] =
		    proto->epinfo.ep_hash %
		    pathgrp->pg_num_paths[IPS_PATH_NORMAL_PRIORITY];
		pathgrp->pg_next_path[IPS_PATH_LOW_PRIORITY] =
		    proto->epinfo.ep_hash %
		    pathgrp->pg_num_paths[IPS_PATH_LOW_PRIORITY];
	}

	/* Add path group into cache */
	strcpy(elid.key, eplid);
	elid.data = (void *)pathgrp;
	hsearch_r(elid, ENTER, &epath, &proto->ips_path_grp_hash);

	*ppathgrp = pathgrp;

fail:
	if (err != PSM2_OK)
		_HFI_CONNDBG
		    ("Unable to get path record for LID 0x%x <---> DLID 0x%x.\n",
		     slid, dlid);
	return err;
}

static psm2_error_t ips_opp_fini(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	if (proto->opp_lib)
		dlclose(proto->opp_lib);

	return err;
}

psm2_error_t psm3_ips_opp_init(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;

	proto->opp_lib = dlopen(DF_OPP_LIBRARY, RTLD_NOW);
	if (!proto->opp_lib) {
		char *err = dlerror();
		_HFI_ERROR
		    ("Unable to open OFED Plus Plus library %s. Error: %s\n",
		     DF_OPP_LIBRARY, err ? err : "no dlerror()");
		goto fail;
	}

	/* Resolve symbols that we require within opp library */
	proto->opp_fn.op_path_find_hca =
	    dlsym(proto->opp_lib, "op_path_find_hfi");
	proto->opp_fn.op_path_open = dlsym(proto->opp_lib, "op_path_open");
	proto->opp_fn.op_path_close = dlsym(proto->opp_lib, "op_path_close");
	proto->opp_fn.op_path_get_path_by_rec =
	    dlsym(proto->opp_lib, "op_path_get_path_by_rec");

	/* If we can't resovle any symbol then fail to load opp module */
	if (!proto->opp_fn.op_path_find_hca || !proto->opp_fn.op_path_open ||
	    !proto->opp_fn.op_path_close
	    || !proto->opp_fn.op_path_get_path_by_rec) {
		_HFI_ERROR
		    ("Unable to resolve symbols in OPP library. Unloading.\n");
		goto fail;
	}

	/* If PSM3_IDENTIFY is set display the OPP library location being used. */
	{
		Dl_info info_opp;
		psm3_print_identify
		    ("%s %s PSM3 path record queries using OFED Plus Plus (%s) from %s\n",
		     psm3_get_mylabel(), psm3_ident_tag,
		     DF_OPP_LIBRARY, dladdr(proto->opp_fn.op_path_open,
					    &info_opp) ? info_opp.
		     dli_fname :
		     "Unknown/unsupported version of OPP library found!");
	}

	/* Obtain handle to hfi (requires verbs on node) */
	proto->hndl = proto->opp_fn.op_path_find_hca(proto->ep->dev_name, &proto->device);
	if (!proto->hndl) {
		_HFI_ERROR
		    ("OPP: Unable to find NIC %s. Disabling OPP interface for path record queries.\n",
		     proto->ep->dev_name);
		goto fail;
	}

	/* Get OPP context */
	proto->opp_ctxt = proto->opp_fn.op_path_open(proto->device, 1);
	if (!proto->opp_ctxt) {
		_HFI_ERROR
		    ("OPP: Unable to obtain OPP context. Disabling OPP interface for path record queries.\n");
		goto fail;
	}

	/* Setup default errorcheck timeout. OPP may change it later. */
	proto->epinfo.ep_timeout_ack =
	    ms_2_cycles(IPS_PROTO_ERRCHK_MS_MIN_DEFAULT);
	proto->epinfo.ep_timeout_ack_max =
	    ms_2_cycles(IPS_PROTO_ERRCHK_MS_MIN_DEFAULT);
	proto->epinfo.ep_timeout_ack_factor = IPS_PROTO_ERRCHK_FACTOR_DEFAULT;

	/* OPP initialized successfully */
	proto->ibta.get_path_rec = ips_opp_path_rec;
	proto->ibta.fini = ips_opp_fini;
	proto->flags |= IPS_PROTO_FLAG_QUERY_PATH_REC;

	return err;

fail:
	_HFI_ERROR("Make sure SM is running...\n");
	_HFI_ERROR("Make sure service ibacm is running...\n");
	_HFI_ERROR("to start ibacm: service ibacm start\n");
	_HFI_ERROR("or enable it at boot time: iefsconfig -E ibacm\n\n");

	err = psm3_handle_error(NULL, PSM2_EPID_PATH_RESOLUTION,
				"Unable to initialize OFED Plus library successfully.\n");

	if (proto->opp_lib)
		dlclose(proto->opp_lib);

	return err;
}
