/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "psm_user.h"
#include "cmarw.h"

/* An iovec looks like this:
 * struct iovec {
 *       void  *iov_base;    // Starting address
 *       size_t iov_len;     // Number of bytes to transfer
 * };
 */

#if 0
#define __NR_process_vm_readv			310
#define __NR_process_vm_writev			311

#define process_vm_readv(pid, local_iov, liovcnt, remote_iov, riovcnt, flags) \
	syscall(__NR_process_vm_readv, \
		pid, local_iov, liovcnt, remote_iov, riovcnt, flags)

#define process_vm_writev(pid, local_iov, liovcnt, remote_iov, riovcnt, flags) \
	syscall(__NR_process_vm_writev, \
		pid, local_iov, liovcnt, remote_iov, riovcnt, flags)
#endif

/*CMA syscall wrappers were added in glibc 2.15.  For anything older than that,
   we need to define our own wrappers.  Apparently older (and maybe newer?)
   (2.12 from RHEL6.3 definitely has this bug) glibcs only pass up to 5
   arguments via the generic syscall() function.  These CMA functions, however,
   have 6 arguments.  So for now, we hack our way around it by generating ASM
   code for doing a syscall directly.
*/

#if defined(__GLIBC__) && ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 15))

#ifdef __x86_64__

#define __NR_process_vm_readv			310
#define __NR_process_vm_writev			311

static inline ssize_t __x86_64_syscall6(int syscall,
					pid_t pid,
					const struct iovec *local_iov,
					unsigned long liovcnt,
					const struct iovec *remote_iov,
					unsigned long riovcnt,
					unsigned long flags)
{
	/*GCC inline ASM is annoying -- can't specify all the x86_64 registers
	   directly, so declare register-specific variables and use them. */
	register int64_t rax asm("rax") = syscall;
	register int64_t rdi asm("rdi") = pid;
	register int64_t rsi asm("rsi") = (intptr_t) local_iov;
	register int64_t rdx asm("rdx") = liovcnt;
	register int64_t r10 asm("r10") = (intptr_t) remote_iov;
	register int64_t r8 asm("r8") = riovcnt;
	register int64_t r9 asm("r9") = flags;

	asm volatile ("syscall\n" : "=a" (rax)
		      : "r"(rax), "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10),
		      "r"(r8), "r"(r9)
		      : "%rcx", "%r11", "cc", "memory");
	return rax;
}

#define process_vm_readv(pid, local_iov, liovcnt, remote_iov, riovcnt, flags) \
	__x86_64_syscall6(__NR_process_vm_readv, \
			  pid, local_iov, liovcnt, remote_iov, riovcnt, flags)

#define process_vm_writev(pid, local_iov, liovcnt, remote_iov, riovcnt, flags) \
	__x86_64_syscall6(__NR_process_vm_writev, \
			  pid, local_iov, liovcnt, remote_iov, riovcnt, flags)

#else /* ndef __x86_64__ */
#error "Can't compile CMA support for this architecture."
#endif /* __x86_64__ */
#endif /* __GLIBC__ < 2.15 */

int64_t psm3_cma_get(pid_t pid, const void *src, void *dst, int64_t n)
{
	int64_t nr, sum;
	struct iovec local = {
		.iov_base = dst,
		.iov_len = n
	};
	struct iovec remote = {
		.iov_base = (void *)src,
		.iov_len = n
	};
	nr = sum = 0;
	while (sum != n) {
		nr = process_vm_readv(pid, &local, 1, &remote, 1, 0);
		if (nr == -1) {
			return -1;
		}
		sum += nr;
		local.iov_base = (void *)((uint8_t *)local.iov_base + nr);
		local.iov_len -= nr;
		remote.iov_base = (void *)((uint8_t *)remote.iov_base + nr);
		remote.iov_len -= nr;
	}
	return sum;
}

int64_t psm3_cma_put(const void *src, pid_t pid, void *dst, int64_t n)
{
	int64_t nr, sum;
	struct iovec local = {
		.iov_base = (void *)src,
		.iov_len = n
	};
	struct iovec remote = {
		.iov_base = dst,
		.iov_len = n
	};

	nr = sum = 0;
	while (sum != n) {
		nr = process_vm_writev(pid, &local, 1, &remote, 1, 0);
		if (nr == -1) {
			return -1;
		}
		sum += nr;
		local.iov_base = (void *)((uint8_t *)local.iov_base + nr);
		local.iov_len -= nr;
		remote.iov_base = (void *)((uint8_t *)remote.iov_base + nr);
		remote.iov_len -= nr;
	}
	return sum;
}

/* Test if CMA is available by trying a no-op call. */
int psm3_cma_available(void)
{

	/* Make a no-op CMA syscall. If CMA is present, 0 (bytes transferred)
	 * should be returned.  If not present, expect -ENOSYS. */

	int ret = process_vm_readv(getpid(), NULL, 0, NULL, 0, 0);

	if (ret == 0) {
		/* CMA is available! */
		return 1;
	}

	return 0;
}
