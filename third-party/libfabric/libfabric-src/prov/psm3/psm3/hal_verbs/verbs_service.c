#ifdef PSM_VERBS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2018 Intel Corporation.

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

  Copyright(c) 2018 Intel Corporation.

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

/* This file contains hfi service routine interface used by the low
   level hfi protocol code. */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <poll.h>
#include <inttypes.h>
#include "utils_sysfs.h"
#include "utils_env.h"
#include "verbs_service.h"
#include "psmi_wrappers.h"
#include "psm_netutils.h"

/* get the number of units supported by the driver.  Does not guarantee */
/* that a working chip has been found for each possible unit #. */
/* number of units >=0 (0 means none found). */
/* formerly used sysfs file "num_units" */
int psm3_hfp_verbs_get_num_units(void)
{
	int ret = 0;

	while (1) {
		char pathname[PATH_MAX];
		struct stat st;
		int r;

		snprintf(pathname, sizeof(pathname), "/dev/infiniband/uverbs%d", ret);
		r = stat(pathname, &st);
		if (r) break;

		ret++;
	}
	return ret;
}

/* Given a unit number, returns 1 if any port on the unit is active.
 * ports are also filtered based on PSM3_ADDR_FMT, PSM3_ADDR_PER_NIC and
 * PSM3_SUBNETS and ports without appropriate addresses are treated as
 * not active
 * returns <= 0 if no port on the unit is active.
 */
int psm3_verbs_get_unit_active(int unit, enum verbs_init_max_speed init_max_speed)
{
	int p, lid;

	for (p = HFI_MIN_PORT; p <= HFI_MAX_PORT; p++) {
		lid = psm3_verbs_get_port_lid(unit, p, 0/*addr_index*/, init_max_speed);
		if (lid > 0)
			break;
	}

	if (p <= HFI_MAX_PORT)
	{
		return 1;
	}

	return lid;
}

/* deterine if there are any active units.
 * returns 1 if at least 1 unfiltered, valid, active unit was found
 * returns 0 if none found
 * This routine is used during HAL selection prior to HAL initializion.
 * This routine and the functions it calls may call utils_sysfs.c functions
 * but cannot call any HAL routines (psmi_hal_*).
 * psm3_sysfs_init will have been called prior to this to establish the sysfs
 * path for devices in the HAL being checked
 */
int psm3_hfp_verbs_have_active_unit(int num_units)
{
	int i;
	int ret = 0;
	int find_max = ! psm3_nic_speed_wildcard
						|| (0 == strcmp(psm3_nic_speed_wildcard, "max"));

	psm3_nic_speed_max_found = 0;	// reset from any previous HAL
	for (i=0; i<num_units; i++) {
		if (psm3_verbs_get_unit_active(i, find_max?VIMS_FINDMAX:VIMS_FILTER) > 0) {
			_HFI_DBG("Found unfiltered active unit %d\n", i);
			if (! find_max)
				return 1;
			ret = 1;
		} else
			_HFI_DBG("Skipping unit %d: Filtered or not active\n", i);
	}
	return ret;
}

/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active.
   returns -1 when an error occurred. */
int psm3_hfp_verbs_get_port_active(int unit, int port)
{
	int ret;
	char *state;
	ret = psm3_sysfs_port_read(unit, port, "phys_state", &state);
	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single port chips */
			_HFI_VDBG
			    ("Failed to get phys_state for unit %u:%u: %s\n",
			     unit, port, strerror(errno));
		else
			_HFI_DBG
			    ("Failed to get phys_state for unit %u:%u: %s\n",
			     unit, port, strerror(errno));
		return -1;
	} else {
		if (strncmp(state, "5: LinkUp", 9)) {
			_HFI_DBG("Link is not Up for unit %u:%u\n", unit, port);
			psm3_sysfs_free(state);
			return 0;
		}
		psm3_sysfs_free(state);
		return 1;
	}
}

/* Given a unit number and port number, returns 1 if the unit and port is ethernet.
   returns 0 if the unit and port are not ethernet (IB or OPA).
   returns -1 when an error occurred. */
static int psm3_verbs_get_port_is_ethernet(int unit, int port)
{
	int ret;
	char *link_lyr;
	ret = psm3_sysfs_port_read(unit, port, "link_layer", &link_lyr);
	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single port chips */
			_HFI_VDBG("Failed to get link_layer for unit %u:%u: %s\n",
				  unit, port, strerror(errno));
		else
			_HFI_DBG("Failed to get link_layer for unit %u:%u: %s\n",
				 unit, port, strerror(errno));
		return -1;
	} else {
		ret = (strncmp(link_lyr, "Ethernet", strlen("Ethernet")) == 0);
		_HFI_VDBG("ret %d, unit %d port %d link_layer %s\n",
				  ret, unit, port, link_lyr);
		psm3_sysfs_free(link_lyr);
		return ret;
	}
}

/* Given the unit number, port and addr_index
 * return an error, or the corresponding LID
 * Used so the MPI code can determine it's own
 * LID, and which other LIDs (if any) are also assigned to this node
 * Returns an int, so <0 indicates an error.  0 may indicate that
 * the unit is valid, but no LID has been assigned.
 *
 * This routine is used in many places, such as get_unit_active, to
 * confirm the port is usable.  As such it includes additional checks that
 * the port is active and has an appropriate address based on PSM3_ADDR_FMT,
 * PSM3_ADDR_PER_NIC and PSM3_SUBNETS.  Ports without appropriate addresses
 * are treated as not initialized and return -1.
 *
 * For IB/OPA - actual LID is returned, values of 0 indicate
 *	port is not yet ready for use
 *	A LID of 0xffff causes a return of 0 as this is an uninitialized IB LID
 * For Ethernet (IPv4 or IPv6, RoCE or UDP) 1 is always reported (or <0 for err)
 *
 * No error print because we call this for both potential
 * ports without knowing if both ports exist (or are connected)
 */
int psm3_verbs_get_port_lid(int unit, int port, int addr_index, enum verbs_init_max_speed init_max_speed)
{
	int ret = 0;
	int64_t val = 0;
	uint64_t speed;
	if (port < HFI_MIN_PORT || port > HFI_MAX_PORT)
		return -1;

	if (psm3_hfp_verbs_get_port_active(unit,port) != 1)
		return -2;
	// make sure the port matches the wildcard
	if (1 != psm3_is_nic_allowed(unit))
		return -1;

	ret = psm3_verbs_get_port_is_ethernet(unit, port);
	if (ret < 0)
		return -1;
	if (ret == 0) {	// IB/OPA port
		ret = psm3_sysfs_port_read_s64(unit, port, "lid", &val, 0);
		_HFI_VDBG("ret %d, unit %d port %d lid %ld\n", ret, unit,
			port, (long int)val);
		if (ret < 0) {
			if (errno == ENODEV)
				/* this is "normal" for port != 1, on single port chips */
				_HFI_VDBG("Failed to get LID for unit %u:%u: %s\n",
					unit, port, strerror(errno));
			else
				_HFI_DBG("Failed to get LID for unit %u:%u: %s\n",
					unit, port, strerror(errno));
			return -1;
		}
		// For IB/OPA, PSM3_ADDR_PER_NIC is essentially ignored and addr_index>0
		// reports no LID available.  In future could use addr_index to select
		// among the LMC LIDs and check LMC has > PSM3_ADDR_PER_NIC here and in
		// get_port_subnet filtering of ports
		if (addr_index > 0) {
			_HFI_DBG("Only addr_index 0 supported for IB/OPA for unit %u:%u\n",
					unit, port);
			return 0;
		}
		// be paranoid, for an active port we should have a valid
		// LID 1-0xfffe (technically 1-0xbffff due to multicast)
		if (val == 0xffff)	// uninitialized IB LID
			val = 0;	// simplify job for callers
		if (! val) {
			_HFI_DBG("Uninitialized LID for unit %u:%u\n",
					unit, port);
			// no need to check other filters, can't use this unit
			return 0;
		}
	} else {
		val = 1;	// for Ethernet lid is N/A, use 1
	}

	// make sure can find acceptable addresses and subnets
	if (0 != psm3_hfp_verbs_get_port_subnet(unit, port, addr_index,
					NULL, NULL, NULL, NULL)) {
		_HFI_DBG("Failed to find acceptable subnet for unit %u:%u: %s\n",
			unit, port, strerror(errno));
		return -1;
	}

	if (init_max_speed != VIMS_NOFILTER) {
		if (0 != psm3_hfp_verbs_get_port_speed(unit, port, &speed)) {
			_HFI_DBG("Failed to get port speed for unit %u:%u: %s\n",
				unit, port, strerror(errno));
			return -1;
		}
		if (init_max_speed == VIMS_FINDMAX) {
			if (speed > psm3_nic_speed_max_found) {
				psm3_nic_speed_max_found = speed;
				_HFI_DBG("Updated max NIC speed unit %u:%u: %"PRIu64"\n",
					unit, port, speed);
			}
		} else if (1 != psm3_is_speed_allowed(unit, speed)) {
			return -1;
		}
	}

	ret = val;	// LID we got

	return ret;
}

/* Given the unit number, return an error, or the corresponding GID
 * When filter is set, we will ignore GIDs which aren't a "RoCE v2" type
 * (other possible types are "IB/RoCE v1" or "Invalid GID type")
 * Returns 0 on success, -1 on error.
 * No error print because we call this for both potential
 * ports without knowing if both ports exist (or are connected)
 */
static int psm3_verbs_get_port_gid(int unit, int port, int idx, int filter,
				psmi_gid128_t *gidp)
{
	int ret;
	char *gid_str = NULL;
	char attr_str[64];

	snprintf(attr_str, sizeof(attr_str), "gids/%d", idx < 0 ? 0 : idx);
	ret = psm3_sysfs_port_read(unit, port, attr_str, &gid_str);
	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single
			 * port chips */
			_HFI_VDBG("Failed to get GID %d for unit %u:%u: %s\n",
				  idx, unit, port, strerror(errno));
		else
			_HFI_DBG("Failed to get GID %d for unit %u:%u: %s\n",
				 idx, unit, port, strerror(errno));
	} else {
		uint32_t gid[8] = {0};
		if (sscanf(gid_str, "%4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x",
			   &gid[0], &gid[1], &gid[2], &gid[3],
			   &gid[4], &gid[5], &gid[6], &gid[7]) != 8) {
			_HFI_DBG("Failed to parse GID %d for unit %u:%u: %s\n",
				 idx, unit, port, gid_str);
			errno = EINVAL;
			ret = -1;
		} else {
			gidp->hi = (((uint64_t) gid[0]) << 48)
				| (((uint64_t) gid[1]) << 32)
				| (((uint64_t) gid[2]) << 16)
				| (((uint64_t) gid[3]) << 0);
			gidp->lo = (((uint64_t) gid[4]) << 48)
				| (((uint64_t) gid[5]) << 32)
				| (((uint64_t) gid[6]) << 16)
				| (((uint64_t) gid[7]) << 0);
			ret = 0;
		}
		psm3_sysfs_free(gid_str);
	}
	if (0 == ret && filter && (gidp->lo || gidp->hi)) {
		snprintf(attr_str, sizeof(attr_str), "gid_attrs/types/%d", idx < 0 ? 0 : idx);
		ret = psm3_sysfs_port_read(unit, port, attr_str, &gid_str);
		if (ret == -1) {
			_HFI_DBG("Failed to get GID type for unit %u:%u idx %d: %s\n",
					  unit, port, idx, strerror(errno));
		} else {
			/* gid_str includes newline, ignore it */
			if (strncmp(gid_str, "RoCE v2", strlen("RoCE v2"))) {
				/* treat filtered entries as empty */
				_HFI_DBG("Filtered out GID unit %d port %d idx %d %s %s",
					unit, port, idx,
					psm3_gid128_fmt(*gidp, 0), gid_str);
				gidp->hi = gidp->lo = 0;
			}
			psm3_sysfs_free(gid_str);
			ret = 0;
		}
	}

	return ret;
}

/* Given the unit number, port and addr_index,
 * return an error, or the corresponding subnet
 * address and GID selected for the unit/port/addr_index
 * For IB/OPA the subnet.hi is the hi 64b of the GID, subnet.lo is 0
 *		addr is the 128b GID
 *		prefix_len is always 64
 * For Ethernet IPv4: the subnet is derived from the IPv4 address and netmask
 *		subnet.hi is 0
 *		subnet.lo is the IPv4 address & netmask
 *		addr.lo is the full 32 bit IPv4 address, addr.hi is 0
 *		prefix_len also returned (1-32)
 * For Ethernet IPv6: the subnet is the 128b subnet of the 1st non-IPv4 GID
 *		addr is the full 128b IPv6 address
 *		prefix_len also returned (1-128)
 * idx and gid are always the full GID (RoCEv2 IPv4 style when IPv4 address)
 * All output values are in host byte order
 * Note this layout means (subnet | addr) == addr for all formats
 *
 * PSM3_FMT_ADDR (psm3_addr_fmt) sets preferred address type.
 *  0 (default) - consider all ports
 *	For Ethernet return first IPv4 addr found, if no IPv4 return 1st IPv6
 *	For OPA/IBA return 1st GID found
 *  FMT_IPATH, FMT_OPA - Native, only called for OPA ports, return 1st GID found
 *  FMT_IB - only consider IB/OPA ports
 *  FMT_IPV4 - only consider Ethernet ports with IPv4 addresses (return first)
 *  FMT_IPV6 - only consider Ethernet ports with IPv6 addresses (return first)
 *  When FMT_IB, FMT_IPV4 or FMT_IPV6 specified, non-matching ports return -1.
 *
 * Returns 0 on success, -1 on error.
 *
 * No error print because we call this for all potential
 * ports of a unit without knowing if each port exists (or is connected)
 * For Ethernet a unit will only have a single port (port 1), for IB a unit
 * may have more than 1 port.
*/
int psm3_hfp_verbs_get_port_subnet(int unit, int port, int addr_index,
			psmi_subnet128_t *subnet, psmi_naddr128_t *addr,
			int *idx, psmi_gid128_t *gid)
{
	int i;
	int is_eth = psm3_verbs_get_port_is_ethernet(unit, port);
	int cnt_ipv4_subnet = 0;	// count of IPv4 addrs found
	int ipv4_idx = INT_MAX;
	psmi_gid128_t ipv4_gid = { };
	uint8_t ipv4_prefix_len = 0;
	uint32_t ipv4_addr = 0, ipv4_netmask = 0;
	int cnt_ipv6_subnet = 0;	// count of IPv6 addrs found
	int ipv6_idx = INT_MAX;
	psmi_gid128_t ipv6_gid = { };
	uint8_t ipv6_prefix_len = 0;
	psmi_bare_netaddr128_t ipv6_netmask = { };
	int have_ib_subnet = 0;
	int ib_idx = INT_MAX;
	psmi_gid128_t ib_gid = { };

	if (is_eth < 0) {
		errno = ENODEV;
		return -1;
	}
	if (addr_index < 0 || addr_index > psm3_addr_per_nic) {
		errno = EINVAL;
		return -1;
	}

	// when PSM3_ADDR_FMT is FMT_IB, FMT_IPV4 or FMT_IPV6,
	// filter out ports of wrong type
	if (is_eth && psm3_addr_fmt == PSMI_ADDR_FMT_IB) {
		_HFI_DBG("Skipped Ethernet unit %d port %d PSM3_ADDR_FMT %u\n", unit, port, psm3_addr_fmt);
		return -1;
	}
	if (!is_eth && (psm3_addr_fmt == PSMI_ADDR_FMT_IPV4
		|| psm3_addr_fmt == PSMI_ADDR_FMT_IPV6)) {
		_HFI_DBG("Skipped IB/OPA unit %d port %d PSM3_ADDR_FMT %u\n", unit, port, psm3_addr_fmt);
		return -1;
	}
	// for IB/OPA we only allow addr_index==0 even if PSM3_ADDR_PER_NIC>1
	// In future might use addr_index to select among the LMC LIDs
	if (!is_eth && addr_index > 0) {
		_HFI_DBG("Skipped IB/OPA unit %d port %d addr_index %d\n", unit, port, addr_index);
		return -1;
	}

	for (i =0; ; i++) {
		psmi_gid128_t tmp_gid;
		if (-1 == psm3_verbs_get_port_gid(unit, port, i, is_eth, &tmp_gid))
			break; // stop at 1st non-existent gid (or non-existent port)
		// Skip over empty gid table entries.
		// for IB/OPA, the same SubnetPrefix is used for all entries
		// so just examine low 64 bits (InterfaceId)
		if (tmp_gid.lo == 0 && (! is_eth || tmp_gid.hi == 0))
			continue;

		_HFI_DBG("Examine %s GID unit %d port %d idx %d %s\n",
				is_eth? "Ethernet" : "IB/OPA",
				unit, port, i, psm3_gid128_fmt(tmp_gid, 0));
		// when PSM3_ADDR_FMT is FMT_IB, FMT_IPV4 or FMT_IPV6,
		// filter out GIDs of wrong type
		if (psm3_addr_fmt == PSMI_ADDR_FMT_IPV4
			&& (!is_eth || ! psmi_is_ipv4_gid(tmp_gid))) {
			_HFI_DBG("Skipped GID unit %d port %d PSM3_ADDR_FMT %u idx %d %s\n",
				unit, port, psm3_addr_fmt, i, psm3_gid128_fmt(tmp_gid, 0));
			continue;	// skip IPv6 GIDs if only want IPv4
		}
		if (psm3_addr_fmt == PSMI_ADDR_FMT_IPV6
			&& (!is_eth || psmi_is_ipv4_gid(tmp_gid))) {
			_HFI_DBG("Skipped GID unit %d port %d PSM3_ADDR_FMT %u idx %d %s\n",
				unit, port, psm3_addr_fmt, i, psm3_gid128_fmt(tmp_gid, 0));
			continue;	// skip IPv4 GIDs if only want IPv6
		}

		if (! have_ib_subnet && ! is_eth) {
				// allow subnets DBG logging explains choices
			if (!psm3_allow_ib_subnet(tmp_gid.hi))
				continue;

			ib_idx = i;
			ib_gid = tmp_gid;
			have_ib_subnet = 1;
			_HFI_DBG("Found IB/OPA GID unit %d port %d addr_index %d idx %d %s\n",
				unit, port, 0, i, psm3_gid128_fmt(tmp_gid, 0));
			// 1st IB address finalizes decision
			break;
		}
		if (cnt_ipv4_subnet < psm3_addr_per_nic
				&& is_eth && psmi_is_ipv4_gid(tmp_gid)) {
			uint32_t ipaddr = 0;	// leave as 0 when IB/OPA
			uint32_t nm = 0;;	// leave as 0 when IB/OPA
			uint8_t nm_len = 0;
			ipaddr = psmi_ipv4_from_gid(tmp_gid);
			if (psm3_get_eth_ipv4_netmask(ipaddr, &nm)) {
				_HFI_DBG("Can't determine netmask for IPv4 unit %d port %d idx %d %s\n",
					unit, port, i, psm3_gid128_fmt(tmp_gid, 0));
				continue;
			}

			nm_len = psm3_compute_ipv4_prefix_len(nm);
			if (nm_len <= 0) {
				_HFI_DBG("Invalid netmask for IPv4 unit %d port %d idx %d %s: %s\n",
					unit, port, i,
					psm3_gid128_fmt(tmp_gid, 0),
					psm3_ipv4_fmt(nm, 0, 1));
				continue;
			}

			// allow subnets DBG logging explains choices
			if (!psm3_allow_ipv4_subnet(ipaddr & nm, nm_len))
				continue;

			cnt_ipv4_subnet++;
			_HFI_DBG("Found IPv4 GID unit %d port %d addr_index %d idx %d %s\n",
				unit, port, cnt_ipv4_subnet-1, i, psm3_gid128_fmt(tmp_gid, 0));

			if (cnt_ipv4_subnet-1 != addr_index)
				continue;

			// save addr_index'th valid ipv4 gid
			ipv4_idx = i;
			ipv4_gid = tmp_gid;
			ipv4_prefix_len = nm_len;
			ipv4_addr = ipaddr;
			ipv4_netmask = nm;
			// enough IPv4 address on port to finalize decision?
			if (cnt_ipv4_subnet >= psm3_addr_per_nic
				&& (psm3_addr_fmt == PSMI_ADDR_FMT_IPV4 || ! psm3_addr_fmt))
				break;
		}
		if (cnt_ipv6_subnet < psm3_addr_per_nic
				&& is_eth && ! psmi_is_ipv4_gid(tmp_gid)) {
			psmi_bare_netaddr128_t nm = { };
			uint8_t nm_len = 0;
			if (psm3_get_eth_ipv6_netmask(tmp_gid, &nm)) {
				_HFI_DBG("Can't determine netmask for IPv6 unit %d port %d idx %d %s\n",
					unit, port, i, psm3_gid128_fmt(tmp_gid, 0));
				continue;
			}
			nm_len = psm3_compute_ipv6_prefix_len(nm);
			if (nm_len <= 0) {
				_HFI_DBG("Invalid netmask for IPv6 unit %d port %d idx %d %s: %s\n",
					unit, port, i,
					psm3_gid128_fmt(tmp_gid, 0),
					psm3_ipv6_fmt(nm, 0, 1));
				continue;
			}

			// allow subnets DBG logging explains choices
			if (!psm3_allow_ipv6_subnet(psmi_bare_netaddr128_and(tmp_gid, nm), nm_len))
				continue;

			cnt_ipv6_subnet++;
			_HFI_DBG("Found IPv6 GID unit %d port %d addr_index %d idx %d %s\n",
				unit, port,  cnt_ipv6_subnet-1, i, psm3_gid128_fmt(tmp_gid, 0));

			if (cnt_ipv6_subnet-1 != addr_index)
				continue;

			// addr_index'th valid ipv6
			ipv6_idx = i;
			ipv6_gid = tmp_gid;
			ipv6_prefix_len = nm_len;
			ipv6_netmask = nm;
			// enough IPv6 address on port to finalize decision?
			if (cnt_ipv6_subnet >= psm3_addr_per_nic
				&& psm3_addr_fmt == PSMI_ADDR_FMT_IPV6)
				break;
		}
	}
	if (! is_eth) {
		// IB/OPA
		if (have_ib_subnet) {
			if (idx) *idx = ib_idx;
			if (subnet) *subnet = psm3_build_ib_subnet128(ib_gid.hi);
			if (addr) *addr = psm3_build_ib_naddr128(ib_gid);
			if (gid) *gid = ib_gid;
			_HFI_DBG("Selected IB/OPA for unit %d port %d addr_index %d idx %d %s\n",
				unit, port, addr_index, ib_idx, psm3_gid128_fmt(ib_gid, 0));
			return 0;
		} else {
			_HFI_DBG("None Found IB/OPA unit %d port %d\n", unit, port);
			errno = ENXIO;
			return -1;
		}
	} else if (cnt_ipv4_subnet >= psm3_addr_per_nic) {
		// IPv4 Ethernet
		if (idx) *idx = ipv4_idx;
		if (subnet) *subnet = psm3_build_ipv4_subnet128(ipv4_addr, ipv4_netmask, ipv4_prefix_len);
		if (addr) *addr = psm3_build_ipv4_naddr128(ipv4_addr, ipv4_prefix_len);
		if (gid) *gid = ipv4_gid;
		_HFI_DBG("Selected IPv4 for unit %d port %d addr_index %d idx %d %s\n",
			unit, port, addr_index, ipv4_idx, psm3_gid128_fmt(ipv4_gid, 0));
		return 0;
	} else if (cnt_ipv6_subnet >= psm3_addr_per_nic) {
		// IPv6 Ethernet
		if (idx) *idx = ipv6_idx;
		if (subnet) *subnet = psm3_build_ipv6_subnet128(ipv6_gid, ipv6_netmask, ipv6_prefix_len);
		if (addr) *addr = psm3_build_ipv6_naddr128(ipv6_gid, ipv6_prefix_len);
		if (gid) *gid = ipv6_gid;
		_HFI_DBG("Selected IPv6 for unit %d port %d addr_index %d idx %d %s\n",
			unit, port, addr_index, ipv6_idx, psm3_gid128_fmt(ipv6_gid, 0));
		return 0;
	} else {
		if ((cnt_ipv4_subnet || cnt_ipv6_subnet) && psm3_addr_per_nic > 1)
			_HFI_DBG("None Found Ethernet unit %d port %d addr_index %d (%d IPv4 addrs and %d IPv6 addrs < PSM3_ADDR_PER_NIC %d)\n",
				unit, port, addr_index, cnt_ipv4_subnet, cnt_ipv6_subnet, psm3_addr_per_nic);
		else
			_HFI_DBG("None Found Ethernet unit %d port %d addr_index %d\n", unit, port, addr_index);
		errno = ENXIO;
		return -1;
	}
}

/* in units of bits/sec */
int psm3_hfp_verbs_get_port_speed(int unit, int port, uint64_t *speed)
{
	uint64_t mbps;

	mbps = psm3_parse_force_speed();
	if (! mbps) {
		uint32_t gbps;
		char *speedstr = NULL;
		if (psm3_sysfs_port_read(unit, port, "rate", &speedstr) < 0) {
			_HFI_DBG("Failed to get port speed for unit %u/%u: %s\n",
				unit, port, strerror(errno));
#if PSM_DEFAULT_SPEED > 0
			mbps = PSM_DEFAULT_SPEED;
			goto done;
#else
			return -1;
#endif
		}
		int n = sscanf(speedstr, "%u Gb/sec", &gbps);
		if (n != 1) {
			_HFI_DBG("Failed to parse port speed(%s) for unit %u/%u: sccanf ret = %d\n",
				speedstr, unit, port, n);
			psm3_sysfs_free(speedstr);
#if PSM_DEFAULT_SPEED > 0
			mbps = PSM_DEFAULT_SPEED;
			goto done;
#else
			errno = EINVAL;
			return -1;
#endif
		}
		psm3_sysfs_free(speedstr);
		if (speed) *speed = (uint64_t)gbps * 1000 * 1000 * 1000;
		_HFI_DBG("Got speed for for unit/port %d/%d: %u Gb/s\n",
			unit, port, gbps);
	} else {
#if PSM_DEFAULT_SPEED > 0
done:
#endif
		if (speed) *speed = mbps * 1000 * 1000;
		_HFI_DBG("Got speed for for unit/port %d/%d: %"PRIu64" Mb/s\n",
			unit, port, mbps);
	}
	return 0;
}
#endif /* PSM_VERBS */
