/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "ofi_prov.h"
#include <ofi_util.h>

#include "efa.h"
#include "rdm/efa_rdm_cq.h"
#include "rdm/efa_rdm_pkt_type.h"

#define EFA_FABRIC_PREFIX "EFA-"
#define EFA_FABRIC_NAME "efa"
#define EFA_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)

#define EFA_RDM_TX_CAPS (OFI_TX_MSG_CAPS)
#define EFA_RDM_RX_CAPS (OFI_RX_MSG_CAPS | FI_SOURCE)
#define EFA_DGRM_TX_CAPS (OFI_TX_MSG_CAPS)
#define EFA_DGRM_RX_CAPS (OFI_RX_MSG_CAPS | FI_SOURCE)
#define EFA_RDM_CAPS (EFA_RDM_TX_CAPS | EFA_RDM_RX_CAPS | EFA_DOMAIN_CAPS)
#define EFA_DGRM_CAPS (EFA_DGRM_TX_CAPS | EFA_DGRM_RX_CAPS | EFA_DOMAIN_CAPS)

#define EFA_TX_OP_FLAGS (FI_TRANSMIT_COMPLETE)

#define EFA_RX_MODE (0)

#define EFA_RX_RDM_OP_FLAGS (0)
#define EFA_RX_DGRM_OP_FLAGS (0)

#define EFA_MSG_ORDER (FI_ORDER_NONE)

#define EFA_NO_DEFAULT -1

/**
 * @brief default efa_fabric_attr for prov_info
 */
const struct fi_fabric_attr efa_fabric_attr = {
	.fabric		= NULL,
	.name		= NULL,
	.prov_name	= NULL,
	.prov_version	= OFI_VERSION_DEF_PROV,
};

/**
 * @brief set the fabric_attr field of a prov_info
 *
 * @param 	prov_info[out]		prov_info object
 * @param	device[in]		pointer to an efa_device struct, which contains device attributes
 * @return	0 on sucessess
 * 		-FI_ENOMEM if memory allocation failed
 */
static
int efa_prov_info_set_fabric_attr(struct fi_info *prov_info, struct efa_device *device)
{
	size_t name_len = strlen(EFA_FABRIC_NAME);

	*prov_info->fabric_attr	= efa_fabric_attr;

	name_len = strlen(EFA_FABRIC_NAME);
	prov_info->fabric_attr->name = calloc(1, name_len + 1);
	if (!prov_info->fabric_attr->name)
		return -FI_ENOMEM;

	strcpy(prov_info->fabric_attr->name, EFA_FABRIC_NAME);
	return 0;
}

/**
 * @brief default domain_attr for prov_info
 */
const struct fi_domain_attr efa_domain_attr = {
	.caps			= EFA_DOMAIN_CAPS,
	.threading		= FI_THREAD_DOMAIN,
	.control_progress	= FI_PROGRESS_AUTO,
	.data_progress		= FI_PROGRESS_AUTO,
	.resource_mgmt		= FI_RM_DISABLED,
	.mr_mode		= OFI_MR_BASIC_MAP | FI_MR_LOCAL | FI_MR_BASIC,
	.mr_key_size		= sizeof_field(struct ibv_sge, lkey),
	.cq_data_size		= 0,
	.tx_ctx_cnt		= 1024,
	.rx_ctx_cnt		= 1024,
	.max_ep_tx_ctx		= 1,
	.max_ep_rx_ctx		= 1,
	.mr_iov_limit		= EFA_MR_IOV_LIMIT,
};

/**
 * @brief set the domain_attr field of a prov_info
 *
 * @param 	prov_info[out]		pointer to prov_info object
 * @param	device[in]		pointer to an efa_device struct, which contains device attributes
 * @param	ep_type[in]		endpoint type, can be FI_EP_RDM or FI_EP_DGRAM
 * @return	0 on sucessess
 * 		negative libfabric error code on failure
 */
static
int efa_prov_info_set_domain_attr(struct fi_info *prov_info,
				  struct efa_device *device,
				  enum fi_ep_type ep_type)
{
	size_t name_len;

	*prov_info->domain_attr = efa_domain_attr;
	prov_info->domain_attr->av_type = FI_AV_UNSPEC;

	/* set domain name */
	name_len = strlen(device->ibv_ctx->device->name) + strlen(efa_domain_name_suffix(ep_type));
	prov_info->domain_attr->name = malloc(name_len + 1);
	if (!prov_info->domain_attr->name) {
		return -FI_ENOMEM;
	}

	snprintf(prov_info->domain_attr->name, name_len + 1, "%s%s",
		 device->ibv_ctx->device->name, efa_domain_name_suffix(ep_type));
	prov_info->domain_attr->name[name_len] = '\0';

	/* set domain attributes using device attributes */
	prov_info->domain_attr->cq_cnt		= device->ibv_attr.max_cq;
	prov_info->domain_attr->ep_cnt		= device->ibv_attr.max_qp;
	prov_info->domain_attr->tx_ctx_cnt	= MIN(prov_info->domain_attr->tx_ctx_cnt, device->ibv_attr.max_qp);
	prov_info->domain_attr->rx_ctx_cnt	= MIN(prov_info->domain_attr->rx_ctx_cnt, device->ibv_attr.max_qp);
	prov_info->domain_attr->max_ep_tx_ctx	= 1;
	prov_info->domain_attr->max_ep_rx_ctx	= 1;
	prov_info->domain_attr->resource_mgmt	= FI_RM_DISABLED;
	prov_info->domain_attr->mr_cnt		= device->ibv_attr.max_mr;
	EFA_DBG(FI_LOG_DOMAIN, "Domain attribute :\n"
				"\t prov_info->domain_attr->cq_cnt		= %zu\n"
				"\t prov_info->domain_attr->ep_cnt		= %zu\n"
				"\t prov_info->domain_attr->rx_ctx_cnt	= %zu\n"
				"\t prov_info->domain_attr->tx_ctx_cnt	= %zu\n"
				"\t prov_info->domain_attr->max_ep_tx_ctx	= %zu\n"
				"\t prov_info->domain_attr->max_ep_rx_ctx	= %zu\n",
				prov_info->domain_attr->cq_cnt,
				prov_info->domain_attr->ep_cnt,
				prov_info->domain_attr->tx_ctx_cnt,
				prov_info->domain_attr->rx_ctx_cnt,
				prov_info->domain_attr->max_ep_tx_ctx,
				prov_info->domain_attr->max_ep_rx_ctx);
	return 0;
}

/**
 * @brief default ep attributes for prov_info
 */
const struct fi_ep_attr efa_ep_attr = {
	.protocol		= FI_PROTO_EFA,
	.protocol_version	= 1,
	.msg_prefix_size	= 0,
	.max_order_war_size	= 0,
	.mem_tag_format		= 0,
	.tx_ctx_cnt		= 1,
	.rx_ctx_cnt		= 1,
};

/**
 * @brief set the ep_attr field of a prov_info
 *
 * @param	prov_info[out]		prov_info pointer
 * @param	device[in]		pointer to an efa_device struct, which contains device attributes
 * @param	ep_type			endpoint_type, can be FI_EP_RDM or FI_EP_DGRAM
 */
static
void efa_prov_info_set_ep_attr(struct fi_info *prov_info,
			       struct efa_device *device,
			       enum fi_ep_type ep_type)
{

	*prov_info->ep_attr = efa_ep_attr;
	if (ep_type == FI_EP_DGRAM) {
		prov_info->mode |= FI_MSG_PREFIX;
		prov_info->ep_attr->msg_prefix_size = 40;
	}

	prov_info->ep_attr->protocol	= FI_PROTO_EFA;
	prov_info->ep_attr->type	= ep_type;

	if (prov_info->ep_attr->type == FI_EP_RDM) {
		prov_info->tx_attr->inject_size = device->efa_attr.inline_buf_size;
	} else {
		assert(prov_info->ep_attr->type == FI_EP_DGRAM);
                /*
		 * Currently, there is no mechanism for device to discard
		 * a completion, therefore there is no way for dgram endpoint
		 * to implement FI_INJECT. Because FI_INJECT is not an optional
		 * feature, we had to set inject_size to 0.
                 */
		prov_info->tx_attr->inject_size = 0;
	}

	prov_info->ep_attr->max_msg_size		= device->ibv_port_attr.max_msg_sz;
	prov_info->ep_attr->max_order_raw_size	= device->ibv_port_attr.max_msg_sz;
	prov_info->ep_attr->max_order_waw_size	= device->ibv_port_attr.max_msg_sz;
}

/**
 * @brief default TX attributes for dgram end point
 */
const struct fi_tx_attr efa_dgrm_tx_attr = {
	.caps			= EFA_DGRM_TX_CAPS,
	.mode			= FI_MSG_PREFIX,
	.op_flags		= EFA_TX_OP_FLAGS,
	.msg_order		= EFA_MSG_ORDER,
	.comp_order		= FI_ORDER_NONE,
	.inject_size		= 0,
	.rma_iov_limit		= 0,
};

/**
 * @brief default RX attributes for dgram end point
 */
const struct fi_rx_attr efa_dgrm_rx_attr = {
	.caps			= EFA_DGRM_RX_CAPS,
	.mode			= FI_MSG_PREFIX | EFA_RX_MODE,
	.op_flags		= EFA_RX_DGRM_OP_FLAGS,
	.msg_order		= EFA_MSG_ORDER,
	.comp_order		= FI_ORDER_NONE,
	.total_buffered_recv	= 0,
	.iov_limit		= 1
};

/**
 * @brief default TX attributes for rdm end point
 */
const struct fi_tx_attr efa_rdm_tx_attr = {
	.caps			= EFA_RDM_TX_CAPS,
	.mode			= 0,
	.op_flags		= EFA_TX_OP_FLAGS,
	.msg_order		= EFA_MSG_ORDER,
	.comp_order		= FI_ORDER_NONE,
	.inject_size		= 0,
	.rma_iov_limit		= 1,
};

/**
 * @brief default RX attributes for rdm end point
 */
const struct fi_rx_attr efa_rdm_rx_attr = {
	.caps			= EFA_RDM_RX_CAPS,
	.mode			= EFA_RX_MODE,
	.op_flags		= EFA_RX_RDM_OP_FLAGS,
	.msg_order		= EFA_MSG_ORDER,
	.comp_order		= FI_ORDER_NONE,
	.total_buffered_recv	= 0,
	.iov_limit		= 1
};

/**
 * @brief set the tx/rx_attr fields of a prov info
 *
 * @param	prov_info[out]		pointer to prov_info
 * @param	device[in]		pointer efa_device, which contains device attributes
 */
static
void efa_prov_info_set_tx_rx_attr(struct fi_info *prov_info,
				  struct efa_device *device,
				  enum fi_ep_type ep_type)
{
	if (ep_type == FI_EP_RDM) {
		*prov_info->tx_attr	= efa_rdm_tx_attr;
		*prov_info->rx_attr	= efa_rdm_rx_attr;
	} else {
		assert(ep_type == FI_EP_DGRAM);
		*prov_info->tx_attr	= efa_dgrm_tx_attr;
		*prov_info->rx_attr	= efa_dgrm_rx_attr;
	}

	prov_info->tx_attr->iov_limit = device->efa_attr.max_sq_sge;
	prov_info->tx_attr->size = rounddown_power_of_two(device->efa_attr.max_sq_wr);
	prov_info->rx_attr->iov_limit = device->efa_attr.max_rq_sge;
	prov_info->rx_attr->size = rounddown_power_of_two(device->efa_attr.max_rq_wr / prov_info->rx_attr->iov_limit);

	EFA_DBG(FI_LOG_DOMAIN, "Tx/Rx attribute :\n"
				"\t prov_info->tx_attr->iov_limit		= %zu\n"
				"\t prov_info->tx_attr->size			= %zu\n"
				"\t prov_info->tx_attr->inject_size		= %zu\n"
				"\t prov_info->rx_attr->iov_limit		= %zu\n"
				"\t prov_info->rx_attr->size			= %zu\n",
				prov_info->tx_attr->iov_limit,
				prov_info->tx_attr->size,
				prov_info->tx_attr->inject_size,
				prov_info->rx_attr->iov_limit,
				prov_info->rx_attr->size);
}

/**
 * @brief set the nic field of a prov_info
 *
 * @param	prov_info	pointer to a prov_inof
 * @param	device		pointer to an efa_device struct, which contains device attributes
 *
 * @return	0 on success
 * 		negative libfabric error code on failure
 */
static int efa_prov_info_set_nic_attr(struct fi_info *prov_info, struct efa_device *device)
{
	struct fi_device_attr *device_attr;
	struct fi_link_attr *link_attr;
	struct fi_bus_attr *bus_attr;
	struct fi_pci_attr *pci_attr;
	void *src_addr = NULL;
	int ret, link_addr_len;

	/* Sets nic ops and allocates basic structure */
	prov_info->nic = ofi_nic_dup(NULL);
	if (!prov_info->nic)
		return -FI_ENOMEM;

	device_attr = prov_info->nic->device_attr;
	bus_attr = prov_info->nic->bus_attr;
	pci_attr = &bus_attr->attr.pci;
	link_attr = prov_info->nic->link_attr;

	/* fi_device_attr */
	device_attr->name = strdup(device->ibv_ctx->device->name);
	if (!device_attr->name) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	ret = asprintf(&device_attr->device_id, "0x%x",
		       device->ibv_attr.vendor_part_id);
	if (ret < 0) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	ret = efa_device_get_version(device, &device_attr->device_version);
	if (ret != 0){
		goto err_free;
	}

	ret = asprintf(&device_attr->vendor_id, "0x%x",
		       device->ibv_attr.vendor_id);
	if (ret < 0) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	ret = efa_device_get_driver(device, &device_attr->driver);
	if (ret != 0) {
		goto err_free;
	}

	device_attr->firmware = strdup(device->ibv_attr.fw_ver);
	if (!device_attr->firmware) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	/* fi_bus_attr */
	bus_attr->bus_type = FI_BUS_PCI;

	/* fi_pci_attr */
	ret = efa_device_get_pci_attr(device, pci_attr);
	if (ret != 0) {
		goto err_free;
	}
	/* fi_link_attr */
	src_addr = calloc(1, EFA_EP_ADDR_LEN);
	if (!src_addr) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	memcpy(src_addr, &device->ibv_gid, sizeof(device->ibv_gid));

	link_addr_len = strlen(EFA_FABRIC_PREFIX) + INET6_ADDRSTRLEN;
	link_attr->address = calloc(1, link_addr_len + 1);
	if (!link_attr->address) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	strcpy(link_attr->address, EFA_FABRIC_PREFIX);
	if (!inet_ntop(AF_INET6, device->ibv_gid.raw,
		       (char *)link_attr->address + strlen(EFA_FABRIC_PREFIX),
		       INET6_ADDRSTRLEN)) {
		ret = -errno;
		goto err_free;
	}

	link_attr->mtu = device->ibv_port_attr.max_msg_sz - efa_rdm_pkt_type_get_max_hdr_size();
	link_attr->speed = ofi_vrb_speed(device->ibv_port_attr.active_speed,
	                                 device->ibv_port_attr.active_width);

	switch (device->ibv_port_attr.state) {
	case IBV_PORT_DOWN:
		link_attr->state = FI_LINK_DOWN;
		break;
	case IBV_PORT_ACTIVE:
		link_attr->state = FI_LINK_UP;
		break;
	default:
		link_attr->state = FI_LINK_UNKNOWN;
		break;
	}

	link_attr->network_type = strdup("Ethernet");
	if (!link_attr->network_type) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	free(src_addr);
	return FI_SUCCESS;

err_free:
	free(src_addr);
	/* fi_close will free all attributes of the fi_nic struct */
	fi_close(&prov_info->nic->fid);
	prov_info->nic = NULL;
	return ret;
}

#if HAVE_CUDA || HAVE_NEURON || HAVE_SYNAPSEAI
void efa_prov_info_set_hmem_flags(struct fi_info *prov_info)
{
	if (prov_info->ep_attr->type == FI_EP_RDM &&
	    (ofi_hmem_is_initialized(FI_HMEM_CUDA) ||
	     ofi_hmem_is_initialized(FI_HMEM_NEURON) ||
	     ofi_hmem_is_initialized(FI_HMEM_SYNAPSEAI))) {
		prov_info->caps			|= FI_HMEM;
		prov_info->tx_attr->caps		|= FI_HMEM;
		prov_info->rx_attr->caps		|= FI_HMEM;
		prov_info->domain_attr->mr_mode	|= FI_MR_HMEM;
	}
}
#else
void efa_prov_info_set_hmem_flags(struct fi_info *prov_info)
{
}
#endif

/**
 * @brief allocate a prov_info object that matches device's attributes
 *
 * A prov_info is a fi_info object that contains used by ofi_check_info() to validate
 * fi_info passed by user (known as user_info).
 *
 * This function allocate a prov_info object that matches EFA device's
 * capablity. The allocated prov_info is stored in struct efa_device.
 *
 * @param	prov_info[out]	info object to be allocated
 * @param	device[in]	efa_device that contains device's information
 * @param	ep_type[in]	either FI_EP_RDM or FI_EP_DGRAM
 * @return	0 on success
 * 		negative libfabric error code on failure. Possible errors are:
 * 		-FI_ENOMEM	cannot allocate memory for the info object
 * 		-FI_ENODATA	unsupported endpoint type
 */
int efa_prov_info_alloc(struct fi_info **prov_info_ptr,
			struct efa_device *device,
			enum fi_ep_type ep_type)
{
	struct fi_info *prov_info;
	int err;

	prov_info = fi_allocinfo();
	if (!prov_info)
		return -FI_ENOMEM;

	if (ep_type != FI_EP_RDM && ep_type != FI_EP_DGRAM) {
		EFA_WARN(FI_LOG_DOMAIN, "Unsupported endpoint type: %d\n",
			 ep_type);
		return -FI_ENODATA;
	}

	prov_info->caps	= (ep_type == FI_EP_RDM) ? EFA_RDM_CAPS : EFA_DGRM_CAPS;
	prov_info->handle = NULL;
	prov_info->addr_format = FI_ADDR_EFA;
	prov_info->src_addr = calloc(1, EFA_EP_ADDR_LEN);
	if (!prov_info->src_addr) {
		err = -FI_ENOMEM;
		goto err_free;
	}
	prov_info->src_addrlen = EFA_EP_ADDR_LEN;
	memcpy(prov_info->src_addr, &device->ibv_gid, sizeof(device->ibv_gid));

	err = efa_prov_info_set_fabric_attr(prov_info, device);
	if (err) {
		goto err_free;
	}

	err = efa_prov_info_set_domain_attr(prov_info, device, ep_type);
	if (err) {
		goto err_free;
	}

	efa_prov_info_set_ep_attr(prov_info, device, ep_type);

	efa_prov_info_set_tx_rx_attr(prov_info, device, ep_type);

	err = efa_prov_info_set_nic_attr(prov_info, device);
	if (err) {
		goto err_free;
	}

	efa_prov_info_set_hmem_flags(prov_info);

	*prov_info_ptr = prov_info;
	return 0;

err_free:
	fi_freeinfo(prov_info);
	*prov_info_ptr = NULL;
	return err;
}

/**
 * @brief allocate an prov_info object that matches the functionality of EFA RDM endpoint
 *
 * EFA RDM endpoint implemented mulitple additional functionalities over rdma-core
 * SRD QP. The added functionalities
 * include:
 *
 * tag matching, orderred send/receive, message segmentation,
 * emulated RMA/atomic emulation, multi recv.
 *
 * This function allocates a prov_info object the matches EFA RDM endpoint's functionality.
 *
 * Note that this info object is not to be confused the with rdm_info in struct efa_device.
 * That info object translates the rdma-core QP's capability to libfabric's info object.
 *
 * @param[out]		prov_info_rdm_ptr	pointer to pointer of prov_info_rdm
 * @param[in]		device			efa_device information
 * @return		0 on success
 * 			-FI_ENOMEM if memory allocation failed.
 */
int efa_prov_info_alloc_for_rdm(struct fi_info **prov_info_rdm_ptr,
				struct efa_device *device)
{
	uint64_t efa_rdm_added_tx_caps = FI_TAGGED | OFI_TX_RMA_CAPS | FI_ATOMIC;

	uint64_t efa_rdm_added_rx_caps = FI_TAGGED | OFI_RX_RMA_CAPS | FI_ATOMIC |
				     FI_SOURCE | FI_MULTI_RECV | FI_DIRECTED_RECV;

	uint64_t efa_domain_caps = FI_LOCAL_COMM | FI_REMOTE_COMM;

	struct fi_info *prov_info_rdm;

	assert(device->rdm_info);

	prov_info_rdm  = fi_dupinfo(device->rdm_info);
	if (!prov_info_rdm)
		return -FI_ENOMEM;

	prov_info_rdm->caps |= efa_rdm_added_tx_caps | efa_rdm_added_rx_caps | efa_domain_caps;

	/* update domain_attr */
	{
		/* EFA RDM endpoint ensure thread safety by pthread lock */
		prov_info_rdm->domain_attr->threading = FI_THREAD_SAFE;
		/* EFA RDM endpoint handles Receiver Not Ready (RNR) events by queuing the send,
		 * hence resource_mgmt is enabled.
		 */
		prov_info_rdm->domain_attr->resource_mgmt = FI_RM_ENABLED;
		/*
		 * The device endpoint requires a memory descriptor for any send/receive.
		 * Therefore it set the FI_MR_LOCAL mode.
		 * buffer. EFA RDM endpoint does not have this requirement, hence unset the flag
		 */
		prov_info_rdm->domain_attr->mr_mode &= ~FI_MR_LOCAL;

		/* EFA RDM endpoint support writing CQ data by put it in packet header
		 */
		prov_info_rdm->domain_attr->cq_data_size = EFA_RDM_CQ_DATA_SIZE;
	}

	/* update ep_attr */
	{
		int max_atomic_size;

		prov_info_rdm->ep_attr->protocol = FI_PROTO_EFA;
		prov_info_rdm->ep_attr->mem_tag_format = FI_TAG_GENERIC;
		prov_info_rdm->ep_attr->protocol_version = EFA_RDM_PROTOCOL_VERSION;
		/*
		 * EFA RDM endpoint support message segmentation, hence increase the max_msg_size
		 */
		prov_info_rdm->ep_attr->max_msg_size = UINT64_MAX;

		/*
		 * EFA RDM endpoint implemented emulated atomic, hence set atomic size
		 */
		max_atomic_size = device->rdm_info->ep_attr->max_msg_size
					- sizeof(struct efa_rdm_rta_hdr)
					- device->rdm_info->src_addrlen
					- EFA_RDM_IOV_LIMIT * sizeof(struct fi_rma_iov);
		prov_info_rdm->ep_attr->max_order_raw_size = max_atomic_size;
	}

	/* update tx_attr */
	{
		int min_pkt_size;
		/*
		 * EFA RDM endpoint supports ordered two-sided/atomic by putting messages by
		 * software reorder buffer, hence set tx_attr->message order accordingly.
		 */
		prov_info_rdm->tx_attr->caps |= efa_rdm_added_tx_caps;
		prov_info_rdm->tx_attr->msg_order = FI_ORDER_SAS | FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW |
						    FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW;

		/*
		 * EFA RDM endpoint supports injection by software emulation.
		 * EFA RDM endpoint supports delivery complete by using DC capable protocols.
		 * Therefore changing the default op_flags
		 */
		prov_info_rdm->tx_attr->op_flags = FI_INJECT | FI_COMPLETION | FI_TRANSMIT_COMPLETE |
						   FI_DELIVERY_COMPLETE;

		/* Here we calculate the max msg size for emulated injection of EFA RDM endpoint.
		 * The requirement for inject is: upon return, the user buffer can be reused immediately.
		 *
		 * For EFA, inject is implement as: construct a packet entry, copy user data to packet entry
		 * then send the packet entry. Therefore the maximum inject size is
		 *    pkt_entry_size - maximum_header_size.
		 */
		if (efa_env.enable_shm_transfer)
			min_pkt_size = MIN(device->rdm_info->ep_attr->max_msg_size - efa_rdm_pkt_type_get_max_hdr_size(),
					   SHM_MAX_INJECT_SIZE);
		else
			min_pkt_size = device->rdm_info->ep_attr->max_msg_size - efa_rdm_pkt_type_get_max_hdr_size();

		if (min_pkt_size < efa_rdm_pkt_type_get_max_hdr_size()) {
			prov_info_rdm->tx_attr->inject_size = 0;
		} else {
			prov_info_rdm->tx_attr->inject_size = min_pkt_size;
		}

		/*
		 * EFA RDM endpoint support multiple IOV by segmentation.
		 */
		prov_info_rdm->tx_attr->iov_limit = EFA_RDM_IOV_LIMIT;

		if (efa_env.tx_size > 0)
			prov_info_rdm->tx_attr->size = efa_env.tx_size;

	}

	{
		prov_info_rdm->rx_attr->caps |= efa_rdm_added_rx_caps;
		prov_info_rdm->rx_attr->msg_order = FI_ORDER_SAS | FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW |
						    FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW;
		prov_info_rdm->rx_attr->op_flags = FI_COMPLETION | FI_MULTI_RECV;
		prov_info_rdm->rx_attr->iov_limit = EFA_RDM_IOV_LIMIT;

		if (efa_env.rx_size > 0)
			prov_info_rdm->rx_attr->size = efa_env.rx_size;
	}

	*prov_info_rdm_ptr = prov_info_rdm;
	return 0;
}


static int efa_node_matches_addr(struct efa_ep_addr *addr, const char *node)
{
	struct efa_ep_addr eaddr;

	efa_str_to_ep_addr(node, NULL, &eaddr);
	return memcmp(&eaddr.raw, &addr->raw, sizeof(addr->raw));
}

/**
 * @brief compare the src_addr field of an info object with user information
 *
 * @param	info[in]	info object
 * @param	node[in]	node from user's call to fi_getinfo()
 * @param	flags[in]	flags from user's call to fi_getinfo()
 * @param	hints[in]	hints from user's call to fi_getinfo()
 *
 * return	the difference between info->src_addr and user provided src_addr.
 *		Note 0 means no difference.
 */
int efa_prov_info_compare_src_addr(const char *node, uint64_t flags, const struct fi_info *hints,
				   const struct fi_info *info)
{
	if (flags & FI_SOURCE) {
		if (node)
			return efa_node_matches_addr(info->src_addr, node);
	} else if (hints && hints->src_addr) {
		return memcmp(info->src_addr, hints->src_addr, EFA_EP_ADDR_LEN);
	}

	return 0;
}

/*
 * @brief Compare the domain name specified via hints and match it with the
 *		  domain name in prov_info
 *
 * @param      info[in]        info object
 * @param      hints[in]       hints from user's call to fi_getinfo()
 *
 * return      1 - If the names are different
 *             0 - No difference, names match.
 */
int efa_prov_info_compare_domain_name(const struct fi_info *hints,
                                      const struct fi_info *info)
{
       if (hints && hints->domain_attr && hints->domain_attr->name)
               return strcmp(info->domain_attr->name, hints->domain_attr->name);

       return 0;
}

/*
 * @brief Compare the bus id specified via hints and match it with the
 *		  bus_id in prov_info
 *
 * @param[in]  info        info object
 * @param[in]  hints       hints from user's call to fi_getinfo()
 *
 * return      1 - If the bus_id in hints does not match info
 *             0 - If the bus_id in hints match info, or user did not specify bus ID in hints
 */
int efa_prov_info_compare_pci_bus_id(const struct fi_info *hints,
                                     const struct fi_info *info)
{
	if (hints && hints->nic && hints->nic->bus_attr && hints->nic->bus_attr->bus_type == FI_BUS_PCI) {
		return (hints->nic->bus_attr->attr.pci.bus_id == info->nic->bus_attr->attr.pci.bus_id) ? 0 : 1;
	}

	return 0;
}
