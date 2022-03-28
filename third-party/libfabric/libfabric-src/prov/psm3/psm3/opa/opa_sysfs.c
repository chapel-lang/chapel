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

#include "opa_service.h"

static char sysfs_paths[PSMI_MAX_RAILS][PATH_MAX];
static int  sysfs_path_count = -1;
static long sysfs_page_size;
#define SYSFS_DIR "/sys/class/infiniband/"

int filter_dir(const struct dirent *item) {
	if (item->d_name[0] == '.') return 0;
	return 1;
}

int sysfs_init(const char *dflt_hfi_class_path)
{
	char *hfi_env;

	if (NULL != (hfi_env = getenv("PSM3_SYSFS_PATH")))
	{
		snprintf(sysfs_paths[0], PATH_MAX, "%s", hfi_env);
		sysfs_path_count = 1;
	}
	if (sysfs_path_count < 1) {
		struct dirent **d = NULL;
		int i, n = scandir(SYSFS_DIR, &d, filter_dir, alphasort);
		sysfs_path_count = 0;
		for (i = 0; i < n; i++) {
			if (d[i] != NULL) {
				if (sysfs_path_count < PSMI_MAX_RAILS) {
					struct stat s;
					snprintf(sysfs_paths[sysfs_path_count], PATH_MAX, SYSFS_DIR "%s", d[i]->d_name);
					if (stat(sysfs_paths[sysfs_path_count], &s) || !S_ISDIR(s.st_mode)) {
						memset(sysfs_paths[sysfs_path_count], 0, PATH_MAX);
					} else {
						sysfs_path_count++;
					}
				} else {
					_HFI_INFO("Max " SYSFS_DIR " device count (%d) reached: Skipping %s\n", PSMI_MAX_RAILS, d[i]->d_name);
				}
				free(d[i]);
			}
		}
		if (d) free(d);
	}


	if (!sysfs_page_size)
		sysfs_page_size = sysconf(_SC_PAGESIZE);

	if (_HFI_DBG_ON) {
		int i;
		_HFI_DBG("Found %u devices:\n", sysfs_path_count);
		for (i = 0; i < sysfs_path_count; i++) {
			_HFI_DBG(" Device[%u]: %s\n", i, sysfs_paths[i]);
		}
	}


	return sysfs_path_count >= 1 ? 0 : -1;
}

void sysfs_fini(void)
{
	memset(sysfs_paths, 0, sizeof(sysfs_paths));
	sysfs_path_count = -1;
}

const char *sysfs_unit_path(int unit_id)
{
	if (sysfs_path_count > 0 && unit_id < sysfs_path_count) {
		return sysfs_paths[unit_id];
	}
	return NULL;
}

const char *sysfs_unit_dev_name(int unit_id)
{
	if (unit_id >= 0 && unit_id < sysfs_path_count) {
		char *dev_name = strrchr(sysfs_paths[unit_id], '/');
		if (dev_name && *dev_name)
			return dev_name+1;
	}
	return "";	// make it easier to use in output messages
}

// accepts a unit number (>=0) or a case insenstive unit name
// there must be no trailing whitespace
// will accept unit number in decimal or hex (0x prefix required)
int sysfs_find_unit(const char *name)
{
	int i;
	long unit;
	char *end;

	if (! name || ! *name)
		return -1;

	// unit specified by name
	for (i=0; i< sysfs_path_count; i++) {
		const char *dev_name = sysfs_unit_dev_name(i);
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
	if (unit >= 0 && unit < sysfs_path_count)
		return unit;

	// invalid
	return -1;
}


int hfi_sysfs_unit_open(uint32_t unit, const char *attr, int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = sysfs_unit_path(unit);

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
	}

	errno = saved_errno;
	return fd;
}

static int hfi_sysfs_unit_open_for_node(uint32_t unit, int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = sysfs_unit_path(unit);

	if (unitpath == NULL) {
		_HFI_DBG("Failed to open attribute numa_node of unit %d: %s\n",
			 unit, "unit id not valid");
		return -1;
	}

	snprintf(buf, sizeof(buf), "%s/device/numa_node", unitpath);
	fd = open(buf, flags);
	saved_errno = errno;

	if (fd == -1) {
		_HFI_DBG("Failed to open attribute numa_node of unit %d: %s\n",
			 unit, strerror(errno));
		_HFI_DBG("Offending file name: %s\n", buf);
	}

	errno = saved_errno;
	return fd;
}

int hfi_sysfs_port_open(uint32_t unit, uint32_t port, const char *attr,
			int flags)
{
	int saved_errno;
	char buf[1024];
	int fd;
	const char *unitpath = sysfs_unit_path(unit);

	if (unitpath == NULL) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d: %s\n", attr,
			 unit, "unit id not valid");
		return -1;
	}
	snprintf(buf, sizeof(buf), "%s/ports/%u/%s", unitpath, port, attr);
	fd = open(buf, flags);
	saved_errno = errno;

	if (fd == -1) {
		_HFI_DBG("Failed to open attribute '%s' of unit %d:%d: %s\n",
			 attr, unit, port, strerror(errno));
		_HFI_DBG("Offending file name: %s\n", buf);
	}

	errno = saved_errno;
	return fd;
}


static int read_page(int fd, char **datap)
{
	char *data = NULL;
	int saved_errno;
	int ret = -1;

	data = malloc(sysfs_page_size);
	saved_errno = errno;

	if (!data) {
		_HFI_DBG("Could not allocate memory: %s\n", strerror(errno));
		goto bail;
	}

	ret = read(fd, data, sysfs_page_size);
	saved_errno = errno;

	if (ret == -1) {
		_HFI_DBG("Read of attribute failed: %s\n", strerror(errno));
		goto bail;
	}

bail:
	if (ret == -1) {
		free(data);
	} else {
		if (ret < sysfs_page_size)
			data[ret] = 0;
		else
			data[sysfs_page_size-1] = 0;
		*datap = data;
	}

	errno = saved_errno;
	return ret;
}

/*
 * On return, caller must free *datap.
 */
int hfi_sysfs_unit_read(uint32_t unit, const char *attr, char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = hfi_sysfs_unit_open(unit, attr, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	saved_errno = errno;

bail:
	if (ret == -1)
		*datap = NULL;

	if (fd != -1) {
		close(fd);
	}

	errno = saved_errno;
	return ret;
}

/* read a string value into buff, no more than size bytes.
   returns the number of bytes read */
size_t hfi_sysfs_unit_port_read(uint32_t unit, uint32_t port, const char *attr,
			char *buff, size_t size)
{
	int fd = -1;
	size_t rv = -1;

	fd = hfi_sysfs_port_open(unit, port, attr, O_RDONLY);

	if (fd == -1)
		return rv;

	rv = read(fd, buff, size);

	close(fd);

	if (rv < size)
		buff[rv] = 0;
	else
		buff[size-1] = 0;

	return rv;
}

/*
 * On return, caller must free *datap.
 */
int hfi_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr,
			char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = hfi_sysfs_port_open(unit, port, attr, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	saved_errno = errno;

bail:
	if (ret == -1)
		*datap = NULL;

	if (fd != -1) {
		close(fd);
	}

	errno = saved_errno;
	return ret;
}


int hfi_sysfs_unit_read_s64(uint32_t unit, const char *attr,
			    int64_t *valp, int base)
{
	char *data=NULL, *end;
	int saved_errno;
	long long val;
	int ret;

	ret = hfi_sysfs_unit_read(unit, attr, &data);
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
		free(data);
	errno = saved_errno;
	return ret;
}

static int hfi_sysfs_unit_read_node(uint32_t unit, char **datap)
{
	int fd = -1, ret = -1;
	int saved_errno;

	fd = hfi_sysfs_unit_open_for_node(unit, O_RDONLY);
	saved_errno = errno;

	if (fd == -1)
		goto bail;

	ret = read_page(fd, datap);
	if (ret == -1)
		*datap = NULL;

	saved_errno = errno;
	close(fd);
bail:
	errno = saved_errno;
	return ret;
}

int64_t hfi_sysfs_unit_read_node_s64(uint32_t unit)
{
	char *data=NULL, *end;
	int saved_errno;
	long long val;
	int64_t ret = -1;

	saved_errno = errno;
	if (hfi_sysfs_unit_read_node(unit, &data) == -1) {
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
	free(data);
	errno = saved_errno;
	return ret;
}

int hfi_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr,
			    int64_t *valp, int base)
{
	char *data, *end;
	int saved_errno;
	long long val;
	int ret;

	ret = hfi_sysfs_port_read(unit, port, attr, &data);
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
	free(data);
	errno = saved_errno;
	return ret;
}
