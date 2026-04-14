/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include <ofi_prov.h>
#include <glob.h>

#include "lpp.h"

/* From fi_getinfo(3), regarding FI_MSG/FI_TAGGED: "In the absence of any
 * relevant flags, FI_MSG implies the ability to send and receive messages." */
#define IMPLIED_MSG_CAPS(attr, sendrecv) (((attr)->caps & (FI_SEND | FI_RECV)) == 0 && \
					  ((attr)->caps & (FI_MSG | FI_TAGGED)) != 0 ? sendrecv : 0)

/* From fi_getinfo(3), regarding FI_RMA/FI_ATOMIC: "In the absence of any
 * relevant flags, FI_RMA implies the ability to initiate and be the target
 * of remote memory reads and writes." */
#define IMPLIED_RMA_CAPS(attr) (((attr)->caps & (FI_WRITE | FI_READ | FI_REMOTE_WRITE | FI_REMOTE_READ)) == 0 && \
				((attr)->caps & (FI_RMA | FI_ATOMIC)) != 0 ? \
						(FI_WRITE | FI_READ | FI_REMOTE_WRITE | FI_REMOTE_READ) : 0)

static int lpp_getinfo_match(const struct klppioc_lf *klpp_devinfo,
			const struct fi_info *hints, uint32_t version)
{
	if (!hints)
		return 0;
	// Compare the modes the application supports to those we require.
	if ((hints->mode & LPP_MODE) != LPP_MODE) {
		FI_INFO(&lpp_prov, FI_LOG_FABRIC, "hint mode %lx invalid for LPP mode %x\n", hints->mode, LPP_MODE);
		return -FI_ENODATA;
	}

	// Compare the capabilities requested to those we can provide.
	if ((hints->caps & LPP_CAPS) != hints->caps) {
		FI_INFO(&lpp_prov, FI_LOG_FABRIC, "hint caps %lx invalid for LPP caps %llx\n", hints->caps, LPP_CAPS);
		return -FI_ENODATA;
	}

	// Check the various attributes.
	if (hints->domain_attr && (lpp_domain_verify_attrs(klpp_devinfo, hints->domain_attr, version) != 0))
		return -FI_ENODATA;

	if (hints->fabric_attr && (lpp_fabric_verify_attrs(klpp_devinfo, hints->fabric_attr) != 0))
		return -FI_ENODATA;

	if(hints->ep_attr && (lpp_verify_ep_attrs(klpp_devinfo, hints->ep_attr) !=0))
		return -FI_ENODATA;

	if (hints->tx_attr && (lpp_verify_tx_attrs(klpp_devinfo, hints->tx_attr) != 0))
		return -FI_ENODATA;

	if (hints->rx_attr && (lpp_verify_rx_attrs(klpp_devinfo, hints->rx_attr) != 0))
		return -FI_ENODATA;

	return 0;
}

static struct fi_info *lpp_getinfo_device(const struct klppioc_lf *klpp_devinfo,
			const struct fi_info *hints, uint32_t version)
{
	struct fi_info *tmp = NULL;
	int ret = FI_SUCCESS;

	tmp = fi_allocinfo();
	if (!tmp)
		return NULL;

	tmp->next = NULL;
	tmp->caps = (!hints || hints->caps == 0) ? LPP_CAPS : hints->caps & LPP_CAPS;

	tmp->mode = LPP_MODE;
	tmp->addr_format = FI_SOCKADDR_IN;
	tmp->src_addr = strdup("");
	tmp->src_addrlen = sizeof(struct sockaddr_in);
	tmp->dest_addr = strdup("");
	tmp->dest_addrlen = sizeof(struct sockaddr_in);

	*tmp->tx_attr = lpp_tx_attrs(klpp_devinfo);
	if (hints && hints->caps) {
		tmp->tx_attr->caps = LPP_CAPS_TX_SECONDARY;
		tmp->tx_attr->caps |= LPP_CAPS_PRIMARY;
		tmp->tx_attr->caps |= IMPLIED_MSG_CAPS(tmp->tx_attr, FI_SEND);
		tmp->tx_attr->caps |= IMPLIED_RMA_CAPS(tmp->tx_attr);
		tmp->tx_attr->caps = (tmp->tx_attr->caps & hints->caps);
	} else {
		tmp->tx_attr->caps = 0;
	}

	if (hints && hints->tx_attr)
		tmp->tx_attr->op_flags = hints->tx_attr->op_flags;

	*tmp->rx_attr = lpp_rx_attrs(klpp_devinfo);
	if (hints && hints->caps) {
		tmp->rx_attr->caps = LPP_CAPS_RX_SECONDARY;
		tmp->rx_attr->caps |= LPP_CAPS_PRIMARY;
		tmp->rx_attr->caps |= IMPLIED_MSG_CAPS(tmp->rx_attr, FI_RECV);
		tmp->rx_attr->caps |= IMPLIED_RMA_CAPS(tmp->rx_attr);
		tmp->rx_attr->caps = (tmp->rx_attr->caps & hints->caps);
	} else {
		tmp->rx_attr->caps = 0;
	}

	if (hints && hints->rx_attr)
		tmp->rx_attr->op_flags = hints->rx_attr->op_flags;

	lpp_fabric_setup_attrs(klpp_devinfo, hints, tmp->fabric_attr);
	lpp_ep_setup_attrs(klpp_devinfo, hints, tmp->ep_attr);
	lpp_domain_setup_attrs(klpp_devinfo, hints, tmp->domain_attr);
	if (hints && hints->domain_attr) {
		tmp->domain_attr->mr_mode &= hints->domain_attr->mr_mode;

		ret = ofi_check_domain_attr(&lpp_prov, version, tmp->domain_attr, hints);
		if (ret != FI_SUCCESS)
			goto out;

		ret = ofi_check_mr_mode(&lpp_prov, version, tmp->domain_attr->mr_mode, hints);
		if (ret != FI_SUCCESS)
			goto out;
	}

	if (hints && hints->rx_attr) {
		ret = ofi_check_rx_attr(&lpp_prov, tmp, hints->rx_attr, hints->mode);
		if (ret != FI_SUCCESS)
			goto out;
	}

	if (hints && hints->tx_attr) {
		ret = ofi_check_tx_attr(&lpp_prov, tmp->tx_attr, hints->tx_attr, hints->mode);
		if (ret != FI_SUCCESS)
			goto out;
	}

	tmp->nic = ofi_nic_dup(NULL);
	if (!tmp->nic) {
		ret = -FI_ENOMEM;
		goto out;
	}
	/* There are actually always 2 devices of interest: the NT endpoint and
	 * a DMA engine. The devices are not necessarily close in the PCI tree.
	 * The device we return is the NT endpoint. */
	tmp->nic->bus_attr->bus_type = FI_BUS_PCI;
	tmp->nic->bus_attr->attr.pci.domain_id = klpp_devinfo->pci_attrs.domain;
	tmp->nic->bus_attr->attr.pci.bus_id = klpp_devinfo->pci_attrs.bus;
	tmp->nic->bus_attr->attr.pci.device_id = klpp_devinfo->pci_attrs.device;
	tmp->nic->bus_attr->attr.pci.function_id = klpp_devinfo->pci_attrs.function;
out:
	if (ret != FI_SUCCESS) {
		free(tmp);
		tmp = NULL;
	}

	return tmp;
}

int lpp_fi_getinfo(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info *hints,
		   struct fi_info **info)
{

	struct fi_info		*tmp = NULL;
	struct fi_info		*tail = NULL;
	struct klppioc_lf	klpp_devinfo;
	glob_t			globmatch;
	int			klpp_fd;
	int			ret;

	/* We only support FI_EP_RDM. */
	if (hints && hints->ep_attr) {
		if ((hints->ep_attr->type != FI_EP_UNSPEC) &&
			(hints->ep_attr->type != FI_EP_RDM)) {
			FI_WARN(&lpp_prov, FI_LOG_FABRIC,
				"only RDM msg type is supported, requested %x\n",
				hints->ep_attr->type);
			return -FI_ENODATA;
		}
	}

	ret = glob("/dev/klpp[0-9]", GLOB_ERR, NULL, &globmatch);
	if (ret == GLOB_NOMATCH) {
		FI_WARN(&lpp_prov, FI_LOG_FABRIC, "no KLPP devices found\n");
		return -FI_ENODATA;
	} else if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_FABRIC, "failed to enumerate KLPP devices (%d)\n",
		    ret);
		return -FI_ENODATA;
	}

	*info = NULL;
	for (size_t i = 0; i < globmatch.gl_pathc; i++) {
		klpp_fd = open(globmatch.gl_pathv[i], O_RDWR);
		if (klpp_fd < 0) {
			FI_WARN(&lpp_prov, FI_LOG_FABRIC, "failed to open %s (errno %d: %s)\n",
				globmatch.gl_pathv[i], errno, strerror(errno));
			continue;
		}
		if (klpp_getdevice(klpp_fd, &klpp_devinfo) < 0) {
			FI_WARN(&lpp_prov, FI_LOG_FABRIC, "failed to query KLPP device %ld\n", i);
			continue;
		}
		close(klpp_fd);

		if (lpp_getinfo_match(&klpp_devinfo, hints, version) == 0) {
			tmp = lpp_getinfo_device(&klpp_devinfo, hints, version);
			if (tmp) {
				if (*info == NULL)
					*info = tmp;
				else
					tail->next = tmp;
				tail = tmp;
			}
		}
	}

	globfree(&globmatch);

	return (*info) ? 0 : -FI_ENODATA;
}
