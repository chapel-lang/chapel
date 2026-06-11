/*
 * Copyright (c) Intel Corporation. All rights reserved
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

#include "smr_dsa.h"

#if SHM_HAVE_DSA

#include <accel-config/libaccel_config.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <immintrin.h> // _mm_pause
#include <linux/idxd.h>
#include <numa.h>

#define MAX_WQS_PER_EP 4
#define GENCAP_CACHE_CTRL_MEM 0x4
#define LIMITED_MSIX_PORTAL_OFFSET 0x1000

#define CMD_CONTEXT_COUNT 32

// Number of DSA descriptors must be large enough to fill the
// maximum number of SAR buffers.
#define MAX_CMD_BATCH_SIZE (SMR_BUF_BATCH_MAX + SMR_IOV_LIMIT)

struct dsa_bitmap {
	int		size;
	atomic_int	data;
};

struct dsa_cmd_context {
	size_t		bytes_in_progress;
	struct		smr_pend_entry *pend;
	int		batch_size;
	int		index;
};

struct wq_handle {
	union {
		void 	*mmapped;
		int	fd;
	};
};

struct smr_dsa_context {
	struct dsa_hw_desc		dsa_work_desc[MAX_CMD_BATCH_SIZE *
						      CMD_CONTEXT_COUNT];

	struct dsa_completion_record	dsa_work_comp[MAX_CMD_BATCH_SIZE *
						      CMD_CONTEXT_COUNT];

	struct dsa_cmd_context		dsa_cmd_context[CMD_CONTEXT_COUNT];

	struct dsa_bitmap		dsa_bitmap;
	struct wq_handle		wq_handle[MAX_WQS_PER_EP];
	int				wq_count;
	int				next_wq;
	int				(*submit_cmd)(
						struct wq_handle *wq_handle,
						struct dsa_hw_desc *desc);
	void				(*close_wq)(
						struct wq_handle *wq_handle);

	unsigned long			copy_type_stats[2];
	unsigned long			page_fault_stats[2];
};

struct dsa_ops {
	struct accfg_device 	*(*accfg_wq_get_device)(struct accfg_wq *wq);
	int			(*accfg_device_get_cdev_major)(
						struct accfg_device *dev);
	int			(*accfg_wq_get_cdev_minor)(struct accfg_wq *wq);
	int			(*accfg_new)(struct accfg_ctx **ctx);
	enum accfg_device_state	(*accfg_device_get_state)(
						struct accfg_device *device);
	uint64_t		(*accfg_device_get_gen_cap)(
						struct accfg_device *device);
	int			(*accfg_device_get_numa_node)(
						struct accfg_device *device);
	enum accfg_wq_state	(*accfg_wq_get_state)(struct accfg_wq *wq);
	uint64_t		(*accfg_wq_get_max_transfer_size)(
						struct accfg_wq *wq);
	enum accfg_wq_type	(*accfg_wq_get_type)(struct accfg_wq *wq);
	enum accfg_wq_mode	(*accfg_wq_get_mode)(struct accfg_wq *wq);
	const char		*(*accfg_wq_get_devname)(struct accfg_wq *wq);
	struct accfg_ctx	*(*accfg_unref)(struct accfg_ctx *ctx);
	struct accfg_device	*(*accfg_device_get_first)(struct accfg_ctx *ctx);
	struct accfg_device	*(*accfg_device_get_next)(
						struct accfg_device *device);
	struct accfg_wq		*(*accfg_wq_get_first)
						(struct accfg_device *device);
	struct accfg_wq		*(*accfg_wq_get_next)(struct accfg_wq *wq);
};

#define dsa_foreach_device(ctx, device) \
	for (device = dsa_ops.accfg_device_get_first(ctx);	\
	     device != NULL;					\
	     device = dsa_ops.accfg_device_get_next(device))

#define dsa_foreach_wq(device, wq) \
	for (wq = dsa_ops.accfg_wq_get_first(device);	\
	     wq != NULL;				\
	     wq = dsa_ops.accfg_wq_get_next(wq))

static void *libdsa_handle = NULL;
static struct dsa_ops dsa_ops;

static int dsa_enq_cmd(struct wq_handle *wq_handle, struct dsa_hw_desc *desc)
{
	volatile void *reg = (void *) wq_handle->mmapped;
	unsigned char retry;

	asm volatile(".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
		     "setz %0\t\n"
		     : "=r"(retry)
		     : "a"(reg), "d"(desc));
	return (int) retry;
}

static int dsa_write_cmd(struct wq_handle *wq_handle, struct dsa_hw_desc *desc)
{
	int ret;
	int wq_fd = wq_handle->fd;

	ret = write(wq_fd, desc, sizeof(*desc));
	return ret != sizeof(*desc) ? 1 : 0;
}

static __always_inline void dsa_desc_submit(struct smr_dsa_context *dsa_ctx,
					    struct dsa_hw_desc *hw)
{
	int status;

	// make sure writes (e.g., comp.status = 0) are ordered wrt to enqcmd
	{ asm volatile("sfence":::"memory"); }

	do {
		status = dsa_ctx->submit_cmd(
			&dsa_ctx->wq_handle[dsa_ctx->next_wq], hw);
		dsa_ctx->next_wq = (dsa_ctx->next_wq + 1) % (dsa_ctx->wq_count);
	} while (status);
}

static int dsa_open_wq(struct accfg_wq *wq)
{
	struct accfg_device *dev;
	int major, minor, fd;
	char path[1024];

	dev = (*dsa_ops.accfg_wq_get_device)(wq);
	major = (*dsa_ops.accfg_device_get_cdev_major)(dev);
	if (major < 0)
		return -1;
	minor = (*dsa_ops.accfg_wq_get_cdev_minor)(wq);
	if (minor < 0)
		return -1;

	sprintf(path, "/dev/char/%u:%u", major, minor);
	fd = open(path, O_RDWR);
	if (fd < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "dsa_open_wq error\n");
		return -1;
	}

	return fd;
}

static bool dsa_write_is_supported(int fd)
{
	int ret;
	int max_comp_checks = 65536;
	struct dsa_hw_desc desc __attribute__((aligned(64))) = {0};
	struct dsa_completion_record comp __attribute__((aligned(32))) = {0};

	desc.opcode = DSA_OPCODE_NOOP;
	desc.flags = IDXD_OP_FLAG_CRAV | IDXD_OP_FLAG_RCR;
	desc.completion_addr = (unsigned long) &comp;

	// make sure writes (e.g., comp.status = 0) are ordered wrt to write()
	{ asm volatile("sfence":::"memory"); }

	ret = write(fd, &desc, sizeof(desc));

	if (ret != sizeof(desc))
		return false;

	while (comp.status == DSA_COMP_NONE && max_comp_checks > 0) {
		max_comp_checks--;
		_mm_pause();
	}

	return comp.status == DSA_COMP_SUCCESS;
}

static void *dsa_idxd_wq_mmap(struct accfg_wq *wq)
{
	int fd;
	void *wq_reg;

	fd = dsa_open_wq(wq);

	wq_reg = mmap(NULL, LIMITED_MSIX_PORTAL_OFFSET, PROT_WRITE,
		      MAP_SHARED | MAP_POPULATE, fd, 0);
	close(fd);

	if (wq_reg == MAP_FAILED)
		return NULL;

	return wq_reg;
}

static int dsa_idxd_wq_fd(struct accfg_wq *wq, bool check_write_support)
{
	int fd;

	fd = dsa_open_wq(wq);

	if (fd < 0)
		return fd;

	if (check_write_support) {
		if (!dsa_write_is_supported(fd)) {
			close(fd);
			return -1;
		}
	}
	return fd;
}

static void dsa_idxd_wq_unmap(struct wq_handle *wq_handle)
{
	munmap(wq_handle->mmapped, LIMITED_MSIX_PORTAL_OFFSET);
}

static void dsa_idxd_wq_close(struct wq_handle *wq_handle)
{
	close(wq_handle->fd);
}

static int dsa_idxd_init_wq_array(int shared, int numa_node,
				  struct smr_dsa_context *dsa_ctx)
{
	static struct accfg_ctx *ctx;
	struct accfg_wq *wq;
	void *wq_reg;
	enum accfg_device_state dstate;
	enum accfg_wq_state wstate;
	enum accfg_wq_type type;
	int mode, fd, wq_count = 0;
	struct accfg_device *device;
	bool wq_mmap_support = true;
	bool wq_write_support = false;

	if ((*dsa_ops.accfg_new)(&ctx) < 0)
		return 0;

	dsa_foreach_device(ctx, device) {
		dstate = (*dsa_ops.accfg_device_get_state)(device);
		if (dstate != ACCFG_DEVICE_ENABLED)
			continue;

		/* Make sure cache control is supported for memory operations */
		if (((*dsa_ops.accfg_device_get_gen_cap)(device) &
		    GENCAP_CACHE_CTRL_MEM) == 0)
			continue;

		if (numa_node != -1 &&
		    (*dsa_ops.accfg_device_get_numa_node)(device) != numa_node)
			continue;

		dsa_foreach_wq(device, wq) {
			wstate = (*dsa_ops.accfg_wq_get_state)(wq);
			if (wstate != ACCFG_WQ_ENABLED)
				continue;

			if ((*dsa_ops.accfg_wq_get_max_transfer_size)(wq) <
			    SMR_SAR_SIZE)
				continue;

			/* The wq type should be user */
			type = (*dsa_ops.accfg_wq_get_type)(wq);
			if (type != ACCFG_WQT_USER)
				continue;

			mode = (*dsa_ops.accfg_wq_get_mode)(wq);
			if ((mode == ACCFG_WQ_SHARED && !shared) ||
			    (mode == ACCFG_WQ_DEDICATED && shared))
				continue;

			FI_DBG(&smr_prov, FI_LOG_EP_CTRL,
			       "DSA WQ: %s\n",
			       (*dsa_ops.accfg_wq_get_devname)(wq));

			fd = -1;
			wq_reg = NULL;

			if (wq_mmap_support) {
				wq_reg = dsa_idxd_wq_mmap(wq);
				if (!wq_reg && wq_count == 0) {
					wq_mmap_support = false;
					wq_write_support = true;
				} else if (wq_reg != NULL) {
					dsa_ctx->wq_handle[wq_count].mmapped =
									wq_reg;
				}
			}

			if (wq_write_support) {
				fd = dsa_idxd_wq_fd(wq, wq_count == 0);
				if (fd < 0 && wq_count == 0)
					wq_write_support = false;
				else if (fd >= 0)
					dsa_ctx->wq_handle[wq_count].fd = fd;
			}

			if (wq_reg || fd >= 0 ) {
				wq_count++;
				break;
			}

		}

		if (wq_count >= MAX_WQS_PER_EP)
			break;
	}

	if (wq_mmap_support) {
		dsa_ctx->submit_cmd = dsa_enq_cmd;
		dsa_ctx->close_wq = dsa_idxd_wq_unmap;
	} else if (wq_write_support) {
		dsa_ctx->submit_cmd = dsa_write_cmd;
		dsa_ctx->close_wq = dsa_idxd_wq_close;
	} else {
		assert(wq_count == 0);
	}

	(*dsa_ops.accfg_unref)(ctx);
	return wq_count;
}

static int dsa_bitmap_test_and_set_bit(struct dsa_bitmap *bitmap, int index)
{
	assert(index < bitmap->size);
	return atomic_fetch_or(&bitmap->data, 1ULL << index) & (1ULL << index);
}

static void dsa_bitmap_clear_bit(struct dsa_bitmap *bitmap, int index)
{
	assert(index < bitmap->size);
	atomic_fetch_and(&bitmap->data, ~(1ULL << index));
}

static void dsa_bitmap_allocate(struct dsa_bitmap *bitmap, int size)
{
	atomic_init(&bitmap->data, 0);
	bitmap->size = size;
}

static int dsa_bitmap_test_bit(struct dsa_bitmap *bitmap, int index)
{
	assert(index < bitmap->size);
	return atomic_load(&bitmap->data) & (1ULL << index);
}

static bool dsa_bitmap_is_empty(struct dsa_bitmap *bitmap)
{
	return atomic_load(&bitmap->data) == 0;
}

static struct dsa_cmd_context * dsa_alloc_cmd(struct smr_dsa_context *dsa_ctx)
{
	struct dsa_cmd_context *cmd_ctx;
	int i;

	for (i = 0; i < CMD_CONTEXT_COUNT; i++) {
		if (!dsa_bitmap_test_and_set_bit(&dsa_ctx->dsa_bitmap, i))
			break;
	}

	if (i == CMD_CONTEXT_COUNT)
		return NULL;

	cmd_ctx = &dsa_ctx->dsa_cmd_context[i];
	memset(cmd_ctx, 0, sizeof(*cmd_ctx));
	cmd_ctx->index = i;

	return cmd_ctx;
}

static void dsa_free_cmd(struct dsa_cmd_context *cmd_ctx,
			 struct smr_dsa_context *dsa_ctx)
{
	dsa_bitmap_clear_bit(&dsa_ctx->dsa_bitmap, cmd_ctx->index);
}

static struct dsa_hw_desc *dsa_get_desc(struct dsa_cmd_context *cmd_ctx,
					struct smr_dsa_context *dsa_ctx)
{
	return &dsa_ctx->dsa_work_desc[cmd_ctx->index * MAX_CMD_BATCH_SIZE];
}

static struct dsa_hw_desc *dsa_alloc_desc(struct dsa_cmd_context *cmd_ctx,
					  struct smr_dsa_context *dsa_ctx)
{
	struct dsa_hw_desc *free_desc;
	struct dsa_completion_record *free_comp;

	free_desc = &dsa_ctx->dsa_work_desc[cmd_ctx->index *
				MAX_CMD_BATCH_SIZE + cmd_ctx->batch_size];
	free_comp = &dsa_ctx->dsa_work_comp[cmd_ctx->index *
				MAX_CMD_BATCH_SIZE + cmd_ctx->batch_size++];

	memset(free_desc, 0, sizeof(*free_desc));
	memset(free_comp, 0, sizeof(*free_comp));
	free_desc->completion_addr = (uintptr_t) free_comp;

	return free_desc;
}

static struct dsa_completion_record *dsa_get_comp_ptr(
				struct dsa_cmd_context *dsa_cmd_context,
				struct smr_dsa_context *dsa_context)
{
	return &dsa_context->dsa_work_comp[dsa_cmd_context->index *
					   MAX_CMD_BATCH_SIZE];
}

static struct dsa_cmd_context *dsa_get_cmd(struct smr_dsa_context *dsa_context,
					   int index)
{
	if (dsa_bitmap_test_bit(&dsa_context->dsa_bitmap, index))
		return &dsa_context->dsa_cmd_context[index];
	return NULL;
}

static int dsa_is_work_in_progress(struct smr_dsa_context *dsa_context)
{
	return !dsa_bitmap_is_empty(&dsa_context->dsa_bitmap);
}

static void dsa_touch_buffer_pages(struct dsa_hw_desc *desc)
{
	size_t i;
	volatile char *src_addr;
	volatile char *dst_addr;
	size_t page_size = ofi_get_page_size();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

	for (i = 0; i < desc->xfer_size; i += page_size) {
		src_addr = (char *)desc->src_addr + i;
		dst_addr = (char *)desc->dst_addr + i;

		*src_addr;
		*dst_addr = *dst_addr;
	}

	/* Touch last byte in case start of buffer is not aligned to page
	 * boundary
	 */
	src_addr = (char *)desc->src_addr + (desc->xfer_size - 1);
	dst_addr = (char *)desc->dst_addr + (desc->xfer_size - 1);

	*src_addr;
	*dst_addr = *dst_addr;

#pragma GCC diagnostic pop
}

static void dsa_prepare_desc(struct dsa_hw_desc *desc, uint32_t xfer_size,
			     uint64_t src_addr, uint64_t dst_addr)
{
	desc->opcode = DSA_OPCODE_MEMMOVE;
	desc->flags = IDXD_OP_FLAG_CRAV | IDXD_OP_FLAG_RCR | IDXD_OP_FLAG_CC;
	desc->xfer_size = xfer_size;
	desc->src_addr = src_addr;
	desc->dst_addr = dst_addr;
}

ssize_t smr_dsa_copy_sar(struct smr_ep *ep, struct smr_pend_entry *pend)
{
	struct smr_dsa_context *dsa_ctx = ep->dsa_context;
	struct dsa_cmd_context *cmd_ctx;
	struct smr_region *peer_smr;
	struct smr_freestack *sar_pool;
	struct smr_sar_buf *smr_sar_buf;
	size_t remaining_sar_size, remaining_iov_size, iov_len, iov_index = 0;
	size_t iov_offset, sar_offset = 0, cmd_size = 0, dsa_bytes_pending = 0;
	int sar_index = 0, cmd_index = 0;
	char *iov_buf = NULL, *sar_buf = NULL;
	struct dsa_hw_desc *desc = NULL;

	assert(smr_env.use_dsa_sar);

	if (pend->type == SMR_RX_ENTRY) {
		peer_smr = smr_peer_region(ep, pend->cmd->hdr.rx_id);
		if (smr_peer_data(peer_smr)[pend->cmd->hdr.tx_id].sar_status !=
		    SMR_SAR_READY)
			return -FI_EAGAIN;
	}
	cmd_ctx = dsa_alloc_cmd(ep->dsa_context);
	if (!cmd_ctx)
		return -FI_ENOMEM;

	cmd_ctx->pend = pend;

	iov_offset = pend->bytes_done;
	for (iov_index = 0; iov_index < pend->iov_count; iov_index++) {
		iov_len = pend->iov[iov_index].iov_len;

		if (iov_offset < iov_len)
			break;
		iov_offset -= iov_len;
	}

	sar_pool = smr_pend_sar_pool(ep, pend);
	while ((iov_index < pend->iov_count) &&
	       (sar_index < pend->cmd->data.buf_batch_size) &&
	       (cmd_index < MAX_CMD_BATCH_SIZE)) {
		smr_sar_buf = smr_freestack_get_entry_from_index(
				sar_pool, pend->cmd->data.sar[sar_index]);
		iov_len = pend->iov[iov_index].iov_len;

		iov_buf = (char *)pend->iov[iov_index].iov_base + iov_offset;
		sar_buf = (char *)smr_sar_buf->buf + sar_offset;

		remaining_sar_size = SMR_SAR_SIZE - sar_offset;
		remaining_iov_size = iov_len - iov_offset;
		cmd_size = MIN(remaining_iov_size, remaining_sar_size);
		assert(cmd_size > 0);

		desc = dsa_alloc_desc(cmd_ctx, ep->dsa_context);

		if (pend->sar_dir == OFI_COPY_BUF_TO_IOV)
			dsa_prepare_desc(desc, cmd_size, (uintptr_t) sar_buf,
					 (uintptr_t) iov_buf);
		else
			dsa_prepare_desc(desc, cmd_size, (uintptr_t) iov_buf,
					 (uintptr_t) sar_buf);

		dsa_desc_submit(ep->dsa_context, desc);

		cmd_index++;
		dsa_bytes_pending += cmd_size;

		if (remaining_sar_size > remaining_iov_size) {
			iov_index++;
			iov_offset = 0;
			sar_offset += cmd_size;
		} else if (remaining_sar_size < remaining_iov_size) {
			sar_index++;
			sar_offset = 0;
			iov_offset += cmd_size;
		} else {
			iov_index++;
			iov_offset = 0;
			sar_index++;
			sar_offset = 0;
		}
	}
	assert(dsa_bytes_pending > 0);

	cmd_ctx->bytes_in_progress = dsa_bytes_pending;
	dsa_ctx->copy_type_stats[pend->sar_dir]++;

	/* FI_EBUSY indicates command was issued successfully but contents are
	 * not ready yet */
	return -FI_EBUSY;
}

static void dsa_process_partial_copy(struct smr_dsa_context *dsa_ctx,
				     struct dsa_hw_desc *dsa_desc)
{
	uint32_t new_xfer_size, bytes_completed;
	uint64_t new_src_addr, new_dst_addr;
	struct dsa_completion_record *comp =
	    (struct dsa_completion_record *)dsa_desc->completion_addr;

	bytes_completed = comp->bytes_completed;

	/* Update descriptor src & dst buffer based on copy direction
	 * See 8.3.4 of DSA spec
	 */
	new_xfer_size = dsa_desc->xfer_size - bytes_completed;
	new_src_addr = (comp->result ? dsa_desc->src_addr :
				       dsa_desc->src_addr + bytes_completed);
	new_dst_addr = (comp->result ? dsa_desc->dst_addr :
				       dsa_desc->dst_addr + bytes_completed);

	memset(comp, 0, sizeof(*comp));

	dsa_prepare_desc(dsa_desc, new_xfer_size, new_src_addr, new_dst_addr);

	dsa_touch_buffer_pages(dsa_desc);

	dsa_desc_submit(dsa_ctx, dsa_desc);
}

static void dsa_complete_tx_work(struct smr_ep *ep, struct smr_pend_entry *pend)
{
	int ret;

	if (pend->cmd->hdr.op == ofi_op_read_req) {
		if (pend->bytes_done == pend->cmd->hdr.size) {
			ret = smr_complete_tx(ep, pend->comp_ctx, pend->cmd->hdr.op,
					pend->comp_flags);
			if (ret)
				FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"unable to process tx completion\n");

			smr_free_sar_bufs(ep, pend->cmd, pend);

			smr_peer_data(ep->region)[pend->cmd->hdr.tx_id].sar_status =
								SMR_SAR_FREE;
			smr_freestack_push(smr_cmd_stack(ep->region), pend->cmd);
			ofi_buf_free(pend);
			return;
		} else {
			smr_try_send_cmd(ep, pend->cmd);
		}
	}

	smr_peer_data(ep->region)[pend->cmd->hdr.tx_id].sar_status =
							SMR_SAR_READY;
}

static void dsa_complete_rx_work(struct smr_ep *ep, struct smr_pend_entry *pend)
{
	int ret;

	if (pend->bytes_done == pend->cmd->hdr.size) {
		ret = smr_complete_rx(ep, pend->comp_ctx, pend->cmd->hdr.op,
				      pend->comp_flags, pend->bytes_done,
				      pend->iov[0].iov_base,
				      pend->cmd->hdr.rx_id, pend->cmd->hdr.tag,
				      pend->cmd->hdr.cq_data);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process rx completion\n");
		}
		pend->cmd->hdr.rx_ctx = 0;
		if (pend->rx_entry)
			ep->srx->owner_ops->free_entry(pend->rx_entry);
	}
	smr_return_cmd(ep, pend->cmd);
}

static void dsa_process_complete_work(struct smr_ep *ep,
				      struct dsa_cmd_context *cmd_ctx)
{
	cmd_ctx->pend->bytes_done += cmd_ctx->bytes_in_progress;

	if (cmd_ctx->pend->type == SMR_RX_ENTRY)
		dsa_complete_rx_work(ep, cmd_ctx->pend);
	else
		dsa_complete_tx_work(ep, cmd_ctx->pend);

	dsa_free_cmd(cmd_ctx, ep->dsa_context);
}

static inline void dsa_page_fault_debug_info(struct dsa_cmd_context *cmd_ctx,
					     struct dsa_completion_record *comp)
{
	FI_TRACE(&smr_prov, FI_LOG_EP_CTRL,
		"handle_page_fault read_fault %d\
		write_fault %d addr %p dir: %d cmd_idx: %d\n",
		!(comp->status & DSA_COMP_STATUS_WRITE),
		comp->status & DSA_COMP_STATUS_WRITE,
		(void *)comp->fault_addr,
		cmd_ctx->pend->sar_dir, cmd_ctx->index);
}

static bool dsa_check_cmd_status(struct smr_dsa_context *dsa_ctx,
				 struct dsa_cmd_context *cmd_ctx)
{
	int i;
	struct dsa_hw_desc *dsa_work;
	struct dsa_completion_record *comp;
	bool cmd_completed = true;
	uint8_t status = 0;

	dsa_work = dsa_get_desc(cmd_ctx, dsa_ctx);
	comp = dsa_get_comp_ptr(cmd_ctx, dsa_ctx);

	for (i = 0; i < cmd_ctx->batch_size; i++) {
		status = comp[i].status & DSA_COMP_STATUS_MASK;

		switch (status) {
		case DSA_COMP_SUCCESS:
			break;
		case DSA_COMP_NONE:
			cmd_completed = false;
			break;
		case DSA_COMP_PAGE_FAULT_NOBOF:
			dsa_page_fault_debug_info(cmd_ctx, &comp[i]);
			dsa_process_partial_copy(dsa_ctx, &dsa_work[i]);
			dsa_ctx->page_fault_stats[cmd_ctx->pend->sar_dir]++;
			cmd_completed = false;
			break;
		default:
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Unhandled status codes: 0x%x\n", status);
			assert(0);
		}
	}

	return cmd_completed;
}

/* SMR functions */
void smr_dsa_init(void)
{
	libdsa_handle = dlopen("libaccel-config.so", RTLD_NOW);
	if (!libdsa_handle) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libaccel-config.so\n");
		return;
	}

	dsa_ops.accfg_wq_get_device =
			dlsym(libdsa_handle, "accfg_wq_get_device");
	if (!dsa_ops.accfg_wq_get_device) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_device\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_cdev_major =
			dlsym(libdsa_handle, "accfg_device_get_cdev_major");
	if (!dsa_ops.accfg_device_get_cdev_major) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_cdev_major\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_cdev_minor =
			dlsym(libdsa_handle, "accfg_wq_get_cdev_minor");
	if (!dsa_ops.accfg_wq_get_cdev_minor) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_cdev_minor\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_new = dlsym(libdsa_handle, "accfg_new");
	if (!dsa_ops.accfg_new) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find accfg_new\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_state =
			dlsym(libdsa_handle, "accfg_device_get_state");
	if (!dsa_ops.accfg_device_get_state) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_state\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_gen_cap =
			dlsym(libdsa_handle, "accfg_device_get_gen_cap");
	if (!dsa_ops.accfg_device_get_gen_cap) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_gen_cap\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_numa_node =
			dlsym(libdsa_handle, "accfg_device_get_numa_node");
	if (!dsa_ops.accfg_device_get_numa_node) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_numa_node\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_state = dlsym(libdsa_handle, "accfg_wq_get_state");
	if (!dsa_ops.accfg_wq_get_state) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_state\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_max_transfer_size =
			dlsym(libdsa_handle, "accfg_wq_get_max_transfer_size");
	if (!dsa_ops.accfg_wq_get_max_transfer_size) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_max_transfer_size\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_type = dlsym(libdsa_handle, "accfg_wq_get_type");
	if (!dsa_ops.accfg_wq_get_type) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_type\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_mode = dlsym(libdsa_handle, "accfg_wq_get_mode");
	if (!dsa_ops.accfg_wq_get_mode) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_mode\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_devname =
			dlsym(libdsa_handle, "accfg_wq_get_devname");
	if (!dsa_ops.accfg_wq_get_devname) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_devname\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_unref = dlsym(libdsa_handle, "accfg_unref");
	if (!dsa_ops.accfg_unref) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_unref\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_first = dlsym(libdsa_handle, "accfg_wq_get_first");
	if (!dsa_ops.accfg_wq_get_first) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_first\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_next = dlsym(libdsa_handle, "accfg_wq_get_next");
	if (!dsa_ops.accfg_wq_get_next) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_next\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_first =
			dlsym(libdsa_handle, "accfg_device_get_first");
	if (!dsa_ops.accfg_device_get_first) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_first\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_next =
			dlsym(libdsa_handle, "accfg_device_get_next");
	if (!dsa_ops.accfg_device_get_next) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_next\n");
		goto err_dlclose;
	}

	return;

err_dlclose:
	dlclose(libdsa_handle);
	libdsa_handle = NULL;
}

void smr_dsa_cleanup(void)
{
	if (libdsa_handle)
		dlclose(libdsa_handle);
}

void smr_dsa_context_init(struct smr_ep *ep)
{
	int i, cpu;
	int wq_count;
	struct smr_dsa_context *dsa_context;
	unsigned int numa_node;

	cpu = sched_getcpu();
	numa_node = numa_node_of_cpu(cpu);

	ep->dsa_context = aligned_alloc(sizeof(struct dsa_hw_desc),
					sizeof(*dsa_context));

	if (!ep->dsa_context) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"aligned_alloc failed for dsa_context\n");
		goto alloc_error;
	}

	dsa_context = ep->dsa_context;
	memset(dsa_context, 0, sizeof(*dsa_context));

	wq_count = dsa_idxd_init_wq_array(1, numa_node, dsa_context);

	if (wq_count == 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"error: wq mmap and wq write not supported\n");
		goto wq_get_error;
	}

	dsa_bitmap_allocate(&dsa_context->dsa_bitmap, CMD_CONTEXT_COUNT);

	for (i = 0; i < CMD_CONTEXT_COUNT; i++)
		dsa_context->dsa_cmd_context[i].index = -1;

	dsa_context->next_wq = 0;
	dsa_context->wq_count = wq_count;

	FI_DBG(&smr_prov, FI_LOG_EP_CTRL, "Numa node of endpoint CPU: %d\n",
	       numa_node);
	return;

wq_get_error:
	free(dsa_context);
alloc_error:
	smr_env.use_dsa_sar = 0;
}

void smr_dsa_context_cleanup(struct smr_ep *ep)
{
	struct smr_dsa_context *dsa_context = ep->dsa_context;
	int i;

	if (!dsa_context)
		return;

	FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "Stats:\n\
		User to Sar Buffer: dsa cmds %ld page faults %ld\n\
		Sar Buffer to User: dsa cmds %ld page faults %ld\n",
		dsa_context->copy_type_stats[OFI_COPY_IOV_TO_BUF],
		dsa_context->page_fault_stats[OFI_COPY_IOV_TO_BUF],
		dsa_context->copy_type_stats[OFI_COPY_BUF_TO_IOV],
		dsa_context->page_fault_stats[OFI_COPY_BUF_TO_IOV]);

	if (!dsa_bitmap_is_empty(&dsa_context->dsa_bitmap))
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Warning: TBD outstanding DSA commands while "
			"doing cleanup\n");

	for (i = 0; i < dsa_context->wq_count; i++)
		dsa_context->close_wq(&dsa_context->wq_handle[i]);

	free(ep->dsa_context);
}

void smr_dsa_progress(struct smr_ep *ep)
{
	int index;
	struct dsa_cmd_context *cmd_ctx;
	bool dsa_cmd_completed;
	struct smr_dsa_context *dsa_context = ep->dsa_context;

	if (!dsa_is_work_in_progress(ep->dsa_context))
		return;

	for (index = 0; index < CMD_CONTEXT_COUNT; index++) {
		cmd_ctx = dsa_get_cmd(dsa_context, index);

		if (!cmd_ctx)
			continue;

		dsa_cmd_completed = dsa_check_cmd_status(dsa_context, cmd_ctx);

		if (dsa_cmd_completed)
			dsa_process_complete_work(ep, cmd_ctx);
	}
}

#else

void smr_dsa_init(void) {}
void smr_dsa_cleanup(void) {}

ssize_t smr_dsa_copy_sar(struct smr_ep *ep, struct smr_pend_entry *pend)
{
	return -FI_ENOSYS;
}

void smr_dsa_context_init(struct smr_ep *ep)
{
	smr_env.use_dsa_sar = 0;
	return;
}

void smr_dsa_context_cleanup(struct smr_ep *ep) {}

void smr_dsa_progress(struct smr_ep *ep) {}

#endif /* SHM_HAVE_DSA */