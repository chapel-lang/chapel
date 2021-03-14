/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2016 Cray Inc. All rights reserved.
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

#include "gnix.h"
#include "gnix_poll.h"

/*******************************************************************************
 * API Functionality.
 ******************************************************************************/
DIRECT_FN int gnix_poll_open(struct fid_domain *domain,
			     struct fi_poll_attr *attr,
			     struct fid_poll **pollset)
{
	return -FI_ENOSYS;
}

/**
 * Poll progress and events across multiple completion queues and counters.
 *
 * @param[in] pollset		the pollset
 * @param[in/out] context	user context values associated with completion
 * queues or counters
 * @param[in] count		number of entries in context
 *
 * @return FI_SUCCESS	upon successfully polling progress
 * @return -FI_ERRNO	upon an error
 * @return -FI_ENOSYS	if this operation is not supported
 */
DIRECT_FN int gnix_poll_poll(struct fid_poll *pollset, void **context,
			     int count)
{
	return -FI_ENOSYS;
}

/**
 * Associates a completions queue or counter with a poll set
 *
 * @param[in] pollset	the pollset
 * @param[in] event_fid	the queue or counter
 * @param[in] flags	flags for the requests
 *
 * @return FI_SUCCESS	upon adding the completion queue or counter
 * @return -FI_ERRNO	upon an error
 * @return -FI_ENOSYS	if this operation is not supported
 */
DIRECT_FN int gnix_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			    uint64_t flags)
{
	return -FI_ENOSYS;
}

/**
 * Removes a completion queue or counter from a poll set.
 *
 * @param[in] pollset	the pollset
 * @param[in] event_fid	the queue or counter
 * @param[in] flags	flags for the requests
 *
 * @return FI_SUCCESS	upon removing the completion queue or counter
 * @return -FI_ERRNO	upon an error
 * @return -FI_ENOSYS	if this operation is not supported
 */
DIRECT_FN int gnix_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			    uint64_t flags)
{
	return -FI_ENOSYS;
}
