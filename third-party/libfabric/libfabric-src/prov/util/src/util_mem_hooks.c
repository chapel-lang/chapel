/*
 * Copyright (c) 2016 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2019 Intel Corporation, Inc.  All rights reserved.
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

struct ofi_memhooks memhooks;
struct ofi_mem_monitor *memhooks_monitor = &memhooks.monitor;


#if defined(__linux__) && defined(HAVE_ELF_H) && defined(HAVE_SYS_AUXV_H)

#include <elf.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <link.h>


struct ofi_intercept {
	struct dlist_entry 		entry;
	const char			*symbol;
	void				*our_func;
	struct dlist_entry		dl_intercept_list;
};

struct ofi_dl_intercept {
	struct dlist_entry 		entry;
	void 				**dl_func_addr;
	void				*dl_func;
};

enum {
	OFI_INTERCEPT_DLOPEN,
	OFI_INTERCEPT_MMAP,
	OFI_INTERCEPT_MUNMAP,
	OFI_INTERCEPT_MREMAP,
	OFI_INTERCEPT_MADVISE,
	OFI_INTERCEPT_SHMAT,
	OFI_INTERCEPT_SHMDT,
	OFI_INTERCEPT_BRK,
	OFI_INTERCEPT_MAX
};

static void *ofi_intercept_dlopen(const char *filename, int flag);
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
	[OFI_INTERCEPT_DLOPEN] = { .symbol = "dlopen",
				.our_func = ofi_intercept_dlopen},
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

struct ofi_mem_calls {
	void *(*dlopen) (const char *, int);
	void *(*mmap)(void *, size_t, int, int, int, off_t);
	int (*munmap)(void *, size_t);
	void *(*mremap)(void *old_address, size_t old_size,
			size_t new_size, int flags, ... /* void *new_address */ );
	int (*madvise)(void *addr, size_t length, int advice);
	void *(*shmat)(int shmid, const void *shmaddr, int shmflg);
	int (*shmdt)(const void *shmaddr);
	int (*brk)(const void *brkaddr);
};

static struct ofi_mem_calls real_calls;


static const ElfW(Phdr) *
ofi_get_phdr_dynamic(const ElfW(Phdr) *phdr, uint16_t phnum, int phent)
{
	uint16_t i;

	for (i = 0 ; i < phnum; i++) {
		if (phdr->p_type == PT_DYNAMIC)
			return phdr;
		phdr = (ElfW(Phdr)*) ((intptr_t) phdr + phent);
	}

	return NULL;
}

static void *ofi_get_dynentry(ElfW(Addr) base, const ElfW(Phdr) *pdyn,
			      ElfW(Sxword) type)
{
	ElfW(Dyn) *dyn;

	for (dyn = (ElfW(Dyn)*) (base + pdyn->p_vaddr); dyn->d_tag; ++dyn) {
		if (dyn->d_tag == type)
			return (void *) (uintptr_t) dyn->d_un.d_val;
	}

	return NULL;
}

#if SIZE_MAX > UINT_MAX
#define OFI_ELF_R_SYM ELF64_R_SYM
#else
#define OFI_ELF_R_SYM ELF32_R_SYM
#endif

static void *ofi_dl_func_addr(ElfW(Addr) base, const ElfW(Phdr) *phdr,
			      int16_t phnum, int phent, const char *symbol)
{
	const ElfW(Phdr) *dphdr;
	ElfW(Rela) *reloc;
	void *jmprel, *strtab;
	char *elf_sym;
	uint32_t relsymidx;
	ElfW(Sym) *symtab;
	size_t pltrelsz;

	dphdr = ofi_get_phdr_dynamic(phdr, phnum, phent);
	jmprel = ofi_get_dynentry(base, dphdr, DT_JMPREL);
	symtab = (ElfW(Sym) *) ofi_get_dynentry(base, dphdr, DT_SYMTAB);
	strtab = ofi_get_dynentry (base, dphdr, DT_STRTAB);
	pltrelsz = (uintptr_t) ofi_get_dynentry(base, dphdr, DT_PLTRELSZ);

	for (reloc = jmprel; (intptr_t) reloc < (intptr_t) jmprel + pltrelsz;
	     reloc++) {
		relsymidx = OFI_ELF_R_SYM(reloc->r_info);
		elf_sym = (char *) strtab + symtab[relsymidx].st_name;
		if (!strcmp(symbol, elf_sym))
			return (void *) (base + reloc->r_offset);
        }

        return NULL;
}

static int ofi_intercept_dl_calls(ElfW(Addr) base, const ElfW(Phdr) *phdr,
				  const char *phname, int16_t phnum, int phent,
				  struct ofi_intercept *intercept)
{
	struct ofi_dl_intercept *dl_entry;
	long page_size = ofi_get_page_size();
	void **func_addr, *page;
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepting symbol %s from dl\n", intercept->symbol);
	func_addr = ofi_dl_func_addr(base, phdr, phnum, phent, intercept->symbol);
	if (!func_addr)
		return FI_SUCCESS;

	page = (void *) ((intptr_t) func_addr & ~(page_size - 1));
	ret = mprotect(page, page_size, PROT_READ | PROT_WRITE);
	if (ret < 0)
		return -FI_ENOSYS;

	if (*func_addr != intercept->our_func) {
		dl_entry = malloc(sizeof(*dl_entry));
		if (!dl_entry)
			return -FI_ENOMEM;

		dl_entry->dl_func_addr = func_addr;
		dl_entry->dl_func = *func_addr;
		*func_addr = intercept->our_func;
		dlist_insert_tail(&dl_entry->entry, &intercept->dl_intercept_list);
	}

	return FI_SUCCESS;
}

static int ofi_intercept_phdr_handler(struct dl_phdr_info *info,
                                    size_t size, void *data)
{
	struct ofi_intercept *intercept = data;
	int phent, ret;

	phent = getauxval(AT_PHENT);
	if (phent <= 0) {
		FI_DBG(&core_prov, FI_LOG_MR, "failed to read phent size");
		return -FI_EINVAL;
	}

	ret = ofi_intercept_dl_calls(info->dlpi_addr, info->dlpi_phdr,
				     info->dlpi_name, info->dlpi_phnum,
				     phent, intercept);
	return ret;
}

static void *ofi_intercept_dlopen(const char *filename, int flag)
{
	struct ofi_intercept  *intercept;
	void *handle;

	handle = real_calls.dlopen(filename, flag);
	if (!handle)
		return NULL;

	pthread_mutex_lock(&memhooks_monitor->lock);
	dlist_foreach_container(&memhooks.intercept_list, struct ofi_intercept,
		intercept, entry) {
		dl_iterate_phdr(ofi_intercept_phdr_handler, intercept);
	}
	pthread_mutex_unlock(&memhooks_monitor->lock);
	return handle;
}

static int ofi_restore_dl_calls(ElfW(Addr) base, const ElfW(Phdr) *phdr,
				const char *phname, int16_t phnum, int phent,
				struct ofi_intercept *intercept)
{
	struct ofi_dl_intercept *dl_entry;
	long page_size = ofi_get_page_size();
	void **func_addr, *page;
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "releasing symbol %s from dl\n", intercept->symbol);
	func_addr = ofi_dl_func_addr(base, phdr, phnum, phent, intercept->symbol);
	if (!func_addr)
		return FI_SUCCESS;

	page = (void *) ((intptr_t) func_addr & ~(page_size - 1));
	ret = mprotect(page, page_size, PROT_READ | PROT_WRITE);
	if (ret < 0)
		return -FI_ENOSYS;

	dlist_foreach_container_reverse(&intercept->dl_intercept_list,
		struct ofi_dl_intercept, dl_entry, entry) {

		if (dl_entry->dl_func_addr != func_addr)
			continue;

		assert(*func_addr == intercept->our_func);
		*func_addr = dl_entry->dl_func;
		dlist_remove(&dl_entry->entry);
		free(dl_entry);
		FI_DBG(&core_prov, FI_LOG_MR,
		       "dl symbol %s restored\n", intercept->symbol);
		break;
	}

	return FI_SUCCESS;
}

static int ofi_restore_phdr_handler(struct dl_phdr_info *info,
                                    size_t size, void *data)
{
	struct ofi_intercept *intercept = data;
	int phent, ret;

	phent = getauxval(AT_PHENT);
	if (phent <= 0) {
		FI_DBG(&core_prov, FI_LOG_MR, "failed to read phent size");
		return -FI_EINVAL;
	}

	ret = ofi_restore_dl_calls(info->dlpi_addr, info->dlpi_phdr,
				   info->dlpi_name, info->dlpi_phnum,
				   phent, intercept);
	return ret;
}

static void ofi_restore_intercepts(void)
{
	struct ofi_intercept *intercept;

	dlist_foreach_container(&memhooks.intercept_list, struct ofi_intercept,
		intercept, entry) {
		dl_iterate_phdr(ofi_restore_phdr_handler, intercept);
	}
}

static int ofi_intercept_symbol(struct ofi_intercept *intercept, void **real_func)
{
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepting symbol %s\n", intercept->symbol);
	ret = dl_iterate_phdr(ofi_intercept_phdr_handler, intercept);
	if (ret)
		return ret;

	*real_func = dlsym(RTLD_DEFAULT, intercept->symbol);
	if (*real_func == intercept->our_func) {
		(void) dlerror();
		*real_func = dlsym(RTLD_NEXT, intercept->symbol);
	}

	if (!*real_func) {
		FI_DBG(&core_prov, FI_LOG_MR,
		       "could not find symbol %s\n", intercept->symbol);
		ret = -FI_ENOMEM;
		return ret;
	}
	dlist_insert_tail(&intercept->entry, &memhooks.intercept_list);

	return ret;
}

void ofi_intercept_handler(const void *addr, size_t len)
{
	pthread_mutex_lock(&memhooks_monitor->lock);
	ofi_monitor_notify(memhooks_monitor, addr, len);
	pthread_mutex_unlock(&memhooks_monitor->lock);
}

static void *ofi_intercept_mmap(void *start, size_t length,
                            int prot, int flags, int fd, off_t offset)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted mmap start %p len %zu\n", start, length);
	ofi_intercept_handler(start, length);

	return real_calls.mmap(start, length, prot, flags, fd, offset);
}

static int ofi_intercept_munmap(void *start, size_t length)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted munmap start %p len %zu\n", start, length);
	ofi_intercept_handler(start, length);

	return real_calls.munmap(start, length);
}

static void *ofi_intercept_mremap(void *old_address, size_t old_size,
		size_t new_size, int flags, void *new_address)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted mremap old_addr %p old_size %zu\n",
	       old_address, old_size);
	ofi_intercept_handler(old_address, old_size);

	return real_calls.mremap(old_address, old_size, new_size, flags,
				 new_address);
}

static int ofi_intercept_madvise(void *addr, size_t length, int advice)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted madvise addr %p len %zu\n", addr, length);
	ofi_intercept_handler(addr, length);

	return real_calls.madvise(addr, length, advice);
}

static void *ofi_intercept_shmat(int shmid, const void *shmaddr, int shmflg)
{
	struct shmid_ds ds;
	const void *start;
	size_t len;
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted shmat addr %p\n", shmaddr);

	if (shmflg & SHM_REMAP) {
		ret = shmctl(shmid, IPC_STAT, &ds);
		len = (ret < 0) ? 0 : ds.shm_segsz;

		if (shmflg & SHM_RND) {
			start = (char *) shmaddr + ((uintptr_t) shmaddr) % SHMLBA;
			len += ((uintptr_t) shmaddr) % SHMLBA;
		} else {
			start = shmaddr;
		}

		ofi_intercept_handler(start, len);
	}

	return real_calls.shmat(shmid, shmaddr, shmflg);
}

static int ofi_intercept_shmdt(const void *shmaddr)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "intercepted shmdt addr %p\n", shmaddr);
	/* Overly aggressive, but simple.  Invalidate everything after shmaddr */
	ofi_intercept_handler(shmaddr, SIZE_MAX - (uintptr_t) shmaddr);

	return real_calls.shmdt(shmaddr);
}

static int ofi_intercept_brk(const void *brkaddr)
{
	void *old_addr;

	FI_DBG(&core_prov, FI_LOG_MR,
	      "intercepted brk addr %p\n", brkaddr);

	old_addr = sbrk (0);

	if(brkaddr > old_addr) {
		ofi_intercept_handler(brkaddr, (intptr_t) brkaddr -
							  (intptr_t) old_addr);
	}

	return real_calls.brk(brkaddr);
}

static int ofi_memhooks_subscribe(struct ofi_mem_monitor *monitor,
				 const void *addr, size_t len)
{
	/* no-op */
	return FI_SUCCESS;
}

static void ofi_memhooks_unsubscribe(struct ofi_mem_monitor *monitor,
				    const void *addr, size_t len)
{
	/* no-op */
}

int ofi_memhooks_init(void)
{
	int i, ret;

	if (memhooks_monitor->subscribe == ofi_memhooks_subscribe)
		return 0;

	memhooks_monitor->subscribe = ofi_memhooks_subscribe;
	memhooks_monitor->unsubscribe = ofi_memhooks_unsubscribe;
	dlist_init(&memhooks.intercept_list);

	for (i = 0; i < OFI_INTERCEPT_MAX; ++i)
		dlist_init(&intercepts[i].dl_intercept_list);

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_DLOPEN],
				   (void **) &real_calls.dlopen);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept dlopen failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_MMAP],
				   (void **) &real_calls.mmap);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept mmap failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_MUNMAP],
				   (void **) &real_calls.munmap);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept munmap failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_MREMAP],
				   (void **) &real_calls.mremap);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept mremap failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_MADVISE],
				   (void **) &real_calls.madvise);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept madvise failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_SHMAT],
				   (void **) &real_calls.shmat);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept shmat failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_SHMDT],
				   (void **) &real_calls.shmdt);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept shmdt failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	ret = ofi_intercept_symbol(&intercepts[OFI_INTERCEPT_BRK],
				   (void **) &real_calls.brk);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
		       "intercept brk failed %d %s\n", ret, fi_strerror(ret));
		return ret;
	}

	return 0;
}

void ofi_memhooks_cleanup(void)
{
	ofi_restore_intercepts();
	memhooks_monitor->subscribe = NULL;
	memhooks_monitor->unsubscribe = NULL;
}

#else

int ofi_memhooks_init(void)
{
	return -FI_ENOSYS;
}

void ofi_memhooks_cleanup(void)
{
}

#endif
