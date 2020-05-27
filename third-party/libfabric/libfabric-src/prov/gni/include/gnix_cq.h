/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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

#ifndef _GNIX_CQ_H_
#define _GNIX_CQ_H_

#include <ofi.h>

#include "gnix_progress.h"
#include "gnix_queue.h"
#include "gnix_wait.h"
#include "gnix_util.h"
#include <ofi_list.h>
#include <stdbool.h>

#define GNIX_CQ_DEFAULT_FORMAT struct fi_cq_entry
#define GNIX_CQ_DEFAULT_SIZE   256
#define GNIX_CQ_MAX_ERR_DATA_SIZE 64

/* forward declaration */
struct gnix_fid_ep;

struct gnix_cq_entry {
	void *the_entry;
	fi_addr_t src_addr;
	struct slist_entry item;
};

struct gnix_fid_cq {
	struct fid_cq cq_fid;
	struct gnix_fid_domain *domain;

	struct gnix_queue *events;
	struct gnix_queue *errors;

	struct fi_cq_attr attr;
	size_t entry_size;

	struct fid_wait *wait;

	fastlock_t lock;
	struct gnix_reference ref_cnt;

	struct gnix_prog_set pset;

	bool requires_lock;
	char err_data[GNIX_CQ_MAX_ERR_DATA_SIZE];
};

ssize_t _gnix_cq_add_event(struct gnix_fid_cq *cq, struct gnix_fid_ep *ep,
			   void *op_context, uint64_t flags, size_t len,
			   void *buf, uint64_t data, uint64_t tag,
			   fi_addr_t src_addr);

ssize_t _gnix_cq_add_error(struct gnix_fid_cq *cq, void *op_context,
			  uint64_t flags, size_t len, void *buf,
			  uint64_t data, uint64_t tag, size_t olen,
			  int err, int prov_errno, void *err_data,
			  size_t err_data_size);

int _gnix_cq_poll_obj_add(struct gnix_fid_cq *cq, void *obj,
			  int (*prog_fn)(void *data));
int _gnix_cq_poll_obj_rem(struct gnix_fid_cq *cq, void *obj,
			  int (*prog_fn)(void *data));

#endif
