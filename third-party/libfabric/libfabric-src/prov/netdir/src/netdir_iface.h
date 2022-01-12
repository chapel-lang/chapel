/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_IFACE_H_
#define _FI_NETDIR_IFACE_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <ndspi.h>

#include "rdma/fabric.h"

#include "ofi_mem.h"
#include "ofi_list.h"
#include "rdma/fi_eq.h"
#include "rdma/fi_domain.h"
#include "rdma/fi_endpoint.h"

#include "netdir_buf.h"
#include "netdir_queue.h"
#include "netdir_ov.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void(*nd_free_event_t)(struct nd_event_base* base);
typedef void(*nd_event_t)(struct nd_event_base* base, DWORD bytes);
typedef void(*nd_err_t)(struct nd_event_base* base, DWORD bytes, DWORD err);

typedef struct nd_event_base {
	OVERLAPPED		ov;

	nd_free_event_t		free;
	nd_event_t		event_cb;
	nd_err_t		err_cb;
} nd_event_base;

struct nd_fabric {
	struct fid_fabric	fid;
};

typedef struct nd_flow_cntrl_flags {
	unsigned req_ack : 1;
	unsigned ack : 1;
	unsigned empty : 1;
} nd_flow_cntrl_flags;

struct nd_msgheader {
	uint64_t		data;
	enum ofi_nd_cq_event	event;
	nd_flow_cntrl_flags	flags;
	size_t			location_cnt;
};

struct nd_msgprefix {
	UINT32			token;
	struct nd_msgheader	header;
};

struct nd_inlinebuf {
	UINT32			token;
	void*			buffer;
};

struct nd_msg_location {
	uint64_t addr;
	size_t	 len;
	uint32_t remote_mr_token;
};

struct nd_notifybuf {
	UINT32			token;
	struct nd_msg_location	location[ND_MSG_IOV_LIMIT];
};

OFI_ND_NB_BUF_TYPED(nd_msgprefix, struct nd_msgprefix);
OFI_ND_NB_BUF_TYPED(nd_inlinebuf, struct nd_inlinebuf);
OFI_ND_NB_BUF_TYPED(nd_notifybuf, struct nd_notifybuf);

struct nd_domain {
	struct fid_domain		fid;
	struct nd_eq			*eq;
	struct fi_info			*info;

	uint64_t			eq_flags;

	IND2Adapter			*adapter;
	IND2CompletionQueue		*cq;

	nd_event_base			ov;

	HANDLE				adapter_file;
	ND2_ADAPTER_INFO		ainfo;

	LONG64				msg_cnt;

	LONG				cq_canceled;

	ND_BUF_FOOTER(nd_msgprefix)	msgfooter;
	ND_BUF_FOOTER(nd_inlinebuf)	inlinebuf;
	ND_BUF_FOOTER(nd_notifybuf)	notifybuf;

	union {
		struct sockaddr		addr;
		struct sockaddr_in	addr4;
		struct sockaddr_in6	addr6;
	} addr;
#if 0
	pthread_t			progress_thread;
	int				do_progress;
#endif
	struct dlist_entry		ep_list;
};

struct nd_pep {
	struct fid_pep	fid;
	struct fi_info	*info;

	struct nd_eq	*eq;

	IND2Adapter	*adapter;
	IND2Listener	*listener;

	HANDLE		adapter_file;
};

struct nd_eq {
	struct fid_eq		fid;
	size_t			cnum;
	HANDLE			iocp;
	HANDLE			err;
	volatile LONG		count; /* total number of available events,
				          including peek, queued & errors */
	struct nd_eq_event	*peek;

	CRITICAL_SECTION	lock;
	void*			errdata;
};


struct nd_cq {
	struct fid_cq		fid;
	enum fi_cq_format	format;

	HANDLE			iocp;
	HANDLE			err;
	volatile LONG		count; /* total number of available events,
					  including queued & errors */
};

struct nd_cntr {
	struct fid_cntr		fid;
	volatile LONG64		counter;
	volatile LONG64		err;
};

struct nd_connreq {
	struct fid	handle;
	IND2Connector	*connector;
};

struct nd_unexpected {
	IND2MemoryRegion		*mr;
	UINT32				token;
	struct nd_unexpected_buf	**unexpected;
#if 0
	size_t				used_counter;
#endif
	CRITICAL_SECTION		unexp_lock;
	struct dlist_entry		received;
	LONG				active;
};

typedef struct nd_flow_block_flags {
	unsigned is_send_blocked : 1;
} nd_flow_block_flags;

struct nd_ep {
	struct fid_ep			fid;
	struct fi_info			*info;

	struct nd_domain		*domain;
	struct nd_eq			*eq;
	struct nd_srx			*srx;

	struct nd_cq			*cq_send;
	struct nd_cq			*cq_recv;

	uint64_t			send_flags;
	uint64_t			recv_flags;

	struct nd_cntr			*cntr_send;
	struct nd_cntr			*cntr_recv;
	struct nd_cntr			*cntr_read;
	struct nd_cntr			*cntr_write;

	IND2Connector			*connector;
	IND2QueuePair			*qp;

	struct nd_unexpected		unexpected;
	struct nd_queue_queue		prepost;
	struct nd_queue_queue		internal_prepost;

	nd_event_base			disconnect_ov;

	CRITICAL_SECTION		prepost_lock;
	LONG				shutdown;
	LONG				connected;

	struct dlist_entry		entry;
	struct {
		nd_flow_block_flags	flags;
		size_t			used_counter;
		CRITICAL_SECTION	send_lock;
	} send_op;
	struct nd_queue_queue		send_queue;
};

struct nd_srx {
	struct fid_ep		fid;
	struct fi_rx_attr	attr;
	IND2SharedReceiveQueue	*srx;
	struct nd_domain	*domain;
	struct dlist_entry	received;
	CRITICAL_SECTION	prepost_lock;
	struct nd_queue_queue	prepost;
};

struct nd_mr {
	struct fid_mr		fid;

	IND2MemoryRegion	*mr;
	IND2MemoryWindow	*wnd;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_IFACE_H_ */

