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

#ifndef _PSMI_IN_USER_H
#error psm_utils.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_UTILS_H
#define _PSMI_UTILS_H

#include <arpa/inet.h>		/* ipv4addr */
#include <stdlib.h>		/* malloc/free */
#include <psm_netutils.h>

/*
 * Endpoint 'id' hash table, with iterator interface
 */
struct psmi_epid_table {
	struct psmi_epid_tabentry *table;
	int tabsize;
	int tabsize_used;
	pthread_mutex_t tablock;
};
/*
 * Endpoint address hash table
 */
struct psmi_epid_tabentry {
	void *entry;
	uint64_t key;
	psm2_ep_t ep;
	psm2_epid_t epid;
};

extern struct psmi_epid_table psmi_epid_table;
#define EPADDR_DELETED	((void *)-1)	/* tag used to mark deleted entries */

psm2_error_t psmi_epid_init();
psm2_error_t psmi_epid_fini();
void *psmi_epid_lookup(psm2_ep_t ep, psm2_epid_t epid);
void *psmi_epid_remove(psm2_ep_t ep, psm2_epid_t epid);
void psmi_epid_remove_all(psm2_ep_t ep);
psm2_error_t psmi_epid_add(psm2_ep_t ep, psm2_epid_t epid, void *entry);
#define PSMI_EP_HOSTNAME    ((psm2_ep_t) -1)	/* Special endpoint handle we use
						 * to register hostnames */
#define PSMI_EP_CROSSTALK   ((psm2_ep_t) -2)	/* Second special endpoint handle
						 * to log which nodes we've seen
						 * crosstalk from */
struct psmi_eptab_iterator {
	int i;			/* last index looked up */
	psm2_ep_t ep;
};
void psmi_epid_itor_init(struct psmi_eptab_iterator *itor, psm2_ep_t ep);
void *psmi_epid_itor_next(struct psmi_eptab_iterator *itor);
void psmi_epid_itor_fini(struct psmi_eptab_iterator *itor);

uint64_t psmi_epid_version(psm2_epid_t epid);

/*
 * Hostname manipulation
 */
char *psmi_gethostname(void);
const char *psmi_epaddr_fmt_addr(psm2_epid_t epid);
const char *psmi_epaddr_get_hostname(psm2_epid_t epid);
const char *psmi_epaddr_get_name(psm2_epid_t epid);
psm2_error_t psmi_epid_set_hostname(uint64_t nid, const char *hostname,
				   int overwrite);

/*
 * Memory allocation, use macros only.
 *
 * In all calls, ep can be a specific endpoint (valid psm2_ep_t) or PSMI_EP_NONE
 * if no endpoint is available.
 *
 *   psmi_malloc_usable_size(void *ptr)
 *   psmi_malloc(ep, memtype, size)
 *   psmi_realloc(ep, memtype, ptr, newsize)
 *   psmi_memalign(ep, memtype, alignment, size)
 *   psmi_calloc(ep, memtype, elemsz, numelems)
 *   psmi_strdup(ep, memtype, ptr)
 *   psmi_free(ptr)
 *
 */
typedef enum psmi_memtype {
	TOTAL = 0,		/* Logged automatically by malloc/calloc */
	UNDEFINED,		/* For tracking "other types" of allocations */
	PER_PEER_ENDPOINT,	/* For tracking "per peer" allocations */
	NETWORK_BUFFERS,	/* For tracking network buffers */
	DESCRIPTORS,		/* For tracking send/recv descriptors */
	UNEXPECTED_BUFFERS,	/* For tracking unexpected recv buffers */
	STATS,			/* For tracking stats-related allocs */
#ifdef RNDV_MOD
	// TBD, should we just tabulate this into PER_PEER_ENDPOINT
	// maybe once debugged we should consolidate?
	PEER_RNDV,			/* for tracking Rendezvous per RC QP resources */
#endif
} psmi_memtype_t;

/*
 * We track allocation stats.
 */
struct psmi_stats_malloc {
	int64_t m_all_total;
	int64_t m_all_max;
	int64_t m_perpeer_total;
	int64_t m_perpeer_max;
	int64_t m_netbufs_total;
	int64_t m_netbufs_max;
	int64_t m_descriptors_total;
	int64_t m_descriptors_max;
	int64_t m_unexpbufs_total;
	int64_t m_unexpbufs_max;
	int64_t m_undefined_total;
	int64_t m_undefined_max;
	int64_t m_stats_total;
	int64_t m_stats_max;
#ifdef RNDV_MOD
	int64_t m_peerrndv_total;
	int64_t m_peerrndv_max;
#endif
};

extern struct psmi_stats_malloc psmi_stats_memory;

void psmi_mem_stats_register(void);

void *psmi_malloc_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t sz,
			   const char *curloc);
void *psmi_realloc_internal(psm2_ep_t ep, psmi_memtype_t mt, void *ptr,
			    size_t newSz, const char *curloc);
void *psmi_memalign_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t alignment,
			     size_t sz, const char *curloc);
void *psmi_calloc_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t num,
			   size_t sz, const char *curloc);
void *psmi_strdup_internal(psm2_ep_t ep, const char *string, const char *curloc);

void MOCKABLE(psmi_free_internal)(void *ptr, const char *curLoc);
MOCK_DCL_EPILOGUE(psmi_free_internal);

size_t psmi_malloc_usable_size_internal(void *ptr, const char *curLoc);

#ifdef PSM_HEAP_DEBUG
/* During heap debug code, we can sprinkle function calls:
   psmi_heapdebug_val_heapallocs(), that will examine all of the heap allocations
   to ensure integrity. */
void _psmi_heapdebug_val_heapallocs(const char *curloc);

#define psmi_heapdebug_val_heapallocs() _psmi_heapdebug_val_heapallocs(PSMI_CURLOC)

/* Finialize the heapdebug functionality after tear down of the psm
   session when you are certain that all heap allocations have been
   freed. psmi_heapdebug_finalize() will emit all of the extant
   heap allocations and abort if there are any.  This is to aid
   in debug of heap leaks. */
void psmi_heapdebug_finalize(void);

#else

#define psmi_heapdebug_val_heapallocs() /* nothing */
#define psmi_heapdebug_finalize() /* nothing */

#endif

#define psmi_strdup(ep, string) psmi_strdup_internal(ep, string, PSMI_CURLOC)
#define psmi_calloc(ep, mt, nelem, elemsz) \
	psmi_calloc_internal(ep, mt, nelem, elemsz, PSMI_CURLOC)
#define psmi_malloc(ep, mt, sz) psmi_malloc_internal(ep, mt, sz, PSMI_CURLOC)
#define psmi_realloc(ep, mt, ptr, nsz) psmi_realloc_internal(ep, mt, ptr, nsz, PSMI_CURLOC)
#define psmi_memalign(ep, mt, al, sz) \
	psmi_memalign_internal(ep, mt, al, sz, PSMI_CURLOC)
#define psmi_free(ptr)	psmi_free_internal(ptr, PSMI_CURLOC)
#define psmi_malloc_usable_size(ptr) psmi_malloc_usable_size_internal(ptr, PSMI_CURLOC)
#ifndef PSM_IS_TEST
#define malloc(sz)        _use_psmi_malloc_instead_of_plain_malloc
#define realloc(ptr,nsz)  _use_psmi_realloc_instead_of_plain_realloc
#define memalign(algn,sz) _use_psmi_memalign_instead_of_plain_memalign
#define calloc(sz, nelm)  _use_psmi_calloc_instead_of_plain_calloc
#ifdef strdup
#undef strdup
#endif
#define strdup(ptr)             _use_psmi_strdup_instead_of_plain_strdup
#define free(ptr)               _use_psmi_free_instead_of_plain_free
#define malloc_usable_size(ptr) _use_psmi_malloc_usable_size_instead_of_plain_malloc_usable_size
#endif /* PSM_IS_TEST */

void psmi_log_memstats(psmi_memtype_t type, int64_t nbytes);

/*
 * Parse int parameters
 * -1 -> parse error
 */
long psmi_parse_str_long(const char *str);

/*
 * Parsing int parameters set in string tuples.
 */
int psmi_parse_str_tuples(const char *str, int ntup, int *vals);

/*
 * Resource Limiting based on PSM memory mode.
 */
#define PSMI_MEMMODE_NORMAL  0
#define PSMI_MEMMODE_MINIMAL 1
#define PSMI_MEMMODE_LARGE   2
#define PSMI_MEMMODE_NUM     3

struct psmi_rlimit_mpool {
	const char *env;
	const char *descr;
	int env_level;
	uint32_t minval;
	uint32_t maxval;
	struct {
		uint32_t obj_chunk;
		uint32_t obj_max;
	} mode[PSMI_MEMMODE_NUM];
};
psm2_error_t psmi_parse_mpool_env(const psm2_mq_t mq, int level,
				 const struct psmi_rlimit_mpool *rlim,
				 uint32_t *valo, uint32_t *chunkszo);
int psmi_parse_memmode(void);
int psmi_parse_identify(void);
unsigned psmi_parse_senddma(void);
unsigned psmi_parse_rdmamode(void);
#ifdef PSM_CUDA
unsigned psmi_parse_gpudirect(void);
unsigned psmi_parse_gpudirect_send_limit(void);
unsigned psmi_parse_gpudirect_recv_limit(void);
#endif

/*
 * Parsing environment variables
 */

union psmi_envvar_val {
	void *e_void;
	char *e_str;
	int e_int;
	unsigned int e_uint;
	long e_long;
	unsigned long e_ulong;
	unsigned long long e_ulonglong;
};

#define PSMI_ENVVAR_LEVEL_USER	         1
#define PSMI_ENVVAR_LEVEL_HIDDEN         2
#define PSMI_ENVVAR_LEVEL_NEVER_PRINT    4

#define PSMI_ENVVAR_TYPE_YESNO		0
#define PSMI_ENVVAR_TYPE_STR		1
#define PSMI_ENVVAR_TYPE_INT		2
#define PSMI_ENVVAR_TYPE_UINT		3
#define PSMI_ENVVAR_TYPE_UINT_FLAGS	4
#define PSMI_ENVVAR_TYPE_LONG		5
#define PSMI_ENVVAR_TYPE_ULONG		6
#define PSMI_ENVVAR_TYPE_ULONG_FLAGS	7
#define PSMI_ENVVAR_TYPE_ULONG_ULONG    8

#define PSMI_ENVVAR_VAL_YES ((union psmi_envvar_val) 1)
#define PSMI_ENVVAR_VAL_NO  ((union psmi_envvar_val) 0)

int
MOCKABLE(psmi_getenv)(const char *name, const char *descr, int level,
		int type, union psmi_envvar_val defval,
		union psmi_envvar_val *newval);
MOCK_DCL_EPILOGUE(psmi_getenv);
int psmi_parse_val_pattern(const char *env, int def, int def_syntax);
/*
 * Misc functionality
 */
uintptr_t psmi_getpagesize(void);
uint64_t psmi_cycles_left(uint64_t start_cycles, int64_t timeout_ns);
uint32_t psmi_get_ipv4addr();
void psmi_syslog(psm2_ep_t ep, int to_console, int level,
		 const char *format, ...);
void *psmi_memcpyo(void *dst, const void *src, size_t n);
uint32_t psmi_crc(unsigned char *buf, int len);

/*
 * Internal CPUID detection
 */
#define CPUID_FAMILY_MASK       0x00000f00
#define CPUID_MODEL_MASK        0x000000f0
#define CPUID_EXMODEL_MASK      0x000f0000

/*
 * CPUID return values
 */
#define CPUID_FAMILY_XEON       0x00000600
#define CPUID_MODEL_PHI_GEN2    87
#define CPUID_MODEL_PHI_GEN2M   133
/*
 * cpuid function 0, returns "GeniuneIntel" in EBX,ECX,EDX
 * due to Little Endian and Hex it is not so obvious
 */
#define CPUID_GENUINE_INTEL_EBX 0x756e6547 /* "uneG" - Little Endian "Genu" */
#define CPUID_GENUINE_INTEL_ECX 0x6c65746e /* "Ieni" - Little Endian "ineI" */
#define CPUID_GENUINE_INTEL_EDX 0x49656e69 /* "letn" - Little Endian "ntel" */

/*
 * These values are internal only, not real register values
 */
#define CPUID_GENUINE_INTEL     0xf0000000
#define CPUID_MODEL_UNDEFINED   -1

/*
 * Global model so we can tune defaults better for specific cpu's
 */
extern uint32_t psmi_cpu_model;

/*
 * Diagnostics, all in psm_diags.c
 */
int psmi_diags(void);

/*
 * Multiple Endpoints
 */
extern int psmi_multi_ep_enabled;
void psmi_multi_ep_init();

#ifdef PSM_FI
/*
 * Fault injection
 * Controlled by:
 *	PSM3_FI=0/1 - enable
 *	PSM3_FI_TRACEFILE - where to put summary stats at end of run
 *		"stdout", "stderr", of prefix for per process filename
 *	PSM3_FI_VERBOSE - output to std when generate fault
 *	PSM3_FI_RAIL - only generate for secondary EPs/Rails/QPs
 *	PSM3_FI_X - for each fault type: num:denom:seed
 *			fault num/denom of events, seed random for reproducing
 *		recvlost - discard packet on receive before processing
 *		rq_lkey - RQ WQE with bad lkey
 *		rc_rdma_lkey - User RC SQ WQE with bad lkey
 *		rc_rdma_rkey - User RC SQ WQE with bad rkey
 *		rv_rdma_len - RV SQ WQE with bad len
 *		rv_rdma_rkey - RV SQ WQE with bad rkey
 *		sq_lkey - SQ WQE with bad lkey
 *		sendlost - discard packet on send before sending
 *		reg_mr - register MR failure (ENOMEM)
 *		nonpri_reg_mr - non-priority register MR failure (ENOMEM)
 *		pri_reg_mr - priority register MR failure (ENOMEM)
 *		gdrmmap - GPU gdrcopy pin and mmap failure
 */
int psmi_faultinj_enabled;	/* use macro to test */
int psmi_faultinj_verbose;	/* use IS_FAULT macro to test */
int psmi_faultinj_sec_rail;	/* faults only on secondary rails or EPs */

struct psmi_faultinj_spec {
	STAILQ_ENTRY(psmi_faultinj_spec) next;
	char spec_name[PSMI_FAULTINJ_SPEC_NAMELEN];

	uint64_t num_faults;
	uint64_t num_calls;

	struct drand48_data drand48_data;
	int num;
	int denom;
	long int initial_seed;
};

#define PSMI_FAULTINJ_ENABLED()	(!!psmi_faultinj_enabled)
#define PSMI_FAULTINJ_ENABLED_EP(ep)	(PSMI_FAULTINJ_ENABLED() \
		&& (!psmi_faultinj_sec_rail || ((ep)->mctxt_master != (ep))))

int psmi_faultinj_is_fault(struct psmi_faultinj_spec *fi); // use macro instead
#define PSMI_FAULTINJ_IS_FAULT(fi, fmt, ...) \
	(psmi_faultinj_is_fault(fi)? \
			psmi_faultinj_verbose? \
				(printf("%s: injecting fault: %s" fmt "\n", hfi_get_mylabel(), fi->spec_name, ##__VA_ARGS__), fflush(stdout), 1) \
				: 1 \
			: 0)

void psmi_faultinj_init();
void psmi_faultinj_fini();
struct psmi_faultinj_spec *psmi_faultinj_getspec(const char *spec_name,
						 const char *help,
						 int num, int denom);
#define PSMI_FAULTINJ_STATIC_DECL(var, spec_name, help, num, denom)	\
	static struct psmi_faultinj_spec *var;			\
	if_pf(PSMI_FAULTINJ_ENABLED() && (var) == NULL)			\
	    (var) = psmi_faultinj_getspec((spec_name), (help), (num), (denom));

#else
#define PSMI_FAULTINJ_ENABLED()	0
#define PSMI_FAULTINJ_ENABLED_EP(ep)	0
#define PSMI_FAULTINJ_IS_FAULT(fi, fmt, ...) 0
#define PSMI_FAULTINJ_STATIC_DECL(var, spec_name, help, num, denom)
#endif /* #ifdef PSM_FI */
/*
 * PSM core component set/get options
 */
psm2_error_t psmi_core_setopt(const void *core_obj, int optname,
			     const void *optval, uint64_t optlen);

psm2_error_t psmi_core_getopt(const void *core_obj, int optname,
			     void *optval, uint64_t *optlen);

/*
 * PSM AM component set/get options
 */
psm2_error_t psmi_am_setopt(const void *am_obj, int optname,
			   const void *optval, uint64_t optlen);

psm2_error_t psmi_am_getopt(const void *am_obj, int optname,
			   void *optval, uint64_t *optlen);

#endif /* _PSMI_UTILS_H */
