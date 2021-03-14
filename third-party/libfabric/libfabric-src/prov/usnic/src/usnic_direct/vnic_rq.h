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

#ifndef _VNIC_RQ_H_
#define _VNIC_RQ_H_

#include "vnic_dev.h"
#include "vnic_cq.h"

/* Receive queue control */
struct vnic_rq_ctrl {
	u64 ring_base;			/* 0x00 */
	u32 ring_size;			/* 0x08 */
	u32 pad0;
	u32 posted_index;		/* 0x10 */
	u32 pad1;
	u32 cq_index;			/* 0x18 */
	u32 pad2;
	u32 enable;			/* 0x20 */
	u32 pad3;
	u32 running;			/* 0x28 */
	u32 pad4;
	u32 fetch_index;		/* 0x30 */
	u32 pad5;
	u32 error_interrupt_enable;	/* 0x38 */
	u32 pad6;
	u32 error_interrupt_offset;	/* 0x40 */
	u32 pad7;
	u32 error_status;		/* 0x48 */
	u32 pad8;
	u32 tcp_sn;         	/* 0x50 */
	u32 pad9[3];
	u32 dca_select;     	/* 0x60 */
	u32 pad10[3];
	u32 data_ring;      	/* 0x70 */
	u32 pad11;
	u32 header_split;      	/* 0x78 */
	u32 pad12;
};

/* Break the vnic_rq_buf allocations into blocks of 32/64 entries */
#define VNIC_RQ_BUF_MIN_BLK_ENTRIES 32
#define VNIC_RQ_BUF_DFLT_BLK_ENTRIES 64
#define VNIC_RQ_BUF_BLK_ENTRIES(entries) \
	((unsigned int)((entries < VNIC_RQ_BUF_DFLT_BLK_ENTRIES) ? \
	VNIC_RQ_BUF_MIN_BLK_ENTRIES : VNIC_RQ_BUF_DFLT_BLK_ENTRIES))
#define VNIC_RQ_BUF_BLK_SZ(entries) \
	(VNIC_RQ_BUF_BLK_ENTRIES(entries) * sizeof(struct vnic_rq_buf))
#define VNIC_RQ_BUF_BLKS_NEEDED(entries) \
	DIV_ROUND_UP(entries, VNIC_RQ_BUF_BLK_ENTRIES(entries))
#define VNIC_RQ_BUF_BLKS_MAX VNIC_RQ_BUF_BLKS_NEEDED(4096)

struct vnic_rq_buf {
	struct vnic_rq_buf *next;
	dma_addr_t dma_addr;
	void *os_buf;
	unsigned int os_buf_index;
	unsigned int len;
	unsigned int index;
	void *desc;
	uint64_t wr_id;
};

struct vnic_rq {
	unsigned int index;
	struct vnic_dev *vdev;
	struct vnic_rq_ctrl __iomem *ctrl;              /* memory-mapped */
	struct vnic_dev_ring ring;
	struct vnic_rq_buf *bufs[VNIC_RQ_BUF_BLKS_MAX];
	struct vnic_rq_buf *to_use;
	struct vnic_rq_buf *to_clean;
	void *os_buf_head;
	unsigned int pkts_outstanding;
#if defined(__LIBUSNIC__)
	uint32_t qp_num;
#endif

#ifdef ENIC_BUSY_POLL
	atomic_t bpoll_state;
#endif /*ENIC_BUSY_POLL*/
};

static inline unsigned int vnic_rq_desc_avail(struct vnic_rq *rq)
{
	/* how many does SW own? */
	return rq->ring.desc_avail;
}

static inline unsigned int vnic_rq_desc_used(struct vnic_rq *rq)
{
	/* how many does HW own? */
	return rq->ring.desc_count - rq->ring.desc_avail - 1;
}

static inline void *vnic_rq_next_desc(struct vnic_rq *rq)
{
	return rq->to_use->desc;
}

static inline unsigned int vnic_rq_next_index(struct vnic_rq *rq)
{
	return rq->to_use->index;
}

static inline void vnic_rq_post(struct vnic_rq *rq,
	void *os_buf, unsigned int os_buf_index,
	dma_addr_t dma_addr, unsigned int len,
	uint64_t wrid)
{
	struct vnic_rq_buf *buf = rq->to_use;

	buf->os_buf = os_buf;
	buf->os_buf_index = os_buf_index;
	buf->dma_addr = dma_addr;
	buf->len = len;
	buf->wr_id = wrid;

	buf = buf->next;
	rq->to_use = buf;
	rq->ring.desc_avail--;

	/* Move the posted_index every nth descriptor
	 */
#if defined(__LIBUSNIC__)
#define VNIC_RQ_RETURN_RATE		0x0
#endif

#ifndef VNIC_RQ_RETURN_RATE
#define VNIC_RQ_RETURN_RATE		0xf	/* keep 2^n - 1 */
#endif

	if ((buf->index & VNIC_RQ_RETURN_RATE) == 0) {
		/* Adding write memory barrier prevents compiler and/or CPU
		 * reordering, thus avoiding descriptor posting before
		 * descriptor is initialized. Otherwise, hardware can read
		 * stale descriptor fields.
		 */
		wmb();
		iowrite32(buf->index, &rq->ctrl->posted_index);
	}
}

static inline void vnic_rq_post_commit(struct vnic_rq *rq,
	void *os_buf, unsigned int os_buf_index,
	dma_addr_t dma_addr, unsigned int len)
{
	struct vnic_rq_buf *buf = rq->to_use;

	buf->os_buf = os_buf;
	buf->os_buf_index = os_buf_index;
	buf->dma_addr = dma_addr;
	buf->len = len;

	buf = buf->next;
	rq->to_use = buf;
	rq->ring.desc_avail--;

	/* Move the posted_index every descriptor
	 */

	/* Adding write memory barrier prevents compiler and/or CPU
	 * reordering, thus avoiding descriptor posting before
	 * descriptor is initialized. Otherwise, hardware can read
	 * stale descriptor fields.
	 */
	wmb();
	iowrite32(buf->index, &rq->ctrl->posted_index);
}

static inline void vnic_rq_return_descs(struct vnic_rq *rq, unsigned int count)
{
	rq->ring.desc_avail += count;
}

enum desc_return_options {
	VNIC_RQ_RETURN_DESC,
	VNIC_RQ_DEFER_RETURN_DESC,
};

static inline void vnic_rq_service(struct vnic_rq *rq,
	struct cq_desc *cq_desc, u16 completed_index,
	int desc_return, void (*buf_service)(struct vnic_rq *rq,
	struct cq_desc *cq_desc, struct vnic_rq_buf *buf,
	int skipped, void *opaque), void *opaque)
{
	struct vnic_rq_buf *buf;
	int skipped;

	buf = rq->to_clean;
	while (1) {

		skipped = (buf->index != completed_index);

		(*buf_service)(rq, cq_desc, buf, skipped, opaque);

		if (desc_return == VNIC_RQ_RETURN_DESC)
			rq->ring.desc_avail++;

		rq->to_clean = buf->next;

		if (!skipped)
			break;

		buf = rq->to_clean;
	}
}

static inline int vnic_rq_fill(struct vnic_rq *rq,
	int (*buf_fill)(struct vnic_rq *rq))
{
	int err;

	while (vnic_rq_desc_avail(rq) > 0) {

		err = (*buf_fill)(rq);
		if (err)
			return err;
	}

	return 0;
}

static inline int vnic_rq_fill_count(struct vnic_rq *rq,
	int (*buf_fill)(struct vnic_rq *rq), unsigned int count)
{
	int err;

	while ((vnic_rq_desc_avail(rq) > 0) && (count--)) {

		err = (*buf_fill)(rq);
		if (err)
			return err;
	}

	return 0;
}

void vnic_rq_free(struct vnic_rq *rq);
int vnic_rq_alloc(struct vnic_dev *vdev, struct vnic_rq *rq, unsigned int index,
	unsigned int desc_count, unsigned int desc_size);
void vnic_rq_init(struct vnic_rq *rq, unsigned int cq_index,
	unsigned int error_interrupt_enable,
	unsigned int error_interrupt_offset);
void vnic_rq_error_out(struct vnic_rq *rq, unsigned int error);
unsigned int vnic_rq_error_status(struct vnic_rq *rq);
unsigned int vnic_rq_ctrl_error_status(struct vnic_rq_ctrl *ctrl);
void vnic_rq_enable(struct vnic_rq *rq);
int vnic_rq_disable(struct vnic_rq *rq);
void vnic_rq_clean(struct vnic_rq *rq,
	void (*buf_clean)(struct vnic_rq *rq, struct vnic_rq_buf *buf));
#ifndef NOT_FOR_OPEN_ENIC
int vnic_rq_mem_size(struct vnic_rq *rq, unsigned int desc_count,
	unsigned int desc_size);
#endif

#endif /* _VNIC_RQ_H_ */
