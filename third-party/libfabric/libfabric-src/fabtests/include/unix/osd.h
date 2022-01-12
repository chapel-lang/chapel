/*
 * Copyright (c) 2016 Intel Corp, Inc. All rights reserved.
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

#ifndef _FABTESTS_UNIX_OSD_H_
#define _FABTESTS_UNIX_OSD_H_

#include <complex.h>
#include <unistd.h>
#include <fcntl.h>

static inline int ft_startup(void)
{
	return 0;
}

static inline int ft_fd_nonblock(int fd)
{
	long flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return -errno;

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK))
		return -errno;

	return 0;
}

/* complex operations implementation */
#define OFI_COMPLEX(name) ofi_##name##_complex
#define OFI_COMPLEX_OP(name, op) ofi_complex_##name##_##op
#define OFI_COMPLEX_TYPE_DECL(name, type) typedef type complex OFI_COMPLEX(name);

OFI_COMPLEX_TYPE_DECL(float, float)
OFI_COMPLEX_TYPE_DECL(double, double)
OFI_COMPLEX_TYPE_DECL(long_double, long double)

#define OFI_COMPLEX_OPS(name)									      \
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, sum)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{												      \
	return v1 + v2;										      \
}												      \
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, mul)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{												      \
	return v1 * v2;										      \
}												      \
static inline int OFI_COMPLEX_OP(name, equ)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2)		      \
{												      \
	return v1 == v2;                                                                	      \
}												      \
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, land)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2)\
{												      \
	return v1 && v2;      									      \
}												      \
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, lor)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{												      \
	return v1 || v2;									      \
}

OFI_COMPLEX_OPS(float)
OFI_COMPLEX_OPS(double)
OFI_COMPLEX_OPS(long_double)

#endif /* FABTESTS_UNIX_OSD_H */
