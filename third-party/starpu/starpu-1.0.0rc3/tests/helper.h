/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
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

#ifndef _TESTS_HELPER_H
#define _TESTS_HELPER_H
#include <errno.h>
#include <common/utils.h>

#ifdef STARPU_HAVE_VALGRIND_H
#include <valgrind/valgrind.h>
#endif

#define STARPU_TEST_SKIPPED 77

//void *ALL_IS_OK = (void *)123456789L;
//void *ALL_IS_NOT_OK = (void *)987654321L;
//
//#define STARPU_CHECK_MALLOC(ptr) {if (!ptr) { fprintf(stderr, "starpu_malloc failed\n"); return 1; }}
//#define STARPU_CHECK_MALLOC_HAS_FAILED(ptr) {if (ptr) { fprintf(stderr, "starpu_malloc should have failed\n"); return 1; }}

//#define STARPU_CHECK_MALLOC_THREAD(ptr) {if (!ptr) { fprintf(stderr, "starpu_malloc failed\n"); return ALL_IS_NOT_OK; }}
//#define STARPU_CHECK_MALLOC_HAS_FAILED_THREAD(ptr) {if (ptr) { fprintf(stderr, "starpu_malloc should have failed\n"); return ALL_IS_NOT_OK; }}
//#define STARPU_CHECK_RETURN_VALUE_THREAD(err, message) {if (err < 0) { perror(message); return ALL_IS_NOT_OK; }}
//#define STARPU_CHECK_RETURN_VALUE_IS_THREAD(err, value, message) {if (err >= 0 || errno != value) { perror(message); return ALL_IS_NOT_OK; }}

//#define STARPU_TEST_OUTPUT
#define FPRINTF(ofile, fmt, args ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ##args); }} while(0)

#ifdef STARPU_HAVE_VALGRIND_H
static int _starpu_valgrind_print_once STARPU_ATTRIBUTE_UNUSED = 0;

#  define STARPU_SKIP_IF_VALGRIND \
	do \
	{								\
		if(RUNNING_ON_VALGRIND)					\
		{							\
			if (!_starpu_valgrind_print_once)		\
			{						\
				FPRINTF(stderr, "Running on valgrind, skipping the actual computations\n"); \
				_starpu_valgrind_print_once = 1;	\
			}						\
			return;						\
		}							\
	} while(0)
#else
#  define STARPU_SKIP_IF_VALGRIND
#endif

#ifdef STARPU_HAVE_VALGRIND_H
#  define STARPU_RETURN(ret) \
	do								\
	{								\
		if(RUNNING_ON_VALGRIND)					\
		{							\
			FPRINTF(stderr, "Running on valgrind, ignoring return value\n"); \
			return 0;					\
		} \
		else return ret; \
	} while(0)
#else
#  define STARPU_RETURN(ret) return ret
#endif
#endif /* _TESTS_HELPER_H */
