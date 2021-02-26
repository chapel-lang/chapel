/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
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

#ifndef PROV_GNI_INCLUDE_GNIX_SMRN_H
#define PROV_GNI_INCLUDE_GNIX_SMRN_H

#include "include/ofi_list.h"
#include "include/ofi_lock.h"
#include "config.h"

#include "gnix_mr_notifier.h"

/**
 * @brief shared memory registration notifier
 *
 * @var   lock      Only used for set up and tear down (no guarantees
 *                  if reading or writing while setting up or tearing down)
 */
struct gnix_smrn {
	fastlock_t lock;
	struct gnix_mr_notifier *notifier;
	struct dlist_entry rq_head;
	int references;
};

struct gnix_smrn_rq {
	fastlock_t lock;
	struct dlist_entry list;
	struct dlist_entry entry;
};

struct gnix_smrn_context {
	struct gnix_smrn_rq *rq;
	uint64_t cookie;
	struct dlist_entry entry;
};

int _gnix_smrn_init(void);

/**
 * @brief open the prepare for notifications
 *
 * @param[in,out] k     Empty and initialized gnix_smrn struct
 * @return              FI_SUCESSS on success
 *                      -FI_EBUSY if device already open
 *                      -FI_ENODATA if user delta unavailable
 *                      -fi_errno or -errno on other failures
 */
int _gnix_smrn_open(struct gnix_smrn **smrn);

/**
 * @brief close the kdreg device and zero the notifier
 *
 * @param[in] k         gnix_smrn struct
 * @return              FI_SUCESSS on success
 *                      -fi_errno or -errno on other failures
 */
int _gnix_smrn_close(struct gnix_smrn *mrn);

/**
 * @brief monitor a memory region
 *
 * @param[in] k         gnix_smrn struct
 * @param[in] addr      address of memory region to monitor
 * @param[in] len       length of memory region
 * @param[in] cookie    user identifier associated with the region
 * @return              FI_SUCESSS on success
 *                      -fi_errno or -errno on failure
 */
int _gnix_smrn_monitor(struct gnix_smrn *smrn,
	struct gnix_smrn_rq *rq,
	void *addr,
	uint64_t len,
	uint64_t cookie,
	struct gnix_smrn_context *context);

/**
 * @brief stop monitoring a memory region
 *
 * @param[in]  k        gnix_smrn struct
 * @param[out] cookie   user identifier for notification event
 * @return              FI_SUCESSS on success
 *                      -fi_errno or -errno on failure
 */
int _gnix_smrn_unmonitor(struct gnix_smrn *smrn,
	uint64_t cookie,
	struct gnix_smrn_context *context);

/**
 * @brief get a monitoring event
 *
 * @param[in]  k        gnix_smrn struct
 * @param[out] buf      buffer to write event data
 * @param[in]  len      length of buffer
 * @return              Number of bytes read on success
 *                      -FI_EINVAL if invalid arguments
 *                      -FI_EAGAIN if nothing to read
 *                      -fi_errno or -errno on failure
 */
int _gnix_smrn_get_event(struct gnix_smrn *smrn,
	struct gnix_smrn_rq *rq,
	struct gnix_smrn_context **context);

#endif
