/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#ifndef __STARPU_TASK_H__
#define __STARPU_TASK_H__

#include <starpu.h>
#include <starpu_data.h>
#include <starpu_task_bundle.h>
#include <errno.h>

#if defined STARPU_USE_CUDA && !defined STARPU_DONT_INCLUDE_CUDA_HEADERS
# include <cuda.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define STARPU_CPU	((1ULL)<<1)
#define STARPU_CUDA	((1ULL)<<3)
#define	STARPU_SPU	((1ULL)<<4),
#define	STARPU_GORDON	((1ULL)<<5)
#define STARPU_OPENCL	((1ULL)<<6)

/* Codelet types */
enum starpu_codelet_type
{
	STARPU_SEQ,
	STARPU_SPMD,
	STARPU_FORKJOIN
};

/* task status */
enum starpu_task_status
{
	STARPU_TASK_INVALID,
	STARPU_TASK_BLOCKED,
	STARPU_TASK_READY,
	STARPU_TASK_RUNNING,
	STARPU_TASK_FINISHED,
	STARPU_TASK_BLOCKED_ON_TAG,
	STARPU_TASK_BLOCKED_ON_TASK,
	STARPU_TASK_BLOCKED_ON_DATA
};

typedef uint64_t starpu_tag_t;

typedef void (*starpu_cpu_func_t)(void **, void*);    /* CPU core */
typedef void (*starpu_cuda_func_t)(void **, void*);   /* NVIDIA CUDA device */
typedef void (*starpu_opencl_func_t)(void **, void*); /* OpenCL CUDA device */
typedef uint8_t starpu_gordon_func_t; /* Cell SPU */

#define STARPU_MULTIPLE_CPU_IMPLEMENTATIONS    ((starpu_cpu_func_t) -1)
#define STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS   ((starpu_cuda_func_t) -1)
#define STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS ((starpu_opencl_func_t) -1)
#define STARPU_MULTIPLE_GORDON_IMPLEMENTATIONS 255

/*
 * A codelet describes the various function
 * that may be called from a worker
 */
struct starpu_task;
struct starpu_codelet
{
	/* where can it be performed ? */
	uint32_t where;
	int (*can_execute)(unsigned workerid, struct starpu_task *task, unsigned nimpl);
	enum starpu_codelet_type type;
	int max_parallelism;

	/* the different implementations of the codelet */
	starpu_cuda_func_t cuda_func STARPU_DEPRECATED;
	starpu_cpu_func_t cpu_func STARPU_DEPRECATED;
	starpu_opencl_func_t opencl_func STARPU_DEPRECATED;
	uint8_t gordon_func STARPU_DEPRECATED;

	starpu_cpu_func_t cpu_funcs[STARPU_MAXIMPLEMENTATIONS];
	starpu_cuda_func_t cuda_funcs[STARPU_MAXIMPLEMENTATIONS];
	starpu_opencl_func_t opencl_funcs[STARPU_MAXIMPLEMENTATIONS];
	starpu_gordon_func_t gordon_funcs[STARPU_MAXIMPLEMENTATIONS];

	/* how many buffers do the codelet takes as argument ? */
	unsigned nbuffers;
	/* which are the access modes for these buffers */
	enum starpu_access_mode modes[STARPU_NMAXBUFS];

	/* performance model of the codelet */
	struct starpu_perfmodel *model;
	/* consumption model of the codelet.
	 * In the case of parallel codelets, accounts for all units. */
	struct starpu_perfmodel *power_model;

	/* statistics collected at runtime: this is filled by StarPU and should
	 * not be accessed directly (use the starpu_display_codelet_stats
	 * function instead for instance). */
	unsigned long per_worker_stats[STARPU_NMAXWORKERS];

	const char *name;
};

#ifdef STARPU_GCC_PLUGIN
typedef struct starpu_codelet starpu_codelet_gcc;
#endif /* STARPU_GCC_PLUGIN */

struct starpu_task
{
	struct starpu_codelet *cl;

	/* arguments managed by the DSM */
	struct starpu_buffer_descr buffers[STARPU_NMAXBUFS] STARPU_DEPRECATED;
	starpu_data_handle_t handles[STARPU_NMAXBUFS];
	void *interfaces[STARPU_NMAXBUFS];

	/* arguments not managed by the DSM are given as a buffer */
	void *cl_arg;
	/* in case the argument buffer has to be uploaded explicitely */
	size_t cl_arg_size;

	/* when the task is done, callback_func(callback_arg) is called */
	void (*callback_func)(void *);
	void *callback_arg;

	unsigned use_tag;
	starpu_tag_t tag_id;

	/* options for the task execution */
	unsigned synchronous; /* if set, a call to push is blocking */
	int priority; /* STARPU_MAX_PRIO = most important
        		: STARPU_MIN_PRIO = least important */

	/* in case the task has to be executed on a specific worker */
	unsigned execute_on_a_specific_worker;
	unsigned workerid;

	/* Bundle including the task */
	starpu_task_bundle_t bundle;

	/* If this flag is set, it is not possible to synchronize with the task
	 * by the means of starpu_task_wait later on. Internal data structures
	 * are only garanteed to be freed once starpu_task_wait is called if
	 * that flag is not set. */
	int detach;

	/* If that flag is set, the task structure will automatically be freed,
	 * either after the execution of the callback if the task is detached,
	 * or during starpu_task_wait otherwise. If this flag is not set,
	 * dynamically allocated data structures will not be freed until
	 * starpu_task_destroy is called explicitely. Setting this flag for a
	 * statically allocated task structure will result in undefined
	 * behaviour. */
	int destroy;

	/* If this flag is set, the task will be re-submitted to StarPU once it
	 * has been executed. This flag must not be set if the destroy flag is
	 * set too. */
	int regenerate;

	enum starpu_task_status status;

	/* This gets filled when profiling is enabled by using
	 * starpu_profiling_status_set */
	struct starpu_task_profiling_info *profiling_info;

	/* Predicted duration of the task in µs. This field is only valid if the
	 * scheduling strategy uses performance models. */
	double predicted;

	/* Predicted data transfer duration for the task in µs. This field is
	 * only valid if the scheduling strategy uses performance models. */
	double predicted_transfer;

	/* This field are provided for the convenience of the scheduler. */
	struct starpu_task *prev;
	struct starpu_task *next;

	unsigned int mf_skip;

	/* this is private to StarPU, do not modify. If the task is allocated
	 * by hand (without starpu_task_create), this field should be set to
	 * NULL. */
	void *starpu_private;
	int magic;
};

/* It is possible to initialize statically allocated tasks with this value.
 * This is equivalent to initializing a starpu_task structure with the
 * starpu_task_init function. */
#define STARPU_TASK_INITIALIZER 			\
{							\
	.cl = NULL,					\
	.cl_arg = NULL,					\
	.cl_arg_size = 0,				\
	.callback_func = NULL,				\
	.callback_arg = NULL,				\
	.priority = STARPU_DEFAULT_PRIO,		\
	.use_tag = 0,					\
	.synchronous = 0,				\
	.execute_on_a_specific_worker = 0,		\
	.bundle = NULL,					\
	.detach = 1,					\
	.destroy = 0,					\
	.regenerate = 0,				\
	.status = STARPU_TASK_INVALID,			\
	.profiling_info = NULL,				\
	.predicted = -1.0,				\
	.predicted_transfer = -1.0,			\
	.starpu_private = NULL,				\
	.magic = 42                  			\
};

/*
 * handle task dependencies: it is possible to associate a task with a unique
 * "tag" and to express dependencies between tasks by the means of those tags
 *
 * To do so, fill the tag_id field with a tag number (can be arbitrary) and set
 * use_tag to 1.
 *
 * If starpu_tag_declare_deps is called with that tag number, the task will not
 * be started until the task which wears the declared dependency tags are
 * complete.
 */

/*
 * WARNING ! use with caution ...
 *  In case starpu_tag_declare_deps is passed constant arguments, the caller
 *  must make sure that the constants are casted to starpu_tag_t. Otherwise,
 *  due to integer sizes and argument passing on the stack, the C compiler
 *  might consider the tag *  0x200000003 instead of 0x2 and 0x3 when calling:
 *      "starpu_tag_declare_deps(0x1, 2, 0x2, 0x3)"
 *  Using starpu_tag_declare_deps_array is a way to avoid this problem.
 */
/* make id depend on the list of ids */
void starpu_tag_declare_deps(starpu_tag_t id, unsigned ndeps, ...);
void starpu_tag_declare_deps_array(starpu_tag_t id, unsigned ndeps, starpu_tag_t *array);

/* task depends on the tasks in task array */
void starpu_task_declare_deps_array(struct starpu_task *task, unsigned ndeps, struct starpu_task *task_array[]);

int starpu_tag_wait(starpu_tag_t id);
int starpu_tag_wait_array(unsigned ntags, starpu_tag_t *id);

/* The application can feed a tag explicitely */
void starpu_tag_notify_from_apps(starpu_tag_t id);

/* To release resources, tags should be freed after use */
void starpu_tag_remove(starpu_tag_t id);

/* Initialize a task structure with default values. */
void starpu_task_init(struct starpu_task *task);

/* Release all the structures automatically allocated to execute the task. This
 * is called implicitely by starpu_task_destroy, but the task structure itself
 * is not freed. This should be used for statically allocated tasks for
 * instance. */
void starpu_task_deinit(struct starpu_task *task);

/* Allocate a task structure and initialize it with default values. Tasks
 * allocated dynamically with starpu_task_create are automatically freed when
 * the task is terminated. If the destroy flag is explicitely unset, the
 * ressources used by the task are freed by calling starpu_task_destroy.
 * */
struct starpu_task *starpu_task_create(void);

/* Free the ressource allocated during the execution of the task and deallocate
 * the task structure itself. This function can be called automatically after
 * the execution of a task by setting the "destroy" flag of the starpu_task
 * structure (default behaviour). Calling this function on a statically
 * allocated task results in an undefined behaviour. */
void starpu_task_destroy(struct starpu_task *task);
int starpu_task_submit(struct starpu_task *task);

/* This function blocks until the task was executed. It is not possible to
 * synchronize with a task more than once. It is not possible to wait
 * synchronous or detached tasks.
 * Upon successful completion, this function returns 0. Otherwise, -EINVAL
 * indicates that the waited task was either synchronous or detached. */
int starpu_task_wait(struct starpu_task *task);

/* This function waits until all the tasks that were already submitted have
 * been executed. */
int starpu_task_wait_for_all(void);

/* This function waits until there is no more ready task. */
int starpu_task_wait_for_no_ready(void);

void starpu_codelet_init(struct starpu_codelet *cl);

void starpu_display_codelet_stats(struct starpu_codelet *cl);

/* Return the task currently executed by the worker, or NULL if this is called
 * either from a thread that is not a task or simply because there is no task
 * being executed at the moment. */
struct starpu_task *starpu_task_get_current(void);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_TASK_H__ */
