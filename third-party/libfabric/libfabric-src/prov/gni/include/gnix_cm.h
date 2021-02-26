/*
 * Copyright (c) 2016 Cray Inc. All rights reserved.
 * Copyright (c) 2017 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2019 Triad National Security, LLC. All rights reserved.
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

#ifndef _GNIX_CM_H_
#define _GNIX_CM_H_

#include "gnix.h"

#define GNIX_CM_DATA_MAX_SIZE	256
#define GNIX_CM_EQE_BUF_SIZE	(sizeof(struct fi_eq_cm_entry) + \
				 GNIX_CM_DATA_MAX_SIZE)

struct gnix_pep_sock_connreq {
	struct fi_info info;
	struct gnix_ep_name src_addr;
	struct gnix_ep_name dest_addr;
	struct fi_tx_attr tx_attr;
	struct fi_rx_attr rx_attr;
	struct fi_ep_attr ep_attr;
	struct fi_domain_attr domain_attr;
	struct fi_fabric_attr fabric_attr;
	int vc_id;
	gni_smsg_attr_t vc_mbox_attr;
	gni_mem_handle_t cq_irq_mdh;
	uint64_t peer_caps;
	size_t cm_data_len;
	char eqe_buf[GNIX_CM_EQE_BUF_SIZE];
	uint32_t key_offset;
};

enum gnix_pep_sock_resp_cmd {
	GNIX_PEP_SOCK_RESP_ACCEPT,
	GNIX_PEP_SOCK_RESP_REJECT
};

struct gnix_pep_sock_connresp {
	enum gnix_pep_sock_resp_cmd cmd;
	int vc_id;
	gni_smsg_attr_t vc_mbox_attr;
	gni_mem_handle_t cq_irq_mdh;
	uint64_t peer_caps;
	size_t cm_data_len;
	char eqe_buf[GNIX_CM_EQE_BUF_SIZE];
	uint32_t key_offset;
};

struct gnix_pep_sock_conn {
	struct fid fid;
	struct dlist_entry list;
	int sock_fd;
	struct gnix_pep_sock_connreq req;
	int bytes_read;
	struct fi_info *info;
};

int _gnix_pep_progress(struct gnix_fid_pep *pep);
int _gnix_ep_progress(struct gnix_fid_ep *ep);

/**
 * Parse a given address (of format FI_ADDR_GNI) into FI_ADDR_STR.
 * @param ep_name [IN]     the FI_ADDR_GNI address.
 * @param out_buf [IN/OUT] the FI_ADDR_STR address.
 * @return either FI_SUCCESS or a negative integer on failure.
 */
int _gnix_ep_name_to_str(struct gnix_ep_name *ep_name, char **out_buf);

/**
 * Parse a given address (of format FI_ADDR_STR) into FI_ADDR_GNI.
 * @param addr[IN]           the FI_ADDR_STR address.
 * @param resolved_addr[OUT] the FI_ADDR_GNI address.
 * @return either FI_SUCCESS or a negative integer on failure.
 */
int _gnix_ep_name_from_str(const char *addr,
			    struct gnix_ep_name *resolved_addr);

/**
 * Find a FI_ADDR_GNI.
 * @param ep_name[IN]  the array of addresses.
 * @param idx    [IN]  the index of the desired address.
 * @param addr   [OUT] the desired address.
 */
static inline int
_gnix_resolve_gni_ep_name(const char *ep_name, int idx,
			   struct gnix_ep_name *addr)
{
	int ret = FI_SUCCESS;
	static size_t addr_size = sizeof(struct gnix_ep_name);

	GNIX_TRACE(FI_LOG_TRACE, "\n");

	/*TODO (optimization): Just return offset into ep_name */
	memcpy(addr, &ep_name[addr_size * idx], addr_size);
	return ret;
}

/**
 * Find and convert a FI_ADDR_STR to FI_ADDR_GNI.
 * @param ep_name [IN]  the FI_ADDR_STR address.
 * @param idx     [IN]  the index of the desired address.
 * @param addr    [OUT] the desired address converted to FI_ADDR_GNI.
 * @return either FI_SUCCESS or a negative integer on failure.
 */
static inline int
_gnix_resolve_str_ep_name(const char *ep_name, int idx,
			   struct gnix_ep_name *addr)
{
	int ret = FI_SUCCESS;
	static size_t addr_size = GNIX_FI_ADDR_STR_LEN;

	GNIX_TRACE(FI_LOG_TRACE, "\n");

	ret = _gnix_ep_name_from_str(&ep_name[addr_size * idx], addr);
	return ret;
}

/**
 * Find and resolve the given ep_name.
 *
 * @param ep_name [IN]  the ep name to resolve.
 * @param idx     [IN]  the index of the desired address.
 * @param addr    [OUT] the desired address.
 * @param domain  [IN]  the given domain.
 * @return either FI_SUCCESS or a negative integer on failure.
 */
static inline int
_gnix_get_ep_name(const char *ep_name, int idx, struct gnix_ep_name *addr,
		  struct gnix_fid_domain *domain)
{
	int ret = FI_SUCCESS;
	/* Use a function pointer to resolve the address */
	if (domain->addr_format == FI_ADDR_STR) {
		ret = _gnix_resolve_str_ep_name(ep_name, idx, addr);
	} else {
		ret = _gnix_resolve_gni_ep_name(ep_name, idx, addr);
	}
	return ret;
}
#endif

