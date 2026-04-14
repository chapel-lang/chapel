/*
 * Copyright (c) 2022 ORNL. All rights reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "rdma/fi_ext.h"
#include "lnx.h"

#define LNX_PASSTHRU_TX_OP_FLAGS	(FI_INJECT_COMPLETE | \
					 FI_TRANSMIT_COMPLETE | \
					 FI_DELIVERY_COMPLETE)
#define LNX_PASSTHRU_RX_OP_FLAGS	(0ULL)
#define LNX_TX_OP_FLAGS		(FI_INJECT_COMPLETE | FI_COMPLETION | \
				 FI_DELIVERY_COMPLETE | FI_TRANSMIT_COMPLETE)
#define LNX_RX_OP_FLAGS		(FI_COMPLETION)

ofi_spin_t global_bplock;
struct ofi_bufpool *global_recv_bp = NULL;

struct util_fabric lnx_fabric_info;

struct fi_tx_attr lnx_tx_attr = {
	.caps 		= ~0x0ULL,
	.op_flags	= LNX_PASSTHRU_TX_OP_FLAGS | LNX_TX_OP_FLAGS,
	.msg_order 	= ~0x0ULL,
	.comp_order 	= 0,
	.inject_size 	= SIZE_MAX,
	.size 		= SIZE_MAX,
	.iov_limit 	= LNX_IOV_LIMIT,
	.rma_iov_limit = LNX_IOV_LIMIT,
};

struct fi_rx_attr lnx_rx_attr = {
	.caps 			= ~0x0ULL,
	.op_flags		= LNX_PASSTHRU_RX_OP_FLAGS | LNX_RX_OP_FLAGS,
	.msg_order 		= ~0x0ULL,
	.comp_order 		= 0,
	.total_buffered_recv 	= 0,
	.size 			= 1024,
	.iov_limit		= LNX_IOV_LIMIT,
};

struct fi_ep_attr lnx_ep_attr = {
	.type 			= FI_EP_UNSPEC,
	.protocol 		= FI_PROTO_LNX,
	.protocol_version 	= 1,
	.max_msg_size 		= SIZE_MAX,
	.msg_prefix_size	= SIZE_MAX,
	.max_order_raw_size 	= SIZE_MAX,
	.max_order_war_size 	= SIZE_MAX,
	.max_order_waw_size 	= SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
	.tx_ctx_cnt 		= SIZE_MAX,
	.rx_ctx_cnt 		= SIZE_MAX,
	.auth_key		= NULL,
	.auth_key_size		= 0,
};

struct fi_domain_attr lnx_domain_attr = {
	.name			= "lnx",
	.threading 		= FI_THREAD_SAFE,
	.control_progress 	= FI_PROGRESS_AUTO,
	.data_progress 		= FI_PROGRESS_AUTO,
	.resource_mgmt 		= FI_RM_ENABLED,
	.av_type 		= FI_AV_TABLE,
	.mr_mode 		= FI_MR_RAW,
	.mr_key_size		= SIZE_MAX,
	.cq_data_size 		= SIZE_MAX,
	.cq_cnt 		= SIZE_MAX,
	.ep_cnt 		= SIZE_MAX,
	.tx_ctx_cnt 		= SIZE_MAX,
	.rx_ctx_cnt 		= SIZE_MAX,
	.max_ep_tx_ctx 		= SIZE_MAX,
	.max_ep_rx_ctx 		= SIZE_MAX,
	.max_ep_stx_ctx 	= SIZE_MAX,
	.max_ep_srx_ctx 	= SIZE_MAX,
	.cntr_cnt 		= SIZE_MAX,
	.mr_iov_limit 		= SIZE_MAX,
	.caps			= ~0x0ULL,
	.auth_key_size		= SIZE_MAX,
	.max_err_data		= SIZE_MAX,
	.mr_cnt			= SIZE_MAX,
};

struct fi_fabric_attr lnx_fabric_attr = {
	.prov_version = OFI_VERSION_DEF_PROV,
	.name = "ofi_lnx_fabric",
};

struct fi_info lnx_info = {
	.caps = ~0x0ULL,
	.tx_attr = &lnx_tx_attr,
	.rx_attr = &lnx_rx_attr,
	.ep_attr = &lnx_ep_attr,
	.domain_attr = &lnx_domain_attr,
	.fabric_attr = &lnx_fabric_attr
};

static struct fi_ops lnx_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = lnx_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_fabric lnx_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = lnx_domain_open,
	.passive_ep = fi_no_passive_ep,
	/* TODO: Should the eq be supported in the FI_PEER infra? Similar
	 * to how the CQ is supported.
	 */
	.eq_open = ofi_eq_create,
	.wait_open = fi_no_wait_open,
	.trywait = fi_no_trywait
};

struct fi_provider lnx_prov = {
	.name = OFI_LNX,
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = lnx_getinfo,
	.fabric = lnx_fabric,
	.cleanup = lnx_fini
};

struct util_prov lnx_util_prov = {
	.prov = &lnx_prov,
	.info = &lnx_info,
	.flags = 0
};

/* this is a list of all possible links */
struct dlist_entry lnx_links;

struct lnx_fi_cache_entry {
	struct dlist_entry entry;
	struct fi_info *fi;
};

struct lnx_link_info {
	struct dlist_entry entry;
	struct dlist_entry link_providers;
	struct fi_info *fi_link;
};

static void lnx_trim(char *str)
{
	int i, start, end, len;

	if (!str)
		return;

	len = strlen(str);

	for (start = 0; start < len && isspace(str[start]); ++start)
		;
	for (end = len - 1; end >= 0 && isspace(str[end]); --end)
		;

	for (i = 0; i <= (end - start); i++)
		str[i] = str[i + start];

	str[i] = '\0';
}


void lnx_free_links(struct dlist_entry *links)
{
	struct lnx_link_info *link;
	struct lnx_fi_cache_entry *e;
	struct dlist_entry *tmp, *tmp2;

	dlist_foreach_container_safe(links, struct lnx_link_info, link,
				     entry, tmp) {
		dlist_foreach_container_safe(&link->link_providers,
					     struct lnx_fi_cache_entry, e,
					     entry, tmp2) {
			fi_freeinfo(e->fi);
			dlist_remove(&e->entry);
			free(e);
		}
		dlist_remove(&link->entry);
		free(link);
	}
}

static int lnx_cache_info(struct dlist_entry *head,
			  struct fi_info *info, bool new_prov)
{
	struct lnx_fi_cache_entry *e;
	struct fi_info *itr;
	bool found = false;

	if (new_prov)
		goto insert;

	/* find if the provider is already cached */
	dlist_foreach_container(head, struct lnx_fi_cache_entry, e, entry) {
		if (!strcmp(e->fi->fabric_attr->prov_name,
			    info->fabric_attr->prov_name)) {
			for (itr = e->fi; itr; itr = itr->next) {
				if (itr->next == NULL) {
					itr->next = info;
					found = true;
					break;
				}
			}
		}
		if (found)
			return 0;
	}

insert:
	e = calloc(1, sizeof(*e));

	if (!e)
		return -FI_ENOMEM;
	dlist_init(&e->entry);
	e->fi = info;

	dlist_insert_tail(&e->entry, head);

	return 0;
}

static struct lnx_link_info *
lnx_get_link_by_dom(char *domain_name)
{
	struct lnx_link_info *link;

	dlist_foreach_container(&lnx_links, struct lnx_link_info, link,
				entry) {
		if (!strcmp(domain_name, link->fi_link->domain_attr->name))
			return link;
	}

	return NULL;
}

static int lnx_generate_link_info(struct fi_info **info, const struct fi_info *hints)
{
	struct fi_info *itr, *fi = NULL, *next;
	struct lnx_link_info *link;
	struct lnx_fi_cache_entry *e;
	size_t min_inject_size = SIZE_MAX, min_of_max_msg_size = SIZE_MAX;
	size_t min_rx_size = SIZE_MAX, min_tx_size = SIZE_MAX;
	size_t min_iov_limit = LNX_IOV_LIMIT;
	int mr_mode = 0;
	int len;
	int str_len = 1024;
	char *tmp, *tmp2;
	char *link_name;
	char *prov_name;
	bool reset;
	uint64_t caps = 0;

	dlist_foreach_container(&lnx_links, struct lnx_link_info, link,
				entry) {
		link_name = calloc(sizeof(char), str_len);
		if (!link_name)
			return -FI_ENOMEM;

		prov_name = calloc(sizeof(char), 4);
		if (!prov_name) {
			free(link_name);
			return -FI_ENOMEM;
		}
		strcpy(prov_name, "lnx");
		tmp = link_name;
		tmp2 = link_name + str_len - 1;

		reset = true;

		dlist_foreach_container(&link->link_providers,
					struct lnx_fi_cache_entry, e, entry) {
			if (!link->fi_link ||
			    (!strcmp(link->fi_link->fabric_attr->prov_name,
				     "shm"))) {
				if (link->fi_link)
					fi_freeinfo(link->fi_link);
				link->fi_link = fi_dupinfo(e->fi);
				link->fi_link->next = NULL;
			}

			mr_mode |= e->fi->domain_attr->mr_mode;

			if (reset) {
				caps = e->fi->caps;
				min_inject_size = min_of_max_msg_size = SIZE_MAX;
				min_rx_size = min_tx_size = SIZE_MAX;
				min_iov_limit = LNX_IOV_LIMIT;
				reset = false;
			} else {
				caps &= e->fi->caps;
			}
			if (e->fi->ep_attr->max_msg_size < min_of_max_msg_size)
				min_of_max_msg_size = e->fi->ep_attr->max_msg_size;
			if (e->fi->tx_attr->inject_size < min_inject_size)
				min_inject_size = e->fi->tx_attr->inject_size;
			if (e->fi->tx_attr->iov_limit < min_iov_limit)
				min_iov_limit = e->fi->tx_attr->iov_limit;
			if (e->fi->rx_attr->size < min_rx_size)
				min_rx_size = e->fi->rx_attr->size;
			if (e->fi->tx_attr->size < min_tx_size)
				min_tx_size = e->fi->tx_attr->size;

			for (itr = e->fi; itr; itr = itr->next) {
				len = strlen(itr->domain_attr->name) + 1;
				if (len < (tmp2 - tmp) - 1) {
					if (tmp != link_name) {
						tmp[0] = '+';
						tmp++;
					}
					memcpy(tmp, itr->domain_attr->name, len);
					tmp += (len - 1);
				} else {
					str_len = str_len * 2 + len;
					tmp = realloc(link_name, str_len);
					if (!tmp) {
						free(link_name);
						return -FI_ENOMEM;
					}
					link_name = tmp;
					tmp += strlen(link_name);
					tmp2 = link_name + str_len - 1;
					tmp[0] = '+';
					tmp++;
					memcpy(tmp, itr->domain_attr->name, len);
					tmp += (len - 1);
				}
			}
		}

		link_name = realloc(link_name, strlen(link_name)+1);
		free(link->fi_link->fabric_attr->prov_name);
		free(link->fi_link->fabric_attr->name);
		free(link->fi_link->domain_attr->name);
		link->fi_link->fabric_attr->prov_name = prov_name;
		link->fi_link->fabric_attr->name = strdup(prov_name);
		link->fi_link->domain_attr->name = link_name;

		/* there should only be one fi per link that gets
		 * propagated up to the caller. This represents the LNX
		 * link
		 */
		next = fi_dupinfo(link->fi_link);
		next->ep_attr->protocol = FI_PROTO_LNX;
		next->domain_attr->mr_mode = mr_mode;
		next->domain_attr->rx_ctx_cnt = lnx_util_prov.info->domain_attr->rx_ctx_cnt;
		next->domain_attr->tx_ctx_cnt = lnx_util_prov.info->domain_attr->tx_ctx_cnt;
		next->ep_attr->rx_ctx_cnt = lnx_util_prov.info->ep_attr->rx_ctx_cnt;
		next->ep_attr->tx_ctx_cnt = lnx_util_prov.info->ep_attr->tx_ctx_cnt;
		if (hints)
			next->caps = caps & hints->caps;
		else
			next->caps = caps;
		next->tx_attr->inject_size = min_inject_size;
		next->tx_attr->iov_limit = min_iov_limit;
		next->ep_attr->max_msg_size = min_of_max_msg_size;
		next->rx_attr->size = min_rx_size;
		next->tx_attr->size = min_tx_size;

		if (!next)
			return -FI_ENOMEM;
		if (fi) {
			itr = fi;
			while (itr->next)
				itr = itr->next;
			itr->next = next;
		} else {
			fi = next;
		}
	}

	*info = fi;

	return FI_SUCCESS;
}

static struct fi_info *
lnx_filter_info(struct fi_info *fi, char *domain)
{
	struct fi_info *itr, *tmp, *filtered_head = NULL, *filtered_tail = NULL;

	for (itr = fi; itr; itr = itr->next) {
		if (!strcmp(itr->domain_attr->name, "shm") ||
		    (domain && !strcmp(itr->domain_attr->name, domain))) {
			tmp = fi_dupinfo(itr);
			if (!tmp && filtered_head) {
				fi_freeinfo(filtered_head);
				return NULL;
			}
			if (!filtered_head) {
				filtered_head = tmp;
				filtered_tail = filtered_head;
			} else {
				filtered_tail->next = tmp;
				filtered_tail = tmp;
			}
			fi_freeinfo(fi);
			/* TODO: only pick the first entry which matches
			 * this domain. Is there a better way of doing
			 * this? Should the caller be setting further
			 * capabilities
			 */
			break;
		}
	}

	/* We're always going to get a filtered list if we have a shm or
	 * a domain specified. Otherwise return the original fi since
	 * we're going to use all of it
	 */
	return (filtered_head) ? filtered_head : fi;
}

int lnx_getinfo_helper(uint32_t version, char *prov, char *domain,
		       struct fi_info *lnx_hints, bool new_prov,
		       struct dlist_entry *link)
{
	int rc;
	char *orig_prov_name = NULL, *orig_dom_name = NULL;
	struct fi_info *core_info;
	uint64_t caps;
	bool shm = false;

	caps = lnx_hints->caps;

	orig_prov_name = lnx_hints->fabric_attr->prov_name;
	orig_dom_name = lnx_hints->domain_attr->name;

	lnx_hints->fabric_attr->prov_name = prov;
	lnx_hints->domain_attr->name = domain;

	if (!strncmp(prov, "shm", 3)) {
		shm = true;
		lnx_hints->caps &= ~(FI_REMOTE_COMM);
		lnx_hints->domain_attr->caps &= ~(FI_REMOTE_COMM);
	}

	rc = fi_getinfo(version, NULL, NULL, OFI_GETINFO_HIDDEN,
			lnx_hints, &core_info);

	lnx_hints->fabric_attr->prov_name = orig_prov_name;
	lnx_hints->domain_attr->name = orig_dom_name;
	if (rc)
		return rc;

	if (shm)
		lnx_hints->caps = caps;

	core_info = lnx_filter_info(core_info, domain);

	rc = lnx_cache_info(link, core_info, new_prov);

	return rc;
}

int lnx_getinfo(uint32_t version, const char *node, const char *service,
		uint64_t flags, const struct fi_info *hints,
		struct fi_info **info)
{
	int rc;
	bool new_prov;
	struct fi_info *lnx_hints = NULL;
	struct lnx_link_info *link;
	char *linked_provs, *linked_provs_cp;
	char *save_ptr0, *save_ptr1, *save_ptr2, *provider_block,
	     *provider, *domain, *link_block;

	/* If fi_getinfo() has been called already use the links which
	 * were generated. We do not support generating new links within
	 * the same context
	 */
	if (!dlist_empty(&lnx_links))
		goto generate_info;

	rc = fi_param_get_str(&lnx_prov, "prov_links",
			      &linked_provs);
	if (rc)
		return rc;

	if (strstr(linked_provs, "lnx")) {
		FI_WARN(&lnx_prov, FI_LOG_FABRIC,
			"Can't specify the lnx provider as part of the link: %s\n",
			linked_provs);
		return -FI_EINVAL;
	}

	linked_provs_cp = strdup(linked_provs);
	if (!linked_provs_cp)
		return -FI_ENOMEM;

	/* If the hints are not provided then we endup with a new block */
	lnx_hints = fi_dupinfo(hints);
	if (!lnx_hints)
		return -FI_ENOMEM;

	rc = ofi_exclude_prov_name(&lnx_hints->fabric_attr->prov_name, lnx_prov.name);
	if (rc)
		return rc;

	/* get the providers which support peer functionality. These are
	 * the only ones we can link*/
	lnx_hints->caps |= FI_PEER;

	/* Format:
	 *  '+' appends another provider to the link.
	 *  ':' identifies the start of the domains to include. If not
	 *      there all domains will be part of that link
	 *  '|' link separator to allow for the identification of multiple
	 *      links
	 *	ex1: shm+cxi:cxi0,cxi1+tcp:tcp1|shm+cxi:cxi2,cxi3
	 *	ex2: shm+cxi:cxi0|shm+cxi:cxi1|shm+cxi:cxi2|shm+cxi:cxi3
	 *
	 *	LNX fi_infos returned to the application would be:
	 *	  ex1:
	 *	    shm+cxi0,cxi1+tcp1
	 *	    shm+cxi2,cxi3
	 *	  ex2:
	 *	    shm+cxi0
	 *	    shm+cxi1
	 *	    shm+cxi2
	 *	    shm+cxi3
	 * A link can have one provider with one or more
	 * domains
	 *	ex: cxi:cxi0,cxi1
	 */
	link_block = strtok_r(linked_provs_cp, "|", &save_ptr0);
	while (link_block) {
		lnx_trim(link_block);

		link = calloc(sizeof(*link), 1);
		if (!link)
			return -FI_ENOMEM;
		dlist_init(&link->entry);
		dlist_init(&link->link_providers);
		provider_block = strtok_r(link_block, "+", &save_ptr1);
		while (provider_block) {
			lnx_trim(provider_block);

			new_prov = true;
			lnx_hints->fabric_attr->prov_name[0] = '\0';
			provider = strtok_r(provider_block, ":", &save_ptr2);
			lnx_trim(provider);
			/* handle the case where a domain is not specified. Then
			* we use all the domains
			*/
			domain = strtok_r(NULL, ",", &save_ptr2);
			do {
				lnx_trim(domain);
				rc = lnx_getinfo_helper(version, provider, domain,
						lnx_hints, new_prov,
						&link->link_providers);
				if (rc)
					return rc;
				new_prov = false;
			} while ((domain = strtok_r(NULL, ",", &save_ptr2)));
			provider_block = strtok_r(NULL, "+", &save_ptr1);
		}
		dlist_insert_tail(&link->entry, &lnx_links);
		link_block = strtok_r(NULL, "|", &save_ptr0);
	}
	free(linked_provs_cp);

generate_info:
	rc = lnx_generate_link_info(info, hints);

	if (lnx_hints)
		fi_freeinfo(lnx_hints);
	return rc;
}

static int
lnx_setup_core_fab(struct lnx_fabric *lnx_fab, struct fi_info *info,
		   void *context, int i)
{
	int rc = -FI_ENOMEM;
	struct lnx_core_fabric *cf;

	cf = &lnx_fab->lf_core_fabrics[i];

	cf->cf_info = info;

	rc = fi_fabric(info->fabric_attr, &cf->cf_fabric, context);
	if (rc)
		goto fail;

	return 0;

fail:
	free(cf);
	return rc;
}

int
lnx_setup_fabrics(char *name, struct lnx_fabric *lnx_fab,
		  void *context)
{
	int rc;
	struct lnx_link_info *link;
	struct lnx_fi_cache_entry *e;
	int i = 0;

	if (lnx_fab->lf_fab_setup_complete)
		goto out;

	link = lnx_get_link_by_dom(name);
	if (!link)
		return -FI_ENODATA;

	dlist_foreach_container(&link->link_providers,
				struct lnx_fi_cache_entry, e, entry)
		lnx_fab->lf_num_fabs++;

	lnx_fab->lf_core_fabrics = calloc(sizeof(*lnx_fab->lf_core_fabrics),
					  lnx_fab->lf_num_fabs);
	if (!lnx_fab->lf_core_fabrics)
		return -FI_ENOMEM;

	dlist_foreach_container(&link->link_providers,
				struct lnx_fi_cache_entry, e, entry) {
		rc = lnx_setup_core_fab(lnx_fab, e->fi, context, i);
		if (rc)
			return rc;
		i++;
	}

	lnx_fab->lf_fab_setup_complete = true;

out:
	return FI_SUCCESS;
}

int lnx_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context)
{
	struct lnx_fabric *lnx_fab;
	int rc;

	lnx_fab = calloc(sizeof(*lnx_fab), 1);
	if (!lnx_fab)
		return -FI_ENOMEM;

	rc = ofi_fabric_init(&lnx_prov, lnx_info.fabric_attr,
			     lnx_info.fabric_attr,
			     &lnx_fab->lf_util_fabric, context);
	if (rc)
		goto fail;

	lnx_fab->lf_util_fabric.fabric_fid.fid.ops = &lnx_fabric_fi_ops;
	lnx_fab->lf_util_fabric.fabric_fid.ops = &lnx_fabric_ops;
	*fabric = &lnx_fab->lf_util_fabric.fabric_fid;

	return 0;

fail:
	return rc;
}

void lnx_fini(void)
{
	lnx_free_links(&lnx_links);
	ofi_bufpool_destroy(global_recv_bp);
}

int lnx_fabric_close(struct fid *fid)
{
	int rc, i, frc = 0;
	struct lnx_core_fabric *cf;
	struct lnx_fabric *lnx_fab;

	lnx_fab = container_of(fid, struct lnx_fabric, lf_util_fabric.fabric_fid.fid);

	for (i = 0; i < lnx_fab->lf_num_fabs; i++) {
		cf = &lnx_fab->lf_core_fabrics[i];

		rc = fi_close(&cf->cf_fabric->fid);
		if (rc)
			frc = rc;
	}

	rc = ofi_fabric_close(&lnx_fab->lf_util_fabric);
	if (rc)
		frc = rc;

	free(lnx_fab->lf_core_fabrics);

	return frc;
}

void ofi_link_fini(void)
{
	lnx_prov.cleanup();
}

LNX_INI
{
	struct ofi_bufpool_attr bp_attrs = {};
	int ret;

	fi_param_define(&lnx_prov, "prov_links", FI_PARAM_STRING,
			"Specify which providers LNX will link together. Format: "
			"<prov 1>+<prov 2>+...+<prov N>. EX: shm+cxi");

	fi_param_define(&lnx_prov, "disable_shm", FI_PARAM_BOOL,
			"Turn off SHM support. Defaults to 0");

	fi_param_define(&lnx_prov, "dump_stats", FI_PARAM_BOOL,
			"Dump LNX stats on shutdown. Defaults to 0");

	dlist_init(&lnx_links);

	if (!global_recv_bp) {
		bp_attrs.size = sizeof(struct lnx_rx_entry);
		bp_attrs.alignment = 8;
		bp_attrs.max_cnt = UINT16_MAX;
		bp_attrs.chunk_cnt = 64;
		bp_attrs.flags = OFI_BUFPOOL_NO_TRACK;
		ret = ofi_bufpool_create_attr(&bp_attrs, &global_recv_bp);
		if (ret) {
			FI_WARN(&lnx_prov, FI_LOG_FABRIC,
				"Failed to create receive buffer pool");
			return NULL;
		}
		ofi_spin_init(&global_bplock);
	}

	return &lnx_prov;
}
