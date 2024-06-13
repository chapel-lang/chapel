/*
 * Copyright (c) 2016-2018 Intel Corporation. All rights reserved.
 * Copyright (c) 2017, Cisco Systems, Inc. All rights reserved.
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

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#include <rdma/fabric.h>
#include <ofi_abi.h>
#include <ofi_util.h>


/*
 * The conversion from abi 1.0 requires being able to cast from a newer
 * structure back to the older version.
 */
struct fi_fabric_attr_1_0 {
	struct fid_fabric		*fabric;
	char				*name;
	char				*prov_name;
	uint32_t			prov_version;
};

struct fi_domain_attr_1_0 {
	struct fid_domain		*domain;
	char				*name;
	enum fi_threading		threading;
	enum fi_progress		control_progress;
	enum fi_progress		data_progress;
	enum fi_resource_mgmt		resource_mgmt;
	enum fi_av_type			av_type;
	enum fi_mr_mode			mr_mode;
	size_t				mr_key_size;
	size_t				cq_data_size;
	size_t				cq_cnt;
	size_t				ep_cnt;
	size_t				tx_ctx_cnt;
	size_t				rx_ctx_cnt;
	size_t				max_ep_tx_ctx;
	size_t				max_ep_rx_ctx;
	size_t				max_ep_stx_ctx;
	size_t				max_ep_srx_ctx;
};

struct fi_ep_attr_1_0 {
	enum fi_ep_type		type;
	uint32_t		protocol;
	uint32_t		protocol_version;
	size_t			max_msg_size;
	size_t			msg_prefix_size;
	size_t			max_order_raw_size;
	size_t			max_order_war_size;
	size_t			max_order_waw_size;
	uint64_t		mem_tag_format;
	size_t			tx_ctx_cnt;
	size_t			rx_ctx_cnt;
};

struct fi_tx_attr_1_0 {
        uint64_t                caps;
        uint64_t                mode;
        uint64_t                op_flags;
        uint64_t                msg_order;
        uint64_t                comp_order;
        size_t                  inject_size;
        size_t                  size;
        size_t                  iov_limit;
        size_t                  rma_iov_limit;
};

/* External structure is still ABI 1.0 compliant */
#define fi_rx_attr_1_0 fi_rx_attr

struct fi_info_1_0 {
	struct fi_info			*next;
	uint64_t			caps;
	uint64_t			mode;
	uint32_t			addr_format;
	size_t				src_addrlen;
	size_t				dest_addrlen;
	void				*src_addr;
	void				*dest_addr;
	fid_t				handle;
	struct fi_tx_attr_1_0		*tx_attr;
	struct fi_rx_attr_1_0		*rx_attr;
	struct fi_ep_attr_1_0		*ep_attr;
	struct fi_domain_attr_1_0	*domain_attr;
	struct fi_fabric_attr_1_0	*fabric_attr;
};

struct fi_domain_attr_1_1 {
        struct fid_domain       *domain;
        char                    *name;
        enum fi_threading       threading;
        enum fi_progress        control_progress;
        enum fi_progress        data_progress;
        enum fi_resource_mgmt   resource_mgmt;
        enum fi_av_type         av_type;
        int                     mr_mode;
        size_t                  mr_key_size;
        size_t                  cq_data_size;
        size_t                  cq_cnt;
        size_t                  ep_cnt;
        size_t                  tx_ctx_cnt;
        size_t                  rx_ctx_cnt;
        size_t                  max_ep_tx_ctx;
        size_t                  max_ep_rx_ctx;
        size_t                  max_ep_stx_ctx;
        size_t                  max_ep_srx_ctx;
        size_t                  cntr_cnt;
        size_t                  mr_iov_limit;
        uint64_t                caps;
        uint64_t                mode;
        uint8_t                 *auth_key;
        size_t                  auth_key_size;
        size_t                  max_err_data;
        size_t                  mr_cnt;
};

#define fi_tx_attr_1_1 fi_tx_attr_1_0
#define fi_rx_attr_1_1 fi_rx_attr_1_0
#define fi_ep_attr_1_1 fi_ep_attr
#define fi_fabric_attr_1_1 fi_fabric_attr

struct fi_info_1_1 {
	struct fi_info			*next;
	uint64_t			caps;
	uint64_t			mode;
	uint32_t			addr_format;
	size_t				src_addrlen;
	size_t				dest_addrlen;
	void				*src_addr;
	void				*dest_addr;
	fid_t				handle;
	struct fi_tx_attr_1_1		*tx_attr;
	struct fi_rx_attr_1_1		*rx_attr;
	struct fi_ep_attr_1_1		*ep_attr;
	struct fi_domain_attr_1_1	*domain_attr;
	struct fi_fabric_attr_1_1	*fabric_attr;
};

#define fi_tx_attr_1_2 fi_tx_attr_1_1
#define fi_rx_attr_1_2 fi_rx_attr_1_1
#define fi_ep_attr_1_2 fi_ep_attr_1_1
#define fi_domain_attr_1_2 fi_domain_attr_1_1
#define fi_fabric_attr_1_2 fi_fabric_attr_1_1
#define fid_nic_1_2 fid_nic

struct fi_info_1_2 {
        struct fi_info            *next;
        uint64_t                  caps;
        uint64_t                  mode;
        uint32_t                  addr_format;
        size_t                    src_addrlen;
        size_t                    dest_addrlen;
        void                      *src_addr;
        void                      *dest_addr;
        fid_t                     handle;
        struct fi_tx_attr_1_2     *tx_attr;
        struct fi_rx_attr_1_2      *rx_attr;
        struct fi_ep_attr_1_2     *ep_attr;
        struct fi_domain_attr_1_2 *domain_attr;
        struct fi_fabric_attr_1_2 *fabric_attr;
        struct fid_nic_1_2        *nic;
};

struct fi_domain_attr_1_3 {
	struct fid_domain	*domain;
	char			*name;
	enum fi_threading	threading;
	enum fi_progress	control_progress;
	enum fi_progress	data_progress;
	enum fi_resource_mgmt	resource_mgmt;
	enum fi_av_type		av_type;
	int			mr_mode;
	size_t			mr_key_size;
	size_t			cq_data_size;
	size_t			cq_cnt;
	size_t			ep_cnt;
	size_t			tx_ctx_cnt;
	size_t			rx_ctx_cnt;
	size_t			max_ep_tx_ctx;
	size_t			max_ep_rx_ctx;
	size_t			max_ep_stx_ctx;
	size_t			max_ep_srx_ctx;
	size_t			cntr_cnt;
	size_t			mr_iov_limit;
	uint64_t		caps;
	uint64_t		mode;
	uint8_t			*auth_key;
	size_t 			auth_key_size;
	size_t			max_err_data;
	size_t			mr_cnt;
	uint32_t		tclass;
};

#define fi_tx_attr_1_3 fi_tx_attr
#define fi_rx_attr_1_3 fi_rx_attr_1_2
#define fi_ep_attr_1_3 fi_ep_attr_1_2
#define fi_fabric_attr_1_3 fi_fabric_attr_1_2
#define fid_nic_1_3 fid_nic_1_2

struct fi_info_1_3 {
        struct fi_info            *next;
        uint64_t                  caps;
        uint64_t                  mode;
        uint32_t                  addr_format;
        size_t                    src_addrlen;
        size_t                    dest_addrlen;
        void                      *src_addr;
        void                      *dest_addr;
        fid_t                     handle;
        struct fi_tx_attr_1_3     *tx_attr;
        struct fi_rx_attr_1_3     *rx_attr;
        struct fi_ep_attr_1_3     *ep_attr;
        struct fi_domain_attr_1_3 *domain_attr;
        struct fi_fabric_attr_1_3 *fabric_attr;
        struct fid_nic_1_3        *nic;
};

#define ofi_dup_attr(dst, src)				\
	do {						\
		dst = calloc(1, sizeof(*dst));		\
		if (dst)				\
			memcpy(dst, src, sizeof(*src));	\
	} while (0);

#define ofi_free_attr(attr)						\
	do {								\
		if (attr) {						\
			free(attr);					\
			attr = NULL;					\
		}							\
	} while (0);

/* Macro used to duplicate versions of old fi_infos and convert it to the latest
 * definition. Any new fields in the latest definition will be zeroed.
 */
#define ofi_dup_info(base, info)					\
	do {								\
		ofi_dup_attr(base, info);				\
		if (!base)						\
			break;						\
									\
		if (info->tx_attr) {					\
			ofi_dup_attr(base->tx_attr, info->tx_attr);	\
			if (!base->tx_attr)				\
				goto ofi_dup_info_err_free_base;	\
		}							\
									\
		if (info->rx_attr) {					\
			ofi_dup_attr(base->rx_attr, info->rx_attr);	\
			if (!base->rx_attr)				\
				goto ofi_dup_info_err_free_tx_attr;	\
		}							\
									\
		if (info->ep_attr) {					\
			ofi_dup_attr(base->ep_attr, info->ep_attr);	\
			if (!base->ep_attr)				\
				goto ofi_dup_info_err_free_rx_attr;	\
		}							\
									\
		if (info->domain_attr) {				\
			ofi_dup_attr(base->domain_attr,			\
				     info->domain_attr);		\
			if (!base->domain_attr)				\
				goto ofi_dup_info_err_free_ep_attr;	\
		}							\
									\
		if (info->fabric_attr) {				\
			ofi_dup_attr(base->fabric_attr,			\
				     info->fabric_attr);		\
			if (!base->fabric_attr)				\
				goto ofi_dup_info_err_free_domain_attr;	\
		}							\
									\
		/* Success. */						\
		break;							\
									\
ofi_dup_info_err_free_domain_attr:					\
		ofi_free_attr(base->domain_attr);			\
ofi_dup_info_err_free_ep_attr:						\
		ofi_free_attr(base->ep_attr);				\
ofi_dup_info_err_free_rx_attr:						\
		ofi_free_attr(base->rx_attr);				\
ofi_dup_info_err_free_tx_attr:						\
		ofi_free_attr(base->tx_attr);				\
ofi_dup_info_err_free_base:						\
		ofi_free_attr(base);					\
	} while (0);

#define ofi_free_info(base)						\
	do {								\
		ofi_free_attr(base->fabric_attr);			\
		ofi_free_attr(base->domain_attr);			\
		ofi_free_attr(base->ep_attr);				\
		ofi_free_attr(base->rx_attr);				\
		ofi_free_attr(base->tx_attr);				\
		ofi_free_attr(base);					\
	} while (0);

/*
 * ABI 1.0
 */
__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void fi_freeinfo_1_0(struct fi_info_1_0 *info)
{
	fi_freeinfo((struct fi_info *) info);
}
COMPAT_SYMVER(fi_freeinfo_1_0, fi_freeinfo, FABRIC_1.0);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
struct fi_info_1_0 *fi_dupinfo_1_0(const struct fi_info_1_0 *info)
{
	struct fi_info *dup;

	if (!info)
		return (struct fi_info_1_0 *) ofi_allocinfo_internal();

	ofi_dup_attr(dup, info);
	if (dup == NULL) {
		return NULL;
	}
	dup->src_addr = NULL;
	dup->dest_addr = NULL;
	dup->tx_attr = NULL;
	dup->rx_attr = NULL;
	dup->ep_attr = NULL;
	dup->domain_attr = NULL;
	dup->fabric_attr = NULL;
	dup->next = NULL;

	if (info->src_addr != NULL) {
		dup->src_addr = mem_dup(info->src_addr, info->src_addrlen);
		if (dup->src_addr == NULL)
			goto fail;
	}
	if (info->dest_addr != NULL) {
		dup->dest_addr = mem_dup(info->dest_addr, info->dest_addrlen);
		if (dup->dest_addr == NULL)
			goto fail;
	}
	if (info->tx_attr != NULL) {
		ofi_dup_attr(dup->tx_attr, info->tx_attr);
		if (dup->tx_attr == NULL)
			goto fail;
	}
	if (info->rx_attr != NULL) {
		ofi_dup_attr(dup->rx_attr, info->rx_attr);
		if (dup->rx_attr == NULL)
			goto fail;
	}
	if (info->ep_attr != NULL) {
		ofi_dup_attr(dup->ep_attr, info->ep_attr);
		if (dup->ep_attr == NULL)
			goto fail;
	}
	if (info->domain_attr) {
		ofi_dup_attr(dup->domain_attr, info->domain_attr);
		if (dup->domain_attr == NULL)
			goto fail;
		if (info->domain_attr->name != NULL) {
			dup->domain_attr->name = strdup(info->domain_attr->name);
			if (dup->domain_attr->name == NULL)
				goto fail;
		}
	}
	if (info->fabric_attr) {
		ofi_dup_attr(dup->fabric_attr, info->fabric_attr);
		if (dup->fabric_attr == NULL)
			goto fail;
		dup->fabric_attr->name = NULL;
		dup->fabric_attr->prov_name = NULL;
		if (info->fabric_attr->name != NULL) {
			dup->fabric_attr->name = strdup(info->fabric_attr->name);
			if (dup->fabric_attr->name == NULL)
				goto fail;
		}
		if (info->fabric_attr->prov_name != NULL) {
			dup->fabric_attr->prov_name = strdup(info->fabric_attr->prov_name);
			if (dup->fabric_attr->prov_name == NULL)
				goto fail;
		}
	}
	return (struct fi_info_1_0 *) dup;

fail:
	fi_freeinfo(dup);
	return NULL;
}
COMPAT_SYMVER(fi_dupinfo_1_0, fi_dupinfo, FABRIC_1.0);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int fi_getinfo_1_0(uint32_t version, const char *node, const char *service,
		    uint64_t flags, const struct fi_info_1_0 *hints_1_0,
		    struct fi_info_1_0 **info)
{
	struct fi_info *hints;
	int ret;

	if (hints_1_0) {
		hints = (struct fi_info *)fi_dupinfo_1_0(hints_1_0);
		if (!hints)
			return -FI_ENOMEM;
	} else {
		hints = NULL;
	}
	ret = fi_getinfo(version, node, service, flags, hints,
			 (struct fi_info **)info);
	fi_freeinfo(hints);

	return ret;
}
COMPAT_SYMVER(fi_getinfo_1_0, fi_getinfo, FABRIC_1.0);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int fi_fabric_1_0(struct fi_fabric_attr_1_0 *attr_1_0,
		  struct fid_fabric **fabric, void *context)
{
	struct fi_fabric_attr attr;

	if (!attr_1_0)
		return -FI_EINVAL;

	memcpy(&attr, attr_1_0, sizeof(*attr_1_0));

	/* Since the API version is not available in ABI 1.0, set the field to
	 * FI_VERSION(1, 0) for compatibility. The actual API version could be
	 * anywhere from FI_VERSION(1, 0) to FI_VERSION(1, 4).
	 */
	attr.api_version = FI_VERSION(1, 0);
	return fi_fabric(&attr, fabric, context);
}
COMPAT_SYMVER(fi_fabric_1_0, fi_fabric, FABRIC_1.0);


/*
 * ABI 1.1
 */
__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void fi_freeinfo_1_1(struct fi_info_1_1 *info)
{
	fi_freeinfo((struct fi_info *) info);
}
COMPAT_SYMVER(fi_freeinfo_1_1, fi_freeinfo, FABRIC_1.1);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
struct fi_info_1_1 *fi_dupinfo_1_1(const struct fi_info_1_1 *info)
{
	struct fi_info *dup, *base;

	if (!info)
		return (struct fi_info_1_1 *) ofi_allocinfo_internal();

	ofi_dup_info(base, info);
	if (base == NULL)
		return NULL;

	dup = fi_dupinfo(base);

	ofi_free_info(base);
	return (struct fi_info_1_1 *) dup;
}
COMPAT_SYMVER(fi_dupinfo_1_1, fi_dupinfo, FABRIC_1.1);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int fi_getinfo_1_1(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info_1_1 *hints_1_1,
		   struct fi_info_1_1 **info)
{
	struct fi_info *hints;
	int ret;

	if (hints_1_1) {
		hints = (struct fi_info *) fi_dupinfo_1_1(hints_1_1);
		if (!hints)
			return -FI_ENOMEM;
	} else {
		hints = NULL;
	}
	ret = fi_getinfo(version, node, service, flags, hints,
			 (struct fi_info **) info);
	fi_freeinfo(hints);

	return ret;
}
COMPAT_SYMVER(fi_getinfo_1_1, fi_getinfo, FABRIC_1.1);

/*
 * ABI 1.2
 */
__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void fi_freeinfo_1_2(struct fi_info_1_2 *info)
{
	fi_freeinfo((struct fi_info *) info);
}
COMPAT_SYMVER(fi_freeinfo_1_2, fi_freeinfo, FABRIC_1.2);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
struct fi_info_1_2 *fi_dupinfo_1_2(const struct fi_info_1_2 *info)
{
	struct fi_info *dup, *base;

	if (!info)
		return (struct fi_info_1_2 *) ofi_allocinfo_internal();

	ofi_dup_info(base, info);
	if (base == NULL)
		return NULL;

	dup = fi_dupinfo(base);

	ofi_free_info(base);
	return (struct fi_info_1_2 *) dup;
}
COMPAT_SYMVER(fi_dupinfo_1_2, fi_dupinfo, FABRIC_1.2);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int fi_getinfo_1_2(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info_1_2 *hints_1_2,
		   struct fi_info_1_2 **info)
{
	struct fi_info *hints;
	int ret;

	if (hints_1_2) {
		hints = (struct fi_info *) fi_dupinfo_1_2(hints_1_2);
		if (!hints)
			return -FI_ENOMEM;
	} else {
		hints = NULL;
	}
	ret = fi_getinfo(version, node, service, flags, hints,
			 (struct fi_info **) info);
	fi_freeinfo(hints);

	return ret;
}
COMPAT_SYMVER(fi_getinfo_1_2, fi_getinfo, FABRIC_1.2);

/*
 * ABI 1.3
 */
__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void fi_freeinfo_1_3(struct fi_info_1_3 *info)
{
	fi_freeinfo((struct fi_info *) info);
}
COMPAT_SYMVER(fi_freeinfo_1_3, fi_freeinfo, FABRIC_1.3);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
struct fi_info_1_3 *fi_dupinfo_1_3(const struct fi_info_1_3 *info)
{
	struct fi_info *dup, *base;

	if (!info)
		return (struct fi_info_1_3 *) ofi_allocinfo_internal();

	ofi_dup_info(base, info);
	if (base == NULL)
		return NULL;

	dup = fi_dupinfo(base);

	ofi_free_info(base);
	return (struct fi_info_1_3 *) dup;
}
COMPAT_SYMVER(fi_dupinfo_1_3, fi_dupinfo, FABRIC_1.3);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int fi_getinfo_1_3(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info_1_3 *hints_1_3,
		   struct fi_info_1_3 **info)
{
	struct fi_info *hints;
	int ret;

	if (hints_1_3) {
		hints = (struct fi_info *) fi_dupinfo_1_3(hints_1_3);
		if (!hints)
			return -FI_ENOMEM;
	} else {
		hints = NULL;
	}
	ret = fi_getinfo(version, node, service, flags, hints,
			 (struct fi_info **) info);
	fi_freeinfo(hints);

	return ret;
}
COMPAT_SYMVER(fi_getinfo_1_3, fi_getinfo, FABRIC_1.3);
