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

/* This file contains a simple sysfs interface used by the low level
   hfi protocol code.  It also implements the interface to hfifs. */

/* Many of these routines are used during HAL initialization and
 * HAL discovery/selection.  As such these routines cannot call the HAL
 * via any psmi_hal_* macros.
 *
 * Due to their low level nature they are excluded from use of the
 * malloc/free HEAP debug mechanisms found elsewhere in PSM
 */

#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dirent.h>
#include "psm_config.h"

#include "utils_sysfs.h"
#include "utils_env.h"

#ifdef free
#error "heap debugging macros not allowed here, check header inclusion"
#endif

static char psm3_sysfs_paths[PSMI_MAX_RAILS][PATH_MAX];
static psm3_port_path_type psm3_sysfs_port_path_fmt;
static int  psm3_sysfs_path_count = -1;
static long psm3_sysfs_page_size;

static int filter_dir(const struct dirent *item) {
	if (item->d_name[0] == '.') return 0;
	return 1;
}

// Scans nic_class_path andi populates psm3_sysfs_paths[] for use in subsqeuent
// calls to sysfs functions in this file.
// returns -1 if no devices found, 0 if at least 1 device found
// sysfs can only be initialized for a single HAL at a time and retains
// the supplied path and path_fmt until the next time sysfs_init or sysfs_fini
// is called.
int psm3_sysfs_init(const char *nic_class_path, const psm3_port_path_type port_path_fmt)
{
	char *sysfs_path_env;
	union psmi_envvar_val envval;

	// We need explicit HAL selection when explicit device selected.
	// User may get undefined results if they use wildcards in PSM3_HAL.
	// This is an undocumented feature for rare use cases with
	// instruction from an expert.
	if (psm3_env_get("PSM3_HAL") &&
		! psm3_getenv("PSM3_SYSFS_PATH",
				"Directory to use for information on a single NIC to use (instead of /sys/class/....), can use to workaround incomplete or incorrect /sys/class information",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
				(union psmi_envvar_val)"", &envval))
	{
		sysfs_path_env = envval.e_str;
		// exact path to 1 device provided, only consider it
		snprintf(psm3_sysfs_paths[0], PATH_MAX, "%s", sysfs_path_env);
		psm3_sysfs_path_count = 1;
	} else {
		// we always start over every time called
		struct dirent **d = NULL;
		int i, n = scandir(nic_class_path, &d, filter_dir, alphasort);
		memset(psm3_sysfs_paths, 0, sizeof(psm3_sysfs_paths));
		psm3_sysfs_path_count = 0;
		for (i = 0; i < n; i++) {
			if (d[i] != NULL) {
				if (psm3_sysfs_path_count < PSMI_MAX_RAILS) {
					struct stat s;
					snprintf(psm3_sysfs_paths[psm3_sysfs_path_count], PATH_MAX, "%s/%s", nic_class_path, d[i]->d_name);
					if (stat(psm3_sysfs_paths[psm3_sysfs_path_count], &s) || !S_ISDIR(s.st_mode)) {
						memset(psm3_sysfs_paths[psm3_sysfs_path_count], 0, PATH_MAX);
					} else {
						psm3_sysfs_path_count++;
					}
				} else {
					_HFI_INFO("Max %s device count (%d) reached: Skipping %s\n", nic_class_path, PSMI_MAX_RAILS, d[i]->d_name);
				}
				free(d[i]);
				d[i] = NULL;
			}
		}
		if (d) free(d);
	}

	// for psm3_sysfs_port_open construction of path to port attr
	psm3_sysfs_port_path_fmt = port_path_fmt;


	if (!psm3_sysfs_page_size)
		psm3_sysfs_page_size = sysconf(_SC_PAGESIZE);

	if (_HFI_DBG_ON) {
		int i;
		_HFI_DBG("Found %u devices:\n", psm3_sysfs_path_count);
		for (i = 0; i < psm3_sysfs_path_count; i++) {
			_HFI_DBG(" Device[%u]: %s\n", i, psm3_sysfs_paths[i]);
		}
	}


	return psm3_sysfs_path_count >= 1 ? 0 : -1;
}

void psm3_sysfs_fini(void)
{
	memset(psm3_sysfs_paths, 0, sizeof(psm3_sysfs_paths));
	psm3_sysfs_path_count = -1;
}

const char *psm3_sysfs_unit_path(int unit_id)
{
	if (unit_id >= 0 && unit_id < psm3_sysfs_path_count && unit_id < PSMI_MAX_RAILS) {
		return psm3_sysfs_paths[unit_id];
	}
	return NULL;
}

const char *psm3_sysfs_unit_dev_name(int unit_id)
{
	if (unit_id >= 0 && unit_id < psm3_sysfs_path_count && unit_id < PSMI_MAX_RAILS) {
		char *dev_name = strrchr(psm3_sysfs_paths[unit_id], '/');
		if (dev_name && *dev_name)
			return dev_name+1;
	}
	return "";	// make it easier to use in output messages
}

// accepts a unit number (>=0) or a case insenstive unit name
// there must be no trailing whitespace
// will accept unit number in decimal or hex (0x prefix required)
int psm3_sysfs_find_unit(const char *name)
{
	int i;
	long unit;
	char *end;

	if (! name || ! *name)
		return -1;

	// unit specified by name
	for (i=0; i< psm3_sysfs_path_count; i++) {
		const char *dev_name = psm3_sysfs_unit_dev_name(i);
		if (dev_name && *dev_name && 0 == strcasecmp(dev_name, name))
			return i;
	}

	// unit specified by number
	unit = strtol(name, &end, 10);
	if (end == NULL || *end != 0) {
		unit = strtol(name, &end, 16);
		if (end == NULL || *end != 0)
			return -1;
	}
	if (unit >= 0 && unit < psm3_sysfs_path_count)
		return unit;

	// invalid
	return -1;
}


static int psm3_sysfs_unit_open(uint32_t unit, const char *attr, int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = psm3_sysfs_unit_path(unit);

	if (unitpath == NULL) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d: %s\n", attr,
			 unit, "unit id not valid");
		return -1;
	}

	snprintf(buf, sizeof(buf), "%s/%s", unitpath, attr);
	fd = open(buf, flags);
	saved_errno = errno;

	if (fd == -1) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d: %s\n", attr,
			 unit, strerror(errno));
		_HFI_DBG("Offending file name: %s\n", buf);
	} else {
		_HFI_DBG("Opened %s\n", buf);
	}

	errno = saved_errno;
	return fd;
}

static int psm3_sysfs_unit_open_for_node(uint32_t unit, int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = psm3_sysfs_unit_path(unit);

	if (unitpath == NULL) {
		_HFI_DBG("Failed to open attribute 'numa_node' of unit %d: %s\n",
			 unit, "unit id not valid");
		return -1;
	}

	snprintf(buf, sizeof(buf), "%s/device/numa_node", unitpath);
	fd = open(buf, flags);
	saved_errno = errno;

	if (fd == -1) {
		_HFI_DBG("Failed to open attribute 'numa_node' of unit %d: %s\n",
			 unit, strerror(errno));
		_HFI_DBG("Offending file name: %s\n", buf);
	} else {
		_HFI_DBG("Opened %s\n", buf);
	}

	errno = saved_errno;
	return fd;
}

static int psm3_sysfs_port_open(uint32_t unit, uint32_t port, const char *attr,
			int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = psm3_sysfs_unit_path(unit);
	int ret;

	if (unitpath == NULL) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d: %s\n", attr,
			 unit, "unit id not valid");
		errno = EINVAL;
		return -1;
	}
	switch (psm3_sysfs_port_path_fmt) {
	case PSM3_PORT_PATH_TYPE_IB:
		ret = snprintf(buf, sizeof(buf), "%s/ports/%u/%s",
			unitpath, port, attr);
		break;
	case PSM3_PORT_PATH_TYPE_NO_PORT:
		ret = snprintf(buf, sizeof(buf), "%s/%s", unitpath, attr);
		break;
	default:
		ret = -1;
	}
	if (ret < 0 || ret >= sizeof(buf)) {
		errno = EINVAL;
		return -1;
	}
	fd = open(buf, flags);
	saved_errno = errno;

	if (fd == -1) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d:%d: %s\n",
			 attr, unit, port, strerror(errno));
		_HFI_DBG("Offending file name: %s\n", buf);
	} else {
		_HFI_DBG("Opened %s\n", buf);
	}

	errno = saved_errno;
	return fd;
}


static int read_page(int fd, char **datap)
{
	char *data = NULL;
	int saved_errno;
	int ret = -1;

	data = malloc(psm3_sysfs_page_size);
	saved_errno = errno;

	if (!data) {
		_HFI_DBG("Could not allocate memory: %s\n", strerror(errno));
		goto bail;
	}

	ret = read(fd, data, psm3_sysfs_page_size);
	saved_errno = errno;

	if (ret < 0) {
		_HFI_DBG("Read of attribute failed: %s\n", strerror(errno));
		goto bail;
	}

bail:
	if (ret < 0) {
		free(data);
	} else {
		if (ret < psm3_sysfs_page_size)
			data[ret] = 0;
		else
			data[psm3_sysfs_page_size-1] = 0;
		_HFI_DBG("Read: %s\n", data);
		*datap = data;
	}

	errno = saved_errno;
	return ret;
}

/* free data allocated by read_page or any of the other functions in this
 * file which use it
 */
void psm3_sysfs_free(char *data)
{
	free(data);
}

/*
 * On return, caller must free *datap via psm3_sysfs_free
 */
int psm3_sysfs_unit_read(uint32_t unit, const char *attr, char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = psm3_sysfs_unit_open(unit, attr, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	saved_errno = errno;

bail:
	if (ret < 0)
		*datap = NULL;

	if (fd != -1) {
		close(fd);
	}

	errno = saved_errno;
	return ret;
}

#if 0
/* read a string value directly into buff, no more than size bytes.
   returns the number of bytes read */
size_t psm3_sysfs_unit_port_read(uint32_t unit, uint32_t port, const char *attr,
			char *buff, size_t size)
{
	int fd = -1;
	size_t rv = -1;

	fd = psm3_sysfs_port_open(unit, port, attr, O_RDONLY);

	if (fd == -1)
		return rv;

	rv = read(fd, buff, size);

	close(fd);

	if (rv < 0)
		_HFI_DBG("Read of attribute failed: %s\n", strerror(errno));
	else if (rv < size)
		buff[rv] = 0;
	else
		buff[size-1] = 0;

	return rv;
}
#endif

/*
 * On return, caller must free *datap via psm3_sysfs_free
 */
int psm3_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr,
			char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = psm3_sysfs_port_open(unit, port, attr, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	saved_errno = errno;

bail:
	if (ret < 0)
		*datap = NULL;

	if (fd != -1) {
		close(fd);
	}

	errno = saved_errno;
	return ret;
}


int psm3_sysfs_unit_read_s64(uint32_t unit, const char *attr,
			    int64_t *valp, int base)
{
	char *data=NULL, *end;
	int saved_errno;
	long long val;
	int ret;

	ret = psm3_sysfs_unit_read(unit, attr, &data);
	saved_errno = errno;

	if (ret == -1) {
		goto bail;
	}

	val = strtoll(data, &end, base);
	saved_errno = errno;

	if (!*data || !(*end == '\0' || isspace(*end))) {
		ret = -1;
		goto bail;
	}

	*valp = val;
	ret = 0;

bail:
	if (data)
		psm3_sysfs_free(data);
	errno = saved_errno;
	return ret;
}

static int psm3_sysfs_unit_read_node(uint32_t unit, char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = psm3_sysfs_unit_open_for_node(unit, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	if (ret < 0)
		*datap = NULL;

	saved_errno = errno;
	close(fd);
bail:
	errno = saved_errno;
	return ret;
}

int64_t psm3_sysfs_unit_read_node_s64(uint32_t unit)
{
	char *data=NULL, *end;
	int saved_errno;
	long long val;
	int64_t ret = -1;

	saved_errno = errno;
	if (psm3_sysfs_unit_read_node(unit, &data) == -1) {
		goto bail;
	}

	val = strtoll(data, &end, 0);
	saved_errno = errno;

	if (!*data || !(*end == '\0' || isspace(*end))) {
		ret = -1;
		goto bail;
	}

	ret = (int64_t) val;
bail:
	psm3_sysfs_free(data);
	errno = saved_errno;
	return ret;
}

int psm3_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr,
			    int64_t *valp, int base)
{
	char *data, *end;
	int saved_errno;
	long long val;
	int ret;

	ret = psm3_sysfs_port_read(unit, port, attr, &data);
	saved_errno = errno;

	if (ret == -1) {
		goto bail;
	}

	val = strtoll(data, &end, base);
	saved_errno = errno;

	if (!*data || !(*end == '\0' || isspace(*end))) {
		ret = -1;
		goto bail;
	}

	*valp = val;
	ret = 0;

bail:
	psm3_sysfs_free(data);
	errno = saved_errno;
	return ret;
}

int psm3_sysfs_get_unit_cpumask(int unit, cpu_set_t *cpuset)
{
	int ret = -1;
	char *cpulist;

	CPU_ZERO(cpuset);

	ret = psm3_sysfs_unit_read(unit, "device/local_cpulist", &cpulist);
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
			if (first < 0 || first > CPU_SETSIZE) {
				_HFI_VDBG("Failed to parse cpulist: %s\n", cpulist);
				ret = -1;
				break;
			}

			if (dash == NULL || (dash > next_comma && next_comma != NULL)) {
				last = first;
			} else {
				last = atoi(dash + 1);
				if (last < 0 || last > CPU_SETSIZE) {
					_HFI_VDBG("Failed to parse cpulist: %s\n", cpulist);
					ret = -1;
					break;
				}
			}

			for (i = first; i <= last; i++) {
				CPU_SET(i, cpuset);
				ret++;
			}

			temp = next_comma + 1;
		} while (next_comma != NULL);

		psm3_sysfs_free(cpulist);
	}

	return (ret >= 0 ? 0 : -1);
}

int psm3_sysfs_get_unit_pci_bus(int unit, uint32_t *domain, uint32_t *bus,
					uint32_t *device, uint32_t *function)
{
	const char *unitpath = psm3_sysfs_unit_path(unit);
	if (unitpath == NULL) {
		_HFI_DBG("Failed to get deivce for unit %d\n", unit);
		return -1;
	}

	char *path = NULL;
	if (asprintf(&path, "%s/device", unitpath) < 0 || path == NULL) {
		_HFI_DBG("Failed to get sub device for unit %d: %s\n", unit, unitpath);
		return -1;
	}
	int ret = 0;

	char *rpath = NULL;
	if (NULL == (rpath = realpath(path, NULL))) {
		_HFI_DBG("Failed to resolve path of sub device for unit %d: %s\n",
			unit, path);
		ret = -1;
		goto free;
	}

	char *pci = strrchr(rpath, '/');
	if (NULL == pci) {
		_HFI_DBG("Failed to find pci bus in realpath for unit %d: %s\n",
			unit, rpath);
		ret = -1;
		goto free;
	}
	pci++; // move passed '/'

	uint32_t a = 0, b = 0, c = 0, d = 0;
	int n = sscanf(pci, "%x:%x:%x.%x", &a, &b, &c, &d);
	if (n != 4) {
		_HFI_DBG("Failed to parse pci bus in realpath for unit %d: %s\n",
			unit, rpath);
		ret = -1;
		goto free;
	}
	if (domain)     *domain = a;
	if (bus)           *bus = b;
	if (device)     *device = c;
	if (function) *function = d;

free:
	if (rpath) free(rpath);
	free(path);
	return ret;
}

int psm3_sysfs_get_unit_device_id(int unit, char *buf, size_t bufsize)
{
	char *device = NULL;

	int ret = psm3_sysfs_unit_read(unit, "device/device", &device);
	if (ret == -1) {
		_HFI_DBG("Failed to get device id for unit %u: %s\n",
			unit, strerror(errno));
		return ret;
	}
	char *nl = strchr(device, '\n');
	if (nl) *nl = '\0';
	ret = snprintf(buf, bufsize, "%s", device);
	_HFI_VDBG("Got device id for unit %u: %s [%zu]\n", unit, buf, bufsize);

	psm3_sysfs_free(device);

	return ret < 0 ? -1 : 0;
}

int psm3_sysfs_get_unit_device_version(int unit, char *buf, size_t bufsize)
{
	char *device = NULL;

	int ret = psm3_sysfs_unit_read(unit, "device/revision", &device);
	if (ret == -1) {
		// in some virtualized environments the file may not be present
		_HFI_DBG("Failed to get device version for unit %u: %s: Using 'none'\n",
			unit, strerror(errno));
		ret = snprintf(buf, bufsize, "none");
		return ret < 0 ? -1 : 0;
	}
	char *nl = strchr(device, '\n');
	if (nl) *nl = '\0';
	ret = snprintf(buf, bufsize, "%s", device);
	_HFI_VDBG("Got device version for unit %u: %s [%zu]\n", unit, buf, bufsize);

	psm3_sysfs_free(device);

	return ret < 0 ? -1 : 0;
}

int psm3_sysfs_get_unit_vendor_id(int unit, char *buf, size_t bufsize)
{
	char *device = NULL;

	int ret = psm3_sysfs_unit_read(unit, "device/vendor", &device);
	if (ret == -1) {
		_HFI_DBG("Failed to get vendor id for unit %u: %s\n",
			unit, strerror(errno));
		return ret;
	}
	char *nl = strchr(device, '\n');
	if (nl) *nl = '\0';
	ret = snprintf(buf, bufsize, "%s", device);
	_HFI_VDBG("Got vendor id for unit %u: %s [%zu]\n", unit, buf, bufsize);

	psm3_sysfs_free(device);

	return ret < 0 ? -1 : 0;
}

int psm3_sysfs_get_unit_driver(int unit, char *buf, size_t bufsize)
{
	const char *unitpath = psm3_sysfs_unit_path(unit);
	if (unitpath == NULL) {
		_HFI_DBG("Failed to get deivce for unit %d\n", unit);
		return -1;
	}

	char *path = NULL;
	if (asprintf(&path, "%s/device/driver", unitpath) < 0) {
		_HFI_DBG("Failed to get driver path for unit %d: %s\n", unit, unitpath);
		return -1;
	}
	int ret = 0;

	char *rpath = NULL;
	if (NULL == (rpath = realpath(path, NULL))) {
		_HFI_DBG("Failed to resolve path of driver for unit %d: %s\n",
			unit, path);
		ret = -1;
		goto free;
	}

	char *driver = strrchr(rpath, '/');
	if (NULL == driver) {
		_HFI_DBG("Failed to find driver in realpath for unit %d: %s\n",
			unit, rpath);
		ret = -1;
		goto free;
	}
	driver++; // move passed '/'
	ret = snprintf(buf, bufsize, "%s", driver);

free:
	if (rpath) free(rpath);
	free(path);

	return ret < 0 ? -1 : 0;
}
