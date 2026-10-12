/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_hmem.h"
#include "efa_mr.h"
#include "efa_tp.h"
#include "rdm/efa_rdm_pkt_type.h"

struct efa_hmem_info g_efa_hmem_info[OFI_HMEM_MAX];

// TODO double-check for ROCr
#if HAVE_CUDA || HAVE_NEURON || HAVE_ROCR
static size_t efa_max_eager_msg_size_with_largest_header() {
	static bool computed = false;
	static size_t size = 0;

	if (!computed) {
		assert(g_efa_selected_device_list);
		size = g_efa_selected_device_list[0].ibv_port_attr.max_msg_sz - efa_rdm_pkt_type_get_max_hdr_size();
		computed = true;
	}

	return size;
}
#else
static size_t efa_max_eager_msg_size_with_largest_header() {
	return 0;
}
#endif

/**
 * @brief  Initialize the various protocol thresholds tracked in efa_hmem_info
 *         according to the given FI_HMEM interface.
 *
 * @param[in]      iface       The FI_HMEM interface to initialize
 *
 * @return  0
 */
static int efa_hmem_info_init_protocol_thresholds(enum fi_hmem_iface iface)
{
	struct efa_hmem_info *info = &g_efa_hmem_info[iface];
	size_t tmp_value;

	/* Fall back to FI_HMEM_SYSTEM initialization logic when p2p is
	 * unavailable */
	if (ofi_hmem_p2p_disabled() || !info->p2p_supported_by_device)
		iface = FI_HMEM_SYSTEM;

	switch (iface) {
	case FI_HMEM_SYSTEM:
		/* We have not yet tested runting with system memory */
		info->runt_size = 0;
		info->max_medium_msg_size = EFA_DEFAULT_INTER_MAX_MEDIUM_MESSAGE_SIZE;
		info->min_read_msg_size = EFA_DEFAULT_INTER_MIN_READ_MESSAGE_SIZE;
		info->min_read_write_size = EFA_DEFAULT_INTER_MIN_READ_WRITE_SIZE;
		fi_param_get_size_t(&efa_prov, "runt_size", &info->runt_size);
		fi_param_get_size_t(&efa_prov, "inter_max_medium_message_size", &info->max_medium_msg_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_message_size", &info->min_read_msg_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_write_size", &info->min_read_write_size);
		break;
	case FI_HMEM_CUDA:
	case FI_HMEM_ROCR:
		info->runt_size = EFA_DEFAULT_RUNT_SIZE;
		info->max_medium_msg_size = 0;
		info->min_read_msg_size = efa_max_eager_msg_size_with_largest_header() + 1;
		info->min_read_write_size = efa_max_eager_msg_size_with_largest_header() + 1;
		fi_param_get_size_t(&efa_prov, "runt_size", &info->runt_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_message_size", &info->min_read_msg_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_write_size", &info->min_read_write_size);
		if (-FI_ENODATA != fi_param_get(&efa_prov, "inter_max_medium_message_size", &tmp_value)) {
			EFA_WARN(FI_LOG_CORE,
			         "The environment variable FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE was set, "
			         "but only eager and runting read protocols are supported for %s over EFA.\n",
					 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
		}
		break;
	case FI_HMEM_NEURON:
		info->runt_size = EFA_NEURON_RUNT_SIZE;
		info->max_medium_msg_size = 0;
		info->min_read_msg_size = efa_max_eager_msg_size_with_largest_header() + 1;
		info->min_read_write_size = efa_max_eager_msg_size_with_largest_header() + 1;
		fi_param_get_size_t(&efa_prov, "runt_size", &info->runt_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_message_size", &info->min_read_msg_size);
		fi_param_get_size_t(&efa_prov, "inter_min_read_write_size", &info->min_read_write_size);
		if (-FI_ENODATA != fi_param_get(&efa_prov, "inter_max_medium_message_size", &tmp_value)) {
			EFA_WARN(FI_LOG_CORE,
			         "The environment variable FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE was set, "
			         "but EFA HMEM via Neuron API only supports eager and runting read protocols. "
			         "The variable will not modify Neuron memory run config.\n");
		}
		break;
	case FI_HMEM_SYNAPSEAI:
		info->runt_size = 0;
		info->max_medium_msg_size = 0;
		info->min_read_msg_size = 1;
		info->min_read_write_size = 1;
		if (-FI_ENODATA != fi_param_get_size_t(&efa_prov, "inter_max_medium_message_size", &tmp_value) ||
		    -FI_ENODATA != fi_param_get_size_t(&efa_prov, "inter_min_read_message_size", &tmp_value) ||
		    -FI_ENODATA != fi_param_get_size_t(&efa_prov, "inter_min_read_write_size", &tmp_value) ||
		    -FI_ENODATA != fi_param_get_size_t(&efa_prov, "runt_size", &tmp_value)) {
			EFA_WARN(FI_LOG_CORE,
			        "One or more of the following environment variable(s) were set: ["
			        "FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE, "
			        "FI_EFA_INTER_MIN_READ_MESSAGE_SIZE, "
			        "FI_EFA_INTER_MIN_READ_WRITE_SIZE, "
			        "FI_EFA_RUNT_SIZE"
			        "], but EFA HMEM via Synapse only supports long read protocol. "
			        "The variable(s) will not modify Synapse memory run config.\n");
		}
		break;
	default:
		break;
	}
	return 0;
}

static inline void efa_hmem_info_check_p2p_support_cuda(struct efa_hmem_info *info) {
#if HAVE_CUDA
	cudaError_t cuda_ret;
	void *ptr = NULL;
	struct ibv_mr *ibv_mr;
	struct ibv_pd *ibv_pd;
	int ibv_access = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ;
	size_t len = ofi_get_page_size() * 2;
	int ret;
	int dmabuf_fd;
	uint64_t dmabuf_offset;

	cuda_ret = ofi_cudaMalloc(&ptr, len);
	if (cuda_ret != cudaSuccess) {
		info->initialized = false;
		EFA_WARN(FI_LOG_CORE, "Failed to allocate CUDA buffer: %s\n",
			 ofi_cudaGetErrorString(cuda_ret));
		return;
	}

	ibv_pd = ibv_alloc_pd(g_efa_selected_device_list[0].ibv_ctx);
	if (!ibv_pd) {
		EFA_WARN(FI_LOG_CORE, "failed to allocate ibv_pd: %d", errno);
		ofi_cudaFree(ptr);
		return;
	}

#if HAVE_EFA_DMABUF_MR
	if (ofi_hmem_is_dmabuf_env_var_enabled(FI_HMEM_CUDA)) {
		ret = ofi_hmem_get_dmabuf_fd(FI_HMEM_CUDA, ptr, len, &dmabuf_fd, &dmabuf_offset);
		if (ret == FI_SUCCESS) {
			ibv_mr = ibv_reg_dmabuf_mr(ibv_pd, dmabuf_offset,
						   len, (uint64_t)ptr, dmabuf_fd, ibv_access);
			(void)ofi_hmem_put_dmabuf_fd(FI_HMEM_CUDA, dmabuf_fd);
			if (!ibv_mr) {
				EFA_INFO(FI_LOG_CORE,
					"Unable to register CUDA device buffer via dmabuf: %s. "
					"Fall back to ibv_reg_mr\n", fi_strerror(-errno));
				ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
				info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
			} else {
				info->dmabuf_supported_by_device = EFA_DMABUF_SUPPORTED;
			}
		} else {
			EFA_INFO(FI_LOG_CORE,
				"Unable to retrieve dmabuf fd of CUDA device buffer: %d. "
				"Fall back to ibv_reg_mr\n", ret);
			ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
			info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
		}
	} else {
		EFA_INFO(FI_LOG_CORE, "FI_HMEM_CUDA_USE_DMABUF set to false. Not using DMABUF for CUDA.\n");
		ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
		info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
	}
#else
	ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
#endif

	if (!ibv_mr) {
		info->p2p_supported_by_device = false;
		EFA_WARN(FI_LOG_CORE,
			 "Failed to register CUDA buffer with the EFA device, FI_HMEM transfers that require peer to peer support will fail.\n");
		ofi_cudaFree(ptr);
		(void) ibv_dealloc_pd(ibv_pd);
		return;
	}

	ret = ibv_dereg_mr(ibv_mr);
	ofi_cudaFree(ptr);
	(void) ibv_dealloc_pd(ibv_pd);
	if (ret) {
		EFA_WARN(FI_LOG_CORE,
			 "Failed to deregister CUDA buffer: %s\n",
			 fi_strerror(-ret));
		return;
	}

	info->p2p_supported_by_device = true;
	return;

#endif
	return;
}

static inline void efa_hmem_info_check_p2p_support_rocr(struct efa_hmem_info *info) {
#if HAVE_ROCR
	void *ptr = NULL;
	struct ibv_mr *ibv_mr;
	struct ibv_pd *ibv_pd;
	int ibv_access = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ;
	size_t len = ofi_get_page_size() * 2;
	int ret;
	int dmabuf_fd;
	uint64_t dmabuf_offset;

	ptr = rocr_alloc(len);
	if (!ptr) {
		info->initialized = false;
		EFA_WARN(FI_LOG_CORE, "Failed to allocate ROCr buffer\n");
		return;
	}

	ibv_pd = ibv_alloc_pd(g_efa_selected_device_list[0].ibv_ctx);
	if (!ibv_pd) {
		EFA_WARN(FI_LOG_CORE, "Failed to allocate ibv_pd: %d\n", errno);
		rocr_free(ptr);
		return;
	}

#if HAVE_EFA_DMABUF_MR
	if (ofi_hmem_is_dmabuf_env_var_enabled(FI_HMEM_ROCR)) {
		ret = rocr_hmem_get_dmabuf_fd(ptr, len, &dmabuf_fd, &dmabuf_offset);
		if (ret == FI_SUCCESS) {
			ibv_mr = ibv_reg_dmabuf_mr(ibv_pd, dmabuf_offset,
						   len, (uint64_t) ptr, dmabuf_fd, ibv_access);
			(void) rocr_hmem_put_dmabuf_fd(dmabuf_fd);
			if (!ibv_mr) {
				EFA_INFO(FI_LOG_CORE,
					"Unable to register ROCr device buffer via dmabuf: %s. "
					"Fall back to ibv_reg_mr\n", fi_strerror(-errno));
				ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
				info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
			} else {
				info->dmabuf_supported_by_device = EFA_DMABUF_SUPPORTED;
			}
		} else {
			EFA_INFO(FI_LOG_CORE,
				"Unable to retrieve dmabuf fd of ROCr device buffer: %d. "
				"Fall back to ibv_reg_mr\n", ret);
			ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
			info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
		}
	} else {
		EFA_INFO(FI_LOG_CORE, "FI_HMEM_ROCR_USE_DMABUF set to false. Not using DMABUF for ROCr.\n");
		ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
		info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
	}
#else
	ibv_mr = ibv_reg_mr(ibv_pd, ptr, len, ibv_access);
#endif

	if (!ibv_mr) {
		info->p2p_supported_by_device = false;
		EFA_WARN(FI_LOG_CORE,
			 "Failed to register ROCr buffer with the EFA device, FI_HMEM transfers that require peer to peer support will fail.\n");
		rocr_free(ptr);
		(void) ibv_dealloc_pd(ibv_pd);
		return;
	}

	ret = ibv_dereg_mr(ibv_mr);
	rocr_free(ptr);
	(void) ibv_dealloc_pd(ibv_pd);
	if (ret) {
		EFA_WARN(FI_LOG_CORE,
			 "Failed to deregister ROCr buffer: %s\n",
			 fi_strerror(-ret));
		return;
	}

	info->p2p_supported_by_device = true;
	return;

#endif
	return;
}

/**
 * @brief Disable dmabuf if the environment variable is not enabled
 *
 * @param[in,out]  info        HMEM info struct to update
 * @param[in]      iface       HMEM interface
 */
static void
efa_hmem_info_disable_dmabuf_if_env_unset(struct efa_hmem_info *info,
					  enum fi_hmem_iface iface)
{
	if (!ofi_hmem_is_dmabuf_env_var_enabled(iface)) {
		info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
		EFA_INFO(FI_LOG_CORE, "%s DMABUF disabled by environment variable\n",
			 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
	}
}

/**
 * @brief Initialize the efa_hmem_info state for iface
 *
 * @param[in]      iface       HMEM interface
 */
static void
efa_hmem_info_init_iface(enum fi_hmem_iface iface)
{
	struct efa_hmem_info *info = &g_efa_hmem_info[iface];

	if (!ofi_hmem_is_initialized(iface)) {
		EFA_INFO(FI_LOG_CORE, "%s is not initialized\n",
		         fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
		return;
	}

	info->initialized = true;
	info->p2p_supported_by_device = false;
	info->dmabuf_fallback_enabled = false;
#if HAVE_EFA_DMABUF_MR
	info->dmabuf_supported_by_device = EFA_DMABUF_ASSUMED;
#else /* !HAVE_EFA_DMABUF_MR */
	info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
#endif /* HAVE_EFA_DMABUF_MR */

	switch (iface) {
	case FI_HMEM_NEURON:
		if (ofi_hmem_p2p_disabled()) {
			EFA_WARN(FI_LOG_CORE,
				"%s requires p2p, unset FI_HMEM_DISABLE_P2P\n",
				fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			info->initialized = false;
			return;
		}
		if (!efa_device_support_rdma_read()) {
			EFA_WARN(FI_LOG_CORE,
				 "No EFA RDMA read support, transfers using %s will fail.\n",
				 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			info->initialized = false;
			return;
		}
		info->p2p_supported_by_device = true;
		info->dmabuf_fallback_enabled = true;
		efa_hmem_info_disable_dmabuf_if_env_unset(info, iface);
		break;

	case FI_HMEM_SYNAPSEAI:
		if (ofi_hmem_p2p_disabled()) {
			EFA_WARN(FI_LOG_CORE,
				 "%s requires p2p, unset FI_HMEM_DISABLE_P2P\n",
				 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			info->initialized = false;
			return;
		}
		if (!efa_device_support_rdma_read()) {
			EFA_WARN(FI_LOG_CORE,
				 "No EFA RDMA read support, transfers using %s will fail.\n",
				 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			info->initialized = false;
			return;
		}
		info->p2p_supported_by_device = true;
		efa_hmem_info_disable_dmabuf_if_env_unset(info, iface);
		break;

	case FI_HMEM_SYSTEM:
		info->p2p_supported_by_device = true;
		efa_hmem_info_disable_dmabuf_if_env_unset(info, iface);
		break;

	case FI_HMEM_CUDA:
		if (ofi_hmem_p2p_disabled())
			break;
		efa_hmem_info_check_p2p_support_cuda(info);
		if (!info->p2p_supported_by_device)
			EFA_INFO(FI_LOG_CORE, "%s P2P support is not available.\n",
				 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
		break;

	case FI_HMEM_ROCR:
		if (ofi_hmem_p2p_disabled())
			break;
		efa_hmem_info_check_p2p_support_rocr(info);
		if (!info->p2p_supported_by_device)
			EFA_INFO(FI_LOG_CORE, "%s P2P support is not available.\n",
				 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
		break;

	default:
		break;
	}

	efa_hmem_info_init_protocol_thresholds(iface);
}

/**
 * @brief   Validate an FI_OPT_FI_HMEM_P2P (FI_OPT_ENDPOINT) option for a
 *          specified HMEM interface.
 *
 * @param[in]      iface    The fi_hmem_iface enum of the FI_HMEM interface to validate
 * @param[in]      p2p_opt  The P2P option to validate
 *
 * @return  0 if the P2P option is valid for the given interface
 *         -FI_OPNOTSUPP if the P2P option is invalid
 *         -FI_ENODATA if the given HMEM interface was not initialized
 *         -FI_EINVAL if p2p_opt is not a valid FI_OPT_FI_HMEM_P2P option
 */
int efa_hmem_validate_p2p_opt(enum fi_hmem_iface iface, int p2p_opt, uint32_t api_version)
{
	struct efa_hmem_info *info = &g_efa_hmem_info[iface];

	if (OFI_UNLIKELY(!info->initialized))
		return -FI_ENODATA;

	switch (p2p_opt) {
	case FI_HMEM_P2P_REQUIRED:
		if (OFI_UNLIKELY(ofi_hmem_p2p_disabled()) || !info->p2p_supported_by_device)
			return -FI_EOPNOTSUPP;

		return 0;
	/*
	 * According to fi_setopt() document:
	 *
	 *     ENABLED means a provider may use P2P.
	 *     PREFERED means a provider should prefer P2P if it is available.
	 *
	 * These options does not require that p2p is supported by device,
	 * nor do they prohibit that p2p is required by implementation. 
	 * Therefore they are always supported unless p2p is disabled.
	 */
	case FI_HMEM_P2P_PREFERRED:
	case FI_HMEM_P2P_ENABLED:
		if (OFI_UNLIKELY(ofi_hmem_p2p_disabled()))
			return -FI_EOPNOTSUPP;

		return 0;

	case FI_HMEM_P2P_DISABLED:
		/* return -FI_EOPNOTSUPP if p2p is required by implementation */
		if (iface != FI_HMEM_CUDA || FI_VERSION_LT(api_version, FI_VERSION(1, 18)))
			return -FI_EOPNOTSUPP;

		return 0;
	}

	return -FI_EINVAL;
}

/**
 * @brief Initialize the hmem_info structs for
 * all of the HMEM devices. The device hmem_info
 * struct will be used to determine which efa transfer
 * protocol should be selected.
 *
 * @return  0 on success
 *          negative libfabric error code on an unexpected error
 */
int efa_hmem_info_initialize()
{
	int ret = 0;
	enum fi_hmem_iface iface;

	if(g_efa_selected_device_cnt <= 0) {
		return -FI_ENODEV;
	}

	memset(g_efa_hmem_info, 0, OFI_HMEM_MAX * sizeof(struct efa_hmem_info));

	EFA_HMEM_IFACE_FOREACH(iface) {
		efa_hmem_info_init_iface(iface);
	}

	return ret;
}

/**
 * @brief Copy data from a hmem device to a system buffer
 *
 * @param[in]    desc            Pointer to a memory registration descriptor
 * @param[out]   dest            Destination system memory buffer
 * @param[in]    src             Source hmem device memory
 * @param[in]    size            Data size in bytes to copy
 * @return       FI_SUCCESS status code on success, or an error code.
 */
int efa_copy_from_hmem(void *desc, void *dest, const void *src, size_t size)
{
	struct efa_mr_peer peer = { .iface = FI_HMEM_SYSTEM };

	if (desc)
		peer = ((struct efa_mr *) desc)->peer;

	if (peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
		assert(peer.hmem_data);
		switch (peer.iface) {
		/* TODO: Fine tune the max data size to switch from gdrcopy to cudaMemcpy */
		case FI_HMEM_CUDA:
		case FI_HMEM_ROCR:
			efa_tracepoint(dev_reg_copy_from_hmem, &peer, dest, src, size);
			return ofi_hmem_dev_reg_copy_from_hmem(peer.iface, (uint64_t) peer.hmem_data, dest, src, size);
		default:
			break;
		}
	}

	efa_tracepoint(copy_from_hmem, &peer, dest, src, size);
	return ofi_copy_from_hmem(peer.iface, peer.device, dest, src, size);
};

/**
 * @brief Copy data from a system buffer to a hmem device
 *
 * @param[in]    desc            Pointer to a memory registration descriptor
 * @param[out]   dest            Destination hmem device memory
 * @param[in]    src			 Source system memory buffer
 * @param[in]    size            Data size in bytes to copy
 * @return       FI_SUCCESS status code on success, or an error code.
 */
int efa_copy_to_hmem(void *desc, void *dest, const void *src, size_t size)
{
	struct efa_mr_peer peer = { .iface = FI_HMEM_SYSTEM };

	if (desc)
		peer = ((struct efa_mr *) desc)->peer;

	if (peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
		assert(peer.hmem_data);
		switch (peer.iface) {
		/* TODO: Fine tune the max data size to switch from gdrcopy to cudaMemcpy */
		case FI_HMEM_CUDA:
		case FI_HMEM_ROCR:
			efa_tracepoint(dev_reg_copy_to_hmem, &peer, dest, src, size);
			return ofi_hmem_dev_reg_copy_to_hmem(peer.iface, (uint64_t) peer.hmem_data, dest, src, size);
		default:
			break;
		}
	}

	efa_tracepoint(copy_to_hmem, &peer, dest, src, size);
	return ofi_copy_to_hmem(peer.iface, peer.device, dest, src, size);
};

/**
 * @brief Copy data from a hmem IOV to a system buffer
 *
 * @param[in]   desc          Array of memory desc corresponding to IOV buffers
 * @param[out]  buff          Target buffer (system memory)
 * @param[in]   buff_size     The size of the target buffer
 * @param[in]   hmem_iov      IOV data source
 * @param[in]   iov_count     Number of IOV structures in IOV array
 * @return  number of bytes copied on success, or a negative error code
 */
ssize_t efa_copy_from_hmem_iov(void **desc, char *buff, size_t buff_size,
                               const struct iovec *hmem_iov, size_t iov_count)
{
	int ret = -1;
	size_t i, data_size = 0;

	for (i = 0; i < iov_count; i++) {
		if (data_size + hmem_iov[i].iov_len > buff_size) {
			EFA_WARN(FI_LOG_CQ, "IOV is larger than the target buffer\n");
			return -FI_ETRUNC;
		}

		ret = efa_copy_from_hmem(desc[i], buff + data_size,
		                         hmem_iov[i].iov_base, hmem_iov[i].iov_len);

		if (ret < 0)
			return ret;

		data_size += hmem_iov[i].iov_len;
	}

	return data_size;
}

/**
 * @brief Copy data from a system buffer to a hmem IOV
 *
 * @param[in]    desc            Array of memory desc corresponding to IOV buffers
 * @param[out]   hmem_iov        Target IOV (HMEM)
 * @param[in]    iov_count       Number of IOV entries in vector
 * @param[in]    buff            System buffer data source
 * @param[in]    buff_size       Size of data to copy
 * @return  number of bytes copied on success, or a negative error code
 */
ssize_t efa_copy_to_hmem_iov(void **desc, struct iovec *hmem_iov,
                             size_t iov_count, char *buff, size_t buff_size)
{
	int ret;
	size_t i, bytes_remaining = buff_size, size;

	for (i = 0; i < iov_count && bytes_remaining; i++) {
		size = hmem_iov[i].iov_len;
		if (bytes_remaining < size) {
			size = bytes_remaining;
		}

		ret = efa_copy_to_hmem(desc[i], hmem_iov[i].iov_base, buff, size);

		if (ret < 0)
			return ret;

		bytes_remaining -= size;
	}

	if (bytes_remaining) {
		EFA_WARN(FI_LOG_CQ, "Source buffer is larger than target IOV\n");
		return -FI_ETRUNC;
	}
	return buff_size;
}
