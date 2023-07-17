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
#include "ofi_shm.h"
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

struct smr_dsa_context {
	struct dsa_hw_desc dsa_work_desc[MAX_CMD_BATCH_SIZE *
					 CMD_CONTEXT_COUNT];

	struct dsa_completion_record dsa_work_comp[MAX_CMD_BATCH_SIZE *
						   CMD_CONTEXT_COUNT];

	struct dsa_cmd_context dsa_cmd_context[CMD_CONTEXT_COUNT];

	struct dsa_bitmap dsa_bitmap;
	void *wq_portal[MAX_WQS_PER_EP];
	int wq_count;
	int next_wq;
	int enable_dsa_page_touch;

	unsigned long copy_type_stats[2];
	unsigned long page_fault_stats[2];
};


static inline unsigned char dsa_enqcmd(struct dsa_hw_desc *desc,
				       volatile void *reg)
{
	unsigned char retry;

	asm volatile(".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
		     "setz %0\t\n"
		     : "=r"(retry)
		     : "a"(reg), "d"(desc));
	return retry;
}

static __always_inline void dsa_desc_submit(struct smr_dsa_context *dsa_context,
					    struct dsa_hw_desc *hw)
{
	int enq_status;

	// make sure writes (e.g., comp.status = 0) are ordered wrt to enqcmd
	{ asm volatile("sfence":::"memory"); }

	do {
		enq_status = dsa_enqcmd(hw,
				dsa_context->wq_portal[dsa_context->next_wq]);
		dsa_context->next_wq = (dsa_context->next_wq + 1) %
			(dsa_context->wq_count);
	} while (enq_status);
}

static int dsa_open_wq(struct accfg_wq *wq)
{
	struct accfg_device *dev;
	int major, minor, fd;
	char path[1024];

	dev = accfg_wq_get_device(wq);
	major = accfg_device_get_cdev_major(dev);
	if (major < 0)
		return -1;
	minor = accfg_wq_get_cdev_minor(wq);
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

static void *dsa_idxd_wq_mmap(struct accfg_wq *wq)
{
	int fd;
	void *wq_reg;

	fd = dsa_open_wq(wq);

	wq_reg = mmap(NULL, LIMITED_MSIX_PORTAL_OFFSET, PROT_WRITE,
		      MAP_SHARED | MAP_POPULATE, fd, 0);
	close(fd);

	if (wq_reg == MAP_FAILED) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "d_idxd_wq_mmap error\n");
		return NULL;
	}

	return wq_reg;
}

static void dsa_idxd_wq_unmap(void *wq)
{
	munmap(wq, LIMITED_MSIX_PORTAL_OFFSET);
}

static int dsa_idxd_init_wq_array(int shared, int numa_node,
				  void **wq_array)
{
	static struct accfg_ctx *ctx;
	struct accfg_wq *wq;
	void *wq_reg;
	enum accfg_device_state dstate;
	enum accfg_wq_state wstate;
	enum accfg_wq_type type;
	int mode;
	int wq_count = 0;
	struct accfg_device *device;

	if (accfg_new(&ctx) < 0)
		return 0;

	accfg_device_foreach(ctx, device) {
		/* Make sure that the device is enabled */
		dstate = accfg_device_get_state(device);
		if (dstate != ACCFG_DEVICE_ENABLED)
			continue;

		/* Make sure cache control is supported for memory operations */
		if ((accfg_device_get_gen_cap(device) &
		    GENCAP_CACHE_CTRL_MEM) == 0)
			continue;

		/* Match the device to the id requested */
		if (numa_node != -1 &&
		    accfg_device_get_numa_node(device) != numa_node)
			continue;

		accfg_wq_foreach(device, wq)
		{
			/* Get a workqueue that's enabled */
			wstate = accfg_wq_get_state(wq);
			if (wstate != ACCFG_WQ_ENABLED)
				continue;

			if (accfg_wq_get_max_transfer_size(wq) < SMR_SAR_SIZE)
				continue;

			/* The wq type should be user */
			type = accfg_wq_get_type(wq);
			if (type != ACCFG_WQT_USER)
				continue;

			/* Make sure the mode is correct */
			mode = accfg_wq_get_mode(wq);
			if ((mode == ACCFG_WQ_SHARED && !shared) ||
			    (mode == ACCFG_WQ_DEDICATED && shared))
				continue;

			/* This is a candidate wq */
			FI_DBG(&smr_prov, FI_LOG_EP_CTRL,
					"DSA WQ: %s\n",
					accfg_wq_get_devname(wq));

			wq_reg = dsa_idxd_wq_mmap(wq);
			if (wq_reg == NULL)
				continue;
			wq_array[wq_count] = wq_reg;
			wq_count++;
			break;
		}

		if (wq_count >= MAX_WQS_PER_EP)
			break;
	}

	accfg_unref(ctx);
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

	smr_signal(region);
}

static void dsa_handle_page_fault(struct dsa_completion_record *comp)
{
	volatile char *fault_addr;

	if (comp->status & DSA_COMP_PAGE_FAULT_NOBOF) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		fault_addr = (char *)comp->fault_addr;

		if (comp->status & DSA_COMP_STATUS_WRITE)
			*fault_addr = *fault_addr;
		else
			*fault_addr;
#pragma GCC diagnostic pop
	}
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

	dsa_handle_page_fault(comp);

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

	if (dsa_context->enable_dsa_page_touch)
		dsa_touch_buffer_pages(dsa_descriptor);

	dsa_desc_submit(dsa_context, dsa_descriptor);
}

static void dsa_update_tx_entry(struct smr_region *smr,
				struct dsa_cmd_context *dsa_cmd_context)
{
	struct smr_region *peer_smr;
	struct smr_resp *resp;
	struct smr_cmd *cmd;
	struct smr_tx_entry *tx_entry = dsa_cmd_context->entry_ptr;

	tx_entry->bytes_done += dsa_cmd_context->bytes_in_progress;
	cmd = &tx_entry->cmd;
	peer_smr = smr_peer_region(smr, tx_entry->peer_id);
	resp = smr_get_ptr(smr, cmd->msg.hdr.src_data);

	assert(resp->status == SMR_STATUS_BUSY);
	resp->status = (dsa_cmd_context->dir == OFI_COPY_IOV_TO_BUF ?
			SMR_STATUS_SAR_READY : SMR_STATUS_SAR_FREE);
	smr_signal(peer_smr);
}

static void dsa_update_sar_entry(struct smr_region *smr,
				 struct dsa_cmd_context *dsa_cmd_context)
{
	struct smr_sar_entry *sar_entry = dsa_cmd_context->entry_ptr;
	struct smr_region *peer_smr;
	struct smr_resp *resp;
	struct smr_cmd *cmd;

	sar_entry->bytes_done += dsa_cmd_context->bytes_in_progress;
	cmd = &sar_entry->cmd;
	peer_smr = smr_peer_region(smr, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	assert(resp->status == SMR_STATUS_BUSY);
	resp->status = (dsa_cmd_context->dir == OFI_COPY_IOV_TO_BUF ?
			SMR_STATUS_SAR_READY : SMR_STATUS_SAR_FREE);

	smr_signal(peer_smr);
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
void smr_dsa_context_init(struct smr_ep *ep)
{
	int i, cpu;
	int wq_count;
	struct smr_dsa_context *dsa_context;
	unsigned int numa_node;
	int enable_dsa_page_touch = 0;

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

	fi_param_get_bool(&smr_prov, "enable_dsa_page_touch",
			  &enable_dsa_page_touch);

	wq_count = dsa_idxd_init_wq_array(1, numa_node,
				      dsa_context->wq_portal);

	if (wq_count == 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			 "error calling dsa_idxd_init_wq_array()\n");
		goto wq_get_error;
	}

	dsa_bitmap_allocate(&dsa_context->dsa_bitmap, CMD_CONTEXT_COUNT);

	for (i = 0; i < CMD_CONTEXT_COUNT; i++)
		dsa_context->dsa_cmd_context[i].index = -1;

	dsa_context->next_wq = 0;
	dsa_context->wq_count = wq_count;
	dsa_context->enable_dsa_page_touch = enable_dsa_page_touch;

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
		dsa_idxd_wq_unmap(dsa_context->wq_portal[i]);

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
	// Always signal the self to complete dsa, tx or rx.
	smr_signal(ep->region);
	pthread_spin_unlock(&ep->region->lock);
}

size_t smr_dsa_copy_to_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr)
{
	struct dsa_cmd_context *dsa_cmd_context;

	assert(smr_env.use_dsa_sar);

	if (resp->status != SMR_STATUS_SAR_FREE)
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

	if (resp->status != SMR_STATUS_SAR_READY)
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
