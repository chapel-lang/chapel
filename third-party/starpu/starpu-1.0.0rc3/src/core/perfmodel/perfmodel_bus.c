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

#ifdef STARPU_USE_CUDA
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>
#endif
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

#include <starpu.h>
#include <starpu_cuda.h>
#include <starpu_opencl.h>
#include <common/config.h>
#include <core/workers.h>
#include <core/perfmodel/perfmodel.h>

#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif

#ifdef STARPU_HAVE_WINDOWS
#include <windows.h>
#endif

#define SIZE	(32*1024*1024*sizeof(char))
#define NITER	128

#define MAXCPUS	32

/* timing is in µs per byte (i.e. slowness, inverse of bandwidth) */
struct dev_timing
{
	int cpu_id;
	double timing_htod;
	double timing_dtoh;
};

static double bandwidth_matrix[STARPU_MAXNODES][STARPU_MAXNODES] = {{NAN}};
static double latency_matrix[STARPU_MAXNODES][STARPU_MAXNODES] = {{NAN}};
static unsigned was_benchmarked = 0;
static unsigned ncpus = 0;
static int ncuda = 0;
static int nopencl = 0;

/* Benchmarking the performance of the bus */

#ifdef STARPU_USE_CUDA
static int cuda_affinity_matrix[STARPU_MAXCUDADEVS][MAXCPUS];
static double cudadev_timing_htod[STARPU_MAXNODES] = {0.0};
static double cudadev_timing_dtoh[STARPU_MAXNODES] = {0.0};
#ifdef HAVE_CUDA_MEMCPY_PEER
static double cudadev_timing_dtod[STARPU_MAXNODES][STARPU_MAXNODES] = {{0.0}};
#endif
static struct dev_timing cudadev_timing_per_cpu[STARPU_MAXNODES*MAXCPUS];
#endif
#ifdef STARPU_USE_OPENCL
static int opencl_affinity_matrix[STARPU_MAXOPENCLDEVS][MAXCPUS];
static double opencldev_timing_htod[STARPU_MAXNODES] = {0.0};
static double opencldev_timing_dtoh[STARPU_MAXNODES] = {0.0};
static struct dev_timing opencldev_timing_per_cpu[STARPU_MAXNODES*MAXCPUS];
#endif

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)

#ifdef STARPU_HAVE_HWLOC
static hwloc_topology_t hwtopology;
#endif

#ifdef STARPU_USE_CUDA
static void measure_bandwidth_between_host_and_dev_on_cpu_with_cuda(int dev, int cpu, struct dev_timing *dev_timing_per_cpu)
{
	struct _starpu_machine_config *config = _starpu_get_machine_config();
	_starpu_bind_thread_on_cpu(config, cpu);
	size_t size = SIZE;

	/* Initiliaze CUDA context on the device */
	cudaSetDevice(dev);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

	/* hack to force the initialization */
	cudaFree(0);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

        /* Get the maximum size which can be allocated on the device */
	struct cudaDeviceProp prop;
	cudaError_t cures;
	cures = cudaGetDeviceProperties(&prop, dev);
	if (STARPU_UNLIKELY(cures)) STARPU_CUDA_REPORT_ERROR(cures);
        if (size > prop.totalGlobalMem/4) size = prop.totalGlobalMem/4;

	/* Allocate a buffer on the device */
	unsigned char *d_buffer;
	cudaMalloc((void **)&d_buffer, size);
	STARPU_ASSERT(d_buffer);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

	/* Allocate a buffer on the host */
	unsigned char *h_buffer;
	cures = cudaHostAlloc((void **)&h_buffer, size, 0);
	STARPU_ASSERT(cures == cudaSuccess);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

	/* Fill them */
	memset(h_buffer, 0, size);
	cudaMemset(d_buffer, 0, size);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

	unsigned iter;
	double timing;
	struct timeval start;
	struct timeval end;

	/* Measure upload bandwidth */
	gettimeofday(&start, NULL);
	for (iter = 0; iter < NITER; iter++)
	{
		cudaMemcpy(d_buffer, h_buffer, size, cudaMemcpyHostToDevice);
		cudaThreadSynchronize();
	}
	gettimeofday(&end, NULL);
	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_htod = timing/NITER/size;

	/* Measure download bandwidth */
	gettimeofday(&start, NULL);
	for (iter = 0; iter < NITER; iter++)
	{
		cudaMemcpy(h_buffer, d_buffer, size, cudaMemcpyDeviceToHost);
		cudaThreadSynchronize();
	}
	gettimeofday(&end, NULL);
	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_dtoh = timing/NITER/size;

	/* Free buffers */
	cudaFreeHost(h_buffer);
	cudaFree(d_buffer);

	cudaThreadExit();
}

#ifdef HAVE_CUDA_MEMCPY_PEER
static void measure_bandwidth_between_dev_and_dev_cuda(int src, int dst)
{
	size_t size = SIZE;

        /* Get the maximum size which can be allocated on the device */
	struct cudaDeviceProp prop;
	cudaError_t cures;
	cures = cudaGetDeviceProperties(&prop, src);
	if (STARPU_UNLIKELY(cures)) STARPU_CUDA_REPORT_ERROR(cures);
        if (size > prop.totalGlobalMem/4) size = prop.totalGlobalMem/4;
	cures = cudaGetDeviceProperties(&prop, dst);
	if (STARPU_UNLIKELY(cures)) STARPU_CUDA_REPORT_ERROR(cures);
        if (size > prop.totalGlobalMem/4) size = prop.totalGlobalMem/4;

	/* Initiliaze CUDA context on the source */
	cudaSetDevice(src);

	/* Allocate a buffer on the device */
	unsigned char *s_buffer;
	cudaMalloc((void **)&s_buffer, size);
	STARPU_ASSERT(s_buffer);
	cudaMemset(s_buffer, 0, size);

	/* Initiliaze CUDA context on the destination */
	cudaSetDevice(dst);

	/* Allocate a buffer on the device */
	unsigned char *d_buffer;
	cudaMalloc((void **)&d_buffer, size);
	STARPU_ASSERT(d_buffer);
	cudaMemset(d_buffer, 0, size);

	unsigned iter;
	double timing;
	struct timeval start;
	struct timeval end;

	/* Measure upload bandwidth */
	gettimeofday(&start, NULL);
	for (iter = 0; iter < NITER; iter++)
	{
		cudaMemcpyPeer(d_buffer, dst, s_buffer, src, size);
		cudaThreadSynchronize();
	}
	gettimeofday(&end, NULL);
	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	cudadev_timing_dtod[src+1][dst+1] = timing/NITER/size;

	/* Free buffers */
	cudaFree(d_buffer);
	cudaSetDevice(src);
	cudaFree(s_buffer);

	cudaThreadExit();
}
#endif
#endif

#ifdef STARPU_USE_OPENCL
static void measure_bandwidth_between_host_and_dev_on_cpu_with_opencl(int dev, int cpu, struct dev_timing *dev_timing_per_cpu)
{
        cl_context context;
        cl_command_queue queue;
        cl_int err=0;
	size_t size = SIZE;

        struct _starpu_machine_config *config = _starpu_get_machine_config();
	_starpu_bind_thread_on_cpu(config, cpu);

	/* Initialize OpenCL context on the device */
        _starpu_opencl_init_context(dev);
        starpu_opencl_get_context(dev, &context);
        starpu_opencl_get_queue(dev, &queue);

        /* Get the maximum size which can be allocated on the device */
        cl_device_id device;
	cl_ulong maxMemAllocSize;
        starpu_opencl_get_device(dev, &device);
	err = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(maxMemAllocSize), &maxMemAllocSize, NULL);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
        if (size > (size_t)maxMemAllocSize/4) size = maxMemAllocSize/4;

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

	/* Allocate a buffer on the device */
	cl_mem d_buffer;
	d_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &err);
	if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);
        /* Allocate a buffer on the host */
	unsigned char *h_buffer;
        h_buffer = (unsigned char *)malloc(size);
	STARPU_ASSERT(h_buffer);

	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);
        /* Fill them */
	memset(h_buffer, 0, size);
        err = clEnqueueWriteBuffer(queue, d_buffer, CL_TRUE, 0, size, h_buffer, 0, NULL, NULL);
        if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
	/* hack to avoid third party libs to rebind threads */
	_starpu_bind_thread_on_cpu(config, cpu);

        unsigned iter;
	double timing;
	struct timeval start;
	struct timeval end;

	/* Measure upload bandwidth */
	gettimeofday(&start, NULL);
	for (iter = 0; iter < NITER; iter++)
	{
                err = clEnqueueWriteBuffer(queue, d_buffer, CL_TRUE, 0, size, h_buffer, 0, NULL, NULL);
                if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
	}
	gettimeofday(&end, NULL);
	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_htod = timing/NITER/size;

	/* Measure download bandwidth */
	gettimeofday(&start, NULL);
	for (iter = 0; iter < NITER; iter++)
	{
                err = clEnqueueReadBuffer(queue, d_buffer, CL_TRUE, 0, size, h_buffer, 0, NULL, NULL);
                if (err != CL_SUCCESS) STARPU_OPENCL_REPORT_ERROR(err);
	}
	gettimeofday(&end, NULL);
	timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

	dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_dtoh = timing/NITER/size;

	/* Free buffers */
	clReleaseMemObject(d_buffer);
	free(h_buffer);

	/* Uninitiliaze OpenCL context on the device */
        _starpu_opencl_deinit_context(dev);
}
#endif

/* NB: we want to sort the bandwidth by DECREASING order */
static int compar_dev_timing(const void *left_dev_timing, const void *right_dev_timing)
{
	const struct dev_timing *left = (const struct dev_timing *)left_dev_timing;
	const struct dev_timing *right = (const struct dev_timing *)right_dev_timing;

	double left_dtoh = left->timing_dtoh;
	double left_htod = left->timing_htod;
	double right_dtoh = right->timing_dtoh;
	double right_htod = right->timing_htod;

	double bandwidth_sum2_left = left_dtoh*left_dtoh + left_htod*left_htod;
	double bandwidth_sum2_right = right_dtoh*right_dtoh + right_htod*right_htod;

	/* it's for a decreasing sorting */
	return (bandwidth_sum2_left < bandwidth_sum2_right);
}

#ifdef STARPU_HAVE_HWLOC
static int find_numa_node(hwloc_obj_t obj)
{
	STARPU_ASSERT(obj);
	hwloc_obj_t current = obj;

	while (current->depth != HWLOC_OBJ_NODE)
	{
		current = current->parent;

		/* If we don't find a "node" obj before the root, this means
		 * hwloc does not know whether there are numa nodes or not, so
		 * we should not use a per-node sampling in that case. */
		STARPU_ASSERT(current);
	}

	STARPU_ASSERT(current->depth == HWLOC_OBJ_NODE);

	return current->logical_index;
}
#endif

static void measure_bandwidth_between_cpus_and_dev(int dev, struct dev_timing *dev_timing_per_cpu, char type)
{
	/* Either we have hwloc and we measure the bandwith between each GPU
	 * and each NUMA node, or we don't have such NUMA information and we
	 * measure the bandwith for each pair of (CPU, GPU), which is slower.
	 * */
#ifdef STARPU_HAVE_HWLOC
	int cpu_depth = hwloc_get_type_depth(hwtopology, HWLOC_OBJ_CORE);
	int nnuma_nodes = hwloc_get_nbobjs_by_depth(hwtopology, HWLOC_OBJ_NODE);

	/* If no NUMA node was found, we assume that we have a single memory
	 * bank. */
	const unsigned no_node_obj_was_found = (nnuma_nodes == 0);

	unsigned *is_available_per_numa_node = NULL;
	double *dev_timing_htod_per_numa_node = NULL;
	double *dev_timing_dtoh_per_numa_node = NULL;

	if (!no_node_obj_was_found)
	{
		is_available_per_numa_node = (unsigned *)malloc(nnuma_nodes * sizeof(unsigned));
		STARPU_ASSERT(is_available_per_numa_node);

		dev_timing_htod_per_numa_node = (double *)malloc(nnuma_nodes * sizeof(double));
		STARPU_ASSERT(dev_timing_htod_per_numa_node);

		dev_timing_dtoh_per_numa_node = (double *)malloc(nnuma_nodes * sizeof(double));
		STARPU_ASSERT(dev_timing_dtoh_per_numa_node);

		memset(is_available_per_numa_node, 0, nnuma_nodes*sizeof(unsigned));
	}
#endif

	unsigned cpu;
	for (cpu = 0; cpu < ncpus; cpu++)
	{
		dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].cpu_id = cpu;

#ifdef STARPU_HAVE_HWLOC
		int numa_id = 0;

		if (!no_node_obj_was_found)
		{
			hwloc_obj_t obj = hwloc_get_obj_by_depth(hwtopology, cpu_depth, cpu);

			numa_id = find_numa_node(obj);

			if (is_available_per_numa_node[numa_id])
			{
				/* We reuse the previous numbers for that NUMA node */
				dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_htod =
					dev_timing_htod_per_numa_node[numa_id];
				dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_dtoh =
					dev_timing_dtoh_per_numa_node[numa_id];
				continue;
			}
		}
#endif

#ifdef STARPU_USE_CUDA
                if (type == 'C')
                        measure_bandwidth_between_host_and_dev_on_cpu_with_cuda(dev, cpu, dev_timing_per_cpu);
#endif
#ifdef STARPU_USE_OPENCL
                if (type == 'O')
                        measure_bandwidth_between_host_and_dev_on_cpu_with_opencl(dev, cpu, dev_timing_per_cpu);
#endif

#ifdef STARPU_HAVE_HWLOC
		if (!no_node_obj_was_found && !is_available_per_numa_node[numa_id])
		{
			/* Save the results for that NUMA node */
			dev_timing_htod_per_numa_node[numa_id] =
				dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_htod;
			dev_timing_dtoh_per_numa_node[numa_id] =
				dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_dtoh;

			is_available_per_numa_node[numa_id] = 1;
		}
#endif
        }

#ifdef STARPU_HAVE_HWLOC
	if (!no_node_obj_was_found)
	{
		free(is_available_per_numa_node);
		free(dev_timing_htod_per_numa_node);
		free(dev_timing_dtoh_per_numa_node);
	}
#endif /* STARPU_HAVE_HWLOC */
}

static void measure_bandwidth_between_host_and_dev(int dev, double *dev_timing_htod, double *dev_timing_dtoh,
                                                   struct dev_timing *dev_timing_per_cpu, char type)
{
	measure_bandwidth_between_cpus_and_dev(dev, dev_timing_per_cpu, type);

	/* sort the results */
	qsort(&(dev_timing_per_cpu[(dev+1)*MAXCPUS]), ncpus,
              sizeof(struct dev_timing),
			compar_dev_timing);

#ifdef STARPU_VERBOSE
        unsigned cpu;
	for (cpu = 0; cpu < ncpus; cpu++)
	{
		unsigned current_cpu = dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].cpu_id;
		double bandwidth_dtoh = dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_dtoh;
		double bandwidth_htod = dev_timing_per_cpu[(dev+1)*MAXCPUS+cpu].timing_htod;

		double bandwidth_sum2 = bandwidth_dtoh*bandwidth_dtoh + bandwidth_htod*bandwidth_htod;

		_STARPU_DISP("BANDWIDTH GPU %d CPU %u - htod %f - dtoh %f - %f\n", dev, current_cpu, bandwidth_htod, bandwidth_dtoh, sqrt(bandwidth_sum2));
	}

	unsigned best_cpu = dev_timing_per_cpu[(dev+1)*MAXCPUS+0].cpu_id;

	_STARPU_DISP("BANDWIDTH GPU %d BEST CPU %u\n", dev, best_cpu);
#endif

	/* The results are sorted in a decreasing order, so that the best
	 * measurement is currently the first entry. */
	dev_timing_dtoh[dev+1] = dev_timing_per_cpu[(dev+1)*MAXCPUS+0].timing_dtoh;
	dev_timing_htod[dev+1] = dev_timing_per_cpu[(dev+1)*MAXCPUS+0].timing_htod;
}
#endif /* defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL) */

static void benchmark_all_gpu_devices(void)
{
#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
	int i;
#ifdef HAVE_CUDA_MEMCPY_PEER
	int j;
#endif

	_STARPU_DEBUG("Benchmarking the speed of the bus\n");

#ifdef STARPU_HAVE_HWLOC
	hwloc_topology_init(&hwtopology);
	hwloc_topology_load(hwtopology);
#endif

#ifdef STARPU_HAVE_HWLOC
	hwloc_cpuset_t former_cpuset = hwloc_bitmap_alloc();
	hwloc_get_cpubind(hwtopology, former_cpuset, HWLOC_CPUBIND_THREAD);
#elif __linux__
	/* Save the current cpu binding */
	cpu_set_t former_process_affinity;
	int ret;
	ret = sched_getaffinity(0, sizeof(former_process_affinity), &former_process_affinity);
	if (ret)
	{
		perror("sched_getaffinity");
		STARPU_ABORT();
	}
#else
#warning Missing binding support, StarPU will not be able to properly benchmark NUMA topology
#endif

	struct _starpu_machine_config *config = _starpu_get_machine_config();
	ncpus = _starpu_topology_get_nhwcpu(config);

#ifdef STARPU_USE_CUDA
	ncuda = _starpu_get_cuda_device_count();
	for (i = 0; i < ncuda; i++)
	{
		fprintf(stderr," CUDA %d...", i);
		/* measure bandwidth between Host and Device i */
		measure_bandwidth_between_host_and_dev(i, cudadev_timing_htod, cudadev_timing_dtoh, cudadev_timing_per_cpu, 'C');
	}
#ifdef HAVE_CUDA_MEMCPY_PEER
	for (i = 0; i < ncuda; i++)
		for (j = 0; j < ncuda; j++)
			if (i != j)
			{
				fprintf(stderr," CUDA %d -> %d...", i, j);
				/* measure bandwidth between Host and Device i */
				measure_bandwidth_between_dev_and_dev_cuda(i, j);
			}
#endif
#endif
#ifdef STARPU_USE_OPENCL
        nopencl = _starpu_opencl_get_device_count();
	for (i = 0; i < nopencl; i++)
	{
		fprintf(stderr," OpenCL %d...", i);
		/* measure bandwith between Host and Device i */
		measure_bandwidth_between_host_and_dev(i, opencldev_timing_htod, opencldev_timing_dtoh, opencldev_timing_per_cpu, 'O');
	}
#endif

#ifdef STARPU_HAVE_HWLOC
	hwloc_set_cpubind(hwtopology, former_cpuset, HWLOC_CPUBIND_THREAD);
#elif __linux__
	/* Restore the former affinity */
	ret = sched_setaffinity(0, sizeof(former_process_affinity), &former_process_affinity);
	if (ret)
	{
		perror("sched_setaffinity");
		STARPU_ABORT();
	}
#endif

#ifdef STARPU_HAVE_HWLOC
	hwloc_topology_destroy(hwtopology);
#endif

	_STARPU_DEBUG("Benchmarking the speed of the bus is done.\n");
#endif /* defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL) */

	was_benchmarked = 1;
}

static void get_bus_path(const char *type, char *path, size_t maxlen)
{
	_starpu_get_perf_model_dir_bus(path, maxlen);
	strncat(path, type, maxlen);

	char hostname[32];
	char *forced_hostname = getenv("STARPU_HOSTNAME");
	if (forced_hostname && forced_hostname[0])
		snprintf(hostname, sizeof(hostname), "%s", forced_hostname);
	else
		gethostname(hostname, sizeof(hostname));
	strncat(path, ".", maxlen);
	strncat(path, hostname, maxlen);
}

/*
 *	Affinity
 */

static void get_affinity_path(char *path, size_t maxlen)
{
	get_bus_path("affinity", path, maxlen);
}

static void load_bus_affinity_file_content(void)
{
#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
	FILE *f;

	char path[256];
	get_affinity_path(path, 256);

	f = fopen(path, "r");
	STARPU_ASSERT(f);

	struct _starpu_machine_config *config = _starpu_get_machine_config();
	ncpus = _starpu_topology_get_nhwcpu(config);
        int gpu;

#ifdef STARPU_USE_CUDA
	ncuda = _starpu_get_cuda_device_count();
	for (gpu = 0; gpu < ncuda; gpu++)
	{
		int ret;

		int dummy;

		_starpu_drop_comments(f);
		ret = fscanf(f, "%d\t", &dummy);
		STARPU_ASSERT(ret == 1);

		STARPU_ASSERT(dummy == gpu);

		unsigned cpu;
		for (cpu = 0; cpu < ncpus; cpu++)
		{
			ret = fscanf(f, "%d\t", &cuda_affinity_matrix[gpu][cpu]);
			STARPU_ASSERT(ret == 1);
		}

		ret = fscanf(f, "\n");
		STARPU_ASSERT(ret == 0);
	}
#endif /* !STARPU_USE_CUDA */
#ifdef STARPU_USE_OPENCL
        nopencl = _starpu_opencl_get_device_count();
	for (gpu = 0; gpu < nopencl; gpu++)
	{
		int ret;

		int dummy;

		_starpu_drop_comments(f);
		ret = fscanf(f, "%d\t", &dummy);
		STARPU_ASSERT(ret == 1);

		STARPU_ASSERT(dummy == gpu);

		unsigned cpu;
		for (cpu = 0; cpu < ncpus; cpu++)
		{
			ret = fscanf(f, "%d\t", &opencl_affinity_matrix[gpu][cpu]);
			STARPU_ASSERT(ret == 1);
		}

		ret = fscanf(f, "\n");
		STARPU_ASSERT(ret == 0);
	}
#endif /* !STARPU_USE_OPENCL */

	fclose(f);
#endif /* !(STARPU_USE_CUDA_ ||�STARPU_USE_OPENCL */

}

static void write_bus_affinity_file_content(void)
{
	STARPU_ASSERT(was_benchmarked);

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
	FILE *f;
	char path[256];
	get_affinity_path(path, 256);
	f = fopen(path, "w+");
	if (!f)
	{
		perror("fopen write_buf_affinity_file_content");
		_STARPU_DISP("path '%s'\n", path);
		fflush(stderr);
		STARPU_ABORT();
	}

	unsigned cpu;
        int gpu;

        fprintf(f, "# GPU\t");
	for (cpu = 0; cpu < ncpus; cpu++)
		fprintf(f, "CPU%u\t", cpu);
	fprintf(f, "\n");

#ifdef STARPU_USE_CUDA
	for (gpu = 0; gpu < ncuda; gpu++)
	{
		fprintf(f, "%d\t", gpu);

		for (cpu = 0; cpu < ncpus; cpu++)
		{
			fprintf(f, "%d\t", cudadev_timing_per_cpu[(gpu+1)*MAXCPUS+cpu].cpu_id);
		}

		fprintf(f, "\n");
	}
#endif
#ifdef STARPU_USE_OPENCL
	for (gpu = 0; gpu < nopencl; gpu++)
	{
		fprintf(f, "%d\t", gpu);

		for (cpu = 0; cpu < ncpus; cpu++)
		{
                        fprintf(f, "%d\t", opencldev_timing_per_cpu[(gpu+1)*MAXCPUS+cpu].cpu_id);
		}

		fprintf(f, "\n");
	}
#endif

	fclose(f);
#endif
}

static void generate_bus_affinity_file(void)
{
	if (!was_benchmarked)
		benchmark_all_gpu_devices();

	write_bus_affinity_file_content();
}

static void load_bus_affinity_file(void)
{
	int res;

	char path[256];
	get_affinity_path(path, 256);

	res = access(path, F_OK);
	if (res)
	{
		/* File does not exist yet */
		generate_bus_affinity_file();
	}

	load_bus_affinity_file_content();
}

#ifdef STARPU_USE_CUDA
int *_starpu_get_cuda_affinity_vector(unsigned gpuid)
{
        return cuda_affinity_matrix[gpuid];
}
#endif /* STARPU_USE_CUDA */

#ifdef STARPU_USE_OPENCL
int *_starpu_get_opencl_affinity_vector(unsigned gpuid)
{
        return opencl_affinity_matrix[gpuid];
}
#endif /* STARPU_USE_OPENCL */

/*
 *	Latency
 */

static void get_latency_path(char *path, size_t maxlen)
{
	get_bus_path("latency", path, maxlen);
}

static int load_bus_latency_file_content(void)
{
	int n;
	unsigned src, dst;
	FILE *f;

	char path[256];
	get_latency_path(path, 256);

	f = fopen(path, "r");
	STARPU_ASSERT(f);

	for (src = 0; src < STARPU_MAXNODES; src++)
	{
		_starpu_drop_comments(f);
		for (dst = 0; dst < STARPU_MAXNODES; dst++)
		{
			double latency;

			n = fscanf(f, "%lf", &latency);
			if (n != 1)
			{
				fclose(f);
				return 0;
			}
			n = getc(f);
			if (n != '\t')
			{
				fclose(f);
				return 0;
			}

			latency_matrix[src][dst] = latency;
		}

		n = getc(f);
		if (n != '\n')
		{
			fclose(f);
			return 0;
		}
	}

	fclose(f);
	return 1;
}

static void write_bus_latency_file_content(void)
{
        int src, dst, maxnode;
	FILE *f;

	STARPU_ASSERT(was_benchmarked);

	char path[256];
	get_latency_path(path, 256);

	f = fopen(path, "w+");
	if (!f)
	{
		perror("fopen write_bus_latency_file_content");
		_STARPU_DISP("path '%s'\n", path);
		fflush(stderr);
		STARPU_ABORT();
	}

	fprintf(f, "# ");
	for (dst = 0; dst < STARPU_MAXNODES; dst++)
		fprintf(f, "to %d\t\t", dst);
	fprintf(f, "\n");

        maxnode = ncuda;
#ifdef STARPU_USE_OPENCL
        maxnode += nopencl;
#endif
        for (src = 0; src < STARPU_MAXNODES; src++)
	{
		for (dst = 0; dst < STARPU_MAXNODES; dst++)
		{
			double latency;

			if ((src > maxnode) || (dst > maxnode))
			{
				/* convention */
				latency = NAN;
			}
			else if (src == dst)
			{
				latency = 0.0;
			}
			else
			{
				/* µs */
                                latency = ((src && dst)?2000.0:500.0);
			}

			fprintf(f, "%f\t", latency);
		}

		fprintf(f, "\n");
	}

	fclose(f);
}

static void generate_bus_latency_file(void)
{
	if (!was_benchmarked)
		benchmark_all_gpu_devices();

	write_bus_latency_file_content();
}

static void load_bus_latency_file(void)
{
	int res;

	char path[256];
	get_latency_path(path, 256);

	res = access(path, F_OK);
	if (res || !load_bus_latency_file_content())
	{
		/* File does not exist yet or is bogus */
		generate_bus_latency_file();
	}

}


/*
 *	Bandwidth
 */
static void get_bandwidth_path(char *path, size_t maxlen)
{
	get_bus_path("bandwidth", path, maxlen);
}

static int load_bus_bandwidth_file_content(void)
{
	int n;
	unsigned src, dst;
	FILE *f;

	char path[256];
	get_bandwidth_path(path, 256);

	f = fopen(path, "r");
	if (!f)
	{
		perror("fopen load_bus_bandwidth_file_content");
		_STARPU_DISP("path '%s'\n", path);
		fflush(stderr);
		STARPU_ABORT();
	}

	for (src = 0; src < STARPU_MAXNODES; src++)
	{
		_starpu_drop_comments(f);
		for (dst = 0; dst < STARPU_MAXNODES; dst++)
		{
			double bandwidth;

			n = fscanf(f, "%lf", &bandwidth);
			if (n != 1)
			{
				fprintf(stderr,"didn't get a number\n");
				fclose(f);
				return 0;
			}
			n = getc(f);
			if (n != '\t')
			{
				fclose(f);
				return 0;
			}

			bandwidth_matrix[src][dst] = bandwidth;
		}

		n = getc(f);
		if (n != '\n')
		{
			fclose(f);
			return 0;
		}
	}

	fclose(f);
	return 1;
}

static void write_bus_bandwidth_file_content(void)
{
	int src, dst, maxnode;
	FILE *f;

	STARPU_ASSERT(was_benchmarked);

	char path[256];
	get_bandwidth_path(path, 256);

	f = fopen(path, "w+");
	STARPU_ASSERT(f);

	fprintf(f, "# ");
	for (dst = 0; dst < STARPU_MAXNODES; dst++)
		fprintf(f, "to %d\t\t", dst);
	fprintf(f, "\n");

        maxnode = ncuda;
#ifdef STARPU_USE_OPENCL
        maxnode += nopencl;
#endif
	for (src = 0; src < STARPU_MAXNODES; src++)
	{
		for (dst = 0; dst < STARPU_MAXNODES; dst++)
		{
			double bandwidth;

			if ((src > maxnode) || (dst > maxnode))
			{
				bandwidth = NAN;
			}
#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
			else if (src != dst)
			{
				double slowness = 0.0;
				/* Total bandwidth is the harmonic mean of bandwidths */
#ifdef STARPU_USE_CUDA
#ifdef HAVE_CUDA_MEMCPY_PEER
				if (src && src <= ncuda && dst && dst <= ncuda)
					/* Direct GPU-GPU transfert */
					slowness = cudadev_timing_dtod[src][dst];
				else
#endif
				{
					if (src && src <= ncuda)
						slowness += cudadev_timing_dtoh[src];
					if (dst && dst <= ncuda)
						slowness += cudadev_timing_htod[dst];
				}
#endif
#ifdef STARPU_USE_OPENCL
				if (src > ncuda)
					slowness += opencldev_timing_dtoh[src-ncuda];
				if (dst > ncuda)
					slowness += opencldev_timing_htod[dst-ncuda];
#endif
				bandwidth = 1.0/slowness;
			}
#endif
			else
			{
			        /* convention */
			        bandwidth = 0.0;
			}

			fprintf(f, "%f\t", bandwidth);
		}

		fprintf(f, "\n");
	}

	fclose(f);
}

void starpu_bus_print_bandwidth(FILE *f)
{
	int src, dst, maxnode;

        maxnode = ncuda;
#ifdef STARPU_USE_OPENCL
        maxnode += nopencl;
#endif

	fprintf(f, "from\t");
	fprintf(f, "to RAM\t\t");
	for (dst = 0; dst < ncuda; dst++)
		fprintf(f, "to CUDA %d\t", dst);
	for (dst = 0; dst < nopencl; dst++)
		fprintf(f, "to OpenCL %d\t", dst);
	fprintf(f, "\n");

	for (src = 0; src <= maxnode; src++)
	{
		if (!src)
			fprintf(f, "RAM\t");
		else if (src <= ncuda)
			fprintf(f, "CUDA %d\t", src-1);
		else
			fprintf(f, "OpenCL%d\t", src-ncuda-1);
		for (dst = 0; dst <= maxnode; dst++)
			fprintf(f, "%f\t", bandwidth_matrix[src][dst]);

		fprintf(f, "\n");
	}
}

static void generate_bus_bandwidth_file(void)
{
	if (!was_benchmarked)
		benchmark_all_gpu_devices();

	write_bus_bandwidth_file_content();
}

static void load_bus_bandwidth_file(void)
{
	int res;

	char path[256];
	get_bandwidth_path(path, 256);

	res = access(path, F_OK);
	if (res || !load_bus_bandwidth_file_content())
	{
		/* File does not exist yet or is bogus */
		generate_bus_bandwidth_file();
	}
}

/*
 *	Config
 */
static void get_config_path(char *path, size_t maxlen)
{
	get_bus_path("config", path, maxlen);
}

static void check_bus_config_file()
{
        int res;
        char path[256];

        get_config_path(path, 256);
        res = access(path, F_OK);
        if (res)
	{
		fprintf(stderr, "No performance model for the bus, calibrating...");
		starpu_force_bus_sampling();
		fprintf(stderr, "done\n");
        }
        else
	{
                FILE *f;
                int ret, read_cuda = -1, read_opencl = -1;
                unsigned read_cpus = -1;
                struct _starpu_machine_config *config = _starpu_get_machine_config();

                // Loading configuration from file
                f = fopen(path, "r");
                STARPU_ASSERT(f);
                _starpu_drop_comments(f);
                ret = fscanf(f, "%u\t", &read_cpus);
		STARPU_ASSERT(ret == 1);
                _starpu_drop_comments(f);
		ret = fscanf(f, "%d\t", &read_cuda);
		STARPU_ASSERT(ret == 1);
                _starpu_drop_comments(f);
		ret = fscanf(f, "%d\t", &read_opencl);
		STARPU_ASSERT(ret == 1);
                _starpu_drop_comments(f);
                fclose(f);

                // Loading current configuration
                ncpus = _starpu_topology_get_nhwcpu(config);
#ifdef STARPU_USE_CUDA
		ncuda = _starpu_get_cuda_device_count();
#endif
#ifdef STARPU_USE_OPENCL
                nopencl = _starpu_opencl_get_device_count();
#endif

                // Checking if both configurations match
                if (read_cpus != ncpus)
		{
			fprintf(stderr, "Current configuration does not match the bus performance model (CPUS: (stored) %u != (current) %u), recalibrating...", read_cpus, ncpus);
                        starpu_force_bus_sampling();
			fprintf(stderr, "done\n");
                }
                else if (read_cuda != ncuda)
		{
                        fprintf(stderr, "Current configuration does not match the bus performance model (CUDA: (stored) %d != (current) %d), recalibrating...", read_cuda, ncuda);
                        starpu_force_bus_sampling();
			fprintf(stderr, "done\n");
                }
                else if (read_opencl != nopencl)
		{
                        fprintf(stderr, "Current configuration does not match the bus performance model (OpenCL: (stored) %d != (current) %d), recalibrating...", read_opencl, nopencl);
                        starpu_force_bus_sampling();
			fprintf(stderr, "done\n");
                }
        }
}

static void write_bus_config_file_content(void)
{
	FILE *f;
	char path[256];

	STARPU_ASSERT(was_benchmarked);
        get_config_path(path, 256);
        f = fopen(path, "w+");
	STARPU_ASSERT(f);

        fprintf(f, "# Current configuration\n");
        fprintf(f, "%u # Number of CPUs\n", ncpus);
        fprintf(f, "%d # Number of CUDA devices\n", ncuda);
        fprintf(f, "%d # Number of OpenCL devices\n", nopencl);

        fclose(f);
}

static void generate_bus_config_file()
{
	if (!was_benchmarked)
		benchmark_all_gpu_devices();

	write_bus_config_file_content();
}

/*
 *	Generic
 */

void starpu_force_bus_sampling(void)
{
	_starpu_create_sampling_directory_if_needed();

	generate_bus_affinity_file();
	generate_bus_latency_file();
	generate_bus_bandwidth_file();
        generate_bus_config_file();
}

void _starpu_load_bus_performance_files(void)
{
	_starpu_create_sampling_directory_if_needed();

        check_bus_config_file();
	load_bus_affinity_file();
	load_bus_latency_file();
	load_bus_bandwidth_file();
}

/* (in µs) */
double _starpu_predict_transfer_time(unsigned src_node, unsigned dst_node, size_t size)
{
	double bandwidth = bandwidth_matrix[src_node][dst_node];
	double latency = latency_matrix[src_node][dst_node];
	struct starpu_machine_topology *topology = &_starpu_get_machine_config()->topology;

	return latency + (size/bandwidth)*2*(topology->ncudagpus+topology->nopenclgpus);
}
