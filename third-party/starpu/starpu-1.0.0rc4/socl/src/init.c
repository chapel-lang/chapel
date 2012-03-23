/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011 University of Bordeaux
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

#include "socl.h"
#include "gc.h"
#include "mem_objects.h"

int _starpu_init_failed;

/**
 * Initialize SOCL
 */
__attribute__((constructor)) static void socl_init() {

  struct starpu_conf conf;
  starpu_conf_init(&conf);
  conf.ncuda = 0;
  putenv("STARPU_NCUDA=0");
  putenv("STARPU_NOPENCL=1");
  putenv("STARPU_NCPUS=1");

  mem_object_init();

  _starpu_init_failed = starpu_init(&conf);
  if (_starpu_init_failed != 0)
  {
       DEBUG_MSG("Error when calling starpu_init: %d\n", _starpu_init_failed);
  }
  else {
       if (starpu_cpu_worker_get_count() == 0)
       {
	    DEBUG_MSG("StarPU did not find any CPU device. SOCL needs at least 1 CPU.\n");
	    _starpu_init_failed = -ENODEV;
       }
       if (starpu_opencl_worker_get_count() == 0)
       {
	    DEBUG_MSG("StarPU didn't find any OpenCL device. Try disabling CUDA support in StarPU (export STARPU_NCUDA=0).\n");
	    _starpu_init_failed = -ENODEV;
       }
  }

  /* Disable dataflow implicit dependencies */
  starpu_data_set_default_sequential_consistency_flag(0);

  gc_start();
}

/**
 * Shutdown SOCL
 */
__attribute__((destructor)) static void socl_shutdown() {

  starpu_task_wait_for_all();

  gc_stop();

  starpu_task_wait_for_all();

  int active_entities = gc_active_entity_count();

  if (active_entities != 0)
    DEBUG_MSG("Unreleased entities: %d\n", active_entities);

  starpu_shutdown();
}
