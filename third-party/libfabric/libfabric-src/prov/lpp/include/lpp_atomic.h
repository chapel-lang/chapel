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

#ifndef _LPP_ATOMIC_H_
#define _LPP_ATOMIC_H_

#include "lpp.h"

enum lpp_atomic_mode {
	LPP_ATOMIC,
	LPP_ATOMIC_FETCH,
	LPP_ATOMIC_CMP,
};

struct lpp_atomic_hdr {
	uint8_t mode;
	uint8_t op;
	uint8_t datatype;
	uint8_t pad[5];
	uint64_t key;
	uint64_t addr;
	uint64_t count;
	uint64_t pad2[4];
} __attribute__((packed));

extern const struct fi_ops_atomic lpp_ep_atomic;

int lpp_fi_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
			enum fi_op op, struct fi_atomic_attr *attr,
			uint64_t flags);
void lpp_atomic_process_msg(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr);
void lpp_atomic_handle_cmpl(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry,
			    struct klpp_msg_hdr *hdr);

#endif // _LPP_ATOMIC_H_
