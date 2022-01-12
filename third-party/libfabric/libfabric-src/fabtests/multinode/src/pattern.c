/*
 * Copyright (c) 2017-2019 Intel Corporation. All rights reserved.
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
#include <pattern.h>
#include <core.h>
#include <shared.h>

static int broadcast_gather_next(int *cur)
{
	int next;
	if (pm_job.my_rank) 
		return -FI_ENODATA;
	next = *cur + 1;

	if (next >= pm_job.num_ranks)
		return -FI_ENODATA;
	if (next == 0)
		next = 1;

	*cur = next;
	
	return 0;
}

static int broadcast_gather_current(int *cur)
{
	int next;
	if (!pm_job.my_rank) 
		return -FI_ENODATA;
	
	next = *cur + 1;

	if (next > 0)			
		return -FI_ENODATA;

	*cur = next;
	
	return 0; 
}

static int ring_next(int *cur)
{
	if ((pm_job.my_rank == 0 && pm_job.num_ranks - 1 == *cur) ||
		(pm_job.my_rank != 0 && pm_job.my_rank - 1   == *cur))
		return -FI_ENODATA;
		
	if (pm_job.my_rank == 0)
		*cur = pm_job.num_ranks - 1;
	else 			
		*cur = pm_job.my_rank - 1;
	return 0; 
}

static int ring_current(int *cur)
{
	if ((pm_job.my_rank + 1) % pm_job.num_ranks == *cur) 
		return -FI_ENODATA;
	
	*cur = (pm_job.my_rank + 1) % pm_job.num_ranks;
	return 0; 
	
}

static int mesh_next(int *cur)
{
	int next = *cur + 1;

	if (next >= pm_job.num_ranks)
		return -FI_ENODATA;

	*cur = next;
	return 0;
}

struct pattern_ops patterns[] = {
	{
		.name = "ring",
		.next_source = ring_next,
		.next_target = ring_current,
	},
	{
		.name = "gather",
		.next_source = broadcast_gather_next,
		.next_target = broadcast_gather_current,
	},
	{
		.name = "broadcast",
		.next_source = broadcast_gather_current,
		.next_target = broadcast_gather_next,
	},
	{
		.name = "full_mesh",
		.next_source = mesh_next,
		.next_target = mesh_next,
	},
};

const int NUM_TESTS = ARRAY_SIZE(patterns);
