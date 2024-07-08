/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright (c) 2021 Intel Corporation. All rights reserved.
 */

#ifndef __LINUX_DMABUF_REG_H__
#define __LINUX_DMABUF_REG_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define DMABUF_REG_DEV_NAME	"dmabuf_reg"
#define DMABUF_REG_IOCTL	1

enum {
	DMABUF_REG_ADD,
	DMABUF_REG_REMOVE_ADDR,
	DMABUF_REG_REMOVE_FD,
	DMABUF_REG_QUERY,
};

struct dmabuf_reg_param {
	uint64_t base;
	uint64_t size;
	uint32_t fd;
	uint32_t op;
};

#endif /* __LINUX_DMABUF_REG_H__ */
