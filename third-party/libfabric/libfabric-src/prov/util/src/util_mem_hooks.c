/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2009-2017 Cisco Systems, Inc.  All rights reserved
 * Copyright (c) 2013-2018 Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2016-2017 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2016-2020 IBM Corporation.  All rights reserved.
 * Copyright (c) 2019 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2023 Tactical Computing Labs, LLC. All rights reserved.
 *
 * License text from Open-MPI (www.open-mpi.org/community/license.php)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer listed
 * in this license in the documentation and/or other materials
 * provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ofi_mr.h>
#include <ofi_mem.h>

static int ofi_memhooks_start(struct ofi_mem_monitor *monitor);
static void ofi_memhooks_stop(struct ofi_mem_monitor *monitor);

struct ofi_memhooks memhooks = {
	.monitor.iface = FI_HMEM_SYSTEM,
	.monitor.init = ofi_monitor_init,
	.monitor.cleanup = ofi_monitor_cleanup,
	.monitor.start = ofi_memhooks_start,
	.monitor.stop = ofi_memhooks_stop,
	.monitor.name = "memhooks",
};
struct ofi_mem_monitor *memhooks_monitor = &memhooks.monitor;


/* memhook support checks */
#if HAVE_MEMHOOKS_MONITOR

#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <link.h>

#if HAVE_DECL___SYSCALL && defined(HAVE___SYSCALL)
/* calling __syscall is preferred on some systems when some arguments may be 64-bit. it also
 * has the benefit of having an off_t return type */
#define ofi_memhooks_syscall __syscall
#else
#define ofi_memhooks_syscall syscall
#endif

// These op codes used to be in bits/ipc.h but were removed in glibc in 2015
// with a comment saying they should be defined in internal headers:
// https://sourceware.org/bugzilla/show_bug.cgi?id=18560
// and when glibc uses that syscall it seems to do so from its own definitions:
// https://github.com/bminor/glibc/search?q=IPCOP_shmat&unscoped_q=IPCOP_shmat
#if (!defined(SYS_shmat) && !defined(IPCOP_shmat))
#define IPCOP_shmat                21
#endif
#if (!defined(SYS_shmdt) && !defined(IPCOP_shmdt))
#define IPCOP_shmdt                22
#endif

#define OFI_INTERCEPT_MAX_PATCH 32

static bool symbols_intercepted;

struct ofi_intercept {
	struct dlist_entry 		entry;
	const char			*symbol;
	void				*our_func;
	void				*orig_func;
	unsigned char			patch_data[OFI_INTERCEPT_MAX_PATCH]
		                        __attribute__ ((aligned(32)));
	unsigned char			patch_orig_data[OFI_INTERCEPT_MAX_PATCH];
	unsigned			patch_data_size;
	struct dlist_entry		dl_intercept_list;
};

enum {
	OFI_INTERCEPT_MMAP,
	OFI_INTERCEPT_MUNMAP,
	OFI_INTERCEPT_MREMAP,
	OFI_INTERCEPT_MADVISE,
	OFI_INTERCEPT_SHMAT,
	OFI_INTERCEPT_SHMDT,
	OFI_INTERCEPT_BRK,
	OFI_INTERCEPT_MAX
};

static void *ofi_intercept_mmap(void *start, size_t length,
				int prot, int flags, int fd, off_t offset);
static int ofi_intercept_munmap(void *start, size_t length);
static void *ofi_intercept_mremap(void *old_address, size_t old_size,
		size_t new_size, int flags, void *new_address);
static int ofi_intercept_madvise(void *addr, size_t length, int advice);
static void *ofi_intercept_shmat(int shmid, const void *shmaddr, int shmflg);
static int ofi_intercept_shmdt(const void *shmaddr);
static int ofi_intercept_brk(const void *brkaddr);

static struct ofi_intercept intercepts[] = {
	[OFI_INTERCEPT_MMAP] = { .symbol = "mmap",
				.our_func = ofi_intercept_mmap},
	[OFI_INTERCEPT_MUNMAP] = { .symbol = "munmap",
				.our_func = ofi_intercept_munmap},
	[OFI_INTERCEPT_MREMAP] = { .symbol = "mremap",
				.our_func = ofi_intercept_mremap},
	[OFI_INTERCEPT_MADVISE] = { .symbol = "madvise",
				.our_func = ofi_intercept_madvise},
	[OFI_INTERCEPT_SHMAT] = { .symbol = "shmat",
				.our_func = ofi_intercept_shmat},
	[OFI_INTERCEPT_SHMDT] = { .symbol = "shmdt",
				.our_func = ofi_intercept_shmdt},
	[OFI_INTERCEPT_BRK] = { .symbol = "brk",
				.our_func = ofi_intercept_brk},
};

#ifdef HAVE___CURBRK
extern void *__curbrk; /* in libc */
#endif

#ifdef HAVE___CLEAR_CACHE
/*
 * Used on ARM64 platforms, see https://github.com/open-mpi/ompi/issues/5631
 */
static inline void ofi_clear_instruction_cache(uintptr_t address, size_t data_size)
{
	/* do not allow global declaration of compiler intrinsic */
	void __clear_cache(void* beg, void* end);

	__clear_cache ((void *) address, (void *) (address + data_size));
}
#else
static inline void ofi_clear_instruction_cache(uintptr_t address, size_t data_size)
{
	size_t i;
	size_t offset_jump = 16;
#if (defined(__aarch64__) || (defined(__riscv) && (__riscv_xlen == 64)))
	offset_jump = 32;
#endif
	/* align the address */
	address &= ~(offset_jump - 1);

	for (i = 0 ; i < data_size ; i += offset_jump) {
#if (defined(__x86_64__) || defined(__amd64__))
		__asm__ volatile("mfence;clflush %0;mfence"::
				 "m" (*((char*) address + i)));
#elif (defined(__aarch64__))
		__asm__ volatile ("dc cvau, %0\n\t"
			  "dsb ish\n\t"
			  "ic ivau, %0\n\t"
			  "dsb ish\n\t"
			  "isb":: "r" (address + i));
#elif (defined(__riscv) && (__riscv_xlen == 64))
	        __riscv_flush_icache(address, address+data_size, SYS_RISCV_FLUSH_ICACHE_LOCAL);
		__asm__ volatile ("fence.i\n");
#endif
	}
}
#endif

static inline int ofi_write_patch(unsigned char *patch_data, void *address,
				  size_t data_size)
{
	long page_size;
	void *base;
	void *bound;
	size_t length;

	page_size = ofi_get_page_size();
	if (page_size < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"failed to get page size: %s\n", fi_strerror(-page_size));
		return page_size;
	}

	base = ofi_get_page_start(address, page_size);
	bound = ofi_get_page_end( (void *) ((uintptr_t) address + data_size - 1), page_size);
	length = (uintptr_t) bound - (uintptr_t) base + 1;

	if (mprotect(base, length, PROT_EXEC|PROT_READ|PROT_WRITE)) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"mprotect to set PROT_WRITE on %p len %lu failed: %s\n",
			(void *) base, length, strerror(errno));
		return -errno;
	}

	memcpy(address, patch_data, data_size);

	ofi_clear_instruction_cache((uintptr_t) address, data_size);

	/*
	 * Nothing we can do here if this fails so ignore the return code. It
	 * shouldn't due to alignment since the parameters are the same as
	 * before.
	 */
	if (mprotect(base, length, PROT_EXEC|PROT_READ))
		FI_WARN(&core_prov, FI_LOG_MR,
			"mprotect to drop PROT_WRITE on %p len %lu failed: %s\n",
			 base, length, strerror(errno));

	return 0;
}

static int ofi_apply_patch(struct ofi_intercept *intercept)
{
	memcpy(intercept->patch_orig_data, intercept->orig_func,
	       intercept->patch_data_size);
	return ofi_write_patch(intercept->patch_data, intercept->orig_func,
			       intercept->patch_data_size);
}

static int ofi_remove_patch(struct ofi_intercept *intercept)
{
	return ofi_write_patch(intercept->patch_orig_data, intercept->orig_func,
			       intercept->patch_data_size);
}

static void ofi_restore_intercepts(void)
{
	struct ofi_intercept *intercept;

	dlist_foreach_container(&memhooks.intercept_list, struct ofi_intercept,
		intercept, entry)
		ofi_remove_patch(intercept);
}

#if (defined(__x86_64___) || defined(__amd64__))
static int ofi_patch_function(struct ofi_intercept *intercept)
{
	intercept->patch_data_size = 13;
	*(unsigned short*)(intercept->patch_data + 0) = 0xbb49;
	*(unsigned long* )(intercept->patch_data + 2) =
		(unsigned long) intercept->our_func;
	*(unsigned char*) (intercept->patch_data +10) = 0x41;
	*(unsigned char*) (intercept->patch_data +11) = 0xff;
	*(unsigned char*) (intercept->patch_data +12) = 0xe3;

	return ofi_apply_patch(intercept);
}

/*
 * Check to see if there exists at the specified intercept location the
 * pattern of bytes used in ofi_patch_function() to patch in our own
 * monitoring function. These bytes (roughly) represent opcodes to load
 * a supplied address into a register and execute a jump to that location.
 * If they do already exist, then we've almost certainly already patched.
 * Note that we are explicitly ignoring the target address in these checks
 * as that is a transient value and not invarient as the other values are.
 */
static bool ofi_is_function_patched(struct ofi_intercept *intercept)
{
	return (
		(*(unsigned short*)((uintptr_t)intercept->orig_func + 0) == 0xbb49) &&
		(*(unsigned char* )((uintptr_t)intercept->orig_func +10) == 0x41  ) &&
		(*(unsigned char* )((uintptr_t)intercept->orig_func +11) == 0xff  ) &&
		(*(unsigned char* )((uintptr_t)intercept->orig_func +12) == 0xe3  )
	);
}
#elif defined(__aarch64__)
/**
 * @brief Generate a mov immediate instruction
 *
 * @param[in] reg   register number (0-31)
 * @param[in] shift shift amount (0-3) * 16-bits
 * @param[in] value immediate value
 */
static uint32_t mov(unsigned int reg, uint16_t shift, uint16_t value)
{
	return (0x1a5 << 23) + ((uint32_t) shift << 21) + ((uint32_t) value << 5) + reg;
}

/**
 * @brief Generate a mov immediate with keep instruction
 *
 * @param[in] reg   register number (0-31)
 * @param[in] shift shift amount (0-3) * 16-bits
 * @param[in] value immediate value
 */
static uint32_t movk(unsigned int reg, uint16_t shift, uint16_t value)
{
	return (0x1e5 << 23) + ((uint32_t) shift << 21) + ((uint32_t) value << 5) + reg;
}

/**
 * @brief Generate a branch to register instruction
 *
 * @param[in] reg   register number (0-31)
 */
static uint32_t br(unsigned int reg)
{
	return (0xd61f << 16) + (reg << 5);
}

static int ofi_patch_function(struct ofi_intercept *intercept)
{
	/*
	 * r15 is the highest numbered temporary register. I am
	 * assuming this one is safe to use.
	 */
	const unsigned int gr = 15;
	uintptr_t addr = (uintptr_t) intercept->patch_data;
	uintptr_t value = (uintptr_t) intercept->our_func;

	*(uint32_t *) (addr +  0) = mov(gr, 3, value >> 48);
	*(uint32_t *) (addr +  4) = movk(gr, 2, value >> 32);
	*(uint32_t *) (addr +  8) = movk(gr, 1, value >> 16);
	*(uint32_t *) (addr + 12) = movk(gr, 0, value);
	intercept->patch_data_size = 16;

	*(uint32_t *) ((uintptr_t) intercept->patch_data +
		       intercept->patch_data_size) = br(gr);
	intercept->patch_data_size = intercept->patch_data_size + 4;

	return ofi_apply_patch(intercept);
}

/*
 * Please see comments at other ofi_is_function_patched() function
 */
static bool ofi_is_function_patched(struct ofi_intercept *intercept)
{
    uint32_t mov_mask=~((0xFFFF << 5) | 0x1F);
    uint32_t br_mask=~(0x1F << 5);
    uintptr_t addr = (uintptr_t) intercept->orig_func;
    /*
     * Register 15 is used in our patching code, but for checking here let's
     * ignore the register value and instead focus on the surrounding bytes.
     */
    return (
        ((*(uint32_t *) (addr +  0)) & mov_mask) == mov(0, 3, 0) &&
        ((*(uint32_t *) (addr +  4)) & mov_mask) == movk(0, 2, 0) &&
        ((*(uint32_t *) (addr +  8)) & mov_mask) == movk(0, 1, 0) &&
        ((*(uint32_t *) (addr + 12)) & mov_mask) == movk(0, 0, 0) &&
        ((*(uint32_t *) (addr + 16)) & br_mask) == br(0)
	);
}
#elif (defined(__riscv) && \
       defined(__riscv_xlen) && \
       (__riscv_xlen == 64))

/* Registers numbers to use with the move immediate to register.
 * The destination register is X31 (highest temporary).
 * Register X28-X30 are used for block shifting and masking.
 * Register X0 is always zero
 */
#define X31 31
#define X30 30
#define X0  0

/**
 * jalr
 *
 * Add 12 bit immediate to source register
 * save to destination register
 * jump and link from destination register
 *
 */
#define jalr(_regd, _regs, _imm) \
    (((_imm) << 20) | ((_regs) << 15) | (0b000 << 12) | ((_regd) << 7) | (0x67))

/**
 * addi
 *
 * Add 12 bit immediate to source register
 * save to destination register 
 *
 */
#define addi(_regd, _regs, _imm) \
    (((_imm) << 20) | ((_regs) << 15) | (0b000 << 12) | ((_regd) << 7) | (0x13))

#define add(_regd, _regs_a, _regs_b) \
    ((_regs_b << 20) | (_regs_a << 15) | (0b000 << 12) | ((_regd) << 7) | (0x33))

/**
 * lui
 *
 * load upper 20 bit immediate to destination register
 *
 */
#define lui(_regd, _imm) \
    (((_imm) << 12) | ((_regd) << 7) | (0x37))

/**
 * slli
 *
 * left-shift immediate number of bits in source register into destination register
 *
 */
#define slli(_regd, _regs, _imm) \
    (((_imm) << 20) | ((_regs) << 15) | (0b001 << 12) | ((_regd) << 7) | (0x13))

static int ofi_patch_function(struct ofi_intercept *intercept)
{
    /*
     * r15 is the highest numbered temporary register. I am
     * assuming this one is safe to use.
     */
    uintptr_t addr __attribute__ ((aligned(32))) = (uintptr_t) intercept->patch_data;
    uintptr_t value __attribute__ ((aligned(32))) = (uintptr_t) intercept->our_func;

    uint32_t fnaddr_hi = (uint32_t)(value >> 32) + ((uint32_t)(value >> 31) & 1);
    uint32_t fnaddr_lo = (uint32_t) value;

    *(uint32_t*) (addr +  0) = lui  (X31, ((fnaddr_hi >> 12) + ((fnaddr_hi >> 11) & 1)) & 0xFFFFF);
    *(uint32_t*) (addr +  4) = addi (X31, X31, fnaddr_hi & 0xFFF );
    *(uint32_t*) (addr +  8) = lui  (X30, ((fnaddr_lo >> 12) + ((fnaddr_lo >> 11) & 1)) & 0xFFFFF);
    *(uint32_t*) (addr + 12) = slli (X31, X31, 32);
    *(uint32_t*) (addr + 16) = add  (X31, X31, X30);
    *(uint32_t*) (addr + 20) = jalr (X0, X31, fnaddr_lo & 0xFFF);

    intercept->patch_data_size = 24;

    return ofi_apply_patch(intercept);
}

/*
 * Please see comments at other ofi_is_function_patched() function
 */
static bool ofi_is_function_patched(struct ofi_intercept *intercept)
{
    uintptr_t addr = (uintptr_t) intercept->orig_func;
    /*
     * focus on the instructions in the bytes.
     */
    return (
        ((*(uint32_t *) (addr +  0)) & 0xFF) == 0x37 &&
        ((*(uint32_t *) (addr +  4)) & 0xFF) == 0x13 &&
        ((*(uint32_t *) (addr +  8)) & 0xFF) == 0x37 &&
        ((*(uint32_t *) (addr + 12)) & 0xFF) == 0x13 &&
        ((*(uint32_t *) (addr + 16)) & 0xFF) == 0x33 &&
        ((*(uint32_t *) (addr + 20)) & 0xFF) == 0x67
	);
}
#endif

/*
 * This implementation intercepts syscalls by overwriting the beginning of
 * glibc's functions with a jump to our intercept function. After notifying the
 * cache we will make the syscall directly. We store the original instructions
 * and restore them when memhooks is unloaded.
 */
static int ofi_intercept_symbol(struct ofi_intercept *intercept)
{
	void *func_addr;
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "overwriting function %s\n", intercept->symbol);

	func_addr = dlsym(RTLD_NEXT, intercept->symbol);
	if (!func_addr) {
		func_addr = dlsym(RTLD_DEFAULT, intercept->symbol);
		if (!func_addr) {
			FI_DBG(&core_prov, FI_LOG_MR,
			       "could not find symbol %s\n", intercept->symbol);
			ret = -FI_ENOMEM;
			return ret;
		}
	}

	intercept->orig_func = func_addr;

	if (ofi_is_function_patched(intercept)) {
		FI_DBG(&core_prov, FI_LOG_MR,
				"function %s is already patched; stopping further patching\n",
				intercept->symbol);
		ret = -FI_EALREADY;
	} else {
		ret = ofi_patch_function(intercept);
	}

	if (!ret)
		dlist_insert_tail(&intercept->entry, &memhooks.intercept_list);

	return ret;
}

void ofi_intercept_handler(const void *addr, size_t len)
{
	pthread_rwlock_rdlock(&mm_list_rwlock);
	pthread_mutex_lock(&mm_lock);
	ofi_monitor_notify(memhooks_monitor, addr, len);
	pthread_mutex_unlock(&mm_lock);
	pthread_rwlock_unlock(&mm_list_rwlock);
}

static void *ofi_intercept_mmap(void *start, size_t length,
                            int prot, int flags, int fd, off_t offset)
{
	if ((flags & MAP_FIXED) && start)
		ofi_intercept_handler(start, length);

	return (void *)(intptr_t) ofi_memhooks_syscall(SYS_mmap, start, length,
						       prot, flags, fd, offset);
}

static int ofi_intercept_munmap(void *start, size_t length)
{
	ofi_intercept_handler(start, length);

	return ofi_memhooks_syscall(SYS_munmap, start, length);
}

static void *ofi_intercept_mremap(void *old_address, size_t old_size,
		size_t new_size, int flags, void *new_address)
{
	ofi_intercept_handler(old_address, old_size);

#ifdef MREMAP_FIXED
	/*
	 * new_address is an optional argument. Explicitly set it to NULL
	 * if it is not applicable.
	 */
	if (!(flags & MREMAP_FIXED))
		new_address = NULL;
#endif

	return (void *)(intptr_t) ofi_memhooks_syscall(SYS_mremap, old_address,
						       old_size, new_size,
						       flags, new_address);
}

static int ofi_intercept_madvise(void *addr, size_t length, int advice)
{
	if (advice == MADV_DONTNEED ||
#ifdef MADV_FREE
	    advice == MADV_FREE ||
#endif
#ifdef MADV_REMOVE
	    advice == MADV_REMOVE ||
#endif
	    advice == POSIX_MADV_DONTNEED) {
		ofi_intercept_handler(addr, length);
	}

	return ofi_memhooks_syscall(SYS_madvise, addr, length, advice);
}

static void *ofi_intercept_shmat(int shmid, const void *shmaddr, int shmflg)
{
	struct shmid_ds ds;
	const void *start;
	void *result;
	size_t len;
	int ret;

	if (shmaddr && (shmflg & SHM_REMAP)) {
		ret = shmctl(shmid, IPC_STAT, &ds);
		len = (ret < 0) ? 0 : ds.shm_segsz;

		if (shmflg & SHM_RND) {
			start = (char *) shmaddr - ((uintptr_t) shmaddr) % SHMLBA;
			len += ((uintptr_t) shmaddr) % SHMLBA;
		} else {
			start = shmaddr;
		}

		ofi_intercept_handler(start, len);
	}

#ifdef SYS_shmat
	result = (void *) ofi_memhooks_syscall(SYS_shmat, shmid, shmaddr, shmflg);
#else // IPCOP_shmat
	unsigned long sysret;
	sysret = ofi_memhooks_syscall(SYS_ipc, IPCOP_shmat,
				      shmid, shmflg, &shmaddr, shmaddr);
	result = (sysret > -(unsigned long)SHMLBA) ? (void *)sysret :
						     (void *)shmaddr;
#endif
	return result;
}

static int ofi_intercept_shmdt(const void *shmaddr)
{
	int ret;

	/*
	 * Overly aggressive, but simple.  Invalidate everything after shmaddr.
	 * We could choose to find the shared memory segment size in /proc but
	 * that seems like a great way to deadlock ourselves.
	 */
	ofi_intercept_handler(shmaddr, SIZE_MAX - (uintptr_t) shmaddr);

#ifdef SYS_shmdt
	ret = ofi_memhooks_syscall(SYS_shmdt, shmaddr);
#else // IPCOP_shmdt
	ret = ofi_memhooks_syscall(SYS_ipc, IPCOP_shmdt, 0, 0, 0, shmaddr);
#endif
	return ret;
}

static int ofi_intercept_brk(const void *brkaddr)
{
	void *old_addr, *new_addr;

#ifdef HAVE___CURBRK
	old_addr = __curbrk;
#else
	old_addr = sbrk(0);
#endif
	new_addr = (void *) (intptr_t) ofi_memhooks_syscall(SYS_brk, brkaddr);

#ifdef HAVE___CURBRK
	/*
	 * Note: if we were using glibc brk/sbrk, their __curbrk would get
	 * updated, but since we're going straight to the syscall, we have
	 * to update __curbrk or else glibc won't see it.
	 */
	__curbrk = new_addr;
#endif

	if (new_addr < brkaddr) {
		errno = ENOMEM;
		return -1;
	} else if (new_addr < old_addr) {
		ofi_intercept_handler(new_addr, (intptr_t) old_addr -
				      (intptr_t) new_addr);
	}

	return 0;
}

static int ofi_memhooks_subscribe(struct ofi_mem_monitor *monitor,
				  const void *addr, size_t len,
				  union ofi_mr_hmem_info *hmem_info)
{
	/* no-op */
	return FI_SUCCESS;
}

static void ofi_memhooks_unsubscribe(struct ofi_mem_monitor *monitor,
				     const void *addr, size_t len,
				     union ofi_mr_hmem_info *hmem_info)
{
	/* no-op */
}

static bool ofi_memhooks_valid(struct ofi_mem_monitor *monitor,
			       const struct ofi_mr_info *info,
			       struct ofi_mr_entry *entry)
{
	/* no-op */
	return true;
}

static int ofi_memhooks_start(struct ofi_mem_monitor *monitor)
{
	int i, ret;

	if (memhooks_monitor->subscribe == ofi_memhooks_subscribe)
		return 0;

	memhooks_monitor->subscribe = ofi_memhooks_subscribe;
	memhooks_monitor->unsubscribe = ofi_memhooks_unsubscribe;
	memhooks_monitor->valid = ofi_memhooks_valid;
	dlist_init(&memhooks.intercept_list);

	for (i = 0; i < OFI_INTERCEPT_MAX; ++i)
		dlist_init(&intercepts[i].dl_intercept_list);

	for (i = 0; i < OFI_INTERCEPT_MAX; ++i) {
		ret = ofi_intercept_symbol(&intercepts[i]);
		if (ret != 0) {
			FI_DBG(&core_prov, FI_LOG_MR,
				"intercept %s failed %d %s\n", intercepts[i].symbol,
					ret, fi_strerror(ret));
			goto err_intercept_failed;
		}
	}

	symbols_intercepted = true;

	return 0;

err_intercept_failed:
	while (--i >= 0)
		ofi_remove_patch(&intercepts[i]);
	memhooks_monitor->subscribe = NULL;
	memhooks_monitor->unsubscribe = NULL;

	return ret;
}

static void ofi_memhooks_stop(struct ofi_mem_monitor *monitor)
{
	ofi_restore_intercepts();
	memhooks_monitor->subscribe = NULL;
	memhooks_monitor->unsubscribe = NULL;
}

void ofi_memhooks_atfork_handler(void)
{
	if (symbols_intercepted)
		ofi_restore_intercepts();
}

#else

static int ofi_memhooks_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

static void ofi_memhooks_stop(struct ofi_mem_monitor *monitor)
{
}

void ofi_memhooks_atfork_handler(void)
{
}

#endif /* memhook support checks */
