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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "rdma/fi_ext.h"
#include "lnx.h"

struct lnx_peer *
lnx_av_lookup_addr(struct lnx_av *av, fi_addr_t addr)
{
	struct lnx_peer *lp, **lpp;

	if (addr == FI_ADDR_UNSPEC)
		return NULL;

	ofi_genlock_lock(&av->lav_av.lock);

	lpp = ofi_av_get_addr(&av->lav_av, addr);
	lp = *lpp;

	ofi_genlock_unlock(&av->lav_av.lock);

	if (!lp)
		FI_WARN(&lnx_prov, FI_LOG_CORE,
			"Invalid fi_addr %#lx\n", addr);

	return lp;
}

int lnx_av_close(struct fid *fid)
{
	int i, rc, frc = 0;
	struct lnx_core_av *core_av;
	struct lnx_av *lav;

	lav = container_of(fid, struct lnx_av, lav_av.av_fid.fid);

	for (i = 0; i < lav->lav_domain->ld_num_doms; i++) {
		core_av = &lav->lav_core_avs[i];
		rc = fi_close(&core_av->cav_av->fid);
		if (rc)
			frc = rc;
	}

	ofi_av_close(&lav->lav_av);
	free(lav->lav_core_avs);
	free(lav);

	return frc;
}

static struct fi_ops lnx_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = lnx_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static void
lnx_update_msg_entries(struct lnx_qpair *qp,
		       fi_addr_t (*get_addr)(struct fi_peer_rx_entry *))
{
	struct lnx_queue *q = &qp->lqp_unexq;
	struct lnx_rx_entry *rx_entry;
	struct dlist_entry *item;

	dlist_foreach(&q->lq_queue, item) {
		rx_entry = (struct lnx_rx_entry *) item;
		if (rx_entry->rx_entry.addr == FI_ADDR_UNSPEC)
			rx_entry->rx_entry.addr = get_addr(&rx_entry->rx_entry);
	}
}

void
lnx_foreach_unspec_addr(struct fid_peer_srx *srx,
			fi_addr_t (*get_addr)(struct fi_peer_rx_entry *))
{
	struct lnx_ep *lep;
	struct lnx_core_ep *cep;

	cep = (struct lnx_core_ep *) srx->ep_fid.fid.context;
	lep = cep->cep_parent;

	lnx_update_msg_entries(&lep->le_srq.lps_trecv, get_addr);
	lnx_update_msg_entries(&lep->le_srq.lps_recv, get_addr);
}

static int lnx_peer_av_remove(struct lnx_peer *lp)
{
	int i, j, rc, frc = 0;
	struct lnx_core_av *cav;
	struct lnx_peer_map *map_addr;

	for (i = 0; i < lp->lp_av_count; i++) {
		cav = lp->lp_avs[i];
		if (!cav)
			break;
		map_addr = ofi_bufpool_get_ibuf(cav->cav_map, lp->lp_addr);
		for (j = 0; j < map_addr->map_count; j++) {
			rc = fi_av_remove(cav->cav_av, &map_addr->map_addrs[j], 1, 0);
			if (rc)
				frc = rc;
		}
	}

	return frc;
}

static void
lnx_free_src_eps(struct lnx_peer_ep_map *src_eps, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		if (src_eps[i].pem_eps)
			free(src_eps[i].pem_eps);
	}
	free(src_eps);
}

static int lnx_peer_remove(struct lnx_av *lav, fi_addr_t addr)
{
	struct lnx_peer *lp, **lpp;
	int rc = 0;

	ofi_genlock_lock(&lav->lav_av.lock);
	lpp = ofi_av_get_addr(&lav->lav_av, addr);
	if (!lpp)
		goto out;

	lp = *lpp;

	rc = lnx_peer_av_remove(lp);

	rc = ofi_av_remove_addr(&lav->lav_av, addr);

	lnx_free_src_eps(lp->lp_src_eps, lp->lp_ep_count);
	free(lp->lp_avs);
	free(lp);

out:
	ofi_genlock_unlock(&lav->lav_av.lock);
	return rc;
}

int lnx_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
		  uint64_t flags)
{
	struct lnx_av *lav;
	int frc = 0, rc, i;

	lav = container_of(av, struct lnx_av, lav_av.av_fid.fid);

	for (i = 0; i < count; i++) {
		rc = lnx_peer_remove(lav, fi_addr[i]);
		if (rc)
			frc = rc;
	}

	return frc;
}

static int
lnx_setup_ep_mapping(struct lnx_peer *lp,
		    struct lnx_core_ep *ceps[][LNX_MAX_LOCAL_EPS])
{
	int i = 0, j = 0;
	size_t size;
	int counts[LNX_MAX_LOCAL_EPS] = {0};

	for (i = 0; i < LNX_MAX_LOCAL_EPS; i++)  {
		if (ceps[i][0] == NULL)
			break;
		for (j = 0; j < LNX_MAX_LOCAL_EPS; j++) {
			if (ceps[i][j] == NULL)
				break;
			counts[i]++;
		}
		lp->lp_ep_count++;
	}

	size = sizeof(struct lnx_peer_ep_map) * i;

	lp->lp_src_eps = calloc(size, 1);
	if (!lp->lp_src_eps)
		return -FI_ENOMEM;

	for (j = 0; j < i; j++) {
		lp->lp_src_eps[j].pem_eps = calloc(sizeof(struct lnx_core_ep *)*counts[j], 1);
		if (!lp->lp_src_eps[j].pem_eps) {
			lnx_free_src_eps(lp->lp_src_eps, lp->lp_ep_count);
			return -FI_ENOMEM;
		}
		lp->lp_src_eps[j].pem_num_eps = counts[j];
	}

	for (i = 0; i < LNX_MAX_LOCAL_EPS; i++)  {
		if (ceps[i][0] == NULL)
			break;
		for (j = 0; j < LNX_MAX_LOCAL_EPS; j++) {
			if (ceps[i][j] == NULL)
				break;
			lp->lp_src_eps[i].pem_eps[j] = ceps[i][j];
		}
	}

	return FI_SUCCESS;
}

static int
lnx_insert_addr(struct lnx_core_av *core_av, struct lnx_ep_addr *addr,
		struct lnx_peer *lp,
		struct lnx_core_ep *cep_tmp[][LNX_MAX_LOCAL_EPS], bool local)
{
	int i, rc;
	bool present;
	char *prov_name;
	void *core_addr = (char*) addr + sizeof(*addr);
	struct lnx_core_av *cav;
	struct lnx_core_ep *cep;
	struct lnx_ep *lep;
	fi_addr_t core_fi_addr;
	struct lnx_peer_map *map_addr;

	prov_name = core_av->cav_domain->cd_info->fabric_attr->name;
	/* only insert into AVs belonging to compatible domains */
	if (strcmp(prov_name, addr->lea_prov))
		return FI_SUCCESS;
	if (!local && !strcmp(addr->lea_prov, "shm"))
		return FI_SUCCESS;

	/* cache the endpoint information */
	dlist_foreach_container(&core_av->cav_endpoints, struct lnx_core_ep, cep,
				cep_av_entry) {
		lep = cep->cep_parent;
		present = false;
		i = 0;
		while (cep_tmp[lep->le_idx][i] != NULL) {
			if (cep_tmp[lep->le_idx][i] == cep) {
				present = true;
				break;
			}
			i++;
		}
		if (!present)
			cep_tmp[lep->le_idx][i] = cep;
	}

	/* create the address maps. The reason for the lp_avs is that we
	 * can tell whether we inserted the peer on a particular map.
	 * bufpool doesn't have a mechanism to find if an index is already
	 * inserted in the pool or not
	 */
	for (i = 0; i < lp->lp_av_count; i++) {
		cav = lp->lp_avs[i];
		if (!cav)
			break;
		if (cav == core_av) {
			map_addr = ofi_bufpool_get_ibuf(core_av->cav_map, lp->lp_addr);
			goto insert;
		}
	}

	lp->lp_avs[i] = core_av;
	map_addr = ofi_ibuf_alloc_at(core_av->cav_map, lp->lp_addr);
	memset(map_addr, 0, sizeof(*map_addr));
	ofi_atomic_initialize32(&map_addr->map_rr, 0);

insert:
	core_fi_addr = lnx_encode_fi_addr(lp->lp_addr, map_addr->map_count);

	rc = fi_av_insert(core_av->cav_av, core_addr, 1, &core_fi_addr, FI_AV_USER_ID, NULL);
	if (rc <= 0)
		return rc;

	map_addr->map_addrs[map_addr->map_count++] = core_fi_addr;

	return FI_SUCCESS;
}

int lnx_av_insert(struct fid_av *av, const void *addr, size_t count,
		  fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	int i, j, k, rc;
	bool local, once;
	int disable_shm = 0;
	struct lnx_peer *lp;
	char hostname[FI_NAME_MAX];
	struct lnx_av *lav;
	struct lnx_address *la;
	struct lnx_ep_addr *lea;
	struct lnx_core_av *core_av;

	if (flags & FI_AV_USER_ID)
		return -FI_ENOSYS;

	fi_param_get_bool(&lnx_prov, "disable_shm", &disable_shm);

	lav = container_of(av, struct lnx_av, lav_av.av_fid.fid);

	rc = gethostname(hostname, FI_NAME_MAX);

	/* if address insertion fails the assumption is the caller will
	 * properly close the av table, which will do proper clean up
	 */
	la = (struct lnx_address *) addr;
	for (i = 0; i < count; i++) {
		struct lnx_core_ep *
			cep_tmp[LNX_MAX_LOCAL_EPS][LNX_MAX_LOCAL_EPS] = {0};
		once = false;
		local = false;

		lea = (struct lnx_ep_addr *) ((char*)la + sizeof(*la));
		lp = calloc(sizeof(*lp), 1);
		if (!lp)
			return -FI_ENOMEM;

		lp->lp_avs = calloc(sizeof(struct lnx_core_av *) *
				    lav->lav_domain->ld_num_doms, 1);
		lp->lp_av_count = lav->lav_domain->ld_num_doms;
		ofi_atomic_initialize32(&lp->lp_ep_rr, 0);

		/* if the shm address exists then it'll be the first
		 * entry. Only insert the shm address for local peers as
		 * we're only going to talk to the local peer over
		 * shm. The assumption is that shm will always out perform
		 * inter-node providers
		 */
		if (!strcmp(hostname, la->la_hostname) &&
		    !strcmp(lea->lea_prov, "shm") && !disable_shm)
			local = true;

		ofi_genlock_lock(&lav->lav_av.lock);
		rc = ofi_av_insert_addr(&lav->lav_av, &lp, &lp->lp_addr);
		ofi_genlock_unlock(&lav->lav_av.lock);
		if (rc) {
			free(lp->lp_avs);
			free(lp);
			return rc;
		}

		if (fi_addr)
			fi_addr[i] = lp->lp_addr;

		for (j = 0; j < la->la_ep_count; j++) {
			if (once)
				goto skip;
			for (k = 0; k < lav->lav_domain->ld_num_doms; k++) {
				core_av = &lav->lav_core_avs[k];

				rc = lnx_insert_addr(core_av, lea, lp, cep_tmp, local);
				if (rc) {
					(void) lnx_av_remove(&lav->lav_av.av_fid,
							     &lp->lp_addr, 1, 0);
					return rc;
				}
				if (local) {
					once = true;
					break;
				}
			}
skip:
			lea = (struct lnx_ep_addr *)
				((char*)lea + sizeof(*lea) + lea->lea_addr_size);
		}
		rc = lnx_setup_ep_mapping(lp, cep_tmp);
		if (rc) {
			(void) lnx_av_remove(&lav->lav_av.av_fid,
						&lp->lp_addr, 1, 0);
			return rc;
		}
		la = (struct lnx_address *) lea;
	}

	return i;
}

static const char *
lnx_av_straddr(struct fid_av *av, const void *addr,
	       char *buf, size_t *len)
{
	/* TODO: implement */
	return NULL;
}

static int
lnx_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
	      size_t *addrlen)
{
	/* TODO: implement */
	return -FI_EOPNOTSUPP;
}

static struct fi_ops_av lnx_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = lnx_av_insert,
	.remove = lnx_av_remove,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.lookup = lnx_av_lookup,
	.straddr = lnx_av_straddr,
};

int lnx_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av_out, void *context)
{
	struct fi_info *fi;
	struct lnx_domain *lnx_domain;
	struct lnx_core_domain *cd;
	struct lnx_av *av;
	struct lnx_core_av *core_av;
	struct util_av_attr util_attr = {0};
	size_t table_sz;
	int i, rc = 0;
	struct ofi_bufpool_attr pool_attr = {
		.size = sizeof(struct lnx_peer_map),
		.flags = OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_INDEXED,
		.chunk_cnt = 64,
	};

	if (!attr)
		return -FI_EINVAL;

	if (attr->name)
		return -FI_ENOSYS;

	if (attr->type != FI_AV_TABLE && attr->type != FI_AV_UNSPEC)
		return -FI_ENOSYS;

	attr->type = FI_AV_TABLE;

	lnx_domain = container_of(domain, struct lnx_domain,
				  ld_domain.domain_fid.fid);

	av = calloc(sizeof(*av), 1);
	if (!av)
		return -FI_ENOMEM;

	av->lav_core_avs = calloc(sizeof(*av->lav_core_avs),
				  lnx_domain->ld_num_doms);
	if (!av->lav_core_avs) {
		free(av);
		return -FI_ENOMEM;
	}

	table_sz = attr->count ? attr->count : ofi_universe_size;
	table_sz = roundup_power_of_two(table_sz);
	pool_attr.chunk_cnt = table_sz;

	util_attr.addrlen = sizeof(struct lnx_peer *);
	rc = ofi_av_init(&lnx_domain->ld_domain, attr,
			 &util_attr, &av->lav_av, context);
	if (rc) {
		FI_WARN(&lnx_prov, FI_LOG_CORE,
			"failed to initialize AV: %d\n", rc);
		goto out;
	}

	av->lav_max_count = table_sz;
	av->lav_domain = lnx_domain;
	av->lav_av.av_fid.fid.ops = &lnx_av_fi_ops;
	av->lav_av.av_fid.ops = &lnx_av_ops;

	/* walk through the rest of the core providers and open their
	 * respective address vector tables
	 */
	for (i = 0; i < lnx_domain->ld_num_doms; i++) {
		cd = &lnx_domain->ld_core_domains[i];

		core_av = &av->lav_core_avs[i];
		dlist_init(&core_av->cav_endpoints);
		core_av->cav_domain = cd;
		fi = cd->cd_info;
		attr->type = FI_AV_TABLE;
		attr->count = fi->domain_attr->ep_cnt;
		rc = fi_av_open(cd->cd_domain, attr, &core_av->cav_av, context);
		if (rc)
			goto failed;

		/* the cav_map is indexed by the lnx peer addr */
		rc = ofi_bufpool_create_attr(&pool_attr, &core_av->cav_map);
		if (rc)
			goto failed;
	}

	*av_out = &av->lav_av.av_fid;

	return 0;

failed:
	lnx_av_close(&av->lav_av.av_fid.fid);
out:
	return rc;
}


