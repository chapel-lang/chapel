/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
 * Copyright (C) 2011  Télécom-SudParis
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#ifndef __STARPU_SCHEDULER_H__
#define __STARPU_SCHEDULER_H__

#include <starpu.h>

#if ! defined(_MSC_VER)
#  include <pthread.h>
#endif

#ifdef STARPU_HAVE_HWLOC
#include <hwloc.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct starpu_task;

struct starpu_machine_topology
{
	unsigned nworkers;

	unsigned ncombinedworkers;

#ifdef STARPU_HAVE_HWLOC
	hwloc_topology_t hwtopology;
#else
	/* We maintain ABI compatibility with and without hwloc */
	void *dummy;
#endif

	unsigned nhwcpus;
        unsigned nhwcudagpus;
        unsigned nhwopenclgpus;

	unsigned ncpus;
	unsigned ncudagpus;
	unsigned nopenclgpus;
	unsigned ngordon_spus;

	/* Where to bind workers ? */
	unsigned workers_bindid[STARPU_NMAXWORKERS];

	/* Which GPU(s) do we use for CUDA ? */
	unsigned workers_cuda_gpuid[STARPU_NMAXWORKERS];

	/* Which GPU(s) do we use for OpenCL ? */
	unsigned workers_opencl_gpuid[STARPU_NMAXWORKERS];
};

/* This structure contains all the methods that implement a scheduling policy.
 * An application may specify which scheduling strategy in the "sched_policy"
 * field of the starpu_conf structure passed to the starpu_init function. */
struct starpu_sched_policy
{
	/* Initialize the scheduling policy. */
	void (*init_sched)(struct starpu_machine_topology *, struct starpu_sched_policy *);

	/* Cleanup the scheduling policy. */
	void (*deinit_sched)(struct starpu_machine_topology *, struct starpu_sched_policy *);

	/* Insert a task into the scheduler. */
	int (*push_task)(struct starpu_task *);

	/* Notify the scheduler that a task was directly pushed to the worker
	 * without going through the scheduler. This method is called when a
	 * task is explicitely assigned to a worker. This method therefore
	 * permits to keep the timing state of the scheduler coherent even
	 * when StarPU bypasses the scheduling strategy. */
	void (*push_task_notify)(struct starpu_task *, int workerid);

	/* Get a task from the scheduler. The mutex associated to the worker is
	 * already taken when this method is called. */
	struct starpu_task *(*pop_task)(void);

	 /* Remove all available tasks from the scheduler (tasks are chained by
	  * the means of the prev and next fields of the starpu_task
	  * structure). The mutex associated to the worker is already taken
	  * when this method is called. */
	struct starpu_task *(*pop_every_task)(void);

	/* This method is called every time a task is starting. (optional) */
	void (*pre_exec_hook)(struct starpu_task *);

	/* This method is called every time a task has been executed. (optional) */
	void (*post_exec_hook)(struct starpu_task *);

	/* Name of the policy (optionnal) */
	const char *policy_name;

	/* Description of the policy (optionnal) */
	const char *policy_description;
};

/* When there is no available task for a worker, StarPU blocks this worker on a
condition variable. This function specifies which condition variable (and the
associated mutex) should be used to block (and to wake up) a worker. Note that
multiple workers may use the same condition variable. For instance, in the case
of a scheduling strategy with a single task queue, the same condition variable
would be used to block and wake up all workers.  The initialization method of a
scheduling strategy (init_sched) must call this function once per worker. */
#if !defined(_MSC_VER)
void starpu_worker_set_sched_condition(int workerid, pthread_cond_t *sched_cond, pthread_mutex_t *sched_mutex);
#endif

/* Check if the worker specified by workerid can execute the codelet. */
int starpu_worker_can_execute_task(unsigned workerid, struct starpu_task *task, unsigned nimpl);

/* The scheduling policy may put tasks directly into a worker's local queue so
 * that it is not always necessary to create its own queue when the local queue
 * is sufficient. If "back" not null, the task is put at the back of the queue
 * where the worker will pop tasks first. Setting "back" to 0 therefore ensures
 * a FIFO ordering. */
int starpu_push_local_task(int workerid, struct starpu_task *task, int back);

/*
 *	Priorities
 */

/* Provided for legacy reasons */
#define STARPU_MIN_PRIO		(starpu_sched_get_min_priority())
#define STARPU_MAX_PRIO		(starpu_sched_get_max_priority())

/* By convention, the default priority level should be 0 so that we can
 * statically allocate tasks with a default priority. */
#define STARPU_DEFAULT_PRIO	0

int starpu_sched_get_min_priority(void);
int starpu_sched_get_max_priority(void);

void starpu_sched_set_min_priority(int min_prio);
void starpu_sched_set_max_priority(int max_prio);

/*
 *	Parallel tasks
 */

/* Register a new combined worker and get its identifier */
int starpu_combined_worker_assign_workerid(int nworkers, int workerid_array[]);
/* Get the description of a combined worker */
int starpu_combined_worker_get_description(int workerid, int *worker_size, int **combined_workerid);
/* Variant of starpu_worker_can_execute_task compatible with combined workers */
int starpu_combined_worker_can_execute_task(unsigned workerid, struct starpu_task *task, unsigned nimpl);

/*
 *	Data prefetching
 */

/* Whether STARPU_PREFETCH was set */
int starpu_get_prefetch_flag(void);
/* Prefetch data for a given task on a given node */
int starpu_prefetch_task_input_on_node(struct starpu_task *task, uint32_t node);

/*
 *	Performance predictions
 */

/* Return the current date */
double starpu_timing_now(void);
/* Returns expected task duration in µs */
double starpu_task_expected_length(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl);
/* Returns an estimated speedup factor relative to CPU speed */
double starpu_worker_get_relative_speedup(enum starpu_perf_archtype perf_archtype);
/* Returns expected data transfer time in µs */
double starpu_task_expected_data_transfer_time(uint32_t memory_node, struct starpu_task *task);
/* Predict the transfer time (in µs) to move a handle to a memory node */
double starpu_data_expected_transfer_time(starpu_data_handle_t handle, unsigned memory_node, enum starpu_access_mode mode);
/* Returns expected power consumption in J */
double starpu_task_expected_power(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl);
/* Returns expected conversion time in ms (multiformat interface only) */
double starpu_task_expected_conversion_time(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl);
/* Return the expected duration of the entire task bundle in �s. */
double starpu_task_bundle_expected_length(starpu_task_bundle_t bundle, enum starpu_perf_archtype arch, unsigned nimpl);
/* Return the time (in µs) expected to transfer all data used within the bundle */
double starpu_task_bundle_expected_data_transfer_time(starpu_task_bundle_t bundle, unsigned memory_node);
/* Return the expected power consumption of the entire task bundle in J. */
double starpu_task_bundle_expected_power(starpu_task_bundle_t bundle, enum starpu_perf_archtype arch, unsigned nimpl);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_SCHEDULER_H__ */
