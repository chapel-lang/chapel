/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#ifndef __WORKERS_H__
#define __WORKERS_H__

#include <starpu.h>
#include <starpu_scheduler.h>
#include <common/config.h>
#include <pthread.h>
#include <common/timing.h>
#include <common/fxt.h>
#include <core/jobs.h>
#include <core/perfmodel/perfmodel.h>
#include <core/sched_policy.h>
#include <core/topology.h>
#include <core/errorcheck.h>


#ifdef STARPU_HAVE_HWLOC
#include <hwloc.h>
#endif

#ifdef STARPU_USE_CUDA
#include <drivers/cuda/driver_cuda.h>
#endif

#ifdef STARPU_USE_OPENCL
#include <drivers/opencl/driver_opencl.h>
#endif

#ifdef STARPU_USE_GORDON
#include <drivers/gordon/driver_gordon.h>
#endif

#include <drivers/cpu/driver_cpu.h>

#include <datawizard/datawizard.h>

#include <starpu_parameters.h>

struct _starpu_worker
{
	struct _starpu_machine_config *config;
        pthread_mutex_t mutex;
	enum starpu_archtype arch; /* what is the type of worker ? */
	uint32_t worker_mask; /* what is the type of worker ? */
	enum starpu_perf_archtype perf_arch; /* in case there are different models of the same arch */
	pthread_t worker_thread; /* the thread which runs the worker */
	int devid; /* which cpu/gpu/etc is controlled by the workker ? */
	int bindid; /* which cpu is the driver bound to ? (logical index) */
	int workerid; /* uniquely identify the worker among all processing units types */
	int combined_workerid; /* combined worker currently using this worker */
	int current_rank; /* current rank in case the worker is used in a parallel fashion */
	int worker_size; /* size of the worker in case we use a combined worker */
        pthread_cond_t ready_cond; /* indicate when the worker is ready */
	unsigned memory_node; /* which memory node is associated that worker to ? */
	pthread_cond_t *sched_cond; /* condition variable used when the worker waits for tasks. */
	pthread_mutex_t *sched_mutex; /* mutex protecting sched_cond */
	struct starpu_task_list local_tasks; /* this queue contains tasks that have been explicitely submitted to that queue */
	struct starpu_task *current_task; /* task currently executed by this worker */
	struct _starpu_worker_set *set; /* in case this worker belongs to a set */
	struct _starpu_job_list *terminated_jobs; /* list of pending jobs which were executed */
	unsigned worker_is_running;
	unsigned worker_is_initialized;
	enum _starpu_worker_status status; /* what is the worker doing now ? (eg. CALLBACK) */
	char name[48];
	char short_name[10];

#ifdef __GLIBC__
	cpu_set_t initial_cpu_set;
	cpu_set_t current_cpu_set;
#endif /* __GLIBC__ */
#ifdef STARPU_HAVE_HWLOC
	hwloc_cpuset_t initial_hwloc_cpu_set;
	hwloc_cpuset_t current_hwloc_cpu_set;
#endif
};

struct _starpu_combined_worker
{
	enum starpu_perf_archtype perf_arch; /* in case there are different models of the same arch */
	uint32_t worker_mask; /* what is the type of workers ? */
	int worker_size;
	unsigned memory_node; /* which memory node is associated that worker to ? */
	int combined_workerid[STARPU_NMAXWORKERS];

#ifdef __GLIBC__
	cpu_set_t cpu_set;
#endif /* __GLIBC__ */
#ifdef STARPU_HAVE_HWLOC
	hwloc_cpuset_t hwloc_cpu_set;
#endif
};

/* in case a single CPU worker may control multiple
 * accelerators (eg. Gordon for n SPUs) */
struct _starpu_worker_set
{
        pthread_mutex_t mutex;
	pthread_t worker_thread; /* the thread which runs the worker */
	unsigned nworkers;
	unsigned joined; /* only one thread may call pthread_join*/
	void *retval;
	struct _starpu_worker *workers;
        pthread_cond_t ready_cond; /* indicate when the set is ready */
	unsigned set_is_initialized;
};

struct _starpu_machine_config
{
	struct starpu_machine_topology topology;

#ifdef STARPU_HAVE_HWLOC
	int cpu_depth;
#endif

	/* Where to bind workers ? */
	int current_bindid;

	/* Which GPU(s) do we use for CUDA ? */
	int current_cuda_gpuid;

	/* Which GPU(s) do we use for OpenCL ? */
	int current_opencl_gpuid;

	/* Basic workers : each of this worker is running its own driver and
	 * can be combined with other basic workers. */
	struct _starpu_worker workers[STARPU_NMAXWORKERS];

	/* Combined workers: these worker are a combination of basic workers
	 * that can run parallel tasks together. */
	struct _starpu_combined_worker combined_workers[STARPU_NMAX_COMBINEDWORKERS];

	/* This bitmask indicates which kinds of worker are available. For
	 * instance it is possible to test if there is a CUDA worker with
	 * the result of (worker_mask & STARPU_CUDA). */
	uint32_t worker_mask;

        /* either the user given configuration passed to starpu_init or a default configuration */
	struct starpu_conf *conf;
	/* set to 1 if no conf has been given by the user, it
	 * indicates the memory allocated for the default
	 * configuration should be freed on shutdown */
	int default_conf;

	/* this flag is set until the runtime is stopped */
	unsigned running;
};

/* Has starpu_shutdown already been called ? */
unsigned _starpu_machine_is_running(void);

/* Check if there is a worker that may execute the task. */
uint32_t _starpu_worker_exists(struct starpu_task *);

/* Is there a worker that can execute CUDA code ? */
uint32_t _starpu_can_submit_cuda_task(void);

/* Is there a worker that can execute CPU code ? */
uint32_t _starpu_can_submit_cpu_task(void);

/* Is there a worker that can execute OpenCL code ? */
uint32_t _starpu_can_submit_opencl_task(void);

/* Check whether there is anything that the worker should do instead of
 * sleeping (waiting on something to happen). */
unsigned _starpu_worker_can_block(unsigned memnode);

/* This function must be called to block a worker. It puts the worker in a
 * sleeping state until there is some event that forces the worker to wake up.
 * */
void _starpu_block_worker(int workerid, pthread_cond_t *cond, pthread_mutex_t *mutex);

/* The _starpu_worker structure describes all the state of a StarPU worker.
 * This function sets the pthread key which stores a pointer to this structure.
 * */
void _starpu_set_local_worker_key(struct _starpu_worker *worker);

/* Returns the _starpu_worker structure that describes the state of the
 * current worker. */
struct _starpu_worker *_starpu_get_local_worker_key(void);

/* Returns the _starpu_worker structure that describes the state of the
 * specified worker. */
struct _starpu_worker *_starpu_get_worker_struct(unsigned id);

struct _starpu_combined_worker *_starpu_get_combined_worker_struct(unsigned id);

int _starpu_is_initialized(void);

/* Returns the structure that describes the overall machine configuration (eg.
 * all workers and topology). */
struct _starpu_machine_config *_starpu_get_machine_config(void);

/* Retrieve the status which indicates what the worker is currently doing. */
enum _starpu_worker_status _starpu_worker_get_status(int workerid);

/* Change the status of the worker which indicates what the worker is currently
 * doing (eg. executing a callback). */
void _starpu_worker_set_status(int workerid, enum _starpu_worker_status status);

/* TODO move */
unsigned _starpu_execute_registered_progression_hooks(void);

#endif // __WORKERS_H__
