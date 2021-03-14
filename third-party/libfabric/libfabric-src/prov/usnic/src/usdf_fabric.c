/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <arpa/inet.h>
#include <asm/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <ofi_epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_prov.h"

#include "usnic_direct.h"
#include "libnl_utils.h"

#include "usdf.h"
#include "usdf_wait.h"
#include "fi_ext_usnic.h"
#include "usdf_progress.h"
#include "usdf_timer.h"
#include "usdf_dgram.h"
#include "usdf_cm.h"

struct usdf_usnic_info *__usdf_devinfo;

static int usdf_fabric_getname(uint32_t version, struct usd_device_attrs *dap,
			       char **name)
{
	int ret = FI_SUCCESS;
	char *bufp = NULL;
	struct in_addr in;
	char *addrnetw;

	if (FI_VERSION_GE(version, FI_VERSION(1, 4))) {
		in.s_addr = dap->uda_ipaddr_be & dap->uda_netmask_be;
		addrnetw = inet_ntoa(in);
		ret = asprintf(&bufp, "%s/%d", addrnetw, dap->uda_prefixlen);
		if (ret < 0) {
			USDF_DBG(
			    "asprintf failed while creating fabric name\n");
			ret = -ENOMEM;
		}
	} else {
		bufp = strdup(dap->uda_devname);
		if (!bufp) {
			USDF_DBG("strdup failed while creating fabric name\n");
			ret = -errno;
		}
	}

	*name = bufp;

	return ret;
}

static bool usdf_fabric_checkname(uint32_t version,
				  struct usd_device_attrs *dap, const char *hint)
{
	int ret;
	bool valid = false;
	char *reference;

	USDF_DBG("checking devname: version=%d, devname='%s'\n", version, hint);

	if (version) {
		ret = usdf_fabric_getname(version, dap, &reference);
		if (ret < 0)
			return false;

		if (strcmp(reference, hint) == 0) {
			valid = true;
		} else {
			USDF_DBG("hint %s failed to match %s\n", hint,
				 reference);
		}

		free(reference);
		return valid;
	}

	/* The hint string itself is kind of a version check, in pre-1.4 the
	* name was just the device name. In 1.4 and beyond, then name is
	* actually CIDR
	* notation.
	*/
	if (strstr(hint, "/"))
		return usdf_fabric_checkname(FI_VERSION(1, 4), dap, hint);

	return usdf_fabric_checkname(FI_VERSION(1, 3), dap, hint);
}

static int usdf_validate_hints(uint32_t version, const struct fi_info *hints)
{
	struct fi_fabric_attr *fattrp;
	size_t size;

	switch (hints->addr_format) {
	case FI_FORMAT_UNSPEC:
	case FI_SOCKADDR_IN:
		size = sizeof(struct sockaddr_in);
		break;
	case FI_SOCKADDR:
		size = sizeof(struct sockaddr);
		break;
	case FI_ADDR_STR:
		if (hints->src_addr != NULL &&
		    strlen((char *)hints->src_addr) > USDF_ADDR_STR_LEN)
			return -FI_ENODATA;

		if (hints->dest_addr != NULL &&
		    strlen((char *)hints->dest_addr) > USDF_ADDR_STR_LEN)
			return -FI_ENODATA;

		goto skip_sockaddr_size_check;
	default:
		return -FI_ENODATA;
	}

	if (hints->src_addr != NULL && hints->src_addrlen < size) {
		return -FI_ENODATA;
	}
	if (hints->dest_addr != NULL && hints->dest_addrlen < size) {
		return -FI_ENODATA;
	}

skip_sockaddr_size_check:
	if (hints->ep_attr != NULL) {
		switch (hints->ep_attr->protocol) {
		case FI_PROTO_UNSPEC:
		case FI_PROTO_UDP:
		case FI_PROTO_RUDP:
			break;
		default:
			return -FI_ENODATA;
		}

		if (hints->ep_attr->auth_key || hints->ep_attr->auth_key_size) {
			USDF_WARN_SYS(EP_CTRL,
				"\"authorization key\" is not supported in this provider.\n");
			return -FI_ENODATA;
		}
	}

	fattrp = hints->fabric_attr;
	if (fattrp != NULL) {
		if (fattrp->prov_version != 0 &&
		    fattrp->prov_version != USDF_PROV_VERSION) {
			return -FI_ENODATA;
		}
	}
	return FI_SUCCESS;
}

static int
usdf_fill_sockaddr_info(struct fi_info *fi,
	struct sockaddr_in *src, struct sockaddr_in *dest,
	struct usd_device_attrs *dap)
{
	int ret;
	struct sockaddr_in *sin;

	sin = calloc(1, sizeof(*sin));
	fi->src_addr = sin;
	if (sin == NULL) {
		ret = -FI_ENOMEM;
		return ret;
	}
	fi->src_addrlen = sizeof(struct sockaddr_in);
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = dap->uda_ipaddr_be;
	if (src != NULL)
		sin->sin_port = src->sin_port;

	/* copy in dest if specified */
	if (dest != NULL) {
		sin = calloc(1, sizeof(*sin));
		if (NULL == sin) {
			free(fi->src_addr);
			return -FI_ENOMEM;
		}
		*sin = *dest;
		fi->dest_addr = sin;
		fi->dest_addrlen = sizeof(*sin);
	}
	return FI_SUCCESS;
}

static int
usdf_fill_straddr_info(struct fi_info *fi,
	char *src, char *dest, struct usd_device_attrs *dap)
{
	char *address_string;
	struct sockaddr_in *sin;

	/* If NULL, we have to create the sockaddr_in
	 * and convert it to string format.
	 */
	if (src == NULL) {
		sin = calloc(1, sizeof(*sin));
		if (NULL == sin)
			return -FI_ENOMEM;
		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = dap->uda_ipaddr_be;

		address_string = calloc(1, USDF_ADDR_STR_LEN);
		fi->src_addr = address_string;
		fi->src_addrlen = USDF_ADDR_STR_LEN;

		usdf_addr_tostr(sin, fi->src_addr, &fi->src_addrlen);
		free(sin);
	} else {
	/* Otherwise, it is already in string format.
	 * Just copy it.
	 */
		address_string = strdup(src);
		if (NULL == address_string)
			return -FI_ENOMEM;
		fi->src_addr = address_string;
		fi->src_addrlen = strlen(address_string);
	}

	/* Same goes for dest. */
	if (dest != NULL) {
		address_string = strdup(dest);
		fi->dest_addr = address_string;
		fi->dest_addrlen = strlen(address_string);
	}

	return FI_SUCCESS;
}
static int
usdf_fill_addr_info(struct fi_info *fi, uint32_t addr_format,
		void *src, void *dest, struct usd_device_attrs *dap)
{
	int ret;

	if (addr_format != FI_FORMAT_UNSPEC) {
		fi->addr_format = addr_format;
	} else {
		fi->addr_format = FI_SOCKADDR_IN;
	}

	switch (fi->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		ret = usdf_fill_sockaddr_info(fi, src, dest, dap);
		if (ret != FI_SUCCESS)
			goto fail;
		break;
	case FI_ADDR_STR:
		ret = usdf_fill_straddr_info(fi, src, dest, dap);
		if (ret != FI_SUCCESS)
			goto fail;
		break;
	default:
		ret = -FI_ENODATA;
		goto fail;
	}

	return 0;

fail:
	return ret;		// fi_freeinfo() in caller frees all
}

static int validate_modebits(uint32_t version, const struct fi_info *hints,
			       uint64_t supported, uint64_t *mode_out)
{
	uint64_t mode;

	/* If there is no hints, return everything we supported. */
	if (!hints) {
		*mode_out = supported;
		return FI_SUCCESS;
	}

	mode = hints->mode & supported;

	/* Before version 1.5, FI_LOCAL_MR is a requirement. */
	if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		if ((mode & FI_LOCAL_MR) == 0)
			return -FI_ENODATA;
	}

	*mode_out = mode;

	return FI_SUCCESS;
}

static int usdf_alloc_fid_nic(struct fi_info *fi,
			struct usd_device_attrs *dap)
{
	int ret;
	struct fid_nic *nic = NULL;
	struct fi_device_attr *da = NULL;
	struct fi_link_attr *la = NULL;

	nic = ofi_nic_dup(NULL);
	if (!nic)
		goto nomem;

	da = nic->device_attr;
	da->name = strdup(dap->uda_devname);
	if (!da->name)
		goto nomem;
	ret = asprintf(&da->device_id, "%s (%s)",
		usd_devid_to_pid(dap->uda_vendor_id,
				dap->uda_device_id),
		usd_devid_to_nicname(dap->uda_vendor_id,
				dap->uda_device_id));
	if (ret < 0)
		goto nomem;
	ret = asprintf(&da->device_version, "0x%x", dap->uda_vendor_part_id);
	if (ret < 0)
		goto nomem;
	ret = asprintf(&da->vendor_id, "0x%x", dap->uda_vendor_id);
	if (ret < 0)
		goto nomem;
	da->driver = strdup("usnic_verbs");
	if (!da->driver)
		goto nomem;
	da->firmware = strdup(dap->uda_firmware);
	if (!da->firmware)
		goto nomem;

	// usnic does not currently expose PCI bus information, so we
	// set the bus type to unknown.
	nic->bus_attr->bus_type = FI_BUS_UNKNOWN;

	la = nic->link_attr;

	socklen_t size = INET_ADDRSTRLEN;
	la->address = calloc(1, size);
	if (!la->address)
		goto nomem;
	inet_ntop(AF_INET, &dap->uda_ipaddr_be, la->address, size);
	la->mtu = dap->uda_mtu;
	la->speed = dap->uda_bandwidth;
	switch (dap->uda_link_state) {
	case USD_LINK_UP:
		la->state = FI_LINK_UP;
		break;
	case USD_LINK_DOWN:
		la->state = FI_LINK_DOWN;
		break;
	default:
		la->state = FI_LINK_UNKNOWN;
		break;
	}
	la->network_type = strdup("Ethernet");
	if (!la->network_type)
		goto nomem;

	fi->nic = nic;

	return FI_SUCCESS;

nomem:
	if (nic)
		fi_close(&nic->fid);
	return -FI_ENOMEM;
}

static int usdf_fill_info_dgram(
	uint32_t version,
	const struct fi_info *hints,
	void *src,
	void *dest,
	struct usd_device_attrs *dap,
	struct fi_info **fi_first,
	struct fi_info **fi_last)
{
	struct fi_info *fi;
	struct fi_fabric_attr *fattrp;
	uint32_t addr_format;
	int ret;

	fi = fi_allocinfo();
	if (fi == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	fi->caps = USDF_DGRAM_CAPS;

	ret = validate_modebits(version, hints,
				  USDF_DGRAM_SUPP_MODE, &fi->mode);
	if (ret)
		goto fail;

	if (hints != NULL) {
		addr_format = hints->addr_format;

		/* check that we are capable of what's requested */
		if ((hints->caps & ~USDF_DGRAM_CAPS) != 0) {
			ret = -FI_ENODATA;
			goto fail;
		}

		fi->handle = hints->handle;
	} else {
		addr_format = FI_FORMAT_UNSPEC;
	}
	fi->ep_attr->type = FI_EP_DGRAM;

	ret = usdf_fill_addr_info(fi, addr_format, src, dest, dap);
	if (ret != 0) {
		goto fail;
	}

	/* fabric attrs */
	fattrp = fi->fabric_attr;
	ret = usdf_fabric_getname(version, dap, &fattrp->name);
	if (ret < 0 || fattrp->name == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	if (fi->mode & FI_MSG_PREFIX) {
		if (FI_VERSION_GE(version, FI_VERSION(1, 1)))
			fi->ep_attr->msg_prefix_size = USDF_HDR_BUF_ENTRY;
		else
			fi->mode &= ~FI_MSG_PREFIX;
	}

	ret = usdf_dgram_fill_ep_attr(version, hints, fi, dap);
	if (ret)
		goto fail;

	ret = usdf_dgram_fill_dom_attr(version, hints, fi, dap);
	if (ret)
		goto fail;

	ret = usdf_dgram_fill_tx_attr(version, hints, fi, dap);
	if (ret)
		goto fail;

	ret = usdf_dgram_fill_rx_attr(version, hints, fi, dap);
	if (ret)
		goto fail;

	ret = usdf_alloc_fid_nic(fi, dap);
	if (ret)
		goto fail;

	/* add to tail of list */
	if (*fi_first == NULL) {
		*fi_first = fi;
	} else {
		(*fi_last)->next = fi;
	}
	*fi_last = fi;

	return 0;

fail:
	if (fi != NULL) {
		fi_freeinfo(fi);
	}
	return ret;
}

static int
usdf_get_devinfo(void)
{
	struct usdf_usnic_info *dp;
	struct usdf_dev_entry *dep;
	struct usd_open_params params;
	int ret;
	int d;

	assert(__usdf_devinfo == NULL);

	dp = calloc(1, sizeof(*dp));
	if (dp == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}
	__usdf_devinfo = dp;

	dp->uu_num_devs = USD_MAX_DEVICES;
	ret = usd_get_device_list(dp->uu_devs, &dp->uu_num_devs);
	if (ret != 0) {
		dp->uu_num_devs = 0;
		goto fail;
	}

	for (d = 0; d < dp->uu_num_devs; ++d) {
		dep = &dp->uu_info[d];

		memset(&params, 0, sizeof(params));
		params.flags = UOPF_SKIP_PD_ALLOC;
		params.cmd_fd = -1;
		params.context = NULL;
		ret = usd_open_with_params(dp->uu_devs[d].ude_devname,
						&params, &dep->ue_dev);
		if (ret != 0) {
			continue;
		}

		ret = usd_get_device_attrs(dep->ue_dev, &dep->ue_dattr);
		if (ret != 0) {
			continue;
		}

		dep->ue_dev_ok = 1;	/* this device is OK */

		usd_close(dep->ue_dev);
		dep->ue_dev = NULL;
	}
	return 0;

fail:
	return ret;
}

static int
usdf_get_distance(
    struct usd_device_attrs *dap,
    uint32_t daddr_be,
    int *metric_o)
{
    uint32_t nh_ip_addr;
    int ret;

    USDF_TRACE("\n");

    ret = usnic_nl_rt_lookup(dap->uda_ipaddr_be, daddr_be,
            dap->uda_ifindex, &nh_ip_addr);
    if (ret != 0) {
        *metric_o = -1;
        ret = 0;
    } else if (nh_ip_addr == 0) {
        *metric_o = 0;
    } else {
        *metric_o = 1;
    }

    return ret;
}

/* Check all things related to a device. Make sure it's okay, the source address
 * matches the requested address, the destination is reachable from the device,
 * the device fabric name matches the requested fabric name, and the device
 * domain name matches the requested domain name.
 *
 * @param version Libfabric API version used to verify the domain / fabric name.
 * @param hints   Hints passed to fi_getinfo.
 * @param src     Source address being requested.
 * @param dest    Destination address to communicate with.
 * @param dep     usNIC device entry being checked.
 *
 * @return true on success, false on failure. For debug logging can be enabled
 *         to see why a device was disqualified.
 */
static bool usdf_check_device(uint32_t version, const struct fi_info *hints,
			      void *src, void *dest,
			      struct usdf_dev_entry *dep)
{
	char dest_str[INET_ADDRSTRLEN];
	char src_str[INET_ADDRSTRLEN];
	char dev_str[INET_ADDRSTRLEN];
	struct usd_device_attrs *dap;
	struct sockaddr_in *sin;
	int reachable;
	int ret;

	reachable = -1;
	dap = &dep->ue_dattr;

	/* Skip the device if it has problems. */
	if (!dep->ue_dev_ok) {
		USDF_WARN_SYS(FABRIC, "skipping %s/%s device not ok\n",
			      dap->uda_devname, dap->uda_ifname);
		return false;
	}

	/* If the given source address is not INADDR_ANY, compare against the
	 * device.
	 */
	if (src) {
		sin = usdf_format_to_sin(hints, src);
		if (sin->sin_addr.s_addr != INADDR_ANY) {
			if (sin->sin_addr.s_addr != dap->uda_ipaddr_be) {
				inet_ntop(AF_INET, &sin->sin_addr.s_addr,
					  src_str, sizeof(src_str));
				inet_ntop(AF_INET, &dap->uda_ipaddr_be,
					  dev_str, sizeof(dev_str));
				USDF_WARN_SYS(FABRIC,
					      "src addr<%s> != dev addr<%s>\n",
					      src_str, dev_str);
				goto fail;
			}
		}

		usdf_free_sin_if_needed(hints, sin);
	}

	/* Check that the given destination address is reachable from the
	 * interface.
	 */
	if (dest) {
		sin = usdf_format_to_sin(hints, dest);
		if (sin->sin_addr.s_addr != INADDR_ANY) {
			ret = usdf_get_distance(dap, sin->sin_addr.s_addr,
						&reachable);
			if (ret) {
				inet_ntop(AF_INET,
					  &sin->sin_addr.s_addr, dest_str,
					  sizeof(dest_str));
				USDF_WARN_SYS(FABRIC,
					      "get_distance failed @ %s\n",
					      dest_str);
				goto fail;
			}
		}

		if (reachable == -1) {
			inet_ntop(AF_INET, &sin->sin_addr.s_addr, dest_str,
				  sizeof(dest_str));
			USDF_WARN_SYS(FABRIC,
				      "dest %s unreachable from %s/%s, skipping\n",
				      dest_str, dap->uda_devname,
				      dap->uda_ifname);
			goto fail;
		}

		usdf_free_sin_if_needed(hints, sin);
	}

	/* Checks that the fabric name is correct for the given interface. The
	 * fabric name contains the CIDR notation for the interface.
	 */
	if (hints && hints->fabric_attr && hints->fabric_attr->name) {
		if (!usdf_fabric_checkname(version, dap,
					  hints->fabric_attr->name))
			return false;
	}

	/* Check that the domain name is correct for the given interface. The
	 * domain name is the device name.
	 */
	if (hints && hints->domain_attr && hints->domain_attr->name) {
		if (!usdf_domain_checkname(version, dap,
					   hints->domain_attr->name))
			return false;
	}

	return true;

fail:
	usdf_free_sin_if_needed(hints, sin);

	return false;
}

static int
usdf_handle_node_and_service(const char *node, const char *service,
		uint64_t flags, void **src, void **dest,
		const struct fi_info *hints, struct addrinfo **ai)
{
	int ret;
	struct sockaddr_in *sin;

	if (node != NULL || service != NULL) {
		if (hints && hints->addr_format == FI_ADDR_STR) {
			/* FI_ADDR_STR can't have service param. */
			if (service)
				return -FI_EINVAL;

			sin = usdf_format_to_sin(hints, node);

			if (!sin)
				/* This could be invalid or no memory. */
				return -FI_EINVAL;
		} else {
			ret = getaddrinfo(node, service, NULL, ai);
			if (ret != 0) {
				USDF_DBG("getaddrinfo failed: %d: <%s>\n", ret,
					 gai_strerror(ret));
				return ret;
			}
			sin = (struct sockaddr_in *)(*ai)->ai_addr;
		}

		if (flags & FI_SOURCE)
			*src = usdf_sin_to_format(hints, sin, NULL);
		else
			*dest = usdf_sin_to_format(hints, sin, NULL);
	}

	return FI_SUCCESS;
}

static int
usdf_getinfo(uint32_t version, const char *node, const char *service,
	       uint64_t flags, const struct fi_info *hints, struct fi_info **info)
{
	struct usdf_usnic_info *dp;
	struct usdf_dev_entry *dep;
	struct usd_device_attrs *dap;
	struct fi_info *fi_first;
	struct fi_info *fi_last;
	struct addrinfo *ai;
	void *src;
	void *dest;
	enum fi_ep_type ep_type;
	int d;
	int ret;

	USDF_TRACE("\n");

	fi_first = NULL;
	fi_last = NULL;
	ai = NULL;
	src = NULL;
	dest = NULL;

	/*
	 * Get and cache usNIC device info
	 */
	if (__usdf_devinfo == NULL) {
		ret = usdf_get_devinfo();
		if (ret != 0) {
			USDF_WARN("failed to usdf_get_devinfo, ret=%d (%s)\n",
					ret, fi_strerror(-ret));
			if (ret == -FI_ENODEV)
				ret = -FI_ENODATA;
			goto fail;
		}
	}
	dp = __usdf_devinfo;

	/* Check the hints up front and fail if they're invalid. */
	if (hints) {
		ret = usdf_validate_hints(version, hints);
		if (ret) {
			USDF_WARN_SYS(FABRIC, "hints failed to validate\n");
			goto fail;
		}
	}

	/* Get the src and dest if user specified. */
	ret = usdf_handle_node_and_service(node, service, flags,
					   &src, &dest, hints, &ai);
	if (ret) {
		USDF_WARN_SYS(FABRIC, "failed to handle node and service.\n");
		goto fail;
	}

	if (hints != NULL) {
		if (dest == NULL && hints->dest_addr != NULL)
			dest = hints->dest_addr;
		if (src == NULL && hints->src_addr != NULL)
			src = hints->src_addr;
	}

	for (d = 0; d < dp->uu_num_devs; ++d) {
		dep = &dp->uu_info[d];
		dap = &dep->ue_dattr;

		/* If the device has an issue or the hints don't match the
		 * device information, then skip.
		 */
		if (!usdf_check_device(version, hints, src, dest, dep))
			continue;

		if (hints && hints->ep_attr)
			ep_type = hints->ep_attr->type;
		else
			ep_type = FI_EP_UNSPEC;

		if (ep_type == FI_EP_DGRAM || ep_type == FI_EP_UNSPEC) {
			ret = usdf_fill_info_dgram(version, hints, src, dest,
					dap, &fi_first, &fi_last);
			if (ret != 0 && ret != -FI_ENODATA) {
				goto fail;
			}
		}
	}

	if (fi_first != NULL) {
		*info = fi_first;
		ret = 0;
	} else {
		ret = -FI_ENODATA;
	}


fail:
	if (ai)
		freeaddrinfo(ai);

	if (ret != 0) {
		fi_freeinfo(fi_first);
		USDF_INFO("returning %d (%s)\n", ret, fi_strerror(-ret));
	}

	return ret;
}

static int
usdf_fabric_close(fid_t fid)
{
	struct usdf_fabric *fp;
	int ret;
	void *rv;

	USDF_TRACE("\n");

	fp = fab_fidtou(fid);
	if (ofi_atomic_get32(&fp->fab_refcnt) > 0) {
		return -FI_EBUSY;
	}
	/* Tell progression thread to exit */
	fp->fab_exit = 1;

	free(fp->fab_attr.name);
	free(fp->fab_attr.prov_name);

	if (fp->fab_thread) {
		ret = usdf_fabric_wake_thread(fp);
		if (ret != 0) {
			return ret;
		}
		pthread_join(fp->fab_thread, &rv);
	}
	usdf_timer_deinit(fp);
	if (fp->fab_epollfd != OFI_EPOLL_INVALID) {
		ofi_epoll_close(fp->fab_epollfd);
	}
	if (fp->fab_eventfd != -1) {
		close(fp->fab_eventfd);
	}
	if (fp->fab_arp_sockfd != -1) {
		close(fp->fab_arp_sockfd);
	}

	free(fp);
	return 0;
}

static struct fi_ops usdf_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = usdf_fabric_ops_open,
};

static struct fi_ops_fabric usdf_ops_fabric = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = usdf_domain_open,
	.passive_ep = usdf_pep_open,
	.eq_open = usdf_eq_open,
	.wait_open = usdf_wait_open,
	.trywait = usdf_trywait
};

static int
usdf_fabric_open(struct fi_fabric_attr *fattrp, struct fid_fabric **fabric,
	       void *context)
{
	struct fid_fabric *ff;
	struct usdf_fabric *fp;
	struct usdf_usnic_info *dp;
	struct usdf_dev_entry *dep;
	struct sockaddr_in sin;
	int ret;
	int d;

	USDF_TRACE("\n");

	/* Make sure this fabric exists */
	dp = __usdf_devinfo;
	for (d = 0; d < dp->uu_num_devs; ++d) {
		dep = &dp->uu_info[d];
		if (dep->ue_dev_ok &&
		    usdf_fabric_checkname(0, &(dep->ue_dattr), fattrp->name)) {
			break;
		}
	}
	if (d >= dp->uu_num_devs) {
		USDF_INFO("device \"%s\" does not exit, returning -FI_ENODEV\n",
				fattrp->name);
		return -FI_ENODEV;
	}

	fp = calloc(1, sizeof(*fp));
	if (fp == NULL) {
		USDF_INFO("unable to allocate memory for fabric\n");
		return -FI_ENOMEM;
	}
	fp->fab_epollfd = OFI_EPOLL_INVALID;
	fp->fab_arp_sockfd = -1;
	LIST_INIT(&fp->fab_domain_list);

	fp->fab_attr.fabric = fab_utof(fp);
	fp->fab_attr.name = strdup(fattrp->name);
	fp->fab_attr.prov_name = strdup(USDF_PROV_NAME);
	fp->fab_attr.prov_version = USDF_PROV_VERSION;
	if (fp->fab_attr.name == NULL ||
			fp->fab_attr.prov_name == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	fp->fab_fid.fid.fclass = FI_CLASS_FABRIC;
	fp->fab_fid.fid.context = context;
	fp->fab_fid.fid.ops = &usdf_fi_ops;
	fp->fab_fid.ops = &usdf_ops_fabric;

	fp->fab_dev_attrs = &dep->ue_dattr;

	ret = ofi_epoll_create(&fp->fab_epollfd);
	if (ret) {
		USDF_INFO("unable to allocate epoll fd\n");
		goto fail;
	}

	fp->fab_eventfd = eventfd(0, EFD_NONBLOCK | EFD_SEMAPHORE);
	if (fp->fab_eventfd == -1) {
		ret = -errno;
		USDF_INFO("unable to allocate event fd\n");
		goto fail;
	}
	fp->fab_poll_item.pi_rtn = usdf_fabric_progression_cb;
	fp->fab_poll_item.pi_context = fp;
	ret = ofi_epoll_add(fp->fab_epollfd, fp->fab_eventfd, OFI_EPOLL_IN,
			    &fp->fab_poll_item);
	if (ret) {
		USDF_INFO("unable to EPOLL_CTL_ADD\n");
		goto fail;
	}

	/* initialize timer subsystem */
	ret = usdf_timer_init(fp);
	if (ret != 0) {
		USDF_INFO("unable to initialize timer\n");
		goto fail;
	}

	/* create and bind socket for ARP resolution */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = fp->fab_dev_attrs->uda_ipaddr_be;
	fp->fab_arp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fp->fab_arp_sockfd == -1) {
		USDF_INFO("unable to create socket\n");
		goto fail;
	}
	ret = bind(fp->fab_arp_sockfd, (struct sockaddr *) &sin, sizeof(sin));
	if (ret == -1) {
		ret = -errno;
		goto fail;
	}

	ofi_atomic_initialize32(&fp->fab_refcnt, 0);
	ofi_atomic_initialize32(&fp->num_blocked_waiting, 0);

	ret = pthread_create(&fp->fab_thread, NULL,
			usdf_fabric_progression_thread, fp);
	if (ret != 0) {
		ret = -ret;
		USDF_INFO("unable to create progress thread\n");
		goto fail;
	}

	fattrp->fabric = fab_utof(fp);
	fattrp->prov_version = USDF_PROV_VERSION;
	*fabric = fab_utof(fp);
	USDF_INFO("successfully opened %s/%s\n", fattrp->name,
			fp->fab_dev_attrs->uda_ifname);
	return 0;

fail:
	free(fp->fab_attr.name);
	free(fp->fab_attr.prov_name);
	ff = fab_utof(fp);
	usdf_fabric_close(&ff->fid);
	USDF_DBG("returning %d (%s)\n", ret, fi_strerror(-ret));
	return ret;
}

static void usdf_fini(void)
{
	USDF_TRACE("\n");
}

struct fi_provider usdf_ops = {
	.name = USDF_PROV_NAME,
	.version = USDF_PROV_VERSION,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = usdf_getinfo,
	.fabric = usdf_fabric_open,
	.cleanup =  usdf_fini
};

USNIC_INI
{
#if USNIC_BUILD_FAKE_VERBS_DRIVER
	usdf_setup_fake_ibv_provider();
#endif
	return (&usdf_ops);
}
