/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include "efa.h"
#include "efa_prov_info.h"

static
int efa_check_hints_dgram(uint32_t version, const struct fi_info *hints,
			  const struct fi_info *info)
{
	uint64_t prov_mode;
	size_t size;
	int ret;

	if (hints->caps & ~(info->caps)) {
		EFA_INFO(FI_LOG_CORE, "Unsupported capabilities\n");
		OFI_INFO_CHECK(&efa_prov, info, hints, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	prov_mode = ofi_mr_get_prov_mode(version, hints, info);

	if ((hints->mode & prov_mode) != prov_mode) {
		EFA_INFO(FI_LOG_CORE, "Required hints mode bits not set\n");
		OFI_INFO_MODE(&efa_prov, prov_mode, hints->mode);
		return -FI_ENODATA;
	}

	if (hints->fabric_attr) {
		ret = ofi_check_fabric_attr(&efa_prov, info->fabric_attr,
					    hints->fabric_attr);

		if (ret)
			return ret;
	}

	switch (hints->addr_format) {
	case FI_FORMAT_UNSPEC:
	case FI_ADDR_EFA:
		size = EFA_EP_ADDR_LEN;
		break;
	default:
		EFA_INFO(FI_LOG_CORE,
			 "Address format not supported: hints[%u], supported[%u,%u]\n",
			 hints->addr_format, FI_FORMAT_UNSPEC, FI_ADDR_EFA);
		return -FI_ENODATA;
	}

	if (hints->src_addr && hints->src_addrlen < size)
		return -FI_ENODATA;

	if (hints->dest_addr && hints->dest_addrlen < size)
		return -FI_ENODATA;

	if (hints->domain_attr) {
		ret = ofi_check_domain_attr(&efa_prov, version, info->domain_attr, hints);
		if (ret)
			return ret;
	}

	if (hints->ep_attr) {
		ret = ofi_check_ep_attr(&efa_util_prov, info->fabric_attr->api_version, info, hints);
		if (ret)
			return ret;
	}

	if (hints->rx_attr) {
		ret = ofi_check_rx_attr(&efa_prov, info, hints->rx_attr, hints->mode);
		if (ret)
			return ret;
	}

	if (hints->tx_attr) {
		ret = ofi_check_tx_attr(&efa_prov, info->tx_attr, hints->tx_attr, hints->mode);
		if (ret)
			return ret;
	}

	return 0;
}

/**
 * @brief set the desc_addr field of user info
 *
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	service[in]	service from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	fi[out]		user_info object to be updated, can be a list of infos
 *
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
int efa_user_info_set_dest_addr(const char *node, const char *service, uint64_t flags,
				const struct fi_info *hints, struct fi_info *fi)
{
	struct efa_ep_addr tmp_addr;
	void *dest_addr = NULL;
	int ret = FI_SUCCESS;
	struct fi_info *cur;

	if (flags & FI_SOURCE) {
		if (hints && hints->dest_addr)
			dest_addr = hints->dest_addr;
	} else {
		if (node || service) {
			ret = efa_str_to_ep_addr(node, service, &tmp_addr);
			if (ret)
				return ret;
			dest_addr = &tmp_addr;
		} else if (hints && hints->dest_addr) {
			dest_addr = hints->dest_addr;
		}
	}

	if (dest_addr) {
		for (cur = fi; cur; cur = cur->next) {
			cur->dest_addr = malloc(EFA_EP_ADDR_LEN);
			if (!cur->dest_addr) {
				for (; fi->dest_addr; fi = fi->next)
					free(fi->dest_addr);
				return -FI_ENOMEM;
			}
			memcpy(cur->dest_addr, dest_addr, EFA_EP_ADDR_LEN);
			cur->dest_addrlen = EFA_EP_ADDR_LEN;
		}
	}
	return ret;
}

/**
 * @brief check the src_addr and desc_addr field of user provided hints
 *
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	service[in]	service from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	fi[out]		user_info object to be updated, can be a list of infos
 *
 * @return	0, if hints matches EFA provider's capability
 * 		-FI_ENODATA, otherwise
 */
int efa_user_info_check_hints_addr(const char *node, const char *service,
				   uint64_t flags, const struct fi_info *hints)
{
	if (!(flags & FI_SOURCE) && hints && hints->src_addr &&
	    hints->src_addrlen != EFA_EP_ADDR_LEN)
		return -FI_ENODATA;

	if (((!node && !service) || (flags & FI_SOURCE)) &&
	    hints && hints->dest_addr &&
	    hints->dest_addrlen != EFA_EP_ADDR_LEN)
		return -FI_ENODATA;

	return 0;
}

/**
 * @brief get a list of dgram info the fit user's requirements
 *
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	service[in]	service from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	info[out]	a linked list of user_info that met user's requirements
 */
int efa_user_info_get_dgram(uint32_t version, const char *node, const char *service,
			    uint64_t flags, const struct fi_info *hints, struct fi_info **user_info)
{
	int ret, i;
	struct fi_info *prov_info_dgram, *dupinfo, *tail;

	ret = efa_user_info_check_hints_addr(node, service, flags, hints);
	if (ret) {
		*user_info = NULL;
		return ret;
	}

	*user_info = NULL;
	dupinfo = NULL;
	tail = NULL;
	for (i = 0; i < g_device_cnt; ++i) {
		prov_info_dgram = g_device_list[i].dgram_info;

		ret = efa_prov_info_compare_src_addr(node, flags, hints, prov_info_dgram);
		if (ret)
			continue;

		ret = efa_prov_info_compare_domain_name(hints, prov_info_dgram);
		if (ret)
			continue;

		ret = efa_prov_info_compare_pci_bus_id(hints, prov_info_dgram);
		if (ret)
			continue;

		EFA_INFO(FI_LOG_FABRIC, "found match for interface %s %s\n",
			 node, prov_info_dgram->fabric_attr->name);
		if (hints) {
			ret = efa_check_hints_dgram(version, hints, prov_info_dgram);
			if (ret)
				continue;
		}

		dupinfo = fi_dupinfo(prov_info_dgram);
		if (!dupinfo) {
			ret = -FI_ENOMEM;
			goto err_free;
		}

		dupinfo->fabric_attr->api_version = version;

		if (!*user_info)
			*user_info = dupinfo;
		else
			tail->next = dupinfo;
		tail = dupinfo;
		dupinfo = NULL;
	}

	if (!*user_info) {
		ret = -FI_ENODATA;
		goto err_free;
	}

	ret = efa_user_info_set_dest_addr(node, service, flags, hints, *user_info);
	if (ret)
		goto err_free;

	ofi_alter_info(*user_info, hints, version);
	return 0;

err_free:
	fi_freeinfo(dupinfo);
	fi_freeinfo(*user_info);
	*user_info = NULL;
	return -FI_ENODATA;
}

/**
 * @brief update an info to match user hints
 *
 * the input info is a duplicate of prov info, which matches
 * the capability of the EFA device. This function tailor it
 * so it matches user provided hints
 *
 * @param	info[in,out]	info to be updated
 * @param	hints[in]	user provided hints
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
static
int efa_user_info_alter_rxr(struct fi_info *info, const struct fi_info *hints)
{
	uint64_t atomic_ordering;

	/*
	 * Do not advertise FI_HMEM capabilities when the core can not support
	 * it or when the application passes NULL hints (given this is a primary
	 * cap). The logic for device-specific checks pertaining to HMEM comes
	 * further along this path.
	 */
	if (!hints) {
		info->caps &= ~FI_HMEM;
	}

	/*
	 * Handle user-provided hints and adapt the info object passed back up
	 * based on EFA-specific constraints.
	 */
	if (hints) {
		if (hints->tx_attr) {
			atomic_ordering = FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW |
					  FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW;
			if (!(hints->tx_attr->msg_order & atomic_ordering)) {
				info->ep_attr->max_order_raw_size = 0;
			}
		}

		/* We only support manual progress for RMA operations */
		if (hints->caps & FI_RMA) {
			info->domain_attr->control_progress = FI_PROGRESS_MANUAL;
			info->domain_attr->data_progress = FI_PROGRESS_MANUAL;
		}


#if HAVE_CUDA || HAVE_NEURON || HAVE_SYNAPSEAI
		/* If the application requires HMEM support, we will add
		 * FI_MR_HMEM to mr_mode, because we need application to
		 * provide descriptor for device buffer. Note we did
		 * not add FI_MR_LOCAL here because according to FI_MR man
		 * page:
		 *
		 *     "If FI_MR_HMEM is set, but FI_MR_LOCAL is unset,
		 *      only device buffers must be registered when used locally.
		 *      "
		 * which means FI_MR_HMEM implies FI_MR_LOCAL for device buffer.
		 */
		if (hints->caps & FI_HMEM) {
			if (ofi_hmem_p2p_disabled()) {
				EFA_WARN(FI_LOG_CORE,
					"FI_HMEM capability currently requires peer to peer support, which is disabled.\n");
				return -FI_ENODATA;
			}
			//TODO: remove the rdma checks once FI_HMEM w/o p2p is supported

			if (!efa_device_support_rdma_read()) {
				EFA_WARN(FI_LOG_CORE,
				        "FI_HMEM capability requires RDMA, which this device does not support.\n");
				return -FI_ENODATA;

			}

			if (!rxr_env.use_device_rdma) {
				EFA_WARN(FI_LOG_CORE,
				        "FI_HMEM capability requires RDMA, which is turned off. You can turn it on by set environment variable FI_EFA_USE_DEVICE_RDMA to 1.\n");
				return -FI_ENODATA;
			}

			if (hints->domain_attr &&
			    !(hints->domain_attr->mr_mode & FI_MR_HMEM)) {
				EFA_WARN(FI_LOG_CORE,
				        "FI_HMEM capability requires device registrations (FI_MR_HMEM)\n");
				return -FI_ENODATA;
			}

			info->domain_attr->mr_mode |= FI_MR_HMEM;

		} else {
			/*
			 * FI_HMEM is a primary capability. Providers should
			 * only enable it if requested by applications.
			 */
			info->caps &= ~FI_HMEM;
		}
#endif
		/*
		 * The provider does not force applications to register buffers
		 * with the device, but if an application is able to, reuse
		 * their registrations and avoid the bounce buffers.
		 */
		if (hints->domain_attr && hints->domain_attr->mr_mode & FI_MR_LOCAL)
			info->domain_attr->mr_mode |= FI_MR_LOCAL;

		/*
		 * Same goes for prefix mode, where the protocol does not
		 * absolutely need a prefix before receive buffers, but it can
		 * use it when available to optimize transfers with endpoints
		 * having the following profile:
		 *	- Requires FI_MSG and not FI_TAGGED/FI_ATOMIC/FI_RMA
		 *	- Can handle registrations (FI_MR_LOCAL)
		 *	- No need for FI_DIRECTED_RECV
		 *	- Guaranteed to send msgs smaller than info->nic->link_attr->mtu
		 */
		if (hints->mode & FI_MSG_PREFIX) {
			FI_INFO(&rxr_prov, FI_LOG_CORE,
				"FI_MSG_PREFIX supported by application.\n");
			info->mode |= FI_MSG_PREFIX;
			info->tx_attr->mode |= FI_MSG_PREFIX;
			info->rx_attr->mode |= FI_MSG_PREFIX;
			info->ep_attr->msg_prefix_size = RXR_MSG_PREFIX_SIZE;
			FI_INFO(&rxr_prov, FI_LOG_CORE,
				"FI_MSG_PREFIX size = %ld\n", info->ep_attr->msg_prefix_size);
		}
	}

	/* Use a table for AV if the app has no strong requirement */
	if (!hints || !hints->domain_attr ||
	    hints->domain_attr->av_type == FI_AV_UNSPEC)
		info->domain_attr->av_type = FI_AV_TABLE;
	else
		info->domain_attr->av_type = hints->domain_attr->av_type;

	if (!hints || !hints->domain_attr ||
	    hints->domain_attr->resource_mgmt == FI_RM_UNSPEC)
		info->domain_attr->resource_mgmt = FI_RM_ENABLED;
	else
		info->domain_attr->resource_mgmt = hints->domain_attr->resource_mgmt;

	return 0;
}

/**
 * @brief get a list of rdm info the fit user's requirements
 *
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	service[in]	service from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	info[out]	a linked list of user_info that met user's requirements
 * @return 	0 on success
 * 		negative libfabric error code on failure
 */
static
int efa_user_info_get_rdm(uint32_t version, const char *node,
			  const char *service, uint64_t flags,
			  const struct fi_info *hints, struct fi_info **info)
{
	const struct fi_info *prov_info_rxr;
	struct fi_info *dupinfo, *tail;
	int ret;

	ret = efa_user_info_check_hints_addr(node, service, flags, hints);
	if (ret) {
		*info = NULL;
		return ret;
	}

	if (hints) {
		ret = ofi_prov_check_info(&rxr_util_prov, version, hints);
		if (ret) {
			*info = NULL;
			return ret;
		}
	}

	*info = tail = NULL;
	for (prov_info_rxr = rxr_util_prov.info;
	     prov_info_rxr;
	     prov_info_rxr = prov_info_rxr->next) {
		ret = efa_prov_info_compare_src_addr(node, flags, hints, prov_info_rxr);
		if (ret)
			continue;

		ret = efa_prov_info_compare_domain_name(hints, prov_info_rxr);
		if (ret)
			continue;

		ret = efa_prov_info_compare_pci_bus_id(hints, prov_info_rxr);
		if (ret)
			continue;

		dupinfo = fi_dupinfo(prov_info_rxr);
		if (!dupinfo) {
			ret = -FI_ENOMEM;
			goto free_info;
		}

		ret = efa_user_info_set_dest_addr(node, service, flags, hints, dupinfo);
		if (ret)
			goto free_info;

		dupinfo->fabric_attr->api_version = version;

		ret = efa_user_info_alter_rxr(dupinfo, hints);
		if (ret)
			goto free_info;

		ofi_alter_info(dupinfo, hints, version);

		/* If application asked for FI_REMOTE_COMM but not FI_LOCAL_COMM, it
		 * does not want to use shm. In this case, we honor the request by
		 * unsetting the FI_LOCAL_COMM flag in info. This way rxr_endpoint()
		 * should disable shm transfer for the endpoint
		 */
		if (hints && hints->caps & FI_REMOTE_COMM && !(hints->caps & FI_LOCAL_COMM))
			dupinfo->caps &= ~FI_LOCAL_COMM;

		if (!*info)
			*info = dupinfo;
		else
			tail->next = dupinfo;
		tail = dupinfo;
	}

	return 0;
free_info:
	fi_freeinfo(dupinfo);
	fi_freeinfo(*info);
	*info = NULL;
	return ret;
}

/**
 * @brief get a list of info the fit user's requirements
 * 
 * This is EFA provider's implemenation of fi_getinfo() API.
 *
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	service[in]	service from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	info[out]	a linked list of user_info that met user's requirements
 * @return 	0 on success
 * 		negative libfabric error code on failure
 */
int efa_getinfo(uint32_t version, const char *node,
		const char *service, uint64_t flags,
		const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *dgram_info_list, *rdm_info_list;
	int err;
	static bool shm_info_initialized = false;

	/*
	 * efa_shm_info_initialize() initializes the global variable g_shm_info.
	 * Ideally it should be called during provider initialization. However,
	 * At the time of EFA provider initialization, shm provider has not been
	 * initialized yet, therefore g_shm_info cannot be initialized. As a workaround,
	 * we initialize g_shm_info when the rxr_getinfo() is called 1st time,
	 * at this point all the providers have been initialized.
	 */
	if (!shm_info_initialized) {
		efa_shm_info_initialize(hints);
		shm_info_initialized = true;
	}

	if (hints && hints->ep_attr && hints->ep_attr->type == FI_EP_DGRAM) {
		err = efa_user_info_get_dgram(version, node, service, flags, hints, info);
		if (err)
			goto error;
		return 0;
	}

	if (hints && hints->ep_attr && hints->ep_attr->type == FI_EP_RDM) {
		err = efa_user_info_get_rdm(version, node, service, flags, hints, info);
		if (err)
			goto error;
		return 0;
	}

	if (hints && hints->ep_attr && hints->ep_attr->type != FI_EP_UNSPEC) {
		EFA_WARN(FI_LOG_DOMAIN, "unsupported endpoint type: %d\n",
			 hints->ep_attr->type);
		err = -FI_ENODATA;
		goto error;
	}

	err = efa_user_info_get_dgram(version, node, service, flags, hints, &dgram_info_list);
	if (err && err != -FI_ENODATA) {
		goto error;
	}

	err = efa_user_info_get_rdm(version, node, service, flags, hints, &rdm_info_list);
	if (err && err != -FI_ENODATA) {
		fi_freeinfo(dgram_info_list);
		goto error;
	}

	if (rdm_info_list && dgram_info_list) {
		struct fi_info *tail;

		tail = rdm_info_list;
		while (tail->next)
			tail = tail->next;

		tail->next = dgram_info_list;
		*info = rdm_info_list;
		return 0;
	}

	if (rdm_info_list) {
		assert(!dgram_info_list);
		*info = rdm_info_list;
		return 0;
	}

	if (dgram_info_list) {
		assert(!rdm_info_list);
		*info = dgram_info_list;
		return 0;
	}

	err = -FI_ENODATA;
error:
	if (shm_info_initialized) {
		efa_shm_info_finalize();
		shm_info_initialized = false;
	}
	return err;
}

