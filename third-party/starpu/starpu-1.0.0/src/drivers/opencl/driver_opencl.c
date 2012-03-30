/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2010  Mehdi Juhoor <mjuhoor@gmail.com>
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

#include <math.h>
#include <starpu.h>
#include <starpu_profiling.h>
#include <common/config.h>
#include <common/utils.h>
#include <core/debug.h>
#include <starpu_opencl.h>
#include <drivers/driver_common/driver_common.h>
#include "driver_opencl.h"
#include "driver_opencl_utils.h"
#include <common/utils.h>

static pthread_mutex_t big_lock = PTHREAD_MUTEX_INITIALIZER;

static cl_context contexts[STARPU_MAXOPENCLDEVS];
static cl_device_id devices[STARPU_MAXOPENCLDEVS];
static cl_command_queue queues[STARPU_MAXOPENCLDEVS];
static cl_command_queue transfer_queues[STARPU_MAXOPENCLDEVS];
static cl_uint nb_devices = -1;
static int init_done = 0;
extern char *_starpu_opencl_program_dir;

/* In case we want to cap the amount of memory available on the GPUs by the
 * mean of the STARPU_LIMIT_GPU_MEM, we allocate a big buffer when the driver
 * is launched. */
static cl_mem wasted_memory[STARPU_MAXOPENCLDEVS];

static void limit_gpu_mem_if_needed(int devid)
{
	cl_int err;

	int limit = starpu_get_env_number("STARPU_LIMIT_GPU_MEM");
	if (limit == -1)
	{
		wasted_memory[devid] = NULL;
		return;
	}

	/* Request the size of the current device's memory */
	cl_ulong totalGlobalMem;
	err = clGetDeviceInfo(devices[devid], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(totalGlobalMem), &totalGlobalMem, NULL);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	/* How much memory to waste ? */
	size_t to_waste = (size_t)totalGlobalMem - (size_t)limit*1024*1024;

	_STARPU_DEBUG("OpenCL device %d: Wasting %ld MB / Limit %ld MB / Total %ld MB / Remains %ld MB\n",
                      devid, (size_t)to_waste/(1024*1024), (size_t)limit, (size_t)totalGlobalMem/(1024*1024),
                      (size_t)(totalGlobalMem - to_waste)/(1024*1024));

	/* Allocate a large buffer to waste memory and constraint the amount of available memory. */
	wasted_memory[devid] = clCreateBuffer(contexts[devid], CL_MEM_READ_WRITE, to_waste, NULL, &err);
	if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
}

static void unlimit_gpu_mem_if_needed(int devid)
{
	if (wasted_memory[devid])
	{
		cl_int err = clReleaseMemObject(wasted_memory[devid]);
		if (err != CL_SUCCESS)
			STARPU_OPENCL_REPORT_ERROR(err);
		wasted_memory[devid] = NULL;
	}
}

size_t starpu_opencl_get_global_mem_size(int devid)
{
	cl_int err;
	cl_ulong totalGlobalMem;

	/* Request the size of the current device's memory */
	err = clGetDeviceInfo(devices[devid], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(totalGlobalMem), &totalGlobalMem, NULL);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	return (size_t)totalGlobalMem;
}

void starpu_opencl_get_context(int devid, cl_context *context)
{
        *context = contexts[devid];
}

void starpu_opencl_get_device(int devid, cl_device_id *device)
{
        *device = devices[devid];
}

void starpu_opencl_get_queue(int devid, cl_command_queue *queue)
{
        *queue = queues[devid];
}

void starpu_opencl_get_current_queue(cl_command_queue *queue)
{
	struct _starpu_worker *worker = _starpu_get_local_worker_key();
	STARPU_ASSERT(queue);
        *queue = queues[worker->devid];
}

void starpu_opencl_get_current_context(cl_context *context)
{
	struct _starpu_worker *worker = _starpu_get_local_worker_key();
	STARPU_ASSERT(context);
        *context = contexts[worker->devid];
}

cl_int _starpu_opencl_init_context(int devid)
{
	cl_int err;

	_STARPU_PTHREAD_MUTEX_LOCK(&big_lock);

        _STARPU_DEBUG("Initialising context for dev %d\n", devid);

        // Create a compute context
	err = 0;
        contexts[devid] = clCreateContext(NULL, 1, &devices[devid], NULL, NULL, &err);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        // Create execution queue for the given device
        queues[devid] = clCreateCommandQueue(contexts[devid], devices[devid], 0, &err);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        // Create transfer queue for the given device
        cl_command_queue_properties props;
        err = clGetDeviceInfo(devices[devid], CL_DEVICE_QUEUE_PROPERTIES, sizeof(props), &props, NULL);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);
        props &= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        transfer_queues[devid] = clCreateCommandQueue(contexts[devid], devices[devid], props, &err);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&big_lock);

	limit_gpu_mem_if_needed(devid);

	return CL_SUCCESS;
}

cl_int _starpu_opencl_deinit_context(int devid)
{
        cl_int err;

	_STARPU_PTHREAD_MUTEX_LOCK(&big_lock);

        _STARPU_DEBUG("De-initialising context for dev %d\n", devid);

	unlimit_gpu_mem_if_needed(devid);

        err = clReleaseContext(contexts[devid]);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        err = clReleaseCommandQueue(queues[devid]);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        err = clReleaseCommandQueue(transfer_queues[devid]);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        contexts[devid] = NULL;

	_STARPU_PTHREAD_MUTEX_UNLOCK(&big_lock);

        return CL_SUCCESS;
}

cl_int starpu_opencl_allocate_memory(cl_mem *mem, size_t size, cl_mem_flags flags)
{
	cl_int err;
        cl_mem memory;
        struct _starpu_worker *worker = _starpu_get_local_worker_key();

	memory = clCreateBuffer(contexts[worker->devid], flags, size, NULL, &err);
	if (err == CL_OUT_OF_HOST_MEMORY) return err;
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	/*
	 * OpenCL uses lazy memory allocation: we will only know if the
	 * allocation failed when trying to copy data onto the device. But we
	 * want to know this __now__, so we just perform a dummy copy.
	 */
	char dummy = 0;
	err = clEnqueueWriteBuffer(queues[worker->devid], memory, CL_TRUE,
				   0, sizeof(dummy), &dummy,
				   0, NULL, NULL);
	clFinish(queues[worker->devid]);
	if (err == CL_MEM_OBJECT_ALLOCATION_FAILURE)
		return err;
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

        *mem = memory;
        return CL_SUCCESS;
}

cl_int starpu_opencl_copy_ram_to_opencl(void *ptr, unsigned src_node STARPU_ATTRIBUTE_UNUSED, cl_mem buffer, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, size_t size, size_t offset, cl_event *event, int *ret)
{
        cl_int err;
        struct _starpu_worker *worker = _starpu_get_local_worker_key();

        if (event)
                _STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
        err = clEnqueueWriteBuffer(transfer_queues[worker->devid], buffer, CL_FALSE, offset, size, ptr, 0, NULL, event);
        if (event)
                _STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
        if (STARPU_LIKELY(err == CL_SUCCESS))
	{
		if (event == NULL)
		{
			/* We want a synchronous copy, let's synchronise the queue */
			clFinish(transfer_queues[worker->devid]);
		}
		if (ret)
		{
			*ret = (event == NULL) ? 0 : -EAGAIN;
		}
	}
	return err;
}

cl_int starpu_opencl_copy_opencl_to_ram(cl_mem buffer, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *ptr, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, size_t size, size_t offset, cl_event *event, int *ret)
{
        cl_int err;
        struct _starpu_worker *worker = _starpu_get_local_worker_key();

        if (event)
                _STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
        err = clEnqueueReadBuffer(transfer_queues[worker->devid], buffer, CL_FALSE, offset, size, ptr, 0, NULL, event);
        if (event)
                _STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
        if (STARPU_LIKELY(err == CL_SUCCESS))
	{
		if (event == NULL)
		{
			/* We want a synchronous copy, let's synchronise the queue */
			clFinish(transfer_queues[worker->devid]);
		}
		if (ret)
		{
			*ret = (event == NULL) ? 0 : -EAGAIN;
		}
	}
	return err;
}

#if 0
cl_int _starpu_opencl_copy_rect_opencl_to_ram(cl_mem buffer, unsigned src_node STARPU_ATTRIBUTE_UNUSED, void *ptr, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, const size_t buffer_origin[3], const size_t host_origin[3],
                                              const size_t region[3], size_t buffer_row_pitch, size_t buffer_slice_pitch,
                                              size_t host_row_pitch, size_t host_slice_pitch, cl_event *event)
{
        cl_int err;
        struct _starpu_worker *worker = _starpu_get_local_worker_key();
        cl_bool blocking;

        blocking = (event == NULL) ? CL_TRUE : CL_FALSE;
        if (event)
                _STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
        err = clEnqueueReadBufferRect(transfer_queues[worker->devid], buffer, blocking, buffer_origin, host_origin, region, buffer_row_pitch,
                                      buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, 0, NULL, event);
        if (event)
                _STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        return CL_SUCCESS;
}

cl_int _starpu_opencl_copy_rect_ram_to_opencl(void *ptr, unsigned src_node STARPU_ATTRIBUTE_UNUSED, cl_mem buffer, unsigned dst_node STARPU_ATTRIBUTE_UNUSED, const size_t buffer_origin[3], const size_t host_origin[3],
                                              const size_t region[3], size_t buffer_row_pitch, size_t buffer_slice_pitch,
                                              size_t host_row_pitch, size_t host_slice_pitch, cl_event *event)
{
        cl_int err;
        struct _starpu_worker *worker = _starpu_get_local_worker_key();
        cl_bool blocking;

        blocking = (event == NULL) ? CL_TRUE : CL_FALSE;
        if (event)
                _STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node);
        err = clEnqueueWriteBufferRect(transfer_queues[worker->devid], buffer, blocking, buffer_origin, host_origin, region, buffer_row_pitch,
                                       buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, 0, NULL, event);
        if (event)
                _STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

        return CL_SUCCESS;
}
#endif

void _starpu_opencl_init(void)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&big_lock);
        if (!init_done)
	{
                cl_platform_id platform_id[_STARPU_OPENCL_PLATFORM_MAX];
                cl_uint nb_platforms;
                cl_int err;
                unsigned int i;
                cl_device_type device_type = CL_DEVICE_TYPE_GPU|CL_DEVICE_TYPE_ACCELERATOR;

                _STARPU_DEBUG("Initialising OpenCL\n");

                // Get Platforms
		if (starpu_get_env_number("STARPU_OPENCL_ON_CPUS") > 0)
		     device_type |= CL_DEVICE_TYPE_CPU;
                err = clGetPlatformIDs(_STARPU_OPENCL_PLATFORM_MAX, platform_id, &nb_platforms);
                if (err != CL_SUCCESS) nb_platforms=0;
                _STARPU_DEBUG("Platforms detected: %d\n", nb_platforms);

                // Get devices
                nb_devices = 0;
                {
                        for (i=0; i<nb_platforms; i++)
			{
                                cl_uint num;
				int platform_valid = 1;
				char name[1024], vendor[1024];

				err = clGetPlatformInfo(platform_id[i], CL_PLATFORM_NAME, 1024, name, NULL);
				if (err != CL_SUCCESS)
				{
					STARPU_OPENCL_REPORT_ERROR_WITH_MSG("clGetPlatformInfo NAME", err);
					platform_valid = 0;
				}
				else
				{
					err = clGetPlatformInfo(platform_id[i], CL_PLATFORM_VENDOR, 1024, vendor, NULL);
					if (err != CL_SUCCESS)
					{
						STARPU_OPENCL_REPORT_ERROR_WITH_MSG("clGetPlatformInfo VENDOR", err);
						platform_valid = 0;
					}
				}
#ifdef STARPU_VERBOSE
				if (platform_valid)
					_STARPU_DEBUG("Platform: %s - %s\n", name, vendor);
				else
					_STARPU_DEBUG("Platform invalid\n");
#endif
				if (platform_valid)
				{
					err = clGetDeviceIDs(platform_id[i], device_type, STARPU_MAXOPENCLDEVS-nb_devices, &devices[nb_devices], &num);
					if (err == CL_DEVICE_NOT_FOUND)
					{
						_STARPU_DEBUG("  No devices detected on this platform\n");
					}
					else
					{
						if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
						_STARPU_DEBUG("  %d devices detected\n", num);
						nb_devices += num;
					}
				}
			}
		}

                // Get location of OpenCl kernel source files
                _starpu_opencl_program_dir = getenv("STARPU_OPENCL_PROGRAM_DIR");

		if (nb_devices > STARPU_MAXOPENCLDEVS)
		{
			_STARPU_DISP("# Warning: %u OpenCL devices available. Only %d enabled. Use configure option --enable-maxopencldev=xxx to update the maximum value of supported OpenCL devices?\n", nb_devices, STARPU_MAXOPENCLDEVS);
			nb_devices = STARPU_MAXOPENCLDEVS;
		}

                // initialise internal structures
                for(i=0 ; i<nb_devices ; i++)
		{
                        contexts[i] = NULL;
                        queues[i] = NULL;
                        transfer_queues[i] = NULL;
                }

                init_done=1;
        }
	_STARPU_PTHREAD_MUTEX_UNLOCK(&big_lock);
}

static unsigned _starpu_opencl_get_device_name(int dev, char *name, int lname);
static int _starpu_opencl_execute_job(struct _starpu_job *j, struct _starpu_worker *args);

void *_starpu_opencl_worker(void *arg)
{
	struct _starpu_worker* args = arg;

	int devid = args->devid;
	int workerid = args->workerid;

#ifdef USE_FXT
	fxt_register_thread(args->bindid);
#endif

	unsigned memnode = args->memory_node;
	_STARPU_TRACE_WORKER_INIT_START(_STARPU_FUT_OPENCL_KEY, devid, memnode);

	_starpu_bind_thread_on_cpu(args->config, args->bindid);

	_starpu_set_local_memory_node_key(&memnode);

	_starpu_set_local_worker_key(args);

	_starpu_opencl_init_context(devid);

	/* one more time to avoid hacks from third party lib :) */
	_starpu_bind_thread_on_cpu(args->config, args->bindid);

	args->status = STATUS_UNKNOWN;

	/* get the device's name */
	char devname[128];
	_starpu_opencl_get_device_name(devid, devname, 128);
	snprintf(args->name, sizeof(args->name), "OpenCL %d (%s)", args->devid, devname);
	snprintf(args->short_name, sizeof(args->short_name), "OpenCL %d", args->devid);

	_STARPU_DEBUG("OpenCL (%s) dev id %d thread is ready to run on CPU %d !\n", devname, devid, args->bindid);

	_STARPU_TRACE_WORKER_INIT_END

	/* tell the main thread that this one is ready */
	_STARPU_PTHREAD_MUTEX_LOCK(&args->mutex);
	args->worker_is_initialized = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&args->ready_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&args->mutex);

	struct _starpu_job * j;
	struct starpu_task *task;
	int res;

	while (_starpu_machine_is_running())
	{
		_STARPU_TRACE_START_PROGRESS(memnode);
		_starpu_datawizard_progress(memnode, 1);
		_STARPU_TRACE_END_PROGRESS(memnode);

		_STARPU_PTHREAD_MUTEX_LOCK(args->sched_mutex);

		task = _starpu_pop_task(args);

                if (task == NULL)
		{
			if (_starpu_worker_can_block(memnode))
				_starpu_block_worker(workerid, args->sched_cond, args->sched_mutex);

			_STARPU_PTHREAD_MUTEX_UNLOCK(args->sched_mutex);

			continue;
		};

		_STARPU_PTHREAD_MUTEX_UNLOCK(args->sched_mutex);

		STARPU_ASSERT(task);
		j = _starpu_get_job_associated_to_task(task);

		/* can OpenCL do that task ? */
		if (!_STARPU_OPENCL_MAY_PERFORM(j))
		{
			/* this is not a OpenCL task */
			_starpu_push_task(j);
			continue;
		}

		_starpu_set_current_task(j->task);
		args->current_task = j->task;

		res = _starpu_opencl_execute_job(j, args);

		_starpu_set_current_task(NULL);
		args->current_task = NULL;

                if (res)
		{
			switch (res)
			{
				case -EAGAIN:
					_STARPU_DISP("ouch, put the codelet %p back ... \n", j);
					_starpu_push_task(j);
					STARPU_ABORT();
					continue;
				default:
					STARPU_ASSERT(0);
			}
		}

		_starpu_handle_job_termination(j);
	}

	_STARPU_TRACE_WORKER_DEINIT_START

	_starpu_handle_all_pending_node_data_requests(memnode);

        _starpu_opencl_deinit_context(devid);

	pthread_exit(NULL);

	return NULL;
}

static unsigned _starpu_opencl_get_device_name(int dev, char *name, int lname)
{
	int err;

        if (!init_done)
	{
                _starpu_opencl_init();
        }

	// Get device name
	err = clGetDeviceInfo(devices[dev], CL_DEVICE_NAME, lname, name, NULL);
	if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	_STARPU_DEBUG("Device %d : [%s]\n", dev, name);
	return EXIT_SUCCESS;
}

unsigned _starpu_opencl_get_device_count(void)
{
        if (!init_done)
	{
                _starpu_opencl_init();
        }
	return nb_devices;
}

cl_device_type _starpu_opencl_get_device_type(int devid)
{
	int err;
	cl_device_type type;

	if (!init_done)
		_starpu_opencl_init();

	err = clGetDeviceInfo(devices[devid], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	return type;
}

static int _starpu_opencl_execute_job(struct _starpu_job *j, struct _starpu_worker *args)
{
	int ret;
	uint32_t mask = 0;

	STARPU_ASSERT(j);
	struct starpu_task *task = j->task;

	int profiling = starpu_profiling_status_get();
	struct timespec codelet_start, codelet_end;

	STARPU_ASSERT(task);
	struct starpu_codelet *cl = task->cl;
	STARPU_ASSERT(cl);

	ret = _starpu_fetch_task_input(j, mask);
	if (ret != 0)
	{
		/* there was not enough memory, so the input of
		 * the codelet cannot be fetched ... put the
		 * codelet back, and try it later */
		return -EAGAIN;
	}

	_starpu_driver_start_job(args, j, &codelet_start, 0, profiling);

	starpu_opencl_func_t func = _starpu_task_get_opencl_nth_implementation(cl, j->nimpl);
	STARPU_ASSERT(func);
	func(task->interfaces, task->cl_arg);

	_starpu_driver_end_job(args, j, args->perf_arch, &codelet_end, 0, profiling);

	_starpu_driver_update_job_feedback(j, args, args->perf_arch,
					   &codelet_start, &codelet_end, profiling);

	_starpu_push_task_output(j, mask);

	return EXIT_SUCCESS;
}
