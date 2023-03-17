/*
 * Copyright (C) 2023 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1.h"

#include "rdma/fi_direct_eq.h"

//based on psm2 implementation

__OPX_FORCE_INLINE__
int normalize_core_id(int core_id, int num_cores)
{
	if (core_id < 0)
		core_id += num_cores;

	if (core_id < 0)
		core_id = 0;

	if (core_id >= num_cores)
		core_id = num_cores - 1;

	return core_id;
}

// Affinity for the progress thread based on value for FI_OPX_PROG_AFFINITY
// or default value in OPX_DEFAULT_PROG_AFFINITY_STR if not set
int fi_opx_progress_set_affinity(char *affinity)
{
        int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
        int core_id;
        cpu_set_t cpuset;
        char *triplet;
        int n, start, end, stride;
        int set_count = 0;

        if (!affinity) {
                FI_INFO(fi_opx_global.prov, FI_LOG_CQ,
                        "progress thread affinity not set\n");
                return 0;
        }

        CPU_ZERO(&cpuset);

        for (triplet = affinity; triplet; triplet = strchr(triplet, 'c')) {
                if (triplet[0] == ',')
                        triplet++;

                stride = 1;
                n = sscanf(triplet, "%d:%d:%d", &start, &end, &stride);
                if (n < 1)
                        continue;

                if (n < 2)
                        end = start;

                if (stride < 1)
                        stride = 1;

                start = normalize_core_id(start, num_cores);
                end = normalize_core_id(end, num_cores);

                for (core_id = start; core_id <= end; core_id += stride) {
                        CPU_SET(core_id, &cpuset);
                        set_count++;
                }

                FI_INFO(fi_opx_global.prov, FI_LOG_CQ,
                        "core set [%d:%d:%d] added to progress thread affinity set\n",
                        start, end, stride);
        }

        if (set_count)
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
        else
                FI_INFO(fi_opx_global.prov, FI_LOG_CQ,
                        "progress thread affinity not set due to invalid format\n");

        return set_count;
}

// function the progress thread runs
void* fi_opx_progress_func (void *args) {
	struct progress_func_args *func_args = (struct progress_func_args *) args;
	struct fid_cq *cq = func_args->cq;
	char* prog_affinity = func_args->prog_affinity;
	struct fi_opx_progress_track *progress_track = func_args->progress_track;
	int progress_interval = func_args->progress_interval;
	int affinity_set;
	int sleep_usec;
	struct timespec ts;
	affinity_set = fi_opx_progress_set_affinity(prog_affinity);

	if (progress_interval == 0) {
		if (affinity_set) {
			sleep_usec = 1;
		} else {
			sleep_usec = 1000;
		}
	} else {
		sleep_usec = progress_interval;
	}

	ts.tv_sec = 0;
	ts.tv_nsec = 1000;
	while (!cq || !cq->ops) {
		usleep(200);
	}
	int i;
	while (progress_track->keep_running) {
		fi_cq_read(cq, NULL, 0);
		//This is done such that we are not waiting too long for the thread to close out while it is sleeping
		for (i = 0; i<sleep_usec && progress_track->keep_running; i++)
			nanosleep(&ts, NULL);
	}

	pthread_exit(args);
}

// start the progress thread
void fi_opx_start_progress(struct fi_opx_progress_track *progress_track, struct fid_cq *cq, char* prog_affinity, int progress_interval)
{

	progress_track->keep_running = true;

        int err;
	struct progress_func_args *args = malloc(sizeof(struct progress_func_args));

	if (!args) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Unable to create arguments needed for PROGRESS_AUTO");
		goto err;
	}

	args->cq = cq;
	args->progress_interval = progress_interval;
	args->progress_track = progress_track;
	if (prog_affinity)
		args->prog_affinity = prog_affinity;
	else {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Progress affinity incorrectly set\n");
		goto err;
	}

	progress_track->progress_thread = malloc(sizeof(pthread_t));
	if (!progress_track->progress_thread){
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ,
			"Unable to created thread for PROGRESS_AUTO");
		goto err;
	}
        err = pthread_create(progress_track->progress_thread, NULL,
                             fi_opx_progress_func, (void *)args);
        if (err) {
		progress_track->keep_running = false;
		FI_INFO(fi_opx_global.prov, FI_LOG_CQ,
			"pthread_create returns %d\n", err);
		goto err;
        } else {
                FI_INFO(fi_opx_global.prov, FI_LOG_CQ, "progress thread started\n");
        }

	return;

err:
	if (args) {
		free(args);
		args = NULL;
	}
	if (progress_track->progress_thread) {
		free(progress_track->progress_thread);
		progress_track->progress_thread = NULL;
	}
	abort();
}

// Stop the progress thread
void fi_opx_stop_progress(struct fi_opx_progress_track *progress_track)
{
	assert(progress_track && progress_track->progress_thread);

	progress_track->keep_running = false;
	pthread_join(*progress_track->progress_thread, &progress_track->returned_value);

	if (progress_track->returned_value) {
		free(progress_track->returned_value);
		progress_track->returned_value = NULL;
	}

	if (progress_track->progress_thread) {
		free(progress_track->progress_thread);
		progress_track->progress_thread = NULL;
	}
}
