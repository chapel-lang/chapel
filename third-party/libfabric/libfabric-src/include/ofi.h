/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
 * Copyright (c) 2016-2018 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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

#ifndef _OFI_H_
#define _OFI_H_

#include <config.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <ofi_abi.h>
#include <ofi_file.h>
#include <ofi_lock.h>
#include <ofi_atom.h>
#include <ofi_net.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/providers/fi_log.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>

#include <ofi_osd.h>


#ifdef __cplusplus
extern "C" {
#endif

/* For in-tree providers */
#define OFI_VERSION_LATEST	FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION)
/* The lower minor digit is reserved for custom libfabric builds */
#define OFI_VERSION_DEF_PROV \
	FI_VERSION(FI_MAJOR_VERSION * 100 + FI_MINOR_VERSION, \
		   FI_REVISION_VERSION * 10)

#define OFI_GETINFO_INTERNAL	(1ULL << 58)
#define OFI_CORE_PROV_ONLY	(1ULL << 59)
#define OFI_GETINFO_HIDDEN	(1ULL << 60)
#define OFI_OFFLOAD_PROV_ONLY	(1ULL << 61)

#define OFI_ORDER_RAR_SET	(FI_ORDER_RAR | FI_ORDER_RMA_RAR | \
				 FI_ORDER_ATOMIC_RAR)
#define OFI_ORDER_RAW_SET	(FI_ORDER_RAW | FI_ORDER_RMA_RAW | \
				 FI_ORDER_ATOMIC_RAW)
#define OFI_ORDER_WAR_SET	(FI_ORDER_WAR | FI_ORDER_RMA_WAR | \
				 FI_ORDER_ATOMIC_WAR)
#define OFI_ORDER_WAW_SET	(FI_ORDER_WAW | FI_ORDER_RMA_WAW | \
				 FI_ORDER_ATOMIC_WAW)

#define OFI_PRIMARY_TX_CAPS \
	(FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | FI_MULTICAST | \
	 FI_READ | FI_WRITE | FI_SEND | \
	 FI_COLLECTIVE | FI_NAMED_RX_CTX | FI_HMEM)

#define OFI_SECONDARY_TX_CAPS \
	(FI_TRIGGER | FI_FENCE | FI_RMA_PMEM)

#define OFI_PRIMARY_RX_CAPS \
	(FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | \
	 FI_REMOTE_READ | FI_REMOTE_WRITE | FI_RECV | \
	 FI_DIRECTED_RECV | FI_VARIABLE_MSG | \
	 FI_COLLECTIVE | FI_HMEM)

#define OFI_SECONDARY_RX_CAPS \
	(FI_MULTI_RECV | FI_TRIGGER | FI_RMA_PMEM | FI_SOURCE | \
	 FI_RMA_EVENT | FI_SOURCE_ERR)

#define OFI_PRIMARY_CAPS \
	(OFI_PRIMARY_TX_CAPS | OFI_PRIMARY_RX_CAPS | \
	 FI_REMOTE_COMM | FI_LOCAL_COMM)

#define OFI_SECONDARY_CAPS \
	(OFI_SECONDARY_TX_CAPS | OFI_SECONDARY_RX_CAPS | \
	 FI_SHARED_AV)

#define OFI_TX_MSG_CAPS (FI_MSG | FI_SEND)
#define OFI_RX_MSG_CAPS (FI_MSG | FI_RECV)
#define OFI_TX_RMA_CAPS (FI_RMA | FI_READ | FI_WRITE)
#define OFI_RX_RMA_CAPS (FI_RMA | FI_REMOTE_READ | FI_REMOTE_WRITE)

#define OFI_IGNORED_TX_CAPS /* older Rx caps not applicable to Tx */ \
	(FI_REMOTE_READ | FI_REMOTE_WRITE | FI_RECV | FI_DIRECTED_RECV | \
	 FI_VARIABLE_MSG | FI_MULTI_RECV | FI_SOURCE | FI_RMA_EVENT | \
	 FI_SOURCE_ERR)
#define OFI_IGNORED_RX_CAPS /* Older Tx caps not applicable to Rx */ \
	(FI_READ | FI_WRITE | FI_SEND | FI_FENCE | FI_MULTICAST | \
	 FI_NAMED_RX_CTX)

#define OFI_TX_OP_FLAGS \
	(FI_COMMIT_COMPLETE | FI_COMPLETION | FI_DELIVERY_COMPLETE | \
	 FI_INJECT | FI_INJECT_COMPLETE | FI_MULTICAST | FI_TRANSMIT_COMPLETE)

#define OFI_RX_OP_FLAGS \
	(FI_COMPLETION | FI_MULTI_RECV)


#define sizeof_field(type, field) sizeof(((type *)0)->field)

#ifndef MIN
#define MIN(a, b) \
	({ typeof (a) _a = (a); \
		typeof (b) _b = (b); \
		_a < _b ? _a : _b; })
#endif

#ifndef MAX
#define MAX(a, b) \
	({ typeof (a) _a = (a); \
		typeof (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif

#define ofi_div_ceil(a, b) ((a + b - 1) / b)

static inline int ofi_val64_gt(uint64_t x, uint64_t y) {
	return ((int64_t) (x - y)) > 0;
}
static inline int ofi_val64_ge(uint64_t x, uint64_t y) {
	return ((int64_t) (x - y)) >= 0;
}
#define ofi_val64_lt(x, y) ofi_val64_gt(y, x)

static inline int ofi_val32_gt(uint32_t x, uint32_t y) {
	return ((int32_t) (x - y)) > 0;
}
static inline int ofi_val32_ge(uint32_t x, uint32_t y) {
	return ((int32_t) (x - y)) >= 0;
}
#define ofi_val32_lt(x, y) ofi_val32_gt(y, x)

#define ofi_val32_inrange(start, length, value) \
    ofi_val32_ge(value, start) && ofi_val32_lt(value, start + length)
#define ofi_val64_inrange(start, length, value) \
    ofi_val64_ge(value, start) && ofi_val64_lt(value, start + length)

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

#ifndef BIT_ULL
#define BIT_ULL(nr) (1ULL << (nr))
#endif

#ifndef GENMASK
#define GENMASK(h, l) (((1U << ((h) - (l) + 1)) - 1) << (l))
#endif

#ifndef GENMASK_ULL
#define GENMASK_ULL(h, l) (((~0ULL) << (l)) & (~0ULL >> (64 - 1 - (h))))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef STRINGIFY
#define __STRINGIFY(expr) #expr
#define STRINGIFY(expr) __STRINGIFY(expr)
#endif

#define TAB "    "

#define CASEENUMSTRN(SYM, N) \
	case SYM: { ofi_strncatf(buf, N, #SYM); break; }
#define IFFLAGSTRN(flags, SYM, N) \
	do { if (flags & SYM) ofi_strncatf(buf, N, #SYM ", "); } while(0)


/*
 * CPU specific features
 */

/* X86_64 */
enum {
	OFI_CLWB_REG		= 1,
	OFI_CLWB_BIT		= (1 << 24),
	OFI_CLFLUSHOPT_REG	= 1,
	OFI_CLFLUSHOPT_BIT	= (1 << 23),
	OFI_CLFLUSH_REG		= 3,
	OFI_CLFLUSH_BIT		= (1 << 19),
};

int ofi_cpu_supports(unsigned func, unsigned reg, unsigned bit);


enum ofi_prov_type {
	OFI_PROV_CORE,
	OFI_PROV_UTIL,
	OFI_PROV_HOOK,
	OFI_PROV_OFFLOAD,
};

/* Restrict to size of struct fi_provider::context (struct fi_context) */
struct ofi_prov_context {
	enum ofi_prov_type type;
	bool disable_logging;
	bool disable_layering;	/* applies to core providers only */
};

static inline struct ofi_prov_context *
ofi_prov_ctx(const struct fi_provider *prov)
{
	return (struct ofi_prov_context *) &prov->context;
}

struct ofi_filter {
	char **names;
	bool negated;
};

extern struct ofi_filter prov_log_filter;
extern struct fi_provider core_prov;
extern const char *log_prefix;

void ofi_create_filter(struct ofi_filter *filter, const char *env_name);
void ofi_free_filter(struct ofi_filter *filter);
int ofi_apply_filter(struct ofi_filter *filter, const char *name);

int ofi_nic_close(struct fid *fid);
struct fid_nic *ofi_nic_dup(const struct fid_nic *nic);
int ofi_nic_tostr(const struct fid *fid_nic, char *buf, size_t len);

void fi_log_init(void);
void fi_log_fini(void);
void fi_param_init(void);
void fi_param_fini(void);
void fi_param_undefine(const struct fi_provider *provider);
void ofi_remove_comma(char *buffer);
void ofi_strncatf(char *dest, size_t n, const char *fmt, ...);

const char *ofi_hex_str(const uint8_t *data, size_t len);

static inline uint64_t roundup_power_of_two(uint64_t n)
{
	if (!n || !(n & (n - 1)))
		return n;
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	n++;
	return n;
}

static inline uint64_t rounddown_power_of_two(uint64_t n)
{
	uint64_t pof2 = roundup_power_of_two(n);
	return (pof2 > n) ? (pof2 >> 1) : pof2;
}

static inline size_t ofi_get_aligned_size(size_t size, size_t alignment)
{
	return ((size % alignment) == 0) ?
		size : ((size / alignment) + 1) * alignment;
}

static inline void *ofi_get_page_start(const void *addr, size_t page_size)
{
	return (void *)((uintptr_t) addr & ~(page_size - 1));
}

static inline void *ofi_get_page_end(const void *addr, size_t page_size)
{
	return (void *)((uintptr_t)ofi_get_page_start((const char *)addr
			+ page_size, page_size) - 1);
}

static inline size_t
ofi_get_page_bytes(const void *addr, size_t len, size_t page_size)
{
	char *start = (char *)ofi_get_page_start(addr, page_size);
	char *end = (char *)ofi_get_page_start((const char*)addr + len - 1, page_size)
		    + page_size;
	size_t result = end - start;

	assert(result % page_size == 0);
	return result;
}

#define FI_TAG_GENERIC	0xAAAAAAAAAAAAAAAAULL


uint64_t ofi_max_tag(uint64_t mem_tag_format);
uint64_t ofi_tag_format(uint64_t max_tag);
uint8_t ofi_msb(uint64_t num);
uint8_t ofi_lsb(uint64_t num);

extern size_t ofi_universe_size;
extern int ofi_av_remove_cleanup;

bool ofi_send_allowed(uint64_t caps);
bool ofi_recv_allowed(uint64_t caps);
bool ofi_rma_initiate_allowed(uint64_t caps);
bool ofi_rma_target_allowed(uint64_t caps);
bool ofi_needs_tx(uint64_t caps);
bool ofi_needs_rx(uint64_t caps);

int ofi_ep_bind_valid(const struct fi_provider *prov, struct fid *bfid,
		      uint64_t flags);
int ofi_check_rx_mode(const struct fi_info *info, uint64_t flags);

uint64_t ofi_gettime_ns(void);
uint64_t ofi_gettime_us(void);
uint64_t ofi_gettime_ms(void);

static inline uint64_t ofi_timeout_time(int timeout)
{
	return (timeout >= 0) ? ofi_gettime_ms() + timeout : 0;
}

static inline int ofi_adjust_timeout(uint64_t timeout_time, int *timeout)
{
	if (*timeout >= 0) {
		*timeout = (int) (timeout_time - ofi_gettime_ms());
		return (*timeout <= 0) ? -FI_ETIMEDOUT : 0;
	}
	return 0;
}

#define OFI_ENUM_VAL(X) X
#define OFI_STR(X) #X
#define OFI_STR_INT(X) OFI_STR(X)


/*
 * Key Index
 */

/*
 * The key_idx object and related functions can be used to generate unique keys
 * from an index. The key and index would refer to an object defined by the user.
 * A local endpoint can exchange this key with a remote endpoint in the first message.
 * The remote endpoint would then use this key in subsequent messages to reference
 * the correct object at the local endpoint.
 */
struct ofi_key_idx {
	uint64_t seq_no;
	/* The uniqueness of the generated key would depend on how many bits are
	 * used for the index */
	uint8_t idx_bits;
};

static inline void ofi_key_idx_init(struct ofi_key_idx *key_idx, uint8_t idx_bits)
{
	key_idx->seq_no = 0;
	key_idx->idx_bits = idx_bits;
}

static inline uint64_t ofi_idx2key(struct ofi_key_idx *key_idx, uint64_t idx)
{
	return ((++(key_idx->seq_no)) << key_idx->idx_bits) | idx;
}

static inline uint64_t ofi_key2idx(struct ofi_key_idx *key_idx, uint64_t key)
{
	return key & ((1ULL << key_idx->idx_bits) - 1);
}

static inline uint32_t ofi_xorshift_random(uint32_t val)
{
	/*
	 * Xorshift Random Number Generators are from 224.
	 * R. P. Brent, Some long-period random number
	 * generators using shifts and xors, ANZIAM
	 * Journal 48 (CTAC2006), C188-C202, 2007.
	 * Presented at the 13th Biennial Computational
	 * Techniques and Applications
	 * Conference (CTAC06), Townsville, 2-5 July 2006.
	 * arXiv:1004.3115v1
	 */
	val ^= val << 13;
	val ^= val >> 17;
	val ^= val << 5;

	return val;
}

static inline uint32_t ofi_xorshift_random_r(uint32_t *seed)
{
	return *seed = ofi_xorshift_random(*seed);
}

uint32_t ofi_generate_seed(void);

size_t ofi_vrb_speed(uint8_t speed, uint8_t width);

int ofi_open_log(uint32_t version, void *attr, size_t attr_len,
		 uint64_t flags, struct fid **fid, void *context);
void ofi_tostr_log_level(char *buf, size_t len, enum fi_log_level level);
void ofi_tostr_log_subsys(char *buf, size_t len, enum fi_log_subsys subsys);

#ifdef __cplusplus
}
#endif

#endif /* _OFI_H_ */
