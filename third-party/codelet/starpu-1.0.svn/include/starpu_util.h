/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
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

#ifndef __STARPU_UTIL_H__
#define __STARPU_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <starpu.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define STARPU_POISON_PTR	((void *)0xdeadbeef)

#define STARPU_MIN(a,b)	((a)<(b)?(a):(b))
#define STARPU_MAX(a,b)	((a)<(b)?(b):(a))

#ifdef STARPU_NO_ASSERT
#define STARPU_ASSERT(x)		do { (void) (x);} while(0)
#define STARPU_ASSERT_MSG(x, msg)	do { (void) (x);} while(0)
#else
#  if defined(__CUDACC__) && defined(STARPU_HAVE_WINDOWS)
#    define STARPU_ASSERT(x)		do { if (!(x)) *(int*)NULL = 0; } while(0)
#    define STARPU_ASSERT_MSG(x, msg)	do { if (!(x)) { fprintf(stderr, "%s\n", msg); *(int*)NULL = 0; }} while(0)
#  else
#    define STARPU_ASSERT(x)		assert(x)
#    define STARPU_ASSERT_MSG(x, msg)	do { if (!(x)) { fprintf(stderr, "%s\n", msg); } ; assert(x); } while(0)

#  endif
#endif

#define STARPU_ABORT() do {                                          \
	fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, __func__); \
	abort();                                                     \
} while(0)

#if defined(STARPU_HAVE_STRERROR_R)
#  define STARPU_CHECK_RETURN_VALUE(err, message) {if (err < 0) { \
			char xmessage[256]; strerror_r(-err, xmessage, 256); \
			fprintf(stderr, "StarPU function <%s> returned unexpected value: <%d:%s>\n", message, err, xmessage); \
			STARPU_ASSERT(0); }}
#  define STARPU_CHECK_RETURN_VALUE_IS(err, value, message) {if (err != value) { \
			char xmessage[256]; strerror_r(-err, xmessage, 256); \
			fprintf(stderr, "StarPU function <%s> returned unexpected value: <%d:%s>\n", message, err, xmessage); \
			STARPU_ASSERT(0); }}
#else
#  define STARPU_CHECK_RETURN_VALUE(err, message) {if (err < 0) {		\
			fprintf(stderr, "StarPU function <%s> returned unexpected value: <%d>\n", message, err); \
			STARPU_ASSERT(0); }}
#  define STARPU_CHECK_RETURN_VALUE_IS(err, value, message) {if (err != value) { \
			fprintf(stderr, "StarPU function <%s> returned unexpected value: <%d>\n", message, err); \
			STARPU_ASSERT(0); }}
#endif /* STARPU_HAVE_STRERROR_R */

/* Return true (non-zero) if GCC version MAJ.MIN or later is being used
 * (macro taken from glibc.)  */
#if defined __GNUC__ && defined __GNUC_MINOR__
# define STARPU_GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define STARPU_GNUC_PREREQ(maj, min) 0
#endif

#ifdef __GNUC__
#  define STARPU_UNLIKELY(expr)          (__builtin_expect(!!(expr),0))
#  define STARPU_LIKELY(expr)            (__builtin_expect(!!(expr),1))
#  define STARPU_ATTRIBUTE_UNUSED                  __attribute__((unused))
#  define STARPU_ATTRIBUTE_INTERNAL      __attribute__ ((visibility ("internal")))
#else
#  define STARPU_UNLIKELY(expr)          (expr)
#  define STARPU_LIKELY(expr)            (expr)
#  define STARPU_ATTRIBUTE_UNUSED
#  define STARPU_ATTRIBUTE_INTERNAL
#endif

#if STARPU_GNUC_PREREQ(3, 1) && !defined(BUILDING_STARPU) && !defined(STARPU_USE_DEPRECATED_API)
#define STARPU_DEPRECATED  __attribute__((__deprecated__))
#else
#define STARPU_DEPRECATED
#endif /* __GNUC__ */

#if STARPU_GNUC_PREREQ(3,3)
#define STARPU_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#else
#define STARPU_WARN_UNUSED_RESULT
#endif /* __GNUC__ */

#if defined(__i386__) || defined(__x86_64__)

static __inline unsigned starpu_cmpxchg(unsigned *ptr, unsigned old, unsigned next)
{
	__asm__ __volatile__("lock cmpxchgl %2,%1": "+a" (old), "+m" (*ptr) : "q" (next) : "memory");
	return old;
}
static __inline unsigned starpu_xchg(unsigned *ptr, unsigned next)
{
	/* Note: xchg is always locked already */
	__asm__ __volatile__("xchgl %1,%0": "+m" (*ptr), "+q" (next) : : "memory");
	return next;
}
#define STARPU_HAVE_XCHG
#endif

#define STARPU_ATOMIC_SOMETHING(name,expr) \
static __inline unsigned starpu_atomic_##name(unsigned *ptr, unsigned value) \
{ \
	unsigned old, next; \
	while (1) \
	{ \
		old = *ptr; \
		next = expr; \
		if (starpu_cmpxchg(ptr, old, next) == old) \
			break; \
	}; \
	return expr; \
}

#ifdef STARPU_HAVE_SYNC_FETCH_AND_ADD
#define STARPU_ATOMIC_ADD(ptr, value)  (__sync_fetch_and_add ((ptr), (value)) + (value))
#elif defined(STARPU_HAVE_XCHG)
STARPU_ATOMIC_SOMETHING(add, old + value)
#define STARPU_ATOMIC_ADD(ptr, value) starpu_atomic_add(ptr, value)
#endif

#ifdef STARPU_HAVE_SYNC_FETCH_AND_OR
#define STARPU_ATOMIC_OR(ptr, value)  (__sync_fetch_and_or ((ptr), (value)))
#elif defined(STARPU_HAVE_XCHG)
STARPU_ATOMIC_SOMETHING(or, old | value)
#define STARPU_ATOMIC_OR(ptr, value) starpu_atomic_or(ptr, value)
#endif

#ifdef STARPU_HAVE_SYNC_BOOL_COMPARE_AND_SWAP
#define STARPU_BOOL_COMPARE_AND_SWAP(ptr, old, value)  (__sync_bool_compare_and_swap ((ptr), (old), (value)))
#elif defined(STARPU_HAVE_XCHG)
#define STARPU_BOOL_COMPARE_AND_SWAP(ptr, old, value) (starpu_cmpxchg((ptr), (old), (value)) == (old))
#endif

#ifdef STARPU_HAVE_SYNC_LOCK_TEST_AND_SET
#define STARPU_TEST_AND_SET(ptr, value) (__sync_lock_test_and_set ((ptr), (value)))
#define STARPU_RELEASE(ptr) (__sync_lock_release ((ptr)))
#elif defined(STARPU_HAVE_XCHG)
#define STARPU_TEST_AND_SET(ptr, value) (starpu_xchg((ptr), (value)))
#define STARPU_RELEASE(ptr) (starpu_xchg((ptr), 0))
#endif

#ifdef STARPU_HAVE_SYNC_SYNCHRONIZE
#define STARPU_SYNCHRONIZE() __sync_synchronize()
#elif defined(__i386__)
#define STARPU_SYNCHRONIZE() __asm__ __volatile__("lock; addl $0,0(%%esp)" ::: "memory")
#elif defined(__x86_64__)
#define STARPU_SYNCHRONIZE() __asm__ __volatile__("mfence" ::: "memory")
#elif defined(__ppc__) || defined(__ppc64__)
#define STARPU_SYNCHRONIZE() __asm__ __volatile__("sync" ::: "memory")
#endif

#ifdef __cplusplus
}
#endif

/* Include this only here so that <starpu_data_interfaces.h> can use the
 * macros above.  */
#include <starpu_task.h>

#ifdef __cplusplus
extern "C"
{
#endif

static __inline int starpu_get_env_number(const char *str)
{
	char *strval;

	strval = getenv(str);
	if (strval)
	{
		/* the env variable was actually set */
		unsigned val;
		char *check;

		val = (int)strtol(strval, &check, 10);
		STARPU_ASSERT(strcmp(check, "\0") == 0);

		/* fprintf(stderr, "ENV %s WAS %d\n", str, val); */
		return val;
	}
	else
	{
		/* there is no such env variable */
		/* fprintf("There was no %s ENV\n", str); */
		return -1;
	}
}

/* Add an event in the execution trace if FxT is enabled */
void starpu_trace_user_event(unsigned long code);

/* Some helper functions for application using CUBLAS kernels */
void starpu_helper_cublas_init(void);
void starpu_helper_cublas_shutdown(void);

/* Call func(arg) on every worker matching the "where" mask (eg.
 * STARPU_CUDA|STARPU_CPU to execute the function on every CPU and every CUDA
 * device). This function is synchronous, but the different workers may execute
 * the function in parallel.
 * */
void starpu_execute_on_each_worker(void (*func)(void *), void *arg, uint32_t where);

/* This creates (and submits) an empty task that unlocks a tag once all its
 * dependencies are fulfilled. */
void starpu_create_sync_task(starpu_tag_t sync_tag, unsigned ndeps, starpu_tag_t *deps,
				void (*callback)(void *), void *callback_arg);

/* Copy the content of the src_handle into the dst_handle handle.  The
 * asynchronous parameter indicates whether the function should block or not.
 * In the case of an asynchronous call, it is possible to synchronize with the
 * termination of this operation either by the means of implicit dependencies
 * (if enabled) or by calling starpu_task_wait_for_all(). If callback_func is
 * not NULL, this callback function is executed after the handle has been
 * copied, and it is given the callback_arg pointer as argument.*/
int starpu_data_cpy(starpu_data_handle_t dst_handle, starpu_data_handle_t src_handle, int asynchronous, void (*callback_func)(void*), void *callback_arg);

/* Constants used by the starpu_insert_task helper to determine the different types of argument */
#define STARPU_VALUE		(1<<4)	/* Pointer to a constant value */
#define STARPU_CALLBACK		(1<<5)	/* Callback function */
#define STARPU_CALLBACK_WITH_ARG	(1<<6)	/* Callback function */
#define STARPU_CALLBACK_ARG	(1<<7)	/* Argument of the callback function (of type void *) */
#define STARPU_PRIORITY		(1<<8)	/* Priority associated to the task */
#define STARPU_EXECUTE_ON_NODE	(1<<9)	/* Used by MPI to define which task is going to execute the codelet */
#define STARPU_EXECUTE_ON_DATA	(1<<10)	/* Used by MPI to define which task is going to execute the codelet */

/* Wrapper to create a task. */
int starpu_insert_task(struct starpu_codelet *cl, ...);

/* Retrieve the arguments of type STARPU_VALUE associated to a task
 * automatically created using starpu_insert_task. */
void starpu_codelet_unpack_args(void *cl_arg, ...);

/* Pack arguments of type STARPU_VALUE into a buffer which can be
 * given to a codelet and later unpacked with starpu_codelet_unpack_args */
void starpu_codelet_pack_args(char **arg_buffer, size_t *arg_buffer_size, ...);

#ifdef __cplusplus
}
#endif

#endif /* __STARPU_UTIL_H__ */
