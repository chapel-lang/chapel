/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2016 Cray Inc.  All rights reserved.
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

#ifndef _GNIX_WAIT_H_
#define _GNIX_WAIT_H_

#include <pthread.h>
#include <rdma/fi_eq.h>
#include <ofi_list.h>

#define WAIT_SUB (FI_LOG_CQ | FI_LOG_EQ)

enum {
	WAIT_READ,
	WAIT_WRITE
};

struct gnix_wait_entry {
	struct fid *wait_obj;
	struct slist_entry entry;
};

struct gnix_fid_wait {
	struct fid_wait wait;
	struct gnix_fid_fabric *fabric;

	enum fi_cq_wait_cond cond_type;
	enum fi_wait_obj type;

	union {
		int fd[2];
		struct {
			pthread_mutex_t mutex;
			pthread_cond_t cond;
		};
	};

	struct slist set;
};

extern uint32_t gnix_wait_thread_sleep_time;

/*
 * API Functions
 */
int gnix_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset);
int gnix_wait_close(struct fid *wait);
int gnix_wait_wait(struct fid_wait *wait, int timeout);

/*
 * Exposed internal functions.
 */
int _gnix_wait_set_add(struct fid_wait *wait, struct fid *wait_obj);
int _gnix_wait_set_remove(struct fid_wait *wait, struct fid *wait_obj);
int _gnix_get_wait_obj(struct fid_wait *wait, void *arg);
void _gnix_signal_wait_obj(struct fid_wait *wait);

#endif
