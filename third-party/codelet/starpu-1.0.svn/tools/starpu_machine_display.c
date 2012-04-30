/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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
#include <config.h>
#include <stdio.h>
#include <starpu.h>

#define PROGNAME "starpu_machine_display"

static void display_worker_names(enum starpu_archtype type)
{
	unsigned nworkers = starpu_worker_get_count_by_type(type);

	int ids[nworkers];
	starpu_worker_get_ids_by_type(type, ids, nworkers);

	unsigned i;
	for (i = 0; i < nworkers; i++)
	{
		char name[256];
		starpu_worker_get_name(ids[i], name, 256);
		fprintf(stdout, "\t\t%s\n", name);
	}
}

static void display_combined_worker(unsigned workerid)
{
	int worker_size;
	int *combined_workerid;
	starpu_combined_worker_get_description(workerid, &worker_size, &combined_workerid);

	fprintf(stdout, "\t\t");

	int i;
	for (i = 0; i < worker_size; i++)
	{
		char name[256];

		starpu_worker_get_name(combined_workerid[i], name, 256);
		
		fprintf(stdout, "%s\t", name);
	}

	fprintf(stdout, "\n");
}

static void display_all_combined_workers(void)
{
	unsigned ncombined_workers = starpu_combined_worker_get_count();

	if (ncombined_workers == 0)
		return;

	unsigned nworkers = starpu_worker_get_count();

	fprintf(stdout, "\t%d Combined workers\n", ncombined_workers);

	unsigned i;
	for (i = 0; i < ncombined_workers; i++)
		display_combined_worker(nworkers + i);
}

static void parse_args(int argc, char **argv)
{
	if (argc == 1)
		return;

	if (argc > 2 || /* Argc should be either 1 or 2 */
	    strncmp(argv[1], "--help", 6) == 0 ||
	    strncmp(argv[1], "-h", 2) == 0)
	{
		(void) fprintf(stderr, "\
Show the processing units that StarPU can use, and the \
bandwitdh measured between the memory nodes.                  \n\
                                                              \n\
Usage: %s [OPTION]                                            \n\
                                                              \n\
Options:                                                      \n\
	-h, --help       display this help and exit           \n\
	-v, --version    output version information and exit  \n\
                                                              \n\
Report bugs to <" PACKAGE_BUGREPORT ">.",
PROGNAME);
	}
	else if (strncmp(argv[1], "--version", 9) == 0 ||
		 strncmp(argv[1], "-v", 2) == 0)
	{
		(void) fprintf(stderr, "%s %d.%d\n",
			PROGNAME, STARPU_MAJOR_VERSION, STARPU_MINOR_VERSION);
	}
	else
	{
		fprintf(stderr, "Unknown arg %s\n", argv[1]);
	}

	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	parse_args(argc, argv);

	/* Even if starpu_init returns -ENODEV, we should go on : we will just
	 * print that we found no device. */
	(void) starpu_init(NULL);

	unsigned ncpu = starpu_cpu_worker_get_count();
	unsigned ncuda = starpu_cuda_worker_get_count();
	unsigned nopencl = starpu_opencl_worker_get_count();

	fprintf(stdout, "StarPU has found :\n");

	fprintf(stdout, "\t%d CPU cores\n", ncpu);
	display_worker_names(STARPU_CPU_WORKER);

	fprintf(stdout, "\t%d CUDA devices\n", ncuda);
	display_worker_names(STARPU_CUDA_WORKER);

	fprintf(stdout, "\t%d OpenCL devices\n", nopencl);
	display_worker_names(STARPU_OPENCL_WORKER);

	display_all_combined_workers();

	starpu_bus_print_bandwidth(stdout);

	starpu_shutdown();

	return 0;
}
