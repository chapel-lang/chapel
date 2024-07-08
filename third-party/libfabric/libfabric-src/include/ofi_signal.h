/*
 * Copyright (c) 2011-2015 Intel Corporation.  All rights reserved.
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

#ifndef _OFI_SIGNAL_H_
#define _OFI_SIGNAL_H_

#include "config.h"

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <ofi_file.h>
#include <ofi_osd.h>
#include <ofi_atom.h>
#include <rdma/fi_errno.h>


enum {
	FI_READ_FD,
	FI_WRITE_FD
};

struct fd_signal {
	ofi_mutex_t lock;
	int fd[2];
	int byte_avail;
};

static inline int fd_signal_init(struct fd_signal *signal)
{
	int ret;

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, signal->fd);
	if (ret < 0)
		return -ofi_sockerr();

	signal->byte_avail = 0;
	ret = ofi_mutex_init(&signal->lock);
	if (ret)
		goto err1;

	/* The read fd is accessed directly by fd_signal users to add
	 * it to epoll fd's and wait sets.
	 */
	ret = fi_fd_nonblock(signal->fd[FI_READ_FD]);
	if (ret)
		goto err2;

	return 0;

err2:
	ofi_mutex_destroy(&signal->lock);
err1:
	ofi_close_socket(signal->fd[0]);
	ofi_close_socket(signal->fd[1]);
	return ret;
}

static inline void fd_signal_free(struct fd_signal *signal)
{
	ofi_close_socket(signal->fd[0]);
	ofi_close_socket(signal->fd[1]);
	ofi_mutex_destroy(&signal->lock);
}

static inline void fd_signal_set(struct fd_signal *signal)
{
	char c = 0;
	int ret;

	ofi_mutex_lock(&signal->lock);
	if (!signal->byte_avail) {
		ret = ofi_write_socket(signal->fd[FI_WRITE_FD], &c, sizeof c);
		assert(ret == sizeof c);
		if (ret == sizeof c)
			signal->byte_avail++;
	}
	ofi_mutex_unlock(&signal->lock);
}

static inline int fd_signal_poll(struct fd_signal *signal, int timeout)
{
	int ret;

	ret = fi_poll_fd(signal->fd[FI_READ_FD], timeout);
	if (ret < 0)
		return ret;

	return (ret == 0) ? -FI_ETIMEDOUT : 0;
}

/* There's a race where we can write data to the fd and increment byte_avail,
 * but the kernel won't have the data available for reading from the fd yet.
 * If the data isn't ready for reading, but has already been written, we'll
 * wait for it to show up.  A timeout is given just so that we don't end up
 * blocking forever in case something goes terribly astray.
 */
static inline void fd_signal_reset(struct fd_signal *signal)
{
	char c;
	int ret;

	ofi_mutex_lock(&signal->lock);
	while (signal->byte_avail) {
		ret = ofi_read_socket(signal->fd[FI_READ_FD], &c, sizeof c);
		if (ret == sizeof c) {
			signal->byte_avail--;
			continue;
		}
		if (!OFI_SOCK_TRY_SND_RCV_AGAIN(ofi_sockerr())) {
			assert(0);
			break;
		}

		/* Give the kernel up to 10 seconds to get the data there. */
		ret = fd_signal_poll(signal, 10000);
		if (ret) {
			assert(0);
			break;
		}
	}
	ofi_mutex_unlock(&signal->lock);
}

static inline int fd_signal_get(struct fd_signal *signal)
{
	return signal->fd[FI_READ_FD];
}

#endif /* _OFI_SIGNAL_H_ */
