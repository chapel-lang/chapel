/*
 * Copyright (c) 2014 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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
 *
 */

#ifndef _OFI_RBUF_H_
#define _OFI_RBUF_H_

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <ofi.h>
#include <ofi_file.h>
#include <stdlib.h>


/*
 * Circular queue/array template
 */
#define OFI_DECLARE_CIRQUE(entrytype, name)                     \
struct name {							\
	size_t		size;					\
	size_t		size_mask;				\
	size_t		rcnt;					\
	size_t		wcnt;					\
	entrytype	buf[];					\
};								\
								\
static inline void name ## _init(struct name *cq, size_t size)	\
{								\
	assert(size == roundup_power_of_two(size));		\
	cq->size = size;					\
	cq->size_mask = cq->size - 1;				\
	cq->rcnt = 0;						\
	cq->wcnt = 0;						\
}								\
								\
static inline struct name * name ## _create(size_t size)	\
{								\
	struct name *cq;					\
	cq = (struct name*) calloc(1, sizeof(*cq) + sizeof(entrytype) *	\
		    (roundup_power_of_two(size)));		\
	if (cq)							\
		name ##_init(cq, roundup_power_of_two(size));	\
	return cq;						\
}								\
								\
static inline void name ## _free(struct name *cq)		\
{								\
	free(cq);						\
}

#define ofi_cirque_isempty(cq)		((cq)->wcnt == (cq)->rcnt)
#define ofi_cirque_usedcnt(cq)		((cq)->wcnt - (cq)->rcnt)
#define ofi_cirque_freecnt(cq)		((cq)->size - ofi_cirque_usedcnt(cq))
#define ofi_cirque_isfull(cq)		(ofi_cirque_freecnt(cq) <= 0)

#define ofi_cirque_rindex(cq)		((cq)->rcnt & (cq)->size_mask)
#define ofi_cirque_windex(cq)		((cq)->wcnt & (cq)->size_mask)
#define ofi_cirque_head(cq)		(&(cq)->buf[ofi_cirque_rindex(cq)])
#define ofi_cirque_tail(cq)		(&(cq)->buf[ofi_cirque_windex(cq)])
#define ofi_cirque_insert(cq, x)	(cq)->buf[(cq)->wcnt++ & (cq)->size_mask] = x
#define ofi_cirque_remove(cq)		(&(cq)->buf[(cq)->rcnt++ & (cq)->size_mask])
#define ofi_cirque_discard(cq)		((cq)->rcnt++)
#define ofi_cirque_commit(cq)		((cq)->wcnt++)


/*
 * Simple ring buffer
 */
struct ofi_ringbuf {
	size_t		size;
	size_t		size_mask;
	size_t		rcnt;
	size_t		wcnt;
	size_t		wpos;
	void		*buf;
};

static inline int ofi_rbinit(struct ofi_ringbuf *rb, size_t size)
{
	rb->size = roundup_power_of_two(size);
	rb->size_mask = rb->size - 1;
	rb->rcnt = 0;
	rb->wcnt = 0;
	rb->wpos = 0;
	rb->buf = calloc(1, rb->size);
	if (!rb->buf)
		return -ENOMEM;
	return 0;
}

static inline void ofi_rbreset(struct ofi_ringbuf *rb)
{
	rb->rcnt = 0;
	rb->wcnt = 0;
	rb->wpos = 0;
}

static inline void ofi_rbfree(struct ofi_ringbuf *rb)
{
	free(rb->buf);
}

static inline int ofi_rbfull(struct ofi_ringbuf *rb)
{
	return rb->wcnt - rb->rcnt >= rb->size;
}

static inline int ofi_rbempty(struct ofi_ringbuf *rb)
{
	return rb->wcnt == rb->rcnt;
}

static inline size_t ofi_rbused(struct ofi_ringbuf *rb)
{
	return rb->wcnt - rb->rcnt;
}

static inline size_t ofi_rbavail(struct ofi_ringbuf *rb)
{
	return rb->size - ofi_rbused(rb);
}

static inline void ofi_rbwrite(struct ofi_ringbuf *rb, const void *buf, size_t len)
{
	size_t endlen;

	endlen = rb->size - (rb->wpos & rb->size_mask);
	if (len <= endlen) {
		memcpy((char*)rb->buf + (rb->wpos & rb->size_mask), buf, len);
	} else {
		memcpy((char*)rb->buf + (rb->wpos & rb->size_mask), buf, endlen);
		memcpy(rb->buf, (char*)buf + endlen, len - endlen);
	}
	rb->wpos += len;
}

static inline void ofi_rbcommit(struct ofi_ringbuf *rb)
{
	rb->wcnt = rb->wpos;
}

static inline void ofi_rbabort(struct ofi_ringbuf *rb)
{
	rb->wpos = rb->wcnt;
}

static inline void ofi_rbpeek(struct ofi_ringbuf *rb, void *buf, size_t len)
{
	size_t endlen;

	endlen = rb->size - (rb->rcnt & rb->size_mask);
	if (len <= endlen) {
		memcpy(buf, (char*)rb->buf + (rb->rcnt & rb->size_mask), len);
	} else {
		memcpy(buf, (char*)rb->buf + (rb->rcnt & rb->size_mask), endlen);
		memcpy((char*)buf + endlen, rb->buf, len - endlen);
	}
}

static inline void ofi_rbread(struct ofi_ringbuf *rb, void *buf, size_t len)
{
	ofi_rbpeek(rb, buf, len);
	rb->rcnt += len;
}

static inline size_t ofi_rbdiscard(struct ofi_ringbuf *rb, size_t len)
{
	size_t used_len = MIN(ofi_rbused(rb), len);
	rb->rcnt += used_len;
	return used_len;
}

/*
 * Ring buffer with blocking read support using an fd
 */
enum {
	OFI_RB_READ_FD,
	OFI_RB_WRITE_FD
};

struct ofi_ringbuffd {
	struct ofi_ringbuf	rb;
	int			fdrcnt;
	int			fdwcnt;
	int			fd[2];
};

static inline int ofi_rbfdinit(struct ofi_ringbuffd *rbfd, size_t size)
{
	int ret;

	rbfd->fdrcnt = 0;
	rbfd->fdwcnt = 0;
	ret = ofi_rbinit(&rbfd->rb, size);
	if (ret)
		return ret;

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, rbfd->fd);
	if (ret < 0) {
		ret = -ofi_sockerr();
		goto err1;
	}

	ret = fi_fd_nonblock(rbfd->fd[OFI_RB_READ_FD]);
	if (ret)
		goto err2;

	return 0;

err2:
	ofi_close_socket(rbfd->fd[0]);
	ofi_close_socket(rbfd->fd[1]);
err1:
	ofi_rbfree(&rbfd->rb);
	return ret;
}

static inline void ofi_rbfdfree(struct ofi_ringbuffd *rbfd)
{
	ofi_rbfree(&rbfd->rb);
	ofi_close_socket(rbfd->fd[0]);
	ofi_close_socket(rbfd->fd[1]);
}

static inline int ofi_rbfdfull(struct ofi_ringbuffd *rbfd)
{
	return ofi_rbfull(&rbfd->rb);
}

static inline int ofi_rbfdempty(struct ofi_ringbuffd *rbfd)
{
	return ofi_rbempty(&rbfd->rb);
}

static inline size_t ofi_rbfdused(struct ofi_ringbuffd *rbfd)
{
	return ofi_rbused(&rbfd->rb);
}

static inline size_t ofi_rbfdavail(struct ofi_ringbuffd *rbfd)
{
	return ofi_rbavail(&rbfd->rb);
}

static inline void ofi_rbfdsignal(struct ofi_ringbuffd *rbfd)
{
	char c = 0;
	if (rbfd->fdwcnt == rbfd->fdrcnt) {
		if (ofi_write_socket(rbfd->fd[OFI_RB_WRITE_FD], &c, sizeof c) == sizeof c)
			rbfd->fdwcnt++;
	}
}

static inline void ofi_rbfdreset(struct ofi_ringbuffd *rbfd)
{
	char c;

	if (ofi_rbfdempty(rbfd) && (rbfd->fdrcnt != rbfd->fdwcnt)) {
		if (ofi_read_socket(rbfd->fd[OFI_RB_READ_FD], &c, sizeof c) == sizeof c)
			rbfd->fdrcnt++;
	}
}

static inline void ofi_rbfdwrite(struct ofi_ringbuffd *rbfd, const void *buf, size_t len)
{
	ofi_rbwrite(&rbfd->rb, buf, len);
}

static inline void ofi_rbfdcommit(struct ofi_ringbuffd *rbfd)
{
	ofi_rbcommit(&rbfd->rb);
	ofi_rbfdsignal(rbfd);
}

static inline void ofi_rbfdabort(struct ofi_ringbuffd *rbfd)
{
	ofi_rbabort(&rbfd->rb);
}

static inline void ofi_rbfdpeek(struct ofi_ringbuffd *rbfd, void *buf, size_t len)
{
	ofi_rbpeek(&rbfd->rb, buf, len);
}

static inline void ofi_rbfdread(struct ofi_ringbuffd *rbfd, void *buf, size_t len)
{
	ofi_rbread(&rbfd->rb, buf, len);
	ofi_rbfdreset(rbfd);
}

static inline size_t ofi_rbfdsread(struct ofi_ringbuffd *rbfd, void *buf, size_t len,
				int timeout)
{
	int ret;
	size_t avail;

	avail = ofi_rbfdused(rbfd);
	if (avail) {
		len = MIN(len, avail);
		ofi_rbfdread(rbfd, buf, len);
		return len;
	}

	ret = fi_poll_fd(rbfd->fd[OFI_RB_READ_FD], timeout);
	if (ret == 1) {
		len = MIN(len, ofi_rbfdused(rbfd));
		ofi_rbfdread(rbfd, buf, len);
		return len;
	}
	return ret;
}

static inline size_t ofi_rbfdwait(struct ofi_ringbuffd *rbfd, int timeout)
{
	return  fi_poll_fd(rbfd->fd[OFI_RB_READ_FD], timeout);
}


#endif /* _OFI_RBUF_H_ */
