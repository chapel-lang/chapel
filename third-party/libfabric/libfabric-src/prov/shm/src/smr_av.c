/*
 * Copyright (c) Intel Corporation. All rights reserved.
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

#include "smr.h"
#include <sys/stat.h>

void smr_map_to_endpoint(struct smr_ep *ep, int64_t id)
{
	int ret;
	struct smr_region *peer_smr;
	struct smr_peer_data *local_peers;

	assert(&ep->util_ep.av->lock);
	peer_smr = smr_peer_region(ep, id);
	if (!ep->map->peers[id].id_assigned || !peer_smr)
	    return;

	local_peers = smr_peer_data(ep->region);
	local_peers[id].local_region = (uintptr_t) peer_smr;

	if (ep->region == peer_smr || !(ep->region->flags & SMR_FLAG_CMA_INIT))
		smr_cma_check(ep->region, peer_smr);

	/* enable xpmem locally if the peer also has it enabled */
	if (smr_get_vma_cap(peer_smr->self_vma_caps, FI_SHM_P2P_XPMEM) &&
	    smr_get_vma_cap(ep->region->self_vma_caps, FI_SHM_P2P_XPMEM)) {
		ret = ofi_xpmem_enable(&peer_smr->xpmem_self,
				       &local_peers[id].xpmem);
		if (ret) {
			local_peers[id].xpmem.avail = false;
			smr_set_vma_cap(&ep->region->self_vma_caps,
					FI_SHM_P2P_XPMEM, false);
			return;
		}
		local_peers[id].xpmem.avail = true;
		local_peers[id].xpmem.addr_max =
					peer_smr->xpmem_self.address_max;
	} else {
		local_peers[id].xpmem.avail = false;
	}

	smr_set_ipc_valid(ep, id);

	return;
}

static int smr_match_name(struct dlist_entry *item, const void *args)
{
	return !strcmp(container_of(item, struct smr_ep_name, entry)->name,
		       (char *) args);
}

int smr_map_to_region(struct smr_map *map, int64_t id)
{
	struct smr_peer *peer_buf = &map->peers[id];
	struct smr_region *peer;
	struct util_ep *util_ep;
	struct smr_ep *smr_ep;
	struct smr_av *av = container_of(map, struct smr_av, smr_map);
	size_t size;
	int fd, ret = 0;
	struct stat sts;
	struct dlist_entry *entry;
	const char *name = smr_no_prefix(peer_buf->name);
	char tmp[SMR_PATH_MAX];

	pthread_mutex_lock(&ep_list_lock);
	entry = dlist_find_first_match(&ep_name_list, smr_match_name, name);
	if (entry) {
		peer_buf->region = container_of(entry, struct smr_ep_name,
						entry)->region;
		pthread_mutex_unlock(&ep_list_lock);
		return FI_SUCCESS;
	}
	pthread_mutex_unlock(&ep_list_lock);

	if (peer_buf->region)
		return FI_SUCCESS;

	assert(ofi_genlock_held(&av->util_av.lock));
	fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		FI_WARN_ONCE(&smr_prov, FI_LOG_AV,
			     "shm_open error: name %s errno %d\n", name, errno);
		return -errno;
	}

	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, sizeof(tmp), "%s%s", SMR_DIR, name);
	if (stat(tmp, &sts) == -1) {
		ret = -errno;
		goto out;
	}

	if (sts.st_size < sizeof(*peer)) {
		ret = -FI_ENOENT;
		goto out;
	}

	peer = mmap(NULL, sizeof(*peer), PROT_READ | PROT_WRITE,
		    MAP_SHARED, fd, 0);
	if (peer == MAP_FAILED) {
		FI_WARN(&smr_prov, FI_LOG_AV, "mmap error\n");
		ret = -errno;
		goto out;
	}

	assert((uintptr_t) peer % SMR_PREFETCH_SZ == 0);

	if (!peer->pid) {
		FI_WARN(&smr_prov, FI_LOG_AV, "peer not initialized\n");
		munmap(peer, sizeof(*peer));
		ret = -FI_ENOENT;
		goto out;
	}

	size = peer->total_size;
	munmap(peer, sizeof(*peer));

	peer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	peer_buf->region = peer;

	assert((uintptr_t) peer % SMR_PREFETCH_SZ == 0);

	if (map->flags & SMR_FLAG_HMEM_ENABLED) {
		ret = ofi_hmem_host_register(peer, peer->total_size);
		if (ret)
			FI_WARN(&smr_prov, FI_LOG_AV,
				"unable to register shm with iface\n");
		if (ofi_hmem_is_initialized(FI_HMEM_ZE)) {
			peer_buf->pid_fd = ofi_pidfd_open(peer->pid, 0);
			if (peer_buf->pid_fd < 0) {
				FI_WARN(&smr_prov, FI_LOG_AV,
					"unable to open pidfd\n");
			}
		} else {
			peer_buf->pid_fd = -1;
		}
	}

	dlist_foreach_container(&av->util_av.ep_list, struct util_ep, util_ep,
				av_entry) {
		smr_ep = container_of(util_ep, struct smr_ep, util_ep);
		smr_map_to_endpoint(smr_ep, id);
	}

out:
	close(fd);
	return ret;
}

static void smr_unmap_from_endpoint(struct smr_ep *ep, int64_t id)
{
	struct smr_region *peer_smr;
	struct smr_peer_data *local_peers, *peer_peers;
	int64_t peer_id;

	if (!ep->map->peers[id].id_assigned)
		return;

	peer_smr = smr_peer_region(ep, id);
	assert(peer_smr);
	peer_peers = smr_peer_data(peer_smr);
	peer_id = smr_peer_data(ep->region)[id].id;

	peer_peers[peer_id].id = -1;
	peer_peers[peer_id].name_sent = 0;

	local_peers = smr_peer_data(ep->region);
	ofi_xpmem_release(&local_peers[peer_id].xpmem);
}

void smr_unmap_region(struct smr_map *map, int64_t peer_id, bool local)
{
	struct smr_region *peer_region;
	struct smr_peer *peer;
	struct util_ep *util_ep;
	struct smr_ep *smr_ep;
	struct smr_av *av;
	int ret = 0;

	av = container_of(map, struct smr_av, smr_map);

	assert(ofi_genlock_held(&av->util_av.lock));
	peer_region = map->peers[peer_id].region;
	if (!peer_region)
		return;

	peer = &map->peers[peer_id];
	dlist_foreach_container(&av->util_av.ep_list, struct util_ep, util_ep,
				av_entry) {
		smr_ep = container_of(util_ep, struct smr_ep, util_ep);
		smr_unmap_from_endpoint(smr_ep, peer_id);
	}

	/* Don't unmap memory owned by this pid because the endpoint it belongs
	 * to might still be active.
	 */
	if (local)
		return;

	if (map->flags & SMR_FLAG_HMEM_ENABLED) {
		ret = ofi_hmem_host_unregister(peer_region);
		if (ret)
			FI_WARN(&smr_prov, FI_LOG_AV,
				"unable to unregister shm with iface\n");

		if (peer->pid_fd != -1) {
			close(peer->pid_fd);
			peer->pid_fd = -1;
		}
	}

	munmap(peer_region, peer_region->total_size);
	peer->region = NULL;
}

void smr_map_add(struct smr_map *map, const char *name, int64_t *id)
{
	struct ofi_rbnode *node;
	const char *shm_name = smr_no_prefix(name);
	int tries = 0, ret = 0;

	assert(ofi_genlock_held(&container_of(map, struct smr_av,
	       smr_map)->util_av.lock));

	ret = ofi_rbmap_insert(&map->rbmap, (void *) shm_name,
			       (void *) (intptr_t) *id, &node);
	if (ret) {
		assert(ret == -FI_EALREADY);
		*id = (intptr_t) node->data;
		return;
	}

	while (map->peers[map->cur_id].id_assigned && tries < SMR_MAX_PEERS) {
		if (++map->cur_id == SMR_MAX_PEERS)
			map->cur_id = 0;
		tries++;
	}

	assert(map->cur_id < SMR_MAX_PEERS && tries < SMR_MAX_PEERS);
	*id = map->cur_id;
	if (++map->cur_id == SMR_MAX_PEERS)
		map->cur_id = 0;
	node->data = (void *) (intptr_t) *id;
	strncpy(map->peers[*id].name, shm_name, SMR_NAME_MAX);
	map->peers[*id].name[SMR_NAME_MAX - 1] = '\0';
	map->peers[*id].region = NULL;
	map->num_peers++;
	map->peers[*id].id_assigned = true;
}

static void smr_map_del(struct smr_map *map, int64_t id)
{
	struct smr_ep_name *name;
	bool local = false;

	assert(ofi_genlock_held(&container_of(map, struct smr_av,
					      smr_map)->util_av.lock));

	assert(id >= 0 && id < SMR_MAX_PEERS);
	pthread_mutex_lock(&ep_list_lock);
	dlist_foreach_container(&ep_name_list, struct smr_ep_name, name,
				entry) {
		if (!strcmp(name->name, map->peers[id].name)) {
			local = true;
			break;
		}
	}
	pthread_mutex_unlock(&ep_list_lock);


	smr_unmap_region(map, id, local);
	map->peers[id].fiaddr = FI_ADDR_NOTAVAIL;
	map->peers[id].id_assigned = false;
	map->num_peers--;
	ofi_rbmap_find_delete(&map->rbmap, map->peers[id].name);
}

struct smr_region *smr_map_get(struct smr_map *map, int64_t id)
{
	if (id < 0 || id >= SMR_MAX_PEERS)
		return NULL;

	return map->peers[id].region;
}

static int smr_name_compare(struct ofi_rbmap *map, void *key, void *data)
{
	struct smr_map *smr_map;

	smr_map = container_of(map, struct smr_map, rbmap);

	return strncmp(smr_map->peers[(uintptr_t) data].name,
		       (char *) key, SMR_NAME_MAX);
}

static int smr_map_init(struct smr_map *map, int peer_count, uint16_t flags)
{
	int i;

	for (i = 0; i < peer_count; i++) {
		memset(&map->peers[i].name, 0, SMR_NAME_MAX);
		map->peers[i].id_assigned = 0;
		map->peers[i].fiaddr = FI_ADDR_NOTAVAIL;
	}
	map->flags = flags;

	ofi_rbmap_init(&map->rbmap, smr_name_compare);

	return 0;
}

static void smr_map_cleanup(struct smr_av *av)
{
	int64_t i;

	ofi_genlock_lock(&av->util_av.lock);
	for (i = 0; i < SMR_MAX_PEERS; i++) {
		if (av->smr_map.peers[i].id_assigned)
			smr_map_del(&av->smr_map, i);
	}
	ofi_rbmap_cleanup(&av->smr_map.rbmap);
	ofi_genlock_unlock(&av->util_av.lock);
}

static int smr_av_close(struct fid *fid)
{
	struct smr_av *av;
	int ret;

	av = container_of(fid, struct smr_av, util_av.av_fid);

	smr_map_cleanup(av);

	ret = ofi_av_close(&av->util_av);
	if (ret)
		return ret;

	free(av);
	return 0;
}

static fi_addr_t smr_get_addr(struct fi_peer_rx_entry *rx_entry)
{
	struct smr_cmd_ctx *cmd_ctx = rx_entry->peer_context;
	struct smr_av *av;

	av = container_of(cmd_ctx->ep->util_ep.av, struct smr_av, util_av);

	return av->smr_map.peers[cmd_ctx->cmd->hdr.rx_id].fiaddr;
}

static int smr_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct util_av *util_av;
	struct util_ep *util_ep;
	struct smr_av *smr_av;
	struct smr_ep *smr_ep;
	struct dlist_entry *av_entry;
	fi_addr_t util_addr;
	int64_t shm_id = -1;
	int i, ret;
	int succ_count = 0;

	util_av = container_of(av_fid, struct util_av, av_fid);
	smr_av = container_of(util_av, struct smr_av, util_av);

	ofi_genlock_lock(&util_av->lock);
	for (i = 0; i < count; i++, addr = (char *) addr + strlen(addr) + 1) {
		FI_INFO(&smr_prov, FI_LOG_AV, "%s\n", (const char *) addr);

		util_addr = FI_ADDR_NOTAVAIL;
		if (smr_av->used < SMR_MAX_PEERS) {
			smr_map_add(&smr_av->smr_map, addr, &shm_id);
			ret = ofi_av_insert_addr(util_av, &shm_id, &util_addr);
		} else {
			FI_WARN(&smr_prov, FI_LOG_AV,
				"AV insert failed. The maximum number of AV "
				"entries shm supported has been reached.\n");
			ret = -FI_ENOMEM;
		}

		FI_INFO(&smr_prov, FI_LOG_AV, "fi_addr: %" PRIu64 "\n",
			util_addr);

		if (ret) {
			if (fi_addr)
				fi_addr[i] = util_addr;
			if (shm_id >= 0)
				smr_map_del(&smr_av->smr_map, shm_id);
			continue;
		}

		assert(shm_id >= 0 && shm_id < SMR_MAX_PEERS);
		if (flags & FI_AV_USER_ID) {
			assert(fi_addr);
			smr_av->smr_map.peers[shm_id].fiaddr = fi_addr[i];
		} else {
			smr_av->smr_map.peers[shm_id].fiaddr = util_addr;
		}
		succ_count++;
		smr_av->used++;

		if (fi_addr)
			fi_addr[i] = util_addr;

		assert(smr_av->smr_map.num_peers > 0);
		dlist_foreach(&util_av->ep_list, av_entry) {
        		util_ep = container_of(av_entry, struct util_ep,
					       av_entry);
        		smr_ep = container_of(util_ep, struct smr_ep, util_ep);
			smr_ep->region->max_sar_buf_per_peer =
				MIN(SMR_BUF_BATCH_MAX,
				    SMR_MAX_PEERS / smr_av->smr_map.num_peers);
			ofi_genlock_lock(&util_ep->lock);
			smr_ep->srx->owner_ops->foreach_unspec_addr(
						smr_ep->srx, &smr_get_addr);
			ofi_genlock_unlock(&util_ep->lock);
		}
	}
	ofi_genlock_unlock(&util_av->lock);

	return succ_count;
}

static int smr_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	struct util_av *util_av;
	struct util_ep *util_ep;
	struct smr_av *smr_av;
	struct smr_ep *smr_ep;
	struct dlist_entry *av_entry;
	int i, ret = 0;
	int64_t id;

	util_av = container_of(av_fid, struct util_av, av_fid);
	smr_av = container_of(util_av, struct smr_av, util_av);

	ofi_genlock_lock(&util_av->lock);
	for (i = 0; i < count; i++) {
		FI_INFO(&smr_prov, FI_LOG_AV, "%" PRIu64 "\n", fi_addr[i]);
		id = smr_addr_lookup(util_av, fi_addr[i]);
		ret = ofi_av_remove_addr(util_av, fi_addr[i]);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_AV,
				"Unable to remove address from AV\n");
			break;
		}

		smr_map_del(&smr_av->smr_map, id);
		dlist_foreach(&util_av->ep_list, av_entry) {
			util_ep = container_of(av_entry, struct util_ep,
					       av_entry);
			smr_ep = container_of(util_ep, struct smr_ep, util_ep);
			if (smr_av->smr_map.num_peers > 0)
				smr_ep->region->max_sar_buf_per_peer =
						SMR_MAX_PEERS /
						smr_av->smr_map.num_peers;
			else
				smr_ep->region->max_sar_buf_per_peer =
							SMR_BUF_BATCH_MAX;
		}
		smr_av->used--;
	}

	ofi_genlock_unlock(&util_av->lock);
	return ret;
}

static int smr_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			 size_t *addrlen)
{
	struct util_av *util_av;
	struct smr_av *smr_av;
	int64_t id;
	char *name;

	util_av = container_of(av, struct util_av, av_fid);
	smr_av = container_of(util_av, struct smr_av, util_av);

	id = smr_addr_lookup(util_av, fi_addr);
	name = smr_av->smr_map.peers[id].name;

	strncpy((char *) addr, name, *addrlen);

	((char *) addr)[MIN(*addrlen - 1, strlen(name))] = '\0';
	*addrlen = strlen(name) + 1;
	return 0;
}

static const char *smr_av_straddr(struct fid_av *av, const void *addr,
				  char *buf, size_t *len)
{
	/* the input address is a string format */
	if (buf)
		strncpy(buf, (const char *)addr, *len);

	*len = strlen((const char *)addr) + 1;
	return buf;
}

static struct fi_ops smr_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = smr_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av smr_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = smr_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = smr_av_remove,
	.lookup = smr_av_lookup,
	.straddr = smr_av_straddr,
};

int smr_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context)
{
	struct util_domain *util_domain;
	struct util_av_attr util_attr;
	struct smr_av *smr_av;
	int ret;

	if (!attr) {
		FI_INFO(&smr_prov, FI_LOG_AV, "invalid attr\n");
		return -FI_EINVAL;
	}

	if (attr->name) {
		FI_INFO(&smr_prov, FI_LOG_AV, "shared AV not supported\n");
		return -FI_ENOSYS;
	}

	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	util_domain = container_of(domain, struct util_domain, domain_fid);

	smr_av = calloc(1, sizeof *smr_av);
	if (!smr_av)
		return -FI_ENOMEM;

	util_attr.addrlen = sizeof(int64_t);
	util_attr.context_len = 0;
	util_attr.flags = 0;
	if (attr->count > SMR_MAX_PEERS) {
		FI_INFO(&smr_prov, FI_LOG_AV,
			"count %d exceeds max peers\n", (int) attr->count);
		ret = -FI_ENOSYS;
		goto out;
	}

	ret = ofi_av_init(util_domain, attr, &util_attr, &smr_av->util_av,
			  context);
	if (ret)
		goto out;

	smr_av->used = 0;
	*av = &smr_av->util_av.av_fid;
	(*av)->fid.ops = &smr_av_fi_ops;
	(*av)->ops = &smr_av_ops;

	ret = smr_map_init(&smr_av->smr_map, SMR_MAX_PEERS,
			   util_domain->info_domain_caps & FI_HMEM ?
			   SMR_FLAG_HMEM_ENABLED : 0);
	if (ret)
		goto close;

	return 0;

close:
	(void) ofi_av_close(&smr_av->util_av);
out:
	free(smr_av);
	return ret;
}