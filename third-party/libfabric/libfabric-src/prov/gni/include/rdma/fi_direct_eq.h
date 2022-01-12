/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
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

#ifndef _FI_DIRECT_EQ_H_
#define _FI_DIRECT_EQ_H_

#define FABRIC_DIRECT_EQ 1

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern int gnix_fabric_trywait(struct fid_fabric *fabric, struct fid **fids,
                               int count);

extern int gnix_wait_wait(struct fid_wait *wait, int timeout);

extern int gnix_poll_poll(struct fid_poll *pollset, void **context, int count);

extern int gnix_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			 uint64_t flags);

extern int gnix_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			 uint64_t flags);

extern int gnix_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			struct fid_eq **eq, void *context);

extern ssize_t gnix_eq_read(struct fid_eq *eq, uint32_t *event, void *buf,
			size_t len, uint64_t flags);

extern ssize_t gnix_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf,
			   uint64_t flags);

extern ssize_t gnix_eq_write(struct fid_eq *eq, uint32_t event, const void *buf,
			 size_t len, uint64_t flags);

extern ssize_t gnix_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf,
			     size_t len, int timeout, uint64_t flags);

extern const char *gnix_eq_strerror(struct fid_eq *eq, int prov_errno,
				    const void *err_data, char *buf,
				    size_t len);

extern ssize_t gnix_cq_read(struct fid_cq *cq, void *buf, size_t count);

extern ssize_t gnix_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
			    fi_addr_t *src_addr);

extern ssize_t gnix_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
			       uint64_t flags);

extern ssize_t gnix_cq_sread(struct fid_cq *cq, void *buf, size_t count,
			 const void *cond, int timeout);

extern ssize_t gnix_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
			     fi_addr_t *src_addr, const void *cond,
			     int timeout);

extern int gnix_cq_signal(struct fid_cq *cq);

extern const char *gnix_cq_strerror(struct fid_cq *cq, int prov_errno,
				    const void *err_data, char *buf,
				    size_t len);

extern uint64_t gnix_cntr_read(struct fid_cntr *cntr);

extern uint64_t gnix_cntr_readerr(struct fid_cntr *cntr);

extern int gnix_cntr_add(struct fid_cntr *cntr, uint64_t value);

extern int gnix_cntr_set(struct fid_cntr *cntr, uint64_t value);

extern int gnix_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
			  int timeout);

extern int gnix_cntr_adderr(struct fid_cntr *cntr, uint64_t value);

extern int gnix_cntr_seterr(struct fid_cntr *cntr, uint64_t value);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline int
fi_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	return gnix_fabric_trywait(fabric, fids, count);
}

static inline int fi_wait(struct fid_wait *waitset, int timeout)
{
	return gnix_wait_wait(waitset, timeout);
}

static inline int fi_poll(struct fid_poll *pollset, void **context, int count)
{
	return gnix_poll_poll(pollset, context, count);
}

static inline int fi_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return gnix_poll_add(pollset, event_fid, flags);
}

static inline int fi_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return gnix_poll_del(pollset, event_fid, flags);
}

static inline int fi_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			     struct fid_eq **eq, void *context)
{
	return gnix_eq_open(fabric, attr, eq, context);
}

static inline ssize_t fi_eq_read(struct fid_eq *eq, uint32_t *event, void *buf,
				 size_t len, uint64_t flags)
{
	return gnix_eq_read(eq, event, buf, len, flags);
}

static inline ssize_t fi_eq_readerr(struct fid_eq *eq,
				    struct fi_eq_err_entry *buf, uint64_t flags)
{
	return gnix_eq_readerr(eq, buf, flags);
}

static inline ssize_t fi_eq_write(struct fid_eq *eq, uint32_t event,
				  const void *buf, size_t len, uint64_t flags)
{
	return gnix_eq_write(eq, event, buf, len, flags);
}

static inline ssize_t fi_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf,
				  size_t len, int timeout, uint64_t flags)
{
	return gnix_eq_sread(eq, event, buf, len, timeout, flags);
}

static inline const char *fi_eq_strerror(struct fid_eq *eq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return gnix_eq_strerror(eq, prov_errno, err_data, buf, len);
}

static inline ssize_t fi_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	return gnix_cq_read(cq, buf, count);
}

static inline ssize_t fi_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				     fi_addr_t *src_addr)
{
	return gnix_cq_readfrom(cq, buf, count, src_addr);
}

static inline ssize_t fi_cq_readerr(struct fid_cq *cq,
				    struct fi_cq_err_entry *buf, uint64_t flags)
{
	return gnix_cq_readerr(cq, buf, flags);
}

static inline ssize_t fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
				  const void *cond, int timeout)
{
	return gnix_cq_sread(cq, buf, count, cond, timeout);
}

static inline ssize_t fi_cq_sreadfrom(struct fid_cq *cq, void *buf,
				      size_t count, fi_addr_t *src_addr,
				      const void *cond, int timeout)
{
	return gnix_cq_sreadfrom(cq, buf, count, src_addr, cond, timeout);
}

static inline int fi_cq_signal(struct fid_cq *cq)
{
	return gnix_cq_signal(cq);
}

static inline const char *fi_cq_strerror(struct fid_cq *cq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return gnix_cq_strerror(cq, prov_errno, err_data, buf, len);
}

static inline uint64_t fi_cntr_read(struct fid_cntr *cntr)
{
	return gnix_cntr_read(cntr);
}

static inline uint64_t fi_cntr_readerr(struct fid_cntr *cntr)
{
	return gnix_cntr_readerr(cntr);
}

static inline int fi_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	return gnix_cntr_add(cntr, value);
}

static inline int fi_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	return gnix_cntr_set(cntr, value);
}

static inline int fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
			       int timeout)
{
	return gnix_cntr_wait(cntr, threshold, timeout);
}

static inline int fi_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	return gnix_cntr_adderr(cntr, value);
}

static inline int fi_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	return gnix_cntr_seterr(cntr, value);
}

#endif /* _FI_DIRECT_EQ_H_ */
