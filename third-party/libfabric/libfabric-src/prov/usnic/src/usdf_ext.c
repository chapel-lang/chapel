/*
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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

#include "ofi.h"

#include "usdf.h"
#include "usnic_direct.h"
#include "fi_ext_usnic.h"
#include "usdf_av.h"

/*******************************************************************************
 * Fabric extensions
 ******************************************************************************/
static int
usdf_usnic_getinfo_v1(uint32_t version, struct fid_fabric *fabric,
			struct fi_usnic_info *uip)
{
	struct usdf_fabric *fp;
	struct usd_device_attrs *dap;

	USDF_TRACE("\n");

	fp = fab_ftou(fabric);
	dap = fp->fab_dev_attrs;

	/* this assignment was missing in libfabric v1.1.1 and earlier */
	uip->ui_version = 1;

	uip->ui.v1.ui_link_speed = dap->uda_bandwidth;
	uip->ui.v1.ui_netmask_be = dap->uda_netmask_be;
	strcpy(uip->ui.v1.ui_ifname, dap->uda_ifname);
	uip->ui.v1.ui_num_vf = dap->uda_num_vf;
	uip->ui.v1.ui_qp_per_vf = dap->uda_qp_per_vf;
	uip->ui.v1.ui_cq_per_vf = dap->uda_cq_per_vf;

	return 0;
}

static int usdf_usnic_getinfo_v2(uint32_t version, struct fid_fabric *ffabric,
		struct fi_usnic_info *uip)
{
	struct usd_open_params params;
	struct usd_device_attrs *dap;
	struct usdf_fabric *fabric;
	struct usd_device *dev;
	struct fi_usnic_cap **cap;
	size_t len;
	int ret;
	int i;

	USDF_TRACE("\n");

	fabric = fab_ftou(ffabric);
	dap = fabric->fab_dev_attrs;

	memset(&params, 0, sizeof(params));
	params.flags = UOPF_SKIP_LINK_CHECK | UOPF_SKIP_PD_ALLOC;
	params.cmd_fd = -1;
	params.context = NULL;

	ret = usd_open_with_params(dap->uda_devname, &params, &dev);
	if (ret)
		return -ret;

	uip->ui_version = FI_EXT_USNIC_INFO_VERSION;

	len = ARRAY_SIZE(uip->ui.v2.ui_devname);
	strncpy(uip->ui.v2.ui_devname, dap->uda_devname, len - 1);
	uip->ui.v2.ui_devname[len - 1] = '\0';

	len = ARRAY_SIZE(uip->ui.v2.ui_ifname);
	strncpy(uip->ui.v2.ui_ifname, dap->uda_ifname, len - 1);
	uip->ui.v2.ui_ifname[len - 1] = '\0';

	memcpy(uip->ui.v2.ui_mac_addr, dap->uda_mac_addr,
			MIN(sizeof(dap->uda_mac_addr),
				sizeof(uip->ui.v2.ui_mac_addr)));

	uip->ui.v2.ui_ipaddr_be = dap->uda_ipaddr_be;
	uip->ui.v2.ui_netmask_be = dap->uda_netmask_be;
	uip->ui.v2.ui_prefixlen = dap->uda_prefixlen;
	uip->ui.v2.ui_mtu = dap->uda_mtu;
	uip->ui.v2.ui_link_up = dap->uda_link_state;

	uip->ui.v2.ui_vendor_id = dap->uda_vendor_id;
	uip->ui.v2.ui_vendor_part_id = dap->uda_vendor_part_id;
	uip->ui.v2.ui_device_id = dap->uda_device_id;

	len = ARRAY_SIZE(uip->ui.v2.ui_firmware);
	strncpy(uip->ui.v2.ui_firmware, dap->uda_firmware, len - 1);
	uip->ui.v2.ui_firmware[len - 1] = '\0';

	uip->ui.v2.ui_num_vf = dap->uda_num_vf;
	uip->ui.v2.ui_cq_per_vf = dap->uda_cq_per_vf;
	uip->ui.v2.ui_qp_per_vf = dap->uda_qp_per_vf;
	uip->ui.v2.ui_intr_per_vf = dap->uda_intr_per_vf;
	uip->ui.v2.ui_max_cq = dap->uda_max_cq;
	uip->ui.v2.ui_max_qp = dap->uda_max_qp;

	uip->ui.v2.ui_link_speed = dap->uda_bandwidth;
	uip->ui.v2.ui_max_cqe = dap->uda_max_cqe;
	uip->ui.v2.ui_max_send_credits = dap->uda_max_send_credits;
	uip->ui.v2.ui_max_recv_credits = dap->uda_max_recv_credits;

	uip->ui.v2.ui_caps = calloc(USD_CAP_MAX + 1,
			sizeof(*uip->ui.v2.ui_caps));
	if (!uip->ui.v2.ui_caps)
		return -FI_ENOMEM;

	uip->ui.v2.ui_nicname = usd_devid_to_nicname(uip->ui.v2.ui_vendor_id,
			uip->ui.v2.ui_device_id);
	uip->ui.v2.ui_pid = usd_devid_to_pid(uip->ui.v2.ui_vendor_id,
			uip->ui.v2.ui_device_id);

	for (i = 0; i < USD_CAP_MAX; i++) {
		uip->ui.v2.ui_caps[i] = calloc(1,
				sizeof(*(uip->ui.v2.ui_caps[i])));

		if (!uip->ui.v2.ui_caps[i]) {
			ret = -FI_ENOMEM;
			goto fail;
		}

		uip->ui.v2.ui_caps[i]->uc_capability = usd_capability(i);
		uip->ui.v2.ui_caps[i]->uc_present = usd_get_cap(dev, i);
	}

	usd_close(dev);

	return FI_SUCCESS;

fail:
	for (cap = uip->ui.v2.ui_caps; *cap; cap++)
		free(*cap);

	free(uip->ui.v2.ui_caps);

	usd_close(dev);

	return ret;
}

static int usdf_usnic_getinfo(uint32_t version, struct fid_fabric *fabric,
		struct fi_usnic_info *uip)
{
	assert(FI_EXT_USNIC_INFO_VERSION == 2);

	switch (version) {
	case 1:
		return usdf_usnic_getinfo_v1(version, fabric, uip);
	case 2:
		return usdf_usnic_getinfo_v2(version, fabric, uip);
	default:
		USDF_DBG_SYS(FABRIC, "invalid version\n");
		return -FI_EINVAL;
	}
}

static struct fi_usnic_ops_fabric usdf_usnic_ops_fabric = {
	.size = sizeof(struct fi_usnic_ops_fabric),
	.getinfo = usdf_usnic_getinfo
};

int
usdf_fabric_ops_open(struct fid *fid, const char *ops_name, uint64_t flags,
		void **ops, void *context)
{
	USDF_TRACE("\n");

	if (strcmp(ops_name, FI_USNIC_FABRIC_OPS_1) == 0) {
		*ops = &usdf_usnic_ops_fabric;
	} else {
		return -FI_EINVAL;
	}

	return 0;
}

/*******************************************************************************
 * Address vector extensions
 ******************************************************************************/
static int
usdf_am_get_distance(struct fid_av *fav, void *addr, int *metric_o)
{
	struct usdf_av *av;
	struct usdf_domain *udp;
	struct sockaddr_in *sin;
	int ret;

	USDF_TRACE_SYS(DOMAIN, "\n");

	av = av_ftou(fav);
	udp = av->av_domain;
	sin = addr;

	ret = usd_get_dest_distance(udp->dom_dev,
			sin->sin_addr.s_addr, metric_o);
	return ret;
}

static struct fi_usnic_ops_av usdf_usnic_ops_av = {
	.size = sizeof(struct fi_usnic_ops_av),
	.get_distance = usdf_am_get_distance,
};

int usdf_av_ops_open(struct fid *fid, const char *ops_name, uint64_t flags,
		void **ops, void *context)
{
	USDF_TRACE_SYS(AV, "\n");

	if (strcmp(ops_name, FI_USNIC_AV_OPS_1) == 0) {
		*ops = &usdf_usnic_ops_av;
	} else {
		return -FI_EINVAL;
	}

	return 0;
}
