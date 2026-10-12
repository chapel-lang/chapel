/*
 * Copyright (C) 2022-2025 Cornelis Networks.
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

#include "rdma/opx/fi_opx_progress.h"
#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>

__OPX_FORCE_INLINE__
int opx_progress_normalize_core_id(int core_id, int num_cores)
{
	if (core_id < 0) {
		core_id += num_cores;
	}

	if (core_id < 0) {
		core_id = 0;
	}

	if (core_id >= num_cores) {
		core_id = num_cores - 1;
	}

	return core_id;
}

// Affinity for the progress thread based on value for FI_OPX_PROG_AFFINITY
// No affinity is set by default
void opx_progress_set_affinity(pthread_attr_t *thread_attr, char *affinity)
{
	int	  num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	int	  core_id;
	cpu_set_t cpuset;
	char	 *triplet;
	int	  n, start, end, stride;
	int	  set_count = 0;

	if (!affinity) {
		FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "progress thread affinity not set\n");
		return;
	}

	CPU_ZERO(&cpuset);

	for (triplet = affinity; triplet; triplet = strchr(triplet, 'c')) {
		if (triplet[0] == ',') {
			triplet++;
		}

		stride = 1;
		n      = sscanf(triplet, "%d:%d:%d", &start, &end, &stride);
		if (n < 1) {
			continue;
		}

		if (n < 2) {
			end = start;
		}

		if (stride < 1) {
			stride = 1;
		}

		start = opx_progress_normalize_core_id(start, num_cores);
		end   = opx_progress_normalize_core_id(end, num_cores);

		for (core_id = start; core_id <= end; core_id += stride) {
			CPU_SET(core_id, &cpuset);
			set_count++;
		}

		FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "core set [%d:%d:%d] added to progress thread affinity set\n",
			start, end, stride);
	}

	if (set_count) {
		pthread_attr_setaffinity_np(thread_attr, sizeof(cpu_set_t), &cpuset);
	} else {
		FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "progress thread affinity not set due to invalid format\n");
	}
}

__OPX_FORCE_INLINE__
int opx_progress_set_poll_fds(struct fi_opx_cq *cq, int event_fd, struct pollfd *poll_fd)
{
	fi_opx_lock(&cq->lock);

	int i;
	for (i = 0; i < cq->progress.ep_count; i++) {
		poll_fd[(i << 1)].fd	  = cq->progress.ep[i]->hfi->fd_cdev;
		poll_fd[(i << 1)].events  = POLLIN;
		poll_fd[(i << 1)].revents = 0;

		poll_fd[(i << 1) + 1].fd      = cq->progress.ep[i]->rx->shm.segment_fd;
		poll_fd[(i << 1) + 1].events  = POLLIN;
		poll_fd[(i << 1) + 1].revents = 0;
	}

	fi_opx_unlock(&cq->lock);

	poll_fd[i << 1].fd	= event_fd;
	poll_fd[i << 1].events	= POLLIN;
	poll_fd[i << 1].revents = 0;

	return (i << 1) + 1;
}

void *opx_progress_func(void *args)
{
	struct opx_progress_thread *progress_thr = (struct opx_progress_thread *) args;
	struct fi_opx_cq	   *cq		 = progress_thr->cq;
	struct pollfd		    poll_fds[(OPX_CQ_MAX_ENDPOINTS * 2) + 1];
	uint64_t		    event_buf = 0;
	int			    event_fd  = progress_thr->event_fd;
	ssize_t			    read_rc;

	while (1) {
		if (!cq->cq_fid.ops || cq->progress.ep_count == 0) {
			usleep(OPX_PROGRESS_CQ_WAIT_USEC);
			read_rc = read(event_fd, &event_buf, sizeof(event_buf));
			if (read_rc != -1) {
				assert(read_rc == sizeof(OPX_PROGRESS_EVENT_TERMINATE));
				assert(event_buf == OPX_PROGRESS_EVENT_TERMINATE);
				FI_DBG(fi_opx_global.prov, FI_LOG_CQ,
				       "Auto progress thread received terminate signal\n");
				break;
			}
			continue;
		}

		int poll_fd_count = opx_progress_set_poll_fds(cq, event_fd, poll_fds);
		int poll_rc	  = poll(poll_fds, poll_fd_count, -1);

		if (OFI_UNLIKELY(poll_rc < 0)) {
			/* Poll error */
			FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
				"Auto progress thread poll error: errno %d (%s), stopping thread\n", errno,
				strerror(errno));
			break;
		} else if (OFI_UNLIKELY((poll_fds[poll_fd_count - 1].revents & POLLIN) != 0)) {
			/* Parent thread signalled terminate, time to exit */
			read_rc = read(event_fd, &event_buf, sizeof(event_buf));
			assert(read_rc == sizeof(OPX_PROGRESS_EVENT_TERMINATE));
			assert(event_buf == OPX_PROGRESS_EVENT_TERMINATE);

			FI_DBG(fi_opx_global.prov, FI_LOG_CQ, "Auto progress thread received terminate signal\n");

			break;
		} else {
			fi_opx_lock(&cq->lock);
			fi_opx_cq_poll_inline(&cq->cq_fid, NULL, 0, NULL, FI_CQ_FORMAT_UNSPEC, FI_OPX_LOCK_REQUIRED,
					      OFI_RELIABILITY_KIND_ONLOAD, FI_OPX_HDRQ_MASK_RUNTIME, 0UL,
					      OPX_SW_HFI1_TYPE, OPX_IS_CTX_SHARING_ENABLED);
			fi_opx_unlock(&cq->lock);
		}
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_CQ, "Auto progress thread terminating, stopping thread\n");

	pthread_exit(NULL);
}

void *opx_progress_init(struct fi_opx_cq *cq, char *affinity)
{
	struct opx_progress_thread *progress_thr;

	if (posix_memalign((void **) &progress_thr, 64, sizeof(struct opx_progress_thread))) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Auto progress thread creation error: Memory allocation failed\n");
		return NULL;
	}
	progress_thr->cq = cq;

	progress_thr->event_fd = eventfd(0, EFD_NONBLOCK);
	if (progress_thr->event_fd == -1) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Auto progress thread creation error: Error creating eventfd: errno=%d (%s)\n", errno,
			strerror(errno));
		goto init_err;
	}
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	opx_progress_set_affinity(&thread_attr, affinity);

	int thr_crt_rc =
		pthread_create(&progress_thr->p_thread, &thread_attr, opx_progress_func, (void *) progress_thr);
	if (thr_crt_rc) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Auto progress thread creation error: pthread_create returned %d\n", thr_crt_rc);
		goto init_err;
	}
	FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "Auto progress thread started\n");

	return (void *) progress_thr;

init_err:
	free(progress_thr);
	return NULL;
}

void opx_progress_stop(void *progress_thread_ptr)
{
	assert(progress_thread_ptr);
	struct opx_progress_thread *progress_thread = (struct opx_progress_thread *) progress_thread_ptr;

	FI_DBG(fi_opx_global.prov, FI_LOG_CQ, "Stopping Auto progress thread\n");
	ssize_t rc =
		write(progress_thread->event_fd, &OPX_PROGRESS_EVENT_TERMINATE, sizeof(OPX_PROGRESS_EVENT_TERMINATE));

	if (OFI_UNLIKELY(rc != sizeof(OPX_PROGRESS_EVENT_TERMINATE))) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Auto progress thread termination error: Attempting to send terminate signal, write() returned %ld, errno=%d (%s)\n",
			rc, errno, strerror(errno));
	}

	pthread_join(progress_thread->p_thread, NULL);

	close(progress_thread->event_fd);

	free(progress_thread);
}
