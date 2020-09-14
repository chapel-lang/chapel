/*
 * Copyright (c) 2011-s2018 Intel Corporation.  All rights reserved.
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

enum ofi_pollfds_ctl {
	POLLFDS_CTL_ADD,
	POLLFDS_CTL_DEL,
	POLLFDS_CTL_MOD,
};

struct ofi_pollfds_work_item {
	int		fd;
	uint32_t	events;
	void		*context;
	enum ofi_pollfds_ctl type;
	struct slist_entry entry;
};

struct ofi_pollfds {
	int		size;
	int		nfds;
	struct pollfd	*fds;
	void		**context;
	int		index;
	struct fd_signal signal;
	struct slist	work_item_list;
	fastlock_t	lock;
};

int ofi_pollfds_create(struct ofi_pollfds **pfds);
int ofi_pollfds_add(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context);
int ofi_pollfds_mod(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context);
int ofi_pollfds_del(struct ofi_pollfds *pfds, int fd);
int ofi_pollfds_wait(struct ofi_pollfds *pfds, void **contexts,
		     int max_contexts, int timeout);
void ofi_pollfds_close(struct ofi_pollfds *pfds);


#ifdef HAVE_EPOLL
#include <sys/epoll.h>

#define OFI_EPOLL_IN  EPOLLIN
#define OFI_EPOLL_OUT EPOLLOUT

typedef int ofi_epoll_t;
#define OFI_EPOLL_INVALID -1

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

static inline int ofi_epoll_wait(int ep, void **contexts, int max_contexts,
                                int timeout)
{
	struct epoll_event events[max_contexts];
	int ret;
	int i;

	ret = epoll_wait(ep, events, max_contexts, timeout);
	if (ret == -1)
		return -ofi_syserr();

	for (i = 0; i < ret; i++)
		contexts[i] = events[i].data.ptr;
	return ret;
}

static inline void ofi_epoll_close(int ep)
{
	close(ep);
}

#else

#define OFI_EPOLL_IN  POLLIN
#define OFI_EPOLL_OUT POLLOUT

typedef struct ofi_pollfds *ofi_epoll_t;
#define OFI_EPOLL_INVALID NULL

#define ofi_epoll_create ofi_pollfds_create
#define ofi_epoll_add ofi_pollfds_add
#define ofi_epoll_mod ofi_pollfds_mod
#define ofi_epoll_del ofi_pollfds_del
#define ofi_epoll_wait ofi_pollfds_wait
#define ofi_epoll_close ofi_pollfds_close

#define EPOLL_CTL_ADD POLLFDS_CTL_ADD
#define EPOLL_CTL_DEL POLLFDS_CTL_DEL
#define EPOLL_CTL_MOD POLLFDS_CTL_MOD

#endif /* HAVE_EPOLL */

#endif  /* _OFI_EPOLL_H_ */
