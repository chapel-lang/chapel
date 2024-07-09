/*
 * Copyright (c) 2018-2023 Intel Corporation. All rights reserved.
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
#include "ofi_hook.h"
#include "ofi_prov.h"
#include "ofi_iov.h"
#include <config.h>

#include <rdma/fi_profile.h>
struct hook_trace_ep {
	struct hook_ep hook_ep;
	struct fid_profile *prof_fid;
	int var_count;
	struct fi_profile_desc *varlist;
	int event_count;
	struct fi_profile_desc *eventlist;
};

#define HOOK_TRACE_EP2_PROV(ep)   ((ep)->hook_ep.domain->fabric->hprov)

#define TRACE_PROF_VAR(prov, desc)		  \
	FI_TRACE((prov), FI_LOG_EP_CTRL,		\
		"\t[%u, %s, size: %zu, type: %d(%d)]\n",   \
		(desc)->id, (desc)->name, (desc)->size,     \
		((desc)->datatype_sel == fi_primitive_type) ? \
			(desc)->datatype.primitive : (desc)->datatype.defined, \
	(desc)->datatype_sel)

#define TRACE_PROF_EVENT(prov, event)     \
	FI_TRACE((prov), FI_LOG_EP_CTRL, "\t[%u, %s, %s]\n", \
		(event)->id, (event)->name, (event)->desc)

#ifdef HAVE_FABRIC_PROFILE

static struct fi_profile_desc *
get_var_desc(struct hook_trace_ep *ep, int var_id)
{
	int i;

	for (i = 0; i < ep->var_count; i++) {
		if (ep->varlist[i].id == var_id)
			return (strlen(ep->varlist[i].name)) ?
			       &(ep->varlist[i]) : NULL;
	}
	return NULL;
}

/* sample callback
 */
static int
trace_event_unexp_msg(struct fid_profile *prof_fid,
		      struct fi_profile_desc *event,
		      void *param, size_t size, void *context)
{
	int ret;
	int var_id = -1;
	uint64_t data;
	struct fi_profile_desc *desc;

	struct hook_trace_ep *myep = (struct hook_trace_ep *)context;

	switch (event->id) {
	case FI_EVENT_UNEXP_MSG_MATCHED:
	case FI_EVENT_UNEXP_MSG_RECVD:
		var_id = FI_VAR_UNEXP_MSG_CNT;
		break;
	default:
		break;
	}
	if (var_id == -1) {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"unknown event %s.\n", event->name);
		return -FI_EINVAL;
	}

	desc = get_var_desc(myep, var_id);
	if (!desc) {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"variable %d is not supported\n", var_id);
		return -FI_EINVAL;
	}

	ret = fi_profile_read_u64(prof_fid, var_id, &data);
	if (!ret) {
		FI_TRACE(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			 "prof event %d, data [%s, %lu]\n",
			 event->id, desc->name, data);
	} else {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"failed on getting data for variable %s, ret %d\n",
			 desc->name, ret);
	}

	return ret;
}

/*
 * init for using fabric profiling interface
 */
static void hook_trace_prof_init(void *context)
{
	int ret;
	size_t count = 0;
	struct hook_trace_ep *myep = (struct hook_trace_ep *)context;
	struct fid *hep_fid;
	uint64_t flags = 0;
	struct fid_profile *prof_fid;
	struct fi_profile_desc *desc;
	struct fi_profile_desc *event;
	size_t size;
	int i;

	hep_fid = &(myep->hook_ep.hep->fid);
	ret = fi_profile_open(hep_fid, flags, &prof_fid, myep);

	if (ret)  {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"Failed on open profile object, %d\n", ret);
		return;
	}

	myep->prof_fid = prof_fid;

	size = fi_profile_query_vars(prof_fid, NULL, &count);
	myep->varlist = calloc(count, sizeof(struct fi_profile_desc));
	if (!myep->varlist) {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"Unable to allocate memory\n");
		return;
	}
	size  = fi_profile_query_vars(prof_fid, myep->varlist, &count);
	myep->var_count = size;

	count = 0;
	size = fi_profile_query_events(prof_fid, NULL, &count);
	myep->eventlist = calloc(count, sizeof(struct fi_profile_desc));
	if (!myep->eventlist) {
		FI_WARN(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
			"Unable to allocate memory\n");
		return;
	}
	size = fi_profile_query_events(prof_fid, myep->eventlist, &count);
	myep->event_count = size;

	ret = fi_profile_register_callback(prof_fid, FI_EVENT_UNEXP_MSG_RECVD,
					   trace_event_unexp_msg, myep);
	ret = fi_profile_register_callback(prof_fid, FI_EVENT_UNEXP_MSG_MATCHED,
					   trace_event_unexp_msg, myep);

	FI_TRACE(HOOK_TRACE_EP2_PROV(myep), FI_LOG_EP_CTRL,
		 "profile support: vars %d, events %d\n",
		  myep->var_count, myep->event_count);

	for (i = 0; i < myep->var_count; i++) {
		desc = &(myep->varlist[i]);
		TRACE_PROF_VAR(HOOK_TRACE_EP2_PROV(myep), desc);
	}
	for (i = 0; i < myep->event_count; i++) {
		event = &(myep->eventlist[i]);
		TRACE_PROF_EVENT(HOOK_TRACE_EP2_PROV(myep), event);
	}
}

#else

#define hook_trace_prof_init(context)    do {} while (0)

#endif

#define TRACE_BUF_SIZE	1024

#define IOV_BASE(iov, count)	(count ? iov[0].iov_base : NULL)
#define IOV_LEN(iov, count)	    ofi_total_iov_len(iov, count)
#define MSG_DATA(data, flags)   (flags & FI_REMOTE_CQ_DATA ? data : 0)

#define TRACE_CM(ret, fabric, ep, flags)  \
	if (!(ret)) { \
		if ((flags)) \
			FI_TRACE((fabric)->hprov, FI_LOG_EP_CTRL, \
				 "ep/pep %p flags 0x%lx\n", \
				 (void *)(ep), (uint64_t)(flags)); \
		else \
			FI_TRACE((fabric)->hprov, FI_LOG_EP_CTRL, \
				 "ep/pep %p\n", (void *)(ep)); \
	}

#define TRACE_CM_ADDR(ret, fabric, addr)  \
	if (!(ret)) { \
		ofi_straddr_log(fabric->hprov, FI_LOG_TRACE, FI_LOG_EP_CTRL, \
				"addr", addr);	\
	}

#define TRACE_EP_MSG(ret, ep, buf, len, addr, data, flags, context) \
	if (!(ret)) { \
		FI_TRACE((ep)->domain->fabric->hprov, FI_LOG_EP_DATA, \
			"buf %p len %zu addr %zu data %lu " \
			"flags 0x%zx ctx %p\n", \
			buf, len, addr, (uint64_t)data, \
			(uint64_t)flags, context); \
	}

#define TRACE_EP_RMA(ret, ep, buf, len, addr, raddr, data, flags, key, context) \
	if (!(ret)) { \
		FI_TRACE((ep)->domain->fabric->hprov, FI_LOG_EP_DATA, \
			"buf %p len %zu addr %zu raddr %lu data %lu " \
			"flags 0x%zx key 0x%zx ctx %p\n", \
			buf, len, addr, (uint64_t)raddr, (uint64_t)data, \
			(uint64_t)flags, (uint64_t)key, context); \
	}

#define TRACE_EP_TAGGED(ret, ep, buf, len, addr, data, flags, tag, ignore, context) \
	if (!(ret)) { \
		FI_TRACE((ep)->domain->fabric->hprov, FI_LOG_EP_DATA, \
			"buf %p len %zu addr %zu data %lu " \
			"flags 0x%zx tag 0x%lx ignore 0x%zx ctx %p\n", \
			buf, len, addr, (uint64_t)data, (uint64_t)flags, \
			(uint64_t)tag, (uint64_t)ignore, context); \
	}

#define TRACE_MR_ATTR(ret, buf, size, dom, mr, len, flags, attr)    \
	if (!(ret)) { \
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN, \
		     "mr %p len %lu flags 0x%lx\n%s",  mr, (len), (flags), \
		      fi_tostr_r(buf, size, (void *)attr, FI_TYPE_MR_ATTR)); \
	}

#define TRACE_ENDPOINT(buf, len, dom, name, ep, context, info)  \
	do {	 \
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN,   \
		    "%s %p context %p\n", name, ep, context);  \
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN, "\n%s",  \
		    fi_tostr_r(buf, len, info->ep_attr, FI_TYPE_EP_ATTR));  \
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN, "\n%s",  \
		    fi_tostr_r(buf,len, info->tx_attr, FI_TYPE_TX_ATTR));   \
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN, "\n%s",  \
		    fi_tostr_r(buf,len, info->rx_attr, FI_TYPE_RX_ATTR));  \
	} while (0);

typedef void (*trace_cq_entry_fn)(const struct fi_provider *prov,
				const char *func, int line,
				int count, void *buf, uint64_t data);

static void
trace_cq_unknown(const struct fi_provider *prov, const char *func,
		 int line, int count, void *buf, uint64_t data)
{
	// do nothing
}

static void
trace_cq_context_entry(const struct fi_provider *prov, const char *func,
		       int line, int count, void *buf, uint64_t data)
{
	int i;
	struct fi_cq_entry *entry = (struct fi_cq_entry *)buf;

	for (i = 0; i < count; i++) {
		fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
		       "ctx %p\n", entry[i].op_context);
	}
}

static void
trace_cq_msg_entry(const struct fi_provider *prov, const char *func,
		   int line, int count, void *buf, uint64_t data)
{
	int i;
	struct fi_cq_msg_entry *entry = (struct fi_cq_msg_entry *)buf;

	for (i = 0; i < count; i++) {
		if (entry[i].flags & FI_RECV) {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx len %zu\n",
			       entry[i].op_context, entry[i].flags,  entry[i].len);
		} else {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx\n",
			       entry[i].op_context, entry[i].flags);
		}
	}
}

static void
trace_cq_data_entry(const struct fi_provider *prov, const char *func,
		    int line, int count, void *buf, uint64_t data)
{
	int i;
	struct fi_cq_data_entry *entry = (struct fi_cq_data_entry *)buf;

	for (i = 0; i < count; i++) {
		if (entry[i].flags & FI_RECV) {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx len %zu buf %p, data %lu\n",
			       entry[i].op_context, entry[i].flags,
			       entry[i].len, entry[i].buf,
			       (entry[i].flags & FI_REMOTE_CQ_DATA) ? entry[i].data : 0);
		} else {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx\n",
			       entry[i].op_context, entry[i].flags);
		}
	}
}

static void
trace_cq_tagged_entry(const struct fi_provider *prov, const char *func,
		      int line, int count, void *buf, uint64_t data)
{
	int i;
	struct fi_cq_tagged_entry *entry = (struct fi_cq_tagged_entry *)buf;

	for (i = 0; i < count; i++) {
		if (entry[i].flags & FI_RECV) {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx len %zu buf %p, data %lu tag 0x%lx\n",
			       entry[i].op_context, entry[i].flags,
			       entry[i].len, entry[i].buf,
			       (entry[i].flags & FI_REMOTE_CQ_DATA) ? entry[i].data : 0,
			       entry[i].tag);
		} else {
			fi_log(prov, FI_LOG_TRACE, FI_LOG_CQ, func, line,
			       "ctx %p flags 0x%lx\n",
			       entry[i].op_context, entry[i].flags);
		}
	}
}

static trace_cq_entry_fn trace_cq_entry[] = {
	trace_cq_unknown,
	trace_cq_context_entry,
	trace_cq_msg_entry,
	trace_cq_data_entry,
	trace_cq_tagged_entry
};

static inline void
trace_cq(struct hook_cq *cq, const char *func, int line,
	 int count, void *buf, uint64_t data)
{
	if ((count > 0) &&
	    fi_log_enabled(cq->domain->fabric->hprov, FI_LOG_TRACE, FI_LOG_CQ)) {
		trace_cq_entry[cq->format](cq->domain->fabric->hprov, func,
					   line, count, buf, data);
	}
}

static inline void
trace_cq_err(struct hook_cq *cq, const char *func, int line,
	     struct fi_cq_err_entry *entry,  uint64_t flags)
{
	char err_buf[80];

	if (!fi_log_enabled(cq->domain->fabric->hprov, FI_LOG_TRACE, FI_LOG_CQ))
		return;

	fi_cq_strerror(cq->hcq, entry->prov_errno, entry->err_data, err_buf, 80);
	if (entry->flags & FI_RECV) {
		fi_log(cq->domain->fabric->hprov, FI_LOG_TRACE, FI_LOG_CQ,
		       func, line,
		       "ctx %p flags 0x%lx, len %zu buf %p data %lu tag 0x%lx "
		       "olen %zu err %d (%s) prov_errno %d (%s)\n",
		       entry->op_context, entry->flags, entry->len, entry->buf,
		       entry->data, entry->tag, entry->olen,  
		       entry->err, fi_strerror(entry->err),
		       entry->prov_errno, err_buf);
	} else {
		fi_log(cq->domain->fabric->hprov, FI_LOG_TRACE, FI_LOG_CQ,
		       func, line,
		       "ctx %p flags 0x%lx, data %lu tag 0x%lx "
		       "olen %zu err %d (%s) prov_errno %d (%s)\n",
		       entry->op_context, entry->flags,
		       entry->data, entry->tag, entry->olen,
		       entry->err, fi_strerror(entry->err),
		       entry->prov_errno, err_buf);
	}
}



static ssize_t
trace_atomic_write(struct fid_ep *ep,
		   const void *buf, size_t count, void *desc,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomic(myep->hep, buf, count, desc, dest_addr,
			addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_writev(struct fid_ep *ep,
		    const struct fi_ioc *iov, void **desc, size_t count,
		    fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		    enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicv(myep->hep, iov, desc, count, dest_addr,
			 addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_writemsg(struct fid_ep *ep,
		      const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicmsg(myep->hep, msg, flags);
	return ret;
}

static ssize_t
trace_atomic_inject(struct fid_ep *ep, const void *buf, size_t count,
		    fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		    enum fi_datatype datatype, enum fi_op op)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_atomic(myep->hep, buf, count, dest_addr,
			       addr, key, datatype, op);
	return ret;
}

static ssize_t
trace_atomic_readwrite(struct fid_ep *ep,
		       const void *buf, size_t count, void *desc,
		       void *result, void *result_desc,
		       fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		       enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomic(myep->hep, buf, count, desc,
			      result, result_desc, dest_addr,
			      addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_readwritev(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, fi_addr_t dest_addr,
			uint64_t addr, uint64_t key, enum fi_datatype datatype,
			enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicv(myep->hep, iov, desc, count,
			       resultv, result_desc, result_count,
			       dest_addr, addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_readwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			  struct fi_ioc *resultv, void **result_desc,
			  size_t result_count, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicmsg(myep->hep, msg, resultv, result_desc,
				 result_count, flags);
	return ret;
}

static ssize_t
trace_atomic_compwrite(struct fid_ep *ep,
		       const void *buf, size_t count, void *desc,
		       const void *compare, void *compare_desc,
		       void *result, void *result_desc,
		       fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		       enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomic(myep->hep, buf, count, desc,
				compare, compare_desc, result, result_desc,
				dest_addr, addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_compwritev(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicv(myep->hep, iov, desc, count,
				 comparev, compare_desc, compare_count,
				 resultv, result_desc, result_count, dest_addr,
				 addr, key, datatype, op, context);
	return ret;
}

static ssize_t
trace_atomic_compwritemsg(struct fid_ep *ep,
			  const struct fi_msg_atomic *msg,
			  const struct fi_ioc *comparev, void **compare_desc,
			  size_t compare_count, struct fi_ioc *resultv,
			  void **result_desc, size_t result_count,
			  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicmsg(myep->hep, msg,
				   comparev, compare_desc, compare_count,
				   resultv, result_desc, result_count, flags);
	return ret;
}

static int
trace_atomic_writevalid(struct fid_ep *ep, enum fi_datatype datatype,
			enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

static int
trace_atomic_readwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			    enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

static int
trace_atomic_compwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			    enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

struct fi_ops_atomic trace_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = trace_atomic_write,
	.writev = trace_atomic_writev,
	.writemsg = trace_atomic_writemsg,
	.inject = trace_atomic_inject,
	.readwrite = trace_atomic_readwrite,
	.readwritev = trace_atomic_readwritev,
	.readwritemsg = trace_atomic_readwritemsg,
	.compwrite = trace_atomic_compwrite,
	.compwritev = trace_atomic_compwritev,
	.compwritemsg = trace_atomic_compwritemsg,
	.writevalid = trace_atomic_writevalid,
	.readwritevalid = trace_atomic_readwritevalid,
	.compwritevalid = trace_atomic_compwritevalid,
};


static ssize_t
trace_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
	   fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recv(myep->hep, buf, len, desc, src_addr, context);
	TRACE_EP_MSG(ret, myep, buf, len, src_addr, 0, 0, context);

	return ret;
}

static ssize_t
trace_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	    size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvv(myep->hep, iov, desc, count, src_addr, context);
	TRACE_EP_MSG(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
		     src_addr, 0, 0, context);

	return ret;
}

static ssize_t
trace_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvmsg(myep->hep, msg, flags);
	TRACE_EP_MSG(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
		     IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
		     flags & FI_REMOTE_CQ_DATA ? msg->data : 0,
		     flags, msg->context);

	return ret;
}

static ssize_t
trace_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	   fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_send(myep->hep, buf, len, desc, dest_addr, context);
	TRACE_EP_MSG(ret, myep, buf, len, dest_addr, 0, 0, context);

	return ret;
}

static ssize_t
trace_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	    size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_sendv(myep->hep, iov, desc, count, dest_addr, context);
	TRACE_EP_MSG(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
		     dest_addr, 0, 0, context);

	return ret;
}

static ssize_t
trace_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_sendmsg(myep->hep, msg, flags);
	TRACE_EP_MSG(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
		     IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
		     MSG_DATA(msg->data, flags), flags, msg->context);

	return ret;
}

static ssize_t
trace_inject(struct fid_ep *ep, const void *buf, size_t len,
	     fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject(myep->hep, buf, len, dest_addr);
	TRACE_EP_MSG(ret, myep, buf, len, dest_addr, 0, 0, NULL);

	return ret;
}

static ssize_t
trace_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	       uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_senddata(myep->hep, buf, len, desc, data, dest_addr, context);
	TRACE_EP_MSG(ret, myep, buf, len, dest_addr, data, 0, context);

	return ret;
}

static ssize_t
trace_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		 uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_injectdata(myep->hep, buf, len, data, dest_addr);
	TRACE_EP_MSG(ret, myep, buf, len, dest_addr, data, 0,  NULL);

	return ret;
}

static struct fi_ops_msg trace_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = trace_recv,
	.recvv = trace_recvv,
	.recvmsg = trace_recvmsg,
	.send = trace_send,
	.sendv = trace_sendv,
	.sendmsg = trace_sendmsg,
	.inject = trace_inject,
	.senddata = trace_senddata,
	.injectdata = trace_injectdata,
};


static ssize_t
trace_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	   fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_read(myep->hep, buf, len, desc, src_addr, addr, key, context);
	TRACE_EP_RMA(ret, myep, buf, len, src_addr, addr, 0, 0, key, context);

	return ret;
}

static ssize_t
trace_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	    size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	    void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_readv(myep->hep, iov, desc, count, src_addr,
		       addr, key, context);
	TRACE_EP_RMA(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
		     src_addr, addr, 0, 0, key, context);

	return ret;
}

static ssize_t
trace_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_readmsg(myep->hep, msg, flags);
	TRACE_EP_RMA(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
		     IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
		     msg->rma_iov_count ? msg->rma_iov[0].addr : 0,
		     MSG_DATA(msg->data, flags), flags,
		     msg->rma_iov_count ? msg->rma_iov[0].key : 0,
		     msg->context);

	return ret;
}

static ssize_t
trace_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	    fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_write(myep->hep, buf, len, desc, dest_addr,
		       addr, key, context);
	TRACE_EP_RMA(ret, myep, buf, len, dest_addr, addr, 0, 0, key, context);

	return ret;
}

static ssize_t
trace_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
	     size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
	     void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_writev(myep->hep, iov, desc, count, dest_addr,
			addr, key, context);
	TRACE_EP_RMA(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
		     dest_addr, addr, 0, 0, key, context);

	return ret;
}

static ssize_t
trace_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_writemsg(myep->hep, msg, flags);
	TRACE_EP_RMA(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
		     IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
		     msg->rma_iov_count ? msg->rma_iov[0].addr : 0,
		     MSG_DATA(msg->data, flags), flags,
		     msg->rma_iov_count ? msg->rma_iov[0].key : 0,
		     msg->context);

	return ret;
}

static ssize_t
trace_inject_write(struct fid_ep *ep, const void *buf, size_t len,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_write(myep->hep, buf, len, dest_addr, addr, key);
	TRACE_EP_RMA(ret, myep, buf, len, dest_addr, addr, 0, 0, key, NULL);

	return ret;
}

static ssize_t
trace_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_writedata(myep->hep, buf, len, desc, data,
			   dest_addr, addr, key, context);
	TRACE_EP_RMA(ret, myep, buf, len, dest_addr, addr, data, 0, key, context);

	return ret;
}

static ssize_t
trace_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		       uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_writedata(myep->hep, buf, len, data, dest_addr,
				  addr, key);
	TRACE_EP_RMA(ret, myep, buf, len, dest_addr, addr, data, 0, key, NULL);

	return ret;
}

static struct fi_ops_rma trace_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = trace_read,
	.readv = trace_readv,
	.readmsg = trace_readmsg,
	.write = trace_write,
	.writev = trace_writev,
	.writemsg = trace_writemsg,
	.inject = trace_inject_write,
	.writedata = trace_writedata,
	.injectdata = trace_inject_writedata,
};


static ssize_t
trace_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
	    fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
	    void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecv(myep->hep, buf, len, desc, src_addr,
		       tag, ignore, context);
	TRACE_EP_TAGGED(ret, myep, buf, len, src_addr, 0, 0, tag, ignore, context);

	return ret;
}

static ssize_t
trace_trecvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	     size_t count, fi_addr_t src_addr, uint64_t tag,
	     uint64_t ignore, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvv(myep->hep, iov, desc, count, src_addr,
			tag, ignore, context);
	TRACE_EP_TAGGED(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
			src_addr, 0, 0, tag, ignore, context);

	return ret;
}

static ssize_t
trace_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
	       uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvmsg(myep->hep, msg, flags);
	TRACE_EP_TAGGED(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
			IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
			MSG_DATA(msg->data, flags), flags,
			msg->tag, msg->ignore, msg->context);

	return ret;
}

static ssize_t
trace_tsend(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	    fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsend(myep->hep, buf, len, desc, dest_addr, tag, context);
	TRACE_EP_TAGGED(ret, myep, buf, len, dest_addr, 0, 0, tag, 0, context);

	return ret;
}

static ssize_t
trace_tsendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	     size_t count, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsendv(myep->hep, iov, desc, count, dest_addr, tag, context);
	TRACE_EP_TAGGED(ret, myep, IOV_BASE(iov, count), IOV_LEN(iov, count),
			dest_addr, 0, 0, tag, 0, context);

	return ret;
}

static ssize_t
trace_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
	       uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsendmsg(myep->hep, msg, flags);
	TRACE_EP_TAGGED(ret, myep, IOV_BASE(msg->msg_iov, msg->iov_count),
			IOV_LEN(msg->msg_iov, msg->iov_count), msg->addr,
			MSG_DATA(msg->data, flags), flags,
			msg->tag, 0, msg->context);

	return ret;
}

static ssize_t
trace_tinject(struct fid_ep *ep, const void *buf, size_t len,
	      fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinject(myep->hep, buf, len, dest_addr, tag);
	TRACE_EP_TAGGED(ret, myep, buf, len, dest_addr, 0, 0, tag, 0, NULL);

	return ret;
}

static ssize_t
trace_tsenddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		uint64_t data, fi_addr_t dest_addr, uint64_t tag,
		void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsenddata(myep->hep, buf, len, desc, data,
			   dest_addr, tag, context);
	TRACE_EP_TAGGED(ret, myep, buf, len, dest_addr, data, 0, tag, 0, context);

	return ret;
}

static ssize_t
trace_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
		  uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinjectdata(myep->hep, buf, len, data, dest_addr, tag);
	TRACE_EP_TAGGED(ret, myep, buf, len, dest_addr, data, 0, tag, 0, NULL);

	return ret;
}

static struct fi_ops_tagged trace_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = trace_trecv,
	.recvv = trace_trecvv,
	.recvmsg = trace_trecvmsg,
	.send = trace_tsend,
	.sendv = trace_tsendv,
	.sendmsg = trace_tsendmsg,
	.inject = trace_tinject,
	.senddata = trace_tsenddata,
	.injectdata = trace_tinjectdata,
};


static int trace_setname(fid_t fid, void *addr, size_t addrlen)
{
	int ret = 0;
	struct hook_fabric *myfabric = hook_to_fabric(fid);

	ret = fi_setname(hook_to_hfid(fid), addr, addrlen);
	TRACE_CM_ADDR(ret, myfabric, addr);

	return ret;
}

static int trace_getname(fid_t fid, void *addr, size_t *addrlen)
{
	int ret = 0;
	struct hook_fabric *myfabric = hook_to_fabric(fid);

	ret = fi_getname(hook_to_hfid(fid), addr, addrlen);
	TRACE_CM_ADDR(ret, myfabric, addr);

	return ret;
}

static int trace_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	int ret = 0;
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	ret = fi_getpeer(myep->hep, addr, addrlen);
	TRACE_CM_ADDR(ret, myep->domain->fabric, addr);

	return ret;
}

static int trace_connect(struct fid_ep *ep, const void *addr,
	    const void *param, size_t paramlen)
{
	int ret = 0;
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	ret = fi_connect(myep->hep, addr, param, paramlen);
	TRACE_CM_ADDR(ret, myep->domain->fabric, addr);

	return ret;
}

static int trace_listen(struct fid_pep *pep)
{
	int ret = 0;
	struct hook_pep *mypep = container_of(pep, struct hook_pep, pep);

	ret = fi_listen(mypep->hpep);
	TRACE_CM(ret, mypep->fabric, (void *)mypep->hpep, 0);

	return ret;
}

static int trace_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	int ret = 0;
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	ret = fi_accept(myep->hep, param, paramlen);
	TRACE_CM(ret, myep->domain->fabric, (void *)myep->hep, 0);

	return ret;
}

static int trace_reject(struct fid_pep *pep, fid_t handle,
			const void *param, size_t paramlen)
{
	int ret = 0;
	struct hook_pep *mypep = container_of(pep, struct hook_pep, pep);

	ret = fi_reject(mypep->hpep, handle, param, paramlen);
	TRACE_CM(ret, mypep->fabric, (void *)mypep->hpep, 0);

	return ret;
}

static int trace_shutdown(struct fid_ep *ep, uint64_t flags)
{
	int ret = 0;
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	ret = fi_shutdown(myep->hep, flags);
	TRACE_CM(ret, myep->domain->fabric, (void *)myep->hep, flags);

	return ret;
}

static int trace_join(struct fid_ep *ep, const void *addr, uint64_t flags,
		      struct fid_mc **mc, void *context)
{
	int ret = 0;
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	ret = fi_join(myep->hep, addr, flags, mc, context);
	TRACE_CM(ret, myep->domain->fabric, (void *)myep->hep, flags);

	return ret;
}

struct fi_ops_cm trace_cm_ops = {
    .size = sizeof(struct fi_ops_cm),
    .setname = trace_setname,
    .getname = trace_getname,
    .getpeer = trace_getpeer,
    .connect = trace_connect,
    .listen = trace_listen,
    .accept = trace_accept,
    .reject = trace_reject,
    .shutdown = trace_shutdown,
    .join = trace_join,
};

static int trace_ep_init(struct fid *fid)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);

	/* ep->atomic = &trace_atomic_ops; */
	ep->msg = &trace_msg_ops;
	ep->rma = &trace_rma_ops;
	ep->tagged = &trace_tagged_ops;
	ep->cm = &trace_cm_ops;
	return 0;
}

static int trace_pep_init(struct fid *fid)
{
	struct fid_pep *pep = container_of(fid, struct fid_pep, fid);

	pep->cm = &trace_cm_ops;
	return 0;
}

static ssize_t trace_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_read(mycq->hcq, buf, count);
	trace_cq(mycq, __func__, __LINE__, ret, buf, 0);
	return ret;
}

static ssize_t
trace_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readerr(mycq->hcq, buf, flags);
	if (ret > 0)
		trace_cq_err(mycq, __func__, __LINE__, buf, flags);
	return ret;
}

static ssize_t
trace_cq_readfrom(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readfrom(mycq->hcq, buf, count, src_addr);
	trace_cq(mycq, __func__, __LINE__, ret, buf, src_addr ? *src_addr : 0);
	return ret;
}

static ssize_t
trace_cq_sread(struct fid_cq *cq, void *buf, size_t count,
	      const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sread(mycq->hcq, buf, count, cond, timeout);
	trace_cq(mycq, __func__, __LINE__, ret, buf, 0);
	return ret;
}

static ssize_t
trace_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
		  fi_addr_t *src_addr, const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sreadfrom(mycq->hcq, buf, count, src_addr, cond, timeout);
	trace_cq(mycq, __func__, __LINE__, ret, buf, src_addr ? *src_addr : 0);
	return ret;
}

static int trace_cq_signal(struct fid_cq *cq)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	int ret;

	ret = fi_cq_signal(mycq->hcq);
	return ret;
}

struct fi_ops_cq trace_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = trace_cq_read,
	.readfrom = trace_cq_readfrom,
	.readerr = trace_cq_readerr,
	.sread = trace_cq_sread,
	.sreadfrom = trace_cq_sreadfrom,
	.signal = trace_cq_signal,
	.strerror = hook_cq_strerror,
};

/* av ops */
static int
trace_av_insert(struct fid_av *av, const void *addr, size_t count,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insert(myav->hav, addr, count, fi_addr, flags, context);
}

static int
trace_av_insertsvc(struct fid_av *av, const char *node,
		   const char *service, fi_addr_t *fi_addr, uint64_t flags,
		   void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insertsvc(myav->hav, node, service, fi_addr, flags,
			   context);
}

static int
trace_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		   const char *service, size_t svccnt, fi_addr_t *fi_addr,
		   uint64_t flags, void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insertsym(myav->hav, node, nodecnt, service, svccnt,
			       fi_addr, flags, context);
}

static int
trace_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
		uint64_t flags)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_remove(myav->hav, fi_addr, count, flags);
}

static int
trace_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
		size_t *addrlen)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_lookup(myav->hav, fi_addr, addr, addrlen);
}

static const char *
trace_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_straddr(myav->hav, addr, buf, len);
}

static struct fi_ops_av trace_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = trace_av_insert,
	.insertsvc = trace_av_insertsvc,
	.insertsym = trace_av_insertsym,
	.remove = trace_av_remove,
	.lookup = trace_av_lookup,
	.straddr = trace_av_straddr,
};

/* domain ops */
static int
trace_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
	      struct fid_av **av, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_av *myav;
	char buf[TRACE_BUF_SIZE];
	int ret;

	myav = calloc(1, sizeof *myav);
	if (!myav)
		return -FI_ENOMEM;

	myav->domain = dom;
	myav->av.fid.fclass = FI_CLASS_AV;
	myav->av.fid.context = context;
	myav->av.fid.ops = &hook_fid_ops;
	myav->av.ops = &trace_av_ops;

	ret = fi_av_open(dom->hdomain, attr, &myav->hav, &myav->av.fid);
	if (!ret) {
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN,
			"av %p, context %p\n%s",
			&myav->av, context,
			fi_tostr_r(buf, TRACE_BUF_SIZE, attr, FI_TYPE_AV_ATTR));
		*av = &myav->av;
	} else {
		free(myav);
	}

	return ret;
}

static int
trace_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	      struct fid_cq **cq, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct fi_cq_attr hattr = *attr;
	struct hook_cq *mycq;
	char buf[TRACE_BUF_SIZE];
	int ret = 0;

	mycq = calloc(1, sizeof *mycq);
	if (!mycq)
		return -FI_ENOMEM;

	mycq->domain = dom;
	mycq->cq.fid.fclass = FI_CLASS_CQ;
	mycq->cq.fid.context = context;
	mycq->cq.fid.ops = &hook_fid_ops;
	mycq->cq.ops = &trace_cq_ops;

	if (attr->wait_obj == FI_WAIT_SET)
		hattr.wait_set = hook_to_hwait(attr->wait_set);

	ret = fi_cq_open(dom->hdomain, &hattr, &mycq->hcq, &mycq->cq.fid);
	if (!ret) {
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN,
		    "cq %p domain %p context %p\n%s",
		    &mycq->hcq, dom->hdomain, context,
		    fi_tostr_r(buf, TRACE_BUF_SIZE, &hattr, FI_TYPE_CQ_ATTR));
		mycq->format = hattr.format;
		*cq = &mycq->cq;
	} else {
		free(mycq);
	}

	return ret;
}

static int
trace_endpoint(struct fid_domain *domain, struct fi_info *info,
	       struct fid_ep **ep, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_trace_ep *myep;
	char buf[TRACE_BUF_SIZE];
	int ret = 0;

	myep = calloc(1, sizeof *myep);
	if (!myep)
		return -FI_ENOMEM;

	ret = hook_endpoint_init(domain, info, ep, context, &(myep->hook_ep));

	if (!ret)  {
		TRACE_ENDPOINT(buf, TRACE_BUF_SIZE, dom,
			       "ep", (void *)*ep, context, info);
		trace_ep_init(&(*ep)->fid);
	} else {
		free(myep);
		return ret;
	}

	hook_trace_prof_init(myep);

	return 0;
}

static int
trace_scalable_ep(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **sep, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	char buf[TRACE_BUF_SIZE];
	int ret = 0;

	ret = hook_scalable_ep(domain, info, sep, context);
	if (!ret) {
		TRACE_ENDPOINT(buf, TRACE_BUF_SIZE, dom,
			       "sep", (void *)*sep, context, info);
		trace_ep_init(&(*sep)->fid);
	}

	return ret;
}

static int
trace_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct fi_cntr_attr hattr = *attr;
	struct hook_cntr *mycntr;
	char buf[TRACE_BUF_SIZE];
	int ret = 0;

	mycntr = calloc(1, sizeof *mycntr);
	if (!mycntr)
		return -FI_ENOMEM;

	mycntr->domain = dom;
	mycntr->cntr.fid.fclass = FI_CLASS_CNTR;
	mycntr->cntr.fid.context = context;
	mycntr->cntr.fid.ops = &hook_fid_ops;
	mycntr->cntr.ops = &hook_cntr_ops;

	if (attr->wait_obj == FI_WAIT_SET)
		hattr.wait_set = hook_to_hwait(attr->wait_set);

	ret = fi_cntr_open(dom->hdomain, &hattr, &mycntr->hcntr,
			   &mycntr->cntr.fid);
	if (!ret) {
		FI_TRACE(dom->fabric->hprov, FI_LOG_DOMAIN,
		     "cntr %p, domain %p context %p \n%s",
		      &mycntr->hcntr, dom->hdomain, context,
		      fi_tostr_r(buf, TRACE_BUF_SIZE, &hattr, FI_TYPE_CNTR_ATTR));
		*cntr = &mycntr->cntr;
	} else {
		free(mycntr);
	}

	return ret;
}

static struct fi_ops_domain trace_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = trace_av_open,
	.cq_open = trace_cq_open,
	.endpoint = trace_endpoint,
	.scalable_ep = trace_scalable_ep,
	.cntr_open = trace_cntr_open,
	.poll_open = hook_poll_open,
	.stx_ctx = hook_stx_ctx,
	.srx_ctx = hook_srx_ctx,
	.query_atomic = hook_query_atomic,
	.query_collective = hook_query_collective,
};

/* mr ops */
static int
trace_mr_reg(struct fid *fid, const void *buf, size_t len,
	       uint64_t access, uint64_t offset, uint64_t requested_key,
	       uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	char logbuf[TRACE_BUF_SIZE];
	int ret = 0;
	struct fi_mr_attr attr;
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	attr.mr_iov = &iov;
	attr.iov_count = 1;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;
	attr.iface = FI_HMEM_SYSTEM;

	ret = fi_mr_reg(dom->hdomain, buf, len, access, offset, requested_key,
			flags, mr, context);
	TRACE_MR_ATTR(ret, logbuf, TRACE_BUF_SIZE, dom, *mr, len, flags, &attr);

	return ret;
}

static int
trace_mr_regv(struct fid *fid, const struct iovec *iov,
	      size_t count, uint64_t access,
	      uint64_t offset, uint64_t requested_key,
	      uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	char logbuf[TRACE_BUF_SIZE];
	int ret = 0;
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;
	attr.iface = FI_HMEM_SYSTEM;

	ret = fi_mr_regv(dom->hdomain, iov, count, access, offset,
			 requested_key, flags, mr, context);
	TRACE_MR_ATTR(ret, logbuf, TRACE_BUF_SIZE, dom, *mr,
		      IOV_LEN(iov, count), flags, &attr);

	return ret;
}

static int
trace_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		 uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	char logbuf[TRACE_BUF_SIZE];
	int ret = 0;

	ret = fi_mr_regattr(dom->hdomain, attr, flags, mr);
	TRACE_MR_ATTR(ret, logbuf, TRACE_BUF_SIZE, dom,  *mr,
		      IOV_LEN(attr->mr_iov, attr->iov_count), flags, attr);

	return ret;
}

static struct fi_ops_mr trace_mr_ops = {
    .size = sizeof(struct fi_ops_mr),
    .reg = trace_mr_reg,
    .regv = trace_mr_regv,
    .regattr = trace_mr_regattr,
};


static int trace_domain_init(struct fid *fid)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);
	domain->ops = &trace_domain_ops;
	domain->mr = &trace_mr_ops;

	return 0;
}

static struct fi_ops trace_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

struct hook_prov_ctx hook_trace_ctx;

static int hook_trace_fabric(struct fi_fabric_attr *attr,
			     struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct hook_fabric *fab;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing trace hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	hook_fabric_init(fab, HOOK_TRACE, attr->fabric, hprov,
			 &trace_fabric_fid_ops, &hook_trace_ctx);
	*fabric = &fab->fabric;
	return 0;
}

struct hook_prov_ctx hook_trace_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_trace",
		.getinfo = NULL,
		.fabric = hook_trace_fabric,
		.cleanup = NULL,
	},
};

HOOK_TRACE_INI
{
	hook_trace_ctx.ini_fid[FI_CLASS_DOMAIN] = trace_domain_init;
	hook_trace_ctx.ini_fid[FI_CLASS_PEP] = trace_pep_init;

	return &hook_trace_ctx.prov;
}
