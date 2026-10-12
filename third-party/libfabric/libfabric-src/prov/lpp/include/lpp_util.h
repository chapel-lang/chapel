/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#ifndef _LPP_UTIL_H_
#define _LPP_UTIL_H_

#include "lpp.h"

/* General rules for locking:
 * - cq/cntr locks are mutually exclusive (not held at the same time)
 * - cq/cntr lock is acquired before umc lock
 * - No further locks are taken after acquiring one of the below:
 *	- domain ep_lock
 *	- domain cntr lock */
static inline void lpp_lock(struct lpp_domain *lpp_domain, ofi_mutex_t *lock)
{
	lpp_domain->lock_acquire(lock);
}

static inline void lpp_unlock(struct lpp_domain *lpp_domain, ofi_mutex_t *lock)
{
	lpp_domain->lock_release(lock);
}

int lpp_translate_errno(int prov_errno);
uint64_t lpp_get_time_ms();

#endif // __LPP_UTIL_H_
