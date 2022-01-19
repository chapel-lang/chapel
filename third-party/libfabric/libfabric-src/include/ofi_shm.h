/*
 * Copyright (c) 2016-2021 Intel Corporation. All rights reserved.
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

#ifndef _OFI_SHM_H_
#define _OFI_SHM_H_

#include "config.h"

#include <stdint.h>
#include <stddef.h>
#include <sys/un.h>

#include <ofi_atom.h>
#include <ofi_proto.h>
#include <ofi_mem.h>
#include <ofi_rbuf.h>
#include <ofi_tree.h>

#include <rdma/providers/fi_prov.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SMR_VERSION	1

#ifdef HAVE_ATOMICS
#define SMR_FLAG_ATOMIC	(1 << 0)
#else
#define SMR_FLAG_ATOMIC	(0 << 0)
#endif

#if ENABLE_DEBUG
#define SMR_FLAG_DEBUG	(1 << 1)
#else
#define SMR_FLAG_DEBUG	(0 << 1)
#endif

#define SMR_FLAG_IPC_SOCK (1 << 2)

#define SMR_CMD_SIZE		128	/* align with 64-byte cache line */

/* SMR op_src: Specifies data source location */
enum {
	smr_src_inline,	/* command data */
	smr_src_inject,	/* inject buffers */
	smr_src_iov,	/* reference iovec via CMA */
	smr_src_mmap,	/* mmap-based fallback protocol */
	smr_src_sar,	/* segmentation fallback protocol */
	smr_src_ipc,	/* device IPC handle protocol */
};

//reserves 0-255 for defined ops and room for new ops
//256 and beyond reserved for ctrl ops
#define SMR_OP_MAX (1 << 8)

#define SMR_REMOTE_CQ_DATA	(1 << 0)
#define SMR_RMA_REQ		(1 << 1)
#define SMR_TX_COMPLETION	(1 << 2)
#define SMR_RX_COMPLETION	(1 << 3)
#define SMR_MULTI_RECV		(1 << 4)

/* CMA capability */
enum {
	SMR_CMA_CAP_NA,
	SMR_CMA_CAP_ON,
	SMR_CMA_CAP_OFF,
};

/*
 * Unique smr_op_hdr for smr message protocol:
 * 	addr - local shm_id of peer sending msg (for shm lookup)
 * 	op - type of op (ex. ofi_op_msg, defined in ofi_proto.h)
 * 	op_src - msg src (ex. smr_src_inline, defined above)
 * 	op_flags - operation flags (ex. SMR_REMOTE_CQ_DATA, defined above)
 * 	src_data - src of additional op data (inject offset / resp offset)
 * 	data - remote CQ data
 */
struct smr_msg_hdr {
	uint64_t		msg_id;
	int64_t			id;
	uint32_t		op;
	uint16_t		op_src;
	uint16_t		op_flags;

	uint64_t		size;
	uint64_t		src_data;
	uint64_t		data;
	union {
		uint64_t	tag;
		struct {
			uint8_t	datatype;
			uint8_t	atomic_op;
		};
	};
};

#define SMR_MSG_DATA_LEN	(SMR_CMD_SIZE - sizeof(struct smr_msg_hdr))
#define SMR_COMP_DATA_LEN	(SMR_MSG_DATA_LEN / 2)

#define IPC_HANDLE_SIZE		64
struct smr_ipc_info {
	uint64_t	iface;
	union {
		uint8_t		ipc_handle[IPC_HANDLE_SIZE];
		struct {
			uint64_t	device;
			uint64_t	offset;
			uint64_t	fd_handle;
		};
	};
};

union smr_cmd_data {
	uint8_t			msg[SMR_MSG_DATA_LEN];
	struct {
		size_t		iov_count;
		struct iovec	iov[(SMR_MSG_DATA_LEN - sizeof(size_t)) /
				    sizeof(struct iovec)];
	};
	struct {
		uint8_t		buf[SMR_COMP_DATA_LEN];
		uint8_t		comp[SMR_COMP_DATA_LEN];
	};
	struct {
		uint64_t	sar;
	};
	struct smr_ipc_info	ipc_info;
};

struct smr_cmd_msg {
	struct smr_msg_hdr	hdr;
	union smr_cmd_data	data;
};

#define SMR_RMA_DATA_LEN	(128 - sizeof(uint64_t))
struct smr_cmd_rma {
	uint64_t		rma_count;
	union {
		struct fi_rma_iov	rma_iov[SMR_RMA_DATA_LEN /
						sizeof(struct fi_rma_iov)];
		struct fi_rma_ioc	rma_ioc[SMR_RMA_DATA_LEN /
						sizeof(struct fi_rma_ioc)];
	};
};

struct smr_cmd {
	union {
		struct smr_cmd_msg	msg;
		struct smr_cmd_rma	rma;
	};
};

#define SMR_INJECT_SIZE		4096
#define SMR_COMP_INJECT_SIZE	(SMR_INJECT_SIZE / 2)
#define SMR_SAR_SIZE		16384

#define SMR_NAME_MAX		256
#define SMR_SOCK_NAME_MAX sizeof(((struct sockaddr_un *)0)->sun_path)

struct smr_addr {
	char		name[SMR_NAME_MAX];
	int64_t		id;
};

struct smr_peer_data {
	struct smr_addr		addr;
	uint32_t		sar_status;
	uint32_t		name_sent;
};

extern struct dlist_entry ep_name_list;
extern pthread_mutex_t ep_list_lock;

struct smr_region;

struct smr_ep_name {
	char name[SMR_NAME_MAX];
	struct smr_region *region;
	struct dlist_entry entry;
};

static inline const char *smr_no_prefix(const char *addr)
{
	char *start;

	return (start = strstr(addr, "://")) ? start + 3 : addr;
}

struct smr_peer {
	struct smr_addr		peer;
	fi_addr_t		fiaddr;
	struct smr_region	*region;
};

#define SMR_MAX_PEERS	256

struct smr_map {
	fastlock_t		lock;
	int64_t			cur_id;
	struct ofi_rbmap	rbmap;
	struct smr_peer		peers[SMR_MAX_PEERS];
};

struct smr_region {
	uint8_t		version;
	uint8_t		resv;
	uint16_t	flags;
	int		pid;
	uint8_t		cma_cap_peer;
	uint8_t		cma_cap_self;
	void		*base_addr;
	fastlock_t	lock; /* lock for shm access
				 Must hold smr->lock before tx/rx cq locks
				 in order to progress or post recv */
	ofi_atomic32_t	signal;

	struct smr_map	*map;

	size_t		total_size;
	size_t		cmd_cnt; /* Doubles as a tracker for number of cmds AND
				    number of inject buffers available for use,
				    to ensure 1:1 ratio of cmds to inject bufs.
				    Might not always be paired consistently with
				    cmd alloc/free depending on protocol
				    (Ex. unexpected messages, RMA requests) */
	size_t		sar_cnt;

	/* offsets from start of smr_region */
	size_t		cmd_queue_offset;
	size_t		resp_queue_offset;
	size_t		inject_pool_offset;
	size_t		sar_pool_offset;
	size_t		peer_data_offset;
	size_t		name_offset;
	size_t		sock_name_offset;
};

struct smr_resp {
	uint64_t	msg_id;
	uint64_t	status;
};

struct smr_inject_buf {
	union {
		uint8_t		data[SMR_INJECT_SIZE];
		struct {
			uint8_t	buf[SMR_COMP_INJECT_SIZE];
			uint8_t comp[SMR_COMP_INJECT_SIZE];
		};
	};
};

enum {
	SMR_SAR_FREE = 0, /* buffer can be used */
	SMR_SAR_READY, /* buffer has data in it */
};

struct smr_sar_buf {
	uint64_t	status;
	uint8_t		buf[SMR_SAR_SIZE];
};

struct smr_sar_msg {
	struct smr_sar_buf	sar[2];
};

OFI_DECLARE_CIRQUE(struct smr_cmd, smr_cmd_queue);
OFI_DECLARE_CIRQUE(struct smr_resp, smr_resp_queue);
SMR_DECLARE_FREESTACK(struct smr_inject_buf, smr_inject_pool);
SMR_DECLARE_FREESTACK(struct smr_sar_msg, smr_sar_pool);

static inline struct smr_region *smr_peer_region(struct smr_region *smr, int i)
{
	return smr->map->peers[i].region;
}
static inline struct smr_cmd_queue *smr_cmd_queue(struct smr_region *smr)
{
	return (struct smr_cmd_queue *) ((char *) smr + smr->cmd_queue_offset);
}
static inline struct smr_resp_queue *smr_resp_queue(struct smr_region *smr)
{
	return (struct smr_resp_queue *) ((char *) smr + smr->resp_queue_offset);
}
static inline struct smr_inject_pool *smr_inject_pool(struct smr_region *smr)
{
	return (struct smr_inject_pool *) ((char *) smr + smr->inject_pool_offset);
}
static inline struct smr_peer_data *smr_peer_data(struct smr_region *smr)
{
	return (struct smr_peer_data *) ((char *) smr + smr->peer_data_offset);
}
static inline struct smr_sar_pool *smr_sar_pool(struct smr_region *smr)
{
	return (struct smr_sar_pool *) ((char *) smr + smr->sar_pool_offset);
}
static inline const char *smr_name(struct smr_region *smr)
{
	return (const char *) smr + smr->name_offset;
}

static inline char *smr_sock_name(struct smr_region *smr)
{
	return (char *) smr + smr->sock_name_offset;
}

static inline void smr_set_map(struct smr_region *smr, struct smr_map *map)
{
	smr->map = map;
}

struct smr_attr {
	const char	*name;
	size_t		rx_count;
	size_t		tx_count;
};

size_t smr_calculate_size_offsets(size_t tx_count, size_t rx_count,
				  size_t *cmd_offset, size_t *resp_offset,
				  size_t *inject_offset, size_t *sar_offset,
				  size_t *peer_offset, size_t *name_offset,
				  size_t *sock_offset);
void	smr_cma_check(struct smr_region *region, struct smr_region *peer_region);
void	smr_cleanup(void);
int	smr_map_create(const struct fi_provider *prov, int peer_count,
		       struct smr_map **map);
int	smr_map_to_region(const struct fi_provider *prov,
			  struct smr_peer *peer_buf);
void	smr_map_to_endpoint(struct smr_region *region, int64_t id);
void	smr_unmap_from_endpoint(struct smr_region *region, int64_t id);
void	smr_exchange_all_peers(struct smr_region *region);
int	smr_map_add(const struct fi_provider *prov,
		    struct smr_map *map, const char *name, int64_t *id);
void	smr_map_del(struct smr_map *map, int64_t id);
void	smr_map_free(struct smr_map *map);

struct smr_region *smr_map_get(struct smr_map *map, int64_t id);

int	smr_create(const struct fi_provider *prov, struct smr_map *map,
		   const struct smr_attr *attr, struct smr_region *volatile *smr);
void	smr_free(struct smr_region *smr);

static inline void smr_signal(struct smr_region *smr)
{
	ofi_atomic_set32(&smr->signal, 1);
}

#ifdef __cplusplus
}
#endif

#endif /* _OFI_SHM_H_ */
