/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#ifdef UMR_CACHE
#ifndef _PSMI_VERBS_UMRC_H
#define _PSMI_VERBS_UMRC_H
#ifndef _PSMI_IN_USER_H
#error psm_verbs_umrc.h not meant to be included directly, include psm_user.h instead
#endif

#ifdef PSM_HAVE_REG_MR
#include <stdint.h>
#include <pthread.h>
#include <linux/userfaultfd.h>
#include "psm_lock.h"
#include "utils_queue.h"

#define UFFD_POLL_TIMEOUT_MS	100
#define UMR_CACHE_QUEUE_DEPTH	32

typedef struct psm2_umrc *psm2_umrc_t;
typedef struct psm2_umrc_event *psm2_umrc_event_t;
typedef struct psm2_umr_cache *psm2_umr_cache_t;

struct uffd_handler_thread {
	int pipefd[2];
	pthread_t thread;
};

struct psm2_umrc {
	int fd;
	psm2_ep_t ep;
	void *mr_cache;
	int event_queue;
	int thread;
	struct uffd_handler_thread *uffd_thread;
};

struct psm2_umrc_event {
	uint64_t addr;
	uint64_t length;
	uint32_t event;
	TAILQ_ENTRY(psm2_umrc_event) next;
};

struct psm2_umr_cache {
	int *fd;
	int save_errno;
	int lock;
	uint32_t page_size;
	int event_queue;
	uint32_t queue_cnt;
	struct {
		uint64_t evict;
		uint64_t remap;
		uint64_t remove;
		uint64_t unmap;
	} stats;
	void *mr_cache;
	psm2_umrc_event_t mm_events;
};

int psm3_verbs_umrc_init(psm2_umrc_t umrc, int thread_opt);
void psm3_verbs_umrc_stop(psm2_umrc_t umrc);
void psm3_verbs_umrc_free(psm2_umrc_t umrc);
int psm3_verbs_umrc_register(psm2_umr_cache_t umr_cache, uint64_t addr, uint64_t len);
int psm3_verbs_umrc_unregister(psm2_umr_cache_t umr_cache, uint64_t addr, uint64_t len);
void psm3_verbs_uffd_event(psm2_ep_t ep);

#endif // PSM_HAVE_REG_MR
#endif // _PSMI_VERBS_UMRC_H
#endif // UMR_CACHE
