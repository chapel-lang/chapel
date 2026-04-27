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

#include "smr_util.h"
#include "ofi_shm_p2p.h"
#include <sys/stat.h>

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

	if (smr != peer_smr && peer_smr->flags & SMR_FLAG_CMA_INIT) {
		smr_set_vma_cap(&smr->peer_vma_caps, FI_SHM_P2P_CMA,
				smr_get_vma_cap(peer_smr->peer_vma_caps,
						FI_SHM_P2P_CMA));
		smr->flags |= SMR_FLAG_CMA_INIT;
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
		smr_set_vma_cap(&smr->self_vma_caps, FI_SHM_P2P_CMA,
				(ret == -1) ? false : true);
	} else {
		smr_set_vma_cap(&smr->peer_vma_caps, FI_SHM_P2P_CMA,
				(ret == -1) ? false : true);
		smr_set_vma_cap(&peer_smr->peer_vma_caps, FI_SHM_P2P_CMA,
				smr_get_vma_cap(smr->peer_vma_caps,
						FI_SHM_P2P_CMA));
		smr->flags |= SMR_FLAG_CMA_INIT;
		peer_smr->flags |= SMR_FLAG_CMA_INIT;
	}
}

size_t smr_calculate_size_offsets(size_t tx_count, size_t rx_count,
				  size_t *cmd_offset, size_t *cs_offset,
				  size_t *inject_offset, size_t *rq_offset,
				  size_t *sar_offset, size_t *peer_offset,
				  size_t *name_offset)
{
	size_t cmd_queue_offset, cmd_stack_offset, inject_pool_offset;
	size_t ret_queue_offset, sar_pool_offset, peer_data_offset;
	size_t ep_name_offset, tx_size, rx_size, total_size;

	tx_size = roundup_power_of_two(tx_count);
	rx_size = roundup_power_of_two(rx_count);

	/* Align cmd_queue offset to cache line */
	cmd_queue_offset = ofi_get_aligned_size(sizeof(struct smr_region), 64);
	cmd_stack_offset = cmd_queue_offset + sizeof(struct smr_cmd_queue) +
		sizeof(struct smr_cmd_queue_entry) * rx_size;
	inject_pool_offset = cmd_stack_offset +
		freestack_size(sizeof(struct smr_cmd), tx_size);
	ret_queue_offset = inject_pool_offset + sizeof(struct smr_inject_buf) *
		tx_size;
	ret_queue_offset = ofi_get_aligned_size(ret_queue_offset, 64);
	sar_pool_offset = ret_queue_offset + sizeof(struct smr_return_queue) +
		sizeof(struct smr_return_queue_entry) * tx_size;
	peer_data_offset = sar_pool_offset +
		freestack_size(sizeof(struct smr_sar_buf), SMR_MAX_PEERS);
	ep_name_offset = peer_data_offset + sizeof(struct smr_peer_data) *
		SMR_MAX_PEERS;

	total_size = ep_name_offset + SMR_NAME_MAX;

	if (cmd_offset)
		*cmd_offset = cmd_queue_offset;
	if (cs_offset)
		*cs_offset = cmd_stack_offset;
	if (inject_offset)
		*inject_offset = inject_pool_offset;
	if (rq_offset)
		*rq_offset = ret_queue_offset;
	if (sar_offset)
		*sar_offset = sar_pool_offset;
	if (peer_offset)
		*peer_offset = peer_data_offset;
	if (name_offset)
		*name_offset = ep_name_offset;

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

	assert((uintptr_t) old_shm % SMR_PREFETCH_SZ == 0);

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

static inline void smr_cmd_init(void *buf)
{
	struct smr_cmd_entry *ce = buf;
	ce->ptr = (uintptr_t) &ce->cmd;
}

/* TODO: Determine if aligning SMR data helps performance */
int smr_create(const struct fi_provider *prov, const struct smr_attr *attr,
	       struct smr_region *volatile *smr)
{
	struct smr_ep_name *ep_name;
	size_t total_size, cmd_queue_offset, ret_queue_offset, peer_data_offset;
	size_t cmd_stack_offset, inject_pool_offset, sar_pool_offset;
	size_t name_offset;
	int fd, ret, i;
	void *mapped_addr;
	size_t tx_size, rx_size;

	tx_size = roundup_power_of_two(attr->tx_count);
	rx_size = roundup_power_of_two(attr->rx_count);
	total_size = smr_calculate_size_offsets(
				tx_size, rx_size, &cmd_queue_offset,
				&cmd_stack_offset, &inject_pool_offset,
				&ret_queue_offset, &sar_pool_offset,
				&peer_data_offset, &name_offset);

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

	assert((uintptr_t) mapped_addr % SMR_PREFETCH_SZ == 0);
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

	(*smr)->version = SMR_VERSION;

	(*smr)->flags = attr->flags;

	if (xpmem && smr_env.use_xpmem &&
	    !(attr->flags & SMR_FLAG_HMEM_ENABLED)) {
		smr_set_vma_cap(&(*smr)->self_vma_caps, FI_SHM_P2P_XPMEM, true);
		(*smr)->xpmem_self = xpmem->pinfo;
	}

	(*smr)->base_addr = (uintptr_t) (*smr);

	(*smr)->total_size = total_size;
	(*smr)->cmd_queue_offset = cmd_queue_offset;
	(*smr)->cmd_stack_offset = cmd_stack_offset;
	(*smr)->inject_pool_offset = inject_pool_offset;
	(*smr)->ret_queue_offset = ret_queue_offset;
	(*smr)->sar_pool_offset = sar_pool_offset;
	(*smr)->peer_data_offset = peer_data_offset;
	(*smr)->name_offset = name_offset;
	(*smr)->max_sar_buf_per_peer = SMR_BUF_BATCH_MAX;

	smr_cmd_queue_init(smr_cmd_queue(*smr), rx_size, smr_cmd_init);
	smr_return_queue_init(smr_return_queue(*smr), tx_size, NULL);

	smr_freestack_init(smr_cmd_stack(*smr), tx_size,
			   sizeof(struct smr_cmd));
	smr_freestack_init(smr_sar_pool(*smr), SMR_MAX_PEERS,
			   sizeof(struct smr_sar_buf));
	for (i = 0; i < SMR_MAX_PEERS; i++) {
		smr_peer_data(*smr)[i].id = -1;
		smr_peer_data(*smr)[i].sar_status = SMR_SAR_FREE;
		smr_peer_data(*smr)[i].name_sent = 0;
		smr_peer_data(*smr)[i].xpmem.avail = false;
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