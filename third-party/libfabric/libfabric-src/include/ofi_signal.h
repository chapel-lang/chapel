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

enum ofi_signal_state {
	OFI_SIGNAL_UNSET,
	OFI_SIGNAL_WRITE_PREPARE,
	OFI_SIGNAL_SET,
	OFI_SIGNAL_READ_PREPARE,
};

struct fd_signal {
	ofi_atomic32_t	state;
	int		fd[2];

#if ENABLE_DEBUG
	ofi_atomic32_t debug_cnt;
#endif
};

static inline int fd_signal_init(struct fd_signal *signal)
{
	int ret;

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, signal->fd);
	if (ret < 0)
		return -ofi_sockerr();

	ret = fi_fd_nonblock(signal->fd[FI_READ_FD]);
	if (ret)
		goto err;

	ofi_atomic_initialize32(&signal->state, OFI_SIGNAL_UNSET);

#if ENABLE_DEBUG
	ofi_atomic_initialize32(&signal->debug_cnt, 0);
#endif
	return 0;

err:
	ofi_close_socket(signal->fd[0]);
	ofi_close_socket(signal->fd[1]);
	return ret;
}

static inline void fd_signal_free(struct fd_signal *signal)
{
	ofi_close_socket(signal->fd[0]);
	ofi_close_socket(signal->fd[1]);
}

static inline void fd_signal_set(struct fd_signal *signal)
{
	char c = 0;
	bool cas; /* cas result */
	int write_rc;

	cas = ofi_atomic_cas_bool_strong32(&signal->state,
					   OFI_SIGNAL_UNSET,
					   OFI_SIGNAL_WRITE_PREPARE);
	if (cas) {
		write_rc = ofi_write_socket(signal->fd[FI_WRITE_FD], &c,
					    sizeof c);
		if (write_rc == sizeof c) {
#if ENABLE_DEBUG
			assert(ofi_atomic_inc32(&signal->debug_cnt) == 1);
#endif
			ofi_atomic_set32(&signal->state, OFI_SIGNAL_SET);
		} else {
			/* XXX: Setting the signal failed, a polling thread
			 * will not be woken up now and the system might
			 * get stuck.
			 * Also, typically this will be totally
			 * untested code path, as it basically will never
			 * come up.
			 */
			ofi_atomic_set32(&signal->state, OFI_SIGNAL_UNSET);
		}
	}
}

static inline void fd_signal_reset(struct fd_signal *signal)
{
	char c;
	bool cas; /* cas result */
	enum ofi_signal_state state;
	int read_rc;

	do {
		cas = ofi_atomic_cas_bool_weak32(&signal->state,
						 OFI_SIGNAL_SET,
						 OFI_SIGNAL_READ_PREPARE);
		if (cas) {
			read_rc = ofi_read_socket(signal->fd[FI_READ_FD], &c,
						  sizeof c);
			if (read_rc == sizeof c) {
#if ENABLE_DEBUG
				assert(ofi_atomic_dec32(&signal->debug_cnt) == 0);
#endif
				ofi_atomic_set32(&signal->state,
						 OFI_SIGNAL_UNSET);
				break;
			} else {
				ofi_atomic_set32(&signal->state, OFI_SIGNAL_SET);

				/* Avoid spinning forever in this highly
				 * unlikely code path.
				 */
				break;
			}
		}

		state = ofi_atomic_get32(&signal->state);

		/* note that this loop also needs to include
		 * OFI_SIGNAL_WRITE_PREPARE, as the writing thread sets
		 * the signal to the socket in _WRITE_PREPARE state. The reading
		 * thread might then race with the writing thread and then
		 * end up here before the state was switched to OFI_SIGNAL_SET.
		 */
	} while (state == OFI_SIGNAL_WRITE_PREPARE || state == OFI_SIGNAL_SET);
}

static inline int fd_signal_poll(struct fd_signal *signal, int timeout)
{
	int ret;

	ret = fi_poll_fd(signal->fd[FI_READ_FD], timeout);
	if (ret < 0)
		return ret;

	return (ret == 0) ? -FI_ETIMEDOUT : 0;
}

static inline int fd_signal_get(struct fd_signal *signal)
{
	return signal->fd[FI_READ_FD];
}

#endif /* _OFI_SIGNAL_H_ */
