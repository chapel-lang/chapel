/*
 * Copyright (c) 2016-2021 Intel Corporation. All rights reserved.
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

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <ofi_xpmem.h>

#include "smr_util.h"
#include "smr.h"

struct dlist_entry ep_name_list;
DEFINE_LIST(ep_name_list);
pthread_mutex_t ep_list_lock = PTHREAD_MUTEX_INITIALIZER;

void smr_cleanup(void)
{
	struct smr_ep_name *ep_name;
	struct dlist_entry *tmp;

	pthread_mutex_lock(&ep_list_lock);
	dlist_foreach_container_safe(&ep_name_list, struct smr_ep_name,
				     ep_name, entry, tmp)
		free(ep_name);
	pthread_mutex_unlock(&ep_list_lock);
}

void smr_cma_check(struct smr_region *smr, struct smr_region *peer_smr)
{
	struct iovec local_iov, remote_iov;
	int remote_pid;
	int ret;

	if (smr != peer_smr && peer_smr->cma_cap_peer != SMR_VMA_CAP_NA) {
		smr->cma_cap_peer = peer_smr->cma_cap_peer;
		return;
	}
	remote_pid = peer_smr->pid;
	local_iov.iov_base = &remote_pid;
	local_iov.iov_len = sizeof(remote_pid);
	remote_iov.iov_base = (char *)peer_smr->base_addr +
			      ((char *)&peer_smr->pid - (char *)peer_smr);
	remote_iov.iov_len = sizeof(peer_smr->pid);
	ret = ofi_process_vm_writev(peer_smr->pid, &local_iov, 1,
				    &remote_iov, 1, 0);
	assert(remote_pid == peer_smr->pid);

	if (smr == peer_smr) {
		smr->cma_cap_self = (ret == -1) ? SMR_VMA_CAP_OFF : SMR_VMA_CAP_ON;
	} else {
		smr->cma_cap_peer = (ret == -1) ? SMR_VMA_CAP_OFF : SMR_VMA_CAP_ON;
		peer_smr->cma_cap_peer = smr->cma_cap_peer;
	}
}

size_t smr_calculate_size_offsets(size_t tx_count, size_t rx_count,
				  size_t *cmd_offset, size_t *resp_offset,
				  size_t *inject_offset, size_t *sar_offset,
				  size_t *peer_offset, size_t *name_offset,
				  size_t *sock_offset)
{
	size_t cmd_queue_offset, resp_queue_offset, inject_pool_offset;
	size_t sar_pool_offset, peer_data_offset, ep_name_offset;
	size_t tx_size, rx_size, total_size, sock_name_offset;

	tx_size = roundup_power_of_two(tx_count);
	rx_size = roundup_power_of_two(rx_count);

	/* Align cmd_queue offset to cache line */
	cmd_queue_offset = ofi_get_aligned_size(sizeof(struct smr_region), 64);
	resp_queue_offset = cmd_queue_offset + sizeof(struct smr_cmd_queue) +
			    sizeof(struct smr_cmd_queue_entry) * rx_size;
	inject_pool_offset = resp_queue_offset + sizeof(struct smr_resp_queue) +
			     sizeof(struct smr_resp) * tx_size;
	sar_pool_offset = inject_pool_offset +
		freestack_size(sizeof(struct smr_inject_buf), rx_size);
	peer_data_offset = sar_pool_offset +
		freestack_size(sizeof(struct smr_sar_buf), SMR_MAX_PEERS);
	ep_name_offset = peer_data_offset + sizeof(struct smr_peer_data) *
		SMR_MAX_PEERS;

	sock_name_offset = ep_name_offset + SMR_NAME_MAX;

	if (cmd_offset)
		*cmd_offset = cmd_queue_offset;
	if (resp_offset)
		*resp_offset = resp_queue_offset;
	if (inject_offset)
		*inject_offset = inject_pool_offset;
	if (sar_offset)
		*sar_offset = sar_pool_offset;
	if (peer_offset)
		*peer_offset = peer_data_offset;
	if (name_offset)
		*name_offset = ep_name_offset;
	if (sock_offset)
		*sock_offset = sock_name_offset;

	total_size = sock_name_offset + SMR_SOCK_NAME_MAX;

	/*
 	 * Revisit later to see if we really need the size adjustment, or
 	 * at most align to a multiple of a page size.
 	 */
	total_size = roundup_power_of_two(total_size);

	return total_size;
}

static int smr_retry_map(const char *name, int *fd)
{
	char tmp[NAME_MAX];
	struct smr_region *old_shm;
	struct stat sts;
	int shm_pid;

	*fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (*fd < 0)
		return -errno;

	old_shm = mmap(NULL, sizeof(*old_shm), PROT_READ | PROT_WRITE,
		       MAP_SHARED, *fd, 0);
	if (old_shm == MAP_FAILED)
		goto err;

        /* No backwards compatibility for now. */
	if (old_shm->version != SMR_VERSION) {
		munmap(old_shm, sizeof(*old_shm));
		goto err;
	}
	shm_pid = old_shm->pid;
	munmap(old_shm, sizeof(*old_shm));

	if (!shm_pid)
		return FI_SUCCESS;

	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, sizeof(tmp), "/proc/%d", shm_pid);

	if (stat(tmp, &sts) == -1 && errno == ENOENT)
		return FI_SUCCESS;

err:
	close(*fd);
	shm_unlink(name);
	return -FI_EBUSY;
}

static void smr_lock_init(pthread_spinlock_t *lock)
{
	pthread_spin_init(lock, PTHREAD_PROCESS_SHARED);
}

/* TODO: Determine if aligning SMR data helps performance */
int smr_create(const struct fi_provider *prov, struct smr_map *map,
	       const struct smr_attr *attr, struct smr_region *volatile *smr)
{
	struct smr_ep_name *ep_name;
	size_t total_size, cmd_queue_offset, peer_data_offset;
	size_t resp_queue_offset, inject_pool_offset, name_offset;
	size_t sar_pool_offset, sock_name_offset;
	int fd, ret, i;
	void *mapped_addr;
	size_t tx_size, rx_size;

	tx_size = roundup_power_of_two(attr->tx_count);
	rx_size = roundup_power_of_two(attr->rx_count);
	total_size = smr_calculate_size_offsets(tx_size, rx_size, &cmd_queue_offset,
					&resp_queue_offset, &inject_pool_offset,
					&sar_pool_offset, &peer_data_offset,
					&name_offset, &sock_name_offset);

	fd = shm_open(attr->name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		if (errno != EEXIST) {
			FI_WARN(prov, FI_LOG_EP_CTRL,
				"shm_open error (%s): %s\n",
				attr->name, strerror(errno));
			return -errno;
		}

		ret = smr_retry_map(attr->name, &fd);
		if (ret) {
			FI_WARN(prov, FI_LOG_EP_CTRL, "shm file in use (%s)\n",
				attr->name);
			return ret;
		}
		FI_WARN(prov, FI_LOG_EP_CTRL,
			"Overwriting shm from dead process (%s)\n", attr->name);
	}

	ep_name = calloc(1, sizeof(*ep_name));
	if (!ep_name) {
		FI_WARN(prov, FI_LOG_EP_CTRL, "calloc error\n");
		ret = -FI_ENOMEM;
		goto close;
	}
	strncpy(ep_name->name, (char *)attr->name, SMR_NAME_MAX - 1);
	ep_name->name[SMR_NAME_MAX - 1] = '\0';

	pthread_mutex_lock(&ep_list_lock);
	dlist_insert_tail(&ep_name->entry, &ep_name_list);

	ret = ftruncate(fd, total_size);
	if (ret < 0) {
		FI_WARN(prov, FI_LOG_EP_CTRL, "ftruncate error\n");
		ret = -errno;
		goto remove;
	}

	mapped_addr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
			   MAP_SHARED, fd, 0);
	if (mapped_addr == MAP_FAILED) {
		FI_WARN(prov, FI_LOG_EP_CTRL, "mmap error\n");
		ret = -errno;
		goto remove;
	}

	close(fd);

	if (attr->flags & SMR_FLAG_HMEM_ENABLED) {
		ret = ofi_hmem_host_register(mapped_addr, total_size);
		if (ret)
			FI_WARN(prov, FI_LOG_EP_CTRL,
				"unable to register shm with iface\n");
	}

	ep_name->region = mapped_addr;
	pthread_mutex_unlock(&ep_list_lock);

	*smr = mapped_addr;
	smr_lock_init(&(*smr)->lock);

	(*smr)->map = map;
	(*smr)->version = SMR_VERSION;

	(*smr)->flags = attr->flags;
#ifdef HAVE_ATOMICS
	(*smr)->flags |= SMR_FLAG_ATOMIC;
#endif
#if ENABLE_DEBUG
	(*smr)->flags |= SMR_FLAG_DEBUG;
#endif

	(*smr)->cma_cap_peer = SMR_VMA_CAP_NA;
	(*smr)->cma_cap_self = SMR_VMA_CAP_NA;

	(*smr)->xpmem_cap_self = SMR_VMA_CAP_OFF;
	if (xpmem && smr_env.use_xpmem) {
		(*smr)->xpmem_cap_self = SMR_VMA_CAP_ON;
		(*smr)->xpmem_self = xpmem->pinfo;
	}

	(*smr)->base_addr = *smr;

	(*smr)->total_size = total_size;
	(*smr)->cmd_queue_offset = cmd_queue_offset;
	(*smr)->resp_queue_offset = resp_queue_offset;
	(*smr)->inject_pool_offset = inject_pool_offset;
	(*smr)->sar_pool_offset = sar_pool_offset;
	(*smr)->peer_data_offset = peer_data_offset;
	(*smr)->name_offset = name_offset;
	(*smr)->sock_name_offset = sock_name_offset;
	(*smr)->max_sar_buf_per_peer = SMR_BUF_BATCH_MAX;

	smr_cmd_queue_init(smr_cmd_queue(*smr), rx_size);
	smr_resp_queue_init(smr_resp_queue(*smr), tx_size);
	smr_freestack_init(smr_inject_pool(*smr), rx_size,
			sizeof(struct smr_inject_buf));
	smr_freestack_init(smr_sar_pool(*smr), SMR_MAX_PEERS,
			sizeof(struct smr_sar_buf));
	for (i = 0; i < SMR_MAX_PEERS; i++) {
		smr_peer_data(*smr)[i].addr.id = -1;
		smr_peer_data(*smr)[i].sar_status = 0;
		smr_peer_data(*smr)[i].name_sent = 0;
		smr_peer_data(*smr)[i].xpmem.cap = SMR_VMA_CAP_OFF;
	}

	strncpy((char *) smr_name(*smr), attr->name, total_size - name_offset);

	/* Must be set last to signal full initialization to peers */
	(*smr)->pid = getpid();
	return 0;

remove:
	dlist_remove(&ep_name->entry);
	pthread_mutex_unlock(&ep_list_lock);
	free(ep_name);
close:
	close(fd);
	shm_unlink(attr->name);
	return ret;
}

void smr_free(struct smr_region *smr)
{
	if (smr->flags & SMR_FLAG_HMEM_ENABLED)
		(void) ofi_hmem_host_unregister(smr);
	shm_unlink(smr_name(smr));
	munmap(smr, smr->total_size);
}

static int smr_match_name(struct dlist_entry *item, const void *args)
{
	return !strcmp(container_of(item, struct smr_ep_name, entry)->name,
		       (char *) args);
}

int smr_map_to_region(const struct fi_provider *prov, struct smr_map *map,
		      int64_t id)
{
	struct smr_peer *peer_buf = &map->peers[id];
	struct smr_region *peer;
	struct util_ep *util_ep;
	struct smr_ep *smr_ep;
	struct smr_av *av;
	size_t size;
	int fd, ret = 0;
	struct stat sts;
	struct dlist_entry *entry;
	const char *name = smr_no_prefix(peer_buf->peer.name);
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

	assert(ofi_spin_held(&map->lock));
	fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		FI_WARN_ONCE(prov, FI_LOG_AV,
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
		FI_WARN(prov, FI_LOG_AV, "mmap error\n");
		ret = -errno;
		goto out;
	}

	if (!peer->pid) {
		FI_WARN(prov, FI_LOG_AV, "peer not initialized\n");
		munmap(peer, sizeof(*peer));
		ret = -FI_ENOENT;
		goto out;
	}

	size = peer->total_size;
	munmap(peer, sizeof(*peer));

	peer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	peer_buf->region = peer;

	if (map->flags & SMR_FLAG_HMEM_ENABLED) {
		ret = ofi_hmem_host_register(peer, peer->total_size);
		if (ret)
			FI_WARN(prov, FI_LOG_EP_CTRL,
				"unable to register shm with iface\n");
		if (ofi_hmem_is_initialized(FI_HMEM_ZE)) {
			peer_buf->pid_fd = ofi_pidfd_open(peer->pid, 0);
			if (peer_buf->pid_fd < 0) {
				FI_WARN(prov, FI_LOG_EP_CTRL,
					"unable to open pidfd\n");
			}
		} else {
			peer_buf->pid_fd = -1;
		}
	}

	av = container_of(map, struct smr_av, smr_map);
	dlist_foreach_container(&av->util_av.ep_list, struct util_ep, util_ep,
				av_entry) {
		smr_ep = container_of(util_ep, struct smr_ep, util_ep);
		smr_map_to_endpoint(smr_ep->region, id);
	}

out:
	close(fd);
	return ret;
}

void smr_map_to_endpoint(struct smr_region *region, int64_t id)
{
	int ret;
	struct smr_region *peer_smr;
	struct smr_peer_data *local_peers;

	assert(ofi_spin_held(&region->map->lock));
	peer_smr = smr_peer_region(region, id);
	if (region->map->peers[id].peer.id < 0 || !peer_smr)
	    return;

	local_peers = smr_peer_data(region);

	if ((region != peer_smr && region->cma_cap_peer == SMR_VMA_CAP_NA) ||
	    (region == peer_smr && region->cma_cap_self == SMR_VMA_CAP_NA))
		smr_cma_check(region, peer_smr);

	/* enable xpmem locally if the peer also has it enabled */
	if (peer_smr->xpmem_cap_self == SMR_VMA_CAP_ON &&
	    region->xpmem_cap_self == SMR_VMA_CAP_ON) {
		ret = ofi_xpmem_enable(&peer_smr->xpmem_self,
				       &local_peers[id].xpmem);
		if (ret) {
			local_peers[id].xpmem.cap = SMR_VMA_CAP_OFF;
			region->xpmem_cap_self = SMR_VMA_CAP_OFF;
			return;
		}
		local_peers[id].xpmem.cap = SMR_VMA_CAP_ON;
		local_peers[id].xpmem.addr_max = peer_smr->xpmem_self.address_max;
	} else {
		local_peers[id].xpmem.cap = SMR_VMA_CAP_OFF;
	}

	smr_set_ipc_valid(region, id);

	return;
}

void smr_unmap_region(const struct fi_provider *prov, struct smr_map *map,
		     int64_t peer_id, bool local)
{
	struct smr_region *peer_region;
	struct smr_peer *peer;
	struct util_ep *util_ep;
	struct smr_ep *smr_ep;
	struct smr_av *av;
	int ret = 0;

	assert(ofi_spin_held(&map->lock));
	peer_region = map->peers[peer_id].region;
	if (!peer_region)
		return;

	peer = &map->peers[peer_id];
	av = container_of(map, struct smr_av, smr_map);
	dlist_foreach_container(&av->util_av.ep_list, struct util_ep, util_ep,
				av_entry) {
		smr_ep = container_of(util_ep, struct smr_ep, util_ep);
		smr_unmap_from_endpoint(smr_ep->region, peer_id);
	}

	/* Don't unmap memory owned by this pid because the endpoint it belongs
	 * to might still be active.
	 */
	if (local)
		return;

	if (map->flags & SMR_FLAG_HMEM_ENABLED) {
		ret = ofi_hmem_host_unregister(peer_region);
		if (ret)
			FI_WARN(prov, FI_LOG_EP_CTRL,
				"unable to unregister shm with iface\n");

		if (peer->pid_fd != -1) {
			close(peer->pid_fd);
			peer->pid_fd = -1;
		}
	}

	munmap(peer_region, peer_region->total_size);
	peer->region = NULL;
}

void smr_unmap_from_endpoint(struct smr_region *region, int64_t id)
{
	struct smr_region *peer_smr;
	struct smr_peer_data *local_peers, *peer_peers;
	int64_t peer_id;

	if (region->map->peers[id].peer.id < 0)
		return;

	peer_smr = smr_peer_region(region, id);
	assert(peer_smr);
	peer_peers = smr_peer_data(peer_smr);
	peer_id = smr_peer_data(region)[id].addr.id;

	peer_peers[peer_id].addr.id = -1;
	peer_peers[peer_id].name_sent = 0;

	local_peers = smr_peer_data(region);
	ofi_xpmem_release(&local_peers[peer_id].xpmem);
}

void smr_exchange_all_peers(struct smr_region *region)
{
	int64_t i;

	ofi_spin_lock(&region->map->lock);
	for (i = 0; i < SMR_MAX_PEERS; i++)
		smr_map_to_endpoint(region, i);

	ofi_spin_unlock(&region->map->lock);
}

int smr_map_add(const struct fi_provider *prov, struct smr_map *map,
		const char *name, int64_t *id)
{
	struct ofi_rbnode *node;
	const char *shm_name = smr_no_prefix(name);
	int tries = 0, ret = 0;

	ofi_spin_lock(&map->lock);
	ret = ofi_rbmap_insert(&map->rbmap, (void *) shm_name,
			       (void *) (intptr_t) *id, &node);
	if (ret) {
		assert(ret == -FI_EALREADY);
		*id = (intptr_t) node->data;
		goto out;
	}

	while (map->peers[map->cur_id].peer.id != -1 && tries < SMR_MAX_PEERS) {
		if (++map->cur_id == SMR_MAX_PEERS)
			map->cur_id = 0;
		tries++;
	}

	assert(map->cur_id < SMR_MAX_PEERS && tries < SMR_MAX_PEERS);
	*id = map->cur_id;
	if (++map->cur_id == SMR_MAX_PEERS)
		map->cur_id = 0;
	node->data = (void *) (intptr_t) *id;
	strncpy(map->peers[*id].peer.name, shm_name, SMR_NAME_MAX);
	map->peers[*id].peer.name[SMR_NAME_MAX - 1] = '\0';
	map->peers[*id].region = NULL;
	map->num_peers++;
	map->peers[*id].peer.id = *id;

out:
	ofi_spin_unlock(&map->lock);
	return FI_SUCCESS;
}

void smr_map_del(struct smr_map *map, int64_t id)
{
	struct smr_ep_name *name;
	bool local = false;

	assert(id >= 0 && id < SMR_MAX_PEERS);
	pthread_mutex_lock(&ep_list_lock);
	dlist_foreach_container(&ep_name_list, struct smr_ep_name, name, entry) {
		if (!strcmp(name->name, map->peers[id].peer.name)) {
			local = true;
			break;
		}
	}
	pthread_mutex_unlock(&ep_list_lock);
	ofi_spin_lock(&map->lock);
	smr_unmap_region(&smr_prov, map, id, local);
	map->peers[id].fiaddr = FI_ADDR_NOTAVAIL;
	map->peers[id].peer.id = -1;
	map->num_peers--;
	ofi_rbmap_find_delete(&map->rbmap, map->peers[id].peer.name);
	ofi_spin_unlock(&map->lock);
}

struct smr_region *smr_map_get(struct smr_map *map, int64_t id)
{
	if (id < 0 || id >= SMR_MAX_PEERS)
		return NULL;

	return map->peers[id].region;
}
