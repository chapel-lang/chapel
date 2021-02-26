/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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

#ifndef _FI_PSM2_TRIGGER_H
#define _FI_PSM2_TRIGGER_H

#ifdef __cplusplus
extern "C" {
#endif

enum psmx2_triggered_op {
	PSMX2_TRIGGERED_SEND,
	PSMX2_TRIGGERED_SENDV,
	PSMX2_TRIGGERED_RECV,
	PSMX2_TRIGGERED_TSEND,
	PSMX2_TRIGGERED_TSENDV,
	PSMX2_TRIGGERED_TRECV,
	PSMX2_TRIGGERED_WRITE,
	PSMX2_TRIGGERED_WRITEV,
	PSMX2_TRIGGERED_READ,
	PSMX2_TRIGGERED_READV,
	PSMX2_TRIGGERED_ATOMIC_WRITE,
	PSMX2_TRIGGERED_ATOMIC_WRITEV,
	PSMX2_TRIGGERED_ATOMIC_READWRITE,
	PSMX2_TRIGGERED_ATOMIC_READWRITEV,
	PSMX2_TRIGGERED_ATOMIC_COMPWRITE,
	PSMX2_TRIGGERED_ATOMIC_COMPWRITEV,
};

struct psmx2_trigger {
	enum psmx2_triggered_op	op;
	struct psmx2_fid_cntr	*cntr;
	size_t			threshold;
	union {
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} send;
		struct {
			struct fid_ep	*ep;
			const struct iovec *iov;
			size_t		count;
			void		**desc;
			fi_addr_t	dest_addr;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} sendv;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			void		*context;
			uint64_t	flags;
		} recv;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	tag;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} tsend;
		struct {
			struct fid_ep	*ep;
			const struct iovec *iov;
			size_t		count;
			void		**desc;
			fi_addr_t	dest_addr;
			uint64_t	tag;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} tsendv;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			uint64_t	tag;
			uint64_t	ignore;
			void		*context;
			uint64_t	flags;
		} trecv;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} write;
		struct {
			struct fid_ep	*ep;
			const struct iovec *iov;
			size_t		count;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} writev;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
		} read;
		struct {
			struct fid_ep	*ep;
			const struct iovec *iov;
			size_t		count;
			void		*desc;
			fi_addr_t	src_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
		} readv;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_write;
		struct {
			struct fid_ep	*ep;
			const struct fi_ioc *iov;
			size_t		count;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_writev;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			void		*result;
			void		*result_desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_readwrite;
		struct {
			struct fid_ep	*ep;
			const struct fi_ioc *iov;
			size_t		count;
			void		**desc;
			struct fi_ioc	*resultv;
			void		**result_desc;
			size_t		result_count;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_readwritev;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			const void	*compare;
			void		*compare_desc;
			void		*result;
			void		*result_desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_compwrite;
		struct {
			struct fid_ep	*ep;
			const struct fi_ioc *iov;
			size_t		count;
			void		**desc;
			const struct fi_ioc *comparev;
			void		**compare_desc;
			size_t		compare_count;
			struct fi_ioc	*resultv;
			void		**result_desc;
			size_t		result_count;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_compwritev;
	};
	struct psmx2_trigger *next;	/* used for randomly accessed trigger list */
	struct slist_entry list_entry;	/* used for ready-to-fire trigger queue */
};

ssize_t psmx2_send_generic(
			struct fid_ep *ep,
			const void *buf, size_t len,
			void *desc, fi_addr_t dest_addr,
			void *context, uint64_t flags,
			uint64_t data);

ssize_t psmx2_sendv_generic(
			struct fid_ep *ep,
			const struct iovec *iov, void **desc,
			size_t count, fi_addr_t dest_addr,
			void *context, uint64_t flags,
			uint64_t data);

ssize_t psmx2_recv_generic(
			struct fid_ep *ep,
			void *buf, size_t len, void *desc,
			fi_addr_t src_addr, void *context,
			uint64_t flags);

ssize_t psmx2_tagged_send_generic(
			struct fid_ep *ep,
			const void *buf, size_t len,
			void *desc, fi_addr_t dest_addr,
			uint64_t tag, void *context,
			uint64_t flags, uint64_t data);

ssize_t psmx2_tagged_sendv_generic(
			struct fid_ep *ep,
			const struct iovec *iov, void **desc,
			size_t count, fi_addr_t dest_addr,
			uint64_t tag, void *context,
			uint64_t flags, uint64_t data);

ssize_t psmx2_tagged_recv_generic(
			struct fid_ep *ep,
			void *buf, size_t len,
			void *desc, fi_addr_t src_addr,
			uint64_t tag, uint64_t ignore,
			void *context, uint64_t flags);

ssize_t psmx2_write_generic(
			struct fid_ep *ep,
			const void *buf, size_t len,
			void *desc, fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			void *context, uint64_t flags,
			uint64_t data);

ssize_t psmx2_writev_generic(
			struct fid_ep *ep,
			const struct iovec *iov, void **desc,
			size_t count, fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			void *context, uint64_t flags,
			uint64_t data);

ssize_t psmx2_read_generic(
			struct fid_ep *ep,
			void *buf, size_t len,
			void *desc, fi_addr_t src_addr,
			uint64_t addr, uint64_t key,
			void *context, uint64_t flags);

ssize_t psmx2_readv_generic(
			struct fid_ep *ep,
			const struct iovec *iov, void *desc,
			size_t count, fi_addr_t src_addr,
			uint64_t addr, uint64_t key,
			void *context, uint64_t flags);

ssize_t psmx2_atomic_write_generic(
			struct fid_ep *ep,
			const void *buf,
			size_t count, void *desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context,
			uint64_t flags);

ssize_t psmx2_atomic_readwrite_generic(
			struct fid_ep *ep,
			const void *buf,
			size_t count, void *desc,
			void *result, void *result_desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context,
			uint64_t flags);

ssize_t psmx2_atomic_compwrite_generic(
			struct fid_ep *ep,
			const void *buf,
			size_t count, void *desc,
			const void *compare, void *compare_desc,
			void *result, void *result_desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context,
			uint64_t flags);

static inline
int psmx2_process_trigger(struct psmx2_trx_ctxt *trx_ctxt,
			  struct psmx2_trigger *trigger)
{
	switch (trigger->op) {
	case PSMX2_TRIGGERED_SEND:
		psmx2_send_generic(trigger->send.ep,
				   trigger->send.buf,
				   trigger->send.len,
				   trigger->send.desc,
				   trigger->send.dest_addr,
				   trigger->send.context,
				   trigger->send.flags,
				   trigger->send.data);
		break;
	case PSMX2_TRIGGERED_SENDV:
		psmx2_sendv_generic(trigger->sendv.ep,
				    trigger->sendv.iov,
				    trigger->sendv.desc,
				    trigger->sendv.count,
				    trigger->sendv.dest_addr,
				    trigger->sendv.context,
				    trigger->sendv.flags,
				    trigger->sendv.data);
		break;
	case PSMX2_TRIGGERED_RECV:
		psmx2_recv_generic(trigger->recv.ep,
				   trigger->recv.buf,
				   trigger->recv.len,
				   trigger->recv.desc,
				   trigger->recv.src_addr,
				   trigger->recv.context,
				   trigger->recv.flags);
		break;
	case PSMX2_TRIGGERED_TSEND:
		psmx2_tagged_send_generic(trigger->tsend.ep,
					  trigger->tsend.buf,
					  trigger->tsend.len,
					  trigger->tsend.desc,
					  trigger->tsend.dest_addr,
					  trigger->tsend.tag,
					  trigger->tsend.context,
					  trigger->tsend.flags,
					  trigger->tsend.data);
		break;
	case PSMX2_TRIGGERED_TSENDV:
		psmx2_tagged_sendv_generic(trigger->tsendv.ep,
					   trigger->tsendv.iov,
					   trigger->tsendv.desc,
					   trigger->tsendv.count,
					   trigger->tsendv.dest_addr,
					   trigger->tsendv.tag,
					   trigger->tsendv.context,
					   trigger->tsendv.flags,
					   trigger->tsendv.data);
		break;
	case PSMX2_TRIGGERED_TRECV:
		psmx2_tagged_recv_generic(trigger->trecv.ep,
					  trigger->trecv.buf,
					  trigger->trecv.len,
					  trigger->trecv.desc,
					  trigger->trecv.src_addr,
					  trigger->trecv.tag,
					  trigger->trecv.ignore,
					  trigger->trecv.context,
					  trigger->trecv.flags);
		break;
	case PSMX2_TRIGGERED_WRITE:
		psmx2_write_generic(trigger->write.ep,
				    trigger->write.buf,
				    trigger->write.len,
				    trigger->write.desc,
				    trigger->write.dest_addr,
				    trigger->write.addr,
				    trigger->write.key,
				    trigger->write.context,
				    trigger->write.flags,
				    trigger->write.data);
		break;

	case PSMX2_TRIGGERED_WRITEV:
		psmx2_writev_generic(trigger->writev.ep,
				     trigger->writev.iov,
				     trigger->writev.desc,
				     trigger->writev.count,
				     trigger->writev.dest_addr,
				     trigger->writev.addr,
				     trigger->writev.key,
				     trigger->writev.context,
				     trigger->writev.flags,
				     trigger->writev.data);
		break;

	case PSMX2_TRIGGERED_READ:
		psmx2_read_generic(trigger->read.ep,
				   trigger->read.buf,
				   trigger->read.len,
				   trigger->read.desc,
				   trigger->read.src_addr,
				   trigger->read.addr,
				   trigger->read.key,
				   trigger->read.context,
				   trigger->read.flags);
		break;

	case PSMX2_TRIGGERED_READV:
		psmx2_readv_generic(trigger->readv.ep,
				    trigger->readv.iov,
				    trigger->readv.desc,
				    trigger->readv.count,
				    trigger->readv.src_addr,
				    trigger->readv.addr,
				    trigger->readv.key,
				    trigger->readv.context,
				    trigger->readv.flags);
		break;

	case PSMX2_TRIGGERED_ATOMIC_WRITE:
		psmx2_atomic_write_generic(
				trigger->atomic_write.ep,
				trigger->atomic_write.buf,
				trigger->atomic_write.count,
				trigger->atomic_write.desc,
				trigger->atomic_write.dest_addr,
				trigger->atomic_write.addr,
				trigger->atomic_write.key,
				trigger->atomic_write.datatype,
				trigger->atomic_write.atomic_op,
				trigger->atomic_write.context,
				trigger->atomic_write.flags);
		break;

	case PSMX2_TRIGGERED_ATOMIC_READWRITE:
		psmx2_atomic_readwrite_generic(
				trigger->atomic_readwrite.ep,
				trigger->atomic_readwrite.buf,
				trigger->atomic_readwrite.count,
				trigger->atomic_readwrite.desc,
				trigger->atomic_readwrite.result,
				trigger->atomic_readwrite.result_desc,
				trigger->atomic_readwrite.dest_addr,
				trigger->atomic_readwrite.addr,
				trigger->atomic_readwrite.key,
				trigger->atomic_readwrite.datatype,
				trigger->atomic_readwrite.atomic_op,
				trigger->atomic_readwrite.context,
				trigger->atomic_readwrite.flags);
		break;

	case PSMX2_TRIGGERED_ATOMIC_COMPWRITE:
		psmx2_atomic_compwrite_generic(
				trigger->atomic_compwrite.ep,
				trigger->atomic_compwrite.buf,
				trigger->atomic_compwrite.count,
				trigger->atomic_compwrite.desc,
				trigger->atomic_compwrite.compare,
				trigger->atomic_compwrite.compare_desc,
				trigger->atomic_compwrite.result,
				trigger->atomic_compwrite.result_desc,
				trigger->atomic_compwrite.dest_addr,
				trigger->atomic_compwrite.addr,
				trigger->atomic_compwrite.key,
				trigger->atomic_compwrite.datatype,
				trigger->atomic_compwrite.atomic_op,
				trigger->atomic_compwrite.context,
				trigger->atomic_compwrite.flags);
		break;
	default:
		FI_INFO(&psmx2_prov, FI_LOG_CQ,
			"%d unsupported op\n", trigger->op);
		break;
	}

	free(trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_trecv(struct fid_ep *ep, void *buf,
			      size_t len, void *desc,
			      fi_addr_t src_addr, 
			      uint64_t tag, uint64_t ignore,
			      void *context, uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_TRECV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->trecv.ep = ep;
	trigger->trecv.buf = buf;
	trigger->trecv.len = len;
	trigger->trecv.desc = desc;
	trigger->trecv.src_addr = src_addr;
	trigger->trecv.tag = tag;
	trigger->trecv.ignore = ignore;
	trigger->trecv.context = context;
	trigger->trecv.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_tsend(struct fid_ep *ep,
			      const void *buf, size_t len,
			      void *desc, fi_addr_t dest_addr,
			      uint64_t tag, void *context,
			      uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_TSEND;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->tsend.ep = ep;
	trigger->tsend.buf = buf;
	trigger->tsend.len = len;
	trigger->tsend.desc = desc;
	trigger->tsend.dest_addr = dest_addr;
	trigger->tsend.tag = tag;
	trigger->tsend.context = context;
	trigger->tsend.flags = flags & ~FI_TRIGGER;
	trigger->tsend.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_tsendv(struct fid_ep *ep,
			       const struct iovec *iov, void *desc,
			       size_t count, fi_addr_t dest_addr,
			       uint64_t tag, void *context,
			       uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_TSENDV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->tsendv.ep = ep;
	trigger->tsendv.iov = iov;
	trigger->tsendv.desc = desc;
	trigger->tsendv.count = count;
	trigger->tsendv.dest_addr = dest_addr;
	trigger->tsendv.tag = tag;
	trigger->tsendv.context = context;
	trigger->tsendv.flags = flags & ~FI_TRIGGER;
	trigger->tsendv.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_recv(struct fid_ep *ep, void *buf, size_t len,
			     void *desc, fi_addr_t src_addr, void *context,
			     uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_RECV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->recv.ep = ep;
	trigger->recv.buf = buf;
	trigger->recv.len = len;
	trigger->recv.desc = desc;
	trigger->recv.src_addr = src_addr;
	trigger->recv.context = context;
	trigger->recv.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_send(struct fid_ep *ep, const void *buf, size_t len,
			     void *desc, fi_addr_t dest_addr, void *context,
			     uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_SEND;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->send.ep = ep;
	trigger->send.buf = buf;
	trigger->send.len = len;
	trigger->send.desc = desc;
	trigger->send.dest_addr = dest_addr;
	trigger->send.context = context;
	trigger->send.flags = flags & ~FI_TRIGGER;
	trigger->send.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_sendv(struct fid_ep *ep, const struct iovec *iov,
			      void *desc, size_t count, fi_addr_t dest_addr,
			      void *context, uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_SENDV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->sendv.ep = ep;
	trigger->sendv.iov = iov;
	trigger->sendv.desc = desc;
	trigger->sendv.count = count;
	trigger->sendv.dest_addr = dest_addr;
	trigger->sendv.context = context;
	trigger->sendv.flags = flags & ~FI_TRIGGER;
	trigger->sendv.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_read(struct fid_ep *ep, void *buf, size_t len,
			     void *desc, fi_addr_t src_addr,
			     uint64_t addr, uint64_t key, void *context,
			     uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_READ;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->read.ep = ep;
	trigger->read.buf = buf;
	trigger->read.len = len;
	trigger->read.desc = desc;
	trigger->read.src_addr = src_addr;
	trigger->read.addr = addr;
	trigger->read.key = key;
	trigger->read.context = context;
	trigger->read.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_readv(struct fid_ep *ep, const struct iovec *iov,
			      void *desc, size_t count, fi_addr_t src_addr,
			      uint64_t addr, uint64_t key, void *context,
			      uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_READV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->readv.ep = ep;
	trigger->readv.iov = iov;
	trigger->readv.count = count;
	trigger->readv.desc = desc;
	trigger->readv.src_addr = src_addr;
	trigger->readv.addr = addr;
	trigger->readv.key = key;
	trigger->readv.context = context;
	trigger->readv.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_write(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, fi_addr_t dest_addr,
			      uint64_t addr, uint64_t key, void *context,
			      uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_WRITE;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->write.ep = ep;
	trigger->write.buf = buf;
	trigger->write.len = len;
	trigger->write.desc = desc;
	trigger->write.dest_addr = dest_addr;
	trigger->write.addr = addr;
	trigger->write.key = key;
	trigger->write.context = context;
	trigger->write.flags = flags & ~FI_TRIGGER;
	trigger->write.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_writev(struct fid_ep *ep, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t dest_addr,
			       uint64_t addr, uint64_t key, void *context,
			       uint64_t flags, uint64_t data)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_WRITEV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->writev.ep = ep;
	trigger->writev.iov = iov;
	trigger->writev.count = count;
	trigger->writev.desc = desc;
	trigger->writev.dest_addr = dest_addr;
	trigger->writev.addr = addr;
	trigger->writev.key = key;
	trigger->writev.context = context;
	trigger->writev.flags = flags & ~FI_TRIGGER;
	trigger->writev.data = data;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_write(struct fid_ep *ep,
				     const void *buf,
				     size_t count, void *desc,
				     fi_addr_t dest_addr,
				     uint64_t addr, uint64_t key,
				     enum fi_datatype datatype,
				     enum fi_op op, void *context,
				     uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_WRITE;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_write.ep = ep;
	trigger->atomic_write.buf = buf;
	trigger->atomic_write.count = count;
	trigger->atomic_write.desc = desc;
	trigger->atomic_write.dest_addr = dest_addr;
	trigger->atomic_write.addr = addr;
	trigger->atomic_write.key = key;
	trigger->atomic_write.datatype = datatype;
	trigger->atomic_write.atomic_op = op;
	trigger->atomic_write.context = context;
	trigger->atomic_write.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_writev(struct fid_ep *ep,
				      const struct fi_ioc *iov,
				      void **desc, size_t count,
				      fi_addr_t dest_addr,
				      uint64_t addr, uint64_t key,
				      enum fi_datatype datatype,
				      enum fi_op op, void *context,
				      uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_WRITEV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_writev.ep = ep;
	trigger->atomic_writev.iov = iov;
	trigger->atomic_writev.count = count;
	trigger->atomic_writev.desc = desc;
	trigger->atomic_writev.dest_addr = dest_addr;
	trigger->atomic_writev.addr = addr;
	trigger->atomic_writev.key = key;
	trigger->atomic_writev.datatype = datatype;
	trigger->atomic_writev.atomic_op = op;
	trigger->atomic_writev.context = context;
	trigger->atomic_writev.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_readwrite(struct fid_ep *ep,
				         const void *buf,
				         size_t count, void *desc,
				         void *result, void *result_desc,
				         fi_addr_t dest_addr,
				         uint64_t addr, uint64_t key,
				         enum fi_datatype datatype,
				         enum fi_op op, void *context,
				         uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_READWRITE;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_readwrite.ep = ep;
	trigger->atomic_readwrite.buf = buf;
	trigger->atomic_readwrite.count = count;
	trigger->atomic_readwrite.desc = desc;
	trigger->atomic_readwrite.result = result;
	trigger->atomic_readwrite.result_desc = result_desc;
	trigger->atomic_readwrite.dest_addr = dest_addr;
	trigger->atomic_readwrite.addr = addr;
	trigger->atomic_readwrite.key = key;
	trigger->atomic_readwrite.datatype = datatype;
	trigger->atomic_readwrite.atomic_op = op;
	trigger->atomic_readwrite.context = context;
	trigger->atomic_readwrite.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_readwritev(struct fid_ep *ep,
					  const struct fi_ioc *iov,
					  void **desc, size_t count,
					  struct fi_ioc *resultv,
					  void **result_desc,
					  size_t result_count,
					  fi_addr_t dest_addr,
					  uint64_t addr, uint64_t key,
					  enum fi_datatype datatype,
					  enum fi_op op, void *context,
					  uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_READWRITEV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_readwritev.ep = ep;
	trigger->atomic_readwritev.iov = iov;
	trigger->atomic_readwritev.count = count;
	trigger->atomic_readwritev.desc = desc;
	trigger->atomic_readwritev.resultv = resultv;
	trigger->atomic_readwritev.result_desc = result_desc;
	trigger->atomic_readwritev.result_count = result_count;
	trigger->atomic_readwritev.dest_addr = dest_addr;
	trigger->atomic_readwritev.addr = addr;
	trigger->atomic_readwritev.key = key;
	trigger->atomic_readwritev.datatype = datatype;
	trigger->atomic_readwritev.atomic_op = op;
	trigger->atomic_readwritev.context = context;
	trigger->atomic_readwritev.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_compwrite(struct fid_ep *ep,
				         const void *buf,
				         size_t count, void *desc,
				         const void *compare, void *compare_desc,
				         void *result, void *result_desc,
				         fi_addr_t dest_addr,
				         uint64_t addr, uint64_t key,
				         enum fi_datatype datatype,
				         enum fi_op op, void *context,
				         uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_COMPWRITE;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_compwrite.ep = ep;
	trigger->atomic_compwrite.buf = buf;
	trigger->atomic_compwrite.count = count;
	trigger->atomic_compwrite.desc = desc;
	trigger->atomic_compwrite.compare = compare;
	trigger->atomic_compwrite.compare_desc = compare_desc;
	trigger->atomic_compwrite.result = result;
	trigger->atomic_compwrite.result_desc = result_desc;
	trigger->atomic_compwrite.dest_addr = dest_addr;
	trigger->atomic_compwrite.addr = addr;
	trigger->atomic_compwrite.key = key;
	trigger->atomic_compwrite.datatype = datatype;
	trigger->atomic_compwrite.atomic_op = op;
	trigger->atomic_compwrite.context = context;
	trigger->atomic_compwrite.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

static inline
int psmx2_trigger_queue_atomic_compwritev(struct fid_ep *ep,
					  const struct fi_ioc *iov,
					  void **desc, size_t count,
					  const struct fi_ioc *comparev,
					  void **compare_desc,
					  size_t compare_count,
					  struct fi_ioc *resultv,
					  void **result_desc,
					  size_t result_count,
					  fi_addr_t dest_addr,
					  uint64_t addr, uint64_t key,
					  enum fi_datatype datatype,
					  enum fi_op op, void *context,
					  uint64_t flags)
{
	struct psmx2_trigger *trigger;
	struct fi_triggered_context *ctxt = context;

	trigger = calloc(1, sizeof(*trigger));
	if (!trigger)
		return -FI_ENOMEM;

	trigger->op = PSMX2_TRIGGERED_ATOMIC_COMPWRITEV;
	trigger->cntr = container_of(ctxt->trigger.threshold.cntr,
				     struct psmx2_fid_cntr, cntr);
	trigger->threshold = ctxt->trigger.threshold.threshold;
	trigger->atomic_compwritev.ep = ep;
	trigger->atomic_compwritev.iov = iov;
	trigger->atomic_compwritev.desc = desc;
	trigger->atomic_compwritev.count = count;
	trigger->atomic_compwritev.comparev = comparev;
	trigger->atomic_compwritev.compare_desc = compare_desc;
	trigger->atomic_compwritev.compare_count = compare_count;
	trigger->atomic_compwritev.resultv = resultv;
	trigger->atomic_compwritev.result_desc = result_desc;
	trigger->atomic_compwritev.result_count = result_count;
	trigger->atomic_compwritev.dest_addr = dest_addr;
	trigger->atomic_compwritev.addr = addr;
	trigger->atomic_compwritev.key = key;
	trigger->atomic_compwritev.datatype = datatype;
	trigger->atomic_compwritev.atomic_op = op;
	trigger->atomic_compwritev.context = context;
	trigger->atomic_compwritev.flags = flags & ~FI_TRIGGER;

	psmx2_cntr_add_trigger(trigger->cntr, trigger);
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif

