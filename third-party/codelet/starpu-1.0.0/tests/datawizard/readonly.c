/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include <starpu.h>
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif
#include "../helper.h"

static void codelet(void *descr[], __attribute__ ((unused)) void *_args)
{
     FPRINTF(stderr, "codelet\n");
}

static struct starpu_codelet cl =
{
#ifdef STARPU_USE_OPENCL
     .opencl_funcs = {codelet, NULL},
#endif
     .nbuffers = 1,
     .modes = {STARPU_R}
};

int main(int argc, char **argv)
{
     int ret;
     int var = 42;
     starpu_data_handle_t handle;

     ret = starpu_init(NULL);
     if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
     STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

     int copy = starpu_asynchronous_copy_disabled();
     FPRINTF(stderr, "copy %d\n", copy);

     starpu_variable_data_register(&handle, 0, (uintptr_t)&var, sizeof(var));

     ret = starpu_insert_task(&cl,
			      STARPU_R, handle,
			      0);
     if (ret == -ENODEV) goto enodev;
     STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");

     starpu_task_wait_for_all();

     starpu_data_unregister(handle);

     starpu_shutdown();

     return 0;

enodev:
     starpu_data_unregister(handle);
     starpu_shutdown();
     /* yes, we do not perform the computation but we did detect that no one
      * could perform the kernel, so this is not an error from StarPU */
     fprintf(stderr, "WARNING: No one can execute this task\n");
     return STARPU_TEST_SKIPPED;
}
