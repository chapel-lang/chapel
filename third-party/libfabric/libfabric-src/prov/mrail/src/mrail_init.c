/*
 * Copyright (c) 2018-2019 Intel Corporation, Inc.  All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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

#include <shared/ofi_str.h>

#include "mrail.h"

static char **mrail_addr_strv = NULL;
/* Not thread safe */
struct fi_info *mrail_info_vec[MRAIL_MAX_INFO] = {0};
size_t mrail_num_info = 0;

struct mrail_config mrail_config[MRAIL_MAX_CONFIG] = {
	{ .max_size = 16384, .policy = MRAIL_POLICY_FIXED },
	{ .max_size = ULONG_MAX, .policy = MRAIL_POLICY_STRIPING },
};
int mrail_num_config = 2;
int mrail_local_rank = 0;

static inline char **mrail_split_addr_strc(const char *addr_strc)
{
	char **addr_strv = ofi_split_and_alloc(addr_strc, ",", NULL);
	if (!addr_strv) {
		FI_WARN(&mrail_prov, FI_LOG_CORE,
			"Unable to split a FI_ADDR_STRV string\n");
		return NULL;
	}
	return addr_strv;
}

static int mrail_parse_env_vars(void)
{
	char *str, *token, *alg, *p;
	char *addr_strc;
	int ret;
	int i;

	/* experimental, subject to change */
	fi_param_define(&mrail_prov, "config", FI_PARAM_STRING,
			"Comma separated list of '<max_size>:<policy>' pairs, "
			"with <max_size> in ascending order and <policy> being "
			"fixed, round-robin, or striping");
	ret = fi_param_get_str(&mrail_prov, "config", &str);
	if (!ret) {
		for (i = 0; i < MRAIL_MAX_CONFIG; i++) {
			token = strsep(&str, ",");
			if (!token)
				break;

			mrail_config[i].max_size = strtoul(token, &p, 0);
			if (p == token)
				mrail_config[i].max_size = ULONG_MAX;

			mrail_config[i].policy = MRAIL_POLICY_FIXED;
			alg = strchr(token, ':');
			if (!alg)
				continue;

			alg++;
			if (!alg[0] || !strcasecmp(alg, "fixed")) {
				/* use the default */
			} else if (!strcasecmp(alg, "round-robin")) {
				mrail_config[i].policy = MRAIL_POLICY_ROUND_ROBIN;
			} else if (!strcasecmp(alg, "striping")) {
				mrail_config[i].policy = MRAIL_POLICY_STRIPING;
			} else {
				FI_WARN(&mrail_prov, FI_LOG_CORE, "Invalid policy "
					"specification %s\n", alg);
				break;
			}
		}
		mrail_num_config = i;
	}

	fi_param_define(&mrail_prov, "addr_strc", FI_PARAM_STRING, "Deprecated. "
			"Replaced by FI_OFI_MRAIL_ADDR.");

	fi_param_define(&mrail_prov, "addr", FI_PARAM_STRING, "Comma separated list "
			"of rail addresses (FI_ADDR_STR, host name, IP address, or "
			"netdev interface name)");

	ret = fi_param_get_str(&mrail_prov, "addr", &addr_strc);
	if (ret)
		ret = fi_param_get_str(&mrail_prov, "addr_strc", &addr_strc);
	if (ret) {
		FI_INFO(&mrail_prov, FI_LOG_CORE, "unable to read "
			"FI_OFI_MRAIL_ADDR env variable\n");
		return ret;
	}
	mrail_addr_strv = mrail_split_addr_strc(addr_strc);
	if (!mrail_addr_strv) {
		FI_WARN(&mrail_prov, FI_LOG_CORE, "unable to alloc memory\n");
		return -FI_ENOMEM;
	}

	/*
	 * Local rank is used to set the default tx rail when fixed mapping
	 * is used.
	 */
	str = getenv("MPI_LOCALRANKID");
	if (!str)
		str = getenv("OMPI_COMM_WORLD_LOCAL_RANK");
	if (str)
		mrail_local_rank = atoi(str);

	return 0;
}

static struct fi_info *mrail_create_core_hints(const struct fi_info *hints)
{
	struct fi_info *core_hints;
	uint64_t removed_mode;
	uint64_t removed_mr_mode;

	core_hints = fi_dupinfo(hints);
	if (!core_hints)
		return NULL;

	if (!hints) {
		core_hints->mode = MRAIL_PASSTHRU_MODES;
		assert(core_hints->domain_attr);
		core_hints->domain_attr->mr_mode = MRAIL_PASSTHRU_MR_MODES;
	} else {
		removed_mode = core_hints->mode & ~MRAIL_PASSTHRU_MODES;
		if (removed_mode) {
			FI_INFO(&mrail_prov, FI_LOG_CORE,
				"Unable to pass through given modes: %s\n",
				fi_tostr(&removed_mode, FI_TYPE_MODE));
		}
		core_hints->mode &= MRAIL_PASSTHRU_MODES;

		if (core_hints->domain_attr) {
			if (core_hints->domain_attr->mr_mode == FI_MR_BASIC)
				core_hints->domain_attr->mr_mode = OFI_MR_BASIC_MAP;
			removed_mr_mode = core_hints->domain_attr->mr_mode &
					  ~MRAIL_PASSTHRU_MR_MODES;
			if (removed_mr_mode) {
				FI_INFO(&mrail_prov, FI_LOG_CORE,
					"Unable to pass through given MR modes: %s\n",
					fi_tostr(&removed_mr_mode, FI_TYPE_MR_MODE));
			}
			core_hints->domain_attr->mr_mode &= MRAIL_PASSTHRU_MR_MODES;
		}

		if (hints->tx_attr) {
			if (hints->tx_attr->iov_limit)
				core_hints->tx_attr->iov_limit =
					hints->tx_attr->iov_limit + 1;
			if (hints->rx_attr->iov_limit)
				core_hints->rx_attr->iov_limit =
					hints->rx_attr->iov_limit + 1;
			core_hints->tx_attr->op_flags &= ~FI_COMPLETION;
		}
	}

	core_hints->mode |= FI_BUFFERED_RECV;
	core_hints->caps |= FI_SOURCE;

	if (!core_hints->fabric_attr) {
		core_hints->fabric_attr = calloc(1, sizeof(*core_hints->fabric_attr));
		if (!core_hints->fabric_attr)
			goto err;
	}

	if (!core_hints->domain_attr) {
		core_hints->domain_attr = calloc(1, sizeof(*core_hints->domain_attr));
		if (!core_hints->domain_attr)
			goto err;
	}
	core_hints->domain_attr->av_type = FI_AV_TABLE;
	return core_hints;

err:
	fi_freeinfo(core_hints);
	return NULL;
}

/*
 * Gather entries from an array of per-rail info lists to form an array of multi-rail info.
 * Each multi-rail info is a list consists of one info per rail.
 */
static int mrail_gather_rail_info(struct fi_info **rail_info, int num_rails,
				  struct fi_info ***info_out, int *num_info_out)
{
	struct fi_info **mrail_info, **tails, *p;
	int num_mrail_info = 0;
	int i, j, n;

	/*
	 * find the shortest length of the per-rail list, this is the number of
	 * multi-rail info available
	 */
	for (i = 0; i < num_rails; i++) {
		n = 0;
		p = rail_info[i];
		while (p) {
			n++;
			p = p->next;
		}
		if (i == 0 || num_mrail_info > n)
			num_mrail_info = n;
	}

	if (!num_mrail_info) {
		FI_WARN(&mrail_prov, FI_LOG_FABRIC,
			"Some rails cannot be used!\n");
		return -FI_ENODATA;
	}

	mrail_info = calloc(num_mrail_info, sizeof(*mrail_info));
	if (!mrail_info)
		return -FI_ENOMEM;

	tails = calloc(num_mrail_info, sizeof(*tails));
	if (!tails) {
		free(mrail_info);
		return -FI_ENOMEM;
	}

	/*
	 * Do the gathering -- Take one item from each rail and form a list.
	 * Repeat until some rails run out of entries. Extra emtries are dumped.
	 */
	for (i = 0; i < num_rails; i++) {
		p = rail_info[i];
		for (j = 0; j < num_mrail_info; j++) {
			if (!tails[j])
				mrail_info[j] = p;
			else
				tails[j]->next = p;
			tails[j] = p;
			p = p->next;
			tails[j]->next = NULL;
		}
		if (p)
			fi_freeinfo(p);
	}

	free(tails);
	*info_out = mrail_info;
	*num_info_out = num_mrail_info;
	return 0;
}

static int mrail_get_core_info(uint32_t version, const char *node, const char *service,
			       uint64_t flags, const struct fi_info *hints,
			       struct fi_info ***core_info_array, int *num_core_info)
{
	struct fi_info *core_hints;
	struct fi_info **rail_info = NULL;
	size_t i;
	int ret = 0;
	int num_rails;
	enum fi_log_level level = ((hints && hints->fabric_attr &&
				    hints->fabric_attr->prov_name) ?
				   FI_LOG_WARN : FI_LOG_INFO);

	if (!mrail_addr_strv) {
		FI_LOG(&mrail_prov, level, FI_LOG_FABRIC,
		       "OFI_MRAIL_ADDR_STRC env variable not set!\n");
		return -FI_ENODATA;
	}

	for (i = 0, num_rails = 0; mrail_addr_strv[i]; i++)
		num_rails++;

	if (!num_rails) {
		FI_WARN(&mrail_prov, FI_LOG_FABRIC,
			"OFI_MRAIL_ADDR_STRC env variable is set but empty!\n");
		return -FI_ENODATA;
	}

	core_hints = mrail_create_core_hints(hints);
	if (!core_hints)
		return -FI_ENOMEM;

	ret = ofi_exclude_prov_name(&core_hints->fabric_attr->prov_name,
				    mrail_prov.name);
	if (ret)
		goto out;

	rail_info = calloc(num_rails, sizeof(*rail_info));
	if (!rail_info) {
		ret = -FI_ENOMEM;
		goto out;
	}

	for (i = 0; i < num_rails; i++) {
		free(core_hints->src_addr);
		ret = ofi_str_toaddr(mrail_addr_strv[i],
				     &core_hints->addr_format,
				     &core_hints->src_addr,
				     &core_hints->src_addrlen);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_FABRIC,
				"Unable to convert FI_ADDR_STR to device "
				"specific address\n");
			goto err;
		}

		FI_DBG(&mrail_prov, FI_LOG_CORE,
		       "--- Begin fi_getinfo for rail: %zd ---\n", i);

		if (!hints || !hints->caps) {
			struct fi_info *tmp_info = NULL;
			uint64_t saved_core_hints_caps = core_hints->caps;
			/*
			 * Get the default caps that would be returned for empty
			 * hints, otherwise the returned caps would only contain
			 * those specifed in the hints (FI_SOURCE) and secondary
			 * capabilities.
			 */
			core_hints->caps = 0;
			ret = fi_getinfo(version, NULL, NULL,
					 OFI_GETINFO_INTERNAL, core_hints,
					 &tmp_info);
			if (tmp_info) {
				core_hints->caps = tmp_info->caps |
						   saved_core_hints_caps;
				fi_freeinfo(tmp_info);
			} else {
				core_hints->caps = saved_core_hints_caps;
			}

			ret = fi_getinfo(version, NULL, NULL,
					 OFI_GETINFO_INTERNAL, core_hints,
					 &rail_info[i]);
			core_hints->caps = saved_core_hints_caps;
		} else {
			ret = fi_getinfo(version, NULL, NULL,
					 OFI_GETINFO_INTERNAL, core_hints,
					 &rail_info[i]);
		}

		FI_DBG(&mrail_prov, FI_LOG_CORE,
		       "--- End fi_getinfo for rail: %zd ---\n", i);
		if (ret)
			goto err;
	}

	ret = mrail_gather_rail_info(rail_info, num_rails, core_info_array, num_core_info);
	goto out;

err:
	for (i = 0; i < num_rails; i++)
		fi_freeinfo(rail_info[i]);
out:
	free(rail_info);
	fi_freeinfo(core_hints);
	return ret;
}

static void mrail_adjust_info(struct fi_info *info, const struct fi_info *hints)
{
	info->mode &= ~FI_BUFFERED_RECV;

	if (!hints)
		return;

	if (hints->domain_attr) {
		if (hints->domain_attr->av_type)
			info->domain_attr->av_type = hints->domain_attr->av_type;
	}

	if (hints->tx_attr) {
		if (hints->tx_attr->op_flags & FI_COMPLETION)
			info->tx_attr->op_flags |= FI_COMPLETION;
	}
}

static struct fi_info *mrail_get_prefix_info(struct fi_info *core_info, int id)
{
	struct fi_info *fi;
	uint32_t num_rails;
	char *s;

	for (fi = core_info, num_rails = 0; fi; fi = fi->next, ++num_rails)
		;

	fi = fi_dupinfo(core_info);
	if (!fi)
		return NULL;

	free(fi->fabric_attr->name);
	free(fi->domain_attr->name);

	fi->fabric_attr->name = NULL;
	fi->domain_attr->name = NULL;

	if (asprintf(&s, "%s_%d", mrail_info.fabric_attr->name, id) < 0)
		goto err;
	fi->fabric_attr->name = s;

	fi->domain_attr->name = strdup(mrail_info.domain_attr->name);
	if (!fi->domain_attr->name)
		goto err;

	fi->ep_attr->protocol		= mrail_info.ep_attr->protocol;
	fi->ep_attr->protocol_version	= mrail_info.ep_attr->protocol_version;
	fi->fabric_attr->prov_version	= OFI_VERSION_DEF_PROV;
	fi->domain_attr->mr_key_size	= (num_rails *
					   sizeof(struct mrail_addr_key));
	fi->domain_attr->mr_mode	|= FI_MR_RAW;

	/* Account for one iovec buffer used for mrail header */
	assert(fi->tx_attr->iov_limit);
	fi->tx_attr->iov_limit--;

	/* Claiming messages larger than FI_OPT_BUFFERED_LIMIT would consume
	 * a scatter/gather entry for mrail_hdr */
	fi->rx_attr->iov_limit--;

	if (fi->tx_attr->inject_size < sizeof(struct mrail_hdr))
		fi->tx_attr->inject_size = 0;
	else
		fi->tx_attr->inject_size -= sizeof(struct mrail_hdr);
	return fi;
err:
	fi_freeinfo(fi);
	return NULL;
}

static int mrail_getinfo(uint32_t version, const char *node, const char *service,
			 uint64_t flags, const struct fi_info *hints,
			 struct fi_info **info)
{
	struct fi_info *fi, *tail;
	struct fi_info **core_info;
	int num_core_info = 0;
	int ret;
	int i;

	if (mrail_num_info >= MRAIL_MAX_INFO) {
		FI_WARN(&mrail_prov, FI_LOG_CORE,
			"Max mrail_num_info reached\n");
		assert(0);
		return -FI_ENODATA;
	}

	ret = mrail_get_core_info(version, node, service, flags, hints, &core_info, &num_core_info);
	if (ret)
		return ret;

	*info = tail = NULL;
	for (i = 0; i < num_core_info; i++) {
		fi = mrail_get_prefix_info(core_info[i], mrail_num_info);
		if (!fi) {
			ret = -FI_ENOMEM;
			goto err;
		}

		mrail_adjust_info(fi, hints);

		if (!tail)
			*info = fi;
		else
			tail->next = fi;
		tail = fi;

		/* save a copy of the mrail info header */
		mrail_info_vec[mrail_num_info] = fi_dupinfo(fi);
		if (!mrail_info_vec[mrail_num_info]) {
			ret = -FI_ENOMEM;
			goto err;
		}

		/* link the saved header to per-rail information */
		mrail_info_vec[mrail_num_info]->next = core_info[i];
		core_info[i] = NULL;

		mrail_num_info++;
		if (mrail_num_info >= MRAIL_MAX_INFO) {
			FI_WARN(&mrail_prov, FI_LOG_CORE,
				"Max mrail_num_info reached, some info may be dropped\n");
			break;
		}
	}
out:
	for (i = 0; i < num_core_info; i++)
		fi_freeinfo(core_info[i]);
	free(core_info);
	return ret;

err:
	fi_freeinfo(*info);
	goto out;
}

static void mrail_fini(void)
{
	size_t i;
	for (i = 0; i < mrail_num_info; i++)
		fi_freeinfo(mrail_info_vec[i]);
}

struct fi_provider mrail_prov = {
	.name = OFI_UTIL_PREFIX "mrail",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = mrail_getinfo,
	.fabric = mrail_fabric_open,
	.cleanup = mrail_fini
};

struct util_prov mrail_util_prov = {
	.prov = &mrail_prov,
	.info = &mrail_info,
	.flags = 0,
};

MRAIL_INI
{
	mrail_parse_env_vars();
	return &mrail_prov;
}

