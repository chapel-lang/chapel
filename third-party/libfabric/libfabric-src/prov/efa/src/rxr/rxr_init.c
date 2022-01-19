/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "rxr.h"
#include "efa.h"
#include "ofi_hmem.h"

struct fi_info *shm_info;

struct fi_provider *lower_efa_prov;
struct efa_ep_addr *local_efa_addr;


struct rxr_env rxr_env = {
	.rx_window_size	= RXR_DEF_MAX_RX_WINDOW,
	.tx_max_credits = RXR_DEF_MAX_TX_CREDITS,
	.tx_min_credits = RXR_DEF_MIN_TX_CREDITS,
	.tx_queue_size = 0,
	.enable_shm_transfer = 1,
	.use_device_rdma = 0,
	.use_zcpy_rx = 1,
	.zcpy_rx_seed = 0,
	.shm_av_size = 128,
	.shm_max_medium_size = 4096,
	.recvwin_size = RXR_RECVWIN_SIZE,
	.ooo_pool_chunk_size = 64,
	.unexp_pool_chunk_size = 1024,
	.readcopy_pool_size = 256,
	.atomrsp_pool_size = 1024,
	.cq_size = RXR_DEF_CQ_SIZE,
	.max_memcpy_size = 4096,
	.mtu_size = 0,
	.tx_size = 0,
	.rx_size = 0,
	.tx_iov_limit = 0,
	.rx_iov_limit = 0,
	.rx_copy_unexp = 1,
	.rx_copy_ooo = 1,
	.rnr_backoff_wait_time_cap = RXR_DEFAULT_RNR_BACKOFF_WAIT_TIME_CAP,
	.rnr_backoff_initial_wait_time = 0, /* 0 is random wait time  */
	.efa_cq_read_size = 50,
	.shm_cq_read_size = 50,
	.efa_max_medium_msg_size = 65536,
	.efa_min_read_msg_size = 1048576,
	.efa_min_read_write_size = 65536,
	.efa_read_segment_size = 1073741824,
	.rnr_retry = 3, /* Setting this value to EFA_RNR_INFINITE_RETRY makes the firmware retry indefinitey */
};

/* @brief Read and store the FI_EFA_* environment variables.
 */
static void rxr_init_env(void)
{
	int fork_safe = 0;

	fi_param_get_int(&rxr_prov, "rx_window_size", &rxr_env.rx_window_size);
	fi_param_get_int(&rxr_prov, "tx_max_credits", &rxr_env.tx_max_credits);
	fi_param_get_int(&rxr_prov, "tx_min_credits", &rxr_env.tx_min_credits);
	fi_param_get_int(&rxr_prov, "tx_queue_size", &rxr_env.tx_queue_size);
	fi_param_get_int(&rxr_prov, "enable_shm_transfer", &rxr_env.enable_shm_transfer);
	fi_param_get_int(&rxr_prov, "use_device_rdma", &rxr_env.use_device_rdma);
	fi_param_get_int(&rxr_prov, "use_zcpy_rx", &rxr_env.use_zcpy_rx);
	fi_param_get_int(&rxr_prov, "zcpy_rx_seed", &rxr_env.zcpy_rx_seed);
	fi_param_get_int(&rxr_prov, "shm_av_size", &rxr_env.shm_av_size);
	fi_param_get_int(&rxr_prov, "shm_max_medium_size", &rxr_env.shm_max_medium_size);
	fi_param_get_int(&rxr_prov, "recvwin_size", &rxr_env.recvwin_size);
	fi_param_get_int(&rxr_prov, "readcopy_pool_size", &rxr_env.readcopy_pool_size);
	fi_param_get_int(&rxr_prov, "cq_size", &rxr_env.cq_size);
	fi_param_get_size_t(&rxr_prov, "max_memcpy_size",
			    &rxr_env.max_memcpy_size);
	fi_param_get_bool(&rxr_prov, "mr_cache_enable",
			  &efa_mr_cache_enable);
	fi_param_get_size_t(&rxr_prov, "mr_max_cached_count",
			    &efa_mr_max_cached_count);
	fi_param_get_size_t(&rxr_prov, "mr_max_cached_size",
			    &efa_mr_max_cached_size);
	fi_param_get_size_t(&rxr_prov, "mtu_size",
			    &rxr_env.mtu_size);
	fi_param_get_size_t(&rxr_prov, "tx_size", &rxr_env.tx_size);
	fi_param_get_size_t(&rxr_prov, "rx_size", &rxr_env.rx_size);
	fi_param_get_size_t(&rxr_prov, "tx_iov_limit", &rxr_env.tx_iov_limit);
	fi_param_get_size_t(&rxr_prov, "rx_iov_limit", &rxr_env.rx_iov_limit);
	fi_param_get_bool(&rxr_prov, "rx_copy_unexp",
			  &rxr_env.rx_copy_unexp);
	fi_param_get_bool(&rxr_prov, "rx_copy_ooo",
			  &rxr_env.rx_copy_ooo);

	fi_param_get_int(&rxr_prov, "max_timeout", &rxr_env.rnr_backoff_wait_time_cap);
	if (rxr_env.rnr_backoff_wait_time_cap > RXR_MAX_RNR_BACKOFF_WAIT_TIME_CAP)
		rxr_env.rnr_backoff_wait_time_cap = RXR_MAX_RNR_BACKOFF_WAIT_TIME_CAP;

	fi_param_get_int(&rxr_prov, "timeout_interval",
			 &rxr_env.rnr_backoff_initial_wait_time);
	fi_param_get_size_t(&rxr_prov, "efa_cq_read_size",
			 &rxr_env.efa_cq_read_size);
	fi_param_get_size_t(&rxr_prov, "shm_cq_read_size",
			 &rxr_env.shm_cq_read_size);
	fi_param_get_size_t(&rxr_prov, "inter_max_medium_message_size",
			    &rxr_env.efa_max_medium_msg_size);
	fi_param_get_size_t(&rxr_prov, "inter_min_read_message_size",
			    &rxr_env.efa_min_read_msg_size);
	fi_param_get_size_t(&rxr_prov, "inter_min_read_write_size",
			    &rxr_env.efa_min_read_write_size);
	fi_param_get_size_t(&rxr_prov, "inter_read_segment_size",
			    &rxr_env.efa_read_segment_size);

	/* Initialize EFA's fork support flag based on the environment and
	 * system support. */
	efa_fork_status = EFA_FORK_SUPPORT_OFF;

#if HAVE_IBV_IS_FORK_INITIALIZED == 1
	if (ibv_is_fork_initialized() == IBV_FORK_UNNEEDED)
		efa_fork_status = EFA_FORK_SUPPORT_UNNEEDED;
#endif

	if (efa_fork_status != EFA_FORK_SUPPORT_UNNEEDED) {
		fi_param_get_bool(&rxr_prov, "fork_safe", &fork_safe);

		/*
		 * Check if any environment variables which would trigger
		 * libibverbs' fork support are set. These variables are
		 * defined by ibv_fork_init(3).
		 */
		if (fork_safe || getenv("RDMAV_FORK_SAFE") || getenv("IBV_FORK_SAFE"))
			efa_fork_status = EFA_FORK_SUPPORT_ON;
	}
}

/* @brief convert raw address to an unique shm endpoint name (smr_name)
 *
 * Note even though all shm endpoints are on same instance. But because
 * one instance can have multiple EFA device, it is still necessary
 * to include GID on the name.
 *
 * a smr name consist of the following 4 parts:
 *
 *    GID:   ipv6 address from inet_ntop
 *    QPN:   %04x format
 *    QKEY:  %08x format
 *    UID:   %04x format
 *
 * each part is connected via an underscore.
 *
 * The following is an example:
 *
 *    fe80::4a5:28ff:fe98:e500_0001_12918366_03e8
 *
 * @param[in]	ptr		pointer to raw address (struct efa_ep_addr)
 * @param[out]	smr_name	an unique name for shm ep
 * @return	0 on success.
 * 		negative error code on failure.
 */
int rxr_raw_addr_to_smr_name(void *ptr, char *smr_name)
{
	struct efa_ep_addr *raw_addr;
	char gidstr[INET6_ADDRSTRLEN] = { 0 };
	int ret;

	raw_addr = (struct efa_ep_addr *)ptr;
	if (!inet_ntop(AF_INET6, raw_addr->raw, gidstr, INET6_ADDRSTRLEN)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Failed to convert GID to string errno: %d\n", errno);
		return -errno;
	}

	ret = snprintf(smr_name, EFA_SHM_NAME_MAX, "%s_%04x_%08x_%04x",
		       gidstr, raw_addr->qpn, raw_addr->qkey, getuid());
	if (ret <= 0)
		return ret;

	if (ret >= EFA_SHM_NAME_MAX)
		return -FI_EINVAL;

	return FI_SUCCESS;
}

void rxr_info_to_core_mr_modes(uint32_t version,
			       const struct fi_info *hints,
			       struct fi_info *core_info)
{
	if (hints && hints->domain_attr &&
	    (hints->domain_attr->mr_mode & (FI_MR_SCALABLE | FI_MR_BASIC))) {
		core_info->mode = FI_LOCAL_MR | FI_MR_ALLOCATED;
		core_info->domain_attr->mr_mode = hints->domain_attr->mr_mode;
	} else if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		core_info->mode |= FI_LOCAL_MR | FI_MR_ALLOCATED;
		core_info->domain_attr->mr_mode = FI_MR_UNSPEC;
	} else {
		core_info->domain_attr->mr_mode |=
			FI_MR_LOCAL | FI_MR_ALLOCATED;
		if (!hints)
			core_info->domain_attr->mr_mode |= OFI_MR_BASIC_MAP;
		else {
			if (hints->domain_attr)
				core_info->domain_attr->mr_mode |=
					hints->domain_attr->mr_mode & OFI_MR_BASIC_MAP;
			core_info->addr_format = hints->addr_format;
		}
#if HAVE_LIBCUDA
		core_info->domain_attr->mr_mode |= FI_MR_HMEM;
#endif
	}
}

static int rxr_copy_attr(const struct fi_info *info, struct fi_info *dup)
{
	if (info->src_addr) {
		dup->src_addrlen = info->src_addrlen;
		dup->src_addr = mem_dup(info->src_addr,
					info->src_addrlen);
		if (!dup->src_addr)
			return -FI_ENOMEM;
	}
	if (info->dest_addr) {
		dup->dest_addrlen = info->dest_addrlen;
		dup->dest_addr = mem_dup(info->dest_addr,
					 info->dest_addrlen);
		if (!dup->dest_addr)
			return -FI_ENOMEM;
	}
	if (info->fabric_attr) {
		if (info->fabric_attr->name) {
			dup->fabric_attr->name =
				strdup(info->fabric_attr->name);
			if (!dup->fabric_attr->name)
				return -FI_ENOMEM;
		}
	}
	if (info->domain_attr) {
		if (info->domain_attr->name) {
			dup->domain_attr->name =
				strdup(info->domain_attr->name);
			if (!dup->domain_attr->name)
				return -FI_ENOMEM;
		}
	}
	if (info->nic) {
		dup->nic = ofi_nic_dup(info->nic);
		if (!dup->nic)
			return -FI_ENOMEM;
	}
	if (info->caps & FI_HMEM)
		dup->caps |= FI_HMEM;

	return 0;
}

static int rxr_info_to_core(uint32_t version, const struct fi_info *rxr_info,
			    struct fi_info **core_info)
{
	int ret = 0;
	*core_info = fi_allocinfo();
	if (!*core_info)
		return -FI_ENOMEM;

	rxr_info_to_core_mr_modes(version, rxr_info, *core_info);
	(*core_info)->caps = FI_MSG;
	(*core_info)->ep_attr->type = FI_EP_RDM;
	(*core_info)->tx_attr->op_flags = FI_TRANSMIT_COMPLETE;

	/*
	 * Skip copying address, domain, fabric info.
	 */
	if (!rxr_info)
		return 0;

	ret = rxr_copy_attr(rxr_info, *core_info);
	if (ret)
		fi_freeinfo(*core_info);
	return ret;
}

/* Explicitly set all necessary bits before calling shm provider's getinfo function */
static void rxr_set_shm_hints(const struct fi_info *app_hints, struct fi_info *shm_hints)
{
	shm_hints->caps = FI_MSG | FI_TAGGED | FI_RECV | FI_SEND | FI_READ
			   | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE
			   | FI_MULTI_RECV | FI_RMA;
	shm_hints->domain_attr->av_type = FI_AV_TABLE;
	shm_hints->domain_attr->mr_mode = FI_MR_VIRT_ADDR;
	shm_hints->domain_attr->caps |= FI_LOCAL_COMM;
	shm_hints->tx_attr->msg_order = FI_ORDER_SAS;
	shm_hints->rx_attr->msg_order = FI_ORDER_SAS;
	shm_hints->fabric_attr->name = strdup("shm");
	shm_hints->fabric_attr->prov_name = strdup("shm");
	shm_hints->ep_attr->type = FI_EP_RDM;

	/*
	 * We validate whether FI_HMEM is supported before this function is
	 * called, so it's safe to check for this via the app hints directly.
	 * We should combine this and the earlier FI_HMEM validation when we
	 * clean up the getinfo path. That's not possible at the moment as we
	 * only have one SHM info for the entire provider which isn't right.
	 */
	if (app_hints && (app_hints->caps & FI_HMEM)) {
		shm_hints->caps |= FI_HMEM;
		shm_hints->domain_attr->mr_mode |= FI_MR_HMEM;
	}
}

/* Pass tx/rx attr that user specifies down to core provider */
void rxr_reset_rx_tx_to_core(const struct fi_info *user_info,
			     struct fi_info *core_info)
{
	/* rx attr */
	core_info->rx_attr->total_buffered_recv =
		user_info->rx_attr->total_buffered_recv < core_info->rx_attr->total_buffered_recv ?
		user_info->rx_attr->total_buffered_recv : core_info->rx_attr->total_buffered_recv;
	core_info->rx_attr->size =
		user_info->rx_attr->size < core_info->rx_attr->size ?
		user_info->rx_attr->size : core_info->rx_attr->size;
	/* tx attr */
	core_info->tx_attr->inject_size =
		user_info->tx_attr->inject_size < core_info->tx_attr->inject_size ?
		user_info->tx_attr->inject_size : core_info->tx_attr->inject_size;
	core_info->tx_attr->size =
		user_info->tx_attr->size < core_info->tx_attr->size ?
		user_info->tx_attr->size : core_info->tx_attr->size;
}

/*
 * Used to set tx/rx attributes that are characteristic of the device for the
 * two endpoint types and not emulated in software.
 */
void rxr_set_rx_tx_size(struct fi_info *info,
			const struct fi_info *core_info)
{
	if (rxr_env.tx_size > 0)
		info->tx_attr->size = rxr_env.tx_size;
	else
		info->tx_attr->size = core_info->tx_attr->size;

	if (rxr_env.rx_size > 0)
		info->rx_attr->size = rxr_env.rx_size;
	else
		info->rx_attr->size = core_info->rx_attr->size;

	if (rxr_env.tx_iov_limit > 0)
		info->tx_attr->iov_limit = rxr_env.tx_iov_limit;

	if (rxr_env.rx_iov_limit > 0)
		info->rx_attr->iov_limit = rxr_env.rx_iov_limit;
}

static int rxr_dgram_info_to_rxr(uint32_t version,
				 const struct fi_info *core_info,
				 struct fi_info *info) {
	rxr_set_rx_tx_size(info, core_info);
	return 0;
}

static int rxr_info_to_rxr(uint32_t version, const struct fi_info *core_info,
			   struct fi_info *info, const struct fi_info *hints)
{
	uint64_t atomic_ordering;
	uint64_t max_atomic_size;
	uint64_t min_pkt_size;

	info->caps = rxr_info.caps;
	info->mode = rxr_info.mode;

	*info->tx_attr = *rxr_info.tx_attr;
	*info->rx_attr = *rxr_info.rx_attr;
	*info->ep_attr = *rxr_info.ep_attr;
	*info->domain_attr = *rxr_info.domain_attr;

	/*
	 * The requirement for inject is: upon return, the user buffer can be reused immediately.
	 *
	 * For EFA, inject is implement as: construct a packet entry, copy user data to packet entry
	 * then send the packet entry. Therefore the maximum inject size is
	 *    pkt_entry_size - maximum_header_size.
	 */
	if (rxr_env.enable_shm_transfer)
		min_pkt_size = MIN(core_info->ep_attr->max_msg_size, rxr_env.shm_max_medium_size);
	else
		min_pkt_size = core_info->ep_attr->max_msg_size;

	info->tx_attr->inject_size = min_pkt_size - rxr_pkt_max_header_size();
	rxr_info.tx_attr->inject_size = info->tx_attr->inject_size;

	info->addr_format = core_info->addr_format;
	info->domain_attr->ep_cnt = core_info->domain_attr->ep_cnt;
	info->domain_attr->cq_cnt = core_info->domain_attr->cq_cnt;
	info->domain_attr->mr_key_size = core_info->domain_attr->mr_key_size;

	/*
	 * Do not advertise FI_HMEM capabilities when the core can not support
	 * it or when the application passes NULL hints (given this is a primary
	 * cap). The logic for device-specific checks pertaining to HMEM comes
	 * further along this path.
	 */
	if ((core_info && !(core_info->caps & FI_HMEM)) || !hints) {
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
			if (hints->tx_attr->msg_order & atomic_ordering) {
				max_atomic_size = core_info->ep_attr->max_msg_size
						  - sizeof(struct rxr_rta_hdr)
						  - core_info->src_addrlen
						  - RXR_IOV_LIMIT * sizeof(struct fi_rma_iov);

				if (hints->tx_attr->msg_order & FI_ORDER_ATOMIC_RAW) {
					info->ep_attr->max_order_raw_size = max_atomic_size;
					rxr_info.ep_attr->max_order_raw_size = max_atomic_size;
				}

				if (hints->tx_attr->msg_order & FI_ORDER_ATOMIC_WAR) {
					info->ep_attr->max_order_war_size = max_atomic_size;
					rxr_info.ep_attr->max_order_war_size = max_atomic_size;
				}

				if (hints->tx_attr->msg_order & FI_ORDER_ATOMIC_WAW) {
					info->ep_attr->max_order_waw_size = max_atomic_size;
					rxr_info.ep_attr->max_order_waw_size = max_atomic_size;
				}
			}
		}

		/* We only support manual progress for RMA operations */
		if (hints->caps & FI_RMA) {
			info->domain_attr->control_progress = FI_PROGRESS_MANUAL;
			info->domain_attr->data_progress = FI_PROGRESS_MANUAL;
		}

#if HAVE_LIBCUDA
		/* If the application requires HMEM support, we will add FI_MR_HMEM
		 * to mr_mode, because we need application to provide descriptor
		 * for cuda buffer.
		 * Note we did not add FI_MR_LOCAL here because according
		 * to FI_MR man page:
		 *
		 *     "If FI_MR_HMEM is set, but FI_MR_LOCAL is unset,
		 *      only device buffers must be registered when used locally.
		 *      "
		 * which means FI_MR_HMEM implies FI_MR_LOCAL for cuda buffer
		 */
		if (hints->caps & FI_HMEM) {

			if (!efa_device_support_rdma_read()) {
				FI_WARN(&rxr_prov, FI_LOG_CORE,
				        "FI_HMEM capability requires RDMA, which this device does not support.\n");
				return -FI_ENODATA;

			}

			if (!rxr_env.use_device_rdma) {
				FI_WARN(&rxr_prov, FI_LOG_CORE,
				        "FI_HMEM capability requires RDMA, which is turned off. You can turn it on by set environment variable FI_EFA_USE_DEVICE_RDMA to 1.\n");
				return -FI_ENODATA;
			}

			if (hints->domain_attr &&
			    !(hints->domain_attr->mr_mode & FI_MR_HMEM)) {
				FI_WARN(&rxr_prov, FI_LOG_CORE,
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

	if (!hints || !hints->domain_attr ||
	    hints->domain_attr->resource_mgmt == FI_RM_UNSPEC)
		info->domain_attr->resource_mgmt = FI_RM_ENABLED;
	else
		info->domain_attr->resource_mgmt = hints->domain_attr->resource_mgmt;

	rxr_set_rx_tx_size(info, core_info);
	return 0;
}

/*
 * For the RDM endpoint, translate user hints to hints for the lower layer and
 * call getinfo on the lower layer.
 */
int rxr_get_lower_rdm_info(uint32_t version, const char *node,
			   const char *service, uint64_t flags,
			   const struct util_prov *util_prov,
			   const struct fi_info *util_hints,
			   struct fi_info **core_info)
{
	struct fi_info *core_hints = NULL;
	int ret;

	ret = ofi_prov_check_info(&rxr_util_prov, version, util_hints);
	if (ret)
		return ret;

	ret = rxr_info_to_core(version, util_hints, &core_hints);
	if (ret)
		return ret;

	ret = lower_efa_prov->getinfo(version, node, service, flags,
				      core_hints, core_info);
	fi_freeinfo(core_hints);
	return ret;
}

/*
 * Call getinfo on lower efa provider to get all locally qualified fi_info
 * structure, then store the corresponding efa nic GIDs
 */
int rxr_get_local_gids(struct fi_provider *lower_efa_prov)
{
	struct fi_info *core_info, *cur;
	struct efa_ep_addr *cur_efa_addr;
	int ret;

	cur_efa_addr = local_efa_addr = NULL;
	core_info = cur = NULL;

	ret = lower_efa_prov->getinfo(rxr_prov.fi_version, NULL, NULL, 0, NULL, &core_info);
	if (ret)
		return ret;

	local_efa_addr = (struct efa_ep_addr *)malloc(sizeof(struct efa_ep_addr));
	if (!local_efa_addr) {
		ret = -FI_ENOMEM;
		goto out;
	}
	local_efa_addr->next = NULL;

	cur_efa_addr = local_efa_addr;
	for (cur = core_info; cur; cur = cur->next) {
		memcpy(cur_efa_addr->raw, ((struct efa_ep_addr *)cur->src_addr)->raw, 16);
		if (cur->next) {
			cur_efa_addr->next = (struct efa_ep_addr *)malloc(sizeof(struct efa_ep_addr));
			if (!cur_efa_addr->next) {
				ret = -FI_ENOMEM;
				goto out;
			}
			cur_efa_addr = cur_efa_addr->next;
			cur_efa_addr->next = NULL;
		}
	}

out:
	fi_freeinfo(core_info);
	return ret;
}

static int rxr_dgram_getinfo(uint32_t version, const char *node,
			     const char *service, uint64_t flags,
			     const struct fi_info *hints, struct fi_info **info,
			     struct fi_info **tail)
{
	struct fi_info *core_info, *util_info, *cur;
	int ret;

	core_info = NULL;

	ret = lower_efa_prov->getinfo(version, node, service,
				      flags, hints, &core_info);

	if (ret)
		return ret;

	ret = -FI_ENODATA;

	for (cur = core_info; cur; cur = cur->next) {
		/* Skip non DGRAM info structs */
		if (cur->ep_attr->type != FI_EP_DGRAM)
			continue;

		ret = 0;

		util_info = fi_dupinfo(cur);
		if (!util_info) {
			ret = -FI_ENOMEM;
			fi_freeinfo(*info);
			goto out;
		}

		rxr_dgram_info_to_rxr(version, cur, util_info);

		if (!*info)
			*info = util_info;
		else
			(*tail)->next = util_info;
		*tail = util_info;
	}

out:
	fi_freeinfo(core_info);
	return ret;
}

static int rxr_getinfo(uint32_t version, const char *node,
		       const char *service, uint64_t flags,
		       const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *core_info, *util_info, *cur, *tail;
	struct fi_info *shm_hints;
	int ret;

	*info = tail = core_info = NULL;

	if (hints && hints->ep_attr && hints->ep_attr->type == FI_EP_DGRAM)
		goto dgram_info;


	ret = rxr_get_lower_rdm_info(version, node, service, flags,
				     &rxr_util_prov, hints, &core_info);

	if (ret == -FI_ENODATA)
		goto dgram_info;

	if (ret)
		return ret;

	for (cur = core_info; cur; cur = cur->next) {
		util_info = fi_allocinfo();
		if (!util_info) {
			ret = -FI_ENOMEM;
			goto free_info;
		}

		ret = rxr_info_to_rxr(version, cur, util_info, hints);
		if (ret)
			goto free_info;

		ret = rxr_copy_attr(cur, util_info);
		if (ret)
			goto free_info;

		ofi_alter_info(util_info, hints, version);

		/* If application asked for FI_REMOTE_COMM but not FI_LOCAL_COMM, it
		 * does not want to use shm. In this case, we honor the request by
		 * unsetting the FI_LOCAL_COMM flag in info. This way rxr_endpoint()
		 * should disable shm transfer for the endpoint
		 */
		if (hints && hints->caps & FI_REMOTE_COMM && !(hints->caps & FI_LOCAL_COMM))
			util_info->caps &= ~FI_LOCAL_COMM;

		if (!*info)
			*info = util_info;
		else
			tail->next = util_info;
		tail = util_info;
	}

dgram_info:
	ret = rxr_dgram_getinfo(version, node, service, flags, hints, info,
				&tail);
	/*
	 * Ignore dgram getinfo return code if rdm getinfo was successful.
	 */
	if (ret == -FI_ENODATA && *info)
		ret = 0;

	if (!ret && rxr_env.enable_shm_transfer && !shm_info) {
		shm_info = NULL;
		shm_hints = fi_allocinfo();
		rxr_set_shm_hints(hints, shm_hints);
		ret = fi_getinfo(FI_VERSION(1, 8), NULL, NULL,
		                 OFI_GETINFO_HIDDEN, shm_hints, &shm_info);
		fi_freeinfo(shm_hints);
		if (ret) {
			FI_WARN(&rxr_prov, FI_LOG_CORE, "Disabling EFA shared memory support; failed to get shm provider's info: %s\n",
				fi_strerror(-ret));
			rxr_env.enable_shm_transfer = 0;
			ret = 0;
		} else {
			assert(!strcmp(shm_info->fabric_attr->name, "shm"));
		}
	}

	fi_freeinfo(core_info);
	return ret;
free_info:
	fi_freeinfo(core_info);
	fi_freeinfo(util_info);
	fi_freeinfo(*info);
	*info = NULL;
	return ret;
}

static void rxr_fini(void)
{
	struct efa_ep_addr *cur;

	if (lower_efa_prov)
		lower_efa_prov->cleanup();

	if (rxr_env.enable_shm_transfer) {
		/* Cleanup all local efa nic GIDs */
		while (local_efa_addr) {
			cur = local_efa_addr;
			local_efa_addr = local_efa_addr->next;
			free(cur);
		}
		if (shm_info)
			fi_freeinfo(shm_info);
	}

#if HAVE_EFA_DL
	ofi_monitors_cleanup();
	ofi_hmem_cleanup();
	ofi_mem_fini();
#endif
}

struct fi_provider rxr_prov = {
	.name = "efa",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = rxr_getinfo,
	.fabric = rxr_fabric,
	.cleanup = rxr_fini
};

EFA_INI
{
	fi_param_define(&rxr_prov, "rx_window_size", FI_PARAM_INT,
			"Defines the maximum window size that a receiver will return for matched large messages. (Default: 128).");
	fi_param_define(&rxr_prov, "tx_max_credits", FI_PARAM_INT,
			"Defines the maximum number of credits a sender requests from a receiver (Default: 64).");
	fi_param_define(&rxr_prov, "tx_min_credits", FI_PARAM_INT,
			"Defines the minimum number of credits a sender requests from a receiver (Default: 32).");
	fi_param_define(&rxr_prov, "tx_queue_size", FI_PARAM_INT,
			"Defines the maximum number of unacknowledged sends with the NIC.");
	fi_param_define(&rxr_prov, "enable_shm_transfer", FI_PARAM_INT,
			"Enable using SHM provider to provide the communication between processes on the same system. (Default: 1)");
	fi_param_define(&rxr_prov, "use_device_rdma", FI_PARAM_INT,
			"whether to use device's RDMA functionality for one-sided and two-sided transfer.");
	fi_param_define(&rxr_prov, "use_zcpy_rx", FI_PARAM_INT,
			"Enables the use of application's receive buffers in place of bounce-buffers when feasible. (Default: 1)");
	fi_param_define(&rxr_prov, "zcpy_rx_seed", FI_PARAM_INT,
			"Defines the number of bounce-buffers the provider will prepost during EP initialization.  (Default: 0)");
	fi_param_define(&rxr_prov, "shm_av_size", FI_PARAM_INT,
			"Defines the maximum number of entries in SHM provider's address vector (Default 128).");
	fi_param_define(&rxr_prov, "shm_max_medium_size", FI_PARAM_INT,
			"Defines the switch point between small/medium message and large message. The message larger than this switch point will be transferred with large message protocol (Default 4096).");
	fi_param_define(&rxr_prov, "recvwin_size", FI_PARAM_INT,
			"Defines the size of sliding receive window. (Default: 16384)");
	fi_param_define(&rxr_prov, "readcopy_pool_size", FI_PARAM_INT,
			"Defines the size of readcopy packet pool size. (Default: 256)");
	fi_param_define(&rxr_prov, "cq_size", FI_PARAM_INT,
			"Define the size of completion queue. (Default: 8192)");
	fi_param_define(&rxr_prov, "mr_cache_enable", FI_PARAM_BOOL,
			"Enables using the mr cache and in-line registration instead of a bounce buffer for iov's larger than max_memcpy_size. Defaults to true. When disabled, only uses a bounce buffer.");
	fi_param_define(&rxr_prov, "mr_max_cached_count", FI_PARAM_SIZE_T,
			"Sets the maximum number of memory registrations that can be cached at any time.");
	fi_param_define(&rxr_prov, "mr_max_cached_size", FI_PARAM_SIZE_T,
			"Sets the maximum amount of memory that cached memory registrations can hold onto at any time.");
	fi_param_define(&rxr_prov, "max_memcpy_size", FI_PARAM_SIZE_T,
			"Threshold size switch between using memory copy into a pre-registered bounce buffer and memory registration on the user buffer. (Default: 4096)");
	fi_param_define(&rxr_prov, "mtu_size", FI_PARAM_SIZE_T,
			"Override the MTU size of the device.");
	fi_param_define(&rxr_prov, "tx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of transmit operations before the provider returns -FI_EAGAIN. For only the RDM endpoint, this parameter will cause transmit operations to be queued when this value is set higher than the default and the transmit queue is full.");
	fi_param_define(&rxr_prov, "rx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of receive operations before the provider returns -FI_EAGAIN.");
	fi_param_define(&rxr_prov, "tx_iov_limit", FI_PARAM_SIZE_T,
			"Maximum transmit iov_limit.");
	fi_param_define(&rxr_prov, "rx_iov_limit", FI_PARAM_SIZE_T,
			"Maximum receive iov_limit.");
	fi_param_define(&rxr_prov, "rx_copy_unexp", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy unexpected messages out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&rxr_prov, "rx_copy_ooo", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy out-of-order RTM packets out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&rxr_prov, "max_timeout", FI_PARAM_INT,
			"Set the maximum timeout (us) for backoff to a peer after a receiver not ready error. (Default: 1000000)");
	fi_param_define(&rxr_prov, "timeout_interval", FI_PARAM_INT,
			"Set the time interval (us) for the base timeout to use for exponential backoff to a peer after a receiver not ready error. (Default: 0 [random])");
	fi_param_define(&rxr_prov, "efa_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of EFA completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&rxr_prov, "shm_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of SHM completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&rxr_prov, "inter_max_medium_message_size", FI_PARAM_INT,
			"The maximum message size for inter EFA medium message protocol (Default 65536).");
	fi_param_define(&rxr_prov, "inter_min_read_message_size", FI_PARAM_INT,
			"The minimum message size for inter EFA read message protocol. If instance support RDMA read, messages whose size is larger than this value will be sent by read message protocol (Default 1048576).");

	fi_param_define(&rxr_prov, "inter_min_read_write_size", FI_PARAM_INT,
			"The mimimum message size for inter EFA write to use read write protocol. If firmware support RDMA read, and FI_EFA_USE_DEVICE_RDMA is 1, write requests whose size is larger than this value will use the read write protocol (Default 65536).");
	fi_param_define(&rxr_prov, "inter_read_segment_size", FI_PARAM_INT,
			"Calls to RDMA read is segmented using this value.");
	fi_param_define(&rxr_prov, "fork_safe", FI_PARAM_BOOL,
			"Enables fork support and disables internal usage of huge pages. Has no effect on kernels which set copy-on-fork for registered pages, generally 5.13 and later. (Default: false)");

	rxr_init_env();

#if HAVE_EFA_DL
	ofi_mem_init();
	ofi_hmem_init();
	ofi_monitors_init();
#endif

	lower_efa_prov = init_lower_efa_prov();
	if (!lower_efa_prov)
		return NULL;

	if (rxr_env.enable_shm_transfer && rxr_get_local_gids(lower_efa_prov))
		return NULL;

	return &rxr_prov;
}
