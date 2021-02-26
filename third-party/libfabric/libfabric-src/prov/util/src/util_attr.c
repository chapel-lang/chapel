/*
 * Copyright (c) 2015-2016 Intel Corporation. All rights reserved.
 * Copyright (c) 2018, Cisco Systems, Inc. All rights reserved.
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

#include <stdio.h>

#include <shared/ofi_str.h>
#include <ofi_util.h>

#define OFI_MSG_DIRECTION_CAPS	(FI_SEND | FI_RECV)
#define OFI_RMA_DIRECTION_CAPS	(FI_READ | FI_WRITE | \
				 FI_REMOTE_READ | FI_REMOTE_WRITE)

static int fi_valid_addr_format(uint32_t prov_format, uint32_t user_format)
{
	if (user_format == FI_FORMAT_UNSPEC || prov_format == FI_FORMAT_UNSPEC)
		return 1;

	switch (prov_format) {
	case FI_SOCKADDR:
		/* Provider supports INET and INET6 */
		return user_format <= FI_SOCKADDR_IN6;
	case FI_SOCKADDR_IN:
		/* Provider supports INET only */
		return user_format <= FI_SOCKADDR_IN;
	case FI_SOCKADDR_IN6:
		/* Provider supports INET6 only */
		return user_format <= FI_SOCKADDR_IN6;
	case FI_SOCKADDR_IB:
		/* Provider must support IB, INET, and INET6 */
		return user_format <= FI_SOCKADDR_IB;
	default:
		return prov_format == user_format;
	}
}

/*
char *ofi_strdup_head(const char *str)
{
	char *delim;
	delim = strchr(str, OFI_NAME_DELIM);
	return delim ? strndup(str, delim - str) : strdup(str);
}

char *ofi_strdup_tail(const char *str)
{
	char *delim;
	delim = strchr(str, OFI_NAME_DELIM);
	return delim ? strup(delim + 1) : strdup(str);
}
*/

char *ofi_strdup_append(const char *head, const char *tail)
{
	char *str;
	size_t len;

	len = strlen(head) + strlen(tail) + 2;
	str = malloc(len);
	if (str)
		sprintf(str, "%s%c%s", head, OFI_NAME_DELIM, tail);
	return str;
}

int ofi_exclude_prov_name(char **prov_name_list, const char *util_prov_name)
{
	char *exclude, *name, *temp;

	exclude = malloc(strlen(util_prov_name) + 2);
	if (!exclude)
		return -FI_ENOMEM;

	exclude[0] = '^';
	strcpy(&exclude[1], util_prov_name);

	if (!*prov_name_list)
		goto out;

	name = strdup(*prov_name_list);
	if (!name)
		goto err1;

	ofi_rm_substr_delim(name, util_prov_name, OFI_NAME_DELIM);

	if (strlen(name)) {
		temp = ofi_strdup_append(name, exclude);
		if (!temp)
			goto err2;
		free(exclude);
		exclude = temp;
	}
	free(name);
	free(*prov_name_list);
out:
	*prov_name_list = exclude;
	return 0;
err2:
	free(name);
err1:
	free(exclude);
	return -FI_ENOMEM;
}

static int ofi_dup_addr(const struct fi_info *info, struct fi_info *dup)
{
	dup->addr_format = info->addr_format;
	if (info->src_addr) {
		dup->src_addrlen = info->src_addrlen;
		dup->src_addr = mem_dup(info->src_addr, info->src_addrlen);
		if (dup->src_addr == NULL)
			return -FI_ENOMEM;
	}
	if (info->dest_addr) {
		dup->dest_addrlen = info->dest_addrlen;
		dup->dest_addr = mem_dup(info->dest_addr, info->dest_addrlen);
		if (dup->dest_addr == NULL) {
			free(dup->src_addr);
			dup->src_addr = NULL;
			return -FI_ENOMEM;
		}
	}
	return 0;
}

static int ofi_info_to_core(uint32_t version, const struct fi_provider *prov,
			    const struct fi_info *util_info,
			    ofi_alter_info_t info_to_core,
			    struct fi_info **core_hints)
{
	int ret = -FI_ENOMEM;

	if (!(*core_hints = fi_allocinfo()))
		return -FI_ENOMEM;

	if (info_to_core(version, util_info, *core_hints))
		goto err;

	if (!util_info)
		return 0;

	if (ofi_dup_addr(util_info, *core_hints))
		goto err;

	if (util_info->fabric_attr) {
		if (util_info->fabric_attr->name) {
			(*core_hints)->fabric_attr->name =
				strdup(util_info->fabric_attr->name);
			if (!(*core_hints)->fabric_attr->name) {
				FI_WARN(prov, FI_LOG_FABRIC,
					"Unable to allocate fabric name\n");
				goto err;
			}
		}

		if (util_info->fabric_attr->prov_name) {
			(*core_hints)->fabric_attr->prov_name =
				strdup(util_info->fabric_attr->prov_name);
			if (!(*core_hints)->fabric_attr->prov_name) {
				FI_WARN(prov, FI_LOG_FABRIC,
					"Unable to alloc prov name\n");
				goto err;
			}
			ret = ofi_exclude_prov_name(
					&(*core_hints)->fabric_attr->prov_name,
					prov->name);
			if (ret)
				goto err;
		}
	}

	if (util_info->domain_attr && util_info->domain_attr->name) {
		(*core_hints)->domain_attr->name =
			strdup(util_info->domain_attr->name);
		if (!(*core_hints)->domain_attr->name) {
			FI_WARN(prov, FI_LOG_FABRIC,
				"Unable to allocate domain name\n");
			goto err;
		}
	}
	return 0;

err:
	fi_freeinfo(*core_hints);
	return ret;
}

static int ofi_info_to_util(uint32_t version, const struct fi_provider *prov,
			    struct fi_info *core_info,
			    ofi_alter_info_t info_to_util,
			    struct fi_info **util_info)
{
	if (!(*util_info = fi_allocinfo()))
		return -FI_ENOMEM;

	if (info_to_util(version, core_info, *util_info))
		goto err;

	if (ofi_dup_addr(core_info, *util_info))
		goto err;

	/* Release 1.4 brought standardized domain names across IP based
	 * providers. Before this release, the usNIC provider would return a
	 * NULL domain name from fi_getinfo. For compatibility reasons, allow a
	 * NULL domain name when apps are requesting version < 1.4.
	 */
	assert(FI_VERSION_LT(1, 4) || core_info->domain_attr->name);

	if (core_info->domain_attr->name) {
		(*util_info)->domain_attr->name =
			strdup(core_info->domain_attr->name);

		if (!(*util_info)->domain_attr->name) {
			FI_WARN(prov, FI_LOG_FABRIC,
				"Unable to allocate domain name\n");
			goto err;
		}
	}

	(*util_info)->fabric_attr->name = strdup(core_info->fabric_attr->name);
	if (!(*util_info)->fabric_attr->name) {
		FI_WARN(prov, FI_LOG_FABRIC,
			"Unable to allocate fabric name\n");
		goto err;
	}

	(*util_info)->fabric_attr->prov_name = strdup(core_info->fabric_attr->
						      prov_name);
	if (!(*util_info)->fabric_attr->prov_name) {
		FI_WARN(prov, FI_LOG_FABRIC,
			"Unable to allocate fabric name\n");
		goto err;
	}

	return 0;
err:
	fi_freeinfo(*util_info);
	return -FI_ENOMEM;
}

int ofi_get_core_info(uint32_t version, const char *node, const char *service,
		      uint64_t flags, const struct util_prov *util_prov,
		      const struct fi_info *util_hints, ofi_alter_info_t info_to_core,
		      struct fi_info **core_info)
{
	struct fi_info *core_hints = NULL;
	int ret;

	ret = ofi_prov_check_info(util_prov, version, util_hints);
	if (ret)
		return ret;

	ret = ofi_info_to_core(version, util_prov->prov, util_hints, info_to_core,
			       &core_hints);
	if (ret)
		return ret;

	FI_DBG(util_prov->prov, FI_LOG_CORE, "--- Begin ofi_get_core_info ---\n");

	ret = fi_getinfo(version, node, service, flags | OFI_CORE_PROV_ONLY,
			 core_hints, core_info);

	FI_DBG(util_prov->prov, FI_LOG_CORE, "--- End ofi_get_core_info ---\n");

	fi_freeinfo(core_hints);
	return ret;
}

int ofix_getinfo(uint32_t version, const char *node, const char *service,
		 uint64_t flags, const struct util_prov *util_prov,
		 const struct fi_info *hints, ofi_alter_info_t info_to_core,
		 ofi_alter_info_t info_to_util, struct fi_info **info)
{
	struct fi_info *core_info, *util_info, *cur, *tail;
	int ret;

	ret = ofi_get_core_info(version, node, service, flags, util_prov,
				hints, info_to_core, &core_info);
	if (ret)
		return ret;

	*info = tail = NULL;
	for (cur = core_info; cur; cur = cur->next) {
		ret = ofi_info_to_util(version, util_prov->prov, cur,
				       info_to_util, &util_info);
		if (ret) {
			fi_freeinfo(*info);
			break;
		}

		ofi_alter_info(util_info, hints, version);
		if (!*info)
			*info = util_info;
		else
			tail->next = util_info;
		tail = util_info;
	}
	fi_freeinfo(core_info);
	return ret;
}

/* Caller should use only fabric_attr in returned core_info */
int ofi_get_core_info_fabric(const struct fi_provider *prov,
			     const struct fi_fabric_attr *util_attr,
			     struct fi_info **core_info)
{
	struct fi_info hints;
	int ret;

	/* ofix_getinfo() would append utility provider name after core / lower
	 * layer provider name */
	if (!strstr(util_attr->prov_name, prov->name))
		return -FI_ENODATA;

	memset(&hints, 0, sizeof hints);
	if (!(hints.fabric_attr = calloc(1, sizeof(*hints.fabric_attr))))
		return -FI_ENOMEM;

	hints.fabric_attr->prov_name = strdup(util_attr->prov_name);
	if (!hints.fabric_attr->prov_name) {
		ret = -FI_ENOMEM;
		goto out;
	}

	ret = ofi_exclude_prov_name(&hints.fabric_attr->prov_name, prov->name);
	if (ret)
		goto out;

	hints.fabric_attr->name = util_attr->name;
	hints.fabric_attr->api_version = util_attr->api_version;
	hints.mode = ~0;

	ret = fi_getinfo(util_attr->api_version, NULL, NULL, OFI_CORE_PROV_ONLY,
	                 &hints, core_info);

	free(hints.fabric_attr->prov_name);
out:
	free(hints.fabric_attr);
	return ret;
}

int ofi_check_fabric_attr(const struct fi_provider *prov,
			  const struct fi_fabric_attr *prov_attr,
			  const struct fi_fabric_attr *user_attr)
{
	/* Provider names are checked by the framework */

	if (user_attr->prov_version > prov_attr->prov_version) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported provider version\n");
		return -FI_ENODATA;
	}

	if (FI_VERSION_LT(user_attr->api_version, prov_attr->api_version)) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported api version\n");
		return -FI_ENODATA;
	}

	return 0;
}

/*
 * Threading models ranked by order of parallelism.
 */
static int fi_thread_level(enum fi_threading thread_model)
{
	switch (thread_model) {
	case FI_THREAD_SAFE:
		return 1;
	case FI_THREAD_FID:
		return 2;
	case FI_THREAD_ENDPOINT:
		return 3;
	case FI_THREAD_COMPLETION:
		return 4;
	case FI_THREAD_DOMAIN:
		return 5;
	case FI_THREAD_UNSPEC:
		return 6;
	default:
		return -1;
	}
}

/*
 * Progress models ranked by order of automation.
 */
static int fi_progress_level(enum fi_progress progress_model)
{
	switch (progress_model) {
	case FI_PROGRESS_AUTO:
		return 1;
	case FI_PROGRESS_MANUAL:
		return 2;
	case FI_PROGRESS_UNSPEC:
		return 3;
	default:
		return -1;
	}
}

/*
 * Resource management models ranked by order of enablement.
 */
static int fi_resource_mgmt_level(enum fi_resource_mgmt rm_model)
{
	switch (rm_model) {
	case FI_RM_ENABLED:
		return 1;
	case FI_RM_DISABLED:
		return 2;
	case FI_RM_UNSPEC:
		return 3;
	default:
		return -1;
	}
}

/*
 * Remove unneeded MR mode bits based on the requested capability bits.
 */
static int ofi_cap_mr_mode(uint64_t info_caps, int mr_mode)
{
	if (!ofi_rma_target_allowed(info_caps)) {
		if (!(mr_mode & (FI_MR_LOCAL | FI_MR_HMEM)))
			return 0;

		mr_mode &= ~OFI_MR_MODE_RMA_TARGET;
	}

	return mr_mode & ~(FI_MR_BASIC | FI_MR_SCALABLE);
}

/*
 * Providers should set v1.0 registration modes (FI_MR_BASIC and
 * FI_MR_SCALABLE) that they support, along with all required modes.
 */
int ofi_check_mr_mode(const struct fi_provider *prov, uint32_t api_version,
		      int prov_mode, const struct fi_info *user_info)
{
	int user_mode = user_info->domain_attr->mr_mode;
	int ret = -FI_ENODATA;

	if ((prov_mode & FI_MR_LOCAL) &&
	    !((user_info->mode & FI_LOCAL_MR) || (user_mode & FI_MR_LOCAL)))
		goto out;

	if (FI_VERSION_LT(api_version, FI_VERSION(1, 5))) {
		switch (user_mode) {
		case FI_MR_UNSPEC:
			if (!(prov_mode & (FI_MR_SCALABLE | FI_MR_BASIC)))
				goto out;
			break;
		case FI_MR_BASIC:
			if (!(prov_mode & FI_MR_BASIC))
				goto out;
			break;
		case FI_MR_SCALABLE:
			if (!(prov_mode & FI_MR_SCALABLE))
				goto out;
			break;
		default:
			goto out;
		}
	} else {
		if (user_mode & FI_MR_BASIC) {
			if ((user_mode & ~FI_MR_BASIC) ||
			    !(prov_mode & FI_MR_BASIC))
				goto out;
		} else if (user_mode & FI_MR_SCALABLE) {
			if ((user_mode & ~FI_MR_SCALABLE) ||
			    !(prov_mode & FI_MR_SCALABLE))
				goto out;
		} else {
			prov_mode = ofi_cap_mr_mode(user_info->caps, prov_mode);
			if ((user_mode & prov_mode) != prov_mode)
				goto out;
		}
	}

	ret = 0;
out:
	if (ret) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid memory registration mode\n");
		FI_INFO_MR_MODE(prov, prov_mode, user_mode);
	}

	return ret;
}

int ofi_check_domain_attr(const struct fi_provider *prov, uint32_t api_version,
			  const struct fi_domain_attr *prov_attr,
			  const struct fi_info *user_info)
{
	const struct fi_domain_attr *user_attr = user_info->domain_attr;

	if (fi_thread_level(user_attr->threading) <
	    fi_thread_level(prov_attr->threading)) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid threading model\n");
		return -FI_ENODATA;
	}

	if (fi_progress_level(user_attr->control_progress) <
	    fi_progress_level(prov_attr->control_progress)) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid control progress model\n");
		return -FI_ENODATA;
	}

	if (fi_progress_level(user_attr->data_progress) <
	    fi_progress_level(prov_attr->data_progress)) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid data progress model\n");
		return -FI_ENODATA;
	}

	if (fi_resource_mgmt_level(user_attr->resource_mgmt) <
	    fi_resource_mgmt_level(prov_attr->resource_mgmt)) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid resource mgmt model\n");
		return -FI_ENODATA;
	}

	if ((prov_attr->av_type != FI_AV_UNSPEC) &&
	    (user_attr->av_type != FI_AV_UNSPEC) &&
	    (prov_attr->av_type != user_attr->av_type)) {
		FI_INFO(prov, FI_LOG_CORE, "Invalid AV type\n");
	   	return -FI_ENODATA;
	}

	if (user_attr->cq_data_size > prov_attr->cq_data_size) {
		FI_INFO(prov, FI_LOG_CORE, "CQ data size too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, cq_data_size);
		return -FI_ENODATA;
	}

	if (ofi_check_mr_mode(prov, api_version, prov_attr->mr_mode, user_info))
		return -FI_ENODATA;

	if (user_attr->max_ep_stx_ctx > prov_attr->max_ep_stx_ctx) {
		FI_INFO(prov, FI_LOG_CORE, "max_ep_stx_ctx greater than supported\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, max_ep_stx_ctx);
	}

	if (user_attr->max_ep_srx_ctx > prov_attr->max_ep_srx_ctx) {
		FI_INFO(prov, FI_LOG_CORE, "max_ep_srx_ctx greater than supported\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, max_ep_srx_ctx);
	}

	/* following checks only apply to api 1.5 and beyond */
	if (FI_VERSION_LT(api_version, FI_VERSION(1, 5)))
		return 0;

	if (user_attr->cntr_cnt > prov_attr->cntr_cnt) {
		FI_INFO(prov, FI_LOG_CORE, "Cntr count too large\n");
		return -FI_ENODATA;
	}

	if (user_attr->mr_iov_limit > prov_attr->mr_iov_limit) {
		FI_INFO(prov, FI_LOG_CORE, "MR iov limit too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, mr_iov_limit);
		return -FI_ENODATA;
	}

	if (user_attr->caps & ~(prov_attr->caps)) {
		FI_INFO(prov, FI_LOG_CORE, "Requested domain caps not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	if ((user_attr->mode & prov_attr->mode) != prov_attr->mode) {
		FI_INFO(prov, FI_LOG_CORE, "Required domain mode missing\n");
		FI_INFO_MODE(prov, prov_attr->mode, user_attr->mode);
		return -FI_ENODATA;
	}

	if (user_attr->max_err_data > prov_attr->max_err_data) {
		FI_INFO(prov, FI_LOG_CORE, "Max err data too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, max_err_data);
		return -FI_ENODATA;
	}

	if (user_attr->mr_cnt > prov_attr->mr_cnt) {
		FI_INFO(prov, FI_LOG_CORE, "MR count too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, mr_cnt);
		return -FI_ENODATA;
	}

	return 0;
}

int ofi_check_ep_type(const struct fi_provider *prov,
		      const struct fi_ep_attr *prov_attr,
		      const struct fi_ep_attr *user_attr)
{
	if ((user_attr->type != FI_EP_UNSPEC) &&
	    (prov_attr->type != FI_EP_UNSPEC) &&
	    (user_attr->type != prov_attr->type)) {
		FI_INFO(prov, FI_LOG_CORE, "unsupported endpoint type\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, type, FI_TYPE_EP_TYPE);
		return -FI_ENODATA;
	}
	return 0;
}

int ofi_check_ep_attr(const struct util_prov *util_prov, uint32_t api_version,
		      const struct fi_info *prov_info,
		      const struct fi_info *user_info)
{
	const struct fi_ep_attr *prov_attr = prov_info->ep_attr;
	const struct fi_ep_attr *user_attr = user_info->ep_attr;
	const struct fi_provider *prov = util_prov->prov;
	int ret;

	ret = ofi_check_ep_type(prov, prov_attr, user_attr);
	if (ret)
		return ret;

	if ((user_attr->protocol != FI_PROTO_UNSPEC) &&
	    (user_attr->protocol != prov_attr->protocol)) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported protocol\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, protocol, FI_TYPE_PROTOCOL);
		return -FI_ENODATA;
	}

	if (user_attr->protocol_version &&
	    (user_attr->protocol_version > prov_attr->protocol_version)) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported protocol version\n");
		return -FI_ENODATA;
	}

	if (user_attr->max_msg_size > prov_attr->max_msg_size) {
		FI_INFO(prov, FI_LOG_CORE, "Max message size too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, max_msg_size);
		return -FI_ENODATA;
	}

	if (user_attr->tx_ctx_cnt > prov_info->domain_attr->max_ep_tx_ctx) {
		if (user_attr->tx_ctx_cnt == FI_SHARED_CONTEXT) {
			if (!prov_info->domain_attr->max_ep_stx_ctx) {
				FI_INFO(prov, FI_LOG_CORE,
					"Shared tx context not supported\n");
				return -FI_ENODATA;
			}
		} else {
			FI_INFO(prov, FI_LOG_CORE,
				"Requested tx_ctx_cnt exceeds supported."
				" Expected:%zd, Requested%zd\n",
				prov_info->domain_attr->max_ep_tx_ctx,
				user_attr->tx_ctx_cnt);
			return -FI_ENODATA;
		}
	}

	if (user_attr->rx_ctx_cnt > prov_info->domain_attr->max_ep_rx_ctx) {
		if (user_attr->rx_ctx_cnt == FI_SHARED_CONTEXT) {
			if (!prov_info->domain_attr->max_ep_srx_ctx) {
				FI_INFO(prov, FI_LOG_CORE,
					"Shared rx context not supported\n");
				return -FI_ENODATA;
			}
		} else {
			FI_INFO(prov, FI_LOG_CORE,
				"Requested rx_ctx_cnt exceeds supported."
				" Expected: %zd, Requested:%zd\n",
				prov_info->domain_attr->max_ep_rx_ctx,
				user_attr->rx_ctx_cnt);
			return -FI_ENODATA;
		}
	}

	if (user_info->caps & (FI_RMA | FI_ATOMIC)) {
		if (user_attr->max_order_raw_size >
		    prov_attr->max_order_raw_size) {
			FI_INFO(prov, FI_LOG_CORE,
				"Max order RAW size exceeds supported size\n");
			FI_INFO_CHECK_VAL(prov, prov_attr, user_attr,
					  max_order_raw_size);
			return -FI_ENODATA;
		}

		if (user_attr->max_order_war_size >
		    prov_attr->max_order_war_size) {
			FI_INFO(prov, FI_LOG_CORE,
				"Max order WAR size exceeds supported size\n");
			FI_INFO_CHECK_VAL(prov, prov_attr, user_attr,
					  max_order_war_size);
			return -FI_ENODATA;
		}

		if (user_attr->max_order_waw_size >
		    prov_attr->max_order_waw_size) {
			FI_INFO(prov, FI_LOG_CORE,
				"Max order WAW size exceeds supported size\n");
			FI_INFO_CHECK_VAL(prov, prov_attr, user_attr,
					  max_order_waw_size);
			return -FI_ENODATA;
		}
	}

	if (user_attr->auth_key_size &&
	    (user_attr->auth_key_size != prov_attr->auth_key_size)) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported authentication size.");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, auth_key_size);
		return -FI_ENODATA;
	}

	return 0;
}

int ofi_check_rx_attr(const struct fi_provider *prov,
		      const struct fi_info *prov_info,
		      const struct fi_rx_attr *user_attr, uint64_t info_mode)
{
	const struct fi_rx_attr *prov_attr = prov_info->rx_attr;
	int rm_enabled = (prov_info->domain_attr->resource_mgmt == FI_RM_ENABLED);

	if (user_attr->caps & ~OFI_IGNORED_RX_CAPS)
		FI_INFO(prov, FI_LOG_CORE, "Tx only caps ignored in Rx caps\n");

	if ((user_attr->caps & ~OFI_IGNORED_RX_CAPS) & ~(prov_attr->caps)) {
		FI_INFO(prov, FI_LOG_CORE, "caps not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	info_mode = user_attr->mode ? user_attr->mode : info_mode;
	if ((info_mode & prov_attr->mode) != prov_attr->mode) {
		FI_INFO(prov, FI_LOG_CORE, "needed mode not set\n");
		FI_INFO_MODE(prov, prov_attr->mode, user_attr->mode);
		return -FI_ENODATA;
	}

	if (user_attr->op_flags & ~(prov_attr->op_flags)) {
		FI_INFO(prov, FI_LOG_CORE, "op_flags not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, op_flags,
			     FI_TYPE_OP_FLAGS);
		return -FI_ENODATA;
	}

	if (user_attr->msg_order & ~(prov_attr->msg_order)) {
		FI_INFO(prov, FI_LOG_CORE, "msg_order not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, msg_order,
			     FI_TYPE_MSG_ORDER);
		return -FI_ENODATA;
	}

	if (user_attr->comp_order & ~(prov_attr->comp_order)) {
		FI_INFO(prov, FI_LOG_CORE, "comp_order not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, comp_order,
			     FI_TYPE_MSG_ORDER);
		return -FI_ENODATA;
	}

	if (user_attr->total_buffered_recv > prov_attr->total_buffered_recv) {
		FI_INFO(prov, FI_LOG_CORE, "total_buffered_recv too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr,
				  total_buffered_recv);
		return -FI_ENODATA;
	}

	if (user_attr->size > prov_attr->size) {
		FI_INFO(prov, FI_LOG_CORE, "size is greater than supported\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, size);
		return -FI_ENODATA;
	}

	if (user_attr->iov_limit > prov_attr->iov_limit) {
		FI_INFO(prov, FI_LOG_CORE, "iov_limit too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, iov_limit);
		return -FI_ENODATA;
	}

	if (!rm_enabled &&
	    user_attr->total_buffered_recv > prov_attr->total_buffered_recv) {
		/* Just log a notification, but ignore the value */
		FI_INFO(prov, FI_LOG_CORE,
			"Total buffered recv size exceeds supported size\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr,
				  total_buffered_recv);
	}

	return 0;
}

int ofi_check_attr_subset(const struct fi_provider *prov,
		uint64_t base_caps, uint64_t requested_caps)
{
	uint64_t expanded_caps;

	expanded_caps = base_caps;
	if (base_caps & (FI_MSG | FI_TAGGED)) {
		if (!(base_caps & OFI_MSG_DIRECTION_CAPS))
			expanded_caps |= OFI_MSG_DIRECTION_CAPS;
	}
	if (base_caps & (FI_RMA | FI_ATOMIC)) {
		if (!(base_caps & OFI_RMA_DIRECTION_CAPS))
			expanded_caps |= OFI_RMA_DIRECTION_CAPS;
	}

	if (~expanded_caps & requested_caps) {
		FI_INFO(prov, FI_LOG_CORE,
			"requested caps not subset of base endpoint caps\n");
		FI_INFO_FIELD(prov, expanded_caps, requested_caps,
			"Supported", "Requested", FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	return 0;
}

int ofi_check_tx_attr(const struct fi_provider *prov,
		      const struct fi_tx_attr *prov_attr,
		      const struct fi_tx_attr *user_attr, uint64_t info_mode)
{
	if (user_attr->caps & ~OFI_IGNORED_TX_CAPS)
		FI_INFO(prov, FI_LOG_CORE, "Rx only caps ignored in Tx caps\n");

	if ((user_attr->caps & ~OFI_IGNORED_TX_CAPS) & ~(prov_attr->caps)) {
		FI_INFO(prov, FI_LOG_CORE, "caps not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	info_mode = user_attr->mode ? user_attr->mode : info_mode;
	if ((info_mode & prov_attr->mode) != prov_attr->mode) {
		FI_INFO(prov, FI_LOG_CORE, "needed mode not set\n");
		FI_INFO_MODE(prov, prov_attr->mode, user_attr->mode);
		return -FI_ENODATA;
	}

	if (user_attr->op_flags & ~(prov_attr->op_flags)) {
		FI_INFO(prov, FI_LOG_CORE, "op_flags not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, op_flags,
			     FI_TYPE_OP_FLAGS);
		return -FI_ENODATA;
	}

	if (user_attr->msg_order & ~(prov_attr->msg_order)) {
		FI_INFO(prov, FI_LOG_CORE, "msg_order not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, msg_order,
			     FI_TYPE_MSG_ORDER);
		return -FI_ENODATA;
	}

	if (user_attr->comp_order & ~(prov_attr->comp_order)) {
		FI_INFO(prov, FI_LOG_CORE, "comp_order not supported\n");
		FI_INFO_CHECK(prov, prov_attr, user_attr, comp_order,
			     FI_TYPE_MSG_ORDER);
		return -FI_ENODATA;
	}

	if (user_attr->inject_size > prov_attr->inject_size) {
		FI_INFO(prov, FI_LOG_CORE, "inject_size too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, inject_size);
		return -FI_ENODATA;
	}

	if (user_attr->size > prov_attr->size) {
		FI_INFO(prov, FI_LOG_CORE, "size is greater than supported\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, size);
		return -FI_ENODATA;
	}

	if (user_attr->iov_limit > prov_attr->iov_limit) {
		FI_INFO(prov, FI_LOG_CORE, "iov_limit too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, iov_limit);
		return -FI_ENODATA;
	}

	if (user_attr->rma_iov_limit > prov_attr->rma_iov_limit) {
		FI_INFO(prov, FI_LOG_CORE, "rma_iov_limit too large\n");
		FI_INFO_CHECK_VAL(prov, prov_attr, user_attr, rma_iov_limit);
		return -FI_ENODATA;
	}

	return 0;
}

/* Use if there are multiple fi_info in the provider:
 * check provider's info */
int ofi_prov_check_info(const struct util_prov *util_prov,
			uint32_t api_version,
			const struct fi_info *user_info)
{
	const struct fi_info *prov_info = util_prov->info;
	size_t success_info = 0;
	int ret;

	if (!user_info)
		return FI_SUCCESS;

	for ( ; prov_info; prov_info = prov_info->next) {
		ret = ofi_check_info(util_prov, prov_info,
				     api_version, user_info);
		if (!ret)
			success_info++;
	}

	return (!success_info ? -FI_ENODATA : FI_SUCCESS);
}

/* Use if there are multiple fi_info in the provider:
 * check and duplicate provider's info */
int ofi_prov_check_dup_info(const struct util_prov *util_prov,
			    uint32_t api_version,
			    const struct fi_info *user_info,
			    struct fi_info **info)
{
	const struct fi_info *prov_info = util_prov->info;
	const struct fi_provider *prov = util_prov->prov;
	struct fi_info *fi, *tail;
	int ret;

	if (!info)
		return -FI_EINVAL;

	*info = tail = NULL;

	for ( ; prov_info; prov_info = prov_info->next) {
		ret = ofi_check_info(util_prov, prov_info,
				     api_version, user_info);
	    	if (ret)
			continue;
		if (!(fi = fi_dupinfo(prov_info))) {
			ret = -FI_ENOMEM;
			goto err;
		}
		if (!*info)
			*info = fi;
		else
			tail->next = fi;
		tail = fi;
	}

	return !*info ? -FI_ENODATA : FI_SUCCESS;
err:
	fi_freeinfo(*info);
	FI_INFO(prov, FI_LOG_CORE,
		"cannot copy info\n");
	return ret;
}

/* Use if there is only single fi_info in the provider */
int ofi_check_info(const struct util_prov *util_prov,
		   const struct fi_info *prov_info, uint32_t api_version,
		   const struct fi_info *user_info)
{
	const struct fi_provider *prov = util_prov->prov;
	uint64_t prov_mode;
	int ret;

	if (!user_info)
		return 0;

	/* Check oft-used endpoint type attribute first to avoid any other
	 * unnecessary check */
	if (user_info->ep_attr) {
		ret = ofi_check_ep_type(prov, prov_info->ep_attr,
					user_info->ep_attr);
		if (ret)
			return ret;
	}

	if (user_info->caps & ~(prov_info->caps)) {
		FI_INFO(prov, FI_LOG_CORE, "Unsupported capabilities\n");
		FI_INFO_CHECK(prov, prov_info, user_info, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	prov_mode = ofi_mr_get_prov_mode(api_version, user_info, prov_info);

	if ((user_info->mode & prov_mode) != prov_mode) {
		FI_INFO(prov, FI_LOG_CORE, "needed mode not set\n");
		FI_INFO_MODE(prov, prov_mode, user_info->mode);
		return -FI_ENODATA;
	}

	if (!fi_valid_addr_format(prov_info->addr_format,
				  user_info->addr_format)) {
		FI_INFO(prov, FI_LOG_CORE, "address format not supported\n");
		FI_INFO_CHECK(prov, prov_info, user_info, addr_format,
			      FI_TYPE_ADDR_FORMAT);
		return -FI_ENODATA;
	}

	if (user_info->fabric_attr) {
		ret = ofi_check_fabric_attr(prov, prov_info->fabric_attr,
					    user_info->fabric_attr);
		if (ret)
			return ret;
	}

	if (user_info->domain_attr) {
		ret = ofi_check_domain_attr(prov, api_version,
					    prov_info->domain_attr,
					    user_info);
		if (ret)
			return ret;
	}

	if (user_info->ep_attr) {
		ret = ofi_check_ep_attr(util_prov, api_version,
					prov_info, user_info);
		if (ret)
			return ret;
	}

	if (user_info->rx_attr) {
		ret = ofi_check_rx_attr(prov, prov_info,
					user_info->rx_attr, user_info->mode);
		if (ret)
			return ret;
	}

	if (user_info->tx_attr) {
		ret = ofi_check_tx_attr(prov, prov_info->tx_attr,
					user_info->tx_attr, user_info->mode);
		if (ret)
			return ret;
	}
	return 0;
}

static uint64_t ofi_get_caps(uint64_t info_caps, uint64_t hint_caps,
			    uint64_t attr_caps)
{
	uint64_t caps;

	if (!hint_caps) {
		caps = (info_caps & attr_caps & FI_PRIMARY_CAPS) |
		       (attr_caps & FI_SECONDARY_CAPS);
	} else {
		caps = (hint_caps & FI_PRIMARY_CAPS) |
		       (attr_caps & FI_SECONDARY_CAPS);
	}

	if (caps & (FI_MSG | FI_TAGGED) && !(caps & OFI_MSG_DIRECTION_CAPS))
		caps |= (attr_caps & OFI_MSG_DIRECTION_CAPS);
	if (caps & (FI_RMA | FI_ATOMICS) && !(caps & OFI_RMA_DIRECTION_CAPS))
		caps |= (attr_caps & OFI_RMA_DIRECTION_CAPS);

	return caps;
}

static void fi_alter_domain_attr(struct fi_domain_attr *attr,
				 const struct fi_domain_attr *hints,
				 uint64_t info_caps, uint32_t api_version)
{
	int hints_mr_mode;

	hints_mr_mode = hints ? hints->mr_mode : 0;
	if (hints_mr_mode & (FI_MR_BASIC | FI_MR_SCALABLE)) {
		attr->mr_mode = hints_mr_mode;
	} else if (FI_VERSION_LT(api_version, FI_VERSION(1, 5))) {
		attr->mr_mode = (attr->mr_mode && attr->mr_mode != FI_MR_SCALABLE) ?
				FI_MR_BASIC : FI_MR_SCALABLE;
	} else {
		if ((hints_mr_mode & attr->mr_mode) != attr->mr_mode) {
			attr->mr_mode = ofi_cap_mr_mode(info_caps,
						attr->mr_mode & hints_mr_mode);
		}
	}

	attr->caps = ofi_get_caps(info_caps, hints ? hints->caps : 0, attr->caps);
	if (!hints)
		return;

	if (hints->threading)
		attr->threading = hints->threading;
	if (hints->control_progress)
		attr->control_progress = hints->control_progress;
	if (hints->data_progress)
		attr->data_progress = hints->data_progress;
	if (hints->av_type)
		attr->av_type = hints->av_type;
}

static void fi_alter_ep_attr(struct fi_ep_attr *attr,
			     const struct fi_ep_attr *hints,
			     uint64_t info_caps)
{
	if (!hints)
		return;

	if (info_caps & (FI_RMA | FI_ATOMIC)) {
		if (hints->max_order_raw_size)
			attr->max_order_raw_size = hints->max_order_raw_size;
		if (hints->max_order_war_size)
			attr->max_order_war_size = hints->max_order_war_size;
		if (hints->max_order_waw_size)
			attr->max_order_waw_size = hints->max_order_waw_size;
	}
	if (hints->tx_ctx_cnt)
		attr->tx_ctx_cnt = hints->tx_ctx_cnt;
	if (hints->rx_ctx_cnt)
		attr->rx_ctx_cnt = hints->rx_ctx_cnt;
}

static void fi_alter_rx_attr(struct fi_rx_attr *attr,
			     const struct fi_rx_attr *hints,
			     uint64_t info_caps)
{
	attr->caps = ofi_get_caps(info_caps, hints ? hints->caps : 0, attr->caps);
	if (!hints)
		return;

	attr->op_flags = hints->op_flags;
	attr->total_buffered_recv = hints->total_buffered_recv;
	if (hints->size)
		attr->size = hints->size;
	if (hints->iov_limit)
		attr->iov_limit = hints->iov_limit;
}

static void fi_alter_tx_attr(struct fi_tx_attr *attr,
			     const struct fi_tx_attr *hints,
			     uint64_t info_caps)
{
	attr->caps = ofi_get_caps(info_caps, hints ? hints->caps : 0, attr->caps);
	if (!hints)
		return;

	attr->op_flags = hints->op_flags;
	if (hints->inject_size)
		attr->inject_size = hints->inject_size;
	if (hints->size)
		attr->size = hints->size;
	if (hints->iov_limit)
		attr->iov_limit = hints->iov_limit;
	if (hints->rma_iov_limit)
		attr->rma_iov_limit = hints->rma_iov_limit;
}

static uint64_t ofi_get_info_caps(const struct fi_info *prov_info,
				  const struct fi_info *user_info,
				  uint32_t api_version)
{
	int prov_mode, user_mode;
	uint64_t caps;

	assert(user_info);

	caps = ofi_get_caps(prov_info->caps, user_info->caps, prov_info->caps);

	prov_mode = prov_info->domain_attr->mr_mode;

	if (!ofi_rma_target_allowed(caps) ||
	    !(prov_mode & OFI_MR_MODE_RMA_TARGET))
		return caps;

	if (!user_info->domain_attr)
		goto trim_caps;

	user_mode = user_info->domain_attr->mr_mode;

	if ((FI_VERSION_LT(api_version, FI_VERSION(1,5)) &&
	    (user_mode == FI_MR_UNSPEC)) ||
	    (user_mode == FI_MR_BASIC) ||
	    ((user_mode & prov_mode & OFI_MR_MODE_RMA_TARGET) == 
	     (prov_mode & OFI_MR_MODE_RMA_TARGET)))
		return caps;

trim_caps:
	return caps & ~(FI_REMOTE_WRITE | FI_REMOTE_READ);
}

/*
 * Alter the returned fi_info based on the user hints.  We assume that
 * the hints have been validated and the starting fi_info is properly
 * configured by the provider.
 */
void ofi_alter_info(struct fi_info *info, const struct fi_info *hints,
		    uint32_t api_version)
{
	if (!hints)
		return;

	for (; info; info = info->next) {
		/* This should stay before call to fi_alter_domain_attr as
		 * the checks depend on unmodified provider mr_mode attr */
		info->caps = ofi_get_info_caps(info, hints, api_version);

		if ((info->domain_attr->mr_mode & FI_MR_LOCAL) &&
		    (FI_VERSION_LT(api_version, FI_VERSION(1, 5)) ||
		     (hints && hints->domain_attr &&
		      (hints->domain_attr->mr_mode & (FI_MR_BASIC | FI_MR_SCALABLE)))))
			info->mode |= FI_LOCAL_MR;

		info->handle = hints->handle;

		fi_alter_domain_attr(info->domain_attr, hints->domain_attr,
				     info->caps, api_version);
		fi_alter_ep_attr(info->ep_attr, hints->ep_attr, info->caps);
		fi_alter_rx_attr(info->rx_attr, hints->rx_attr, info->caps);
		fi_alter_tx_attr(info->tx_attr, hints->tx_attr, info->caps);
	}
}
