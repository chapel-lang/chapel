/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_UNEXP_H_
#define _FI_NETDIR_UNEXP_H_

#include <ndspi.h>

#include "netdir_iface.h"
#include "netdir.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OFI_ND_IS_SERVICE_EVENT(event)	\
	((event) == LARGE_MSG_ACK)

typedef struct nd_unexpected_buf {
	struct nd_msgheader	header;
	union received {
		struct nd_msg_location	locations[];
		char			data[];
	} received_buf;
} nd_unexpected_buf;

typedef struct nd_unexpected_ctx {
	struct nd_ep			*ep;
	struct nd_unexpected_buf	*entry;
} nd_unexpected_ctx;

typedef struct nd_unexpected_entry {
	struct dlist_entry		ep_list;
	struct dlist_entry		srx_list;
	ND2_RESULT			result;
	struct nd_unexpected_buf	*buf;
	struct nd_ep			*ep;
} nd_unexpected_entry;

HRESULT ofi_nd_unexp_init(struct nd_ep *ep);
HRESULT ofi_nd_unexp_fini(struct nd_ep *ep);
void ofi_nd_unexp_event(ND2_RESULT *result);
void ofi_nd_unexp_service_event(ND2_RESULT *result);
void ofi_nd_unexp_match(struct nd_ep *ep);
void ofi_nd_srx_match(struct nd_srx *srx);
HRESULT ofi_nd_unexp_repost(struct nd_ep *ep, struct nd_unexpected_buf *entry);
HRESULT ofi_nd_unexp_run(struct nd_ep *ep);
void ofi_nd_release_unexp_entry(nd_unexpected_entry *unexp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_UNEXP_H_ */

