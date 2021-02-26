/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC.
 *                         Allrights reserved.
 * Copyright (c) 2015 Cray Inc. All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <poll.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_util.h"

#define BUF_SIZE 256

static int __gnix_ipaddr_from_iface(const char *iface, struct sockaddr_in *sin)
{
	int ret = FI_SUCCESS;
	struct ifreq ifr = { { { 0 } } };
	int sock = -1;

	GNIX_TRACE(FI_LOG_FABRIC, "\n");

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		GNIX_WARN(FI_LOG_FABRIC, "Socket creation failed: %s\n",
			  strerror(errno));
		return -FI_EIO;
	}

	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", iface);

	ret = ioctl(sock, SIOCGIFADDR, &ifr);
	if (ret == -1) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "Failed to get address for ipogif0: %s\n",
			  strerror(errno));
		goto exit_w_sock;
		ret = -FI_EIO;
	}

	*sin = *(struct sockaddr_in *) &ifr.ifr_addr;

exit_w_sock:

	if (close(sock) == -1) {
		GNIX_WARN(FI_LOG_FABRIC, "Unable to close socket: %s\n",
			  strerror(errno));
	}

	return ret;

}

int _gnix_local_ipaddr(struct sockaddr_in *sin)
{
	int ret;

	/*
	 * Get the address for the ipogif0 interface.  On nodes with KNC
	 * accelerators, the iface is br0.
	 */

	ret =  __gnix_ipaddr_from_iface("ipogif0", sin);
	if (ret != FI_SUCCESS)
		ret =  __gnix_ipaddr_from_iface("br0", sin);

	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "Unable to obtain local iface addr\n");
	}

	return ret;
}

static inline uint64_t __gnix_pe_to_mac(const uint32_t pe)
{
	 return ((pe & 0x3ffff) | 0x000101000000);
}

union mac_addr {
	uint8_t octets[8];
	uint64_t u64;
};

/*
 * IP address       HW type     Flags       HW address            Mask     Device
 * 10.128.0.9       0x1         0x6         00:01:01:00:00:08     *        ipogif0
 */
#define ARP_TABLE_FILE		"/proc/net/arp"
#define ARP_TABLE_FORMAT	"%s %*s %*s %s %*s %*s"

int _gnix_pe_to_ip(const struct gnix_ep_name *ep_name,
		   struct sockaddr_in *saddr)
{
	int ret = -FI_EIO;
	FILE *arp_table;
	char buf[1024];
	char ip_str[128], mac_str[128];
	union mac_addr mac;
	union mac_addr tmp_mac = {0};
	gni_return_t status;
	uint32_t pe, cpu_id;

	status = GNI_CdmGetNicAddress(0, &pe, &cpu_id);
	if (status == GNI_RC_SUCCESS &&
	    ep_name->gnix_addr.device_addr == pe) {
		ret = _gnix_local_ipaddr(saddr);
		saddr->sin_port = ep_name->gnix_addr.cdm_id;
		return ret;
	}

	arp_table = fopen(ARP_TABLE_FILE, "r");
	if (!arp_table) {
		GNIX_WARN(FI_LOG_FABRIC, "Failed to fopen(): %s\n",
			  ARP_TABLE_FILE);
		return -FI_EIO;
	}

	/* Eat header line. */
	if (!fgets(buf, sizeof(buf), arp_table)) {
		GNIX_WARN(FI_LOG_FABRIC, "Failed to fgets(): %s\n",
			  ARP_TABLE_FILE);
		return -FI_EIO;
	}

	mac.u64 = __gnix_pe_to_mac(ep_name->gnix_addr.device_addr);

	while (fscanf(arp_table, ARP_TABLE_FORMAT, ip_str, mac_str) == 2) {
		ret = sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			     &tmp_mac.octets[5], &tmp_mac.octets[4],
			     &tmp_mac.octets[3], &tmp_mac.octets[2],
			     &tmp_mac.octets[1], &tmp_mac.octets[0]);
		if (ret == 6) {
			GNIX_DEBUG(FI_LOG_FABRIC,
				   "Comparing 0x%llx, 0x%llx\n",
				   mac.u64, tmp_mac.u64);
			if (mac.u64 == tmp_mac.u64) {
				saddr->sin_family = AF_INET;
				saddr->sin_port = ep_name->gnix_addr.cdm_id;
				saddr->sin_addr.s_addr = inet_addr(ip_str);
				ret = FI_SUCCESS;
				GNIX_DEBUG(FI_LOG_FABRIC,
					   "Translated %s->%s\n",
					   ip_str, mac_str);
				break;
			}
		} else {
			GNIX_WARN(FI_LOG_FABRIC, "Parse error: %d : %s\n",
				  ret, mac_str);
			break;
		}
	}

	fclose(arp_table);

	return ret;
}

/*
 * get gni nic addr from AF_INET  ip addr, also return local device id on same
 *subnet
 * as the input ip_addr.
 *
 * returns 0 if ipogif entry found
 * otherwise  -errno
 */
static int __gnix_get_pe_from_ip(const char *iface_name, const char *ip_addr,
				 uint32_t *gni_nic_addr)
{
	int scount;
	/* return this if no ipgogif for this ip-addr found */
	int ret = -FI_ENODATA;
	FILE *fd = NULL;
	char line[BUF_SIZE], *tmp;
	char dummy[64], iface[64], fnd_ip_addr[64];
	char mac_str[64];
	int w, x, y;

	GNIX_TRACE(FI_LOG_FABRIC, "\n");

	fd = fopen("/proc/net/arp", "r");
	if (fd == NULL) {
		return -errno;
	}

	if (fd == NULL) {
		return -errno;
	}

	while (1) {
		tmp = fgets(line, BUF_SIZE, fd);
		if (!tmp) {
			break;
		}

		/*
		 * check for a match
		 */
		if ((strstr(line, ip_addr) != NULL) &&
		    (strstr(line, iface_name) != NULL)) {
			ret = 0;
			scount = sscanf(line, "%s%s%s%s%s%s", fnd_ip_addr,
					dummy, dummy, mac_str, dummy, iface);
			if (scount != 6) {
				ret = -EIO;
				goto err;
			}

			/*
			 * check exact match of ip addr
			 */
			if (!strcmp(fnd_ip_addr, ip_addr)) {
				scount =
				    sscanf(mac_str, "00:01:01:%02x:%02x:%02x",
					   &w, &x, &y);
				if (scount != 3) {
					ret = -EIO;
					goto err;
				}

				/*
				 * mysteries of XE/XC mac to nid mapping, see
				 * nid2mac in xt sysutils
				 */
				*gni_nic_addr = (w << 16) | (x << 8) | y;
				ret = FI_SUCCESS;
				break;
			}
		}
	}

err:
	fclose(fd);
	return ret;
}

/*
 * gnix_resolve_name: given a node hint and a valid pointer to a gnix_ep_name
 * will resolve the gnix specific address of node and fill the provided
 * gnix_ep_name pointer with the information.
 *
 * node (IN) : Node name being resolved to gnix specific address
 * service (IN) : Port number being resolved to gnix specific address
 * resolved_addr (IN/OUT) : Pointer that must be provided to contain the
 *	resolved address.
 */
int _gnix_resolve_name(IN const char *node, IN const char *service,
		       IN uint64_t flags,
		       INOUT struct gnix_ep_name *resolved_addr)
{
	uint32_t pe = -1;
	uint32_t cpu_id = -1;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;

	struct sockaddr_in *sa = NULL;
	struct sockaddr_in sin;

	int ret = FI_SUCCESS;
	gni_return_t status = GNI_RC_SUCCESS;

	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_DGRAM,
	};

	GNIX_TRACE(FI_LOG_FABRIC, "\n");

	if (flags & FI_SOURCE)
		hints.ai_flags |= AI_PASSIVE;

	if (flags & FI_NUMERICHOST)
		hints.ai_flags |= AI_NUMERICHOST;

	if (!resolved_addr) {
		GNIX_WARN(FI_LOG_FABRIC,
			 "Resolved_addr must be a valid pointer.\n");
		ret = -FI_EINVAL;
		goto err;
	}

	ret = _gnix_local_ipaddr(&sin);
	if (ret != FI_SUCCESS)
		goto err;

	ret = getaddrinfo(node, service, &hints, &result);
	if (ret != 0) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "Failed to get address for node provided: %s\n",
			  gai_strerror(ret));
		ret = -FI_EINVAL;
		goto err;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		assert(rp->ai_addr->sa_family == AF_INET);
		sa = (struct sockaddr_in *) rp->ai_addr;

		/*
		 * If we are trying to resolve localhost then use
		 * CdmGetNicAddress.
		 */
		if (sa->sin_addr.s_addr == sin.sin_addr.s_addr) {
			status = GNI_CdmGetNicAddress(0, &pe, &cpu_id);
			if(status == GNI_RC_SUCCESS) {
				break;
			} else {
				GNIX_WARN(FI_LOG_FABRIC,
					  "Unable to get NIC address.");
				ret = gnixu_to_fi_errno(status);
				goto err;
			}
		} else {
			ret = __gnix_get_pe_from_ip("ipogif0",
					inet_ntoa(sa->sin_addr), &pe);
			if (ret == 0) {
				break;
			} else  {
				ret = __gnix_get_pe_from_ip("br0",
						inet_ntoa(sa->sin_addr), &pe);
			}
			if (ret == 0)
				break;
		}
	}

	/*
	 * Make sure address is valid.
	 */
	if (pe == -1) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "Unable to acquire valid address for node %s\n",
			  node);
		ret = -FI_EADDRNOTAVAIL;
		goto err;
	}

	/*
	 * Fill the INOUT parameter resolved_addr with the address information
	 * acquired for the provided node parameter.
	 */
	memset(resolved_addr, 0, sizeof(struct gnix_ep_name));

	resolved_addr->gnix_addr.device_addr = pe;
	if (service) {
		/* use resolved service/port */
		resolved_addr->gnix_addr.cdm_id = ntohs(sa->sin_port);
		resolved_addr->name_type = GNIX_EPN_TYPE_BOUND;
		resolved_addr->cm_nic_cdm_id = resolved_addr->gnix_addr.cdm_id;
	} else {
		/* generate port internally */
		resolved_addr->name_type = GNIX_EPN_TYPE_UNBOUND;
	}
	GNIX_INFO(FI_LOG_FABRIC, "Resolved: %s:%s to gnix_addr: 0x%lx\n",
		  node ?: "", service ?: "", resolved_addr->gnix_addr);
err:
	if (result != NULL) {
		freeaddrinfo(result);
	}
	return ret;
}

int _gnix_src_addr(struct gnix_ep_name *resolved_addr)
{
	gni_return_t status;
	uint32_t pe = -1;
	uint32_t cpu_id = -1;

	assert(resolved_addr);
	memset(resolved_addr, 0, sizeof(*resolved_addr));

	status = GNI_CdmGetNicAddress(0, &pe, &cpu_id);
	if (status != GNI_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "Unable to get NIC address.");
		return -FI_ENODATA;
	}

	resolved_addr->gnix_addr.device_addr = pe;
	resolved_addr->name_type = GNIX_EPN_TYPE_UNBOUND;

	return FI_SUCCESS;
}
