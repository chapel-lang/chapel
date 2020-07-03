/*
 * Copyright (c) 2014-2016, Cisco Systems, Inc. All rights reserved.
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
#ifndef _USDF_CQ_H_
#define _USDF_CQ_H_

/* exponential backoff settings for fi_cq_sread */
#define SREAD_EXP_BASE 2
#define SREAD_INIT_SLEEP_TIME_US 1
#define SREAD_MAX_SLEEP_TIME_US 5000

int usdf_cq_make_soft(struct usdf_cq *cq);
int usdf_cq_create_cq(struct usdf_cq *cq, struct usd_cq **ucq, int create_fd);
int usdf_check_empty_hard_cq(struct usdf_cq *cq);
int usdf_check_empty_soft_cq(struct usdf_cq *cq);
int usdf_cq_trywait(struct fid *fcq);

void usdf_progress_hard_cq(struct usdf_cq_hard *hcq);

void usdf_cq_post_soft(struct usdf_cq_hard *hcq, void *context,
		size_t len, int prov_errno, uint64_t flags);

#endif /* _USDF_CQ_H_ */
