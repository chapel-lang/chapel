/*
 * Copyright (c) 2011-s2018 Intel Corporation.  All rights reserved.
 * Copyright (c) 2025 VDURA, Inc.  All rights reserved.
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

#ifndef _OFI_EPOLL_H_
#define _OFI_EPOLL_H_

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <ofi_list.h>
#include <ofi_signal.h>
#include <ofi.h>

#ifdef HAVE_KQUEUE
#include <sys/event.h>
#include <sys/time.h>
#include <time.h>

#define ofi_epollfds_event kevent
#define OFI_EPOLL_EVT_HAS_INPUT(evt) evt.filter == EVFILT_READ
#define OFI_EPOLL_EVT_HAS_OUTPUT(evt) evt.filter == EVFILT_WRITE
#define OFI_EPOLL_EVT_HAS_ERR(evt) evt.flags & EV_ERROR
#define OFI_EPOLL_EVT_DATA(evt) evt.udata
#define OFI_EPOLL_EVT_EVENTS(evt) evt.filter

#define OFI_EPOLL_IN  EVFILT_READ
#define OFI_EPOLL_OUT EVFILT_WRITE

typedef int ofi_epoll_t;
#define OFI_EPOLL_INVALID -1

static const bool ofi_have_epoll = true;

static inline int ofi_epoll_create(int *ep)
{
	*ep = kqueue();
	return *ep < 0 ? -ofi_syserr() : 0;
}

static inline int
ofi_epoll_add(int ep, int fd, uint32_t events, void *context)
{
	struct kevent kevents[2];
	int kevent_cnt = 0;

	/* Translate event bits into corresponding separate filters, since
	 * '.filter' values are not bitmasks
	 */
	if (events & POLLIN)
		EV_SET(&kevents[kevent_cnt++], fd, EVFILT_READ, EV_ADD,
			0, 0, context);

	if (events & POLLOUT)
		EV_SET(&kevents[kevent_cnt++], fd, EVFILT_WRITE, EV_ADD,
			0, 0, context);

	return kevent(ep, kevents, kevent_cnt, NULL, 0, NULL) < 0
		? -ofi_syserr() : 0;
}

static inline int
ofi_epoll_mod(int ep, int fd, uint32_t events, void *context)
{
	/* Ensure no filters not included into the events are present -
	 * contrary to 'epoll' the 'kqueue' stores '(ident, filter)' pairs
	 * separately, while 'epoll' has single entry per FD (since events are
	 * bitmask). As an illustrative example, 'epoll' semantics switching
	 * from POLLIN to POLLOUT would work just fine, while 'kqueue' would
	 * have excessive 'EVFILT_READ' still present.
	*/
	struct kevent kevents[2];
	int kevent_cnt = 0;
	int rc;

	/* Remove filters not having corresponding events bit set */
	if (!(events & POLLIN))
		EV_SET(&kevents[kevent_cnt++], fd, EVFILT_READ, EV_DELETE,
			0, 0, NULL);

	if (!(events & POLLOUT))
		EV_SET(&kevents[kevent_cnt++], fd, EVFILT_WRITE, EV_DELETE,
			0, 0, NULL);

	rc = kevent(ep, kevents, kevent_cnt, NULL, 0, NULL);
	/* Ignore error if no filter to delete has been found */
	if (rc < 0 && errno != ENOENT)
		return -ofi_syserr();

	/* Add requested events as filters */
	return ofi_epoll_add(ep, fd, events, context);
}

static inline int
ofi_epoll_del(int ep, int fd)
{
	struct kevent event[2];
	int rc;

	EV_SET(&event[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	EV_SET(&event[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

	rc = kevent(ep, event, 2, NULL, 0, NULL);

	/* Ignore error if no filter to delete has been found */
	if (rc < 0 && errno != ENOENT)
		return -ofi_syserr();

	return  0;
}

static inline int
ofi_epoll_wait(int ep, struct ofi_epollfds_event *events,
		 int maxevents, int timeout)
{
	int rc;
	struct timespec ts, *tsp = NULL;

	/* Convert milliseconds to 'struct timespec' */
	do {
		/* Timeout is processed only when input is not negative, the
		 * latter indicates polling request
		 */
		if (timeout < 0)
			break;

		tsp = &ts;
		if (timeout == 0) {
			ts.tv_sec = ts.tv_nsec = 0;
			break;
		}

		/* Convert seconds from milliseconds */
		ts.tv_sec  = timeout / 1000;
		/* Convert the remainder to nanoseconds */
		ts.tv_nsec = (timeout % 1000) * 1000000;
	} while (0);

	/* Note that 'kevent()' could return multiple entries for different
	 * conditions over same FD - twice as much compared to 'epoll_wait()',
	 * since read and write readiness are of interest. Hence, returned
	 * number of events will be lower than for 'epoll_wait()', though it
	 * is assumed to be ok since upper layers will capture remaining events
	 * in the next function call.
	 */
	rc  = kevent(ep, NULL, 0, events, maxevents, tsp);
	if (rc < 0)
		return -ofi_syserr();

	return rc;
}

static inline void ofi_epoll_close(int ep)
{
	close(ep);
}

static inline int ofi_epoll_fd(int ep)
{
	return ep;
}

#else  /* HAVE_KQUEUE */

/* Both `epoll` and `poll` share same data structures */
#define OFI_EPOLL_EVT_HAS_INPUT(evt) evt.events & POLLIN
#define OFI_EPOLL_EVT_HAS_OUTPUT(evt) evt.events & POLLOUT
#define OFI_EPOLL_EVT_HAS_ERR(evt) evt.events & POLLERR
#define OFI_EPOLL_EVT_DATA(evt) evt.data.ptr
#define OFI_EPOLL_EVT_EVENTS(evt) evt.events

#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#define ofi_epollfds_event epoll_event

#define OFI_EPOLL_IN  EPOLLIN
#define OFI_EPOLL_OUT EPOLLOUT

typedef int ofi_epoll_t;
#define OFI_EPOLL_INVALID -1

static const bool ofi_have_epoll = true;

static inline int ofi_epoll_create(int *ep)
{
	*ep = epoll_create(4);
	return *ep < 0 ? -ofi_syserr() : 0;
}

static inline int ofi_epoll_add(int ep, int fd, uint32_t events, void *context)
{
	struct epoll_event event;
	int ret;

	event.data.ptr = context;
	event.events = events;
	ret = epoll_ctl(ep, EPOLL_CTL_ADD, fd, &event);
	if ((ret == -1) && (ofi_syserr() != EEXIST))
		return -ofi_syserr();
	return 0;
}

static inline int ofi_epoll_mod(int ep, int fd, uint32_t events, void *context)
{
	struct epoll_event event;

	event.data.ptr = context;
	event.events = events;
	return epoll_ctl(ep, EPOLL_CTL_MOD, fd, &event) ? -ofi_syserr() : 0;
}

static inline int ofi_epoll_del(int ep, int fd)
{
	return epoll_ctl(ep, EPOLL_CTL_DEL, fd, NULL) ? -ofi_syserr() : 0;
}

static inline int
ofi_epoll_wait(int ep, struct ofi_epollfds_event *events,
	       int maxevents, int timeout)
{
	int ret;

	ret = epoll_wait(ep, (struct epoll_event *) events, maxevents,
			 timeout);
	if (ret == -1)
		return -ofi_syserr();

	return ret;
}

static inline void ofi_epoll_close(int ep)
{
	close(ep);
}

static inline int ofi_epoll_fd(int ep)
{
	return ep;
}

#else  /* HAVE_EPOLL */
struct ofi_epollfds_event {
	uint32_t events;
	union {
		void *ptr;
	} data;
};

#define OFI_EPOLL_IN  POLLIN
#define OFI_EPOLL_OUT POLLOUT

typedef struct ofi_pollfds *ofi_epoll_t;
#define OFI_EPOLL_INVALID NULL

static const bool ofi_have_epoll = false;

#define ofi_epoll_create ofi_pollfds_create
#define ofi_epoll_add ofi_pollfds_add
#define ofi_epoll_mod ofi_pollfds_mod
#define ofi_epoll_del ofi_pollfds_del
#define ofi_epoll_wait ofi_pollfds_wait
#define ofi_epoll_close ofi_pollfds_close

static inline int ofi_epoll_fd(ofi_epoll_t ep)
{
	return INVALID_SOCKET;
}

#define EPOLL_CTL_ADD POLLFDS_CTL_ADD
#define EPOLL_CTL_DEL POLLFDS_CTL_DEL
#define EPOLL_CTL_MOD POLLFDS_CTL_MOD

#endif  /* HAVE_EPOLL */
#endif  /* HAVE_KQUEUE */

enum ofi_pollfds_ctl {
	POLLFDS_CTL_ADD,
	POLLFDS_CTL_DEL,
	POLLFDS_CTL_MOD,
};

struct ofi_pollfds_work_item {
	int		fd;
	uint32_t	events;
	void		*context;
	enum ofi_pollfds_ctl op;
	struct slist_entry entry;
};

struct ofi_pollfds_ctx {
	void		*context;
	int		index;
};

struct ofi_pollfds {
	int		size;
	int		nfds;
	struct pollfd	*fds;
	struct ofi_pollfds_ctx *ctx;
	struct fd_signal signal;
	struct slist	work_item_list;
	struct ofi_genlock lock;

	int (*add)(struct ofi_pollfds *pfds, int fd, uint32_t events,
		   void *context);
	int (*del)(struct ofi_pollfds *pfds, int fd);
};

int ofi_pollfds_create_(struct ofi_pollfds **pfds, enum ofi_lock_type lock_type);
int ofi_pollfds_create(struct ofi_pollfds **pfds);
int ofi_pollfds_grow(struct ofi_pollfds *pfds, int max_size);

static inline int
ofi_pollfds_add(struct ofi_pollfds *pfds, int fd, uint32_t events, void *context)
{
	return pfds->add(pfds, fd, events, context);
}

int ofi_pollfds_mod(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context);

static inline int ofi_pollfds_del(struct ofi_pollfds *pfds, int fd)
{
	return pfds->del(pfds, fd);
}

int ofi_pollfds_wait(struct ofi_pollfds *pfds,
		     struct ofi_epollfds_event *events,
		     int maxevents, int timeout);
void ofi_pollfds_close(struct ofi_pollfds *pfds);

/* OS specific */
struct ofi_pollfds_ctx *ofi_pollfds_get_ctx(struct ofi_pollfds *pfds, int fd);
struct ofi_pollfds_ctx *ofi_pollfds_alloc_ctx(struct ofi_pollfds *pfds, int fd);

/* If we HAVE_EPOLL, the values for EPOLLIN and EPOLLOUT are the same as
 * POLLIN and POLLOUT, at least in the gnu headers.  If we don't have
 * epoll support, then we're emulating it using poll, in which case the
 * values are also the same (e.g. OFI_EPOLL_IN == POLLIN).
 *
 * This use of this function helps make it clear that we're passing the
 * correct event values to epoll, versus poll, without actually incurring
 * the unnecessary overhead of converting the values.
 *
 * For HAVE_KQUEUE case the filter values in 'struct kevent.filter' are not
 * done bitwise, hence 'struct pollfd' events being those cannot be directly
 * translated for the 'kevent()', 'ofi_epoll_add()' function handles that
 * semantic difference.
 */
static inline uint32_t ofi_poll_to_epoll(uint32_t events)
{
	return events;
}

/* Dynamic poll: selects between using poll vs epoll.
 */
enum ofi_dynpoll_type {
	OFI_DYNPOLL_UNINIT,
	OFI_DYNPOLL_EPOLL,
	OFI_DYNPOLL_POLL,
};

struct ofi_dynpoll {
	enum ofi_dynpoll_type type;
	union {
		struct ofi_pollfds *pfds;
		ofi_epoll_t ep;
	};

	int	(*add)(struct ofi_dynpoll *dynpoll, int fd, uint32_t events,
			void *context);
	int	(*mod)(struct ofi_dynpoll *dynpoll, int fd, uint32_t events,
			void *context);
	int	(*del)(struct ofi_dynpoll *dynpoll, int fd);
	int	(*wait)(struct ofi_dynpoll *dynpoll,
			struct ofi_epollfds_event *events, int maxevents,
			int timeout);
	int	(*get_fd)(struct ofi_dynpoll *dynpoll);
	void	(*close)(struct ofi_dynpoll *dynpoll);
};

int ofi_dynpoll_create(struct ofi_dynpoll *dynpoll, enum ofi_dynpoll_type type,
		       enum ofi_lock_type lock_type);
void ofi_dynpoll_close(struct ofi_dynpoll *dynpoll);

static inline int
ofi_dynpoll_add(struct ofi_dynpoll *dynpoll, int fd,
		uint32_t events, void *context)
{
	return dynpoll->add(dynpoll, fd, events, context);
}

static inline int
ofi_dynpoll_mod(struct ofi_dynpoll *dynpoll, int fd,
		uint32_t events, void *context)
{
	return dynpoll->mod(dynpoll, fd, events, context);
}

static inline int
ofi_dynpoll_del(struct ofi_dynpoll *dynpoll, int fd)
{
	return dynpoll->del(dynpoll, fd);
}

static inline int
ofi_dynpoll_wait(struct ofi_dynpoll *dynpoll,
		 struct ofi_epollfds_event *events,
		 int maxevents, int timeout)
{
	return dynpoll->wait(dynpoll, events, maxevents, timeout);
}

static inline int
ofi_dynpoll_get_fd(struct ofi_dynpoll *dynpoll)
{
	return dynpoll->get_fd(dynpoll);
}

#endif  /* _OFI_EPOLL_H_ */
