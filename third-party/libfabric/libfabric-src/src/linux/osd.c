/*
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "ofi.h"
#include "ofi_osd.h"

#include <net/if.h>
#include <sys/types.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

ssize_t ofi_get_hugepage_size(void)
{
	FILE *fd;
	char *line = NULL;
	size_t len = 0;
	ssize_t val = -1;

	fd = fopen("/proc/meminfo", "r");
	if (!fd)
		return -errno;

	while (getline(&line, &len, fd) != -1)
		if (sscanf(line, "Hugepagesize: %zu kB", &val) == 1)
			break;

	free(line);
	fclose(fd);

	if (val == -1)
		return -FI_ENOENT;

	return val * 1024;
}

#ifdef HAVE_ETHTOOL

#if HAVE_DECL_ETHTOOL_CMD_SPEED
static inline uint32_t ofi_ethtool_cmd_speed(struct ethtool_cmd *ecmd)
{
	return ethtool_cmd_speed(ecmd);
}
#else /* HAVE_DECL_ETHTOOL_CMD_SPEED */
static inline uint32_t ofi_ethtool_cmd_speed(struct ethtool_cmd *ecmd)
{
	return ecmd->speed;
}
#endif /* HAVE_DECL_ETHTOOL_CMD_SPEED */

#if HAVE_DECL_SPEED_UNKNOWN
static inline int ofi_ethtool_is_known(uint32_t speed_mbps)
{
	return (speed_mbps != SPEED_UNKNOWN);
}
#else /* HAVE_DECL_SPEED_UNKNOWN */
static inline int ofi_ethtool_is_known(uint32_t speed_mbps)
{
	return ((speed_mbps != 0) && ((uint16_t)speed_mbps != (uint16_t)-1));
}
#endif /* HAVE_DECL_SPEED_UNKNOWN */

/* Note: If the value of the speed is unknown, set it to 100 Mb/s */
static inline size_t ofi_ethtool_get_speed(struct ethtool_cmd *ecmd)
{
	uint32_t speed_mbps = ofi_ethtool_cmd_speed(ecmd);

	return (ofi_ethtool_is_known(speed_mbps) ? speed_mbps : 100);
}

size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa)
{
	struct ethtool_cmd cmd = {
		.cmd = ETHTOOL_GSET,
	};
	struct ifreq ifr = {
		.ifr_data = (void *)&cmd,
	};
	SOCKET fd;
	int ret;

	fd = socket(ifa->ifa_addr->sa_family, SOCK_STREAM, IPPROTO_IP);
	if (fd < 0)
		return 0;

	strncpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE);
	ifr.ifr_name[IF_NAMESIZE - 1] = '\0';

	ret = ioctl(fd, SIOCETHTOOL, &ifr);
	if (ret) {
		close(fd);
		return 0;
	}

	close(fd);

	return ofi_ethtool_get_speed(&cmd);
}

#else /* HAVE_ETHTOOL */

size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa)
{
	FILE *fd;
	char *line = NULL;
	size_t len = 0;
	char *speed_filename_prefix = "/sys/class/net/";
	char *speed_filename_suffix = "/speed";
	char *speed_filename;
	size_t speed;
	/* IF_NAMESIZE includes NULL-terminated symbol */
	size_t filename_len = strlen(speed_filename_prefix) +
			      strlen(speed_filename_prefix) +
			      IF_NAMESIZE;

	speed_filename = calloc(1, filename_len);
	if (!speed_filename)
		return 0;

	snprintf(speed_filename, filename_len, "%s%s%s",
		 speed_filename_prefix, ifa->ifa_name, speed_filename_suffix);

	fd = fopen(speed_filename, "r");
	if (!fd)
		 goto err1;

	if (getline(&line, &len, fd) == -1) {
		goto err2;
	}

	if (sscanf(line, "%zu", &speed) != 1)
		goto err3;

	free(line);
	fclose(fd);
	free(speed_filename);

	return speed;
err3:
	free(line);
err2:
	fclose(fd);
err1:
	free(speed_filename);
	return 0;
}

#endif /* HAVE_ETHTOOL */
