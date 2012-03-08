/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
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

/*
 * Compute theoretical upper computation efficiency bound corresponding to
 * some actual execution.
 */

#ifndef __STARPU_BOUND_H__
#define __STARPU_BOUND_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Start recording tasks (resets stats).  `deps' tells whether dependencies
 * should be recorded too (this is quite expensive).  */
void starpu_bound_start(int deps, int prio);
/* Stop recording tasks */
void starpu_bound_stop(void);

/* Print the DAG that was recorded */
void starpu_bound_print_dot(FILE *output);

/* Get theoretical upper bound (in ms) (needs glpk support) */
void starpu_bound_compute(double *res, double *integer_res, int integer);

/* Emit Linear Programming system on output for the recorded tasks in lp format */
void starpu_bound_print_lp(FILE *output);

/* Emit Linear Programming system on output for the recorded tasks in mps format */
void starpu_bound_print_mps(FILE *output);

/* Emit statistics of actual execution vs theoretical upper bound */
void starpu_bound_print(FILE *output, int integer);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_BOUND_H__ */
