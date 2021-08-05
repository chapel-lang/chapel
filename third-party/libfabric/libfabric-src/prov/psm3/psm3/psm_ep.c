/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <stdbool.h>

#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#include "ips_proto_params.h"

#ifdef PSM_CUDA
#include "psm_gdrcpy.h"
#endif
/*
 * Endpoint management
 */
psm2_ep_t psmi_opened_endpoint = NULL;
int psmi_opened_endpoint_count = 0;
static uint32_t *hfi_lids;
static uint32_t nlids;

static psm2_error_t psmi_ep_open_device(const psm2_ep_t ep,
				       const struct psm2_ep_open_opts *opts,
				       const psm2_uuid_t unique_job_key,
				       struct psmi_context *context,
				       psm2_epid_t *epid);

/*
 * Device management
 *
 * PSM uses "devices" as components to manage communication to self, to peers
 * reachable via shared memory and finally to peers reachable only through
 * hfi.
 */

static psm2_error_t psmi_parse_devices(int devices[PTL_MAX_INIT],
				      const char *devstr);
static int psmi_device_is_enabled(const int devices[PTL_MAX_INIT], int devid);
int psmi_ep_device_is_enabled(const psm2_ep_t ep, int devid);

psm2_error_t __psm2_ep_num_devunits(uint32_t *num_units_o)
{
	static int num_units = -1;

	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (num_units == -1) {
		num_units = psmi_hal_get_num_units();
		if (num_units == -1)
			num_units = 0;
	}

	*num_units_o = (uint32_t) num_units;
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}
PSMI_API_DECL(psm2_ep_num_devunits)

static int cmpfunc(const void *p1, const void *p2)
{
	uint64_t a = ((uint64_t *) p1)[0];
	uint64_t b = ((uint64_t *) p2)[0];
	if (a < b)
		return -1;
	if (a == b)
		return 0;
	return 1;
}

// process PSM3_MULTIRAIL and PSM3_MULTIRAIL_MAP and return the
// list of unit/port in unit[0-*num_rails] and port[0-*num_rails]
// When *num_rails is returned as 0, multirail is not enabled and
// other mechanisms (PSM3_NIC, PSM3_NIC_SELECTION_ALG) must be
// used by the caller to select a single NIC for the process
static psm2_error_t
psmi_ep_multirail(int *num_rails, uint32_t *unit, uint16_t *port)
{
	uint32_t num_units;
	uint64_t gid_hi;
	unsigned i, j, count = 0;
	int ret;
	psm2_error_t err = PSM2_OK;
	uint64_t gidh[PSMI_MAX_RAILS][3];
	union psmi_envvar_val env_multirail;
	union psmi_envvar_val env_multirail_map;
	int multirail_within_socket_used = 0;
	int node_id = -1, found = 0;

	psmi_getenv("PSM3_MULTIRAIL",
			"Use all available NICs in the system for communication.\n"
			 "0: Disabled (default),\n"
			 "1: Enable multirail across all available NICs,\n"
			 "2: Enable multirail within socket.\n"
			 "\t For multirail within a socket, we try to find at\n"
			 "\t least one NIC on the same socket as current task.\n"
			 "\t If none found, we continue to use other NICs within\n"
			 "\t the system.",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0,
			&env_multirail);
	if (!env_multirail.e_int) {
		*num_rails = 0;
		return PSM2_OK;
	}

	if (env_multirail.e_int == 2)
		multirail_within_socket_used = 1;

/*
 * map is in format: unit:port,unit:port,...
 * where :port is optional (default of 1) and unit can be name or number
 */
#define MAX_MAP_LEN (PSMI_MAX_RAILS*128)
	if (!psmi_getenv("PSM3_MULTIRAIL_MAP",
		"NIC selections for each rail in format:\n"
		"     rail,rail,...\n"
		"Where rail can be: unit:port or unit\n"
		"When port is omitted, it defaults to 1\n"
		"unit can be device name or unit number\n"
		"default autoselects",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"", &env_multirail_map)) {

		char temp[MAX_MAP_LEN+1];
		char *s;
		char *delim;

		strncpy(temp, env_multirail_map.e_str, MAX_MAP_LEN);
		if (temp[MAX_MAP_LEN-1] != 0)
			return psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3_MULTIRAIL_MAP too long: '%s'",
					env_multirail_map.e_str);
		s = temp;
		psmi_assert(*s);
		do {
			int u, p;
			int skip_port = 0;

			if (! *s)	// trailing ',' on 2nd or later loop
				break;
			if (count >= PSMI_MAX_RAILS)
				return psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP exceeds %u rails: '%s'",
						PSMI_MAX_RAILS, env_multirail_map.e_str);

			// parse unit
			delim = strchr(s, ':');
			if (! delim) {
				delim = strchr(s, ',');
				skip_port = 1;
				p = 1;
			}
			if (! delim && !skip_port)
				return psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP invalid format: '%s'",
						env_multirail_map.e_str);
			if (delim)
				*delim = '\0';
			u = sysfs_find_unit(s);
			if (u < 0)
				return psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP invalid unit: '%s'", s);
			if (delim)
				s = delim+1;

			// optionally parse port
			if (! skip_port) {
				delim = strchr(s, ',');
				if (delim)
					*delim = '\0';
				p = psmi_parse_str_long(s);
				if (p < 0)
					return psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP invalid port: '%s'", s);
				if (delim)
					s = delim+1;
			}

			unit[count] = u;
			port[count] = p;
			count++;
		} while (delim);
		*num_rails = count;

/*
 * Check if any of the port is not usable.
 */
		for (i = 0; i < count; i++) {
			_HFI_VDBG("rail %d:  %u(%s) %u\n", i,
				unit[i], sysfs_unit_dev_name(unit[i]), port[i]);
			ret = psmi_hal_get_port_active(unit[i], port[i]);
			if (ret <= 0)
				return psmi_handle_error(NULL,
						PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP: Unit/port: %d(%s):%d is not active.",
						unit[i], sysfs_unit_dev_name(unit[i]),
						port[i]);
			ret = psmi_hal_get_port_lid(unit[i], port[i]);
			if (ret <= 0)
				return psmi_handle_error(NULL,
						PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP: Couldn't get lid for unit %d(%s):%d",
						unit[i], sysfs_unit_dev_name(unit[i]),
						port[i]);
			ret = psmi_hal_get_port_subnet(unit[i], port[i], NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			if (ret == -1)
				return psmi_handle_error(NULL,
						PSM2_EP_DEVICE_FAILURE,
						"PSM3_MULTIRAIL_MAP: Couldn't get subnet for unit %d(%s):%d",
						unit[i], sysfs_unit_dev_name(unit[i]),
						port[i]);
		}
		return PSM2_OK;
	}

	if ((err = psm2_ep_num_devunits(&num_units))) {
		return err;
	}
	if (num_units > PSMI_MAX_RAILS) {
		_HFI_INFO
		    ("Found %d units, max %d units are supported, use %d\n",
		     num_units, PSMI_MAX_RAILS, PSMI_MAX_RAILS);
		num_units = PSMI_MAX_RAILS;
	}

	/*
	 * PSM3_MULTIRAIL=2 functionality-
	 *   - Try to find at least find one HFI in the same root
	 *     complex. If none found, continue to run and
	 *     use remaining HFIs in the system.
	 *   - If we do find at least one HFI in same root complex, we
	 *     go ahead and add to list.
	 */
	if (multirail_within_socket_used) {
		node_id = psmi_get_current_proc_location();
		for (i = 0; i < num_units; i++) {
			if (psmi_hal_get_unit_active(i) <= 0)
				continue;
			int node_id_i;

			if (!psmi_hal_get_node_id(i, &node_id_i)) {
				if (node_id_i == node_id) {
					found = 1;
					break;
				}
			}
		}
	}
/*
 * Get all the ports with a valid lid and gid, one per unit.
 */
	for (i = 0; i < num_units; i++) {
		int node_id_i;

		if (!psmi_hal_get_node_id(i, &node_id_i))
		{
			if (multirail_within_socket_used &&
			    found && (node_id_i != node_id))
				continue;
		}

		for (j = HFI_MIN_PORT; j <= HFI_MAX_PORT; j++) {
			ret = psmi_hal_get_port_lid(i, j);
			if (ret <= 0)
				continue;
			ret = psmi_hal_get_port_subnet(i, j, &gid_hi, NULL, NULL, NULL, NULL, NULL, NULL);
			if (ret == -1)
				continue;

			gidh[count][0] = gid_hi;
			gidh[count][1] = i;
			gidh[count][2] = j;
			count++;
			break;
		}
	}

/*
 * Sort all the ports with gidh from small to big.
 * This is for multiple fabrics, and we use fabric with the
 * smallest gid to make the master connection.
 */
	qsort(gidh, count, sizeof(uint64_t) * 3, cmpfunc);

	for (i = 0; i < count; i++) {
		unit[i] = (uint32_t) gidh[i][1];
		port[i] = (uint16_t) (uint32_t) gidh[i][2];
	}
	*num_rails = count;
	return PSM2_OK;
}

// this is used to find devices with the same address as another process,
// implying intra-node comms.
#define MAX_GID_IDX 31
static psm2_error_t
psmi_ep_devlids(uint32_t **lids, uint32_t *num_lids_o,
		uint64_t my_gid_hi, uint64_t my_gid_lo, psm2_epid_t my_epid)
{
	uint32_t num_units;
	int i;
	psm2_error_t err = PSM2_OK;

	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (hfi_lids == NULL) {
		if ((err = psm2_ep_num_devunits(&num_units)))
			goto fail;
		hfi_lids = (uint32_t *)
		    psmi_calloc(PSMI_EP_NONE, UNDEFINED,
				num_units * psmi_hal_get_num_ports(), sizeof(*hfi_lids));
		if (hfi_lids == NULL) {
			err = psmi_handle_error(NULL, PSM2_NO_MEMORY,
						"Couldn't allocate memory for dev_lids structure");
			goto fail;
		}

		for (i = 0; i < num_units; i++) {
			int j;
			for (j = HFI_MIN_PORT; j <= HFI_MAX_PORT; j++) {
				int lid = psmi_hal_get_port_lid(i, j);
				int ret, idx = 0;
				uint64_t gid_hi = 0, gid_lo = 0;
				uint64_t actual_gid_hi = 0;
				uint32_t ipaddr = 0;

				// if looking for IB/OPA lid, skip ports we can't get lid for
				if (lid <= 0 && psmi_epid_version(my_epid) == PSMI_EPID_V3)
					continue;
				// we just need subnet and addr within subnet and idx
				ret = psmi_hal_get_port_subnet(i, j, &gid_hi, &gid_lo, &ipaddr, NULL, &idx, &actual_gid_hi, NULL);
				if (ret == -1)
					continue;
				if (my_gid_hi != gid_hi) {
					_HFI_VDBG("LID %d, unit %d, port %d, mismatched "
							  "GID[%d] %llx:%llx and %llx:%llx\n",
						lid, i, j, idx,
						(unsigned long long)gid_hi,
						(unsigned long long)gid_lo,
						(unsigned long long)my_gid_hi,
						(unsigned long long)my_gid_lo);
					continue;
				}
				if (actual_gid_hi != gid_hi) {
					if (_HFI_VDBG_ON) {
						char buf[INET_ADDRSTRLEN];
						_HFI_VDBG("LID %d=>IPaddr %s, unit %d, port %d, matched "
								  "GID[%d] %llx:%llx and %llx:%llx\n",
							lid, psmi_ipv4_ntop(ipaddr, buf, sizeof(buf)), i, j, idx,
							(unsigned long long)gid_hi,
							(unsigned long long)gid_lo,
							(unsigned long long)my_gid_hi,
							(unsigned long long)my_gid_lo);
					}

					hfi_lids[nlids++] = (uint32_t) ipaddr;
				} else {
					_HFI_VDBG("LID %d, unit %d, port %d, matched "
							  "GID[%d] %llx:%llx and %llx:%llx\n",
						lid, i, j, idx,
						(unsigned long long)gid_hi,
						(unsigned long long)gid_lo,
						(unsigned long long)my_gid_hi,
						(unsigned long long)my_gid_lo);

					hfi_lids[nlids++] = (uint16_t) lid;
				}
			}
		}
		if (nlids == 0) {
			err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"Couldn't get lid&gid from any unit/port");
			goto fail;
		}
	}
	*lids = hfi_lids;
	*num_lids_o = nlids;

fail:
	return err;
}

static psm2_error_t
psmi_ep_verify_pkey(psm2_ep_t ep, uint16_t pkey, uint16_t *opkey, uint16_t* oindex)
{
	int i, ret;
	psm2_error_t err;

	for (i = 0; i < 16; i++) {
// TBD - if we adjust HAL to take a hw_context for this function and
// put the verbs_ep inside the HAL hw context, we can eliminate this ifdef
// and simply call into HAL
		_HFI_UDDBG("looking for pkey 0x%x\n", pkey);
		ret = verbs_get_port_index2pkey(ep, ep->portnum, i);
		if (ret < 0) {
			err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"Can't get a valid pkey value from pkey table\n");
			return err;
		}
		// pkey == 0 means get slot 0
		if (! pkey && ! i)
			break;
		if ((pkey & 0x7fff) == (uint16_t)(ret & 0x7fff)) {
			break;
		}
	}

	/* if pkey does not match */
	if (i == 16) {
		err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Wrong pkey 0x%x, please use PSM3_PKEY to specify a valid pkey\n",
					pkey);
		return err;
	}

	if (((uint16_t)ret & 0x8000) == 0) {
		err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Limited Member pkey 0x%x, please use PSM3_PKEY to specify a valid pkey\n",
					(uint16_t)ret);
		return err;
	}

	/* return the final pkey */
	*opkey = (uint16_t)ret;
	*oindex = (uint16_t)i;

	return PSM2_OK;
}

uint64_t __psm2_epid_nid(psm2_epid_t epid)
{
	uint64_t rv;

	PSM2_LOG_MSG("entering");
	rv = (uint64_t) PSMI_EPID_GET_LID(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}
PSMI_API_DECL(psm2_epid_nid)


/* Currently not exposed to users, we don't acknowledge the existence of
 * service levels encoding within epids. This may require
 * changing to expose SLs
 */
uint64_t psmi_epid_version(psm2_epid_t epid)
{
	return (uint64_t) PSMI_EPID_GET_EPID_VERSION(epid);
}

uint64_t __psm2_epid_context(psm2_epid_t epid)
{
	uint64_t rv;

	PSM2_LOG_MSG("entering");
	rv = (uint64_t) PSMI_EPID_GET_CONTEXT(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}
PSMI_API_DECL(psm2_epid_context)

uint64_t __psm2_epid_port(psm2_epid_t epid)
{
	uint64_t rv;
	PSM2_LOG_MSG("entering");
	rv = __psm2_epid_context(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}
PSMI_API_DECL(psm2_epid_port)

psm2_error_t __psm2_ep_query(int *num_of_epinfo, psm2_epinfo_t *array_of_epinfo)
{
	psm2_error_t err = PSM2_OK;
	int i;
	psm2_ep_t ep;

	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (*num_of_epinfo <= 0) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid psm2_ep_query parameters");
		PSM2_LOG_MSG("leaving");
		return err;
	}

	if (psmi_opened_endpoint == NULL) {
		err = psmi_handle_error(NULL, PSM2_EP_WAS_CLOSED,
					"PSM Endpoint is closed or does not exist");
		PSM2_LOG_MSG("leaving");
		return err;
	}

	ep = psmi_opened_endpoint;
	for (i = 0; i < *num_of_epinfo; i++) {
		if (ep == NULL)
			break;
		array_of_epinfo[i].ep = ep;
		array_of_epinfo[i].epid = ep->epid;
		array_of_epinfo[i].jkey = ep->jkey;
		memcpy(array_of_epinfo[i].uuid,
		       (void *)ep->uuid, sizeof(psm2_uuid_t));
		psmi_uuid_unparse(ep->uuid, array_of_epinfo[i].uuid_str);
		ep = ep->user_ep_next;
	}
	*num_of_epinfo = i;
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_ep_query)

psm2_error_t __psm2_ep_epid_lookup(psm2_epid_t epid, psm2_epconn_t *epconn)
{
	psm2_error_t err = PSM2_OK;
	psm2_epaddr_t epaddr;
	psm2_ep_t ep;

	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	/* Need to have an opened endpoint before we can resolve epids */
	if (psmi_opened_endpoint == NULL) {
		err = psmi_handle_error(NULL, PSM2_EP_WAS_CLOSED,
					"PSM Endpoint is closed or does not exist");
		PSM2_LOG_MSG("leaving");
		return err;
	}

	ep = psmi_opened_endpoint;
	while (ep) {
		epaddr = psmi_epid_lookup(ep, epid);
		if (!epaddr) {
			ep = ep->user_ep_next;
			continue;
		}

		/* Found connection for epid. Return info about endpoint to caller. */
		psmi_assert_always(epaddr->ptlctl->ep == ep);
		epconn->addr = epaddr;
		epconn->ep = ep;
		epconn->mq = ep->mq;
		PSM2_LOG_MSG("leaving");
		return err;
	}

	err = psmi_handle_error(NULL, PSM2_EPID_UNKNOWN,
				"Endpoint connection status unknown");
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_ep_epid_lookup);

psm2_error_t __psm2_ep_epid_lookup2(psm2_ep_t ep, psm2_epid_t epid, psm2_epconn_t *epconn)
{
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	/* Need to have an opened endpoint before we can resolve epids */
	if (ep == NULL) {
		err = psmi_handle_error(NULL, PSM2_EP_WAS_CLOSED,
					"PSM Endpoint is closed or does not exist");
		PSM2_LOG_MSG("leaving");
		return err;
	}

	if (epconn == NULL) {
		err = psmi_handle_error(ep, PSM2_PARAM_ERR,
					"Invalid output parameter");
		PSM2_LOG_MSG("leaving");
		return err;
	}

	psm2_epaddr_t epaddr = psmi_epid_lookup(ep, epid);
	if (epaddr) {
		/* Found connection for epid. Return info about endpoint to caller. */
		psmi_assert_always(epaddr->ptlctl->ep == ep);
		epconn->addr = epaddr;
		epconn->ep = ep;
		epconn->mq = ep->mq;
		PSM2_LOG_MSG("leaving");
		return err;
	}

	err = psmi_handle_error(ep, PSM2_EPID_UNKNOWN,
				"Endpoint connection status unknown");
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_ep_epid_lookup2);

psm2_error_t __psm2_epaddr_to_epid(psm2_epaddr_t epaddr, psm2_epid_t *epid)
{
	psm2_error_t err = PSM2_OK;
	PSM2_LOG_MSG("entering");
	if (epaddr && epid) {
		*epid = epaddr->epid;
	}
	else {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid input epaddr or output epid parameter");
	}
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_epaddr_to_epid);

psm2_error_t
__psm2_ep_epid_share_memory(psm2_ep_t ep, psm2_epid_t epid, int *result_o)
{
	int result = 0;
	uint32_t num_lids = 0;
	uint32_t epid_lid;
	uint32_t *lids = NULL;
	int i;
	psm2_error_t err;

	PSM2_LOG_MSG("entering");
	psmi_assert_always(ep != NULL);
	PSMI_ERR_UNLESS_INITIALIZED(ep);

	if ((!psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS)) ||
		(psmi_epid_version(epid) == PSMI_EPID_VERSION_SHM)) {
		/* If we are in the no hfi-mode, or the other process is,
		 * the epid doesn't help us - so assume both we're on the same
		 * machine and try to connect.
		 */
		result = 1;
	} else {
		epid_lid = (uint32_t) psm2_epid_nid(epid);
		err = psmi_ep_devlids(&lids, &num_lids, ep->gid_hi, ep->gid_lo, ep->epid);
		if (err) {
			PSM2_LOG_MSG("leaving");
			return err;
		}
		for (i = 0; i < num_lids; i++) {
			if (epid_lid == lids[i]) {
				/* we share memory if the lid is the same. */
				result = 1;
				break;
			}
		}
	}
	*result_o = result;
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}
PSMI_API_DECL(psm2_ep_epid_share_memory)

psm2_error_t __psm2_ep_open_opts_get_defaults(struct psm2_ep_open_opts *opts)
{
	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (!opts)
		return PSM2_PARAM_ERR;

	/* Set in order in the structure. */
	opts->timeout = 30000000000LL;	/* 30 sec */
	opts->unit = PSM3_NIC_ANY;
	opts->affinity = PSM2_EP_OPEN_AFFINITY_SET;
	opts->shm_mbytes = 0;	/* deprecated in psm2.h */
	opts->sendbufs_num = 1024;
	opts->network_pkey = psmi_hal_get_default_pkey();
	opts->port = PSM3_NIC_PORT_ANY;
	opts->outsl = PSMI_SL_DEFAULT;
	opts->service_id = HFI_DEFAULT_SERVICE_ID;
	opts->path_res_type = PSM2_PATH_RES_NONE;
	opts->senddesc_num = 4096;
	opts->imm_size = VERBS_SEND_MAX_INLINE; // PSM header size is 56
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}
PSMI_API_DECL(psm2_ep_open_opts_get_defaults)

psm2_error_t psmi_poll_noop(ptl_t *ptl, int replyonly);

psm2_error_t
__psm2_ep_open_internal(psm2_uuid_t const unique_job_key, int *devid_enabled,
		       struct psm2_ep_open_opts const *opts_i, psm2_mq_t mq,
		       psm2_ep_t *epo, psm2_epid_t *epido)
{
	psm2_ep_t ep = NULL;
	uint32_t num_units;
	size_t len;
	psm2_error_t err;
	psm2_epaddr_t epaddr = NULL;
	char buf[128], *p;
	union psmi_envvar_val envvar_val;
	size_t ptl_sizes;
	struct psm2_ep_open_opts opts;
	ptl_t *amsh_ptl, *ips_ptl, *self_ptl;
	int i;

	/* First get the set of default options, we overwrite with the user's
	 * desired values afterwards */
	if ((err = psm2_ep_open_opts_get_defaults(&opts)))
		goto fail;

	if (opts_i != NULL) {
		if (opts_i->timeout != -1)
			opts.timeout = opts_i->timeout;
		if (opts_i->unit != -1)
			opts.unit = opts_i->unit;
		if (opts_i->affinity != -1)
			opts.affinity = opts_i->affinity;

		if (opts_i->sendbufs_num != -1)
			opts.sendbufs_num = opts_i->sendbufs_num;

		if (opts_i->network_pkey != psmi_hal_get_default_pkey())
			opts.network_pkey = opts_i->network_pkey;

		if (opts_i->port != 0)
			opts.port = opts_i->port;

		if (opts_i->outsl != -1)
			opts.outsl = opts_i->outsl;

		if (opts_i->service_id)
			opts.service_id = (uint64_t) opts_i->service_id;
		if (opts_i->path_res_type != PSM2_PATH_RES_NONE)
			opts.path_res_type = opts_i->path_res_type;

		if (opts_i->senddesc_num)
			opts.senddesc_num = opts_i->senddesc_num;

		if (opts_i->imm_size)
			opts.imm_size = opts_i->imm_size;
	}

	/* Get Service ID from environment */
	if (!psmi_getenv("PSM3_IB_SERVICE_ID",
			 "Service ID for RV module RC QP connection establishment",
			 PSMI_ENVVAR_LEVEL_USER,
			 PSMI_ENVVAR_TYPE_ULONG_FLAGS, // FLAGS only affects output: hex
			 (union psmi_envvar_val)HFI_DEFAULT_SERVICE_ID,
			 &envvar_val)) {
		opts.service_id = (uint64_t) envvar_val.e_ulonglong;
	}

	opts.path_res_type = PSM2_PATH_RES_NONE;

	/* If a specific unit is set in the environment, use that one. */
	// PSM3_NIC may be a unit name, number, "any" or -1
	if (!psmi_getenv("PSM3_NIC", "Device Unit number or name (-1 or 'any' autodetects)",
			 PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			 (union psmi_envvar_val)"any", &envvar_val)) {
		if (0 == strcasecmp(envvar_val.e_str, "any")) {
			opts.unit = PSM3_NIC_ANY;
		} else {
			// convert name to a unit number since rest of APIs use number
			opts.unit = sysfs_find_unit(envvar_val.e_str);
			if (opts.unit < 0) {
				err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Unit unknown %s", envvar_val.e_str);
				goto fail;
			}
		}
	}

	/* Get user specified port number to use. */
	if (!psmi_getenv("PSM3_NIC_PORT", "NIC Port number (0 autodetects)",
			 PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_LONG,
			 (union psmi_envvar_val)PSM3_NIC_PORT_ANY,
			 &envvar_val)) {
		opts.port = envvar_val.e_long;
	}

	/* Get service level from environment, path-query overrides it */
	if (!psmi_getenv
	    ("PSM3_NIC_SL", "NIC outging ServiceLevel number (default 0)",
	     PSMI_ENVVAR_LEVEL_HIDDEN,
	     PSMI_ENVVAR_TYPE_LONG,
	     (union psmi_envvar_val)PSMI_SL_DEFAULT, &envvar_val)) {
		opts.outsl = envvar_val.e_long;
	}

	/* Get network key from environment. MVAPICH and other vendor MPIs do not
	 * specify it on ep open and we may require it for vFabrics.
	 * path-query will override it.
	 */
	if (!psmi_getenv("PSM3_PKEY",
			 "PKey to use for endpoint (0=use slot 0)",
			 PSMI_ENVVAR_LEVEL_HIDDEN,
			 PSMI_ENVVAR_TYPE_ULONG_FLAGS,	// show in hex
			 (union psmi_envvar_val)((unsigned int)(psmi_hal_get_default_pkey())),
			 &envvar_val)) {
		opts.network_pkey = (uint64_t) envvar_val.e_ulong;
	}

	/* BACKWARDS COMPATIBILITY:  Open MPI likes to choose its own PKEY of
	   0x7FFF.  That's no longer a valid default, so override it if the
	   client was compiled against PSM v1 */
	if (PSMI_VERNO_GET_MAJOR(psmi_verno_client()) < 2 &&
			opts.network_pkey == 0x7FFF) {
		opts.network_pkey = psmi_hal_get_default_pkey();;
	}

	/* Get number of default send buffers from environment */
	if (!psmi_getenv("PSM3_NUM_SEND_BUFFERS",
			 "Number of send buffers to allocate [1024]",
			 PSMI_ENVVAR_LEVEL_HIDDEN,
			 PSMI_ENVVAR_TYPE_UINT,
			 (union psmi_envvar_val)1024, &envvar_val)) {
		opts.sendbufs_num = envvar_val.e_uint;
	}

	/* Get immediate data size - transfers less than immediate data size do
	 * not consume a send buffer and require just a send descriptor.
	 */
	if (!psmi_getenv("PSM3_SEND_IMMEDIATE_SIZE",
			 "Immediate data send size not requiring a buffer [128]",
			 PSMI_ENVVAR_LEVEL_HIDDEN,
			 PSMI_ENVVAR_TYPE_UINT,
			 (union psmi_envvar_val)128, &envvar_val)) {
		opts.imm_size = envvar_val.e_uint;
	}

	/* Get number of send descriptors - by default this is 4 times the number
	 * of send buffers - mainly used for short/inlined messages.
	 */
	if (!psmi_getenv("PSM3_NUM_SEND_DESCRIPTORS",
			 "Number of send descriptors to allocate [4096]",
			 PSMI_ENVVAR_LEVEL_HIDDEN,
			 PSMI_ENVVAR_TYPE_UINT,
			 (union psmi_envvar_val)4096, &envvar_val)) {
		opts.senddesc_num = envvar_val.e_uint;
	}
	if (psmi_device_is_enabled(devid_enabled, PTL_DEVID_IPS)) {
		if ((err = psm2_ep_num_devunits(&num_units)) != PSM2_OK)
			goto fail;
	} else
		num_units = 0;

	/* do some error checking */
	if (opts.timeout < -1) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid timeout value %lld",
					(long long)opts.timeout);
		goto fail;
	} else if (num_units && (opts.unit < -1 || opts.unit >= (int)num_units)) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid Device Unit ID %d (%d units found)",
					opts.unit, num_units);
		goto fail;
	} else if ((opts.port < HFI_MIN_PORT || opts.port > HFI_MAX_PORT) &&
				opts.port != PSM3_NIC_PORT_ANY) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid Device port number %d",
					opts.port);
		goto fail;
	} else if (opts.affinity < 0
		   || opts.affinity > PSM2_EP_OPEN_AFFINITY_FORCE) {
		err =
		    psmi_handle_error(NULL, PSM2_PARAM_ERR,
				      "Invalid Affinity option: %d",
				      opts.affinity);
		goto fail;
	} else if (opts.outsl < PSMI_SL_MIN || opts.outsl > PSMI_SL_MAX) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
					"Invalid SL number: %lld",
					(unsigned long long)opts.outsl);
		goto fail;
	}

	/* Allocate end point structure storage */
	ptl_sizes =
	    (psmi_device_is_enabled(devid_enabled, PTL_DEVID_SELF) ?
	     psmi_ptl_self.sizeof_ptl() : 0) +
	    (psmi_device_is_enabled(devid_enabled, PTL_DEVID_IPS) ?
	     psmi_ptl_ips.sizeof_ptl() : 0) +
	    (psmi_device_is_enabled(devid_enabled, PTL_DEVID_AMSH) ?
	     psmi_ptl_amsh.sizeof_ptl() : 0);
	if (ptl_sizes == 0)
		return PSM2_EP_NO_DEVICE;

	ep = (psm2_ep_t) psmi_memalign(PSMI_EP_NONE, UNDEFINED, 64,
				      sizeof(struct psm2_ep) + ptl_sizes);
	epaddr = (psm2_epaddr_t) psmi_calloc(PSMI_EP_NONE, PER_PEER_ENDPOINT,
					    1, sizeof(struct psm2_epaddr));
	if (ep == NULL || epaddr == NULL) {
		err = psmi_handle_error(NULL, PSM2_NO_MEMORY,
					"Couldn't allocate memory for %s structure",
					ep == NULL ? "psm2_ep" : "psm2_epaddr");
		goto fail;
	}
	memset(ep, 0, sizeof(struct psm2_ep) + ptl_sizes);

	/* Copy PTL enabled status */
	for (i = 0; i < PTL_MAX_INIT; i++)
		ep->devid_enabled[i] = devid_enabled[i];

	/* Matched Queue initialization.  We do this early because we have to
	 * make sure ep->mq exists and is valid before calling ips_do_work.
	 */
	ep->mq = mq;

	/* Get ready for PTL initialization */
	memcpy(&ep->uuid, (void *)unique_job_key, sizeof(psm2_uuid_t));
	ep->epaddr = epaddr;
	ep->memmode = mq->memmode;
	ep->hfi_num_sendbufs = opts.sendbufs_num;
	ep->service_id = opts.service_id;
	ep->path_res_type = opts.path_res_type;
	ep->hfi_num_descriptors = opts.senddesc_num;
	ep->hfi_imm_size = opts.imm_size;
	ep->errh = psmi_errhandler_global;	/* by default use the global one */
	ep->ptl_amsh.ep_poll = psmi_poll_noop;
	ep->ptl_ips.ep_poll = psmi_poll_noop;
	ep->connections = 0;
	ep->rdmamode = psmi_parse_rdmamode();	// PSM3_RDMA
	/* MR cache mode */
	// we need this early when creating the verbs_ep since it may affect
	// if we open rv module.
	// The value returned is a MR_CACHE_MODE_* selection
	{
		union psmi_envvar_val env_mr_cache_mode;

		if (! (ep->rdmamode & IPS_PROTOEXP_FLAG_ENABLED)) {
			env_mr_cache_mode.e_uint = MR_CACHE_MODE_NONE;
		} else if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
			// RDMA enabled in kernel mode.  Must use rv MR cache
			env_mr_cache_mode.e_uint = MR_CACHE_MODE_RV;
		} else {
			/* Behavior of user space MR Cache
			 * when 0, we merely share MRs for concurrently used buffers
			 */
			// mode 2 (user space MR w/cache) is purposely not documented
			psmi_getenv("PSM3_MR_CACHE_MODE",
					"Enable MR caching 0=user space MR no cache"
#ifdef RNDV_MOD
					", 1=kernel MR w/cache [1]",
#else
					"[0]",
#endif
					PSMI_ENVVAR_LEVEL_USER,
					PSMI_ENVVAR_TYPE_UINT,
#ifdef RNDV_MOD
					(union psmi_envvar_val)MR_CACHE_MODE_KERNEL,
#else
					(union psmi_envvar_val)MR_CACHE_MODE_NONE,
#endif
					 &env_mr_cache_mode);
			if (! MR_CACHE_MODE_VALID(env_mr_cache_mode.e_uint)
				|| env_mr_cache_mode.e_uint == MR_CACHE_MODE_RV)
				env_mr_cache_mode.e_uint = MR_CACHE_MODE_NONE;
		}
#ifndef RNDV_MOD
		if (env_mr_cache_mode.e_uint == MR_CACHE_MODE_KERNEL)
			env_mr_cache_mode.e_uint = MR_CACHE_MODE_NONE;
#endif
		ep->mr_cache_mode = env_mr_cache_mode.e_uint;
	}

	/* See how many iterations we want to spin before yielding */
	psmi_getenv("PSM3_YIELD_SPIN_COUNT",
		    "Spin poll iterations before yield",
		    PSMI_ENVVAR_LEVEL_HIDDEN,
		    PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)PSMI_BLOCKUNTIL_POLLS_BEFORE_YIELD,
		    &envvar_val);
	ep->yield_spin_cnt = envvar_val.e_uint;

	/* Set skip_affinity flag if PSM is not allowed to set affinity */
	if (opts.affinity == PSM2_EP_OPEN_AFFINITY_SKIP)
		ep->skip_affinity = true;

	ptl_sizes = 0;
	amsh_ptl = ips_ptl = self_ptl = NULL;
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_AMSH)) {
		amsh_ptl = (ptl_t *) (ep->ptl_base_data + ptl_sizes);
		ptl_sizes += psmi_ptl_amsh.sizeof_ptl();
	}
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		ips_ptl = (ptl_t *) (ep->ptl_base_data + ptl_sizes);
		ptl_sizes += psmi_ptl_ips.sizeof_ptl();
	}
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_SELF)) {
		self_ptl = (ptl_t *) (ep->ptl_base_data + ptl_sizes);
		ptl_sizes += psmi_ptl_self.sizeof_ptl();
	}

	/* Get number of send WQEs
	 */
	psmi_getenv("PSM3_NUM_SEND_WQES",
			"Number of send WQEs to allocate [4080]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_SEND_QP_ENTRIES, &envvar_val);
	ep->hfi_num_send_wqes = envvar_val.e_uint;

	psmi_getenv("PSM3_SEND_REAP_THRESH",
			"Number of outstanding send WQEs before reap CQEs [256]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_SEND_CQ_REAP, &envvar_val);
	ep->hfi_send_reap_thresh = envvar_val.e_uint;

	psmi_getenv("PSM3_NUM_SEND_RDMA",
			"Number of user space send RDMA to allow [128]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_NUM_SEND_RDMA, &envvar_val);
	ep->hfi_num_send_rdma = envvar_val.e_uint;

	/* Get number of recv WQEs
	 */
	psmi_getenv("PSM3_NUM_RECV_WQES",
			"Number of recv WQEs to allocate [4095]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_RECV_QP_ENTRIES, &envvar_val);
	ep->hfi_num_recv_wqes = envvar_val.e_uint;

	/* Get number of recv CQEs
	 */
	psmi_getenv("PSM3_NUM_RECV_CQES",
			"Number of recv CQEs to allocate\n"
			"(0 will calculate as PSM3_NUM_RECV_WQES+1032 for PSM3_RDMA=0-2\n"
			"and 4000 more than that for PSM3_RDMA=3]) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->hfi_num_recv_cqes = envvar_val.e_uint;

	/* Get RC QP timeout and retry
	 */
	psmi_getenv("PSM3_QP_TIMEOUT",
			"Number of microseconds for RC QP timeouts [536870]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_ULONG,
			(union psmi_envvar_val)VERBS_QP_TIMEOUT, &envvar_val);
	ep->hfi_qp_timeout = timeout_usec_to_mult(envvar_val.e_ulong);

	psmi_getenv("PSM3_QP_RETRY",
			"Limit on retries after RC QP timeout or RNR [7]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)VERBS_QP_RETRY, &envvar_val);
	ep->hfi_qp_retry = (envvar_val.e_uint <= VERBS_QP_MAX_RETRY)?
								envvar_val.e_uint:VERBS_QP_MAX_RETRY;
	/* Size of RV Cache - only used for MR_CACHE_MODE_RV or KERNEL,
	 * otherwise ignored
	 */
	psmi_getenv("PSM3_RV_MR_CACHE_SIZE",
			"kernel space MR cache size"
			" (MBs, 0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	// TBD - min should be (HFI_TF_NFLOWS + ep->hfi_num_send_rdma) *
	// chunk size (mq->hfi_base_window_rv after psmi_mq_initialize_defaults
	// TBD actual window_sz may be larger than mq->hfi_base_window_rv
	ep->rv_mr_cache_size = envvar_val.e_uint;

	psmi_getenv("PSM3_RV_QP_PER_CONN",
			"Number of sets of RC QPs per RV connection (0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->rv_num_conn = envvar_val.e_uint;

	psmi_getenv("PSM3_RV_Q_DEPTH",
			"Size of QPs and CQs per RV QP (0 lets rv module decide) [0]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)0, &envvar_val);
	ep->rv_q_depth = envvar_val.e_uint;

	psmi_getenv("PSM3_RV_RECONNECT_TIMEOUT",
			"RV End-point minimum re-connection timeout in seconds. 0 for no connection recovery [30]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)30, &envvar_val);
	ep->rv_reconnect_timeout = envvar_val.e_uint;

	psmi_getenv("PSM3_RV_HEARTBEAT_INTERVAL",
			"RV End-point heartbeat interval in milliseconds. 0 for no heartbeat [1000]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1000, &envvar_val);
	ep->rv_hb_interval = envvar_val.e_uint;

	// HFI Interface.
	if ((err = psmi_ep_open_device(ep, &opts, unique_job_key,
				       &(ep->context), &ep->epid)))
		goto fail;

	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		_HFI_UDDBG("my QPN=%u (0x%x)  EPID=0x%"PRIx64" %s\n",
			ep->verbs_ep.qp->qp_num, ep->verbs_ep.qp->qp_num, (uint64_t)ep->epid,
			psmi_epaddr_fmt_addr(ep->epid));
	}
	psmi_assert_always(ep->epid != 0);
	ep->epaddr->epid = ep->epid;

	_HFI_VDBG("psmi_ep_open_device() passed\n");

	/* Set our new label as soon as we know what it is */
	strncpy(buf, psmi_gethostname(), sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	p = buf + strlen(buf);

	/* If our rank is set, use it (same as mylabel). If not, use context */
	/* hostname.rank# or hostname.# (context), or hostname.pid#
	 */
	if (hfi_get_myrank() >= 0)
		len = snprintf(p, sizeof(buf) - strlen(buf), ":rank%d.", hfi_get_myrank());
	else
		len = snprintf(p, sizeof(buf) - strlen(buf), ":"PSMI_EPID_CONTEXT_FMT".",
				PSMI_EPID_GET_CONTEXT_VAL(ep->epid));
	*(p + len) = '\0';
	ep->context_mylabel = psmi_strdup(ep, buf);
	if (ep->context_mylabel == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	/* hfi_set_mylabel(ep->context_mylabel); */

	if ((err = psmi_epid_set_hostname(psm2_epid_nid(ep->epid), buf, 0)))
		goto fail;

	_HFI_VDBG("start ptl device init...\n");
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_SELF)) {
		if ((err = psmi_ptl_self.init(ep, self_ptl, &ep->ptl_self)))
			goto fail;
	}
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		if ((err = psmi_ptl_ips.init(ep, ips_ptl, &ep->ptl_ips)))
			goto fail;
	}
	/* If we're shm-only, this device is enabled above */
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_AMSH)) {
		if ((err = psmi_ptl_amsh.init(ep, amsh_ptl, &ep->ptl_amsh)))
			goto fail;
	} else {
		/* We may have pre-attached as part of getting our rank for enabling
		 * shared contexts.  */
	}

	_HFI_VDBG("finish ptl device init...\n");

	/*
	 * Keep only IPS since only IPS support multi-rail, other devices
	 * are only setup once. IPS device can come to this function again.
	 */
	for (i = 0; i < PTL_MAX_INIT; i++) {
		if (devid_enabled[i] != PTL_DEVID_IPS) {
			devid_enabled[i] = -1;
		}
	}

	*epido = ep->epid;
	*epo = ep;

	return PSM2_OK;

fail:
	if (ep != NULL) {
		psmi_hal_close_context(&ep->context.psm_hw_ctxt);
		psmi_free(ep);
	}
	if (epaddr != NULL)
		psmi_free(epaddr);
	return err;
}

psm2_error_t
__psm2_ep_open(psm2_uuid_t const unique_job_key,
	      struct psm2_ep_open_opts const *opts_i, psm2_ep_t *epo,
	      psm2_epid_t *epido)
{
	psm2_error_t err;
	psm2_mq_t mq;
	psm2_epid_t epid;
	psm2_ep_t ep, tmp;
	uint32_t units[PSMI_MAX_QPS];
	uint16_t ports[PSMI_MAX_QPS];
	int i, num_rails = 0;
	char *uname = "PSM3_NIC";
	char *pname = "PSM3_NIC_PORT";
	char uvalue[6], pvalue[6];
	int devid_enabled[PTL_MAX_INIT];
	union psmi_envvar_val devs;
	int show_nics = 0;

	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (!epo || !epido)
		return PSM2_PARAM_ERR;

	/* Allowing only one EP (unless explicitly enabled). */
	if (psmi_opened_endpoint_count > 0 && !psmi_multi_ep_enabled) {
		PSM2_LOG_MSG("leaving");
		return PSM2_TOO_MANY_ENDPOINTS;
	}

	/* Matched Queue initialization.  We do this early because we have to
	 * make sure ep->mq exists and is valid before calling ips_do_work.
	 */
	err = psmi_mq_malloc(&mq);
	PSMI_LOCK(psmi_creation_lock);
	if (err != PSM2_OK)
		goto fail;

	/* Set some of the MQ thresholds from the environment.
	   Do this before ptl initialization - the ptl may have other
	   constraints that will limit the MQ's settings. */
	err = psmi_mq_initialize_defaults(mq);
	if (err != PSM2_OK)
		goto fail;

	psmi_init_lock(&(mq->progress_lock));

	/* See which ptl devices we want to use for this ep to be opened */
	psmi_getenv("PSM3_DEVICES",
		    "Ordered list of PSM-level devices",
		    PSMI_ENVVAR_LEVEL_USER,
		    PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)PSMI_DEVICES_DEFAULT, &devs);

	if ((err = psmi_parse_devices(devid_enabled, devs.e_str)))
		goto fail;

	if (psmi_device_is_enabled(devid_enabled, PTL_DEVID_IPS)) {
		show_nics = psmi_parse_identify();
		err = psmi_ep_multirail(&num_rails, units, ports);
		if (err != PSM2_OK)
			goto fail;

		/* If multi-rail is used, set the first ep unit/port */
		if (num_rails > 0) {
			snprintf(uvalue, 6, "%1d", units[0]);
			snprintf(pvalue, 6, "%1d", ports[0]);
			setenv(uname, uvalue, 1);
			setenv(pname, pvalue, 1);
		}
	}

#ifdef PSM_CUDA
	if (PSMI_IS_GDR_COPY_ENABLED)
		hfi_gdr_open();
#endif

	err = __psm2_ep_open_internal(unique_job_key,
				     devid_enabled, opts_i, mq, &ep, &epid);
	if (err != PSM2_OK)
		goto fail;

	if (psmi_opened_endpoint == NULL) {
		psmi_opened_endpoint = ep;
	} else {
		tmp = psmi_opened_endpoint;
		while (tmp->user_ep_next)
			tmp = tmp->user_ep_next;
		tmp->user_ep_next = ep;
	}
	psmi_opened_endpoint_count++;
	ep->mctxt_prev = ep->mctxt_next = ep;
	ep->mctxt_master = ep;
	mq->ep = ep;

	if (show_nics)
		printf("%s %s NIC %u (%s) Port %u\n",
			hfi_get_mylabel(), hfi_ident_tag,
			ep->unit_id,  sysfs_unit_dev_name(ep->unit_id),
			ep->portnum);

	/* Active Message initialization */
	err = psmi_am_init_internal(ep);
	if (err != PSM2_OK)
		goto fail;

	*epo = ep;
	*epido = epid;

	if (psmi_device_is_enabled(devid_enabled, PTL_DEVID_IPS)) {
		int j;
		union psmi_envvar_val envvar_val;

		if (num_rails <= 0) {
			// the NIC has now been selected for our process
			// use the same NIC for any additional QPs below
			num_rails = 1;
			units[0] = ep->unit_id;
			ports[0] = ep->portnum;
		}
		// When QP_PER_NIC >1, creates more than 1 QP on each NIC and then
		// uses the multi-rail algorithms to spread the traffic across QPs
		// This helps get better BW when there are relatively few processes/node
		// care must be taken when combining this with user space RC QPs as
		// scalability (memory footprint) issues can be multiplied
		// This approach duplicates some per NIC resources (CQs, etc) but
		// provides a simple approach
		psmi_getenv("PSM3_QP_PER_NIC",
			"Number of sets of QPs to open per NIC [1]",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)1, &envvar_val);

		if ((num_rails * envvar_val.e_uint) > PSMI_MAX_QPS) {
			err = psmi_handle_error(NULL, PSM2_TOO_MANY_ENDPOINTS,
				"PSM3_QP_PER_NIC (%u) * num_rails (%d) > Max Support QPs (%u)",
				envvar_val.e_uint, num_rails, PSMI_MAX_QPS);
			goto fail;
		}

		for (j= 0; j< envvar_val.e_uint; j++) {
			for (i = 0; i < num_rails; i++) {
				_HFI_VDBG("rail %d unit %u port %u\n", i, units[i], ports[i]);
				// did 0, 0 already above
				if (i == 0 && j== 0)
					continue;
				snprintf(uvalue, 6, "%1d", units[i]);
				snprintf(pvalue, 6, "%1d", ports[i]);
				setenv(uname, uvalue, 1);
				setenv(pname, pvalue, 1);

				/* Create slave EP */
				err = __psm2_ep_open_internal(unique_job_key,
						     devid_enabled, opts_i, mq,
						     &tmp, &epid);
				if (err)
					goto fail;

				/* Point back to shared resources on the master EP */
				tmp->am_htable = ep->am_htable;

				/* Link slave EP after master EP. */
				PSM_MCTXT_APPEND(ep, tmp);
				if (j == 0 && show_nics)
					printf("%s %s NIC %u (%s) Port %u\n",
						hfi_get_mylabel(), hfi_ident_tag,
						tmp->unit_id,  sysfs_unit_dev_name(tmp->unit_id),
						tmp->portnum);
			}
		}
	}

	_HFI_VDBG("psm2_ep_open() OK....\n");

fail:
	fflush(stdout);
	PSMI_UNLOCK(psmi_creation_lock);
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_ep_open)

psm2_error_t __psm2_ep_close(psm2_ep_t ep, int mode, int64_t timeout_in)
{
	psm2_error_t err = PSM2_OK;

	psmi_stats_ep_close();	// allow output of stats on 1st ep close if desired

#if _HFI_DEBUGGING
	uint64_t t_start = 0;
	if (_HFI_PRDBG_ON) {
		t_start = get_cycles();
	}
#endif

#ifdef PSM_CUDA
	/*
	 * The close on the gdr fd needs to be called before the
	 * close on the hfi fd as the the gdr device will hold
	 * reference count on the hfi device which will make the close
	 * on the hfi fd return without actually closing the fd.
	 */
	if (PSMI_IS_GDR_COPY_ENABLED)
		hfi_gdr_close();
#endif
	union psmi_envvar_val timeout_intval;
	psm2_ep_t tmp;
	psm2_mq_t mmq;

	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(ep);
	psmi_assert_always(ep->mctxt_master == ep);

	PSMI_LOCK(psmi_creation_lock);

	psmi_am_fini_internal(ep);

	if (psmi_opened_endpoint == NULL) {
		err = psmi_handle_error(NULL, PSM2_EP_WAS_CLOSED,
					"PSM Endpoint is closed or does not exist");
		PSM2_LOG_MSG("leaving");
		PSMI_UNLOCK(psmi_creation_lock);
		return err;
	}

	tmp = psmi_opened_endpoint;
	while (tmp && tmp != ep) {
		tmp = tmp->user_ep_next;
	}
	if (!tmp) {
		err = psmi_handle_error(NULL, PSM2_EP_WAS_CLOSED,
					"PSM Endpoint is closed or does not exist");
		PSM2_LOG_MSG("leaving");
		PSMI_UNLOCK(psmi_creation_lock);
		return err;
	}

	psmi_getenv("PSM3_CLOSE_TIMEOUT",
		    "End-point close timeout over-ride.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)0, &timeout_intval);

	if (getenv("PSM3_CLOSE_TIMEOUT")) {
		timeout_in = timeout_intval.e_uint * SEC_ULL;
	} else if (timeout_in > 0) {
		/* The timeout parameter provides the minimum timeout. A heuristic
		 * is used to scale up the timeout linearly with the number of
		 * endpoints, and we allow one second per 100 endpoints. */
		timeout_in = max(timeout_in, (ep->connections * SEC_ULL) / 100);
	}

	if (timeout_in > 0 && timeout_in < PSMI_MIN_EP_CLOSE_TIMEOUT)
		timeout_in = PSMI_MIN_EP_CLOSE_TIMEOUT;

	/* Infinite and excessive close time-out are limited here to a max.
	 * The "rationale" is that there is no point waiting around forever for
	 * graceful termination. Normal (or forced) process termination should clean
	 * up the context state correctly even if termination is not graceful. */
	if (timeout_in <= 0 || timeout_in > PSMI_MAX_EP_CLOSE_TIMEOUT)
		timeout_in = PSMI_MAX_EP_CLOSE_TIMEOUT;
	_HFI_PRDBG("Closing endpoint %p with force=%s and to=%.2f seconds and "
		   "%d connections\n",
		   ep, mode == PSM2_EP_CLOSE_FORCE ? "YES" : "NO",
		   (double)timeout_in / 1e9, (int)ep->connections);

	/* XXX We currently cheat in the sense that we leave each PTL the allowed
	 * timeout.  There's no good way to do this until we change the PTL
	 * interface to allow asynchronous finalization
	 */


	/* Check if transfer ownership of receive thread is needed before closing ep.
	 * In case of PSM3_MULTI_EP support receive thread is created and assigned
	 * to first opened endpoint. Receive thread is killed when closing this
	 * endpoint.
	 */
	if (ep->user_ep_next != NULL) {
		/* Receive thread will be transfered and assigned to ep->user_ep_next
		 * only if currently working receive thread (which will be killed) is
		 * assigned to ep and there isn't any assigned to ep->user_ep_next.
		 */
		if ((psmi_ptl_ips_rcvthread.is_enabled(ep->ptl_ips.ptl)) &&
		    (!psmi_ptl_ips_rcvthread.is_enabled(ep->user_ep_next->ptl_ips.ptl)))
			psmi_ptl_ips_rcvthread.transfer_ownership(ep->ptl_ips.ptl, ep->user_ep_next->ptl_ips.ptl);
	}

	/*
	 * Before freeing the master ep itself,
	 * remove it from the global linklist.
	 * We do it here to let atexit handler in ptl_am directory
	 * to search the global linklist and free the shared memory file.
	 */
	if (psmi_opened_endpoint == ep) {
		/* Removing ep from global endpoint list. */
		psmi_opened_endpoint = ep->user_ep_next;
	} else {
		tmp = psmi_opened_endpoint;
		while (tmp->user_ep_next != ep) {
			tmp = tmp->user_ep_next;
		}
		/* Removing ep from global endpoint list. */
		tmp->user_ep_next = ep->user_ep_next;
	}
	psmi_opened_endpoint_count--;

	/*
	 * This do/while loop is used to close and free memory of endpoints.
	 *
	 * If MULTIRAIL feature is disable this loop will be passed only once
	 * and only endpoint passed in psm2_ep_close will be closed/removed.
	 *
	 * If MULTIRAIL feature is enabled then this loop will be passed
	 * multiple times (depending on number of rails). The order in which
	 * endpoints will be closed is shown below:
	 *
	 *                      |--this is master endpoint in case of multirail
	 *	                |  this endpoint is passed to psm2_ep_close and
	 *			V  this is only endpoint known to user.
	 *   +<-Ep0<-Ep1<-Ep2<-Ep3
	 *   |__________________|	Ep3->mctxt_prev points to Ep2
	 *	(3)  (2)  (1)  (4)	Ep2->mctxt_prev points to Ep1
	 *	 ^			Ep1->mctxt_prev points to Ep0
	 *	 |			Ep0->mctxt_prev points to Ep3 (master ep)
	 *	 |
	 *       |---- order in which endpoints will be closed.
	 *
	 * Closing MULTIRAILs starts by closing slaves (Ep2, Ep1, Ep0)
	 * If MULTIRAIL is enabled then Ep3->mctxt_prev will point to Ep2, if
	 * feature is disabled then Ep3->mctxt_prev will point to Ep3 and
	 * do/while loop will have one pass.
	 *
	 * In case of MULTIRAIL enabled Ep3 which is master endpoint will be
	 * closed as the last one.
	 */
	mmq = ep->mq;
	if (mmq) {
		// in case mq_finalize not called, need to get stats out
		// it will be a noop if called twice
		psm2_mq_finalize(mmq);
	}
	tmp = ep->mctxt_prev;
	do {
		ep = tmp;
		tmp = ep->mctxt_prev;

		PSMI_LOCK(ep->mq->progress_lock);

		PSM_MCTXT_REMOVE(ep);
		if (psmi_ep_device_is_enabled(ep, PTL_DEVID_AMSH))
			err =
			    psmi_ptl_amsh.fini(ep->ptl_amsh.ptl, mode,
					       timeout_in);

		if ((err == PSM2_OK || err == PSM2_TIMEOUT) &&
		    psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS))
			err =
			    psmi_ptl_ips.fini(ep->ptl_ips.ptl, mode,
					      timeout_in);

		/* If there's timeouts in the disconnect requests,
		 * still make sure that we still get to close the
		 *endpoint and mark it closed */
		if (psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS))
			psmi_context_close(&ep->context);

		psmi_epid_remove_all(ep);
		psmi_free(ep->epaddr);
		psmi_free(ep->context_mylabel);

		PSMI_UNLOCK(ep->mq->progress_lock);

		ep->mq = NULL;
		__psm2_ep_free_verbs(ep);

		psmi_free(ep);

	} while ((err == PSM2_OK || err == PSM2_TIMEOUT) && tmp != ep);

	if (mmq) {
		psmi_destroy_lock(&(mmq->progress_lock));
		err = psmi_mq_free(mmq);
	}

	if (hfi_lids)
	{
		psmi_free(hfi_lids);
		hfi_lids = NULL;
		nlids = 0;
	}

	PSMI_UNLOCK(psmi_creation_lock);

	if (_HFI_PRDBG_ON) {
		_HFI_PRDBG_ALWAYS("Closed endpoint in %.3f secs\n",
				 (double)cycles_to_nanosecs(get_cycles() -
				 t_start) / SEC_ULL);
	}
	PSM2_LOG_MSG("leaving");
	return err;
}
PSMI_API_DECL(psm2_ep_close)

static
psm2_error_t
psmi_ep_open_device(const psm2_ep_t ep,
		    const struct psm2_ep_open_opts *opts,
		    const psm2_uuid_t unique_job_key,
		    struct psmi_context *context, psm2_epid_t *epid)
{
	psm2_error_t err = PSM2_OK;

	/* Skip affinity.  No affinity if:
	 * 1. User explicitly sets no-affinity=YES in environment.
	 * 2. User doesn't set affinity in environment and PSM is opened with
	 *    option affinity skip.
	 */
	if (psmi_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		union psmi_envvar_val env_rcvthread;
		static int norcvthread;	/* only for first rail */

		ep->out_sl = opts->outsl;

		if ((err =
		     psmi_context_open(ep, opts->unit, opts->port,
				       unique_job_key, opts->timeout,
				       context)) != PSM2_OK)
			goto fail;

		_HFI_DBG("[%d]use unit %d port %d\n", getpid(),
			 ep->unit_id, 1);

		/* At this point, we have the unit id and port number, so
		 * check if pkey is not 0x0/0x7fff/0xffff, and match one
		 * of the pkey in table.
		 */
		if ((err =
		     psmi_ep_verify_pkey(ep, (uint16_t) opts->network_pkey,
					 &ep->network_pkey, &ep->network_pkey_index)) != PSM2_OK)
			goto fail;

		/* See if we want to activate support for receive thread */
		psmi_getenv("PSM3_RCVTHREAD",
			    "Enable Recv thread (0 disables thread)",
			    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
				// default to 0 for all but 1st rail
			    (union psmi_envvar_val)(norcvthread++ ? 0 :
						    PSMI_RCVTHREAD_FLAGS),
			    &env_rcvthread);

		/* If enabled, use the polling capability to implement a receive
		 * interrupt thread that can handle urg packets */
		if (env_rcvthread.e_uint) {
			psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD);
#ifdef PSMI_PLOCK_IS_NOLOCK
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "#define PSMI_PLOCK_IS_NOLOCK not functional yet "
					  "with RCVTHREAD on");
#endif
		}

		*epid = context->epid;
	} else if (psmi_ep_device_is_enabled(ep, PTL_DEVID_AMSH)) {
		*epid = PSMI_EPID_PACK_SHM(getpid(),
								PSMI_EPID_SHM_ONLY); /*is a only-shm epid */
	} else {
		/* Self-only, meaning only 1 proc max */
		*epid = PSMI_EPID_PACK_SHM(0,
								PSMI_EPID_SHM_ONLY); /*is a only-shm epid */
	}

fail:
	return err;
}

/* Get a list of PTLs we want to use.  The order is important, it affects
 * whether node-local processes use shm or ips */
static
psm2_error_t
psmi_parse_devices(int devices[PTL_MAX_INIT], const char *devstring)
{
	char *devstr = NULL;
	char *b_new, *e, *ee, *b;
	psm2_error_t err = PSM2_OK;
	int len;
	int i = 0;

	psmi_assert_always(devstring != NULL);
	len = strlen(devstring) + 1;

	for (i = 0; i < PTL_MAX_INIT; i++)
		devices[i] = -1;

	devstr = (char *)psmi_calloc(PSMI_EP_NONE, UNDEFINED, 2, len);
	if (devstr == NULL)
		goto fail;

	b_new = (char *)devstr;
	e = b_new + len;
	strncpy(e, devstring, len);
	ee = e + len;
	i = 0;
	while (e < ee && *e && i < PTL_MAX_INIT) {
		while (*e && !isalpha(*e))
			e++;
		b = e;
		while (*e && isalpha(*e))
			e++;
		*e = '\0';
		if (*b) {
			if (!strcasecmp(b, "self")) {
				devices[i++] = PTL_DEVID_SELF;
				b_new = strcpy(b_new, "self,");
				b_new += 5;
			} else if (!strcasecmp(b, "shm") ||
					!strcasecmp(b, "shmem") ||
					!strcasecmp(b, "amsh")) {
				devices[i++] = PTL_DEVID_AMSH;
				strcpy(b_new, "amsh,");
				b_new += 5;
			} else if (!strcasecmp(b, "hfi") ||
					!strcasecmp(b, "nic") ||
					!strcasecmp(b, "ipath") ||
					!strcasecmp(b, "ips")) {
				devices[i++] = PTL_DEVID_IPS;
				strcpy(b_new, "ips,");
				b_new += 4;
			} else {
				err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
							"%s set in environment variable PSM_PTL_DEVICES=\"%s\" "
							"is not one of the recognized PTL devices (%s)",
							b, devstring,
							PSMI_DEVICES_DEFAULT);
				goto fail;
			}
			e++;
		}
	}
	if (b_new != devstr)	/* we parsed something, remove trailing comma */
		*(b_new - 1) = '\0';

	_HFI_PRDBG("PSM Device allocation order: %s\n", devstr);
fail:
	if (devstr != NULL)
		psmi_free(devstr);
	return err;

}

static
int psmi_device_is_enabled(const int devid_enabled[PTL_MAX_INIT], int devid)
{
	int i;
	for (i = 0; i < PTL_MAX_INIT; i++)
		if (devid_enabled[i] == devid)
			return 1;
	return 0;
}

int psmi_ep_device_is_enabled(const psm2_ep_t ep, int devid)
{
	return psmi_device_is_enabled(ep->devid_enabled, devid);
}
