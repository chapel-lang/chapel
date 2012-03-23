/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2012  Université de Bordeaux 1
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

#include <stdlib.h>
#include <stdio.h>
#include <common/config.h>
#include <core/workers.h>
#include <core/debug.h>
#include <core/topology.h>
#include <drivers/cuda/driver_cuda.h>
#include <starpu_hash.h>
#include <profiling/profiling.h>

#ifdef STARPU_HAVE_HWLOC
#include <hwloc.h>
#ifndef HWLOC_API_VERSION
#define HWLOC_OBJ_PU HWLOC_OBJ_PROC
#endif
#endif

#ifdef STARPU_HAVE_WINDOWS
#include <windows.h>
#endif
#ifndef HWLOC_BITMAP_H
/* hwloc <1.1 does not offer the bitmap API yet */
#define hwloc_bitmap_alloc hwloc_cpuset_alloc
#define hwloc_bitmap_only hwloc_cpuset_cpu
#define hwloc_bitmap_singlify hwloc_cpuset_singlify
#endif

static unsigned topology_is_initialized = 0;

static void _starpu_initialize_workers_bindid(struct _starpu_machine_config *config);

#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
#  ifdef STARPU_USE_CUDA
static void _starpu_initialize_workers_cuda_gpuid(struct _starpu_machine_config *config);
static struct starpu_htbl32_node *devices_using_cuda = NULL;
#  endif
#  ifdef STARPU_USE_OPENCL
static void _starpu_initialize_workers_opencl_gpuid(struct _starpu_machine_config *config);
#  endif
static void _starpu_initialize_workers_gpuid(int *explicit_workers_gpuid,
                                             int *current, int *workers_gpuid, const char *varname, unsigned nhwgpus);
static unsigned may_bind_automatically = 0;
#endif

/*
 * Discover the topology of the machine
 */

#ifdef STARPU_USE_CUDA
static void _starpu_initialize_workers_cuda_gpuid(struct _starpu_machine_config *config)
{
	struct starpu_machine_topology *topology = &config->topology;

        _starpu_initialize_workers_gpuid(config->conf->use_explicit_workers_cuda_gpuid==0?NULL:(int *)config->conf->workers_cuda_gpuid,
                                         &(config->current_cuda_gpuid), (int *)topology->workers_cuda_gpuid, "STARPU_WORKERS_CUDAID",
                                         topology->nhwcudagpus);
}
#endif

#ifdef STARPU_USE_OPENCL
static void _starpu_initialize_workers_opencl_gpuid(struct _starpu_machine_config *config)
{
	struct starpu_machine_topology *topology = &config->topology;

        _starpu_initialize_workers_gpuid(config->conf->use_explicit_workers_opencl_gpuid==0?NULL:(int *)config->conf->workers_opencl_gpuid,
                                         &(config->current_opencl_gpuid), (int *)topology->workers_opencl_gpuid, "STARPU_WORKERS_OPENCLID",
                                         topology->nhwopenclgpus);

#ifdef STARPU_USE_CUDA
        // Detect devices which are already used with CUDA
        {
                unsigned tmp[STARPU_NMAXWORKERS];
                unsigned nb=0;
                int i;
                for(i=0 ; i<STARPU_NMAXWORKERS ; i++)
		{
                        uint32_t key = starpu_crc32_be(config->topology.workers_opencl_gpuid[i], 0);
                        if (_starpu_htbl_search_32(devices_using_cuda, key) == NULL)
			{
                                tmp[nb] = topology->workers_opencl_gpuid[i];
                                nb++;
                        }
                }
                for(i=nb ; i<STARPU_NMAXWORKERS ; i++) tmp[i] = -1;
                memcpy(topology->workers_opencl_gpuid, tmp, sizeof(unsigned)*STARPU_NMAXWORKERS);
        }
#endif /* STARPU_USE_CUDA */
        {
                // Detect identical devices
                struct starpu_htbl32_node *devices_already_used = NULL;
                unsigned tmp[STARPU_NMAXWORKERS];
                unsigned nb=0;
                int i;

                for(i=0 ; i<STARPU_NMAXWORKERS ; i++)
		{
                        uint32_t key = starpu_crc32_be(topology->workers_opencl_gpuid[i], 0);
                        if (_starpu_htbl_search_32(devices_already_used, key) == NULL)
			{
                                _starpu_htbl_insert_32(&devices_already_used, key, config);
                                tmp[nb] = topology->workers_opencl_gpuid[i];
                                nb ++;
                        }
                }
                for(i=nb ; i<STARPU_NMAXWORKERS ; i++) tmp[i] = -1;
                memcpy(topology->workers_opencl_gpuid, tmp, sizeof(unsigned)*STARPU_NMAXWORKERS);
        }
}
#endif


#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
static void _starpu_initialize_workers_gpuid(int *explicit_workers_gpuid,
                                             int *current, int *workers_gpuid, const char *varname, unsigned nhwgpus)
{
	char *strval;
	unsigned i;

	*current = 0;

	/* conf->workers_bindid indicates the successive cpu identifier that
	 * should be used to bind the workers. It should be either filled
	 * according to the user's explicit parameters (from starpu_conf) or
	 * according to the STARPU_WORKERS_CPUID env. variable. Otherwise, a
	 * round-robin policy is used to distributed the workers over the
	 * cpus. */

	/* what do we use, explicit value, env. variable, or round-robin ? */
	if ((strval = getenv(varname)))
	{
		/* STARPU_WORKERS_CUDAID certainly contains less entries than
		 * STARPU_NMAXWORKERS, so we reuse its entries in a round robin
		 * fashion: "1 2" is equivalent to "1 2 1 2 1 2 .... 1 2". */
		unsigned wrap = 0;
		unsigned number_of_entries = 0;

		char *endptr;
		/* we use the content of the STARPU_WORKERS_CUDAID env. variable */
		for (i = 0; i < STARPU_NMAXWORKERS; i++)
		{
			if (!wrap)
			{
				long int val;
				val = strtol(strval, &endptr, 10);
				if (endptr != strval)
				{
					workers_gpuid[i] = (unsigned)val;
					strval = endptr;
				}
				else
				{
					/* there must be at least one entry */
					STARPU_ASSERT(i != 0);
					number_of_entries = i;

					/* there is no more values in the string */
					wrap = 1;

					workers_gpuid[i] = workers_gpuid[0];
				}
			}
			else
			{
				workers_gpuid[i] = workers_gpuid[i % number_of_entries];
			}
		}
	}
	else if (explicit_workers_gpuid)
	{
		/* we use the explicit value from the user */
		memcpy(workers_gpuid,
                       explicit_workers_gpuid,
                       STARPU_NMAXWORKERS*sizeof(unsigned));
	}
	else
	{
		/* by default, we take a round robin policy */
		if (nhwgpus > 0)
		     for (i = 0; i < STARPU_NMAXWORKERS; i++)
			  workers_gpuid[i] = (unsigned)(i % nhwgpus);

		/* StarPU can use sampling techniques to bind threads correctly */
		may_bind_automatically = 1;
	}
}
#endif

#ifdef STARPU_USE_CUDA
static inline int _starpu_get_next_cuda_gpuid(struct _starpu_machine_config *config)
{
	unsigned i = ((config->current_cuda_gpuid++) % config->topology.ncudagpus);

	return (int)config->topology.workers_cuda_gpuid[i];
}
#endif

#ifdef STARPU_USE_OPENCL
static inline int _starpu_get_next_opencl_gpuid(struct _starpu_machine_config *config)
{
	unsigned i = ((config->current_opencl_gpuid++) % config->topology.nopenclgpus);

	return (int)config->topology.workers_opencl_gpuid[i];
}
#endif

static void _starpu_init_topology(struct _starpu_machine_config *config)
{
	struct starpu_machine_topology *topology = &config->topology;

	if (!topology_is_initialized)
	{
		topology->nhwcpus = 0;
#ifdef STARPU_HAVE_HWLOC
		hwloc_topology_init(&topology->hwtopology);
		hwloc_topology_load(topology->hwtopology);

		config->cpu_depth = hwloc_get_type_depth(topology->hwtopology, HWLOC_OBJ_CORE);

		/* Would be very odd */
		STARPU_ASSERT(config->cpu_depth != HWLOC_TYPE_DEPTH_MULTIPLE);

		if (config->cpu_depth == HWLOC_TYPE_DEPTH_UNKNOWN)
			/* unknown, using logical procesors as fallback */
			config->cpu_depth = hwloc_get_type_depth(topology->hwtopology, HWLOC_OBJ_PU);

		topology->nhwcpus = hwloc_get_nbobjs_by_depth(topology->hwtopology, config->cpu_depth);
#elif defined(__MINGW32__) || defined(__CYGWIN__)
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		topology->nhwcpus += sysinfo.dwNumberOfProcessors;
#elif defined(HAVE_SYSCONF)
		topology->nhwcpus = sysconf(_SC_NPROCESSORS_ONLN);
#else
#warning no way to know number of cores, assuming 1
		topology->nhwcpus = 1;
#endif

#ifdef STARPU_USE_CUDA
                config->topology.nhwcudagpus = _starpu_get_cuda_device_count();
#endif
#ifdef STARPU_USE_OPENCL
                config->topology.nhwopenclgpus = _starpu_opencl_get_device_count();
#endif

		topology_is_initialized = 1;
	}
}

unsigned _starpu_topology_get_nhwcpu(struct _starpu_machine_config *config)
{
	_starpu_init_topology(config);

	return config->topology.nhwcpus;
}

static int _starpu_init_machine_config(struct _starpu_machine_config *config)
{
	int i;
	for (i = 0; i < STARPU_NMAXWORKERS; i++)
		config->workers[i].workerid = i;

	struct starpu_machine_topology *topology = &config->topology;

	topology->nworkers = 0;
	topology->ncombinedworkers = 0;
	_starpu_init_topology(config);

	_starpu_initialize_workers_bindid(config);

#ifdef STARPU_USE_CUDA
	int ncuda = config->conf->ncuda;

	if (ncuda != 0)
	{
		/* The user did not disable CUDA. We need to initialize CUDA
 		 * early to count the number of devices */
		_starpu_init_cuda();
		int nb_devices = _starpu_get_cuda_device_count();

		if (ncuda == -1)
		{
			/* Nothing was specified, so let's choose ! */
			ncuda = nb_devices;
			if (ncuda > STARPU_MAXCUDADEVS)
			{
				fprintf(stderr,
					"# Warning: %d CUDA devices available. Only %d enabled. Use configure option --enable-maxcudadev=xxx to update the maximum value of supported CUDA devices.\n",
					nb_devices, STARPU_MAXCUDADEVS);
				ncuda = STARPU_MAXCUDADEVS;
			}
		}
		else
		{
			if (ncuda > nb_devices)
			{
				/* The user requires more CUDA devices than there is available */
				fprintf(stderr,
					"# Warning: %d CUDA devices requested. Only %d available.\n",
					ncuda, nb_devices);
				ncuda = nb_devices;
			}
			/* Let's make sure this value is OK. */
			if (ncuda > STARPU_MAXCUDADEVS)
			{
				fprintf(stderr,
					"# Warning: %d CUDA devices requested. Only %d enabled. Use configure option --enable-maxcudadev=xxx to update the maximum value of supported CUDA devices.\n",
					ncuda, STARPU_MAXCUDADEVS);
				ncuda = STARPU_MAXCUDADEVS;
			}
		}
	}

	/* Now we know how many CUDA devices will be used */
	topology->ncudagpus = ncuda;
	STARPU_ASSERT(topology->ncudagpus <= STARPU_MAXCUDADEVS);

	_starpu_initialize_workers_cuda_gpuid(config);

	unsigned cudagpu;
	for (cudagpu = 0; cudagpu < topology->ncudagpus; cudagpu++)
	{
		config->workers[topology->nworkers + cudagpu].arch = STARPU_CUDA_WORKER;
		int devid = _starpu_get_next_cuda_gpuid(config);
		enum starpu_perf_archtype arch = (enum starpu_perf_archtype)((int)STARPU_CUDA_DEFAULT + devid);
		config->workers[topology->nworkers + cudagpu].devid = devid;
		config->workers[topology->nworkers + cudagpu].perf_arch = arch;
		config->workers[topology->nworkers + cudagpu].worker_mask = STARPU_CUDA;
		config->worker_mask |= STARPU_CUDA;

                uint32_t key = starpu_crc32_be(devid, 0);
                _starpu_htbl_insert_32(&devices_using_cuda, key, config);
        }

	topology->nworkers += topology->ncudagpus;
#endif

#ifdef STARPU_USE_OPENCL
	int nopencl = config->conf->nopencl;

	if (nopencl != 0)
	{
		/* The user did not disable OPENCL. We need to initialize OpenCL
 		 * early to count the number of devices */
		_starpu_opencl_init();
		int nb_devices;
		nb_devices = _starpu_opencl_get_device_count();

		if (nopencl == -1)
		{
			/* Nothing was specified, so let's choose ! */
			nopencl = nb_devices;
			if (nopencl > STARPU_MAXOPENCLDEVS)
			{
				fprintf(stderr,
					"# Warning: %d OpenCL devices available. Only %d enabled. Use configure option --enable-maxopencldadev=xxx to update the maximum value of supported OpenCL devices.\n",
					nb_devices, STARPU_MAXOPENCLDEVS);
				nopencl = STARPU_MAXOPENCLDEVS;
			}
		}
		else
		{
			/* Let's make sure this value is OK. */
			if (nopencl > nb_devices)
			{
				/* The user requires more OpenCL devices than there is available */
				fprintf(stderr,
					"# Warning: %d OpenCL devices requested. Only %d available.\n",
					nopencl, nb_devices);
				nopencl = nb_devices;
			}
			/* Let's make sure this value is OK. */
			if (nopencl > STARPU_MAXOPENCLDEVS)
			{
				fprintf(stderr,
					"# Warning: %d OpenCL devices requested. Only %d enabled. Use configure option --enable-maxopencldev=xxx to update the maximum value of supported OpenCL devices.\n",
					nopencl, STARPU_MAXOPENCLDEVS);
				nopencl = STARPU_MAXOPENCLDEVS;
			}
		}
	}

	topology->nopenclgpus = nopencl;
	STARPU_ASSERT(topology->nopenclgpus + topology->nworkers <= STARPU_NMAXWORKERS);

	_starpu_initialize_workers_opencl_gpuid(config);

	unsigned openclgpu;
	for (openclgpu = 0; openclgpu < topology->nopenclgpus; openclgpu++)
	{
		int devid = _starpu_get_next_opencl_gpuid(config);
		if (devid == -1)
		{ // There is no more devices left
			topology->nopenclgpus = openclgpu;
			break;
		}
		config->workers[topology->nworkers + openclgpu].arch = STARPU_OPENCL_WORKER;
		enum starpu_perf_archtype arch = (enum starpu_perf_archtype)((int)STARPU_OPENCL_DEFAULT + devid);
		config->workers[topology->nworkers + openclgpu].devid = devid;
		config->workers[topology->nworkers + openclgpu].perf_arch = arch;
		config->workers[topology->nworkers + openclgpu].worker_mask = STARPU_OPENCL;
		config->worker_mask |= STARPU_OPENCL;
	}

	topology->nworkers += topology->nopenclgpus;
#endif

#ifdef STARPU_USE_GORDON
	int ngordon = config->conf->ngordon;

	if (ngordon != 0)
	{
		if (ngordon == -1)
		{
			/* Nothing was specified, so let's choose ! */
			ngordon = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
		}
		else
		{
			STARPU_ASSERT(ngordon <= NMAXGORDONSPUS);
			if (ngordon > STARPU_MAXGORDONSPUS);
			{
				fprintf(stderr,
					"# Warning: %d Gordon CPUs devices requested. Only %d supported\n",
					ngordon, NMAXGORDONSPUS);
				ngordon = NMAXGORDONSPUS;
			}
		}
	}

	topology->ngordon_spus = ngordon;
	STARPU_ASSERT(topology->ngordon_spus + topology->nworkers <= STARPU_NMAXWORKERS);

	unsigned spu;
	for (spu = 0; spu < config->ngordon_spus; spu++)
	{
		config->workers[topology->nworkers + spu].arch = STARPU_GORDON_WORKER;
		config->workers[topology->nworkers + spu].perf_arch = STARPU_GORDON_DEFAULT;
		config->workers[topology->nworkers + spu].id = spu;
		config->workers[topology->nworkers + spu].worker_is_running = 0;
		config->workers[topology->nworkers + spu].worker_mask = STARPU_GORDON;
		config->worker_mask |= STARPU_GORDON;
	}

	topology->nworkers += topology->ngordon_spus;
#endif

/* we put the CPU section after the accelerator : in case there was an
 * accelerator found, we devote one cpu */
#ifdef STARPU_USE_CPU
	int ncpu = config->conf->ncpus;

	if (ncpu != 0)
	{
		if (ncpu == -1)
		{
			unsigned already_busy_cpus = (topology->ngordon_spus?1:0) + topology->ncudagpus + topology->nopenclgpus;
			long avail_cpus = topology->nhwcpus - already_busy_cpus;
			if (avail_cpus < 0)
				avail_cpus = 0;
			ncpu = STARPU_MIN(avail_cpus, STARPU_MAXCPUS);
		}
		else
		{
			if (ncpu > STARPU_MAXCPUS)
			{
				fprintf(stderr,
					"# Warning: %d CPU devices requested. Only %d enabled. Use configure option --enable-maxcpus=xxx to update the maximum value of supported CPU devices.\n",
					ncpu, STARPU_MAXCPUS);
				ncpu = STARPU_MAXCPUS;
			}
		}
	}


	topology->ncpus = ncpu;
	STARPU_ASSERT(topology->ncpus + topology->nworkers <= STARPU_NMAXWORKERS);

	unsigned cpu;
	for (cpu = 0; cpu < topology->ncpus; cpu++)
	{
		config->workers[topology->nworkers + cpu].arch = STARPU_CPU_WORKER;
		config->workers[topology->nworkers + cpu].perf_arch = STARPU_CPU_DEFAULT;
		config->workers[topology->nworkers + cpu].devid = cpu;
		config->workers[topology->nworkers + cpu].worker_mask = STARPU_CPU;
		config->worker_mask |= STARPU_CPU;
	}

	topology->nworkers += topology->ncpus;
#endif

	if (topology->nworkers == 0)
	{
                _STARPU_DEBUG("No worker found, aborting ...\n");
		return -ENODEV;
	}

	return 0;
}

/*
 * Bind workers on the different processors
 */
static void _starpu_initialize_workers_bindid(struct _starpu_machine_config *config)
{
	char *strval;
	unsigned i;

	struct starpu_machine_topology *topology = &config->topology;

	config->current_bindid = 0;

	/* conf->workers_bindid indicates the successive cpu identifier that
	 * should be used to bind the workers. It should be either filled
	 * according to the user's explicit parameters (from starpu_conf) or
	 * according to the STARPU_WORKERS_CPUID env. variable. Otherwise, a
	 * round-robin policy is used to distributed the workers over the
	 * cpus. */

	/* what do we use, explicit value, env. variable, or round-robin ? */
	if ((strval = getenv("STARPU_WORKERS_CPUID")))
	{
		/* STARPU_WORKERS_CPUID certainly contains less entries than
		 * STARPU_NMAXWORKERS, so we reuse its entries in a round robin
		 * fashion: "1 2" is equivalent to "1 2 1 2 1 2 .... 1 2". */
		unsigned wrap = 0;
		unsigned number_of_entries = 0;

		char *endptr;
		/* we use the content of the STARPU_WORKERS_CUDAID env. variable */
		for (i = 0; i < STARPU_NMAXWORKERS; i++)
		{
			if (!wrap)
			{
				long int val;
				val = strtol(strval, &endptr, 10);
				if (endptr != strval)
				{
					topology->workers_bindid[i] = (unsigned)(val % topology->nhwcpus);
					strval = endptr;
				}
				else
				{
					/* there must be at least one entry */
					STARPU_ASSERT(i != 0);
					number_of_entries = i;

					/* there is no more values in the string */
					wrap = 1;

					topology->workers_bindid[i] = topology->workers_bindid[0];
				}
			}
			else
			{
				topology->workers_bindid[i] = topology->workers_bindid[i % number_of_entries];
			}
		}
	}
	else if (config->conf->use_explicit_workers_bindid)
	{
		/* we use the explicit value from the user */
		memcpy(topology->workers_bindid,
			config->conf->workers_bindid,
			STARPU_NMAXWORKERS*sizeof(unsigned));
	}
	else
	{
		/* by default, we take a round robin policy */
		for (i = 0; i < STARPU_NMAXWORKERS; i++)
			topology->workers_bindid[i] = (unsigned)(i % topology->nhwcpus);
	}
}

/* This function gets the identifier of the next cpu on which to bind a
 * worker. In case a list of preferred cpus was specified, we look for a an
 * available cpu among the list if possible, otherwise a round-robin policy is
 * used. */
static inline int _starpu_get_next_bindid(struct _starpu_machine_config *config,
				int *preferred_binding, int npreferred)
{
	struct starpu_machine_topology *topology = &config->topology;

	unsigned found = 0;
	int current_preferred;

	for (current_preferred = 0; current_preferred < npreferred; current_preferred++)
	{
		if (found)
			break;

		unsigned requested_cpu = preferred_binding[current_preferred];

		/* can we bind the worker on the requested cpu ? */
		unsigned ind;
		for (ind = config->current_bindid; ind < topology->nhwcpus; ind++)
		{
			if (topology->workers_bindid[ind] == requested_cpu)
			{
				/* the cpu is available, we  use it ! In order
				 * to make sure that it will not be used again
				 * later on, we remove the entry from the list
				 * */
				topology->workers_bindid[ind] =
					topology->workers_bindid[config->current_bindid];
				topology->workers_bindid[config->current_bindid] = requested_cpu;

				found = 1;

				break;
			}
		}
	}

	unsigned i = ((config->current_bindid++) % STARPU_NMAXWORKERS);

	return (int)topology->workers_bindid[i];
}

void _starpu_bind_thread_on_cpu(struct _starpu_machine_config *config STARPU_ATTRIBUTE_UNUSED, unsigned cpuid)
{
	if (starpu_get_env_number("STARPU_WORKERS_NOBIND") > 0)
		return;
#ifdef STARPU_HAVE_HWLOC
	const struct hwloc_topology_support *support;

	_starpu_init_topology(config);

	support = hwloc_topology_get_support(config->topology.hwtopology);
	if (support->cpubind->set_thisthread_cpubind)
	{
		hwloc_obj_t obj = hwloc_get_obj_by_depth(config->topology.hwtopology, config->cpu_depth, cpuid);
		hwloc_cpuset_t set = obj->cpuset;
		int ret;

		hwloc_bitmap_singlify(set);
		ret = hwloc_set_cpubind(config->topology.hwtopology, set, HWLOC_CPUBIND_THREAD);
		if (ret)
		{
			perror("binding thread");
			STARPU_ABORT();
		}
	}

#elif defined(HAVE_PTHREAD_SETAFFINITY_NP) && defined(__linux__)
	int ret;
	/* fix the thread on the correct cpu */
	cpu_set_t aff_mask;
	CPU_ZERO(&aff_mask);
	CPU_SET(cpuid, &aff_mask);

	pthread_t self = pthread_self();

	ret = pthread_setaffinity_np(self, sizeof(aff_mask), &aff_mask);
	if (ret)
	{
		perror("binding thread");
		STARPU_ABORT();
	}

#elif defined(__MINGW32__) || defined(__CYGWIN__)
	DWORD mask = 1 << cpuid;
	if (!SetThreadAffinityMask(GetCurrentThread(), mask))
	{
		fprintf(stderr,"SetThreadMaskAffinity(%lx) failed\n", mask);
		STARPU_ABORT();
	}
#else
#warning no CPU binding support
#endif
}


void _starpu_bind_thread_on_cpus(struct _starpu_machine_config *config STARPU_ATTRIBUTE_UNUSED, struct _starpu_combined_worker *combined_worker)
{
#ifdef STARPU_HAVE_HWLOC
	const struct hwloc_topology_support *support;

	_starpu_init_topology(config);

	support = hwloc_topology_get_support(config->topology.hwtopology);
	if (support->cpubind->set_thisthread_cpubind)
	{
		hwloc_cpuset_t set = combined_worker->hwloc_cpu_set;
		int ret;

		ret = hwloc_set_cpubind(config->topology.hwtopology, set, HWLOC_CPUBIND_THREAD);
		if (ret)
		{
			perror("binding thread");
			STARPU_ABORT();
		}
	}
#else
#warning no parallel worker CPU binding support
#endif
}


static void _starpu_init_workers_binding(struct _starpu_machine_config *config)
{
	/* launch one thread per CPU */
	unsigned ram_memory_node;

	/* a single cpu is dedicated for the accelerators */
	int accelerator_bindid = -1;

	/* note that even if the CPU cpu are not used, we always have a RAM node */
	/* TODO : support NUMA  ;) */
	ram_memory_node = _starpu_register_memory_node(STARPU_CPU_RAM, -1);

	/* We will store all the busid of the different (src, dst) combinations
	 * in a matrix which we initialize here. */
	_starpu_initialize_busid_matrix();

	unsigned worker;
	for (worker = 0; worker < config->topology.nworkers; worker++)
	{
		unsigned memory_node = -1;
		unsigned is_a_set_of_accelerators = 0;
		struct _starpu_worker *workerarg = &config->workers[worker];

		/* Perhaps the worker has some "favourite" bindings  */
		int *preferred_binding = NULL;
		int npreferred = 0;

		/* select the memory node that contains worker's memory */
		switch (workerarg->arch)
		{
			case STARPU_CPU_WORKER:
			/* "dedicate" a cpu cpu to that worker */
				is_a_set_of_accelerators = 0;
				memory_node = ram_memory_node;
				_starpu_memory_node_worker_add(ram_memory_node);
				break;
#ifdef STARPU_USE_GORDON
			case STARPU_GORDON_WORKER:
				is_a_set_of_accelerators = 1;
				memory_node = ram_memory_node;
				_starpu_memory_node_worker_add(ram_memory_node);
				break;
#endif
#ifdef STARPU_USE_CUDA
			case STARPU_CUDA_WORKER:
				if (may_bind_automatically)
				{
					/* StarPU is allowed to bind threads automatically */
					preferred_binding = _starpu_get_cuda_affinity_vector(workerarg->devid);
					npreferred = config->topology.nhwcpus;
				}
				is_a_set_of_accelerators = 0;
				memory_node = _starpu_register_memory_node(STARPU_CUDA_RAM, workerarg->devid);
				_starpu_memory_node_worker_add(memory_node);

				_starpu_register_bus(0, memory_node);
				_starpu_register_bus(memory_node, 0);
#ifdef HAVE_CUDA_MEMCPY_PEER
				unsigned worker2;
				for (worker2 = 0; worker2 < worker; worker2++)
				{
					struct _starpu_worker *workerarg = &config->workers[worker];
					if (workerarg->arch == STARPU_CUDA_WORKER)
					{
						unsigned memory_node2 = starpu_worker_get_memory_node(worker2);
						_starpu_register_bus(memory_node2, memory_node);
						_starpu_register_bus(memory_node, memory_node2);
					}
				}
#endif
				break;
#endif

#ifdef STARPU_USE_OPENCL
		        case STARPU_OPENCL_WORKER:
				if (may_bind_automatically)
				{
					/* StarPU is allowed to bind threads automatically */
					preferred_binding = _starpu_get_opencl_affinity_vector(workerarg->devid);
					npreferred = config->topology.nhwcpus;
				}
				is_a_set_of_accelerators = 0;
				memory_node = _starpu_register_memory_node(STARPU_OPENCL_RAM, workerarg->devid);
				_starpu_memory_node_worker_add(memory_node);
				_starpu_register_bus(0, memory_node);
				_starpu_register_bus(memory_node, 0);
				break;
#endif

			default:
				STARPU_ABORT();
		}

		if (is_a_set_of_accelerators)
		{
			if (accelerator_bindid == -1)
				accelerator_bindid = _starpu_get_next_bindid(config, preferred_binding, npreferred);

			workerarg->bindid = accelerator_bindid;
		}
		else
		{
			workerarg->bindid = _starpu_get_next_bindid(config, preferred_binding, npreferred);
		}

		workerarg->memory_node = memory_node;

#ifdef __GLIBC__
		/* Save the initial cpuset */
		CPU_ZERO(&workerarg->initial_cpu_set);
		CPU_SET(workerarg->bindid, &workerarg->initial_cpu_set);
		CPU_ZERO(&workerarg->current_cpu_set);
		CPU_SET(workerarg->bindid, &workerarg->current_cpu_set);
#endif /* __GLIBC__ */

#ifdef STARPU_HAVE_HWLOC
		/* Clear the cpu set and set the cpu */
		workerarg->initial_hwloc_cpu_set = hwloc_bitmap_alloc();
		hwloc_bitmap_only(workerarg->initial_hwloc_cpu_set, workerarg->bindid);
		workerarg->current_hwloc_cpu_set = hwloc_bitmap_alloc();
		hwloc_bitmap_only(workerarg->current_hwloc_cpu_set, workerarg->bindid);

		/* Put the worker descriptor in the userdata field of the hwloc object describing the CPU */
		hwloc_obj_t worker_obj;
		worker_obj = hwloc_get_obj_by_depth(config->topology.hwtopology,
					config->cpu_depth, workerarg->bindid);
		worker_obj->userdata = &config->workers[worker];
#endif
	}
}


int _starpu_build_topology(struct _starpu_machine_config *config)
{
	int ret;

	ret = _starpu_init_machine_config(config);
	if (ret)
		return ret;

	/* for the data management library */
	_starpu_init_memory_nodes();

	_starpu_init_workers_binding(config);

	return 0;
}

void _starpu_destroy_topology(struct _starpu_machine_config *config __attribute__ ((unused)))
{
	/* cleanup StarPU internal data structures */
	_starpu_deinit_memory_nodes();

	unsigned worker;
	for (worker = 0; worker < config->topology.nworkers; worker++)
	{
#ifdef STARPU_HAVE_HWLOC
		struct _starpu_worker *workerarg = &config->workers[worker];
		hwloc_bitmap_free(workerarg->initial_hwloc_cpu_set);
		hwloc_bitmap_free(workerarg->current_hwloc_cpu_set);
#endif
	}

#ifdef STARPU_HAVE_HWLOC
	hwloc_topology_destroy(config->topology.hwtopology);
#endif

	topology_is_initialized = 0;
#ifdef STARPU_USE_CUDA
	devices_using_cuda = NULL;
#endif
#if defined(STARPU_USE_CUDA) || defined(STARPU_USE_OPENCL)
	may_bind_automatically = 0;
#endif
}
