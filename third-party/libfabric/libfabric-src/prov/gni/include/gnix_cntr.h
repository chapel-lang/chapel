/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef _GNIX_CNTR_H_
#define _GNIX_CNTR_H_

#include <ofi.h>

#include "gnix.h"
#include "gnix_progress.h"
#include "gnix_wait.h"
#include "gnix_util.h"

struct gnix_fid_cntr {
	struct fid_cntr cntr_fid;
	struct gnix_fid_domain *domain;
	struct fid_wait *wait;
	struct fi_cntr_attr attr;
	ofi_atomic32_t cnt;
	ofi_atomic32_t cnt_err;
	struct gnix_reference ref_cnt;
	struct dlist_entry trigger_list;
	fastlock_t trigger_lock;
	struct gnix_prog_set pset;
	bool requires_lock;
};

/**
 * @brief              Increment event counter associated with a gnix_fid counter
 *                     object
 * @param[in] cntr     pointer to previously allocated gnix_fid_cntr structure
 * @return             FI_SUCCESS on success, -FI_EINVAL on invalid argument
 */
int _gnix_cntr_inc(struct gnix_fid_cntr *cntr);

/**
 * @brief              Increment error event counter associated with a gnix_fid counter
 *                     object
 * @param[in] cntr     pointer to previously allocated gnix_fid_cntr structure
 * @return             FI_SUCCESS on success, -FI_EINVAL on invalid argument
 */
int _gnix_cntr_inc_err(struct gnix_fid_cntr *cntr);

/**
 * @brief              Add an object to the list progressed when fi_cntr_read
 *                     and related functions are called.
 * @param[in] cntr     pointer to previously allocated gnix_fid_cntr structure
 * @param[in] obj      pointer to object to add to the progress list.
 * @param[in] prog_fn  object progress function
 * @return             FI_SUCCESS on success, -FI_EINVAL on invalid argument
 */
int _gnix_cntr_poll_obj_add(struct gnix_fid_cntr *cntr, void *obj,
			    int (*prog_fn)(void *data));

/**
 * @brief              Remove an object from the list progressed when
 *                     fi_cntr_read and related functions are called.
 * @param[in] cntr     pointer to previously allocated gnix_fid_cntr structure
 * @param[in] obj      pointer to previously added object
 * @param[in] prog_fn  object progress function
 * @return             FI_SUCCESS on success, -FI_EINVAL on invalid argument
 */
int _gnix_cntr_poll_obj_rem(struct gnix_fid_cntr *cntr, void *obj,
			    int (*prog_fn)(void *data));

#endif
