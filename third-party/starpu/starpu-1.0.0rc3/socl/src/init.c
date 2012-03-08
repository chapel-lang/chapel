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

  starpu_init(&conf);

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
