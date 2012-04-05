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

#ifndef __STARPU_RAND_H__
#define __STARPU_RAND_H__

#include <stdlib.h>
#include <starpu_config.h>

#ifdef STARPU_USE_DRAND48
#  define starpu_srand48(seed)				srand48(seed)
#  define starpu_drand48()				drand48()
#  define starpu_erand48(xsubi)				erand48(xsubi)
#  ifdef STARPU_USE_ERAND48_R
typedef struct drand48_data starpu_drand48_data;
#    define starpu_srand48_r(seed, buffer)		srand48_r(seed, buffer)
#    define starpu_erand48_r(xsubi, buffer, result)	erand48_r(xsubi, buffer, result)
#else
typedef int starpu_drand48_data;
#    define starpu_srand48_r(seed, buffer)		srand48(seed)
#    define starpu_erand48_r(xsubi, buffer, result)	do {*(result) = erand48(xsubi); } while (0)
#  endif
#else
typedef int starpu_drand48_data;
#  define starpu_srand48(seed)				srand(seed)
#  define starpu_drand48() 				(double)(rand()) / RAND_MAX
#  define starpu_erand48(xsubi)				starpu_drand48()
#  define starpu_srand48_r(seed, buffer) 		srand((unsigned int)seed)
#  define starpu_erand48_r(xsubi, buffer, result)	do {*(result) = ((double)(rand()) / RAND_MAX);} while (0)
#endif

#endif /* __STARPU_RAND_H__ */
