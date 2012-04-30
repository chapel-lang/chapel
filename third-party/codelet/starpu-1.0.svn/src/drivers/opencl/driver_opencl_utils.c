/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
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
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <starpu_opencl.h>
#include <starpu_profiling.h>
#include <core/workers.h>
#include "driver_opencl_utils.h"
#include "driver_opencl.h"

#ifdef HAVE_CL_CL_EXT_H
#include <CL/cl_ext.h>
#endif

char *_starpu_opencl_program_dir;

#define _STARPU_STRINGIFY_(x) #x
#define _STARPU_STRINGIFY(x) _STARPU_STRINGIFY_(x)

static
int _starpu_opencl_locate_file(const char *source_file_name, char *located_file_name, char *located_dir_name)
{
	int ret = EXIT_FAILURE;

        _STARPU_DEBUG("Trying to locate <%s>\n", source_file_name);
        if (access(source_file_name, R_OK) == 0)
	{
                strcpy(located_file_name, source_file_name);
		ret = EXIT_SUCCESS;
        }

	if (ret == EXIT_FAILURE && _starpu_opencl_program_dir)
	{
		sprintf(located_file_name, "%s/%s", _starpu_opencl_program_dir, source_file_name);
		_STARPU_DEBUG("Trying to locate <%s>\n", located_file_name);
		if (access(located_file_name, R_OK) == 0)
			ret = EXIT_SUCCESS;
	}

	if (ret == EXIT_FAILURE)
	{
		sprintf(located_file_name, "%s/%s", _STARPU_STRINGIFY(STARPU_OPENCL_DATADIR), source_file_name);
		_STARPU_DEBUG("Trying to locate <%s>\n", located_file_name);
		if (access(located_file_name, R_OK) == 0)
			ret = EXIT_SUCCESS;
	}

	if (ret == EXIT_FAILURE)
	{
		sprintf(located_file_name, "%s/%s", STARPU_SRC_DIR, source_file_name);
		_STARPU_DEBUG("Trying to locate <%s>\n", located_file_name);
		if (access(located_file_name, R_OK) == 0)
			ret = EXIT_SUCCESS;
	}

	if (ret == EXIT_FAILURE)
	{
		strcpy(located_file_name, "");
		strcpy(located_dir_name, "");
		_STARPU_ERROR("Cannot locate file <%s>\n", source_file_name);
	}
	else
	{
		char *last = strrchr(located_file_name, '/');

		if (!last)
		{
			strcpy(located_dir_name, "");
		}
		else
		{
			sprintf(located_dir_name, "%s", located_file_name);
			located_dir_name[strlen(located_file_name)-strlen(last)+1] = '\0';
		}
	}

        return ret;
}

cl_int starpu_opencl_load_kernel(cl_kernel *kernel, cl_command_queue *queue, struct starpu_opencl_program *opencl_programs,
                                 const char *kernel_name, int devid)
{
        cl_int err;
	cl_device_id device;
        cl_context context;
        cl_program program;

        starpu_opencl_get_device(devid, &device);
        starpu_opencl_get_context(devid, &context);
        starpu_opencl_get_queue(devid, queue);

        program = opencl_programs->programs[devid];
        if (!program)
	{
                _STARPU_DISP("Program not available for device <%d>\n", devid);
                return CL_INVALID_PROGRAM;
        }

        // Create the compute kernel in the program we wish to run
        *kernel = clCreateKernel(program, kernel_name, &err);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

	return CL_SUCCESS;
}

cl_int starpu_opencl_release_kernel(cl_kernel kernel)
{
	cl_int err;

	err = clReleaseKernel(kernel);
	if (err != CL_SUCCESS)
		STARPU_OPENCL_REPORT_ERROR(err);

        return CL_SUCCESS;
}

static
char *_starpu_opencl_load_program_source(const char *filename)
{
        struct stat statbuf;
        FILE        *fh;
        char        *source;
        int         x;
        char        c;

        fh = fopen(filename, "r");
        if (!fh)
                return NULL;

        stat(filename, &statbuf);
        source = (char *) malloc(statbuf.st_size + 1);

        for(c=(char)fgetc(fh), x=0 ; c != EOF ; c =(char)fgetc(fh), x++)
	{
		source[x] = c;
        }
        source[x] = '\0';


        _STARPU_DEBUG("OpenCL kernel <%s>\n", source);

        fclose(fh);

        return source;
}

int starpu_opencl_load_opencl_from_string(const char *opencl_program_source, struct starpu_opencl_program *opencl_programs,
					  const char* build_options)
{
        unsigned int dev;
        unsigned int nb_devices;

        nb_devices = _starpu_opencl_get_device_count();
        // Iterate over each device
        for(dev = 0; dev < nb_devices; dev ++)
	{
                cl_device_id device;
                cl_context   context;
                cl_program   program;
                cl_int       err;

                opencl_programs->programs[dev] = NULL;

                starpu_opencl_get_device(dev, &device);
                starpu_opencl_get_context(dev, &context);
                if (context == NULL)
		{
                        _STARPU_DEBUG("[%d] is not a valid OpenCL context\n", dev);
                        continue;
                }

                // Create the compute program from the source buffer
                program = clCreateProgramWithSource(context, 1, (const char **) &opencl_program_source, NULL, &err);
                if (!program || err != CL_SUCCESS) {
			_STARPU_DISP("Error: Failed to load program source!\n");
			return EXIT_FAILURE;
		}

                // Build the program executable
                err = clBuildProgram(program, 1, &device, build_options, NULL, NULL);

		// Get the status
		{
		     cl_build_status status;
		     size_t len;
		     static char buffer[4096] = "";

		     clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		     if (len > 2)
			  _STARPU_DISP("Compilation output\n%s\n", buffer);

		     clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_STATUS, sizeof(status), &status, NULL);
		     if (err != CL_SUCCESS || status != CL_BUILD_SUCCESS)
		     {
			  _STARPU_DISP("Error: Failed to build program executable!\n");
			  _STARPU_DISP("clBuildProgram: %d - clGetProgramBuildInfo: %d\n", err, status);
			  return EXIT_FAILURE;
		     }

		}

                // Store program
                opencl_programs->programs[dev] = program;
        }
        return EXIT_SUCCESS;
}

int starpu_opencl_load_opencl_from_file(const char *source_file_name, struct starpu_opencl_program *opencl_programs,
					const char* build_options)
{
	int nb_devices;
        char located_file_name[1024];
        char located_dir_name[1024];
	char new_build_options[1024];

	// Do not try to load and compile the file if there is no devices
	nb_devices = starpu_opencl_worker_get_count();
	if (nb_devices == 0) return EXIT_SUCCESS;

        // Locate source file
        _starpu_opencl_locate_file(source_file_name, located_file_name, located_dir_name);
        _STARPU_DEBUG("Source file name : <%s>\n", located_file_name);
        _STARPU_DEBUG("Source directory name : <%s>\n", located_dir_name);

        // Load the compute program from disk into a cstring buffer
        char *opencl_program_source = _starpu_opencl_load_program_source(located_file_name);
        if(!opencl_program_source)
                _STARPU_ERROR("Failed to load compute program from file <%s>!\n", located_file_name);

	if (!strcmp(located_dir_name, ""))
		strcpy(new_build_options, build_options);
	else if (build_options)
		sprintf(new_build_options, "-I %s %s", located_dir_name, build_options);
	else
		sprintf(new_build_options, "-I %s", located_dir_name);
	_STARPU_DEBUG("Build options: <%s>\n", new_build_options);

        return starpu_opencl_load_opencl_from_string(opencl_program_source, opencl_programs, new_build_options);
}

int starpu_opencl_unload_opencl(struct starpu_opencl_program *opencl_programs)
{
        unsigned int dev;
        unsigned int nb_devices;

	if (!starpu_opencl_worker_get_count())
		return 0;

        nb_devices = _starpu_opencl_get_device_count();
        // Iterate over each device
        for(dev = 0; dev < nb_devices; dev ++)
	{
		if (opencl_programs->programs[dev])
		{
			cl_int err;
			err = clReleaseProgram(opencl_programs->programs[dev]);
			if (err != CL_SUCCESS)
				STARPU_OPENCL_REPORT_ERROR(err);
		}
        }
        return 0;
}

int starpu_opencl_collect_stats(cl_event event STARPU_ATTRIBUTE_UNUSED)
{
#if defined(CL_PROFILING_CLOCK_CYCLE_COUNT)||defined(CL_PROFILING_STALL_CYCLE_COUNT)||defined(CL_PROFILING_POWER_CONSUMED)
	struct starpu_task *task = starpu_task_get_current();
	struct starpu_task_profiling_info *info = task->profiling_info;
#endif

#ifdef CL_PROFILING_CLOCK_CYCLE_COUNT
	if (starpu_profiling_status_get() && info)
	{
		cl_int err;
		unsigned int clock_cycle_count;
		size_t size;
		err = clGetEventProfilingInfo(event, CL_PROFILING_CLOCK_CYCLE_COUNT, sizeof(clock_cycle_count), &clock_cycle_count, &size);
		if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
		STARPU_ASSERT(size == sizeof(clock_cycle_count));
		info->used_cycles += clock_cycle_count;
	}
#endif
#ifdef CL_PROFILING_STALL_CYCLE_COUNT
	if (starpu_profiling_status_get() && info)
	{
		cl_int err;
		unsigned int stall_cycle_count;
		size_t size;
		err = clGetEventProfilingInfo(event, CL_PROFILING_STALL_CYCLE_COUNT, sizeof(stall_cycle_count), &stall_cycle_count, &size);
		if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
		STARPU_ASSERT(size == sizeof(stall_cycle_count));

		info->stall_cycles += stall_cycle_count;
	}
#endif
#ifdef CL_PROFILING_POWER_CONSUMED
	if (info && (starpu_profiling_status_get() || (task->cl && task->cl->power_model && task->cl->power_model->benchmarking)))
	{
		cl_int err;
		double power_consumed;
		size_t size;
		err = clGetEventProfilingInfo(event, CL_PROFILING_POWER_CONSUMED, sizeof(power_consumed), &power_consumed, &size);
		if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
		STARPU_ASSERT(size == sizeof(power_consumed));

		info->power_consumed += power_consumed;
	}
#endif

	return 0;
}

void starpu_opencl_display_error(const char *func, const char *file, int line, const char* msg, cl_int status)
{
	const char *errormsg;
	switch (status)
	{
	case CL_SUCCESS:
		errormsg = "success";
		break;
	case CL_DEVICE_NOT_FOUND:
		errormsg = "Device not found";
		break;
	case CL_DEVICE_NOT_AVAILABLE:
		errormsg = "Device not available";
		break;
	case CL_COMPILER_NOT_AVAILABLE:
		errormsg = "Compiler not available";
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		errormsg = "Memory object allocation failure";
		break;
	case CL_OUT_OF_RESOURCES:
		errormsg = "Out of resources";
		break;
	case CL_OUT_OF_HOST_MEMORY:
		errormsg = "Out of host memory";
		break;
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		errormsg = "Profiling info not available";
		break;
	case CL_MEM_COPY_OVERLAP:
		errormsg = "Memory copy overlap";
		break;
	case CL_IMAGE_FORMAT_MISMATCH:
		errormsg = "Image format mismatch";
		break;
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		errormsg = "Image format not supported";
		break;
	case CL_BUILD_PROGRAM_FAILURE:
		errormsg = "Build program failure";
		break;
	case CL_MAP_FAILURE:
		errormsg = "Map failure";
		break;
	case CL_INVALID_VALUE:
		errormsg = "Invalid value";
		break;
	case CL_INVALID_DEVICE_TYPE:
		errormsg = "Invalid device type";
		break;
	case CL_INVALID_PLATFORM:
		errormsg = "Invalid platform";
		break;
	case CL_INVALID_DEVICE:
		errormsg = "Invalid device";
		break;
	case CL_INVALID_CONTEXT:
		errormsg = "Invalid context";
		break;
	case CL_INVALID_QUEUE_PROPERTIES:
		errormsg = "Invalid queue properties";
		break;
	case CL_INVALID_COMMAND_QUEUE:
		errormsg = "Invalid command queue";
		break;
	case CL_INVALID_HOST_PTR:
		errormsg = "Invalid host pointer";
		break;
	case CL_INVALID_MEM_OBJECT:
		errormsg = "Invalid memory object";
		break;
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		errormsg = "Invalid image format descriptor";
		break;
	case CL_INVALID_IMAGE_SIZE:
		errormsg = "Invalid image size";
		break;
	case CL_INVALID_SAMPLER:
		errormsg = "Invalid sampler";
		break;
	case CL_INVALID_BINARY:
		errormsg = "Invalid binary";
		break;
	case CL_INVALID_BUILD_OPTIONS:
		errormsg = "Invalid build options";
		break;
	case CL_INVALID_PROGRAM:
		errormsg = "Invalid program";
		break;
	case CL_INVALID_PROGRAM_EXECUTABLE:
		errormsg = "Invalid program executable";
		break;
	case CL_INVALID_KERNEL_NAME:
		errormsg = "Invalid kernel name";
		break;
	case CL_INVALID_KERNEL_DEFINITION:
		errormsg = "Invalid kernel definition";
		break;
	case CL_INVALID_KERNEL:
		errormsg = "Invalid kernel";
		break;
	case CL_INVALID_ARG_INDEX:
		errormsg = "Invalid argument index";
		break;
	case CL_INVALID_ARG_VALUE:
		errormsg = "Invalid argument value";
		break;
	case CL_INVALID_ARG_SIZE:
		errormsg = "Invalid argument size";
		break;
	case CL_INVALID_KERNEL_ARGS:
		errormsg = "Invalid kernel arguments";
		break;
	case CL_INVALID_WORK_DIMENSION:
		errormsg = "Invalid work dimension";
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		errormsg = "Invalid work group size";
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		errormsg = "Invalid work item size";
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		errormsg = "Invalid global offset";
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		errormsg = "Invalid event wait list";
		break;
	case CL_INVALID_EVENT:
		errormsg = "Invalid event";
		break;
	case CL_INVALID_OPERATION:
		errormsg = "Invalid operation";
		break;
	case CL_INVALID_GL_OBJECT:
		errormsg = "Invalid GL object";
		break;
	case CL_INVALID_BUFFER_SIZE:
		errormsg = "Invalid buffer size";
		break;
	case CL_INVALID_MIP_LEVEL:
		errormsg = "Invalid MIP level";
		break;
#ifdef CL_PLATFORM_NOT_FOUND_KHR
	case CL_PLATFORM_NOT_FOUND_KHR:
		errormsg = "Platform not found";
		break;
#endif
	default:
		errormsg = "unknown error";
		break;
	}
	if (msg)
		printf("oops in %s (%s:%d) (%s) ... <%s> (%d) \n", func, file, line, msg, errormsg, status);
	else
		printf("oops in %s (%s:%d) ... <%s> (%d) \n", func, file, line, errormsg, status);

}

int starpu_opencl_set_kernel_args(cl_int *error, cl_kernel *kernel, ...)
{
	int i;
	va_list ap;

	va_start(ap, kernel);

	for (i = 0; ; i++)
	{
		int size = va_arg(ap, int);
		if (size == 0)
			break;

		cl_mem *ptr = va_arg(ap, cl_mem *);
		int err = clSetKernelArg(*kernel, i, size, ptr);
		if (err != CL_SUCCESS)
		{
			*error = err;
			break;
		}
	}

	va_end(ap);
	return i;
}
