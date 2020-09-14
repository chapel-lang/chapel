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

#ifndef _KCOMPAT_H_
#define _KCOMPAT_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <linux/types.h>
#include <linux_types.h>
#include <netinet/in.h>
#include <kcompat_priv.h>
#include <assert.h>
#include <stdbool.h>

#define EXPORT_SYMBOL(x)
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define ALIGN(x, a)              __ALIGN_MASK(x, (typeof(x))(a)-1)
#define __ALIGN_MASK(x, mask)    (((x)+(mask))&~(mask))
#define ETH_ALEN 6
#define BUG() assert(0)
#define BUG_ON(x) assert(!x)
#define kzalloc(x, flags) calloc(1, x)
#define kfree(x) free(x)

#define __iomem
#define udelay usleep
#define readl ioread32
#define writel iowrite32

typedef int gfp_t;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef offsetof
#define offsetof(t, m) ((size_t) &((t *)0)->m)
#endif

static inline uint32_t ioread32(const volatile void *addr)
{
	return *(volatile uint32_t *)addr;
}

static inline uint16_t ioread16(const volatile void *addr)
{
	return *(volatile uint16_t *)addr;
}

static inline uint8_t ioread8(const volatile void *addr)
{
	return *(volatile uint8_t *)addr;
}

static inline void iowrite64(uint64_t val, const volatile void *addr)
{
	*(volatile uint64_t *)addr = val;
}

static inline void iowrite32(uint32_t val, const volatile void *addr)
{
	*(volatile uint32_t *)addr = val;
}

#endif /* _KCOMPAT_H_ */
