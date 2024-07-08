/*
 * Copyright (C) 2022 Cornelis Networks.
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

#ifndef _FI_PROV_OPX_PROGRESS_H_
#define _FI_PROV_OPX_PROGRESS_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include "rdma/opx/fi_opx_compiler.h"

struct fi_opx_progress_track {
	pthread_t				*progress_thread;
	bool					keep_running;
	void					*returned_value;
};

struct progress_func_args {
        struct fid_cq *cq;
        char* prog_affinity;
	struct fi_opx_progress_track *progress_track;
	int progress_interval;
};

// init progress_track
__OPX_FORCE_INLINE__
void fi_opx_progress_init (struct fi_opx_progress_track *progress) {
	progress->progress_thread = NULL;
	progress->returned_value = NULL;
}
void fi_opx_start_progress (struct fi_opx_progress_track *progress_track, struct fid_cq *cq, char* prog_affinity, int progress_interval);
void fi_opx_stop_progress (struct fi_opx_progress_track *progress_track);
#endif
