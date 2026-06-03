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

#ifndef _LPP_CNTR_H_
#define _LPP_CNTR_H_

#include "lpp.h"

// This calculates 1 page (4K) divided by the size of 2 uint64_t counters.
#define	LPP_CNTR_INCREMENT		(4096/(2*sizeof(uint64_t)))

struct lpp_cntr {
	struct fid_cntr		cntr_fid;
	struct fi_cntr_attr	attrs;
	struct lpp_domain	*domain;
	ofi_atomic32_t          ref_count;
	klpp_id_t		id;
	int			valid;
	int			index;
	_Atomic uint64_t	*counter;
	_Atomic	uint64_t	*errcounter;
	ofi_atomic64_t		last_counter;
	ofi_atomic64_t		last_errcounter;
	struct dlist_entry	ep_list;
	ofi_mutex_t		lock;
};

/* Prototypes. */
int lpp_fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr, struct fid_cntr **cntr, void *context);
int lpp_fi_cntr_close(struct fid *cntr);
uint64_t lpp_fi_cntr_read(struct fid_cntr *cntr);
uint64_t lpp_fi_cntr_readerr(struct fid_cntr *cntr);
int lpp_fi_cntr_add(struct fid_cntr *cntr, uint64_t);
int lpp_fi_cntr_adderr(struct fid_cntr *cntr, uint64_t);
int lpp_fi_cntr_set(struct fid_cntr *cntr, uint64_t);
int lpp_fi_cntr_seterr(struct fid_cntr *cntr, uint64_t);
int lpp_fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout);

#endif // _LPP_CNTR_H_
