/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa_rdm_util.h"

/**
 * @brief Fetch the initial value for use_device_rdma
 *
 * This function fetches the value of FI_EFA_USE_DEVICE_RDMA from the
 * user's environment.  If not set, it uses the API version and the EFA
 * hardware's version and capabilities to decide when to enable or
 * disable use_device_rdma during endpoint initialization.
 *
 * This value can be modified per-endpoint by using fi_setopt if the
 * application uses API>=1.18.
 *
 * The application may abort() during this method for two reasons:
 *  - If the Environment variable is set with non-boolean-like value
 *  - If the Environment variable requests RDMA but no hardware support
 *    is available.
 *
 * @return	bool: use_device_rdma default or environment setting.
 */
bool efa_rdm_get_use_device_rdma(uint32_t fabric_api_version)
{
	int ret;
	int param_val;
	bool hw_support;
	bool default_val;
	uint32_t vendor_part_id;

	vendor_part_id = g_efa_selected_device_list[0].ibv_attr.vendor_part_id;
	hw_support = efa_device_support_rdma_read();

	if (FI_VERSION_GE(fabric_api_version, FI_VERSION(1,18))) {
		default_val = hw_support;
	} else {
		if (vendor_part_id == 0xefa0 || vendor_part_id == 0xefa1) {
			default_val = false;
		} else {
			default_val = true;
		}

		if (default_val && !hw_support) {
			fprintf(stderr, "EFA device with vendor id %x unexpectedly has "
				"no RDMA support. Application will abort().\n",
				vendor_part_id);
			abort();
		}
	}
	param_val = default_val;

	/* Fetch the value of environment variable set by the user if any. */
	ret = fi_param_get_bool(&efa_prov, "use_device_rdma", &param_val);
	if (ret == -EINVAL) {
		fprintf(stderr, "FI_EFA_USE_DEVICE_RDMA was set to an invalid value by the user."
			" FI_EFA_USE_DEVICE_RDMA is boolean and can be set to only 0/false/no/off or"
			" 1/true/yes/on.  Application will abort().\n");
		abort();
	}
	if (ret < 0) return default_val;

	/* When the user requests use device RDMA but the device does not
	   support RDMA, exit the run. */
	if (param_val && !hw_support) {
		fprintf(stderr, "FI_EFA_USE_DEVICE_RDMA=1 was set by user, but "
			"EFA device has no rdma-read capability.  Application "
			"will abort().\n");
		abort();
	}

	return param_val;
}

/**
 * @brief convert EFA descriptors to shm descriptors.
 *
 * Each provider defines its descriptors format. The descriptor for
 * EFA provider is of struct efa_mr *, which shm provider cannot
 * understand. This function convert EFA descriptors to descriptors
 * shm can use.
 *
 * @param numdesc[in]       number of descriptors in the array
 * @param efa_desc[in]      efa descriptors
 * @param shm_desc[out]     shm descriptors
 *                          is shm descriptor.
 */
void efa_rdm_get_desc_for_shm(int numdesc, void **efa_desc, void **shm_desc)
{
	int i;
	struct efa_mr *efa_mr;

	for (i = 0; i < numdesc; ++i) {
		efa_mr = efa_desc[i];
		if (efa_mr)
			shm_desc[i] = fi_mr_desc(efa_mr->shm_mr);
	}
}

/**
 * @brief Construct a message that contains the local and peer information,
 * including the efa address and the host id.
 *
 * @param ep		EFA RDM endpoint
 * @param addr		Remote peer fi_addr_t
 * @param msg		the ptr of the msg to be constructed (needs to be allocated already!)
 * @param base_msg	ptr to the base msg that will show at the beginning of msg
 * @param msg_len	the length of the message
 * @return int 0 on success, negative integer on failure
 */
int efa_rdm_construct_msg_with_local_and_peer_information(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer, char *msg, const char *base_msg, size_t msg_len)
{
	char ep_addr_str[OFI_ADDRSTRLEN] = {0}, peer_addr_str[OFI_ADDRSTRLEN] = {0};
	char peer_host_id_str[EFA_HOST_ID_STRING_LENGTH + 1] = {0};
	char local_host_id_str[EFA_HOST_ID_STRING_LENGTH + 1] = {0};
	size_t len = 0;
	int ret;

	len = sizeof(ep_addr_str);
	efa_base_ep_raw_addr_str(&ep->base_ep, ep_addr_str, &len);
	len = sizeof(peer_addr_str);
	efa_base_ep_get_peer_raw_addr_str(&ep->base_ep, peer->conn->fi_addr, peer_addr_str, &len);

	if (!ep->host_id || EFA_HOST_ID_STRING_LENGTH != snprintf(local_host_id_str, EFA_HOST_ID_STRING_LENGTH + 1, "i-%017lx", ep->host_id)) {
		strcpy(local_host_id_str, "N/A");
	}

	if (!peer->host_id || EFA_HOST_ID_STRING_LENGTH != snprintf(peer_host_id_str, EFA_HOST_ID_STRING_LENGTH + 1, "i-%017lx", peer->host_id)) {
		strcpy(peer_host_id_str, "N/A");
	}

	ret = snprintf(msg, msg_len, "%s My EFA addr: %s My host id: %s Peer EFA addr: %s Peer host id: %s",
				base_msg, ep_addr_str, local_host_id_str, peer_addr_str, peer_host_id_str);

	if (ret < 0 || ret > msg_len - 1) {
		return -FI_EINVAL;
	}

	if (strlen(msg) >= msg_len) {
		return -FI_ENOBUFS;
	}

	return FI_SUCCESS;
}

/**
 * @brief Write the error message and return its byte length
 * @param[in]    ep          EFA RDM endpoint
 * @param[in]    addr        Remote peer fi_addr_t
 * @param[in]    prov_errno  EFA provider * error code(must be positive)
 * @param[out]   err_msg         Pointer to the address of error message written by this function
 * @param[out]   buflen      Pointer to the returned error data size
 * @return       A status code. 0 if the error data was written successfully, otherwise a negative FI error code.
 */
int efa_rdm_write_error_msg(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer, int prov_errno, char *err_msg, size_t *buflen)
{
	const char *base_msg = efa_strerror(prov_errno);
	int ret;

    *buflen = 0;

	ret = efa_rdm_construct_msg_with_local_and_peer_information(ep, peer, err_msg, base_msg, EFA_ERROR_MSG_BUFFER_LENGTH);
	if (ret)
		return ret;

	*buflen = EFA_ERROR_MSG_BUFFER_LENGTH;

	return 0;
}
