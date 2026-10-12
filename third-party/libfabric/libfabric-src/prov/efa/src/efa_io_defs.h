/* SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause */
/*
 * Copyright 2018-2024 Amazon.com, Inc. or its affiliates. All rights reserved.
 */

#ifndef _EFA_IO_DEFS_H_
#define _EFA_IO_DEFS_H_

#include "config.h"

#if HAVE_EFA_DATA_PATH_DIRECT

#define EFA_IO_TX_DESC_NUM_BUFS	       2
#define EFA_IO_TX_DESC_NUM_RDMA_BUFS   1

enum efa_io_queue_type {
	/* send queue (of a QP) */
	EFA_IO_SEND_QUEUE = 1,
	/* recv queue (of a QP) */
	EFA_IO_RECV_QUEUE = 2,
};

enum efa_io_send_op_type {
	/* send message */
	EFA_IO_SEND = 0,
	/* RDMA read */
	EFA_IO_RDMA_READ = 1,
	/* RDMA write */
	EFA_IO_RDMA_WRITE = 2,
};

struct efa_io_tx_meta_desc {
	/* Verbs-generated Request ID */
	uint16_t req_id;

	/*
	 * control flags
	 * 3:0 : op_type - enum efa_io_send_op_type
	 * 4 : has_imm - immediate_data field carries valid
	 *    data.
	 * 5 : inline_msg - inline mode - inline message data
	 *    follows this descriptor (no buffer descriptors).
	 *    Note that it is different from immediate data
	 * 6 : meta_extension - Extended metadata. MBZ
	 * 7 : meta_desc - Indicates metadata descriptor.
	 *    Must be set.
	 */
	uint8_t ctrl1;

	/*
	 * control flags
	 * 0 : phase
	 * 1 : reserved25 - MBZ
	 * 2 : first - Indicates first descriptor in
	 *    transaction. Must be set.
	 * 3 : last - Indicates last descriptor in
	 *    transaction. Must be set.
	 * 4 : comp_req - Indicates whether completion should
	 *    be posted, after packet is transmitted. Valid only
	 *    for the first descriptor
	 * 7:5 : reserved29 - MBZ
	 */
	uint8_t ctrl2;

	uint16_t dest_qp_num;

	/*
	 * If inline_msg bit is set, length of inline message in bytes,
	 *    otherwise length of SGL (number of buffers).
	 */
	uint16_t length;

	/*
	 * immediate data: if has_imm is set, then this field is included
	 *    within Tx message and reported in remote Rx completion.
	 */
	uint32_t immediate_data;

	uint16_t ah;

	uint16_t reserved;

	/* Queue key */
	uint32_t qkey;

	uint8_t reserved2[12];
};

/*
 * Tx queue buffer descriptor, for any transport type. Preceded by metadata
 * descriptor.
 */
struct efa_io_tx_buf_desc {
	/* length in bytes */
	uint32_t length;

	/*
	 * 23:0 : lkey - local memory translation key
	 * 31:24 : reserved - MBZ
	 */
	uint32_t lkey;

	/* Buffer address bits[31:0] */
	uint32_t buf_addr_lo;

	/* Buffer address bits[63:32] */
	uint32_t buf_addr_hi;
};

struct efa_io_remote_mem_addr {
	/* length in bytes */
	uint32_t length;

	/* remote memory translation key */
	uint32_t rkey;

	/* Buffer address bits[31:0] */
	uint32_t buf_addr_lo;

	/* Buffer address bits[63:32] */
	uint32_t buf_addr_hi;
};

struct efa_io_rdma_req {
	/* Remote memory address */
	struct efa_io_remote_mem_addr remote_mem;

	/* Local memory address */
	struct efa_io_tx_buf_desc local_mem[1];
};

/*
 * Tx WQE, composed of tx meta descriptors followed by either tx buffer
 * descriptors or inline data
 */
struct efa_io_tx_wqe {
	/* TX meta */
	struct efa_io_tx_meta_desc meta;

	union {
		/* Send buffer descriptors */
		struct efa_io_tx_buf_desc sgl[2];

		uint8_t inline_data[32];

		/* RDMA local and remote memory addresses */
		struct efa_io_rdma_req rdma_req;
	} data;
};

/*
 * Rx buffer descriptor; RX WQE is composed of one or more RX buffer
 * descriptors.
 */
struct efa_io_rx_desc {
	/* Buffer address bits[31:0] */
	uint32_t buf_addr_lo;

	/* Buffer Pointer[63:32] */
	uint32_t buf_addr_hi;

	/* Verbs-generated request id. */
	uint16_t req_id;

	/* Length in bytes. */
	uint16_t length;

	/*
	 * LKey and control flags
	 * 23:0 : lkey
	 * 29:24 : reserved - MBZ
	 * 30 : first - Indicates first descriptor in WQE
	 * 31 : last - Indicates last descriptor in WQE
	 */
	uint32_t lkey_ctrl;
};

/* Common IO completion descriptor */
struct efa_io_cdesc_common {
	/*
	 * verbs-generated request ID, as provided in the completed tx or rx
	 * descriptor.
	 */
	uint16_t req_id;

	uint8_t status;

	/*
	 * flags
	 * 0 : phase - Phase bit
	 * 2:1 : q_type - enum efa_io_queue_type: send/recv
	 * 3 : has_imm - indicates that immediate data is
	 *    present - for RX completions only
	 * 6:4 : op_type - enum efa_io_send_op_type
	 * 7 : unsolicited - indicates that there is no
	 *    matching request - for RDMA with imm. RX only
	 */
	uint8_t flags;

	/* local QP number */
	uint16_t qp_num;
};

/* Tx completion descriptor */
struct efa_io_tx_cdesc {
	/* Common completion info */
	struct efa_io_cdesc_common common;

	/* MBZ */
	uint16_t reserved16;
};

/* Rx Completion Descriptor */
struct efa_io_rx_cdesc {
	/* Common completion info */
	struct efa_io_cdesc_common common;

	/* Transferred length bits[15:0] */
	uint16_t length;

	/* Remote Address Handle FW index, 0xFFFF indicates invalid ah */
	uint16_t ah;

	uint16_t src_qp_num;

	/* Immediate data */
	uint32_t imm;
};

/* Rx Completion Descriptor RDMA write info */
struct efa_io_rx_cdesc_rdma_write {
	/* Transferred length bits[31:16] */
	uint16_t length_hi;
};

/* Extended Rx Completion Descriptor */
struct efa_io_rx_cdesc_ex {
	/* Base RX completion info */
	struct efa_io_rx_cdesc base;

	union {
		struct efa_io_rx_cdesc_rdma_write rdma_write;

		/*
		 * Valid only in case of unknown AH (0xFFFF) and CQ
		 * set_src_addr is enabled.
		 */
		uint8_t src_addr[16];
	} u;
};
/* Barriers for DMA.

   These barriers are expliclty only for use with user DMA operations. If you
   are looking for barriers to use with cache-coherent multi-threaded
   consitency then look in stdatomic.h. If you need both kinds of synchronicity
   for the same address then use an atomic operation followed by one
   of these barriers.

   When reasoning about these barriers there are two objects:
     - CPU attached address space (the CPU memory could be a range of things:
       cached/uncached/non-temporal CPU DRAM, uncached MMIO space in another
       device, pMEM). Generally speaking the ordering is only relative
       to the local CPU's view of the system. Eg if the local CPU
       is not guaranteed to see a write from another CPU then it is also
       OK for the DMA device to also not see the write after the barrier.
     - A DMA initiator on a bus. For instance a PCI-E device issuing
       MemRd/MemWr TLPs.

   The ordering guarantee is always stated between those two streams. Eg what
   happens if a MemRd TLP is sent in via PCI-E relative to a CPU WRITE to the
   same memory location.

   The providers have a very regular and predictable use of these barriers,
   to make things very clear each narrow use is given a name and the proper
   name should be used in the provider as a form of documentation.
*/

/* Ensure that the device's view of memory matches the CPU's view of memory.
   This should be placed before any MMIO store that could trigger the device
   to begin doing DMA, such as a device doorbell ring.

   eg
    *dma_buf = 1;
    udma_to_device_barrier();
    mmio_write(DO_DMA_REG, dma_buf);
   Must ensure that the device sees the '1'.

   This is required to fence writes created by the libibverbs user. Those
   writes could be to any CPU mapped memory object with any cachability mode.

   NOTE: x86 has historically used a weaker semantic for this barrier, and
   only fenced normal stores to normal memory. libibverbs users using other
   memory types or non-temporal stores are required to use SFENCE in their own
   code prior to calling verbs to start a DMA.
*/
#if defined(__x86_64__)
#define udma_to_device_barrier() asm volatile("" ::: "memory")
#elif defined(__aarch64__)
#define udma_to_device_barrier() asm volatile("dmb oshst" ::: "memory")
#else
#error No architecture specific memory barrier defines found!
#endif

/* Ensure that all ordered stores from the device are observable from the
   CPU. This only makes sense after something that observes an ordered store
   from the device - eg by reading a MMIO register or seeing that CPU memory is
   updated.

   This guarantees that all reads that follow the barrier see the ordered
   stores that preceded the observation.

   For instance, this would be used after testing a valid bit in a memory
   that is a DMA target, to ensure that the following reads see the
   data written before the MemWr TLP that set the valid bit.
*/
#if defined(__x86_64__)
#define udma_from_device_barrier() asm volatile("lfence" ::: "memory")
#elif defined(__aarch64__)
#define udma_from_device_barrier() asm volatile("dmb oshld" ::: "memory")
#else
#error No architecture specific memory barrier defines found!
#endif

/* Order writes to CPU memory so that a DMA device cannot view writes after
   the barrier without also seeing all writes before the barrier. This does
   not guarantee any writes are visible to DMA.

   This would be used in cases where a DMA buffer might have a valid bit and
   data, this barrier is placed after writing the data but before writing the
   valid bit to ensure the DMA device cannot observe a set valid bit with
   unwritten data.

   Compared to udma_to_device_barrier() this barrier is not required to fence
   anything but normal stores to normal malloc memory. Usage should be:

   write_wqe
      udma_to_device_barrier();    // Get user memory ready for DMA
      wqe->addr = ...;
      wqe->flags = ...;
      udma_ordering_write_barrier();  // Guarantee WQE written in order
      wqe->valid = 1;
*/
#define udma_ordering_write_barrier() udma_to_device_barrier()

/* Promptly flush writes to MMIO Write Cominbing memory.
   This should be used after a write to WC memory. This is both a barrier
   and a hint to the CPU to flush any buffers to reduce latency to TLP
   generation.

   This is not required to have any effect on CPU memory.

   If done while holding a lock then the ordering of MMIO writes across CPUs
   must be guaranteed to follow the natural ordering implied by the lock.

   This must also act as a barrier that prevents write combining, eg
     *wc_mem = 1;
     mmio_flush_writes();
     *wc_mem = 2;
   Must always produce two MemWr TLPs, '1' and '2'. Without the barrier
   the CPU is allowed to produce a single TLP '2'.

   Note that there is no order guarantee for writes to WC memory without
   barriers.

   This is intended to be used in conjunction with WC memory to generate large
   PCI-E MemWr TLPs from the CPU.
*/

#if defined(__x86_64__)
#define mmio_flush_writes() asm volatile("sfence" ::: "memory")
#elif defined(__aarch64__)
#define mmio_flush_writes() asm volatile("dsb st" ::: "memory");
#else
#error No architecture specific memory barrier defines found!
#endif

/* Prevent WC writes from being re-ordered relative to other MMIO
   writes. This should be used before a write to WC memory.

   This must act as a barrier to prevent write re-ordering from different
   memory types:
     *mmio_mem = 1;
     mmio_flush_writes();
     *wc_mem = 2;
   Must always produce a TLP '1' followed by '2'.

   This barrier implies udma_to_device_barrier()

   This is intended to be used in conjunction with WC memory to generate large
   PCI-E MemWr TLPs from the CPU.
*/
#define mmio_wc_start() mmio_flush_writes()

/* tx_meta_desc */
#define EFA_IO_TX_META_DESC_OP_TYPE_MASK	GENMASK(3, 0)
#define EFA_IO_TX_META_DESC_HAS_IMM_MASK	BIT(4)
#define EFA_IO_TX_META_DESC_INLINE_MSG_MASK	BIT(5)
#define EFA_IO_TX_META_DESC_META_EXTENSION_MASK BIT(6)
#define EFA_IO_TX_META_DESC_META_DESC_MASK	BIT(7)
#define EFA_IO_TX_META_DESC_PHASE_MASK		BIT(0)
#define EFA_IO_TX_META_DESC_FIRST_MASK		BIT(2)
#define EFA_IO_TX_META_DESC_LAST_MASK		BIT(3)
#define EFA_IO_TX_META_DESC_COMP_REQ_MASK	BIT(4)

/* tx_buf_desc */
#define EFA_IO_TX_BUF_DESC_LKEY_MASK GENMASK(23, 0)

/* rx_desc */
#define EFA_IO_RX_DESC_LKEY_MASK  GENMASK(23, 0)
#define EFA_IO_RX_DESC_FIRST_MASK BIT(30)
#define EFA_IO_RX_DESC_LAST_MASK  BIT(31)

/* cdesc_common */
#define EFA_IO_CDESC_COMMON_PHASE_MASK	     BIT(0)
#define EFA_IO_CDESC_COMMON_Q_TYPE_MASK	     GENMASK(2, 1)
#define EFA_IO_CDESC_COMMON_HAS_IMM_MASK     BIT(3)
#define EFA_IO_CDESC_COMMON_OP_TYPE_MASK     GENMASK(6, 4)
#define EFA_IO_CDESC_COMMON_UNSOLICITED_MASK BIT(7)

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define FIELD_GET(_mask, _reg) \
	({ (typeof(_mask)) (((_reg) & (_mask)) >> __bf_shf(_mask)); })
#define FIELD_PREP(_mask, _val) \
	({ ((typeof(_mask)) (_val) << __bf_shf(_mask)) & (_mask); })

#define EFA_GET(ptr, mask) FIELD_GET(mask##_MASK, *(ptr))

#define EFA_SET(ptr, mask, value)                       \
	({                                              \
		typeof(ptr) _ptr = ptr;                 \
		*_ptr = (*_ptr & ~(mask##_MASK)) |      \
			FIELD_PREP(mask##_MASK, value); \
	})

#endif /* HAVE_EFA_DATA_PATH_DIRECT */
#endif /* _EFA_IO_DEFS_H_ */
