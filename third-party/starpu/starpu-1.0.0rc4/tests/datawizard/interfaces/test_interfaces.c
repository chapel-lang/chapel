/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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
#include <starpu.h>
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif

/* XXX Why cant we dereference a handle without this one ? */
#include <core/sched_policy.h>

#include <assert.h>

#include "test_interfaces.h"
#include "../../helper.h"

/*
 * This is definitely note thrad-safe.
 */
static struct test_config *current_config;

/* TODO :
- OpenCL to OpenCL support
*/

/*
 * Users do not know about this enum. They only know that SUCCESS is 0, and
 * FAILURE is 1. Therefore, the values of SUCCESS and FAILURE shall not be
 * changed.
 */
enum exit_code
{
	SUCCESS                 = 0,
	FAILURE                 = 1,
	UNTESTED                = 2,
	TASK_CREATION_FAILURE   = 3,
	TASK_SUBMISSION_FAILURE = 4
};

static char *
enum_to_string(int exit_code)
{
	switch (exit_code)
	{
		case SUCCESS:
			return "Success";
		case FAILURE:
			return "Failure";
		case UNTESTED:
			return "Untested";
		case TASK_CREATION_FAILURE:
			return "Task creation failed";
		case TASK_SUBMISSION_FAILURE:
			return "Task submission failed";
		default:
			assert(0);
	}
}

struct data_interface_test_summary
{
	int success;

	/* Copy methods */
#ifdef STARPU_USE_CPU
	int cpu_to_cpu;
#endif
#ifdef STARPU_USE_CUDA
	int cpu_to_cuda;
	int cuda_to_cuda;
	int cuda_to_cpu;
	int cpu_to_cuda_async;
	int cuda_to_cpu_async;
	int cuda_to_cuda_async;
#endif
#ifdef STARPU_USE_OPENCL
	int cpu_to_opencl;
	int opencl_to_cpu;
	int cpu_to_opencl_async;
	int opencl_to_cpu_async;
#endif

	/* Other stuff */
	int compare;
#ifdef STARPU_USE_CPU
	int handle_to_pointer;
#endif
};

void data_interface_test_summary_print(FILE *f,
				       struct data_interface_test_summary *s)
{
	if (!f)
		f = stderr;

	FPRINTF(f, "%s : %s\n",
		current_config->name, enum_to_string(s->success));
	FPRINTF(f, "Asynchronous :\n");
#ifdef STARPU_USE_CUDA
	FPRINTF(f, "\tCPU    -> CUDA   : %s\n",
		enum_to_string(s->cpu_to_cuda_async));
	FPRINTF(f, "\tCUDA   -> CUDA   : %s\n",
			enum_to_string(s->cuda_to_cuda_async));
	FPRINTF(f, "\tCUDA   -> CPU    : %s\n",
			enum_to_string(s->cuda_to_cpu_async));
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	FPRINTF(f, "\tCPU    -> OpenCl : %s\n",
		enum_to_string(s->cpu_to_opencl_async));
	FPRINTF(f, "\tOpenCl -> CPU    : %s\n",
		enum_to_string(s->opencl_to_cpu_async));
#endif /* !STARPU_USE_OPENCL */

	FPRINTF(f, "Synchronous :\n");
#ifdef STARPU_USE_CUDA
	FPRINTF(f, "\tCPU    -> CUDA   ; %s\n",
		enum_to_string(s->cpu_to_cuda));
	FPRINTF(f, "\tCUDA   -> CUDA   : %s\n",
		enum_to_string(s->cuda_to_cuda));
	FPRINTF(f, "\tCUDA   -> CPU    : %s\n",
		enum_to_string(s->cuda_to_cpu));
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	FPRINTF(f, "\tCPU    -> OpenCl : %s\n",
		enum_to_string(s->cpu_to_opencl));
	FPRINTF(f, "\tOpenCl -> CPU    : %s\n",
		enum_to_string(s->opencl_to_cpu));
#endif /* !STARPU_USE_OPENCL */
#ifdef STARPU_USE_CPU
	FPRINTF(f, "CPU    -> CPU    : %s\n",
		enum_to_string(s->cpu_to_cpu));
	FPRINTF(f, "handle_to_pointer() : %s\n",
		enum_to_string(s->handle_to_pointer));
#endif /* !STARPU_USE_CPU */
	FPRINTF(f, "compare()        : %s\n",
		enum_to_string(s->compare));
}

int
data_interface_test_summary_success(data_interface_test_summary *s)
{
	return s->success;
}

enum operation
{
	CPU_TO_CPU,
#ifdef STARPU_USE_CUDA
	CPU_TO_CUDA,
	CUDA_TO_CUDA,
	CUDA_TO_CPU,
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	CPU_TO_OPENCL,
	OPENCL_TO_CPU
#endif /* !STARPU_USE_OPENCL */
};

static int*
get_field(struct data_interface_test_summary *s, int async, enum operation op)
{
	switch (op)
	{
#ifdef STARPU_USE_CPU
	case CPU_TO_CPU:
		return &s->cpu_to_cpu;
#endif
#ifdef STARPU_USE_CUDA
	case CPU_TO_CUDA:
		return async?&s->cpu_to_cuda_async:&s->cpu_to_cuda;
	case CUDA_TO_CUDA:
		return async?&s->cuda_to_cuda_async:&s->cuda_to_cuda;
	case CUDA_TO_CPU:
		return async?&s->cuda_to_cpu_async:&s->cuda_to_cpu;
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	case CPU_TO_OPENCL:
		return async?&s->cpu_to_opencl_async:&s->cpu_to_opencl;
	case OPENCL_TO_CPU:
		return async?&s->opencl_to_cpu_async:&s->opencl_to_cpu;
#endif /* !STARPU_USE_OPENCL */
		default:
			STARPU_ASSERT(0);
	}
	/* that instruction should never be reached */
	return NULL;
}

static void
set_field(struct data_interface_test_summary *s, int async,
	  enum operation op, int ret)
{
	int *field = get_field(s, async, op);
	switch (ret)
	{
		case SUCCESS:
			*field = SUCCESS;
			break;
		case FAILURE:
			*field = FAILURE;
			s->success = FAILURE;
			break;
		case UNTESTED:
			*field = UNTESTED;
			break;
		case TASK_CREATION_FAILURE:
			*field = TASK_CREATION_FAILURE;
			break;
		case TASK_SUBMISSION_FAILURE:
			*field = TASK_SUBMISSION_FAILURE;
			break;
		default:
			STARPU_ASSERT(0);
	}
}

static struct data_interface_test_summary summary =
{
#ifdef STARPU_USE_CPU
	.cpu_to_cpu            = UNTESTED,
	.compare               = UNTESTED,
#endif
#ifdef STARPU_USE_CUDA
	.cpu_to_cuda           = UNTESTED,
	.cuda_to_cuda          = UNTESTED,
	.cuda_to_cpu           = UNTESTED,
	.cpu_to_cuda_async     = UNTESTED,
	.cuda_to_cpu_async     = UNTESTED,
	.cuda_to_cuda_async    = UNTESTED,
#endif
#ifdef STARPU_USE_OPENCL
	.cpu_to_opencl         = UNTESTED,
	.opencl_to_cpu         = UNTESTED,
	.cpu_to_opencl_async   = UNTESTED,
	.opencl_to_cpu_async   = UNTESTED,
#endif
#ifdef STARPU_USE_CPU
	.handle_to_pointer     = UNTESTED,
#endif
	.success               = SUCCESS
};


/*
 * This variable has to be either -1 or 1.
 * The kernels should check that the ith value stored in the data interface is
 * equal to i, if factor == 1, or -i, if factor == -1.
 */
static int factor = -1;

/*
 * Creates a complete task, only knowing on what device it should be executed.
 * Note that the global variable <current_config> is heavily used here.
 * Arguments :
 *	- taskp : a pointer to a valid task
 *	- type : STARPU_{CPU,CUDA,OPENCL}_WORKER. Gordon is not supported.
 *      - id   : -1 if you dont care about the device where the task will be
 *		 executed, as long as it has the right type.
 *		 >= 0 if you want to make sure the task will be executed on the
 *		 idth device that has the specified type.
 * Return values :
 * 	-ENODEV
 *	0 : success.
 */
static int
create_task(struct starpu_task **taskp, enum starpu_archtype type, int id)
{
	static int cpu_workers[STARPU_MAXCPUS];
#ifdef STARPU_USE_CUDA
	static int cuda_workers[STARPU_MAXCUDADEVS];
#endif
#ifdef STARPU_USE_OPENCL
	static int opencl_workers[STARPU_MAXOPENCLDEVS];
#endif

	static int n_cpus = -1;
#ifdef STARPU_USE_CUDA
	static int n_cudas = -1;
#endif
#ifdef STARPU_USE_OPENCL
	static int n_opencls = -1;
#endif

	if (n_cpus == -1) /* First time here */
	{
		/* XXX Dont check them all at once. */
		/* XXX Error checking */
		n_cpus = starpu_worker_get_ids_by_type(STARPU_CPU_WORKER,
							cpu_workers,
							STARPU_MAXCPUS);
#ifdef STARPU_USE_CUDA
		n_cudas = starpu_worker_get_ids_by_type(STARPU_CUDA_WORKER,
							cuda_workers,
							STARPU_MAXCUDADEVS);
#endif
#ifdef STARPU_USE_OPENCL
		n_opencls = starpu_worker_get_ids_by_type(STARPU_OPENCL_WORKER,
							opencl_workers,
							STARPU_MAXOPENCLDEVS);
#endif
	}

	int workerid=0;
	static struct starpu_codelet cl;
	cl.nbuffers = 1;
	cl.modes[0] = STARPU_RW;

	switch (type)
	{
		case STARPU_CPU_WORKER:
			if (id != -1)
			{
				if (id >= n_cpus)
				{
					FPRINTF(stderr, "Not enough CPU workers\n");
					return -ENODEV;
				}
				workerid = *(cpu_workers + id);
			}
			cl.where = STARPU_CPU;
			cl.cpu_funcs[0] = current_config->cpu_func;
			break;
#ifdef STARPU_USE_CUDA
		case STARPU_CUDA_WORKER:
			if (id != -1)
			{
				if (id >= n_cudas)
				{
					FPRINTF(stderr, "Not enough CUDA workers\n");
					return -ENODEV;
				}
				workerid = cuda_workers[id];
			}
			cl.where = STARPU_CUDA;
			cl.cuda_funcs[0] = current_config->cuda_func;
			break;
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
		case STARPU_OPENCL_WORKER:
			if (id != -1)
			{
				if (id >= n_opencls)
				{
					FPRINTF(stderr, "Not enough OpenCL workers\n");
					return -ENODEV;
				}
				workerid = *(opencl_workers + id);
			}
			cl.where = STARPU_OPENCL;
			cl.opencl_funcs[0] = current_config->opencl_func;
			break;
#endif /* ! STARPU_USE_OPENCL */
		case STARPU_GORDON_WORKER: /* Not supported */
		default:
			return -ENODEV;
	}


	struct starpu_task *task = starpu_task_create();
	task->synchronous = 1;
	task->cl = &cl;
	task->handles[0] = *current_config->handle;
	task->destroy = 0;
	if (id != -1)
	{
		task->execute_on_a_specific_worker = 1;
		task->workerid = workerid;
	}
	factor = -factor;
	task->cl_arg = &factor;
	task->cl_arg_size = sizeof(&factor);

	*taskp = task;
	return 0;
}

/*
 * <device1>_to_<device2> functions.
 * They all create and submit a task that has to be executed on <device2>,
 * forcing a copy between <device1> and <device2>.
 * XXX : could we sometimes use starp_insert_task() ? It seems hars because we
 * need to set the execute_on_a_specific_worker field...
 */
#ifdef STARPU_USE_CUDA
static enum exit_code
ram_to_cuda(void)
{
	int err;
	struct starpu_task *task;

	err = create_task(&task, STARPU_CUDA_WORKER, 0);
	if (err != 0)
		return TASK_CREATION_FAILURE;

	err = starpu_task_submit(task);
	if (err != 0)
		return TASK_SUBMISSION_FAILURE;

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	return current_config->copy_failed;
}

#ifdef HAVE_CUDA_MEMCPY_PEER
static enum exit_code
cuda_to_cuda(void)
{
	int err;
	struct starpu_task *task;

	err = create_task(&task, STARPU_CUDA_WORKER, 1);
	if (err != 0)
		return TASK_CREATION_FAILURE;

	err = starpu_task_submit(task);
	if (err != 0)
		return TASK_SUBMISSION_FAILURE;

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	return current_config->copy_failed;
}
#endif

static enum exit_code
cuda_to_ram(void)
{
	int err;
	struct starpu_task *task;

	err = create_task(&task, STARPU_CPU_WORKER, -1);
	if (err != 0)
		return TASK_CREATION_FAILURE;

	err = starpu_task_submit(task);
	if (err != 0)
		return TASK_SUBMISSION_FAILURE;

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	return current_config->copy_failed;
}
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static enum exit_code
ram_to_opencl()
{
	int err;
	struct starpu_task *task;

	err = create_task(&task, STARPU_OPENCL_WORKER, 0);
	if (err != 0)
		return TASK_CREATION_FAILURE;

	err = starpu_task_submit(task);
	if (err != 0)
		return TASK_SUBMISSION_FAILURE;

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	return current_config->copy_failed;
}

static enum exit_code
opencl_to_ram()
{
	int err;
	struct starpu_task *task;

	err = create_task(&task, STARPU_CPU_WORKER, -1);
	if (err != 0)
		return TASK_CREATION_FAILURE;

	err = starpu_task_submit(task);
	if (err != 0)
		return TASK_SUBMISSION_FAILURE;

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	return current_config->copy_failed;
}
#endif /* !STARPU_USE_OPENCL */
/* End of the <device1>_to_<device2> functions. */

#ifdef STARPU_USE_CUDA
static void
run_cuda(int async)
{
	/* RAM -> CUDA (-> CUDA) -> RAM */
	int err;
	err = ram_to_cuda();
	set_field(&summary, async, CPU_TO_CUDA, err);
	/* If this failed, there is no point in continuing. */
	if (err != SUCCESS)
		return;

#ifdef HAVE_CUDA_MEMCPY_PEER
	if (starpu_cuda_worker_get_count() >= 2)
	{
		err = cuda_to_cuda();
		set_field(&summary, async, CUDA_TO_CUDA, err);
		/* Even if cuda_to_cuda() failed, a valid copy is left on the first
		 * cuda device, which means we can safely test cuda_to_ram() */
	}
	else
	{
		summary.cuda_to_cuda_async = UNTESTED;
	}
#else
	summary.cuda_to_cuda_async = UNTESTED;
#endif /* !HAVE_CUDA_MEMCPY_PEER */

#ifdef STARPU_USE_CPU
	err = cuda_to_ram();
	set_field(&summary, async, CUDA_TO_CPU, err);
#endif /* !STARPU_USE_CPU */

}
#endif /* !STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
static void
run_opencl(int async)
{
	/* RAM -> OpenCL -> RAM */
	int err;

	err = ram_to_opencl();
	set_field(&summary, async, CPU_TO_OPENCL, err);
	if (err != SUCCESS)
		return;

#ifdef STARPU_USE_CPU
	err = opencl_to_ram();
	set_field(&summary, async, OPENCL_TO_CPU, err);
#endif /*!STARPU_USE_CPU */

}
#endif /* !STARPU_USE_OPENCL */

#ifdef STARPU_USE_CPU
/* Valid data should be in RAM before calling this function */
static void
ram_to_ram(void)
{
	int err;
	struct starpu_task *task;
	starpu_data_handle_t src, dst;
	void *src_interface, *dst_interface;

	src = *current_config->handle;
	dst = *current_config->dummy_handle;

	/* We do not care about the nodes */
	src_interface = starpu_data_get_interface_on_node(src, 0);
	dst_interface = starpu_data_get_interface_on_node(dst, 0);
	src->ops->copy_methods->ram_to_ram(src_interface, 0, dst_interface, 0);

	err = create_task(&task, STARPU_CPU_WORKER, -1);
	if (err != 0)
		goto out;

	task->handles[0] = dst;
	err = starpu_task_submit(task);
	starpu_task_destroy(task);

	if (err != 0)
	{
		err = TASK_SUBMISSION_FAILURE;
		goto out;
	}

	FPRINTF(stderr, "[%s] : %d\n", __func__, current_config->copy_failed);
	err = current_config->copy_failed;

out:
	set_field(&summary, 0, CPU_TO_CPU, err);
}
#endif /* !STARPU_USE_CPU */

static void
run_async(void)
{
	int async = starpu_asynchronous_copy_disabled();
	if (async == 1) {
		FPRINTF(stderr, "Asynchronous copies have been disabled\n");
		return;
	}
#ifdef STARPU_USE_CUDA
	run_cuda(1);
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	run_opencl(1);
#endif /* !STARPU_USE_OPENCL */
}

static void
run_sync(void)
{
	starpu_data_handle_t handle = *current_config->handle;
	struct starpu_data_copy_methods new_copy_methods;
	struct starpu_data_copy_methods *old_copy_methods;

	old_copy_methods = (struct starpu_data_copy_methods *) handle->ops->copy_methods;

	memcpy(&new_copy_methods,
		old_copy_methods,
		sizeof(struct starpu_data_copy_methods));

#ifdef STARPU_USE_CUDA
	new_copy_methods.ram_to_cuda_async = NULL;
	new_copy_methods.cuda_to_cuda_async = NULL;
	new_copy_methods.cuda_to_ram_async = NULL;
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	new_copy_methods.ram_to_opencl_async = NULL;
	new_copy_methods.opencl_to_ram_async = NULL;
#endif /* !STARPU_USE_OPENCL */

	handle->ops->copy_methods = &new_copy_methods;

#ifdef STARPU_USE_CUDA
	run_cuda(0);
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
	run_opencl(0);
#endif /* !STARPU_USE_OPENCL */

	handle->ops->copy_methods = old_copy_methods;
}

static void
compare(void)
{
	int err;
	void *interface_a, *interface_b;
	starpu_data_handle_t handle, dummy_handle;

	handle = *current_config->handle;
	dummy_handle = *current_config->dummy_handle;
	interface_a = starpu_data_get_interface_on_node(handle, 0);
	interface_b = starpu_data_get_interface_on_node(dummy_handle, 0);

	err = handle->ops->compare(interface_a, interface_b);

	if (err == 0)
	{
		summary.compare = FAILURE;
		summary.success = FAILURE;
	}
	else
	{
		summary.compare = SUCCESS;
	}
}

#ifdef STARPU_USE_CPU
static void
handle_to_pointer(void)
{
	void *ptr;
	unsigned int node;
	unsigned int tests = 0;
	starpu_data_handle_t handle;

	handle = *current_config->handle;
	if (!handle->ops->handle_to_pointer)
		return;

	for (node = 0; node < STARPU_MAXNODES; node++)
	{
		if (starpu_node_get_kind(node) != STARPU_CPU_RAM)
			continue;

		ptr = handle->ops->handle_to_pointer(handle, node);
		if (starpu_data_lookup(ptr) != handle)
		{
			summary.handle_to_pointer = FAILURE;
			return;
		}
		tests++;
	}

	if (tests > 0)
		summary.handle_to_pointer = SUCCESS;
}
#endif /* !STARPU_USE_CPU */

static int
load_conf(struct test_config *config)
{
	if (!config ||
#ifdef STARPU_USE_CPU
	    !config->cpu_func ||
	    !config->dummy_handle ||
#endif
#ifdef STARPU_USE_CUDA
	    !config->cuda_func ||
#endif
#ifdef STARPU_USE_OPENCL
	    !config->opencl_func ||
#endif
	    !config->handle)
	{
		return 1;
	}

	current_config = config;
	return 0;
}


data_interface_test_summary*
run_tests(struct test_config *conf)
{
	if (load_conf(conf) == 1)
	{
		FPRINTF(stderr, "Failed to load conf.\n");
		return NULL;
	}
	run_async();
	run_sync();

#ifdef STARPU_USE_CPU
	ram_to_ram();
	compare();
	handle_to_pointer();
#endif

	return &summary;
}
