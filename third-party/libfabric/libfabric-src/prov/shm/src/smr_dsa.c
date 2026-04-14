/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include "smr.h"

#if SHM_HAVE_DSA

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdatomic.h>
#include <sys/un.h>
#include <accel-config/libaccel_config.h>
#include <linux/idxd.h>
#include <numa.h>
#include <immintrin.h> // _mm_pause
#include "smr_util.h"
#include "smr_dsa.h"

#define MAX_WQS_PER_EP 4
#define GENCAP_CACHE_CTRL_MEM 0x4
#define LIMITED_MSIX_PORTAL_OFFSET 0x1000

#define CMD_CONTEXT_COUNT 32

// Number of DSA descriptors must be large enough to fill the
// maximum number of SAR buffers.
#define MAX_CMD_BATCH_SIZE (SMR_BUF_BATCH_MAX + SMR_IOV_LIMIT)

struct dsa_bitmap {
	int size;
	atomic_int data;
};

struct dsa_cmd_context {
	size_t bytes_in_progress;
	int index;
	int batch_size;
	int dir;
	uint32_t op;
	// We keep track of the entry type to know which lock to acquire
	// when we need to do the updates after completion
	void *entry_ptr;
};

struct wq_handle {
	union {
		void *mmapped;
		int fd;
	};
};

struct smr_dsa_context {
	struct dsa_hw_desc dsa_work_desc[MAX_CMD_BATCH_SIZE *
					 CMD_CONTEXT_COUNT];

	struct dsa_completion_record dsa_work_comp[MAX_CMD_BATCH_SIZE *
						   CMD_CONTEXT_COUNT];

	struct dsa_cmd_context dsa_cmd_context[CMD_CONTEXT_COUNT];

	struct dsa_bitmap dsa_bitmap;
	struct wq_handle wq_handle[MAX_WQS_PER_EP];
	int wq_count;
	int next_wq;
	int (*submit_cmd)(struct wq_handle *wq_handle, struct dsa_hw_desc *desc);
	void (*close_wq)(struct wq_handle *wq_handle);

	unsigned long copy_type_stats[2];
	unsigned long page_fault_stats[2];
};

struct dsa_ops {
	struct accfg_device *(*accfg_wq_get_device)(struct accfg_wq *wq);
	int (*accfg_device_get_cdev_major)(struct accfg_device *dev);
	int (*accfg_wq_get_cdev_minor)(struct accfg_wq *wq);
	int (*accfg_new)(struct accfg_ctx **ctx);
	enum accfg_device_state (*accfg_device_get_state)(struct accfg_device *device);
	uint64_t (*accfg_device_get_gen_cap)(struct accfg_device *device);
	int (*accfg_device_get_numa_node)(struct accfg_device *device);
	enum accfg_wq_state (*accfg_wq_get_state)(struct accfg_wq *wq);
	uint64_t (*accfg_wq_get_max_transfer_size)(struct accfg_wq *wq);
	enum accfg_wq_type (*accfg_wq_get_type)(struct accfg_wq *wq);
	enum accfg_wq_mode (*accfg_wq_get_mode)(struct accfg_wq *wq);
	const char *(*accfg_wq_get_devname)(struct accfg_wq *wq);
	struct accfg_ctx *(*accfg_unref)(struct accfg_ctx *ctx);

	struct accfg_device *(*accfg_device_get_first)(struct accfg_ctx *ctx);
	struct accfg_device *(*accfg_device_get_next)(struct accfg_device *device);
	struct accfg_wq *(*accfg_wq_get_first)(struct accfg_device *device);
	struct accfg_wq *(*accfg_wq_get_next)(struct accfg_wq *wq);
};

#define dsa_foreach_device(ctx, device) \
	for (device = dsa_ops.accfg_device_get_first(ctx); \
	     device != NULL; \
	     device = dsa_ops.accfg_device_get_next(device))


#define dsa_foreach_wq(device, wq) \
	for (wq = dsa_ops.accfg_wq_get_first(device); \
	     wq != NULL; \
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

static __always_inline void dsa_desc_submit(struct smr_dsa_context *dsa_context,
					    struct dsa_hw_desc *hw)
{
	int status;

	// make sure writes (e.g., comp.status = 0) are ordered wrt to enqcmd
	{ asm volatile("sfence":::"memory"); }

	do {
		status = dsa_context->submit_cmd(
			&dsa_context->wq_handle[dsa_context->next_wq], hw);
		dsa_context->next_wq =
			(dsa_context->next_wq + 1) % (dsa_context->wq_count);
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
				  struct smr_dsa_context *dsa_context)
{
	static struct accfg_ctx *ctx;
	struct accfg_wq *wq;
	void *wq_reg;
	int fd;
	enum accfg_device_state dstate;
	enum accfg_wq_state wstate;
	enum accfg_wq_type type;
	int mode;
	int wq_count = 0;
	struct accfg_device *device;
	bool wq_mmap_support = true;
	bool wq_write_support = false;

	if ((*dsa_ops.accfg_new)(&ctx) < 0)
		return 0;

	dsa_foreach_device(ctx, device) {
		/* Make sure that the device is enabled */
		dstate = (*dsa_ops.accfg_device_get_state)(device);
		if (dstate != ACCFG_DEVICE_ENABLED)
			continue;

		/* Make sure cache control is supported for memory operations */
		if (((*dsa_ops.accfg_device_get_gen_cap)(device) &
		    GENCAP_CACHE_CTRL_MEM) == 0)
			continue;

		/* Match the device to the id requested */
		if (numa_node != -1 &&
		    (*dsa_ops.accfg_device_get_numa_node)(device) != numa_node)
			continue;

		dsa_foreach_wq(device, wq)
		{
			/* Get a workqueue that's enabled */
			wstate = (*dsa_ops.accfg_wq_get_state)(wq);
			if (wstate != ACCFG_WQ_ENABLED)
				continue;

			if ((*dsa_ops.accfg_wq_get_max_transfer_size)(wq) < SMR_SAR_SIZE)
				continue;

			/* The wq type should be user */
			type = (*dsa_ops.accfg_wq_get_type)(wq);
			if (type != ACCFG_WQT_USER)
				continue;

			/* Make sure the mode is correct */
			mode = (*dsa_ops.accfg_wq_get_mode)(wq);
			if ((mode == ACCFG_WQ_SHARED && !shared) ||
			    (mode == ACCFG_WQ_DEDICATED && shared))
				continue;

			/* This is a candidate wq */
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
					dsa_context->wq_handle[wq_count].mmapped = wq_reg;
				}
			}

			if (wq_write_support) {
				fd = dsa_idxd_wq_fd(wq, wq_count == 0);
				if (fd < 0 && wq_count == 0)
					wq_write_support = false;
				else if (fd >= 0)
					dsa_context->wq_handle[wq_count].fd = fd;
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
		dsa_context->submit_cmd = dsa_enq_cmd;
		dsa_context->close_wq = dsa_idxd_wq_unmap;
	} else if (wq_write_support) {
		dsa_context->submit_cmd = dsa_write_cmd;
		dsa_context->close_wq = dsa_idxd_wq_close;
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

static int dsa_bitmap_allocate(struct dsa_bitmap *bitmap, int size)
{
	atomic_init(&bitmap->data, 0);
	bitmap->size = size;

	return 1;
}

static int dsa_bitmap_test_bit(struct dsa_bitmap *bitmap, int index)
{
	assert(index < bitmap->size);
	return atomic_load(&bitmap->data) & (1ULL << index);
}

static int dsa_bitmap_is_empty(struct dsa_bitmap *bitmap)
{
	return atomic_load(&bitmap->data) == 0;
}

static struct dsa_cmd_context *
dsa_allocate_cmd_context(struct smr_dsa_context *smr_dsa_context)
{
	struct dsa_cmd_context *dsa_cmd_context;
	int i;

	for (i = 0; i < CMD_CONTEXT_COUNT; i++) {
		if (!dsa_bitmap_test_and_set_bit(&smr_dsa_context->dsa_bitmap, i))
			break;
	}

	if (i == CMD_CONTEXT_COUNT)
		return NULL;

	dsa_cmd_context = &smr_dsa_context->dsa_cmd_context[i];
	memset(dsa_cmd_context, 0, sizeof(*dsa_cmd_context));
	dsa_cmd_context->index = i;

	return dsa_cmd_context;
}

static void dsa_free_cmd_context(struct dsa_cmd_context *dsa_cmd_context,
				 struct smr_dsa_context *smr_dsa_context)
{
	dsa_bitmap_clear_bit(&smr_dsa_context->dsa_bitmap,
			     dsa_cmd_context->index);
}

static struct dsa_hw_desc *
dsa_get_work_descriptor_array_ptr(struct dsa_cmd_context *dsa_cmd_context,
				  struct smr_dsa_context *dsa_context)
{
	return &dsa_context->dsa_work_desc[dsa_cmd_context->index *
					   MAX_CMD_BATCH_SIZE];
}

static struct dsa_hw_desc *
dsa_get_free_work_descriptor(struct dsa_cmd_context *dsa_cmd_context,
			     struct smr_dsa_context *dsa_context)
{
	struct dsa_hw_desc *free_desc;
	struct dsa_completion_record *free_comp;

	free_desc = &dsa_context->dsa_work_desc[dsa_cmd_context->index *
		MAX_CMD_BATCH_SIZE + dsa_cmd_context->batch_size];
	free_comp = &dsa_context->dsa_work_comp[dsa_cmd_context->index *
		MAX_CMD_BATCH_SIZE + dsa_cmd_context->batch_size++];

	memset(free_desc, 0, sizeof(*free_desc));
	memset(free_comp, 0, sizeof(*free_comp));
	free_desc->completion_addr = (uintptr_t) free_comp;

	return free_desc;
}

static struct dsa_completion_record *
dsa_get_work_completion_array_ptr(struct dsa_cmd_context *dsa_cmd_context,
				  struct smr_dsa_context *dsa_context)
{
	return &dsa_context->dsa_work_comp[dsa_cmd_context->index *
					   MAX_CMD_BATCH_SIZE];
}

static struct dsa_cmd_context *dsa_get_cmd_context(struct smr_dsa_context
						  *dsa_context, int index)
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

	// Touch last byte in case start of buffer is not aligned to page
	// boundary
	src_addr = (char *)desc->src_addr + (desc->xfer_size - 1);
	dst_addr = (char *)desc->dst_addr + (desc->xfer_size - 1);

	*src_addr;
	*dst_addr = *dst_addr;

#pragma GCC diagnostic pop
}

static void dsa_prepare_copy_desc(struct dsa_hw_desc *desc,
				  uint32_t xfer_size, uint64_t src_addr,
				  uint64_t dst_addr)
{
	desc->opcode = DSA_OPCODE_MEMMOVE;
	desc->flags = IDXD_OP_FLAG_CRAV | IDXD_OP_FLAG_RCR | IDXD_OP_FLAG_CC;
	desc->xfer_size = xfer_size;
	desc->src_addr = src_addr;
	desc->dst_addr = dst_addr;
}

static void smr_dsa_copy_sar(struct smr_freestack *sar_pool,
			struct smr_dsa_context *dsa_context,
			struct dsa_cmd_context *dsa_cmd_context,
			struct smr_resp *resp, struct smr_cmd *cmd,
			const struct iovec *iov, size_t count,
			size_t *bytes_done, struct smr_region *region)
{
	struct smr_sar_buf *smr_sar_buf;
	size_t remaining_sar_size;
	size_t remaining_iov_size;
	size_t iov_len;
	size_t iov_index = 0;
	int sar_index = 0;
	int cmd_index = 0;
	size_t iov_offset = *bytes_done;
	size_t sar_offset = 0;
	size_t cmd_size = 0;
	char *iov_buf = NULL;
	char *sar_buf = NULL;
	struct dsa_hw_desc *desc = NULL;
	size_t dsa_bytes_pending = 0;

	for (iov_index = 0; iov_index < count; iov_index++) {
		iov_len = iov[iov_index].iov_len;

		if (iov_offset < iov_len)
			break;
		iov_offset -= iov_len;
	}

	while ((iov_index < count) &&
	       (sar_index < cmd->msg.data.buf_batch_size) &&
	       (cmd_index < MAX_CMD_BATCH_SIZE)) {
		smr_sar_buf = smr_freestack_get_entry_from_index(
		    sar_pool, cmd->msg.data.sar[sar_index]);
		iov_len = iov[iov_index].iov_len;

		iov_buf = (char *)iov[iov_index].iov_base + iov_offset;
		sar_buf = (char *)smr_sar_buf->buf + sar_offset;

		remaining_sar_size = SMR_SAR_SIZE - sar_offset;
		remaining_iov_size = iov_len - iov_offset;
		cmd_size = MIN(remaining_iov_size, remaining_sar_size);
		assert(cmd_size > 0);

		desc = dsa_get_free_work_descriptor(dsa_cmd_context,
						    dsa_context);

		if (dsa_cmd_context->dir == OFI_COPY_BUF_TO_IOV)
			dsa_prepare_copy_desc(desc, cmd_size, (uintptr_t)
					sar_buf, (uintptr_t) iov_buf);
		else
			dsa_prepare_copy_desc(desc, cmd_size, (uintptr_t)
					iov_buf, (uintptr_t) sar_buf);

		dsa_desc_submit(dsa_context, desc);

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

	resp->status = SMR_STATUS_BUSY;

	dsa_cmd_context->bytes_in_progress = dsa_bytes_pending;
	dsa_context->copy_type_stats[dsa_cmd_context->dir]++;
	dsa_cmd_context->op = cmd->msg.hdr.op;
}


static void
dsa_process_partially_completed_desc(struct smr_dsa_context *dsa_context,
				     struct dsa_hw_desc *dsa_descriptor)
{
	uint32_t new_xfer_size;
	uint64_t new_src_addr;
	uint64_t new_dst_addr;
	uint32_t bytes_completed;
	struct dsa_completion_record *comp =
	    (struct dsa_completion_record *)dsa_descriptor->completion_addr;

	bytes_completed = comp->bytes_completed;

	// Update descriptor src & dst buffer based on copy direction; see 8.3.4
	// of DSA spec
	new_xfer_size = dsa_descriptor->xfer_size - bytes_completed;
	new_src_addr =
	    (comp->result ? dsa_descriptor->src_addr
			  : dsa_descriptor->src_addr + bytes_completed);
	new_dst_addr =
	    (comp->result ? dsa_descriptor->dst_addr
			  : dsa_descriptor->dst_addr + bytes_completed);

	// Reset completion record.
	memset(comp, 0, sizeof(*comp));

	dsa_prepare_copy_desc(dsa_descriptor, new_xfer_size,
			       new_src_addr, new_dst_addr);

	dsa_touch_buffer_pages(dsa_descriptor);

	dsa_desc_submit(dsa_context, dsa_descriptor);
}

static void dsa_update_tx_entry(struct smr_region *smr,
				struct dsa_cmd_context *dsa_cmd_context)
{
	struct smr_resp *resp;
	struct smr_cmd *cmd;
	struct smr_tx_entry *tx_entry = dsa_cmd_context->entry_ptr;

	tx_entry->bytes_done += dsa_cmd_context->bytes_in_progress;
	cmd = &tx_entry->cmd;
	resp = smr_get_ptr(smr, cmd->msg.hdr.src_data);

	assert(resp->status == SMR_STATUS_BUSY);
	resp->status = (dsa_cmd_context->dir == OFI_COPY_IOV_TO_BUF ?
			SMR_STATUS_SAR_FULL : SMR_STATUS_SAR_EMPTY);
}

static void dsa_update_sar_entry(struct smr_region *smr,
				 struct dsa_cmd_context *dsa_cmd_context)
{
	struct smr_pend_entry *sar_entry = dsa_cmd_context->entry_ptr;
	struct smr_region *peer_smr;
	struct smr_resp *resp;
	struct smr_cmd *cmd;

	sar_entry->bytes_done += dsa_cmd_context->bytes_in_progress;
	cmd = &sar_entry->cmd;
	peer_smr = smr_peer_region(smr, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	assert(resp->status == SMR_STATUS_BUSY);
	resp->status = (dsa_cmd_context->dir == OFI_COPY_IOV_TO_BUF ?
			SMR_STATUS_SAR_FULL : SMR_STATUS_SAR_EMPTY);
}

static void dsa_process_complete_work(struct smr_region *smr,
				      struct dsa_cmd_context *dsa_cmd_context,
				      struct smr_dsa_context *dsa_context)
{
	if (dsa_cmd_context->op == ofi_op_read_req) {
		if (dsa_cmd_context->dir == OFI_COPY_BUF_TO_IOV)
			dsa_update_tx_entry(smr, dsa_cmd_context);
		else
			dsa_update_sar_entry(smr, dsa_cmd_context);
	} else {
		if (dsa_cmd_context->dir == OFI_COPY_IOV_TO_BUF)
			dsa_update_tx_entry(smr, dsa_cmd_context);
		else
			dsa_update_sar_entry(smr, dsa_cmd_context);
	}

	dsa_free_cmd_context(dsa_cmd_context, dsa_context);
}

static inline void
dsa_page_fault_debug_info(struct dsa_cmd_context *dsa_cmd_context,
			  struct dsa_completion_record *dsa_work_comp)
{
	FI_TRACE(
		&smr_prov, FI_LOG_EP_CTRL,
		"handle_page_fault read_fault %d\
		write_fault %d addr %p dir: %d cmd_idx: %d\n",
		!(dsa_work_comp->status & DSA_COMP_STATUS_WRITE),
		dsa_work_comp->status & DSA_COMP_STATUS_WRITE,
		(void *)dsa_work_comp->fault_addr,
		dsa_cmd_context->dir, dsa_cmd_context->index);
}

static bool dsa_check_cmd_status(struct smr_dsa_context *dsa_context,
				struct dsa_cmd_context *dsa_cmd_context)
{
	int i;
	struct dsa_hw_desc *dsa_work;
	struct dsa_completion_record *dsa_work_comp;
	bool dsa_cmd_completed = true;
	uint8_t status_value = 0;
	dsa_work = dsa_get_work_descriptor_array_ptr(dsa_cmd_context,
						     dsa_context);
	dsa_work_comp =
	    dsa_get_work_completion_array_ptr(dsa_cmd_context, dsa_context);

	for (i = 0; i < dsa_cmd_context->batch_size; i++) {
		status_value = dsa_work_comp[i].status & DSA_COMP_STATUS_MASK;

		switch (status_value) {
		case DSA_COMP_SUCCESS:
			break;
		case DSA_COMP_NONE:
			dsa_cmd_completed = false;
			break;
		case DSA_COMP_PAGE_FAULT_NOBOF:
			dsa_page_fault_debug_info(dsa_cmd_context,
					&dsa_work_comp[i]);
			dsa_process_partially_completed_desc(dsa_context,
					&dsa_work[i]);
			dsa_context->page_fault_stats[dsa_cmd_context->dir]++;
			dsa_cmd_completed = false;
			break;
		default:
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"Unhandled status codes: 0x%x\n",
					status_value);
			assert(0);
		}
	}

	return dsa_cmd_completed;
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

	dsa_ops.accfg_wq_get_device = dlsym(libdsa_handle,
					    "accfg_wq_get_device");
	if (!dsa_ops.accfg_wq_get_device) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_device\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_cdev_major = dlsym(libdsa_handle,
					      "accfg_device_get_cdev_major");
	if (!dsa_ops.accfg_device_get_cdev_major) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_cdev_major\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_wq_get_cdev_minor = dlsym(libdsa_handle,
					  "accfg_wq_get_cdev_minor");
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


	dsa_ops.accfg_device_get_state = dlsym(libdsa_handle,
					       "accfg_device_get_state");
	if (!dsa_ops.accfg_device_get_state) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_state\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_gen_cap = dlsym(libdsa_handle,
						 "accfg_device_get_gen_cap");
	if (!dsa_ops.accfg_device_get_gen_cap) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_gen_cap\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_numa_node = dlsym(libdsa_handle,
						   "accfg_device_get_numa_node");
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

	dsa_ops.accfg_wq_get_max_transfer_size = dlsym(libdsa_handle,
					"accfg_wq_get_max_transfer_size");
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

	dsa_ops.accfg_wq_get_devname = dlsym(libdsa_handle,
					     "accfg_wq_get_devname");
	if (!dsa_ops.accfg_wq_get_devname) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_wq_get_devname\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_unref = dlsym(libdsa_handle, "accfg_unref");
	if (!dsa_ops.accfg_unref) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find accfg_unref\n");
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

	dsa_ops.accfg_device_get_first = dlsym(libdsa_handle,
					       "accfg_device_get_first");
	if (!dsa_ops.accfg_device_get_first) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find accfg_device_get_first\n");
		goto err_dlclose;
	}

	dsa_ops.accfg_device_get_next = dlsym(libdsa_handle,
					      "accfg_device_get_next");
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
	struct dsa_cmd_context *dsa_cmd_context;
	bool dsa_cmd_completed;
	struct smr_dsa_context *dsa_context = ep->dsa_context;

	if (!dsa_is_work_in_progress(ep->dsa_context))
		return;

	pthread_spin_lock(&ep->region->lock);
	for (index = 0; index < CMD_CONTEXT_COUNT; index++) {
		dsa_cmd_context = dsa_get_cmd_context(dsa_context, index);

		if (!dsa_cmd_context)
			continue;

		dsa_cmd_completed = dsa_check_cmd_status(dsa_context,
				dsa_cmd_context);

		if (dsa_cmd_completed)
			dsa_process_complete_work(ep->region, dsa_cmd_context,
					      dsa_context);
	}
	pthread_spin_unlock(&ep->region->lock);
}

size_t smr_dsa_copy_to_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr)
{
	struct dsa_cmd_context *dsa_cmd_context;

	assert(smr_env.use_dsa_sar);

	if (resp->status != SMR_STATUS_SAR_EMPTY)
		return -FI_EAGAIN;

	dsa_cmd_context = dsa_allocate_cmd_context(ep->dsa_context);
	if (!dsa_cmd_context)
		return -FI_ENOMEM;

	dsa_cmd_context->dir = OFI_COPY_IOV_TO_BUF;
	dsa_cmd_context->entry_ptr = entry_ptr;
	smr_dsa_copy_sar(sar_pool, ep->dsa_context, dsa_cmd_context, resp,
			 cmd, iov, count, bytes_done, ep->region);

	return FI_SUCCESS;
}

size_t smr_dsa_copy_from_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr)
{
	struct dsa_cmd_context *dsa_cmd_context;

	assert(smr_env.use_dsa_sar);

	if (resp->status != SMR_STATUS_SAR_FULL)
		return FI_EAGAIN;

	dsa_cmd_context = dsa_allocate_cmd_context(ep->dsa_context);
	if (!dsa_cmd_context)
		return -FI_ENOMEM;

	dsa_cmd_context->dir = OFI_COPY_BUF_TO_IOV;
	dsa_cmd_context->entry_ptr = entry_ptr;
	smr_dsa_copy_sar(sar_pool, ep->dsa_context, dsa_cmd_context, resp,
			 cmd, iov, count, bytes_done, ep->region);

	return FI_SUCCESS;
}

#else

void smr_dsa_init(void) {}
void smr_dsa_cleanup(void) {}

size_t smr_dsa_copy_to_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr)
{
	return -FI_ENOSYS;
}

size_t smr_dsa_copy_from_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr)
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
