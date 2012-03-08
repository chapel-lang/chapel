/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  Centre National de la Recherche Scientifique
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
#include "../helper.h"
#include <stdlib.h>

#if !defined(STARPU_HAVE_UNSETENV) || !defined(STARPU_USE_CPU)
#warning unsetenv is not defined. Skipping test
int main(int argc, char **argv)
{
	return STARPU_TEST_SKIPPED;
}
#else


static int check_cpu(int env_cpu, int conf_cpu, int expected_cpu, int *cpu)
{
	int ret;

	if (env_cpu != -1)
	{
		char string[50];
		sprintf(string, "STARPU_NCPUS=%d", env_cpu);
		putenv(string);
	}

	struct starpu_conf user_conf;
	starpu_conf_init(&user_conf);
	user_conf.ncpus = conf_cpu;
	ret = starpu_init(&user_conf);

	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	*cpu = starpu_cpu_worker_get_count();
	starpu_shutdown();

	if (env_cpu != -1)
	{
		unsetenv("STARPU_NCPUS");
	}

	if (expected_cpu == -1)
	{
		FPRINTF(stderr, "Number of CPUS: %3d\n", *cpu);
		return 0;
	}
	else
	{
		FPRINTF(stderr, "Number of CPUS: %3d -- Number of expected CPUs: %3d\n", *cpu, expected_cpu);
		return (*cpu != expected_cpu);
	}
}

int main(int argc, char **argv)
{
	int ret;
	int cpu, cpu_init;
	int cpu_test1, cpu_test2, cpu_test3;

	unsetenv("STARPU_NCPUS");

	ret = check_cpu(-1, -1, -1, &cpu_init);
	if (ret) return ret;

	if (cpu_init >= STARPU_MAXCPUS-5)
	{
		cpu_test1 = cpu_init-1;
		cpu_test2 = cpu_init-2;
		cpu_test3 = cpu_init-3;
	}
	else
	{
		cpu_test1 = cpu_init+1;
		cpu_test2 = cpu_init+2;
		cpu_test3 = cpu_init+3;
	}

	ret = check_cpu(cpu_test1, -1, cpu_test1, &cpu);
	if (ret) return ret;

	ret = check_cpu(-1, -1, -1, &cpu);
	if (ret) return ret;
	if (cpu != cpu_init)
	{
		FPRINTF(stderr, "The number of CPUs is incorrect\n");
		return 1;
	}

	ret = check_cpu(-1, cpu_test2, cpu_test2, &cpu);
	if (ret) return ret;

	ret = check_cpu(cpu_test3, cpu_test1, cpu_test3, &cpu);
	if (ret) return ret;

	STARPU_RETURN(ret);
}

 #endif
