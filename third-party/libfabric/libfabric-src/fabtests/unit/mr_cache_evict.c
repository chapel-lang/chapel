/*
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <unistd.h>
#include <stddef.h>
#include <sys/mman.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <malloc.h>

#include "unit_common.h"
#include "shared.h"
#include "hmem.h"

/* Supported memory region types. */
enum alloc_type {
	MMAP,
	BRK,
	SBRK,
	CUDA,
	ROCR,
};

static void *reuse_addr = NULL;
static char err_buf[512];
static size_t mr_buf_size = 16384;

/* Given a time value, determine the expected cached time value. The assumption
 * is the cache value should at least have a CACHE_IMPROVEMENT_PERCENT time
 * improvement over the original time value.
 */
#define CACHE_IMPROVEMENT_PERCENT 80
#define CACHE_TIME_MAX_VALUE(time) ((time) / 100 * \
				    (100 - CACHE_IMPROVEMENT_PERCENT))

#define PAGEMAP_ENTRY_SIZE 8
#define PAGEMAP_PFN_PRESENT (1ULL << 63)
#define PAGEMAP_PFN_MASK ((1ULL << 55) - 1)

/* Function used to get physical address from a virtual address. Must be root
 * to read pagemap.
 */
static int virt_to_phys(const void *va_addr, uint64_t *phy_addr)
{
	int fd;
	int ret;
	uint64_t entry;
	ssize_t read_size;
	off_t seek_ret;
	off_t seek_offset;
	int page_size;

	fd = open("/proc/self/pagemap", O_RDONLY);
	if (fd == -1) {
		ret = -errno;
		FT_UNIT_STRERR(err_buf, "open of /proc/self/pagemap failed",
			       ret);
		return ret;
	}

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1) {
		ret = -errno;
		FT_UNIT_STRERR(err_buf, "sysconf(_SC_PAGESIZE) failed", ret);
		goto out;
	}

	/* Map virtual address to offset in pagemap. */
	seek_offset = (uintptr_t) va_addr / page_size * PAGEMAP_ENTRY_SIZE;

	seek_ret = lseek(fd, seek_offset, SEEK_SET);
	if (seek_ret == -1) {
		ret = -errno;
		FT_UNIT_STRERR(err_buf, "lseek failed", ret);
		goto out;
	}

	read_size = read(fd, &entry, sizeof(entry));
	if (read_size == -1) {
		ret = -errno;
		FT_UNIT_STRERR(err_buf, "read failed", ret);
		goto out;
	} else if (read_size != sizeof(entry)) {
		ret = -ENOSPC;
		FT_UNIT_STRERR(err_buf, "short read", ret);
		goto out;
	}

	if (entry & PAGEMAP_PFN_PRESENT) {
		ret = 0;
		*phy_addr = (entry & PAGEMAP_PFN_MASK) * page_size;
	} else {
		ret = -EFAULT;
		FT_UNIT_STRERR(err_buf, "Failed to find physical address", ret);
	}

out:
	close(fd);

	return ret;
}

/* Sbrk/brk allocations are only intended to support a single outstanding
 * allocation at a time. Extra handling of the program break is needed to make
 * sbrk/brk allocations more flexible including making allocations thread safe.
 */
static void sbrk_free(void *ptr)
{
	void *cur_brk = (void *) ((uint64_t) ptr + mr_buf_size);
	void *rewind_brk = ptr;

	FT_DEBUG("Resetting program break from %p to %p", cur_brk, rewind_brk);
	cur_brk = sbrk(-(intptr_t) mr_buf_size);
	if (cur_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return;
	}

	/* Verify the program break was reset to the expected location. */
	cur_brk = sbrk(0);
	if (cur_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return;
	}

	if (cur_brk != rewind_brk)
		FT_UNIT_STRERR(err_buf, "Failed to reset program break",
			       -ENOMEM);
}

static void *sbrk_alloc(void)
{
	void *prev_brk;
	void *cur_brk;

	prev_brk = sbrk((intptr_t) mr_buf_size);
	if (prev_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return NULL;
	}

	/* Determine the size of the newly allocated buffer. If this operation
	 * fails, memory is leaked.
	 */
	cur_brk = sbrk(0);
	if (cur_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return NULL;
	}

	FT_DEBUG("Moved program break from %p to %p", prev_brk, cur_brk);

	return prev_brk;
}

static void brk_free(void *ptr)
{
	void *cur_brk = (void *) ((uint64_t) ptr + mr_buf_size);
	void *rewind_brk = ptr;
	int ret;

	FT_DEBUG("Resetting program break from %p to %p", cur_brk, rewind_brk);
	ret = brk(rewind_brk);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "brk failed", -errno);
		return;
	}

	/* Verify the program break was reset the the expected location. */
	cur_brk = sbrk(0);
	if (cur_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return;
	}

	if (cur_brk != rewind_brk)
		FT_UNIT_STRERR(err_buf, "Failed to reset program break",
			       -ENOMEM);
}

static void *brk_alloc(void)
{
	void *prev_brk;
	void *cur_brk;
	int ret;

	/* Use sbrk to determine the current program break. This is needed to
	 * determine the brk allocation size.
	 */
	prev_brk = sbrk(0);
	if (prev_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return NULL;
	}

	cur_brk = (void *) ((intptr_t) prev_brk + mr_buf_size);
	ret = brk(cur_brk);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "brk failed", -errno);
		return NULL;
	}

	/* Determine the size of the newly allocated buffer. If this operation
	 * fails, memory is leaked.
	 */
	cur_brk = sbrk(0);
	if (cur_brk == (void *) -1) {
		FT_UNIT_STRERR(err_buf, "sbrk failed", -errno);
		return NULL;
	}

	FT_DEBUG("Moved program break from %p to %p", prev_brk, cur_brk);

	return prev_brk;
}

/* Mmap allocations are only intended to support a single outstanding
 * allocation at a time. Extra handling of the mmap reuse address needs to occur
 * to make mmap allocations more flexible including making allocations thread
 * safe.
 */
static void mmap_free(void *ptr)
{
	if (munmap(ptr, mr_buf_size))
		FT_UNIT_STRERR(err_buf, "munmap failed", -errno);
}

static void *mmap_alloc(void)
{
	void *ptr;
	int flags = MAP_ANONYMOUS | MAP_PRIVATE;

	/* If a reuse address is defined, request MAP_FIXED to require the mmap
	 * allocation to reuse this address.
	 */
	if (reuse_addr)
		flags |= MAP_FIXED;

	ptr = mmap(reuse_addr, mr_buf_size, PROT_READ | PROT_WRITE, flags, -1,
		   0);
	if (ptr == MAP_FAILED) {
		FT_UNIT_STRERR(err_buf, "mmap failed", -errno);
		return NULL;
	}

	/* Cache this virtual address to reuse for future allocations. */
	reuse_addr = ptr;

	return ptr;
}

static void rocr_free(void *ptr)
{
	ft_hmem_free(FI_HMEM_ROCR, ptr);
}

static void *rocr_malloc(void)
{
	int ret;
	void *ptr;

	ret = ft_hmem_alloc(FI_HMEM_ROCR, 0, &ptr, mr_buf_size);
	if (ret)
		return NULL;
	return ptr;
}


static void cuda_free(void *ptr)
{
	ft_hmem_free(FI_HMEM_CUDA, ptr);
}

static void *cuda_malloc(void)
{
	int ret;
	void *ptr;

	ret = ft_hmem_alloc(FI_HMEM_CUDA, 0, &ptr, mr_buf_size);
	if (ret)
		return NULL;
	return ptr;
}

/* Generic allocation/deallocation function. Only a single allocation of any
 * type should be outstanding.
 */
static void mem_free(void *ptr, enum alloc_type type)
{
	switch (type) {
	case SBRK:
		sbrk_free(ptr);
		break;
	case MMAP:
		mmap_free(ptr);
		break;
	case BRK:
		brk_free(ptr);
		break;
	case CUDA:
		cuda_free(ptr);
		break;
	case ROCR:
		rocr_free(ptr);
		break;
	default:
		return;
	}

	FT_DEBUG("Memory freed: va=%p", ptr);
}

static enum fi_hmem_iface alloc_type_to_iface(enum alloc_type type)
{
	switch (type) {
	case CUDA:
		return FI_HMEM_CUDA;
	case ROCR:
		return FI_HMEM_ROCR;
	default:
		return FI_HMEM_SYSTEM;
	}
}

/* User defined global mr_buf_size controls allocation size. */
static void *mem_alloc(enum alloc_type type)
{
	uint64_t phys_addr = 0;
	void *ptr;
	int ret;

	switch (type) {
	case SBRK:
		ptr = sbrk_alloc();
		break;
	case MMAP:
		ptr = mmap_alloc();
		break;
	case BRK:
		ptr = brk_alloc();
		break;
	case CUDA:
		ptr = cuda_malloc();
		break;
	case ROCR:
		ptr = rocr_malloc();
		break;
	default:
		return NULL;
	}

	if (ptr) {
		if (geteuid() == 0 &&
		    alloc_type_to_iface(type) == FI_HMEM_SYSTEM) {
			/* Perform a write to the buffer to ensure the kernel
			 * has faulted in a page for this allocation. This will
			 * help prevent virt_to_phys() from returning an error
			 * due to no PFN.
			 */
			*(uint8_t *) ptr = 0;
			ret = virt_to_phys(ptr, &phys_addr);
			if (ret)
				FT_DEBUG("virt_to_phys() failed: %s",
					 fi_strerror(-ret));
		}

		FT_DEBUG("Memory allocated: va=%p size=%lu phys_addr=0x%lx",
			 ptr, mr_buf_size, phys_addr);
	}

	return ptr;
}

/* MR registration function which returns the MR and the elapsed time, in
 * nanoseconds, to register the MR.
 */
static int mr_register(const void *buf, struct fid_mr **mr, int64_t *elapsed,
		       enum fi_hmem_iface iface)
{
	int ret;
	const struct iovec iov = {
		.iov_base = (void *) buf,
		.iov_len = mr_buf_size,
	};
	struct fi_mr_attr mr_attr = {
		.mr_iov = &iov,
		.iov_count = 1,
		.access = ft_info_to_mr_access(fi),
		.requested_key = FT_MR_KEY,
		.iface = iface,
	};

	ft_start();
	ret = fi_mr_regattr(domain, &mr_attr, 0, mr);
	ft_stop();

	if (ret != FI_SUCCESS) {
		FT_UNIT_STRERR(err_buf, "fi_mr_reg failed", -errno);
		return -errno;
	}

	*elapsed = get_elapsed(&start, &end, NANO);

	return 0;
}

/* Run a test verifing the eviction MR cache entries. The following is how the
 * test works:
 * 1. Prime CPU caches by registering a priming MR. This MR is not used for
 *    cache measurements.
 *
 * 2. Allocate a buffer using mem_alloc() with either MMAP, BRK, or SBRK. The
 *    mem_alloc() allocator is constructed to return the same virtual address
 *    during buffer reallocation.
 *
 * 3. Measure MR registration time of the mem_alloc() buffer. Since this buffer
 *    has not been previously registered, the elapsed time for this MR
 *    registration should be long. This is referred to as the initial MR
 *    registration time.
 *
 * 4. Measure MR registration time of the mem_alloc() buffer again. Since this
 *    buffer has been previously registered, the elapsed time for this MR
 *    registration should be significantly less than the initial MR registration
 *    time. If the registration time is not significantly less, it is assumed
 *    the provider does not support MR caching, and the test will exit. This
 *    elapsed time is referred to as the cached MR registration time.
 *
 * 5. If the provider supports caching, the mem_alloc() buffer is freed and
 *    reallocated. Measures are inplace to have the reallocated mem_alloc()
 *    buffer return the same virtual address. During this this time, the
 *    provider's MR cache should experience an eviction.
 *
 * 6. Measure MR registration time of the mem_alloc() buffer a third time. Since
 *    the provider should have experienced a MR cache eviction, the elapsed time
 *    for this MR registration should not be significantly less than the initial
 *    MR registration time. If this allocation is significantly less, it is
 *    assumed this MR registration incorrectly found a cached MR entry. This
 *    elapsed time is referred to as the reallocated MR registration time.
 */
static int mr_cache_test(enum alloc_type type)
{
	void *prime_buf = NULL;
	struct fid_mr *prime_mr = NULL;
	void *buf = NULL;
	struct fid_mr *mr = NULL;
	int64_t mr_reg_time;
	struct fid_mr *cached_mr = NULL;
	int64_t cached_mr_reg_time;
	struct fid_mr *realloc_mr = NULL;
	int64_t realloc_mr_reg_time;
	int ret;
	void *prev_buf;
	int testret = FAIL;
	enum fi_hmem_iface iface = alloc_type_to_iface(type);

	/* Reallocate the domain to reset the MR cache. */
	if (!domain) {
		ret = -EINVAL;
		FT_UNIT_STRERR(err_buf, "no domain allocated", ret);
		goto cleanup;
	}

	ret = fi_close(&domain->fid);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "Failed to close the domain", ret);
		domain = NULL;
		goto cleanup;
	}

	ret = fi_domain(fabric, fi, &domain, NULL);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "fi_domain failed", ret);
		domain = NULL;
		goto cleanup;
	}

	/* A priming MR registration is used to ensure the first timed MR
	 * registration does not take into account the setting up of CPU caches.
	 */
	switch (iface) {
	case FI_HMEM_CUDA:
		prime_buf = cuda_malloc();
		if (!prime_buf) {
			ret = -ENOMEM;
			FT_UNIT_STRERR(err_buf, "cuda_malloc failed", ret);
			goto cleanup;
		}
		break;

	case FI_HMEM_ROCR:
		prime_buf = rocr_malloc();
		if (!prime_buf) {
			ret = -ENOMEM;
			FT_UNIT_STRERR(err_buf, "rocr_malloc failed", ret);
			goto cleanup;
		}
		break;

	default:
		prime_buf = malloc(mr_buf_size);
		if (!prime_buf) {
			ret = -ENOMEM;
			FT_UNIT_STRERR(err_buf, "malloc failed", ret);
			goto cleanup;
		}
		break;
	}

	ret = mr_register(prime_buf, &prime_mr, &mr_reg_time, iface);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "mr_register failed", ret);
		goto cleanup;
	}

	/* Perform initial MR registration. MR registration elapsed time is
	 * recorded for future comparision.
	 */
	buf = mem_alloc(type);
	if (!buf) {
		ret = -ENOMEM;
		FT_UNIT_STRERR(err_buf, "mem_alloc failed", ret);
		goto cleanup;
	}

	ret = mr_register(buf, &mr, &mr_reg_time, iface);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "mr_register failed", ret);
		goto cleanup;
	}

	FT_DEBUG("Initial MR registration time: %ld nsecs", mr_reg_time);

	/* Perform another allocation using the same buffer. This should hit the
	 * MR cache.
	 */
	ret = mr_register(buf, &cached_mr, &cached_mr_reg_time, iface);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "mr_register failed", ret);
		goto cleanup;
	}

	FT_DEBUG("Cached MR registration time: %ld nsecs", cached_mr_reg_time);

	/* If cached allocation is not within the expected duration, assume the
	 * provider does not support MR caching.
	 */
	if (cached_mr_reg_time > CACHE_TIME_MAX_VALUE(mr_reg_time)) {
		ret = -FI_ENOSYS;
		sprintf(err_buf, "Assuming MR cache not enabled by provider");
		goto cleanup;
	}

	/* Free the buffer without freeing the MR. This should result in the MR
	 * cache evicting/invalidating the MR entry. The buffer will then be
	 * reallocated and re-registered. The newly registered MR should not
	 * have been cached.
	 */
	prev_buf = buf;
	mem_free(buf, type);

	buf = mem_alloc(type);
	if (!buf) {
		ret = -ENOMEM;
		FT_UNIT_STRERR(err_buf, "mem_alloc failed", ret);
		goto cleanup;
	}

	/* We NEED the same pointer to be returned for this test to be valid. */
	if (buf != prev_buf) {
		ret = -EFAULT;
		FT_UNIT_STRERR(err_buf,
			       "Failed to reallocate same virtual address",
			       ret);
		goto cleanup;
	}

	/* Verify reallocated MR registration time is close to the initial MR
	 * registration time and greater than the cached MR registration time.
	 */
	ret = mr_register(buf, &realloc_mr, &realloc_mr_reg_time, iface);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "mr_register failed", ret);
		goto cleanup;
	}

	FT_DEBUG("Reallocated MR registration time: %ld nsecs",
		 realloc_mr_reg_time);

	if (realloc_mr_reg_time <= CACHE_TIME_MAX_VALUE(mr_reg_time)) {
		ret = -EEXIST;
		FT_UNIT_STRERR(err_buf,
			       "Reallocated MR registration time too low. "
			       "Cached MR may have been incorrectly used.",
			       ret);
	} else {
		testret = PASS;
	}

cleanup:
	if (realloc_mr)
		fi_close(&realloc_mr->fid);

	if (cached_mr)
		fi_close(&cached_mr->fid);

	if (mr)
		fi_close(&mr->fid);

	if (buf)
		mem_free(buf, type);

	if (prime_mr)
		fi_close(&prime_mr->fid);

	if (prime_buf) {
		switch (iface) {
		case FI_HMEM_CUDA:
			cuda_free(prime_buf);
			break;

		case FI_HMEM_ROCR:
			rocr_free(prime_buf);
			break;

		default:
			free(prime_buf);
			break;
		}
	}

	return TEST_RET_VAL(ret, testret);
}

/* Run tests using MMAP, BRK, and SBRK. */
static int mr_cache_mmap_test(void)
{
	return mr_cache_test(MMAP);
}

static int mr_cache_brk_test(void)
{
	return mr_cache_test(BRK);
}

static int mr_cache_sbrk_test(void)
{
	return mr_cache_test(SBRK);
}

static int mr_cache_cuda_test(void)
{
	int ret;

	if (!(opts.options & FT_OPT_ENABLE_HMEM)) {
		sprintf(err_buf, "FI_HMEM support not requested");
		return SKIPPED;
	}

	ret = ft_hmem_init(FI_HMEM_CUDA);
	if (ret) {
		sprintf(err_buf, "ft_hmem_init(FI_HMEM_CUDA) failed");
		return TEST_RET_VAL(ret, FAIL);
	}

	ret = mr_cache_test(CUDA);

	ft_hmem_cleanup(FI_HMEM_CUDA);

	return ret;
}

static int mr_cache_rocr_test(void)
{
	int ret;

	if (!(opts.options & FT_OPT_ENABLE_HMEM)) {
		sprintf(err_buf, "FI_HMEM support not requested");
		return SKIPPED;
	}

	ret = ft_hmem_init(FI_HMEM_ROCR);
	if (ret) {
		sprintf(err_buf, "ft_hmem_init(FI_HMEM_ROCR) failed");
		return TEST_RET_VAL(ret, FAIL);
	}

	ret = mr_cache_test(ROCR);

	ft_hmem_cleanup(FI_HMEM_ROCR);

	return ret;
}

struct test_entry test_array[] = {
	TEST_ENTRY(mr_cache_mmap_test, "MR cache eviction test using MMAP"),
	TEST_ENTRY(mr_cache_brk_test, "MR cache eviction test using BRK"),
	TEST_ENTRY(mr_cache_sbrk_test, "MR cache eviction test using SBRK"),
	TEST_ENTRY(mr_cache_cuda_test, "MR cache eviction test using CUDA"),
	TEST_ENTRY(mr_cache_rocr_test, "MR cache eviction test using ROCR"),
	{ NULL, "" }
};

static void usage(void)
{
	ft_unit_usage("fi_mr_cache_evict",
		"Test a provider's ability to evict MR cache entries.\n"
		"Evictions are verified using MMAP, BRK, SBRK, CUDA and ROCR\n"
		"allocations. FI_HMEM support must be enabled to run CUDA and\n"
		"ROCR tests.\n\n"
		"With debug enabled, when running as root, the physical \n"
		"address of the first page of the MMAP, BRK, and SBRK \n"
		"allocation is returned. This can be used to verify the \n"
		"underlying physical memory changes between MMAP, BRK, and \n"
		"SBRK allocations. When running as non-root, the reported \n"
		"physical address is always zero.");
	FT_PRINT_OPTS_USAGE("-s <bytes>", "Memory region size to be tested.");
	FT_PRINT_OPTS_USAGE("-H", "Enable provider FI_HMEM support");
}

int main(int argc, char **argv)
{
	int ret;
	int op;
	int failed = 0;

	/* Force malloc to use mmap by setting M_MMAP_THRESHOLD to 1. This
	 * allows for this application to control the program break. Note that
	 * not all operating systems may support this call. Thus, failure of
	 * mallopt() is not treated as an error. But, this could impact the
	 * results of the test.
	 */
	ret = mallopt(M_MMAP_THRESHOLD, 1);
	if (ret != 1)
		FT_PRINTERR("Failed to set M_MMAP_THRESHOLD to 1. "
			    "System may not support M_MMAP_THRESHOLD. "
			    "Proceeding with test.", -EINVAL);

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, FAB_OPTS "h" "s:")) != -1) {
		switch (op) {
		default:
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 's':
			errno = 0;
			mr_buf_size = strtoul(optarg, NULL, 10);
			if (mr_buf_size == 0)
				ret = -EINVAL;
			else if (mr_buf_size == ULONG_MAX && errno)
				ret = -errno;
			else
				ret = 0;

			if (ret) {
				FT_PRINTERR("Invalid memory region size", ret);
				goto out;
			}
			break;
		case '?':
		case 'h':
			usage();
			return EXIT_FAILURE;
		}
	}

	hints->mode = ~0;
	hints->domain_attr->mode = ~0;
	hints->domain_attr->mr_mode = ~(FI_MR_BASIC | FI_MR_SCALABLE);
	hints->caps |= FI_MSG | FI_RMA;

	if (opts.options & FT_OPT_ENABLE_HMEM)
		hints->caps |= FI_HMEM;

	ret = fi_getinfo(FT_FIVERSION, NULL, 0, 0, hints, &fi);
	if (ret) {
		hints->caps &= ~FI_RMA;
		ret = fi_getinfo(FT_FIVERSION, NULL, 0, 0, hints, &fi);
		if (ret) {
			FT_PRINTERR("fi_getinfo", ret);
			goto out;
		}
	}

	if (!ft_info_to_mr_access(fi))
		goto out;

	if (fi->domain_attr->mr_iov_limit == 0) {
		ret = -EINVAL;
		FT_PRINTERR("mr_iov_limit not set", ret);
		goto out;
	}

	ret = ft_open_fabric_res();
	if (ret)
		goto out;

	printf("Testing MR cache on fabric %s domain %s\n",
	       fi->fabric_attr->name, fi->domain_attr->name);

	failed = run_tests(test_array, err_buf);
	if (failed > 0)
		printf("Summary: %d tests failed\n", failed);
	else
		printf("Summary: all tests passed\n");

out:
	ft_free_res();
	return ret ? ft_exit_code(ret) : (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
