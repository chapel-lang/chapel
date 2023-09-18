/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
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

#include "efa.h"

#if HAVE_CUDA || HAVE_NEURON
static size_t efa_max_eager_msg_size_with_largest_header(struct efa_domain *efa_domain) {
	int mtu_size;

	mtu_size = efa_domain->device->rdm_info->ep_attr->max_msg_size;
	if (rxr_env.mtu_size > 0 && rxr_env.mtu_size < mtu_size)
		mtu_size = rxr_env.mtu_size;
	if (mtu_size > RXR_MTU_MAX_LIMIT)
		mtu_size = RXR_MTU_MAX_LIMIT;

	return mtu_size - rxr_pkt_max_hdr_size();
}
#endif

/**
 * @brief Update the System hmem_info struct
 *
 * @param system_info[out]	System hmem_info struct
 * @return 0 on success
 */
static int efa_hmem_info_update_system(struct efa_hmem_info *system_info)
{
	system_info->initialized = true;
	system_info->p2p_supported = true;

	/* We have not yet tested runting with system memory */
	system_info->runt_size = 0;
	fi_param_get_size_t(&rxr_prov, "runt_size", &system_info->runt_size);

	system_info->max_medium_msg_size = 65536;
	fi_param_get_size_t(&rxr_prov, "inter_max_medium_message_size", &system_info->max_medium_msg_size);

	system_info->min_read_msg_size = 1048576;
	fi_param_get_size_t(&rxr_prov, "inter_min_read_message_size", &system_info->min_read_msg_size);

	system_info->min_read_write_size = 65536;
	fi_param_get_size_t(&rxr_prov, "inter_min_read_write_size", &system_info->min_read_write_size);

	return 0;
}

/**
 * @brief Update the Cuda hmem_info struct
 *
 * @param	cuda_info[out]	Cuda hmem_info struct
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
static int efa_hmem_info_update_cuda(struct efa_hmem_info *cuda_info, struct efa_domain *efa_domain)
{
#if HAVE_CUDA
	cudaError_t cuda_ret;
	void *ptr = NULL;
	struct ibv_mr *ibv_mr;
	int ibv_access = IBV_ACCESS_LOCAL_WRITE;
	size_t len = ofi_get_page_size() * 2, tmp_value;
	int ret;

	if (!ofi_hmem_is_initialized(FI_HMEM_CUDA)) {
		EFA_INFO(FI_LOG_DOMAIN,
		         "FI_HMEM_CUDA is not initialized\n");
		return 0;
	}

	if (efa_device_support_rdma_read())
		ibv_access |= IBV_ACCESS_REMOTE_READ;

	cuda_info->initialized = true;

	cuda_ret = ofi_cudaMalloc(&ptr, len);
	if (cuda_ret != cudaSuccess) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "Failed to allocate CUDA buffer: %s\n",
			 ofi_cudaGetErrorString(cuda_ret));
		return -FI_ENOMEM;
	}

	ibv_mr = ibv_reg_mr(g_device_list[0].ibv_pd, ptr, len, ibv_access);
	if (!ibv_mr) {
		/* In the future when we support cuda on non-gdr systems, we need to set protocol variables for non-p2p cases. */
		EFA_WARN(FI_LOG_DOMAIN,
			 "Failed to register CUDA buffer with the EFA device, FI_HMEM transfers that require peer to peer support will fail.\n");
		ofi_cudaFree(ptr);
		return 0;
	}

	ret = ibv_dereg_mr(ibv_mr);
	ofi_cudaFree(ptr);
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "Failed to deregister CUDA buffer: %s\n",
			 fi_strerror(-ret));
		return ret;
	}

	cuda_info->p2p_supported = true;

	/* Eager and runting read protocols */
	cuda_info->max_medium_msg_size = 0;
	if (-FI_ENODATA != fi_param_get(&rxr_prov, "inter_max_medium_message_size", &tmp_value)) {
		EFA_WARN(FI_LOG_DOMAIN,
		         "The environment variable FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE was set, "
		         "but EFA HMEM via Cuda API only supports eager and runting read protocols. "
				 "The variable will not modify Cuda memory run config.\n");
	}

	cuda_info->runt_size = 307200;
	fi_param_get_size_t(&rxr_prov, "runt_size", &cuda_info->runt_size);

	if (-FI_ENODATA == fi_param_get(&rxr_prov, "inter_min_read_message_size", &cuda_info->min_read_msg_size)) {
		cuda_info->min_read_msg_size = efa_max_eager_msg_size_with_largest_header(efa_domain) + 1;
	}

	if (-FI_ENODATA == fi_param_get(&rxr_prov, "inter_min_read_write_size", &cuda_info->min_read_write_size)) {
		cuda_info->min_read_write_size = efa_max_eager_msg_size_with_largest_header(efa_domain) + 1;
	}

#endif
	return 0;
}

/**
 * @brief Update the Neuron hmem_info struct
 *
 * @param	neuron_info[out]	Neuron hmem_info struct
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
static int efa_hmem_info_update_neuron(struct efa_hmem_info *neuron_info, struct efa_domain *efa_domain)
{
#if HAVE_NEURON
	struct ibv_mr *ibv_mr;
	int ibv_access = IBV_ACCESS_LOCAL_WRITE;
	void *handle;
	void *ptr = NULL;
	size_t len = ofi_get_page_size() * 2, tmp_value;
	int ret;

	if (!ofi_hmem_is_initialized(FI_HMEM_NEURON)) {
		EFA_INFO(FI_LOG_DOMAIN,
		         "FI_HMEM_NEURON is not initialized\n");
		return 0;
	}

	if (g_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ) {
		ibv_access |= IBV_ACCESS_REMOTE_READ;
	} else {
		EFA_WARN(FI_LOG_DOMAIN,
			 "No EFA RDMA read support, transfers using AWS Neuron will fail.\n");
		return 0;
	}

	ptr = neuron_alloc(&handle, len);
	/*
	 * neuron_alloc will fail if application did not call nrt_init,
	 * which is ok if it's not running neuron workloads. libfabric
	 * will move on and leave neuron_info->initialized as false.
	 */
	if (!ptr) {
		EFA_INFO(FI_LOG_DOMAIN,
			"Cannot allocate Neuron buffer. \n");
		return 0;
	}

	neuron_info->initialized = true;

	ibv_mr = ibv_reg_mr(g_device_list[0].ibv_pd, ptr, len, ibv_access);
	if (!ibv_mr) {
		/* We do not expect to support Neuron on non p2p systems */
		EFA_WARN(FI_LOG_DOMAIN,
		         "Failed to register Neuron buffer with the EFA device, "
		         "FI_HMEM transfers that require peer to peer support will fail.\n");
		neuron_free(&handle);
		return 0;
	}

	ret = ibv_dereg_mr(ibv_mr);
	neuron_free(&handle);
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "Failed to deregister Neuron buffer: %s\n",
			 fi_strerror(-ret));
		return ret;
	}

	neuron_info->p2p_supported = true;

	/* Eager and runting read protocols */
	neuron_info->max_medium_msg_size = 0;
	if (-FI_ENODATA != fi_param_get(&rxr_prov, "inter_max_medium_message_size", &tmp_value)) {
		EFA_WARN(FI_LOG_DOMAIN,
		         "The environment variable FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE was set, "
		         "but EFA HMEM via Neuron API only supports eager and runting read protocols. "
				 "The variable will not modify Neuron memory run config.\n");
	}

	neuron_info->runt_size = 307200;
	fi_param_get_size_t(&rxr_prov, "runt_size", &neuron_info->runt_size);

	if (-FI_ENODATA == fi_param_get(&rxr_prov, "inter_min_read_message_size", &neuron_info->min_read_msg_size)) {
		neuron_info->min_read_msg_size = efa_max_eager_msg_size_with_largest_header(efa_domain) + 1;
	}

	if (-FI_ENODATA == fi_param_get(&rxr_prov, "inter_min_read_write_size", &neuron_info->min_read_write_size)) {
		neuron_info->min_read_write_size = efa_max_eager_msg_size_with_largest_header(efa_domain) + 1;
	}
#endif
	return 0;
}

/**
 * @brief Update the Synapseai hmem_info struct
 *
 * @param synapseai_info[out]	Synapseai hmem_info struct
 * @return 0 on success
 */
static int efa_hmem_info_update_synapseai(struct efa_hmem_info *synapseai_info)
{
#if HAVE_SYNAPSEAI
	size_t tmp_value;

	if (!ofi_hmem_is_initialized(FI_HMEM_SYNAPSEAI)) {
		EFA_INFO(FI_LOG_DOMAIN,
		         "FI_HMEM_SYNAPSEAI is not initialized\n");
		return 0;
	}

	if (!(g_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ)) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "No EFA RDMA read support, transfers using Habana Gaudi will fail.\n");
		return 0;
	}

	synapseai_info->initialized = true;
	synapseai_info->p2p_supported = true;

	/*  Only the long read protocol is supported */
	if (-FI_ENODATA != fi_param_get(&rxr_prov, "inter_max_medium_message_size", &tmp_value) ||
		-FI_ENODATA != fi_param_get(&rxr_prov, "inter_min_read_message_size", &tmp_value) ||
		-FI_ENODATA != fi_param_get(&rxr_prov, "runt_size", &tmp_value)) {
		EFA_WARN(FI_LOG_DOMAIN,
		         "One of the following environment variable(s) was set: (FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE, "
		         "FI_EFA_INTER_MIN_READ_MESSAGE_SIZE, and/or FI_EFA_RUNT_SIZE), but EFA HMEM via Synapse "
		         "only supports long read protocol. The variable will not modify Synapse memory run config.\n");
	}

	synapseai_info->max_medium_msg_size = 0;
	synapseai_info->runt_size = 0;
	/* Use Eager Msg Protocol size 0 messages */
	synapseai_info->min_read_msg_size = 1;
	synapseai_info->min_read_write_size = 1;
#endif
	return 0;
}

/**
 * @brief Update the hmem_info structs for
 * all of the HMEM devices. The device hmem_info
 * struct will be used to determine which efa transfer
 * protocol should be selected.
 *
 * @param   efa_domain[out]     The EFA Domain containing efa_hmem_info
 *                              structures
 * @return  0 on success
 *          negative libfabric error code on an unexpected error
 */
int efa_hmem_info_update_all(struct efa_domain *efa_domain)
{
	int ret, err;
	struct efa_hmem_info *hmem_info = efa_domain->hmem_info;

	if(g_device_cnt <= 0) {
		return -FI_ENODEV;
	}

	memset(hmem_info, 0, OFI_HMEM_MAX * sizeof(struct efa_hmem_info));

	ret = 0;

	err = efa_hmem_info_update_system(&hmem_info[FI_HMEM_SYSTEM]);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to populate the System hmem_info struct! err: %d\n",
			 err);
	}

	err = efa_hmem_info_update_cuda(&hmem_info[FI_HMEM_CUDA], efa_domain);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to populate the Cuda hmem_info struct! err: %d\n",
			 err);
	}

	err = efa_hmem_info_update_neuron(&hmem_info[FI_HMEM_NEURON], efa_domain);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to populate the Neuron hmem_info struct! err: %d\n",
			 err);
	}

	err = efa_hmem_info_update_synapseai(&hmem_info[FI_HMEM_SYNAPSEAI]);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to populate the Synapseai hmem_info struct! err: %d\n",
			 err);
	}

	return ret;
}
