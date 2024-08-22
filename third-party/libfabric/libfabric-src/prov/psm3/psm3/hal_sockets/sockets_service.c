#ifdef PSM_SOCKETS
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
#include "sockets_service.h"
#include "psmi_wrappers.h"
#include "psm_netutils.h"
#include <sys/ioctl.h>
#include <net/if.h>

/* get the number of units supported by the driver.  Does not guarantee */
/* that a working chip has been found for each possible unit #. */
/* number of units >=0 (0 means none found). */
/* formerly used sysfs file "num_units" */
int psm3_hfp_sockets_get_num_units(void)
{
	int ret = 0;
	DIR *net_dir = opendir("/sys/class/net/");

	if (!net_dir) return 0;

	while (1) {
		char pathname[PATH_MAX];
		struct stat st;
		int r;

		struct dirent *dir = readdir(net_dir);
		if (!dir) break; // End of directory
		if (dir->d_name[0] == '.') continue; // Skip "." and ".."
		snprintf(pathname, sizeof(pathname), "/sys/class/net/%s", dir->d_name);
		r = stat(pathname, &st);
		if (r) break;

		ret++;
	}
	closedir(net_dir);
	return ret;
}

/* Given a unit number, returns 1 if any port on the unit is active.
 * ports are also filtered based on PSM3_ADDR_FMT, PSM3_ADDR_PER_NIC
 * and PSM3_SUBNETS and ports without appropriate addresses are treated as
 * not active
 * returns <= 0 if no port on the unit is active.
 */
int psm3_sockets_get_unit_active(int unit, enum sockets_init_max_speed init_max_speed)
{
	int p, lid;

	for (p = HFI_MIN_PORT; p <= HFI_MAX_PORT; p++) {
		lid = psm3_sockets_get_port_lid(unit, p, 0 /*addr_index */, init_max_speed);
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
int psm3_hfp_sockets_have_active_unit(int num_units)
{
	int i;
	int ret = 0;
	int find_max = ! psm3_nic_speed_wildcard
						|| (0 == strcmp(psm3_nic_speed_wildcard, "max"));

	psm3_nic_speed_max_found = 0;	// reset from any previous HAL
	for (i=0; i<num_units; i++) {
		if (psm3_sockets_get_unit_active(i, find_max?SIMS_FINDMAX:SIMS_FILTER) > 0) {
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
int psm3_hfp_sockets_get_port_active(int unit, int port)
{
	int ret;
	char *state;
	ret = psm3_sysfs_port_read(unit, port, "operstate", &state);
	if (ret == -1) {
		_HFI_DBG
			("Failed to get operstate for unit %u:%u: %s\n",
			 unit, port, strerror(errno));
		return -1;
	} else {
		if (strncmp(state, "up", 2)) {
			_HFI_DBG("Link is not Up for unit %u:%u\n", unit, port);
			psm3_sysfs_free(state);
			return 0;
		}
		psm3_sysfs_free(state);
		return 1;
	}
}

/* Given the unit number, return an error, or the corresponding LID
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
int psm3_sockets_get_port_lid(int unit, int port, int addr_index, enum sockets_init_max_speed init_max_speed)
{
	uint64_t speed;

	if (port < HFI_MIN_PORT || port > HFI_MAX_PORT)
		return -1;

	if (psm3_hfp_sockets_get_port_active(unit,port) != 1)
		return -2;
	// make sure the port matches the wildcard
	if (1 != psm3_is_nic_allowed(unit))
		return -1;

	// make sure can find acceptable addresses and subnets
	if (0 != psm3_hfp_sockets_get_port_subnet(unit, port, addr_index,
					NULL, NULL, NULL, NULL)) {
		_HFI_DBG("Failed to find acceptable subnet for unit %u:%u: %s\n",
			unit, port, strerror(errno));
		return -1;
	}
	if (0 != psm3_hfp_sockets_get_port_speed(unit, port, &speed)) {
		_HFI_DBG("Failed to get port speed for unit %u:%u: %s\n",
			unit, port, strerror(errno));
		return -1;
	}
	if (init_max_speed != SIMS_NOFILTER) {
		if (init_max_speed == SIMS_FINDMAX) {
			if (speed > psm3_nic_speed_max_found) {
				psm3_nic_speed_max_found = speed;
				_HFI_DBG("Updated max NIC speed unit %u:%u: %"PRIu64"\n",
					unit, port, speed);
			}
		} else if (1 != psm3_is_speed_allowed(unit, speed)) {
			return -1;
		}
	}
	return 1;	// fake a valid LID, similar to value reported by RoCE
}

/* Given the unit number, port and addr_index
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
int psm3_hfp_sockets_get_port_subnet(int unit, int port, int addr_index,
			psmi_subnet128_t *subnet, psmi_naddr128_t *addr,
			int *idx, psmi_gid128_t *gid)
{
	int cnt_ipv4_subnet = 0;	// count of IPv4 addrs found
	uint8_t ipv4_prefix_len = 0;
	uint32_t ipv4_addr = 0, ipv4_netmask = 0;
	int cnt_ipv6_subnet = 0;	// count of IPv6 addrs found
	uint8_t ipv6_prefix_len = 0;
	psmi_bare_netaddr128_t ipv6_netmask = { };
	psmi_gid128_t ipv6_gid = { };
	struct ifaddrs *ifap, *ifa;
	const char *ifname;

	if (addr_index < 0 || addr_index > psm3_addr_per_nic) {
		errno = EINVAL;
		return -1;
	}

	// when PSM3_ADDR_FMT is FMT_IB all ethernet ports filtered out
	if (psm3_addr_fmt == PSMI_ADDR_FMT_IB) {
		_HFI_DBG("Skipped Ethernet unit %d port %d PSM3_ADDR_FMT %u\n", unit, port, psm3_addr_fmt);
		return -1;
	}

	if (port != 1)
		return -1;

	/* get ifname from pathname for device */
	ifname = psm3_sysfs_unit_path(unit);
	if (!ifname)
		return -1;
	ifname = strrchr(ifname, '/');
	if (ifname == NULL)
		return -1;
	ifname++; // Inc past '/'

	if (getifaddrs(&ifap) != 0)
		return -1;

	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
		struct sockaddr *saddr = ifa->ifa_addr;
		struct sockaddr *nmask = ifa->ifa_netmask;

		if (! saddr || ! nmask) continue;
		if (saddr->sa_family != AF_INET
			&& saddr->sa_family != AF_INET6)
			continue;
		if (saddr->sa_family != nmask->sa_family) continue;
		if (strcmp(ifname, ifa->ifa_name)) continue;

		_HFI_DBG("Examine Ethernet addr %s unit %d port %d %s\n",
				ifname, unit, port, psm3_sockaddr_fmt_addr(saddr, 0));
		// when PSM3_ADDR_FMT is FMT_IPV4 or FMT_IPV6,
		// filter out addr of wrong type
		if (psm3_addr_fmt == PSMI_ADDR_FMT_IPV4
			&& saddr->sa_family != AF_INET) {
			_HFI_DBG("Skipped addr %s unit %d port %d PSM3_ADDR_FMT %u %s\n",
				ifname, unit, port, psm3_addr_fmt, psm3_sockaddr_fmt_addr(saddr, 0));
			continue;	// skip IPv6 addr if only want IPv4
		}
		if (psm3_addr_fmt == PSMI_ADDR_FMT_IPV6
			&& saddr->sa_family != AF_INET6) {
			_HFI_DBG("Skipped addr %s unit %d port %d PSM3_ADDR_FMT %u %s\n",
				ifname, unit, port, psm3_addr_fmt, psm3_sockaddr_fmt_addr(saddr, 0));
			continue;	// skip IPv4 addr if only want IPv6
		}

		if (cnt_ipv4_subnet < psm3_addr_per_nic
				&& saddr->sa_family == AF_INET) {
			uint8_t prefix_len;
			uint32_t ip_addr, netmask;

			ip_addr = __be32_to_cpu(((struct sockaddr_in*)saddr)->sin_addr.s_addr);
			netmask = __be32_to_cpu(((struct sockaddr_in*)nmask)->sin_addr.s_addr);
			prefix_len = psm3_compute_ipv4_prefix_len(netmask);
			if (prefix_len <= 0) {
				_HFI_DBG("Invalid netmask for IPv4 %s unit %d port %d %s: %s\n",
					ifname, unit, port,
					psm3_sockaddr_fmt_addr(saddr, 0),
					psm3_sockaddr_fmt_addr(nmask, 0));
				continue;
			}

			// allow subnets DBG logging explains choices
			if (!psm3_allow_ipv4_subnet(ip_addr & netmask, prefix_len))
				continue;

			cnt_ipv4_subnet++;
			_HFI_DBG("Found IPv4 addr %s unit %d port %d addr_index %d %s\n",
				ifname, unit, port, cnt_ipv4_subnet-1, psm3_sockaddr_fmt_addr(saddr, 0));

			if (cnt_ipv4_subnet-1 != addr_index)
				continue;

			// save addr_index'th valid ipv4 addr
			ipv4_addr = ip_addr;
			ipv4_netmask = netmask;
			ipv4_prefix_len = prefix_len;
			// enough IPv4 address on port to finalize decision?
			if (cnt_ipv4_subnet >= psm3_addr_per_nic
				&& (psm3_addr_fmt == PSMI_ADDR_FMT_IPV4 || ! psm3_addr_fmt))
				break;
		}
		if (cnt_ipv6_subnet < psm3_addr_per_nic
				&& saddr->sa_family == AF_INET6) {
			uint32_t *s6;
			uint8_t prefix_len = 0;
			psmi_bare_netaddr128_t netmask = { };
			psmi_gid128_t ip_gid = { };

			s6 = ((struct sockaddr_in6 *)saddr)->sin6_addr.s6_addr32;
			ip_gid.hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
			ip_gid.lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);

			s6 = ((struct sockaddr_in6 *)nmask)->sin6_addr.s6_addr32;
			netmask.hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
			netmask.lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);

			prefix_len = psm3_compute_ipv6_prefix_len(netmask);
			if (prefix_len <= 0) {
				_HFI_DBG("Invalid netmask for IPv6 %s unit %d port %d %s: %s\n",
					ifname, unit, port,
					psm3_sockaddr_fmt_addr(saddr, 0),
					psm3_sockaddr_fmt_addr(nmask, 0));
				continue;
			}

			// allow subnets DBG logging explains choices
			if (!psm3_allow_ipv6_subnet(psmi_bare_netaddr128_and(ip_gid, netmask), prefix_len))
				continue;

			cnt_ipv6_subnet++;
			_HFI_DBG("Found IPv6 addr %s unit %d port %d addr_index %d %s\n",
				ifname, unit, port, cnt_ipv6_subnet-1, psm3_sockaddr_fmt_addr(saddr, 0));

			if (cnt_ipv6_subnet-1 != addr_index)
				continue;

			// save addr_index'th valid ipv6 addr
			ipv6_gid = ip_gid;
			ipv6_netmask = netmask;
			ipv6_prefix_len = prefix_len;
			// enough IPv6 address on port to finalize decision?
			if (cnt_ipv6_subnet >= psm3_addr_per_nic
				&& psm3_addr_fmt == PSMI_ADDR_FMT_IPV6)
				break;
		}
	}
	(void)freeifaddrs(ifap);
	if (cnt_ipv4_subnet >= psm3_addr_per_nic) {
		// IPv4 Ethernet
		if (idx) *idx = 0;	// N/A
		if (subnet) *subnet = psm3_build_ipv4_subnet128(ipv4_addr, ipv4_netmask, ipv4_prefix_len);
		if (addr) *addr = psm3_build_ipv4_naddr128(ipv4_addr, ipv4_prefix_len);
		if (gid) {
			gid->hi = PSMI_IPV4_GID_HI(ipv4_addr);
			gid->lo = PSMI_IPV4_GID_LO(ipv4_addr);
		}
		_HFI_DBG("Selected IPv4 for %s unit %d port %d addr_index %d %s\n",
			ifname, unit, port, addr_index, psm3_ipv4_fmt(ipv4_addr, ipv4_prefix_len, 0));
		return 0;
	} else if (cnt_ipv6_subnet >= psm3_addr_per_nic) {
		// IPv6 Ethernet
		if (idx) *idx = 0;	// N/A
		if (subnet) *subnet = psm3_build_ipv6_subnet128(ipv6_gid, ipv6_netmask, ipv6_prefix_len);
		if (addr) *addr = psm3_build_ipv6_naddr128(ipv6_gid, ipv6_prefix_len);
		if (gid) *gid = ipv6_gid;
		_HFI_DBG("Selected IPv6 for %s unit %d port %d addr_index %d %s\n",
			ifname, unit, port, addr_index, psm3_ipv6_fmt(ipv6_gid, ipv6_prefix_len, 0));
		return 0;
	} else {
		if ((cnt_ipv4_subnet || cnt_ipv6_subnet) && psm3_addr_per_nic > 1)
			_HFI_DBG("None Found Ethernet %s unit %d port %d addr_index %d (%d IPv4 addrs and %d IPv6 addrs < PSM3_ADDR_PER_NIC %d)\n",
				ifname, unit, port, addr_index, cnt_ipv4_subnet, cnt_ipv6_subnet, psm3_addr_per_nic);
		else
			_HFI_DBG("None Found Ethernet %s unit %d port %d addr_index %d\n",
			ifname, unit, port, addr_index);
		errno = ENXIO;
		return -1;
	}
}

/* in units of bits/sec */
int psm3_hfp_sockets_get_port_speed(int unit, int port, uint64_t *speed)
{
	uint64_t mbps;

	mbps = psm3_parse_force_speed();
	if (! mbps) {
		char *speedstr = NULL;
		if (psm3_sysfs_port_read(unit, port, "speed", &speedstr) < 0) {
			_HFI_DBG("Failed to get port speed for unit %u/%u: %s\n",
				unit, port, strerror(errno));
#if PSM_DEFAULT_SPEED > 0
			mbps = PSM_DEFAULT_SPEED;
			goto done;
#else
			return -1;
#endif
		}
		mbps = (uint64_t)strtoul(speedstr, NULL, 0);
		if (mbps == 0 || mbps == ULONG_MAX) {
			_HFI_DBG("Failed to parse port speed(%s) for unit %u/%u: %s\n",
				speedstr, unit, port, strerror(errno));
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
	}
#if PSM_DEFAULT_SPEED > 0
done:
#endif
	if (speed) *speed = (uint64_t)mbps * 1000 * 1000;
	_HFI_DBG("Got speed for for unit/port %d/%d: %"PRIu64" Mb/s \n",
		unit, port, mbps);
	return 0;
}
#endif /* PSM_SOCKETS */
