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

#include <sys/types.h>
#include <linux/userfaultfd.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <time.h>
#ifdef UMR_CACHE_TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __USE_GNU
#include <unistd.h>
#include <sys/mman.h>
#else
#include <psm_user.h>
#include "utils_user.h"
#endif
#include "psm_verbs_umrc.h"

#ifdef PSM_HAVE_REG_MR
// derived from psm_help.h
#define PSMI_ALIGNDOWN(p, P) (((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define PSMI_ALIGNUP(p, P)   (PSMI_ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)), (P)))

int psm3_verbs_umrc_register(psm2_umr_cache_t umr_cache, uint64_t addr, uint64_t len)
{
	int ret = 0;
	struct uffdio_register reg;

	umr_cache->save_errno = 0;
	reg.range.start = PSMI_ALIGNDOWN(addr, umr_cache->page_size);
	reg.range.len = PSMI_ALIGNUP(len, umr_cache->page_size);
	reg.mode = UFFDIO_REGISTER_MODE_MISSING;
	if (-1 == ioctl(*umr_cache->fd, UFFDIO_REGISTER, &reg))
		ret = umr_cache->save_errno = errno;
	_HFI_MMDBG("mr_cache=%p fd=%d addr=0x%lx range.start=0x%llx len=0x%lx range.len=0x%llx errno=%d\n",
				umr_cache->mr_cache, *umr_cache->fd, addr, reg.range.start, len, reg.range.len, ret);
	return ret;
}

int psm3_verbs_umrc_unregister(psm2_umr_cache_t umr_cache, uint64_t addr, uint64_t len)
{
	int ret = 0;
	struct uffdio_range range;

	umr_cache->save_errno = 0;
	range.start = PSMI_ALIGNDOWN(addr, umr_cache->page_size);
	range.len = PSMI_ALIGNUP(len, umr_cache->page_size);
	if (-1 == ioctl(*umr_cache->fd, UFFDIO_UNREGISTER, &range))
		ret = umr_cache->save_errno = errno;
	_HFI_MMDBG("mr_cache=%p fd=%d addr=0x%lx range.start=0x%llx len=0x%lx range.len=0x%llx errno=%d\n",
				umr_cache->mr_cache, *umr_cache->fd, addr, range.start, len, range.len, ret);
	return ret;
}

static inline void poll_uffd_events(psm2_ep_t ep, int timeout)
{
	struct pollfd puffd[PSMI_MAX_QPS];
	psm2_ep_t pep[PSMI_MAX_QPS];
	int num_ep = 0;
	psm2_ep_t first;
	int ret;
	int i;

	first = ep;
	do {
		puffd[num_ep].fd = ep->verbs_ep.umrc.fd;
		puffd[num_ep].events = POLLIN;
		puffd[num_ep].revents = 0;
		pep[num_ep++] = ep;
		ep = ep->mctxt_next;
	} while (ep != first);

	ret = poll(puffd, num_ep, timeout);
	if_pf(ret < 0) {
		if (errno == EINTR)
			_HFI_MMDBG("got signal, keep polling\n");
		else
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"Receive thread poll() error: %s", strerror(errno));
	} else if_pf (ret > 0) {
		for (i=0; i < num_ep; i++) {
			if (puffd[i].revents & POLLIN)
				psm3_verbs_uffd_event(pep[i]);
		}
	}
}

static void *umrc_uffd_handler(void *arg)
{
	psm2_umrc_t umrc = (struct psm2_umrc*)arg;
	struct pollfd fds[1];
	int fd_pipe = umrc->uffd_thread->pipefd[0];
	int timeout = UFFD_POLL_TIMEOUT_MS;
	int ret;

	fds[0].fd = fd_pipe;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int nfds = sizeof(fds)/sizeof(fds[0]);
	while (1) {
		ret = poll(&fds[0], nfds, 0);
		if_pf(ret < 0) {
			if (errno == EINTR)
				_HFI_MMDBG("got signal, keep polling\n");
			else
				break;
		}
		if (fds[0].revents) {
			_HFI_MMDBG("close thread: revents=0x%x\n", fds[0].revents);
			break;
		}
		poll_uffd_events((psm2_ep_t)umrc->ep, timeout);
	}
	close(umrc->fd);
	umrc->fd = 0;
	close(fd_pipe);
	PSM2_LOG_MSG("leaving");
	pthread_exit(NULL);
}

void psm3_verbs_umrc_stop(psm2_umrc_t umrc)
{
	if (umrc->uffd_thread) {
		uint64_t emsg = 0;
		if (write(umrc->uffd_thread->pipefd[1], (const void *)&emsg,
				sizeof(uint64_t)) == -1 ||
				close(umrc->uffd_thread->pipefd[1]) == -1) {
			_HFI_MMDBG("unable to close pipe to receive thread cleanly\n");
			pthread_cancel(umrc->uffd_thread->thread); // try to cancel
		}
		pthread_join(umrc->uffd_thread->thread, NULL);
		psmi_free(umrc->uffd_thread);
	}
	if (!umrc->thread) {
		_HFI_MMDBG("umrc wants to close fd=%d\n", umrc->fd);
		close(umrc->fd);
		umrc->fd = 0;
	}
	if (umrc->event_queue)
		psm3_verbs_umrc_free(umrc);
}

int psm3_verbs_umrc_init(psm2_umrc_t umrc, int thread_opt)
{
	int ret = ENXIO;
	struct uffdio_api api;

	if (thread_opt) {
		umrc->uffd_thread = (struct uffd_handler_thread*)psmi_calloc(PSMI_EP_NONE, UNDEFINED,
						sizeof(struct uffd_handler_thread), 1);
		if (! umrc->uffd_thread) {
			_HFI_ERROR("Couldn't allocate memory for uffd_handler_thread. Cache disabled\n");
			goto fail;
		}
		if (pipe(umrc->uffd_thread->pipefd) != 0) {
			psm3_handle_error(umrc->ep, PSM2_EP_DEVICE_FAILURE,
						"Cannot create a pipe for uffd thread: %s\n",
						strerror(errno));
			goto fail_pipe;
		}
		ret = pthread_create(&umrc->uffd_thread->thread, NULL, umrc_uffd_handler, umrc);
		if (ret)
			goto fail_thread;
		goto done;
	} else
		umrc->uffd_thread = NULL;

	umrc->fd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
	if (umrc->fd < 0) {
		return errno;
	}
	api.api = UFFD_API;
	api.features = UFFD_FEATURE_EVENT_UNMAP | UFFD_FEATURE_EVENT_REMOVE | UFFD_FEATURE_EVENT_REMAP;
	ret = ioctl(umrc->fd, UFFDIO_API, &api);
	if (ret < 0) {
		ret = errno;
		goto fail;
	}
	if (api.api != UFFD_API) {
		ret = ENOSYS;
		goto fail;
	}
	_HFI_MMDBG("uffd fd=%d\n", umrc->fd);
done:
	return 0;
fail_thread:
	close(umrc->uffd_thread->pipefd[0]);
	close(umrc->uffd_thread->pipefd[1]);
fail_pipe:
	psmi_free(umrc->uffd_thread);
fail:
	close(umrc->fd);
	umrc->fd = 0;
	return ret;
}
#endif // PSM_HAVE_REG_MR
#endif // UMR_CACHE
