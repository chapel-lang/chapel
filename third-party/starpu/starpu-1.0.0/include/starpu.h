/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
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

#ifndef __STARPU_H__
#define __STARPU_H__

#include <stdlib.h>

#ifndef _MSC_VER
#include <stdint.h>
#else
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#endif

#include <starpu_config.h>

#ifdef STARPU_HAVE_WINDOWS
#include <windows.h>
#endif

#include <starpu_util.h>
#include <starpu_data.h>
#include <starpu_data_interfaces.h>
#include <starpu_data_filters.h>
#include <starpu_perfmodel.h>
#include <starpu_task.h>
#include <starpu_task_list.h>
#include <starpu_scheduler.h>
#include <starpu_expert.h>
#include <starpu_rand.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct starpu_conf
{
	/* which scheduling policy should be used ? (NULL for default) */
	const char *sched_policy_name;
	struct starpu_sched_policy *sched_policy;

	/* number of CPU workers (-1 for default) */
	int ncpus;
	/* number of CUDA GPU workers (-1 for default) */
	int ncuda;
	/* number of GPU OpenCL device workers (-1 for default) */
	int nopencl;
	/* number of Cell's SPUs (-1 for default) */
	int nspus;

	unsigned use_explicit_workers_bindid;
	unsigned workers_bindid[STARPU_NMAXWORKERS];

	unsigned use_explicit_workers_cuda_gpuid;
	unsigned workers_cuda_gpuid[STARPU_NMAXWORKERS];

	unsigned use_explicit_workers_opencl_gpuid;
	unsigned workers_opencl_gpuid[STARPU_NMAXWORKERS];

	/* calibrate performance models, if any (-1 for default) */
	int calibrate;

	/* Create only one combined worker, containing all CPU workers */
	int single_combined_worker;

        /* indicate if the asynchronous copies should be disabled */
	int disable_asynchronous_copy;
};

/* Initialize a starpu_conf structure with default values. */
int starpu_conf_init(struct starpu_conf *conf);

/* Initialization method: it must be called prior to any other StarPU call
 * Default configuration is used if NULL is passed as argument.
 */
int starpu_init(struct starpu_conf *conf) STARPU_WARN_UNUSED_RESULT;

/* Shutdown method: note that statistics are only generated once StarPU is
 * shutdown */
void starpu_shutdown(void);

/* This function returns the number of workers (ie. processing units executing
 * StarPU tasks). The returned value should be at most STARPU_NMAXWORKERS. */
unsigned starpu_worker_get_count(void);
unsigned starpu_combined_worker_get_count(void);

unsigned starpu_cpu_worker_get_count(void);
unsigned starpu_cuda_worker_get_count(void);
unsigned starpu_spu_worker_get_count(void);
unsigned starpu_opencl_worker_get_count(void);

int starpu_asynchronous_copy_disabled();

/* Return the identifier of the thread in case this is associated to a worker.
 * This will return -1 if this function is called directly from the application
 * or if it is some SPU worker where a single thread controls different SPUs. */
int starpu_worker_get_id(void);

int starpu_combined_worker_get_id(void);
int starpu_combined_worker_get_size(void);
int starpu_combined_worker_get_rank(void);

enum starpu_archtype
{
	STARPU_CPU_WORKER, /* CPU core */
	STARPU_CUDA_WORKER, /* NVIDIA CUDA device */
	STARPU_OPENCL_WORKER, /* OpenCL CUDA device */
	STARPU_GORDON_WORKER /* Cell SPU */
};

/* This function returns the type of worker associated to an identifier (as
 * returned by the starpu_worker_get_id function). The returned value indicates
 * the architecture of the worker: STARPU_CPU_WORKER for a CPU core,
 * STARPU_CUDA_WORKER for a CUDA device, and STARPU_GORDON_WORKER for a Cell
 * SPU. The value returned for an invalid identifier is unspecified.  */
enum starpu_archtype starpu_worker_get_type(int id);

/* Returns the number of workers of the type indicated by the argument. A
 * positive (or null) value is returned in case of success, -EINVAL indicates
 * that the type is not valid otherwise. */
int starpu_worker_get_count_by_type(enum starpu_archtype type);

/* Fill the workerids array with the identifiers of the workers that have the
 * type indicated in the first argument. The maxsize argument indicates the
 * size of the workids array. The returned value gives the number of
 * identifiers that were put in the array. -ERANGE is returned is maxsize is
 * lower than the number of workers with the appropriate type: in that case,
 * the array is filled with the maxsize first elements. To avoid such
 * overflows, the value of maxsize can be chosen by the means of the
 * starpu_worker_get_count_by_type function, or by passing a value greater or
 * equal to STARPU_NMAXWORKERS. */
int starpu_worker_get_ids_by_type(enum starpu_archtype type, int *workerids, int maxsize);

/* StarPU associates a unique human readable string to each processing unit.
 * This function copies at most the "maxlen" first bytes of the unique
 * string associated to a worker identified by its identifier "id" into
 * the "dst" buffer. The caller is responsible for ensuring that the
 * "dst" is a valid pointer to a buffer of "maxlen" bytes at least.
 * Calling this function on an invalid identifier results in an unspecified
 * behaviour. */
void starpu_worker_get_name(int id, char *dst, size_t maxlen);

/* This functions returns the device id of the worker associated to an
 *  identifier (as returned by the starpu_worker_get_id() function)
 */
int starpu_worker_get_devid(int id);

#ifdef __cplusplus
}
#endif

#if defined(STARPU_USE_DEPRECATED_API)
#include "starpu_deprecated_api.h"
#endif /* STARPU_USE_DEPRECATED_API */

#endif /* __STARPU_H__ */
