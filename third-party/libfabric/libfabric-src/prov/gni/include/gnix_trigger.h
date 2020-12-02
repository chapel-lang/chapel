/*
 * Copyright (c) 2016 Cray Inc. All rights reserved.
 * Copyright (c) 2016 Los Alamos National Security, LLC. All rights reserved.
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

/*
 * Triggered operations handling.
 */

#ifndef GNIX_TRIGGER_H_
#define GNIX_TRIGGER_H_

#include "gnix.h"
#include "gnix_cntr.h"
#include "gnix_vc.h"

int _gnix_trigger_queue_req(struct gnix_fab_req *req);
void _gnix_trigger_check_cntr(struct gnix_fid_cntr *cntr);

static inline int _gnix_trigger_pending(struct gnix_fid_cntr *cntr)
{
	return dlist_empty(&cntr->trigger_list) ? 0 : 1;
}

#endif /* GNIX_TRIGGER_H */
