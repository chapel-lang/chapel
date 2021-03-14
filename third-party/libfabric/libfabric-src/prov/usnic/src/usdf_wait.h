/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _USDF_WAIT_H_
#define _USDF_WAIT_H_

#include "ofi_list.h"

struct usdf_wait {
	struct fid_wait		wait_fid;
	struct usdf_fabric	*wait_fabric;

	enum fi_wait_obj	wait_obj;
	union {
		ofi_epoll_t epfd;
		struct fi_mutex_cond mutex_cond;
	} object;

	ofi_atomic32_t		wait_refcnt;

	fastlock_t		lock;
	struct dlist_entry	list;
};

#define wait_ftou(FWT) container_of(FWT, struct usdf_wait, wait_fid)
#define wait_fidtou(FWT) container_of(FWT, struct usdf_wait, wait_fid.fid)

int usdf_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		struct fid_wait **waitset);
int usdf_trywait(struct fid_fabric *fabric, struct fid **fids, int count);

#endif
