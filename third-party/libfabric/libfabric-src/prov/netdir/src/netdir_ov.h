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

#ifndef _FI_NETDIR_OV_H_
#define _FI_NETDIR_OV_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "ndspi.h"

#include "rdma/fabric.h"
#include "ofi_mem.h"

#include "netdir.h"
#include "netdir_buf.h"
#include "netdir_log.h"
#include "netdir_iface.h"
#include "netdir_queue.h"

#include "rdma/fi_eq.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum ofi_nd_cq_state {
	NORMAL_STATE		= 0,
	LARGE_MSG_RECV_REQ	= 1,
	LARGE_MSG_WAIT_ACK	= 2,
	MAX_STATE		= 3
} ofi_nd_cq_state;

typedef enum ofi_nd_cq_event {
	NORMAL_EVENT		= 0,
	LARGE_MSG_REQ		= 1,
	LARGE_MSG_ACK		= 2,
	MAX_EVENT		= 3
} ofi_nd_cq_event;

typedef struct nd_eq_event {
	OVERLAPPED		ov;
	int			is_custom;
	uint32_t		eq_event;
	union {
		struct fi_eq_entry	operation;
		/* fi_eq_cm_entry could not be used here because it has
		   incomplete size */
		/*struct fi_eq_cm_entry	connection;*/
		struct fi_eq_err_entry	error;
	};

	/* connection data */
	void			*data;
	size_t			len;
} nd_eq_event;

typedef struct nd_send_entry nd_send_entry;

typedef struct nd_cq_entry {
	nd_event_base		base;
	struct nd_domain	*domain;
	struct nd_msgprefix	*prefix;
	struct nd_inlinebuf	*inline_buf;
	struct nd_notifybuf	*notify_buf;
	struct iovec		iov[ND_MSG_IOV_LIMIT];
	size_t			iov_cnt;

	/* used for RMA operations */
	size_t			mr_count;
	IND2MemoryRegion	*mr[ND_MSG_IOV_LIMIT];
	ND2_RESULT		result;

	uint64_t		flags;
	uint64_t		seq;
	void*			buf;
	size_t			len;
	uint64_t		data;
	struct nd_queue_item	queue_item;
	int			completed;
	void*			context;

	struct {
		struct nd_msg_location	*locations;
		/* != 0 only in case of large message
		 * receiving via RMA read */
		size_t			count;
	} rma_location;
	struct {
		/* these parameters are specified in
		 * parent's CQ entry to wait until all
		 * read/write operation will be completed */
		size_t comp_count;
		size_t total_count;

		CRITICAL_SECTION comp_lock;
	} wait_completion;
	struct nd_cq_entry	*aux_entry;

	ofi_nd_cq_state		state;
	ofi_nd_cq_event		event;
	nd_flow_cntrl_flags	flow_cntrl_flags;
	nd_send_entry		*send_entry;
} nd_cq_entry;

typedef struct nd_sge {
	ND2_SGE	entries[256];
	ULONG	count;
} nd_sge;

struct nd_send_entry {
	struct nd_queue_item	queue_item;
	nd_sge			*sge;
	nd_cq_entry		*cq_entry;
	nd_cq_entry		*prepost_entry;
	struct nd_ep		*ep;
};

typedef struct nd_buf_fini {
	volatile struct nd_buf_fini *next;
	void(*fini)(void);
} nd_buf_fini;

#define ND_FI_CONTEXT(ptr) ((struct fi_context*)(ptr))->internal[0]

OFI_ND_NB_BUF(nd_buf_fini);
OFI_ND_NB_BUF(nd_eq_event);
OFI_ND_NB_BUF(nd_cq_entry);
OFI_ND_NB_BUF(nd_sge);
OFI_ND_NB_BUF(nd_send_entry);

extern LONG nd_async_progress;
extern volatile nd_buf_fini *nd_buf_fini_head;

static inline void nd_buf_register_fini(void(*fini)(void))
{
	assert(fini);
	nd_buf_fini *fin = ND_BUF_ALLOC(nd_buf_fini);
	if (fin) {
		fin->fini = fini;
		do {
			fin->next = nd_buf_fini_head;
		} while (InterlockedCompareExchangePointer(
			 (PVOID *)&nd_buf_fini_head,
			 fin, (PVOID)fin->next) != fin->next);
	}
	else {
		ND_LOG_WARN(FI_LOG_CORE, "failed to allocate finalizer\n");
	}
}

static inline void nd_buf_fini_apply()
{
	volatile nd_buf_fini *next = nd_buf_fini_head;
	while (next) {
		volatile nd_buf_fini *current = next;
		next = current->next;
		assert(current->fini);
		current->fini();
		ND_BUF_FREE(nd_buf_fini, ((nd_buf_fini*)current));
	}
}

#define ND_REGISTER_FINI(fini)					\
do {								\
	static LONG init_done = 0;				\
	if (!init_done) {					\
		if (!InterlockedExchange(&init_done, 1))	\
			nd_buf_register_fini(fini);		\
	}							\
} while (0)

OFI_ND_NB_BUF(nd_event_base);

static inline void ofi_nd_eq_free_event(nd_eq_event *ev)
{
	assert(ev);

	if (ev->data)
		free(ev->data);
	if (ev->eq_event == FI_CONNREQ) {
		struct fi_eq_cm_entry *cm = (struct fi_eq_cm_entry*)&ev->operation;
		if (cm->info)
			fi_freeinfo(cm->info);
	}

	ND_BUF_FREE(nd_eq_event, ev);
}

void CALLBACK domain_io_cb(DWORD err, DWORD bytes, LPOVERLAPPED ov);

static inline void ofi_nd_eq_push(struct nd_eq *eq, struct nd_eq_event *ev)
{
	assert(eq);
	assert(ev);

	assert(eq->iocp);
	PostQueuedCompletionStatus(eq->iocp, 0, 0, &ev->ov);
	InterlockedIncrement(&eq->count);
	WakeByAddressAll((void*)&eq->count);
}

static inline void ofi_nd_eq_push_err(struct nd_eq *eq, struct nd_eq_event *ev)
{
	assert(eq);
	assert(ev);

	assert(eq->err);
	PostQueuedCompletionStatus(eq->err, 0, 0, &ev->ov);
	InterlockedIncrement(&eq->count);
	WakeByAddressAll((void*)&eq->count);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_OV_H_ */

