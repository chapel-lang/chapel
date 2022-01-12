/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014-2017, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#ifndef _SHARED_H_
#define _SHARED_H_

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <inttypes.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <stdbool.h>

#include <rdma/fabric.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_domain.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FT_FIVERSION
#define FT_FIVERSION FI_VERSION(1,9)
#endif

#include "ft_osd.h"
#define OFI_UTIL_PREFIX "ofi_"
#define OFI_NAME_DELIM ';'

#define ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define ALIGN(x, a) ALIGN_MASK(x, (typeof(x))(a) - 1)
#define ALIGN_DOWN(x, a) ALIGN((x) - ((a) - 1), (a))

#define OFI_MR_BASIC_MAP (FI_MR_ALLOCATED | FI_MR_PROV_KEY | FI_MR_VIRT_ADDR)

/* exit codes must be 0-255 */
static inline int ft_exit_code(int ret)
{
	int absret = ret < 0 ? -ret : ret;
	return absret > 255 ? EXIT_FAILURE : absret;
}

#define ft_sa_family(addr) (((struct sockaddr *)(addr))->sa_family)

struct test_size_param {
	size_t size;
	int enable_flags;
};

extern struct test_size_param test_size[];
extern const unsigned int test_cnt;
#define TEST_CNT test_cnt

#define FT_ENABLE_ALL		(~0)
#define FT_DEFAULT_SIZE		(1 << 0)

enum precision {
	NANO = 1,
	MICRO = 1000,
	MILLI = 1000000,
};

enum ft_comp_method {
	FT_COMP_SPIN = 0,
	FT_COMP_SREAD,
	FT_COMP_WAITSET,
	FT_COMP_WAIT_FD,
	FT_COMP_YIELD,
};

enum {
	FT_OPT_ACTIVE		= 1 << 0,
	FT_OPT_ITER		= 1 << 1,
	FT_OPT_SIZE		= 1 << 2,
	FT_OPT_RX_CQ		= 1 << 3,
	FT_OPT_TX_CQ		= 1 << 4,
	FT_OPT_RX_CNTR		= 1 << 5,
	FT_OPT_TX_CNTR		= 1 << 6,
	FT_OPT_VERIFY_DATA	= 1 << 7,
	FT_OPT_ALIGN		= 1 << 8,
	FT_OPT_BW		= 1 << 9,
	FT_OPT_CQ_SHARED	= 1 << 10,
	FT_OPT_OOB_SYNC		= 1 << 11,
	FT_OPT_SKIP_MSG_ALLOC	= 1 << 12,
	FT_OPT_SKIP_REG_MR	= 1 << 13,
	FT_OPT_OOB_ADDR_EXCH	= 1 << 14,
	FT_OPT_ALLOC_MULT_MR	= 1 << 15,
	FT_OPT_SERVER_PERSIST	= 1 << 16,
	FT_OPT_ENABLE_HMEM	= 1 << 17,
	FT_OPT_USE_DEVICE	= 1 << 18,
	FT_OPT_DOMAIN_EQ	= 1 << 19,
	FT_OPT_FORK_CHILD	= 1 << 20,
	FT_OPT_OOB_CTRL		= FT_OPT_OOB_SYNC | FT_OPT_OOB_ADDR_EXCH,
};

/* for RMA tests --- we want to be able to select fi_writedata, but there is no
 * constant in libfabric for this */
enum ft_rma_opcodes {
	FT_RMA_READ = 1,
	FT_RMA_WRITE,
	FT_RMA_WRITEDATA,
};

enum ft_atomic_opcodes {
	FT_ATOMIC_BASE,
	FT_ATOMIC_FETCH,
	FT_ATOMIC_COMPARE,
};

enum op_state {
	OP_DONE = 0,
	OP_PENDING
};

struct ft_context {
	char *buf;
	void *desc;
	enum op_state state;
	struct fid_mr *mr;
	struct fi_context2 context;
};

struct ft_opts {
	int iterations;
	int warmup_iterations;
	size_t transfer_size;
	int window_size;
	int av_size;
	int verbose;
	int tx_cq_size;
	int rx_cq_size;
	char *src_port;
	char *dst_port;
	char *src_addr;
	char *dst_addr;
	char *av_name;
	int sizes_enabled;
	int options;
	enum ft_comp_method comp_method;
	int machr;
	enum ft_rma_opcodes rma_op;
	char *oob_port;
	int argc;
	int num_connections;
	int address_format;

	uint64_t mr_mode;
	/* Fail if the selected provider does not support FI_MSG_PREFIX.  */
	int force_prefix;
	enum fi_hmem_iface iface;
	uint64_t device;

	char **argv;
};

extern struct fi_info *fi_pep, *fi, *hints;
extern struct fid_fabric *fabric;
extern struct fid_wait *waitset;
extern struct fid_domain *domain;
extern struct fid_poll *pollset;
extern struct fid_pep *pep;
extern struct fid_ep *ep, *alias_ep;
extern struct fid_cq *txcq, *rxcq;
extern struct fid_cntr *txcntr, *rxcntr;
extern struct fid_mr *mr, no_mr;
extern void *mr_desc;
extern struct fid_av *av;
extern struct fid_eq *eq;
extern struct fid_mc *mc;

extern fi_addr_t remote_fi_addr;
extern char *buf, *tx_buf, *rx_buf;
extern struct ft_context *tx_ctx_arr, *rx_ctx_arr;
extern char **tx_mr_bufs, **rx_mr_bufs;
extern size_t buf_size, tx_size, rx_size, tx_mr_size, rx_mr_size;
extern int tx_fd, rx_fd;
extern int timeout;

extern struct fi_context tx_ctx, rx_ctx;
extern uint64_t remote_cq_data;

extern uint64_t tx_seq, rx_seq, tx_cq_cntr, rx_cq_cntr;
extern struct fi_av_attr av_attr;
extern struct fi_eq_attr eq_attr;
extern struct fi_cq_attr cq_attr;
extern struct fi_cntr_attr cntr_attr;

extern struct fi_rma_iov remote;

extern char test_name[50];
extern struct timespec start, end;
extern struct ft_opts opts;

void ft_parseinfo(int op, char *optarg, struct fi_info *hints,
		  struct ft_opts *opts);
void ft_parse_addr_opts(int op, char *optarg, struct ft_opts *opts);
void ft_parsecsopts(int op, char *optarg, struct ft_opts *opts);
int ft_parse_rma_opts(int op, char *optarg, struct fi_info *hints,
		      struct ft_opts *opts);
void ft_addr_usage();
void ft_usage(char *name, char *desc);
void ft_mcusage(char *name, char *desc);
void ft_csusage(char *name, char *desc);

int ft_fill_buf(void *buf, size_t size);
int ft_check_buf(void *buf, size_t size);
int ft_check_opts(uint64_t flags);
uint64_t ft_init_cq_data(struct fi_info *info);
int ft_sock_listen(char *node, char *service);
int ft_sock_connect(char *node, char *service);
int ft_sock_accept();
int ft_sock_send(int fd, void *msg, size_t len);
int ft_sock_recv(int fd, void *msg, size_t len);
int ft_sock_sync(int value);
void ft_sock_shutdown(int fd);
extern int (*ft_mr_alloc_func)(void);
extern uint64_t ft_tag;
extern int ft_parent_proc;
extern int ft_socket_pair[2];
extern int sock;
extern int listen_sock;
#define ADDR_OPTS "B:P:s:a:b::E::C:F:"
#define FAB_OPTS "f:d:p:D:i:HK"
#define INFO_OPTS FAB_OPTS "e:M:"
#define CS_OPTS ADDR_OPTS "I:QS:mc:t:w:l"
#define NO_CQ_DATA 0

extern char default_port[8];

#define INIT_OPTS (struct ft_opts) \
	{	.options = FT_OPT_RX_CQ | FT_OPT_TX_CQ, \
		.iterations = 1000, \
		.warmup_iterations = 10, \
		.transfer_size = 1024, \
		.window_size = 64, \
		.av_size = 1, \
		.tx_cq_size = 0, \
		.rx_cq_size = 0, \
		.verbose = 0, \
		.sizes_enabled = FT_DEFAULT_SIZE, \
		.rma_op = FT_RMA_WRITE, \
		.oob_port = NULL, \
		.mr_mode = FI_MR_LOCAL | OFI_MR_BASIC_MAP, \
		.iface = FI_HMEM_SYSTEM, \
		.device = 0, \
		.argc = argc, .argv = argv, \
		.address_format = FI_FORMAT_UNSPEC \
	}

#define FT_STR_LEN 32
#define FT_MAX_CTRL_MSG 256
#define FT_MR_KEY 0xC0DE
#define FT_TX_MR_KEY (FT_MR_KEY + 1)
#define FT_RX_MR_KEY 0xFFFF
#define FT_MSG_MR_ACCESS (FI_SEND | FI_RECV)
#define FT_RMA_MR_ACCESS (FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE)

int ft_getsrcaddr(char *node, char *service, struct fi_info *hints);
int ft_read_addr_opts(char **node, char **service, struct fi_info *hints,
		uint64_t *flags, struct ft_opts *opts);
char *size_str(char str[FT_STR_LEN], long long size);
char *cnt_str(char str[FT_STR_LEN], long long cnt);
int size_to_count(int size);
size_t datatype_to_size(enum fi_datatype datatype);

static inline int ft_use_size(int index, int enable_flags)
{
	return test_size[index].size <= fi->ep_attr->max_msg_size &&
		((enable_flags == FT_ENABLE_ALL) ||
		(enable_flags & test_size[index].enable_flags));
}

#define FT_PRINTERR(call, retv) \
	do { fprintf(stderr, call "(): %s:%d, ret=%d (%s)\n", __FILE__, __LINE__, \
			(int) (retv), fi_strerror((int) -(retv))); } while (0)

#define FT_LOG(level, fmt, ...) \
	do { fprintf(stderr, "[%s] fabtests:%s:%d: " fmt "\n", level, __FILE__, \
			__LINE__, ##__VA_ARGS__); } while (0)

#define FT_ERR(fmt, ...) FT_LOG("error", fmt, ##__VA_ARGS__)
#define FT_WARN(fmt, ...) FT_LOG("warn", fmt, ##__VA_ARGS__)

#if ENABLE_DEBUG
#define FT_DEBUG(fmt, ...) FT_LOG("debug", fmt, ##__VA_ARGS__)
#else
#define FT_DEBUG(fmt, ...)
#endif

#define FT_EQ_ERR(eq, entry, buf, len)				\
	FT_ERR("eq_readerr (Provider errno: %d) : %s",			\
		entry.prov_errno, fi_eq_strerror(eq, entry.prov_errno,	\
						 entry.err_data,	\
						 buf, len))		\

#define FT_CQ_ERR(cq, entry, buf, len)				\
	FT_ERR("cq_readerr (Provider errno: %d) : %s",			\
		entry.prov_errno, fi_cq_strerror(cq, entry.prov_errno,	\
						 entry.err_data,	\
						 buf, len))		\

#define FT_CLOSE_FID(fd)						\
	do {								\
		int ret;						\
		if ((fd)) {						\
			ret = fi_close(&(fd)->fid);			\
			if (ret)					\
				FT_ERR("fi_close: %s(%d) fid %d",	\
					fi_strerror(-ret), 		\
					ret,				\
					(int) (fd)->fid.fclass);	\
			fd = NULL;					\
		}							\
	} while (0)

#define FT_CLOSEV_FID(fd, cnt)			\
	do {					\
		int i;				\
		if (!(fd))			\
			break;			\
		for (i = 0; i < (cnt); i++) {	\
			FT_CLOSE_FID((fd)[i]);	\
		}				\
	} while (0)

#define FT_EP_BIND(ep, fd, flags)					\
	do {								\
		int ret;						\
		if ((fd)) {						\
			ret = fi_ep_bind((ep), &(fd)->fid, (flags));	\
			if (ret) {					\
				FT_PRINTERR("fi_ep_bind", ret);		\
				return ret;				\
			}						\
		}							\
	} while (0)

int ft_alloc_bufs();
int ft_open_fabric_res();
int ft_getinfo(struct fi_info *hints, struct fi_info **info);
int ft_init_fabric();
int ft_init_oob();
int ft_start_server();
int ft_server_connect();
int ft_client_connect();
int ft_init_fabric_cm(void);
int ft_complete_connect(struct fid_ep *ep, struct fid_eq *eq);
int ft_retrieve_conn_req(struct fid_eq *eq, struct fi_info **fi);
int ft_accept_connection(struct fid_ep *ep, struct fid_eq *eq);
int ft_connect_ep(struct fid_ep *ep,
		struct fid_eq *eq, fi_addr_t *remote_addr);
int ft_alloc_ep_res(struct fi_info *fi);
int ft_alloc_active_res(struct fi_info *fi);
int ft_enable_ep_recv(void);
int ft_enable_ep(struct fid_ep *ep, struct fid_eq *eq, struct fid_av *av,
		 struct fid_cq *txcq, struct fid_cq *rxcq,
		 struct fid_cntr *txcntr, struct fid_cntr *rxcntr);
int ft_init_alias_ep(uint64_t flags);
int ft_av_insert(struct fid_av *av, void *addr, size_t count, fi_addr_t *fi_addr,
		uint64_t flags, void *context);
int ft_init_av(void);
int ft_join_mc(void);
int ft_init_av_dst_addr(struct fid_av *av_ptr, struct fid_ep *ep_ptr,
		fi_addr_t *remote_addr);
int ft_init_av_addr(struct fid_av *av, struct fid_ep *ep,
		fi_addr_t *addr);
int ft_exchange_keys(struct fi_rma_iov *peer_iov);
void ft_free_res();
void init_test(struct ft_opts *opts, char *test_name, size_t test_name_len);

static inline void ft_start(void)
{
	opts.options |= FT_OPT_ACTIVE;
	clock_gettime(CLOCK_MONOTONIC, &start);
}
static inline void ft_stop(void)
{
	clock_gettime(CLOCK_MONOTONIC, &end);
	opts.options &= ~FT_OPT_ACTIVE;
}

/* Set the FI_MSG_PREFIX mode bit in the given fi_info structure and also set
 * the option bit in the given opts structure. If using ft_getinfo, it will
 * return -ENODATA if the provider clears the application requested mdoe bit.
 */
static inline void ft_force_prefix(struct fi_info *info, struct ft_opts *opts)
{
	info->mode |= FI_MSG_PREFIX;
	opts->force_prefix = 1;
}

/* If force_prefix was not requested, just continue. If it was requested,
 * return true if it was respected by the provider.
 */
static inline bool ft_check_prefix_forced(struct fi_info *info,
					 struct ft_opts *opts)
{
	if (opts->force_prefix) {
		return (info->tx_attr->mode & FI_MSG_PREFIX) &&
		       (info->rx_attr->mode & FI_MSG_PREFIX);
	}

	/* Continue if forced prefix wasn't requested. */
	return true;
}

int ft_sync(void);
int ft_sync_pair(int status);
int ft_fork_and_pair(void);
int ft_fork_child(void);
int ft_wait_child(void);
int ft_finalize(void);
int ft_finalize_ep(struct fid_ep *ep);

size_t ft_rx_prefix_size(void);
size_t ft_tx_prefix_size(void);
ssize_t ft_post_rx(struct fid_ep *ep, size_t size, void *ctx);
ssize_t ft_post_rx_buf(struct fid_ep *ep, size_t size, void *ctx,
		       void *op_buf, void *op_mr_desc, uint64_t op_tag);
ssize_t ft_post_tx(struct fid_ep *ep, fi_addr_t fi_addr, size_t size,
		uint64_t data, void *ctx);
ssize_t ft_post_tx_buf(struct fid_ep *ep, fi_addr_t fi_addr, size_t size,
		       uint64_t data, void *ctx,
		       void *op_buf, void *op_mr_desc, uint64_t op_tag);
ssize_t ft_rx(struct fid_ep *ep, size_t size);
ssize_t ft_tx(struct fid_ep *ep, fi_addr_t fi_addr, size_t size, void *ctx);
ssize_t ft_inject(struct fid_ep *ep, fi_addr_t fi_addr, size_t size);
ssize_t ft_post_rma(enum ft_rma_opcodes op, struct fid_ep *ep, size_t size,
		struct fi_rma_iov *remote, void *context);
ssize_t ft_rma(enum ft_rma_opcodes op, struct fid_ep *ep, size_t size,
		struct fi_rma_iov *remote, void *context);
ssize_t ft_post_rma_inject(enum ft_rma_opcodes op, struct fid_ep *ep, size_t size,
		struct fi_rma_iov *remote);


ssize_t ft_post_atomic(enum ft_atomic_opcodes opcode, struct fid_ep *ep,
		       void *compare, void *compare_desc, void *result,
		       void *result_desc, struct fi_rma_iov *remote,
		       enum fi_datatype datatype, enum fi_op atomic_op,
		       void *context);
int check_base_atomic_op(struct fid_ep *endpoint, enum fi_op op,
			 enum fi_datatype datatype, size_t *count);
int check_fetch_atomic_op(struct fid_ep *endpoint, enum fi_op op,
			  enum fi_datatype datatype, size_t *count);
int check_compare_atomic_op(struct fid_ep *endpoint, enum fi_op op,
			    enum fi_datatype datatype, size_t *count);

int ft_cq_readerr(struct fid_cq *cq);
int ft_get_rx_comp(uint64_t total);
int ft_get_tx_comp(uint64_t total);
int ft_recvmsg(struct fid_ep *ep, fi_addr_t fi_addr,
		size_t size, void *ctx, int flags);
int ft_sendmsg(struct fid_ep *ep, fi_addr_t fi_addr,
		size_t size, void *ctx, int flags);
int ft_cq_read_verify(struct fid_cq *cq, void *op_context);

void eq_readerr(struct fid_eq *eq, const char *eq_str);

int64_t get_elapsed(const struct timespec *b, const struct timespec *a,
		enum precision p);
void show_perf(char *name, size_t tsize, int iters, struct timespec *start,
		struct timespec *end, int xfers_per_iter);
void show_perf_mr(size_t tsize, int iters, struct timespec *start,
		struct timespec *end, int xfers_per_iter, int argc, char *argv[]);

int ft_send_recv_greeting(struct fid_ep *ep);
int ft_send_greeting(struct fid_ep *ep);
int ft_recv_greeting(struct fid_ep *ep);

int ft_accept_next_client();

int check_recv_msg(const char *message);
uint64_t ft_info_to_mr_access(struct fi_info *info);
int ft_alloc_bit_combo(uint64_t fixed, uint64_t opt, uint64_t **combos, int *len);
void ft_free_bit_combo(uint64_t *combo);
int ft_cntr_open(struct fid_cntr **cntr);
const char *ft_util_name(const char *str, size_t *len);
const char *ft_core_name(const char *str, size_t *len);
char **ft_split_and_alloc(const char *s, const char *delim, size_t *count);
void ft_free_string_array(char **s);

#define FT_PROCESS_QUEUE_ERR(readerr, rd, queue, fn, str)	\
	do {							\
		if (rd == -FI_EAVAIL) {				\
			readerr(queue, fn " " str);		\
		} else {					\
			FT_PRINTERR(fn, rd);			\
		}						\
	} while (0)

#define FT_PROCESS_EQ_ERR(rd, eq, fn, str) \
	FT_PROCESS_QUEUE_ERR(eq_readerr, rd, eq, fn, str)

#define FT_OPTS_USAGE_FORMAT "%-30s %s"
#define FT_PRINT_OPTS_USAGE(opt, desc) fprintf(stderr, FT_OPTS_USAGE_FORMAT "\n", opt, desc)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ARRAY_SIZE(A) (sizeof(A)/sizeof(*A))

#define TEST_ENUM_SET_N_RETURN(str, len,  enum_val, type, data)	\
	TEST_SET_N_RETURN(str, len, #enum_val, enum_val, type, data)

#define TEST_SET_N_RETURN(str, len, val_str, val, type, data)	\
	do {							\
		if (!strncmp(str, val_str, len)) {	\
			*(type *)(data) = val;			\
			return 0;				\
		}						\
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* _SHARED_H_ */
