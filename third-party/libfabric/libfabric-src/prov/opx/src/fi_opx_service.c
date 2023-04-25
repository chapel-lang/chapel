/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021-2022 Cornelis Networks.

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
  Copyright(c) 2021-2022 Cornelis Networks.

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
#include <sys/ioctl.h>
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
#include "opa_service.h"
#include "rdma/opx/fi_opx_sysfs.h"

typedef union
{
	struct
	{
		uint16_t minor;
		uint16_t major;
	};
	uint32_t version;
} sw_version_t;

static sw_version_t sw_version =
{
	{
	.major = HFI1_USER_SWMAJOR,
	.minor = HFI1_USER_SWMINOR
	}
};

/*
 * This function is necessary in a udev-based world.  There can be an
 * arbitrarily long (but typically less than one second) delay between
 * a driver getting loaded and any dynamic special files turning up.
 *
 * The timeout is in milliseconds.  A value of zero means "callee
 * decides timeout".  Negative is infinite.
 *
 * Returns 0 on success, -1 on error or timeout.  Check errno to see
 * whether there was a timeout (ETIMEDOUT) or an error (any other
 * non-zero value).
 */
static int opx_hfi_wait_for_device(const char *path, long timeout)
{
	int saved_errno;
	struct stat st;
	long elapsed;
	int ret;

	if (timeout == 0)
		timeout = 15000;

	elapsed = 0;

	while (1) {
		static const long default_ms = 250;
		struct timespec req = { 0 };
		long ms;

		ret = stat(path, &st);
		saved_errno = errno;

		if (ret == 0 || (ret == -1 && errno != ENOENT))
			break;

		if ((timeout > 0) && ((timeout - elapsed) <= 0)) {
			saved_errno = ETIMEDOUT;
			break;
		}

		if (elapsed == 0) {
			if (timeout < 0)
				_HFI_DBG
				    ("Device file %s not present on first check; "
				     "waiting indefinitely...\n", path);
			else
				_HFI_DBG
				    ("Device file %s not present on first check; "
				     "waiting up to %.1f seconds...\n", path,
				     timeout / 1e3);
		}

		if (timeout < 0 || timeout - elapsed >= default_ms)
			ms = default_ms;
		else
			ms = timeout;

		elapsed += ms;
		req.tv_nsec = ms * 1000000;

		ret = nanosleep(&req, NULL);
		saved_errno = errno;

		if (ret == -1)
			break;
	}

	if (ret == 0)
		_HFI_INFO("Found %s after %.1f seconds\n", path, elapsed / 1e3);
	else
		_HFI_INFO
		    ("The %s device failed to appear after %.1f seconds: %s\n",
		     path, elapsed / 1e3, strerror(saved_errno));

	errno = saved_errno;
	return ret;
}

/* fwd declaration */
static int _opx_hfi_cmd_write(int fd, struct hfi1_cmd *cmd, size_t count);

#ifdef PSM2_SUPPORT_IW_CMD_API

/* fwd declaration */
static int _hfi_cmd_ioctl(int fd, struct hfi1_cmd *cmd, size_t count);
/* Function pointer. */
static int (*_hfi_cmd_send)(int fd, struct hfi1_cmd *cmd, size_t count) = _hfi_cmd_ioctl;

#else
/* Function pointer. */
static int (*const _hfi_cmd_send)(int fd, struct hfi1_cmd *cmd, size_t count) = _opx_hfi_cmd_write;
#endif

uint16_t opx_hfi_get_user_major_version(void)
{
	return sw_version.major;
}

void opx_hfi_set_user_major_version(uint16_t major_version)
{
	sw_version.major = major_version;
}

uint16_t opx_hfi_get_user_minor_version(void)
{
	return sw_version.minor;
}

void opx_hfi_set_user_version(uint32_t version)
{
	sw_version.version = version;
}

int opx_hfi_context_open(int unit, int port, uint64_t open_timeout)
{
	char dev_name_ignored[256];

	return opx_hfi_context_open_ex(unit, port, open_timeout,
				   dev_name_ignored, sizeof(dev_name_ignored));
}

int opx_hfi_context_open_ex(int unit, int port, uint64_t open_timeout,
		     char *dev_name,size_t dev_name_len)
{
	int fd;

	if (unit != OPX_UNIT_ID_ANY && unit >= 0)
		snprintf(dev_name, dev_name_len, "%s_%u", OPX_DEVICE_PATH,
			 unit);
	else
		snprintf(dev_name, dev_name_len, "%s_%u", OPX_DEVICE_PATH,
			 0);

	if (opx_hfi_wait_for_device(dev_name, (long)open_timeout) == -1) {
		_HFI_DBG("Could not find an HFI Unit on device "
			 "%s (%lds elapsed)", dev_name,
			 (long)open_timeout / 1000);
		return -1;
	}

	if ((fd = open(dev_name, O_RDWR)) == -1) {
		_HFI_DBG("(host:Can't open %s for reading and writing",
			 dev_name);
		return -1;
	}

	if (fcntl(fd, F_SETFD, FD_CLOEXEC))
		_HFI_INFO("Failed to set close on exec for device: %s\n",
			  strerror(errno));

#ifdef PSM2_SUPPORT_IW_CMD_API
	{
		/* if hfi1DriverMajor == -1, then we are potentially talking to a new driver.
		   Let's confirm by issuing an ioctl version request: */
		struct hfi1_cmd c;

		memset(&c, 0, sizeof(struct hfi1_cmd));
		c.type = OPX_HFI_CMD_GET_VERS;
		c.len  = 0;
		c.addr = 0;

		if (opx_hfi_cmd_write(fd, &c, sizeof(c)) == -1) {
			/* Let's assume that the driver is the old driver */
			opx_hfi_set_user_major_version(IOCTL_CMD_API_MODULE_MAJOR - 1);
			/* the old driver uses write() for its command interface: */
			_hfi_cmd_send = _opx_hfi_cmd_write;
		}
		else
		{
			int major = c.addr >> HFI1_SWMAJOR_SHIFT;
			if (major != opx_hfi_get_user_major_version()) {
					/* If there is a skew between the major version of the driver
					   that is executing and the major version which was used during
					   compilation of PSM, we treat that is a fatal error. */
					_HFI_INFO("PSM2 and driver version mismatch: (%d != %d)\n",
						  major, opx_hfi_get_user_major_version());
				close(fd);
				return -1;
			}
		}
	}

#endif
	return fd;
}

void opx_hfi_context_close(int fd)
{
	(void)close(fd);
}

int opx_hfi_cmd_writev(int fd, const struct iovec *iov, int iovcnt)
{
	return writev(fd, iov, iovcnt);
}

int opx_hfi_cmd_write(int fd, struct hfi1_cmd *cmd, size_t count)
{
	return _hfi_cmd_send(fd, cmd, count);
}

static
int _opx_hfi_cmd_write(int fd, struct hfi1_cmd *cmd, size_t count)
{
    static const unsigned int cmdTypeToWriteNum[OPX_HFI_CMD_LAST] = {
        [OPX_HFI_CMD_ASSIGN_CTXT]      = LEGACY_HFI1_CMD_ASSIGN_CTXT,
        [OPX_HFI_CMD_CTXT_INFO]        = LEGACY_HFI1_CMD_CTXT_INFO,
        [OPX_HFI_CMD_USER_INFO]        = LEGACY_HFI1_CMD_USER_INFO,
        [OPX_HFI_CMD_TID_UPDATE]       = LEGACY_HFI1_CMD_TID_UPDATE,
        [OPX_HFI_CMD_TID_FREE]         = LEGACY_HFI1_CMD_TID_FREE,
        [OPX_HFI_CMD_CREDIT_UPD]       = LEGACY_HFI1_CMD_CREDIT_UPD,
        [OPX_HFI_CMD_RECV_CTRL]        = LEGACY_HFI1_CMD_RECV_CTRL,
        [OPX_HFI_CMD_POLL_TYPE]        = LEGACY_HFI1_CMD_POLL_TYPE,
        [OPX_HFI_CMD_ACK_EVENT]        = LEGACY_HFI1_CMD_ACK_EVENT,
        [OPX_HFI_CMD_SET_PKEY]         = LEGACY_HFI1_CMD_SET_PKEY,
        [OPX_HFI_CMD_CTXT_RESET]       = LEGACY_HFI1_CMD_CTXT_RESET,
        [OPX_HFI_CMD_TID_INVAL_READ]   = LEGACY_HFI1_CMD_TID_INVAL_READ,
        [OPX_HFI_CMD_GET_VERS]         = LEGACY_HFI1_CMD_GET_VERS,
    };

    if (cmd->type < OPX_HFI_CMD_LAST) {
        cmd->type = cmdTypeToWriteNum[cmd->type];

	    return write(fd, cmd, count);
    } else {
        errno = EINVAL;
        return -1;
    }
}

#ifdef PSM2_SUPPORT_IW_CMD_API
static
int _hfi_cmd_ioctl(int fd, struct hfi1_cmd *cmd, size_t count)
{
	uint64_t addrOrLiteral[2] = { (uint64_t)cmd->addr, (uint64_t)&cmd->addr };
	static const struct
	{
		unsigned int ioctlCmd;
		unsigned int addrOrLiteralIdx;
	} cmdTypeToIoctlNum[OPX_HFI_CMD_LAST] = {
        [OPX_HFI_CMD_ASSIGN_CTXT]      = {HFI1_IOCTL_ASSIGN_CTXT   , 0},
        [OPX_HFI_CMD_CTXT_INFO]        = {HFI1_IOCTL_CTXT_INFO     , 0},
        [OPX_HFI_CMD_USER_INFO]        = {HFI1_IOCTL_USER_INFO     , 0},
        [OPX_HFI_CMD_TID_UPDATE]       = {HFI1_IOCTL_TID_UPDATE    , 0},
        [OPX_HFI_CMD_TID_FREE]         = {HFI1_IOCTL_TID_FREE      , 0},
        [OPX_HFI_CMD_CREDIT_UPD]       = {HFI1_IOCTL_CREDIT_UPD    , 1},
        [OPX_HFI_CMD_RECV_CTRL]        = {HFI1_IOCTL_RECV_CTRL     , 1},
        [OPX_HFI_CMD_POLL_TYPE]        = {HFI1_IOCTL_POLL_TYPE     , 1},
        [OPX_HFI_CMD_ACK_EVENT]        = {HFI1_IOCTL_ACK_EVENT     , 1},
        [OPX_HFI_CMD_SET_PKEY]         = {HFI1_IOCTL_SET_PKEY      , 1},
        [OPX_HFI_CMD_CTXT_RESET]       = {HFI1_IOCTL_CTXT_RESET    , 1},
        [OPX_HFI_CMD_TID_INVAL_READ]   = {HFI1_IOCTL_TID_INVAL_READ, 0},
        [OPX_HFI_CMD_GET_VERS]         = {HFI1_IOCTL_GET_VERS      , 1},
#ifdef PSM_CUDA
	[OPX_HFI_CMD_TID_UPDATE_V2]	= {HFI1_IOCTL_TID_UPDATE_V2 , 0},
#endif
    };
        _HFI_INFO("command OPX_HFI_CMD %#X, HFI1_IOCTL %#X\n",cmd->type, cmdTypeToIoctlNum[cmd->type].ioctlCmd);
	if (cmd->type < OPX_HFI_CMD_LAST)
		return ioctl(fd,
			     cmdTypeToIoctlNum[cmd->type].ioctlCmd,
			     addrOrLiteral[cmdTypeToIoctlNum[cmd->type].addrOrLiteralIdx]);
	else
	{
		_HFI_ERROR("EINVAL\n");
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
void *opx_hfi_mmap64(void *addr, size_t length, int prot, int flags, int fd,
		 __off64_t offset)
{
	return mmap64(addr, length, prot, flags, fd, offset);
}

/* get the number of units supported by the driver.  Does not guarantee */
/* that a working chip has been found for each possible unit #. */
/* number of units >=0 (0 means none found). */
/* formerly used sysfs file "num_units" */
int opx_hfi_get_num_units(void)
{
	int ret;

	for (ret = 0;; ret++) {
		char pathname[PATH_MAX];
		struct stat st;
		int r;

		snprintf(pathname, sizeof(pathname), OPX_DEVICE_PATH "_%d", ret);
		if (ret == 0)
			/* We only wait for the first device to come up.  Not
			   on subsequent devices in order to save time. */
			r = opx_hfi_wait_for_device(pathname, 0);
		else
			r = stat(pathname, &st);
		if (!r)
			continue;
		else
			break;
	}

	return ret;
}

/* Given a unit number, returns 1 if any port on the unit is active.
   returns 0 if no port on the unit is active. */
int opx_hfi_get_unit_active(int unit)
{
	int p,rv;

	for (p = OPX_MIN_PORT; p <= OPX_MAX_PORT; p++)
		if ((rv=opx_hfi_get_port_lid(unit, p)) > 0)
			break;

	if (p <= OPX_MAX_PORT)
	{
		return 1;
	}

	return (rv>0);
}

/* get the number of contexts from the unit id. */
/* Returns 0 if no unit or no match. */
int opx_hfi_get_num_contexts(int unit_id)
{
	int n = 0;
	int units;
	int64_t val;
	uint32_t p = OPX_MIN_PORT;

	units = opx_hfi_get_num_units();

	if_pf(units <=  0)
		return 0;

	if (unit_id == OPX_UNIT_ID_ANY) {
		uint32_t u;

		for (u = 0; u < units; u++) {
			for (p = OPX_MIN_PORT; p <= OPX_MAX_PORT; p++)
				if (opx_hfi_get_port_lid(u, p) > 0)
					break;

			if (p <= OPX_MAX_PORT &&
			    !opx_sysfs_unit_read_s64(u, "nctxts", &val, 0))
				n += (uint32_t) val;
		}
	} else {
		for (; p <= OPX_MAX_PORT; p++)
			if (opx_hfi_get_port_lid(unit_id, p) > 0)
				break;

		if (p <= OPX_MAX_PORT &&
		    !opx_sysfs_unit_read_s64(unit_id, "nctxts", &val, 0))
			n += (uint32_t) val;
	}

	return n;
}

/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active.
   returns -1 when an error occurred. */
int opx_hfi_get_port_active(int unit, int port)
{
	int ret;
	char *state;

	ret = opx_sysfs_port_read(unit, port, "state", &state);
	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single port chips */
			_HFI_VDBG
			    ("Failed to get logical link state for unit %u:%u: %s\n",
			     unit, port, strerror(errno));
		else
			_HFI_DBG
			    ("Failed to get logical link state for unit %u:%u: %s\n",
			     unit, port, strerror(errno));
		return -1;
	} else {
		if (strncmp(state, "4: ACTIVE", 9)) {
			_HFI_DBG("Link is not Active for unit %u:%u: %s\n", unit, port, state);
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
int opx_hfi_get_port_lid(int unit, int port)
{
	int ret;
	int64_t val;

	if (opx_hfi_get_port_active(unit,port) != 1)
		return -2;
	ret = opx_sysfs_port_read_s64(unit, port, "lid", &val, 0);
	_HFI_VDBG("opx_hfi_get_port_lid: ret %d, unit %d port %d\n", ret, unit,
		  port);

	if (ret == -1) {
		if (errno == ENODEV)
			/* this is "normal" for port != 1, on single port chips */
			_HFI_VDBG("Failed to get LID for unit %u:%u: %s\n",
				  unit, port, strerror(errno));
		else
			_HFI_DBG("Failed to get LID for unit %u:%u: %s\n",
				 unit, port, strerror(errno));
	} else {
		ret = val;
	}

	return ret;
}

/* Given the unit number, return an error, or the corresponding GID
   For now, it's used only so the MPI code can determine its fabric ID.
   Returns an int, so -1 indicates an error.
   No error print because we call this for both potential
   ports without knowing if both ports exist (or are connected) */
int opx_hfi_get_port_gid(int unit, int port, uint64_t *hi, uint64_t *lo)
{
	int ret;
	char *gid_str = NULL;

	ret = opx_sysfs_port_read(unit, port, "gids/0", &gid_str);

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
		int gid[8];
		if (sscanf(gid_str, "%4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x",
			   &gid[0], &gid[1], &gid[2], &gid[3],
			   &gid[4], &gid[5], &gid[6], &gid[7]) != 8) {
			_HFI_DBG("Failed to parse GID for unit %u:%u: %s\n",
				 unit, port, gid_str);
			ret = -1;
		} else {
			*hi = (((uint64_t) gid[0]) << 48) | (((uint64_t) gid[1])
							     << 32) |
			    (((uint64_t)
			      gid[2]) << 16) | (((uint64_t) gid[3]) << 0);
			*lo = (((uint64_t) gid[4]) << 48) | (((uint64_t) gid[5])
							     << 32) |
			    (((uint64_t)
			      gid[6]) << 16) | (((uint64_t) gid[7]) << 0);
		}
		free(gid_str);
	}

	return ret;
}

/* Given the unit number, return an error, or the corresponding LMC value
   for the port */
/* Returns an int, so -1 indicates an error.  0 */
int opx_hfi_get_port_lmc(int unit, int port)
{
	int ret;
	int64_t val;

	ret = opx_sysfs_port_read_s64(unit, port, "lid_mask_count", &val, 0);

	if (ret == -1) {
		_HFI_INFO("Failed to get LMC for unit %u:%u: %s\n",
			  unit, port, strerror(errno));
	} else
		ret = val;

	return ret;
}

/* Given the unit number, return an error, or the corresponding link rate
   for the port */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_rate(int unit, int port)
{
	int ret;
	double rate;
	char *data_rate = NULL, *newptr;

	ret = opx_sysfs_port_read(unit, port, "rate", &data_rate);
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

/* Given a unit, port and SL, return an error, or the corresponding SC for the
   SL as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_sl2sc(int unit, int port, int sl)
{
	int ret;
	int64_t val;
	char sl2scpath[16];

	snprintf(sl2scpath, sizeof(sl2scpath), "sl2sc/%d", sl);
	ret = opx_sysfs_port_read_s64(unit, port, sl2scpath, &val, 0);

	if (ret == -1) {
		_HFI_DBG
		    ("Failed to get SL2SC mapping for SL %d unit %u:%u: %s\n",
		     sl, unit, port, strerror(errno));
	} else
		ret = val;

	return ret;
}

/* Given a unit, port and SC, return an error, or the corresponding VL for the
   SC as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_sc2vl(int unit, int port, int sc)
{
	int ret;
	int64_t val;
	char sc2vlpath[16];

	snprintf(sc2vlpath, sizeof(sc2vlpath), "sc2vl/%d", sc);
	ret = opx_sysfs_port_read_s64(unit, port, sc2vlpath, &val, 0);

	if (ret == -1) {
		_HFI_DBG
		    ("Failed to get SC2VL mapping for SC %d unit %u:%u: %s\n",
		     sc, unit, port, strerror(errno));
	} else
		ret = val;

	return ret;
}

/* Given a unit, port and VL, return an error, or the corresponding MTU for the
   VL as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_vl2mtu(int unit, int port, int vl)
{
	int ret;
	int64_t val;
	char vl2mtupath[16];

	snprintf(vl2mtupath, sizeof(vl2mtupath), "vl2mtu/%d", vl);
	ret = opx_sysfs_port_read_s64(unit, port, vl2mtupath, &val, 0);

	if (ret == -1) {
		_HFI_DBG
		    ("Failed to get VL2MTU mapping for VL %d unit %u:%u: %s\n",
		     vl, unit, port, strerror(errno));
	} else
		ret = val;

	return ret;
}

/* Given a unit, port and index, return an error, or the corresponding pkey
   value for the index as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_index2pkey(int unit, int port, int index)
{
	int ret;
	int64_t val;
	char index2pkeypath[16];

	snprintf(index2pkeypath, sizeof(index2pkeypath), "pkeys/%d", index);
	ret = opx_sysfs_port_read_s64(unit, port, index2pkeypath, &val, 0);

	if (ret == -1) {
		_HFI_DBG
		    ("Failed to get index2pkey mapping for index %d unit %u:%u: %s\n",
		     index, unit, port, strerror(errno));
	} else
		ret = val;

	return ret;
}

/* These have been fixed to read the values, but they are not
 * compatible with the hfi driver, they return new info with
 * the qib driver
 */
int opx_hfi_get_cc_settings_bin(int unit, int port, char *ccabuf)
{
	int fd;
	size_t count;
/*
 * Check qib driver CCA setting, and try to use it if available.
 * Fall to self CCA setting if errors.
 */
	sprintf(ccabuf, OPX_CLASS_PATH "_%d/ports/%d/CCMgtA/cc_settings_bin",
		unit, port);
	fd = open(ccabuf, O_RDONLY);
	if (fd < 0) {
		return 0;
	}
	/*
	 * 4 bytes for 'control map'
	 * 2 bytes 'port control'
	 * 32 (#SLs) * 6 bytes 'congestion setting' (per-SL)
	 */
	count = 4 + 2 + (32 * 6);
	if (read(fd, ccabuf, count) != count) {
		_HFI_CCADBG("Read cc_settings_bin failed. using static CCA\n");
		close(fd);
		return 0;
	}

	close(fd);

	return 1;
}

int opx_hfi_get_cc_table_bin(int unit, int port, uint16_t **cctp)
{
	int i;
	unsigned short ccti_limit;
	uint16_t *cct;
	int fd;
	char pathname[256];

	*cctp = NULL;
	sprintf(pathname, OPX_CLASS_PATH "_%d/ports/%d/CCMgtA/cc_table_bin",
		unit, port);
	fd = open(pathname, O_RDONLY);
	if (fd < 0) {
		_HFI_CCADBG("Open cc_table_bin failed. using static CCA\n");
		return 0;
	}
	if (read(fd, &ccti_limit, sizeof(ccti_limit)) != sizeof(ccti_limit)) {
		_HFI_CCADBG("Read ccti_limit failed. using static CCA\n");
		close(fd);
		return 0;
	}

	_HFI_CCADBG("ccti_limit = %d\n", ccti_limit);

	if (ccti_limit < 63) {
		_HFI_CCADBG("Read ccti_limit %d not in range [63, 65535], "
			    "using static CCA.\n", ccti_limit);
		close(fd);
		return 0;
	}

	i = (ccti_limit + 1) * sizeof(uint16_t);
	cct = malloc(i);
	if (!cct) {
		close(fd);
		return -1;
	}
	if (read(fd, cct, i) != i) {
		_HFI_CCADBG("Read ccti_entry_list, using static CCA\n");
		free(cct);
		close(fd);
		return 0;
	}

	close(fd);

	_HFI_CCADBG("cct[0] = 0x%04x\n", cct[0]);

	*cctp = cct;
	return ccti_limit;
}

/*
 * This is for diag function hfi_wait_for_packet() only
 */
int opx_hfi_cmd_wait_for_packet(int fd)
{
	int ret;
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;

	ret = poll(&pfd, 1, 500 /* ms */);

	return ret;
}

/* 
 * A fast check to count the number of hfi1 devs.
 * Device(s) may not be usable, more checking is needed
 * returns number of hfi1 devices found in /dev (0 means none)
 */
int opx_hfi_get_hfi1_count() {
	struct stat st;
	int ret;
	char hfi1_pathname[256];
	int hfi1_count = 0;

	for (int i = 0; i < FI_OPX_MAX_HFIS; i++) {
		snprintf(hfi1_pathname, sizeof(hfi1_pathname), "%s_%u", 
			OPX_DEVICE_PATH, i);
		ret = stat(hfi1_pathname, &st);
		if (!ret) {
			//TODO add additional check opx_hfi_get_unit_active
			hfi1_count++;
		}
	}
	return hfi1_count;
}
