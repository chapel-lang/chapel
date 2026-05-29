/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
/*
 * Kernel Libfabric PCIe Provider (KLPP)
 *
 * Copyright (c) 2018-2024 GigaIO, Inc.
 */

#ifndef __KLPP_IOCTL_H_
#define __KLPP_IOCTL_H_

#include <linux/klpp/klpp_shared.h>

#define KLPPIOC_MAGIC 'k'

/*
 * ioctl values.
 */
#define KLPPIOC_NOOP		_IOWR(KLPPIOC_MAGIC, 0x01, struct klppioc_lf)
#define KLPPIOC_GETDEVICE	_IOWR(KLPPIOC_MAGIC, 0x03, struct klppioc_lf)

#define KLPPIOC_AV_RESOLVE	_IOWR(KLPPIOC_MAGIC, 0x06, struct klppioc_av)
#define KLPPIOC_AV_LOOKUP	_IOWR(KLPPIOC_MAGIC, 0x07, struct klppioc_av)
#define KLPPIOC_AV_NAME_RESOLVE	_IOWR(KLPPIOC_MAGIC, 0x08, struct klppioc_av)

#define KLPPIOC_CNTR_CREATE	_IOWR(KLPPIOC_MAGIC, 0x0a, struct klppioc_cntr)
#define KLPPIOC_CNTR_CLOSE	_IOWR(KLPPIOC_MAGIC, 0x0c, struct klppioc_cntr)

#define KLPPIOC_CTX_CREATE	_IOWR(KLPPIOC_MAGIC, 0x0d, struct klppioc_ctx)
#define KLPPIOC_CTX_CLOSE	_IOWR(KLPPIOC_MAGIC, 0x0e, struct klppioc_ctx)

#define KLPPIOC_CQ_CREATE	_IOWR(KLPPIOC_MAGIC, 0x0f, struct klppioc_cq)
#define KLPPIOC_CQ_CLOSE	_IOWR(KLPPIOC_MAGIC, 0x10, struct klppioc_cq)
#define KLPPIOC_CQ_TEST		_IOWR(KLPPIOC_MAGIC, 0x90, struct klppioc_cq)

#define KLPPIOC_EP_CREATE	_IOWR(KLPPIOC_MAGIC, 0x11, struct klppioc_ep)
#define KLPPIOC_EP_BIND		_IOWR(KLPPIOC_MAGIC, 0x12, struct klppioc_ep)
#define KLPPIOC_EP_ENABLE	_IOWR(KLPPIOC_MAGIC, 0x13, struct klppioc_ep)
#define KLPPIOC_EP_DISABLE	_IOWR(KLPPIOC_MAGIC, 0x14, struct klppioc_ep)
#define KLPPIOC_EP_CLOSE	_IOWR(KLPPIOC_MAGIC, 0x16, struct klppioc_ep)

#define KLPPIOC_MR_REGISTER	_IOWR(KLPPIOC_MAGIC, 0x17, struct klppioc_mr)
#define KLPPIOC_MR_CLOSE	_IOWR(KLPPIOC_MAGIC, 0x18, struct klppioc_mr)

#define KLPPIOC_DM_PING		_IOWR(KLPPIOC_MAGIC, 0x26, struct klppioc_dm)

#define KLPPIOC_RMR_LABEL	_IOWR(KLPPIOC_MAGIC, 0x28, struct klppioc_rmr)

#define KLPPIOC_READWRITE	_IOWR(KLPPIOC_MAGIC, 0x54, struct klpp_ioc_readwrite)

#define KLPPIOC_KMC_SEND	_IOWR(KLPPIOC_MAGIC, 0x60, struct klpp_ioc_kmc_send)
#define KLPPIOC_UMC_ACQUIRE	_IOWR(KLPPIOC_MAGIC, 0x61, struct klpp_ioc_umc)
#define KLPPIOC_UMC_RELEASE	_IOWR(KLPPIOC_MAGIC, 0x62, struct klpp_ioc_umc)
#define KLPPIOC_PROGRESS	_IOWR(KLPPIOC_MAGIC, 0x52, struct klpp_ioc_prog)

#define KLPPIOCF_KMC_NODQP		(0x1)

struct klpp_ioc_kmc_send {
	struct lpp_fi_addr	dst_addr;
	__aligned_u64		iov_ptr;
	__aligned_u64		iov_cnt;
	__aligned_u64		kmc_flags;
	__aligned_u64		next_ptr;
	__s64			errval;
	struct klpp_msg_hdr	msg_hdr;
};

struct klpp_ioc_umc {
	klpp_id_t		umc_id;
	__aligned_u64		umc_gen;
	__aligned_u64		rx_base_uaddr;
	__aligned_u64		tx_base_uaddr;
	__aligned_u64		k2u_base_uaddr;
	__aligned_u64		u2k_base_uaddr;
	struct lpp_fi_addr	ep_addr;
	__u8			src_id;
	__u16			port;
	__u8			pad[5];
};

struct klpp_ioc_prog {
	__s32	umc_id;
};

struct klpp_ioc_readwrite {
	klpp_id_t		ep_id;
	struct lpp_fi_addr	fi_addr;
	uint64_t		local_uaddr;
	uint64_t		remote_uaddr;
	uint64_t		remote_key;
	uint64_t		length;
	uint64_t		flags;
	uint64_t		context;
};

struct klppioc_av {
	__u32 fabric_index;
	__u32 host_index;
	__u16 port;
	__u8 hostname[KLPP_NAME_MAX];
	struct lpp_fi_addr lpp_fi_addr;
};

struct klppioc_cntr {
	klpp_id_t	id;
	uintptr_t	addr;
	uint64_t	threshold;
	uint64_t	errvalue;
	int		timeout;
};

struct klppioc_ctx {
	klpp_id_t	id;
	size_t		entries;
	int		ctx_class; /* A klpp_class value. */
};

struct klppioc_cq {
	klpp_id_t	id;
	int		type;
	uintptr_t	addr;
	size_t		entries;
};

struct klppioc_ep {
	klpp_id_t	ep_id;
	klpp_id_t	bind_id;
	int		bind_class;

	klpp_id_t	rx_id;
	klpp_id_t	stx_id;

	klpp_id_t	cq_transmit_id;
	klpp_id_t	cq_recv_id;

	klpp_id_t	cntr_send_id;
	klpp_id_t	cntr_recv_id;
	klpp_id_t	cntr_read_id;
	klpp_id_t	cntr_write_id;
	klpp_id_t	cntr_remote_read_id;
	klpp_id_t	cntr_remote_write_id;

	uint64_t	io_stat_uaddr;
};

#define KLPPIOCF_MR_RO		(0x01)
#define KLPPIOCF_MR_GPU		(0x02)
#define KLPPIOCF_MR_EXPORT	(0x04)
#define KLPPIOCF_MR_GPU_CUDA	(0x08)
#define KLPPIOCF_MR_GPU_ROCM	(0x10)

struct klppioc_mr {
	klpp_id_t	id;
	void		*address;
	ssize_t		length;
	uint64_t	flags;
	uint64_t	key;
};

struct klppioc_dm {
	__u32	node_id;
	int	sync;

	int	test_no_reply;
	int	test_delayed_reply;
	int	test_long_delayed_reply;
	int	test_more;
};

struct klppioc_pci_attrs {
	__u16 domain;
	__u8 bus;
	__u8 device;
	__u8 function;
	__u8 _pad[3];
};

struct klppioc_lf {
	klpp_id_t	user_id;
	uint32_t	host_index;
	uint32_t	host_count;
	uint64_t	max_msg_size;
	uint64_t	max_rma_size;
	char		domain[KLPP_NAME_MAX];
	char		fabric[KLPP_NAME_MAX];
	struct klppioc_pci_attrs pci_attrs;
	struct klpp_rmr_region_info rmr_region_info;
	struct klpp_mc_params mc_params;
};

struct klppioc_rmr {
	uintptr_t	addr;
	int		region_class;
};

#endif /* __KLPP_IOCTL_H_ */
