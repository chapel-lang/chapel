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
#include "opa_service_gen1.h"
#include "psmi_wrappers.h"
#include "psm_netutils.h"

#define HFI_UD_NUM_CONTEXTS 	1024
#define HFI_UD_NUM_FREE_CTXTS 	1024








#ifdef PSM2_SUPPORT_IW_CMD_API
ustatic
int _hfi_cmd_ioctl(int fd, struct hfi1_cmd *cmd, size_t count)
{
	uint64_t addrOrLiteral[2] = { (uint64_t)cmd->addr, (uint64_t)&cmd->addr };
	const static struct
	{
		unsigned int ioctlCmd;
		unsigned int addrOrLiteralIdx;
	} cmdTypeToIoctlNum[PSMI_HFI_CMD_LAST] = {
        [PSMI_HFI_CMD_ASSIGN_CTXT]      = {HFI1_IOCTL_ASSIGN_CTXT   , 0},
        [PSMI_HFI_CMD_CTXT_INFO]        = {HFI1_IOCTL_CTXT_INFO     , 0},
        [PSMI_HFI_CMD_USER_INFO]        = {HFI1_IOCTL_USER_INFO     , 0},
        [PSMI_HFI_CMD_TID_UPDATE]       = {HFI1_IOCTL_TID_UPDATE    , 0},
        [PSMI_HFI_CMD_TID_FREE]         = {HFI1_IOCTL_TID_FREE      , 0},
        [PSMI_HFI_CMD_CREDIT_UPD]       = {HFI1_IOCTL_CREDIT_UPD    , 1},
        [PSMI_HFI_CMD_RECV_CTRL]        = {HFI1_IOCTL_RECV_CTRL     , 1},
        [PSMI_HFI_CMD_POLL_TYPE]        = {HFI1_IOCTL_POLL_TYPE     , 1},
        [PSMI_HFI_CMD_ACK_EVENT]        = {HFI1_IOCTL_ACK_EVENT     , 1},
        [PSMI_HFI_CMD_SET_PKEY]         = {HFI1_IOCTL_SET_PKEY      , 1},
        [PSMI_HFI_CMD_CTXT_RESET]       = {HFI1_IOCTL_CTXT_RESET    , 1},
        [PSMI_HFI_CMD_TID_INVAL_READ]   = {HFI1_IOCTL_TID_INVAL_READ, 0},
        [PSMI_HFI_CMD_GET_VERS]         = {HFI1_IOCTL_GET_VERS      , 1},
#ifdef PSM_CUDA
	[PSMI_HFI_CMD_TID_UPDATE_V2]	= {HFI1_IOCTL_TID_UPDATE_V2 , 0},
#endif
    };

	if (cmd->type < PSMI_HFI_CMD_LAST)
		return psmi_ioctl(fd,
			     cmdTypeToIoctlNum[cmd->type].ioctlCmd,
			     addrOrLiteral[cmdTypeToIoctlNum[cmd->type].addrOrLiteralIdx]);
	else
	{
		errno = EINVAL;
		return -1;
	}
}
#endif /* #ifdef PSM2_SUPPORT_IW_CMD_API */

/* we use mmap64() because we compile in both 32 and 64 bit mode,
   and we have to map physical addresses that are > 32 bits long.
   While linux implements mmap64, it doesn't have a man page,
   and isn't declared in any header file, so we declare it here ourselves.

   We'd like to just use -D_LARGEFILE64_SOURCE, to make off_t 64 bits and
   redirects mmap to mmap64 for us, but at least through suse10 and fc4,
   it doesn't work when the address being mapped is > 32 bits.  It chips
   off bits 32 and above.   So we stay with mmap64. */
void *hfi_mmap64(void *addr, size_t length, int prot, int flags, int fd,
		 __off64_t offset)
{
	return mmap64(addr, length, prot, flags, fd, offset);
}

/* get the number of units supported by the driver.  Does not guarantee */
/* that a working chip has been found for each possible unit #. */
/* number of units >=0 (0 means none found). */
/* formerly used sysfs file "num_units" */
int hfi_get_num_units(void)
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
   returns 0 if no port on the unit is active.
   returns -1 when an error occurred. */
int hfi_get_unit_active(int unit)
{
	int p, lid;

	for (p = HFI_MIN_PORT; p <= HFI_MAX_PORT; p++) {
		lid = hfi_get_port_lid(unit, p);
		if (lid > 0 && lid != 0xFFFF)
			break;
	}

	if (p <= HFI_MAX_PORT)
	{
		return 1;
	}

	return lid;
}

/* get the number of contexts from the unit id. */
/* Returns 0 if no unit or no match. */
int hfi_get_num_contexts(int unit_id)
{
	return HFI_UD_NUM_CONTEXTS;
}

/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active.
   returns -1 when an error occurred. */
int hfi_get_port_active(int unit, int port)
{
	int ret;
	char *state;
	ret = hfi_sysfs_port_read(unit, port, "phys_state", &state);
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
			free(state);
			return 0;
		}
		free(state);
		return 1;
	}
}

/* Given the unit number, return an error, or the corresponding LID
   For now, it's used only so the MPI code can determine it's own
   LID, and which other LIDs (if any) are also assigned to this node
   Returns an int, so -1 indicates an error.  0 may indicate that
   the unit is valid, but no LID has been assigned.
   No error print because we call this for both potential
   ports without knowing if both ports exist (or are connected) */
/* This routine is used in many places, such as get_unit_active, to
 * confirm the port is usable.  As such it includes additional checks that
 * the port is active and for link_layer ethernet that it includes a RoCE
 * IPv4 GID whose subnet can be identified
 */
int hfi_get_port_lid(int unit, int port)
{
	int ret = 0;
	int64_t val = 0;

	if (hfi_get_port_active(unit,port) != 1)
		return -2;
	ret = hfi_sysfs_port_read_s64(unit, port, "lid", &val, 0);
	_HFI_VDBG("ret %d, unit %d port %d lid %ld\n", ret, unit,
		  port, (long int)val);

	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single port chips */
			_HFI_VDBG("Failed to get LID for unit %u:%u: %s\n",
				  unit, port, strerror(errno));
		else
			_HFI_DBG("Failed to get LID for unit %u:%u: %s\n",
				 unit, port, strerror(errno));
	} else {
		char *link_lyr;
		ret = hfi_sysfs_port_read(unit, port, "link_layer", &link_lyr);
		if (ret == -1) {
			if (errno == ENODEV)
				/* this is "normal" for port != 1, on single port chips */
				_HFI_VDBG("Failed to get link_layer for unit %u:%u: %s\n",
					  unit, port, strerror(errno));
			else
				_HFI_DBG("Failed to get link_layer for unit %u:%u: %s\n",
					 unit, port, strerror(errno));
		} else {
			_HFI_VDBG("ret %d, unit %d port %d link_layer %s\n",
					  ret, unit, port, link_lyr);

			/* If this port is an Ethernet Port lid does not matter, return 1 */
			if (strncmp(link_lyr, "Ethernet", strlen("Ethernet")) == 0) {
				uint64_t subnet, hi;
				if (0 != hfi_get_port_subnet(unit, port,
								&subnet, NULL, NULL, NULL, NULL, &hi, NULL)) {
					_HFI_DBG("Failed to get subnet for unit %u:%u: %s\n",
						unit, port, strerror(errno));
					ret = -1;
				} else if (subnet == hi) {
					_HFI_DBG("Skipping unit %u:%u: no RoCE IPv4 GID\n",
						 unit, port);
					ret = -1;
				} else
					ret = 1;	// for RoCE LID does not matter, return 1
			} else
				ret = val;	// OPA/IB LID we got
			free(link_lyr);
		}
	}

	return ret;
}

/* Given the unit number, return an error, or the corresponding GID
   For now, it's used only so the MPI code can determine its fabric ID.
   Returns an int, so -1 indicates an error.
   No error print because we call this for both potential
   ports without knowing if both ports exist (or are connected) */
static int hfi_get_port_gid(int unit, int port, int idx, uint64_t *hi, uint64_t *lo)
{
	int ret;
	char *gid_str = NULL;
	char attr_str[64];

	snprintf(attr_str, 64, "gids/%d", idx < 0 ? 0 : idx);
	ret = hfi_sysfs_port_read(unit, port, attr_str, &gid_str);
	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single
			 * port chips */
			_HFI_VDBG("Failed to get GID for unit %u:%u: %s\n",
				  unit, port, strerror(errno));
		else
			_HFI_DBG("Failed to get GID for unit %u:%u: %s\n",
				 unit, port, strerror(errno));
	} else {
		uint32_t gid[8] = {0};
		if (sscanf(gid_str, "%4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x",
			   &gid[0], &gid[1], &gid[2], &gid[3],
			   &gid[4], &gid[5], &gid[6], &gid[7]) != 8) {
			_HFI_DBG("Failed to parse GID for unit %u:%u: %s\n",
				 unit, port, gid_str);
			ret = -1;
		} else {
			*hi = (((uint64_t) gid[0]) << 48)
				| (((uint64_t) gid[1]) << 32)
				| (((uint64_t) gid[2]) << 16)
				| (((uint64_t) gid[3]) << 0);
			*lo = (((uint64_t) gid[4]) << 48)
				| (((uint64_t) gid[5]) << 32)
				| (((uint64_t) gid[6]) << 16)
				| (((uint64_t) gid[7]) << 0);
		}
		free(gid_str);
	}

	return ret;
}
int hfi_get_unit_cpumask(int unit, cpu_set_t *cpuset)
{
	int ret = -1;
	char *cpulist;

	CPU_ZERO(cpuset);

	ret = hfi_sysfs_unit_read(unit, "device/local_cpulist", &cpulist);
	if (ret == -1) {
		_HFI_VDBG("Failed to get cpu list for unit %u: %s\n",
				  unit, strerror(errno));
	} else {
		int i = 0;
		char *next_comma = NULL;
		char *temp = cpulist;
		char *dash;
		int first = -1, last = -1;

		do {
			next_comma = strchr(temp, ',');
			dash = strchr(temp, '-');

			first = atoi(temp);

			if (dash == NULL || (dash > next_comma && next_comma != NULL)) {
				last = first;
			} else {
				last = atoi(dash + 1);
			}

			for (i = first; i <= last; i++) {
				CPU_SET(i, cpuset);
				ret++;
			}

			temp = next_comma + 1;
		} while (next_comma != NULL);

		free(cpulist);
	}

	return (ret >= 0 ? 0 : -1);
}

/* Given the unit number, return an error, or the corresponding subnet
   For IB/OPA the subnet is the hi 64b of the 1st GID
		addr is the low 64b of the gid, ip_addr and netmask are N/A (0)
   For Ethernet it's the IPv4 subnet derived from the 1st RoCE IPv4 GID
		subnet is the upper portion of the ip_addr (& netmask)
		addr is the lower portion of the ip_addr (& ~netmask)
		and ip_addr and netmask are returned
   In all cases, idx, hi and lo are the actual gid
   All values are in host byte order
   Returns an int, so -1 indicates an error.
   No error print because we call this for both potential
   ports without knowing if both ports exist (or are connected) */
int hfi_get_port_subnet(int unit, int port, uint64_t *subnet, uint64_t *addr,
					uint32_t *ip_addr, uint32_t *netmask,
					int *idx, uint64_t *hi, uint64_t *lo)
{
	int i;
	int have_subnet = 0;
	uint64_t gid_hi, gid_lo;

	for (i =0; ; i++) {
		if (-1 == hfi_get_port_gid(unit, port, i, &gid_hi, &gid_lo))
			break; // stop at 1st non-existent gid (or non-existent port)
		if (gid_lo == 0) // Skip over empty gid table entries.
			continue;
		if (! have_subnet) {
			// save 1st valid gid, this is answer unless we find eth
			if (idx) *idx = i;
			if (subnet) *subnet = gid_hi;
			if (addr) *addr = gid_lo;
			if (ip_addr) *ip_addr = 0;
			if (netmask) *netmask = 0;
			if (hi) *hi = gid_hi;
			if (lo) *lo = gid_lo;
			have_subnet = 1;
		}
		// RoCEv2 Gid => ::ffff:<ipv4>
		if (gid_hi == 0x0 && (gid_lo >> 32) == 0x0000ffff) {
			uint32_t ipaddr = (uint32_t)(gid_lo & 0xffffffff);
			__be32 mask = 0;
			if (!psmi_get_eth_netmask(__cpu_to_be32(ipaddr), &mask)) {
				// stop at 1st valid ethernet gid
				uint32_t nm = __be32_to_cpu(mask);
				if (idx) *idx = i;
				if (subnet) *subnet = ipaddr & nm;
				if (addr) *addr = ipaddr & ~nm;
				if (ip_addr) *ip_addr = ipaddr;
				if (netmask) *netmask = nm;
				if (hi) *hi = gid_hi;
				if (lo) *lo = gid_lo;
				break;
			} else {
				return -1;	// we're stuck, can't figure out netmask
			}
		}
	}
	return (have_subnet?0:-1);
}


/* Given the unit number, return an error, or the corresponding link rate
   for the port */
/* Returns an int, so -1 indicates an error. */
int hfi_get_port_rate(int unit, int port)
{
	int ret;
	double rate;
	char *data_rate = NULL, *newptr;

	ret = hfi_sysfs_port_read(unit, port, "rate", &data_rate);
	if (ret == -1)
		goto get_port_rate_error;
	else {
		rate = strtod(data_rate, &newptr);
		if ((rate == 0) && (data_rate == newptr))
			goto get_port_rate_error;
	}

	free(data_rate);
	return ((int)(rate * 2) >> 1);

get_port_rate_error:
	_HFI_INFO("Failed to get link rate for unit %u:%u: %s\n",
		  unit, port, strerror(errno));

	return ret;
}







