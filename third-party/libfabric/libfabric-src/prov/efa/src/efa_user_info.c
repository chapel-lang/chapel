/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_prov_info.h"

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
 * @brief check if user-provided fabric object is valid and assign it
 *
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	dupinfo[out]	provider info object to update
 * @param	prov_info[in]	provider info object for validation
 * @return	0 if valid, negative libfabric error code on failure
 */
int efa_user_info_check_fabric_object(const struct fi_info *hints,
				      struct fi_info *dupinfo,
				      const struct fi_info *prov_info)
{
	struct util_fabric *util_fabric;
	struct fi_fabric_attr fabric_attr;
	int ret;

	if (!hints || !hints->fabric_attr || !hints->fabric_attr->fabric)
		return 0;

	if (hints->fabric_attr->fabric->fid.fclass != FI_CLASS_FABRIC) {
		EFA_WARN(FI_LOG_CORE, "Invalid fabric object\n");
		return -FI_EINVAL;
	}

	util_fabric = container_of(hints->fabric_attr->fabric, struct util_fabric, fabric_fid);
	fabric_attr.name = (char *)util_fabric->name;
	fabric_attr.prov_name = (char *)util_fabric->prov->name;
	fabric_attr.prov_version = util_fabric->prov->version;
	fabric_attr.api_version = hints->fabric_attr->fabric->api_version;

	ret = ofi_check_fabric_attr(efa_util_prov.prov, 
				    prov_info->fabric_attr, 
				    &fabric_attr);
	if (ret) {
		EFA_WARN(FI_LOG_CORE, "Fabric attributes validation failed\n");
		return ret;
	}

	dupinfo->fabric_attr->fabric = hints->fabric_attr->fabric;
	return 0;
}

/**
 * @brief check if user-provided domain object is valid and assign it
 *
 * @param	hints[in]	hints from user's call to fi_getinfo()
 * @param	dupinfo[out]	provider info object to update
 * @return	0 if valid, negative libfabric error code on failure
 */
int efa_user_info_check_domain_object(const struct fi_info *hints,
				      struct fi_info *dupinfo)
{
	struct util_domain *util_domain;
	struct util_fabric *util_fabric;
	struct dlist_entry *item;
	bool domain_found = false;

	if (!hints || !hints->domain_attr || !hints->domain_attr->domain)
		return 0;

	if (hints->domain_attr->domain->fid.fclass != FI_CLASS_DOMAIN) {
		EFA_WARN(FI_LOG_CORE, "Invalid domain object\n");
		return -FI_EINVAL;
	}

	util_domain = container_of(hints->domain_attr->domain, struct util_domain, domain_fid);
	util_fabric = util_domain->fabric;

	if (strcmp(util_domain->name, dupinfo->domain_attr->name))
		return -FI_EINVAL;

	ofi_mutex_lock(&util_fabric->lock);
	dlist_foreach(&util_fabric->domain_list, item) {
		struct util_domain *list_domain = container_of(item, struct util_domain, list_entry);
		if (list_domain == util_domain) {
			domain_found = true;
			break;
		}
	}
	ofi_mutex_unlock(&util_fabric->lock);

	if (!domain_found) {
		EFA_WARN(FI_LOG_CORE, "Domain object does not exist\n");
		return -FI_EINVAL;
	}

	dupinfo->domain_attr->domain = hints->domain_attr->domain;

	util_domain->info_domain_caps |= dupinfo->caps | dupinfo->domain_attr->caps;
	util_domain->info_domain_mode |= dupinfo->mode | dupinfo->domain_attr->mode;
	util_domain->mr_mode |= dupinfo->domain_attr->mr_mode;

	return 0;
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
 * @brief update RDM info to match user hints
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
			/* efa device doesn't have ordering,
			 * if apps request an ordering that is relaxed than
			 * what provider supports, we should respect that.
			 * If no ordering is specified,
			 * the default message order supported by the provider is returned.
			 */
			info->tx_attr->msg_order &= hints->tx_attr->msg_order;

			/* If no atomic ordering is requested, set the max_order_*_size as 0 */
			if (!(hints->tx_attr->msg_order & FI_ORDER_ATOMIC_RAW))
				info->ep_attr->max_order_raw_size = 0;
			if (!(hints->tx_attr->msg_order & FI_ORDER_ATOMIC_WAR))
				info->ep_attr->max_order_war_size = 0;
			if (!(hints->tx_attr->msg_order & FI_ORDER_ATOMIC_WAW))
				info->ep_attr->max_order_waw_size = 0;
		}

		if (hints->rx_attr) {
			/* efa device doesn't have ordering,
			 * if apps request an ordering that is relaxed than
			 * what provider supports, we should respect that.
			 * If no ordering is specified,
			 * the default message order supported by the provider is returned.
			 */
			info->rx_attr->msg_order &= hints->rx_attr->msg_order;
		}

		if (info->tx_attr->msg_order != info->rx_attr->msg_order)
			EFA_INFO(FI_LOG_EP_CTRL, "Inconsistent tx/rx msg order. Tx msg order: %lu, Rx msg order: %lu. "
						 "Libfabric can proceed but it is recommended to align the tx and rx msg order.\n",
						 info->tx_attr->msg_order, info->rx_attr->msg_order);

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

	/* Print a warning and use FI_AV_TABLE if the app requests FI_AV_MAP */
	if (hints && hints->domain_attr && hints->domain_attr->av_type == FI_AV_MAP)
		EFA_INFO(FI_LOG_CORE, "FI_AV_MAP is deprecated in Libfabric 2.x. Please use FI_AV_TABLE. "
					"EFA provider will now switch to using FI_AV_TABLE.\n");
	info->domain_attr->av_type = FI_AV_TABLE;

	if (!hints || !hints->domain_attr ||
	    hints->domain_attr->resource_mgmt == FI_RM_UNSPEC)
		info->domain_attr->resource_mgmt = FI_RM_ENABLED;
	else
		info->domain_attr->resource_mgmt = hints->domain_attr->resource_mgmt;

	return 0;
}

/**
 * @brief update EFA direct info to match user hints
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
int efa_user_info_alter_direct(int version, struct fi_info *info, const struct fi_info *hints)
{
	/*
	 * FI_HMEM is a primary capability, therefore only check
	 * and claim support when explicitly requested
	 */
	if (hints && (hints->caps & FI_HMEM))
		info->caps |= FI_HMEM;
	else
		info->caps &= ~FI_HMEM;

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

	/*
	 * Handle user-provided hints and adapt the info object passed back up
	 * based on EFA-specific constraints.
	 */
	if (hints) {
		/* EFA direct cannot make use of message prefix */
		if (hints->mode & FI_MSG_PREFIX) {
			EFA_INFO(FI_LOG_CORE,
				"FI_MSG_PREFIX supported by application but EFA direct cannot "
				"use prefix. Setting prefix size to 0.\n");
			info->ep_attr->msg_prefix_size = 0;
			EFA_INFO(FI_LOG_CORE,
				"FI_MSG_PREFIX size = %ld\n", info->ep_attr->msg_prefix_size);
		}
		/* When user doesn't request FI_RMA, the max_msg_size should be returned
		 * as the MSG only as RMA will not be used.
		 */
		if (!(hints->caps & FI_RMA))
			info->ep_attr->max_msg_size = g_efa_selected_device_list[0].ibv_port_attr.max_msg_sz;
	}

	/* Print a warning and use FI_AV_TABLE if the app requests FI_AV_MAP */
	if (hints && hints->domain_attr && hints->domain_attr->av_type == FI_AV_MAP)
		EFA_INFO(FI_LOG_CORE, "FI_AV_MAP is deprecated in Libfabric 2.x. Please use FI_AV_TABLE. "
					"EFA direct provider will now switch to using FI_AV_TABLE.\n");
	info->domain_attr->av_type = FI_AV_TABLE;

	return 0;
}

/**
 * @brief check if there is an existing open domain that matches the name, caps,
 * mode, and mr_mode. It compares domain to prov_info instead of user_info
 * because user_info was trimmed based on user hints. We can reuse the domain as
 * long as the provider supports.
 *
 */
static int efa_find_domain(struct dlist_entry *item, const void *arg)
{
	const struct util_domain *domain;
	const struct fi_info *prov_info = arg;

	domain = container_of(item, struct util_domain, list_entry);

	return !strcmp(domain->name, prov_info->domain_attr->name) &&
	       (((prov_info->caps | prov_info->domain_attr->caps) &
		 domain->info_domain_caps) == domain->info_domain_caps) &&
	       (((prov_info->mode | prov_info->domain_attr->mode) &
		 domain->info_domain_mode) == domain->info_domain_mode) &&
	       ((prov_info->domain_attr->mr_mode & domain->mr_mode) ==
		domain->mr_mode);
}

/**
 * @brief get a list of fi_info objects the fit user's requirements
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
int efa_get_user_info(uint32_t version, const char *node,
			  const char *service, uint64_t flags,
			  const struct fi_info *hints, struct fi_info **info)
{
	const struct fi_info *prov_info;
	struct fi_info *dupinfo, *tail;
	struct util_fabric *fabric;
	struct util_domain *domain;
	struct dlist_entry *item;
	int ret;

	ret = efa_user_info_check_hints_addr(node, service, flags, hints);
	if (ret) {
		*info = NULL;
		return ret;
	}

	*info = tail = NULL;
	for (prov_info = efa_util_prov.info;
	     prov_info;
	     prov_info = prov_info->next) {

		ret = ofi_check_info(&efa_util_prov, prov_info, version, hints);
		if (ret)
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

		if (EFA_INFO_TYPE_IS_RDM(prov_info)) {
			ret = efa_user_info_alter_rdm(version, dupinfo, hints);
			if (ret)
				goto free_info;

			/* If application asked for FI_REMOTE_COMM but not FI_LOCAL_COMM, it
			 * does not want to use shm. In this case, we honor the request by
			 * unsetting the FI_LOCAL_COMM flag in info. This way efa_rdm_ep_open()
			 * should disable shm transfer for the endpoint
			 */
			if (hints && hints->caps & FI_REMOTE_COMM && !(hints->caps & FI_LOCAL_COMM))
				dupinfo->caps &= ~FI_LOCAL_COMM;
		}

		if (EFA_INFO_TYPE_IS_DIRECT(prov_info)) {
			ret = efa_user_info_alter_direct(version, dupinfo, hints);
			if (ret)
				goto free_info;
		}

		ofi_alter_info(dupinfo, hints, version);

		/* On input to fi_getinfo, a user may set this to an opened
		 * fabric/domain instance to restrict output to the given fabric/domain. */
		ret = efa_user_info_check_fabric_object(hints, dupinfo, prov_info);
		if (ret)
			continue;

		ret = efa_user_info_check_domain_object(hints, dupinfo);
		if (ret)
			continue;

		if (!dupinfo->fabric_attr->fabric && !dupinfo->domain_attr->domain)
			util_lookup_existing_fabric_domain(&efa_util_prov, hints, &dupinfo);

		if (hints && hints->domain_attr && hints->domain_attr->name &&
		    dupinfo->fabric_attr->fabric) {
			fabric = container_of(dupinfo->fabric_attr->fabric, struct util_fabric, fabric_fid);
			EFA_INFO(FI_LOG_CORE, "Reusing open fabric %s\n", fabric->name);

			/* Use efa specific efa_find_domain instead of util_find_domain */
			ofi_mutex_lock(&fabric->lock);
			if (!dupinfo->domain_attr->domain) {
				item = dlist_find_first_match(&fabric->domain_list, efa_find_domain, prov_info);
				if (item) {
					domain = container_of(item, struct util_domain, list_entry);
					ofi_genlock_lock(&domain->lock);
					dupinfo->domain_attr->domain = &domain->domain_fid;
					EFA_INFO(FI_LOG_CORE, "Reusing open domain %s\n", domain->name);
					domain->info_domain_caps |= dupinfo->caps | dupinfo->domain_attr->caps;
					domain->info_domain_mode |= dupinfo->mode | dupinfo->domain_attr->mode;
					domain->mr_mode |= dupinfo->domain_attr->mr_mode;
					ofi_genlock_unlock(&domain->lock);
				}
			}
			ofi_mutex_unlock(&fabric->lock);
		}

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
	struct fi_info *info_list;
	enum fi_ep_type hints_ep_type;
	int err;

	hints_ep_type = FI_EP_UNSPEC;
	if (hints && hints->ep_attr) {
		hints_ep_type = hints->ep_attr->type;
	}

	if (hints_ep_type != FI_EP_UNSPEC && hints_ep_type != FI_EP_RDM && hints_ep_type != FI_EP_DGRAM) {
		EFA_WARN(FI_LOG_DOMAIN, "unsupported endpoint type: %d\n",
			 hints_ep_type);
		return -FI_ENODATA;
	}

	err = efa_get_user_info(version, node, service, flags, hints, &info_list);
	if (err && err != -FI_ENODATA) {
		return err;
	}

	*info = info_list;
	return FI_SUCCESS;
}
