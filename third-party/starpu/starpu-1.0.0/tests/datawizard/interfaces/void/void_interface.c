/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  INRIA
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
#include "../test_interfaces.h"
#include "../../../helper.h"

static void fake_func(void *buffers[], void *arg)
{
	(void) buffers;
	(void) arg;
}

static starpu_data_handle_t void_handle;
static starpu_data_handle_t void2_handle;

struct test_config void_config =
{
	.cpu_func      = fake_func,
#ifdef STARPU_USE_CUDA
	.cuda_func     = fake_func,
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_func   = fake_func,
#endif
	.handle        = &void_handle,
	.dummy_handle  = &void2_handle,
	.copy_failed   = 0,
	.name          = "void_interface"
};

static void
register_data(void)
{
	starpu_void_data_register(&void_handle);
	starpu_void_data_register(&void2_handle);
}

static void
unregister_data(void)
{
	starpu_data_unregister(void_handle);
	starpu_data_unregister(void2_handle);
}

int
main(void)
{
	data_interface_test_summary *summary;
	struct starpu_conf conf =
	{
		.ncpus   = -1,
		.ncuda   = 2,
		.nopencl = 1
	};

	if (starpu_init(&conf) == -ENODEV || starpu_cpu_worker_get_count() == 0)
		goto enodev;

	register_data();

	summary = run_tests(&void_config);
	if (!summary)
		exit(EXIT_FAILURE);

	unregister_data();

	starpu_shutdown();

	data_interface_test_summary_print(stderr, summary);

	return data_interface_test_summary_success(summary);

enodev:
	return STARPU_TEST_SKIPPED;
}
