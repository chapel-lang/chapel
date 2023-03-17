/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DIRECT_EQ_H_
#define _FI_PROV_OPX_DIRECT_EQ_H_

#define FABRIC_DIRECT_EQ 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FABRIC_DIRECT

#define fi_cq_read(cq, buf, count)					\
	(fi_opx_cq_read_FABRIC_DIRECT(cq, buf, count))

ssize_t
fi_opx_cq_read_FABRIC_DIRECT(struct fid_cq *cq, void *buf, size_t count);


#define fi_cq_readfrom(cq, buf, count, src_addr)			\
	(fi_opx_cq_readfrom_FABRIC_DIRECT(cq, buf, count, src_addr))

ssize_t
fi_opx_cq_readfrom_FABRIC_DIRECT(struct fid_cq *cq, void *buf, size_t count,
		fi_addr_t *src_addr);


static inline
ssize_t fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
		uint64_t flags)
{
	return cq->ops->readerr(cq, buf, flags);
}

static inline
uint64_t fi_cntr_read(struct fid_cntr *cntr)
{
	return cntr->ops->read(cntr);
}

static inline
int fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	return cntr->ops->wait(cntr, threshold, timeout);
}

static inline
int fi_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_wait(struct fid_wait *waitset, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll(struct fid_poll *pollset, void **context, int count)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			     struct fid_eq **eq, void *context)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_read(struct fid_eq *eq, uint32_t *event, void *buf,
				 size_t len, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_readerr(struct fid_eq *eq,
				    struct fi_eq_err_entry *buf, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_write(struct fid_eq *eq, uint32_t event,
				  const void *buf, size_t len, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf,
				  size_t len, int timeout, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
const char *fi_eq_strerror(struct fid_eq *eq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return NULL;		/* TODO - implement this */
}

static inline
ssize_t fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
				  const void *cond, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_cq_sreadfrom(struct fid_cq *cq, void *buf,
				      size_t count, fi_addr_t *src_addr,
				      const void *cond, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_cq_signal(struct fid_cq *cq)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
const char *fi_cq_strerror(struct fid_cq *cq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return NULL;		/* TODO - implement this */
}

static inline
uint64_t fi_cntr_readerr(struct fid_cntr *cntr)
{
	return 0;		/* TODO - implement this */
}

static inline
int fi_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_DIRECT_EQ_H_ */
