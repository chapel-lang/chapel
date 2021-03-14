/*
 * Copyright 2008-2018 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * LICENSE_BEGIN
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * LICENSE_END
 *
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/pci.h>
#ifdef __KERNEL__
#include <linux/delay.h>
#include <linux/slab.h>
#endif

#include "kcompat.h"
#include "vnic_dev.h"
#include "vnic_rq.h"

static int vnic_rq_alloc_bufs(struct vnic_rq *rq)
{
	struct vnic_rq_buf *buf;
	unsigned int i, j, count = rq->ring.desc_count;
	unsigned int blks = VNIC_RQ_BUF_BLKS_NEEDED(count);

	for (i = 0; i < blks; i++) {
		rq->bufs[i] = kzalloc(VNIC_RQ_BUF_BLK_SZ(count), GFP_ATOMIC);
		if (!rq->bufs[i])
			return -ENOMEM;
	}

	for (i = 0; i < blks; i++) {
		buf = rq->bufs[i];
		for (j = 0; j < VNIC_RQ_BUF_BLK_ENTRIES(count); j++) {
			buf->index = i * VNIC_RQ_BUF_BLK_ENTRIES(count) + j;
			buf->desc = (u8 *)rq->ring.descs +
				rq->ring.desc_size * buf->index;
			if (buf->index + 1 == count) {
				buf->next = rq->bufs[0];
				break;
			} else if (j + 1 == VNIC_RQ_BUF_BLK_ENTRIES(count)) {
				buf->next = rq->bufs[i + 1];
			} else {
				buf->next = buf + 1;
				buf++;
			}
		}
	}

	rq->to_use = rq->to_clean = rq->bufs[0];

	return 0;
}

#ifndef NOT_FOR_OPEN_ENIC
int vnic_rq_mem_size(struct vnic_rq *rq, unsigned int desc_count,
	unsigned int desc_size)
{
	int mem_size = 0;

	mem_size += vnic_dev_desc_ring_size(&rq->ring, desc_count, desc_size);

	mem_size += VNIC_RQ_BUF_BLKS_NEEDED(rq->ring.desc_count) *
		VNIC_RQ_BUF_BLK_SZ(rq->ring.desc_count);

	return mem_size;
}

#endif
void vnic_rq_free(struct vnic_rq *rq)
{
	struct vnic_dev *vdev;
	unsigned int i;

	vdev = rq->vdev;

	vnic_dev_free_desc_ring(vdev, &rq->ring);

	for (i = 0; i < VNIC_RQ_BUF_BLKS_MAX; i++) {
		if (rq->bufs[i]) {
			kfree(rq->bufs[i]);
			rq->bufs[i] = NULL;
		}
	}

	rq->ctrl = NULL;
}

int vnic_rq_alloc(struct vnic_dev *vdev, struct vnic_rq *rq, unsigned int index,
	unsigned int desc_count, unsigned int desc_size)
{
	int err;

	rq->index = index;
	rq->vdev = vdev;

	rq->ctrl = vnic_dev_get_res(vdev, RES_TYPE_RQ, index);
	if (!rq->ctrl) {
		pr_err("Failed to hook RQ[%d] resource\n", index);
		return -EINVAL;
	}

	vnic_rq_disable(rq);

	err = vnic_dev_alloc_desc_ring(vdev, &rq->ring, desc_count, desc_size);
	if (err)
		return err;

	err = vnic_rq_alloc_bufs(rq);
	if (err) {
		vnic_rq_free(rq);
		return err;
	}

	return 0;
}

static void vnic_rq_init_start(struct vnic_rq *rq, unsigned int cq_index,
	unsigned int fetch_index, unsigned int posted_index,
	unsigned int error_interrupt_enable,
	unsigned int error_interrupt_offset)
{
	u64 paddr;
	unsigned int count = rq->ring.desc_count;

	paddr = (u64)rq->ring.base_addr | VNIC_PADDR_TARGET;
	writeq(paddr, &rq->ctrl->ring_base);
	iowrite32(count, &rq->ctrl->ring_size);
	iowrite32(cq_index, &rq->ctrl->cq_index);
	iowrite32(error_interrupt_enable, &rq->ctrl->error_interrupt_enable);
	iowrite32(error_interrupt_offset, &rq->ctrl->error_interrupt_offset);
	iowrite32(0, &rq->ctrl->data_ring);
	iowrite32(0, &rq->ctrl->header_split);
	iowrite32(0, &rq->ctrl->error_status);
	iowrite32(fetch_index, &rq->ctrl->fetch_index);
	iowrite32(posted_index, &rq->ctrl->posted_index);

	rq->to_use = rq->to_clean =
		&rq->bufs[fetch_index / VNIC_RQ_BUF_BLK_ENTRIES(count)]
			[fetch_index % VNIC_RQ_BUF_BLK_ENTRIES(count)];
}

void vnic_rq_init(struct vnic_rq *rq, unsigned int cq_index,
	unsigned int error_interrupt_enable,
	unsigned int error_interrupt_offset)
{
	vnic_rq_init_start(rq, cq_index, 0, 0, error_interrupt_enable,
			   error_interrupt_offset);
}

void vnic_rq_error_out(struct vnic_rq *rq, unsigned int error)
{
	iowrite32(error, &rq->ctrl->error_status);
}

unsigned int vnic_rq_error_status(struct vnic_rq *rq)
{
	return vnic_rq_ctrl_error_status(rq->ctrl);
}

EXPORT_SYMBOL(vnic_rq_ctrl_error_status);
unsigned int vnic_rq_ctrl_error_status(struct vnic_rq_ctrl *ctrl)
{
	return ioread32(&ctrl->error_status);
}

void vnic_rq_enable(struct vnic_rq *rq)
{
	iowrite32(1, &rq->ctrl->enable);
}

int vnic_rq_disable(struct vnic_rq *rq)
{
	unsigned int wait;
	int i;

	/*
	 * Due to a race condition with clearing RQ "mini-cache", we need to
	 * disable the RQ twice to guarantee that stale descriptors are not
	 * used when this RQ is re-enabled.
	 */
	for (i = 0; i < 2; ++i) {
		iowrite32(0, &rq->ctrl->enable);

		/* Wait for HW to ACK disable request */
		for (wait = 20000; wait > 0; wait--) {
			if (ioread32(&rq->ctrl->running) == 0)
				break;
		}

		if (wait == 0) {
			pr_err("Failed to disable RQ[%d]\n", rq->index);
			return -ETIMEDOUT;
		}
	}
	return 0;
}

void vnic_rq_clean(struct vnic_rq *rq,
	void (*buf_clean)(struct vnic_rq *rq, struct vnic_rq_buf *buf))
{
	struct vnic_rq_buf *buf;
	u32 fetch_index;
	unsigned int count = rq->ring.desc_count;
	size_t i;

	buf = rq->to_clean;

	for (i = 0; i < rq->ring.desc_count; i++) {
		(*buf_clean)(rq, buf);
		buf = buf->next;
	}
	rq->ring.desc_avail = rq->ring.desc_count - 1;

	/* Use current fetch_index as the ring starting point */
	fetch_index = ioread32(&rq->ctrl->fetch_index);

	if (fetch_index == 0xFFFFFFFF) { /* check for hardware gone  */
		/* Hardware surprise removal: reset fetch_index */
		fetch_index = 0;
	}
	rq->to_use = rq->to_clean =
		&rq->bufs[fetch_index / VNIC_RQ_BUF_BLK_ENTRIES(count)]
			[fetch_index % VNIC_RQ_BUF_BLK_ENTRIES(count)];
	iowrite32(fetch_index, &rq->ctrl->posted_index);

	/*
	 * Anytime we write fetch_index, we need to re-write 0 to RQ.enable
	 * to re-sync internal VIC state on Sereno.
	 */
	iowrite32(0, &rq->ctrl->enable);

	vnic_dev_clear_desc_ring(&rq->ring);
}

