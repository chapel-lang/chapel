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

#ifndef SOCL_DEBUG_H
#define SOCL_DEBUG_H

#include <../src/common/config.h>

#ifdef STARPU_VERBOSE
#define DEBUG
#include <stdio.h>
  #define DEBUG_MSG(...) do { if (!getenv("STARPU_SILENT")) { fprintf(stderr, "[SOCL] [%s] ", __func__); fprintf(stderr, __VA_ARGS__);}} while (0);
  #define DEBUG_MSG_NOHEAD(...) do { if (!getenv("STARPU_SILENT")) { fprintf(stderr, __VA_ARGS__);}} while (0);
  #define DEBUG_ERROR(...) do { if (!getenv("STARPU_SILENT")) { fprintf(stderr, "[SOCL] ERROR: "__VA_ARGS__); } exit(1); } while (0);
#else
   #define DEBUG_MSG(...) while(0);
   #define DEBUG_MSG_NOHEAD(...) while(0);
   #define DEBUG_ERROR(...) while(0);
#endif


#define ERROR_MSG(...) do { fprintf(stderr, "[SOCL] [%s] ERROR: ", __func__); fprintf(stderr, __VA_ARGS__); } while (0);
#define ERROR_MSG_NOHEAD(...) fprintf(stderr, __VA_ARGS__)
#define ERROR_STOP(...) do { ERROR_MSG(__VA_ARGS__); exit(1); } while(0);

#ifdef STARPU_VERBOSE
void DEBUG_CL(char *s, cl_int err);
#else
   #define DEBUG_CL(...) while(0);
#endif

#endif /* SOCL_DEBUG_H */
