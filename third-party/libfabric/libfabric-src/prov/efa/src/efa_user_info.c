/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

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

#if HAVE_CUDA || HAVE_NEURON || HAVE_SYNAPSEAI
/**
 * @brief determine if EFA provider should claim support of FI_HMEM in info
 * @param[in]	version		libfabric API version used by user
 * @return	true, if EFA provider should claim support of FI_HMEM
 * 		false, otherwise
 */
bool efa_user_info_should_support_hmem(int version)
{
	bool any_hmem, rdma_allowed;
	char *extra_info = "";
	int i;

	/* Note that the default behavior of EFA provider is different between
	 * libfabric API version when CUDA is used as HMEM system.
	 *
	 * For libfabric API version 1.17 and earlier, EFA provider does not
	 * support FI_HMEM on CUDA unless GPUDirect RDMA is available.
	 *
	 * For libfabric API version 1.18 and later, EFA provider will claim support
	 * of FI_HMEM on CUDA as long as CUDA library is initialized and a CUDA device is
	 * available. On an system without GPUDirect RDMA, the support of CUDA memory
	 * is implemented by calling CUDA library. If user does not want EFA provider
	 * to use CUDA library, the user can call use fi_setopt to set
	 * FI_OPT_CUDA_API_PERMITTED to false.
	 * On an system without GPUDirect RDMA, such a call would fail.
	 *
	 * For NEURON and SYNAPSEAI HMEM types, use_device_rdma is required no
	 * matter the API version, as is P2P support.
	 */
	if (hmem_ops[FI_HMEM_CUDA].initialized && FI_VERSION_GE(version, FI_VERSION(1, 18))) {
		EFA_INFO(FI_LOG_CORE,
			"User is using API version >= 1.18. CUDA library and "
			"devices are available, claim support of FI_HMEM.\n");
			/* For this API we can support HMEM regardless of
			   use_device_rdma and P2P support, because we can use
			   CUDA api calls.*/
			return true;
	}

	if (hmem_ops[FI_HMEM_CUDA].initialized) {
		extra_info = "For CUDA and libfabric API version <1.18 ";
	}

	any_hmem = false;
	EFA_HMEM_IFACE_FOREACH_NON_SYSTEM(i) {
		enum fi_hmem_iface hmem_iface = efa_hmem_ifaces[i];
		/* Note that .initialized doesn't necessarily indicate there are
		   hardware devices available, only that the libraries are
		   available. */
		if (hmem_ops[hmem_iface].initialized) {
			any_hmem = true;
		}
	}

	if (!any_hmem) {
		EFA_WARN(FI_LOG_CORE,
			"FI_HMEM cannot be supported because no compatible "
			"libraries were found.\n");
		return false;
	}

	/* All HMEM providers require P2P support. */
	if (ofi_hmem_p2p_disabled()) {
		EFA_WARN(FI_LOG_CORE,
			"%sFI_HMEM capability requires peer to peer "
			"support, which is disabled because "
			"FI_HMEM_P2P_DISABLED was set to 1/on/true.\n",
			extra_info);
		return false;
	}

	/* all devices require use_device_rdma. */
	if (!efa_device_support_rdma_read()) {
		EFA_WARN(FI_LOG_CORE,
		"%sEFA cannot support FI_HMEM because the EFA device "
		"does not have RDMA support.\n",
		extra_info);
		return false;
	}

	rdma_allowed = efa_rdm_get_use_device_rdma(version);
	/* not allowed to use rdma, but device supports it... */
	if (!rdma_allowed) {
		EFA_WARN(FI_LOG_CORE,
		"%sEFA cannot support FI_HMEM because the environment "
		"variable FI_EFA_USE_DEVICE_RDMA is 0.\n",
		extra_info);
		return false;
	}

	return true;
}

#else

bool efa_user_info_should_support_hmem(int version)
{
	EFA_WARN(FI_LOG_CORE,
		"EFA cannot support FI_HMEM because it was not compiled "
		"with any supporting FI_HMEM capabilities\n");
	return false;
}

#endif
/**
 * @brief update an info to match user hints
 *
 * the input info is a duplicate of prov info, which matches
 * the capability of the EFA device. This function tailor it
 * so it matches user provided hints
 *
 * @param	version[in]	libfabric API version
 * @param	info[in,out]	info to be updated
 * @param	hints[in]	user provided hints
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
static
int efa_user_info_alter_rdm(int version, struct fi_info *info, const struct fi_info *hints)
{
	uint64_t atomic_ordering;

	if (hints && (hints->caps & FI_HMEM)) {
		/*
		 * FI_HMEM is a primary capability, therefore only check
		 * (and cliam) its support when user explicitly requested it.
		 */
		if (!efa_user_info_should_support_hmem(version)) {
			return -FI_ENODATA;
		}

		info->caps |= FI_HMEM;
	} else {
		info->caps &= ~FI_HMEM;
	}

	if (info->caps & FI_HMEM) {
		/* Add FI_MR_HMEM to mr_mode when claiming support of FI_HMEM
		 * because EFA provider's HMEM support rely on
		 * application to provide descriptor for device buffer.
		 */
		if (hints->domain_attr &&
		    !(hints->domain_attr->mr_mode & FI_MR_HMEM)) {
			EFA_WARN(FI_LOG_CORE,
			        "FI_HMEM capability requires device registrations (FI_MR_HMEM)\n");
			return -FI_ENODATA;
		}

		info->domain_attr->mr_mode |= FI_MR_HMEM;
	}

	if (FI_VERSION_LT(version, FI_VERSION(1, 18)) && info->caps & FI_HMEM) {
		/* our HMEM atomic support rely on calls to CUDA API, which
		 * is disabled if user are using libfabric API version 1.17 and earlier.
		 */
		if (hints->caps & FI_ATOMIC) {
			EFA_WARN(FI_LOG_CORE,
			        "FI_ATOMIC capability with FI_HMEM relies on CUDA API, "
				"which is disable for libfabric API version 1.17 and eariler\n");
			return -FI_ENODATA;
		}
		info->caps &= ~FI_ATOMIC;
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
			EFA_INFO(FI_LOG_CORE,
				"FI_MSG_PREFIX supported by application.\n");
			info->mode |= FI_MSG_PREFIX;
			info->tx_attr->mode |= FI_MSG_PREFIX;
			info->rx_attr->mode |= FI_MSG_PREFIX;
			info->ep_attr->msg_prefix_size = EFA_RDM_MSG_PREFIX_SIZE;
			EFA_INFO(FI_LOG_CORE,
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
	const struct fi_info *prov_info;
	struct fi_info *dupinfo, *tail;
	int ret;

	ret = efa_user_info_check_hints_addr(node, service, flags, hints);
	if (ret) {
		*info = NULL;
		return ret;
	}

	if (hints) {
		ret = ofi_prov_check_info(&efa_util_prov, version, hints);
		if (ret) {
			*info = NULL;
			return ret;
		}
	}

	*info = tail = NULL;
	for (prov_info = efa_util_prov.info;
	     prov_info;
	     prov_info = prov_info->next) {

		if (prov_info->ep_attr->type != FI_EP_RDM)
			continue;

		ret = efa_prov_info_compare_src_addr(node, flags, hints, prov_info);
		if (ret)
			continue;

		ret = efa_prov_info_compare_domain_name(hints, prov_info);
		if (ret)
			continue;

		ret = efa_prov_info_compare_pci_bus_id(hints, prov_info);
		if (ret)
			continue;

		dupinfo = fi_dupinfo(prov_info);
		if (!dupinfo) {
			ret = -FI_ENOMEM;
			goto free_info;
		}

		ret = efa_user_info_set_dest_addr(node, service, flags, hints, dupinfo);
		if (ret)
			goto free_info;

		dupinfo->fabric_attr->api_version = version;

		ret = efa_user_info_alter_rdm(version, dupinfo, hints);
		if (ret)
			goto free_info;

		ofi_alter_info(dupinfo, hints, version);

		/* If application asked for FI_REMOTE_COMM but not FI_LOCAL_COMM, it
		 * does not want to use shm. In this case, we honor the request by
		 * unsetting the FI_LOCAL_COMM flag in info. This way efa_rdm_ep_open()
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

	if (hints && hints->ep_attr && hints->ep_attr->type == FI_EP_DGRAM)
		return efa_user_info_get_dgram(version, node, service, flags, hints, info);

	if (hints && hints->ep_attr && hints->ep_attr->type == FI_EP_RDM)
		return efa_user_info_get_rdm(version, node, service, flags, hints, info);

	if (hints && hints->ep_attr && hints->ep_attr->type != FI_EP_UNSPEC) {
		EFA_WARN(FI_LOG_DOMAIN, "unsupported endpoint type: %d\n",
			 hints->ep_attr->type);
		return -FI_ENODATA;
	}

	err = efa_user_info_get_dgram(version, node, service, flags, hints, &dgram_info_list);
	if (err && err != -FI_ENODATA) {
		return err;
	}

	err = efa_user_info_get_rdm(version, node, service, flags, hints, &rdm_info_list);
	if (err && err != -FI_ENODATA) {
		fi_freeinfo(dgram_info_list);
		return err;
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

	return -FI_ENODATA;
}

