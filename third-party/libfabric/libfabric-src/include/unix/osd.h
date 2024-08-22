/*
 * Copyright (c) 2013-2016 Intel Corporation. All rights reserved.
 * Copyright (c) 2023 Tactical Computing Labs, LLC. All rights reserved.
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

#ifndef _FI_UNIX_OSD_H_
#define _FI_UNIX_OSD_H_

#include "config.h"

#include <stdlib.h>
#include <unistd.h>
#include <complex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <rdma/fi_errno.h>


/* MSG_NOSIGNAL doesn't exist on OS X */
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#define FI_DESTRUCTOR(func) static __attribute__((destructor)) void func

#ifndef UNREFERENCED_PARAMETER
#define OFI_UNUSED(var) (void)var
#else
#define OFI_UNUSED UNREFERENCED_PARAMETER
#endif

#define OFI_SOCK_TRY_SND_RCV_AGAIN(err)		\
	(((err) == EAGAIN)	||		\
	 ((err) == EWOULDBLOCK))

#define OFI_SOCK_TRY_ACCEPT_AGAIN(err)		\
	(((err) == EAGAIN)	||		\
	 ((err) == EWOULDBLOCK))

#define OFI_SOCK_TRY_CONN_AGAIN(err)	\
	((err) == EINPROGRESS)

#define OFI_MAX_SOCKET_BUF_SIZE	SIZE_MAX

struct util_shm
{
	int		shared_fd;
	void		*ptr;
	const char	*name;
	size_t		size;
};

int ofi_mmap_anon_pages(void **memptr, size_t size, int flags);
int ofi_unmap_anon_pages(void *memptr, size_t size);

static inline int ofi_memalign(void **memptr, size_t alignment, size_t size)
{
	return posix_memalign(memptr, alignment, size);
}

static inline void ofi_freealign(void *memptr)
{
	free(memptr);
}

static inline void ofi_osd_init(void)
{
}

static inline void ofi_osd_fini(void)
{
}

static inline int ofi_getsockname(SOCKET fd, struct sockaddr *addr, socklen_t *len)
{
	return getsockname(fd, addr, len);
}

static inline int ofi_getpeername(SOCKET fd, struct sockaddr *addr, socklen_t *len)
{
	return getpeername(fd, addr, len);
}

static inline SOCKET ofi_socket(int domain, int type, int protocol)
{
	return socket(domain, type, protocol);
}

static inline ssize_t
ofi_sendmsg_udp(SOCKET fd, const struct msghdr *msg, int flags)
{
	return sendmsg(fd, msg, flags);
}

static inline ssize_t
ofi_recvmsg_udp(SOCKET fd, struct msghdr *msg, int flags)
{
	return recvmsg(fd, msg, flags);
}

static inline int ofi_shutdown(SOCKET socket, int how)
{
	return shutdown(socket, how);
}

static inline int ofi_close_socket(SOCKET socket)
{
	return close(socket);
}

int fi_fd_nonblock(int fd);

int fi_fd_block(int fd);

static inline int ofi_sockerr(void)
{
	return errno;
}

static inline int ofi_syserr(void)
{
	return errno;
}

/* sysconf can return -1 and not change errno */
static inline long ofi_sysconf(int name)
{
	int ret;

	errno = 0;
	ret = sysconf(name);
	if (ret <= 0)
		return errno ? -errno : -FI_EOTHER;

	return ret;
}

/* OSX has no such definition. So, add it manually */
#ifndef s6_addr32
#define s6_addr32 __u6_addr.__u6_addr32
#endif /* s6_addr32 */

static inline int ofi_is_loopback_addr(struct sockaddr *addr) {
	return (addr->sa_family == AF_INET &&
		((struct sockaddr_in *)addr)->sin_addr.s_addr == htonl(INADDR_LOOPBACK)) ||
		(addr->sa_family == AF_INET6 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.s6_addr32[0] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.s6_addr32[1] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.s6_addr32[2] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.s6_addr32[3] == htonl(1));
}

#if !HAVE_CLOCK_GETTIME

#define CLOCK_REALTIME 0
#define CLOCK_REALTIME_COARSE 0
#define CLOCK_MONOTONIC 0

typedef int clockid_t;

int clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif /* !HAVE_CLOCK_GETTIME */

/* complex operations implementation */

typedef float complex __attribute__((aligned(8))) ofi_complex_float;
typedef double complex __attribute__((aligned(8))) ofi_complex_double;
typedef long double complex __attribute__((aligned(8))) ofi_complex_long_double;

#define OFI_DEF_COMPLEX_OPS(type)				\
static inline int ofi_complex_eq_## type			\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a == b;						\
}								\
static inline ofi_complex_## type ofi_complex_sum_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a + b;						\
}								\
static inline ofi_complex_## type ofi_complex_prod_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a * b;						\
}								\
static inline ofi_complex_## type ofi_complex_land_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a && b;      					\
}								\
static inline ofi_complex_## type ofi_complex_lor_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a || b;						\
}								\
static inline int ofi_complex_lxor_## type			\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return (a && !b) || (!a && b);				\
}								\

OFI_DEF_COMPLEX_OPS(float)
OFI_DEF_COMPLEX_OPS(double)
OFI_DEF_COMPLEX_OPS(long_double)

#ifdef HAVE_ATOMICS
#  include <stdatomic.h>

#elif defined(HAVE_BUILTIN_ATOMICS) || defined(HAVE_BUILTIN_MM_ATOMICS)

/* atomics primitives */
#define memory_order_relaxed __ATOMIC_RELAXED
#define memory_order_consume __ATOMIC_CONSUME
#define memory_order_acquire __ATOMIC_ACQUIRE
#define memory_order_release __ATOMIC_RELEASE
#define memory_order_acq_rel __ATOMIC_ACQ_REL
#define memory_order_seq_cst __ATOMIC_SEQ_CST

#else
/* emulated atomics, no ordering specification allowed */
#define memory_order_relaxed 0
#define memory_order_consume 0
#define memory_order_acquire 0
#define memory_order_release 0
#define memory_order_acq_rel 0
#define memory_order_seq_cst 0

#endif /* HAVE_ATOMICS */

#ifdef HAVE_BUILTIN_ATOMICS
#define ofi_atomic_add_and_fetch(radix, ptr, val) __sync_add_and_fetch((ptr), (val))
#define ofi_atomic_sub_and_fetch(radix, ptr, val) __sync_sub_and_fetch((ptr), (val))
#define ofi_atomic_cas_bool(radix, ptr, expected, desired) 	\
	__sync_bool_compare_and_swap((ptr), (expected), (desired))
#define ofi_atomic_compare_exchange_weak(radix, ptr, expected, desired, \
					 succ_memmodel, fail_memmodel) \
	__atomic_compare_exchange_n(ptr, expected, desired, true, \
				    succ_memmodel, fail_memmodel)
#define ofi_atomic_store_explicit(radix, ptr, value, memmodel) \
	__atomic_store_n(ptr, value, memmodel)
#define ofi_atomic_load_explicit(radix, ptr, memmodel) \
	__atomic_load_n(ptr, memmodel)
#endif /* HAVE_BUILTIN_ATOMICS */

int ofi_set_thread_affinity(const char *s);


#if defined(HAVE_CPUID) && (defined(__x86_64__) || defined(__amd64__))

#include <cpuid.h>

static inline void
ofi_cpuid(unsigned func, unsigned subfunc, unsigned cpuinfo[4])
{
	__cpuid_count(func, subfunc, cpuinfo[0], cpuinfo[1],
		      cpuinfo[2], cpuinfo[3]);
}

#define ofi_clwb(addr) \
	asm volatile(".byte 0x66; xsaveopt %0" : "+m" (*(volatile char *) (addr)))
#define ofi_clflushopt(addr) \
	asm volatile(".byte 0x66; clflush %0" : "+m" (*(volatile char *) addr))
#define ofi_clflush(addr) \
	asm volatile("clflush %0" : "+m" (*(volatile char *) addr))
#define ofi_sfence() asm volatile("sfence" ::: "memory")

#elif  (defined(__riscv) && \
	defined(__riscv_xlen) && \
	(__riscv_xlen == 64) && \
	defined(__linux__) )

#define ofi_cpuid(func, subfunc, cpuinfo)
#define ofi_clwb(addr)
#define ofi_clflushopt(addr)
#define ofi_clflush(addr)
#define ofi_sfence() asm volatile("fence w,w" ::: "memory")

#else /* defined(__x86_64__) || defined(__amd64__) || defined(__riscv) */

#define ofi_cpuid(func, subfunc, cpuinfo)
#define ofi_clwb(addr)
#define ofi_clflushopt(addr)
#define ofi_clflush(addr)
#define ofi_sfence()

#endif /* defined(__x86_64__) || defined(__amd64__) */

#endif /* _FI_UNIX_OSD_H_ */
