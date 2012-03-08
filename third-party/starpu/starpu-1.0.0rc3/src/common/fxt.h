/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

#ifndef __FXT_H__
#define __FXT_H__


#ifndef _GNU_SOURCE
#define _GNU_SOURCE  /* ou _BSD_SOURCE ou _SVID_SOURCE */
#endif

#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <common/config.h>
#include <starpu.h>

/* some key to identify the worker kind */
#define _STARPU_FUT_APPS_KEY	0x100
#define _STARPU_FUT_CPU_KEY	0x101
#define _STARPU_FUT_CUDA_KEY	0x102
#define _STARPU_FUT_OPENCL_KEY	0x103

#define _STARPU_FUT_WORKER_INIT_START	0x5100
#define _STARPU_FUT_WORKER_INIT_END	0x5101

#define	_STARPU_FUT_START_CODELET_BODY	0x5102
#define	_STARPU_FUT_END_CODELET_BODY	0x5103

#define _STARPU_FUT_JOB_PUSH		0x5104
#define _STARPU_FUT_JOB_POP		0x5105

#define _STARPU_FUT_UPDATE_TASK_CNT	0x5106

#define _STARPU_FUT_START_FETCH_INPUT	0x5107
#define _STARPU_FUT_END_FETCH_INPUT	0x5108
#define _STARPU_FUT_START_PUSH_OUTPUT	0x5109
#define _STARPU_FUT_END_PUSH_OUTPUT	0x5110

#define _STARPU_FUT_TAG		0x5111
#define _STARPU_FUT_TAG_DEPS	0x5112

#define _STARPU_FUT_TASK_DEPS		0x5113

#define _STARPU_FUT_DATA_COPY		0x5114
#define _STARPU_FUT_WORK_STEALING	0x5115

#define _STARPU_FUT_WORKER_DEINIT_START	0x5116
#define _STARPU_FUT_WORKER_DEINIT_END	0x5117

#define _STARPU_FUT_WORKER_SLEEP_START	0x5118
#define _STARPU_FUT_WORKER_SLEEP_END	0x5119

#define _STARPU_FUT_USER_DEFINED_START	0x5120
#define _STARPU_FUT_USER_DEFINED_END	0x5121

#define	_STARPU_FUT_NEW_MEM_NODE		0x5122

#define	_STARPU_FUT_START_CALLBACK	0x5123
#define	_STARPU_FUT_END_CALLBACK		0x5124

#define	_STARPU_FUT_TASK_DONE		0x5125
#define	_STARPU_FUT_TAG_DONE		0x5126

#define	_STARPU_FUT_START_ALLOC		0x5127
#define	_STARPU_FUT_END_ALLOC		0x5128

#define	_STARPU_FUT_START_ALLOC_REUSE	0x5129
#define	_STARPU_FUT_END_ALLOC_REUSE	0x5130

#define	_STARPU_FUT_START_MEMRECLAIM	0x5131
#define	_STARPU_FUT_END_MEMRECLAIM	0x5132

#define	_STARPU_FUT_START_DRIVER_COPY	0x5133
#define	_STARPU_FUT_END_DRIVER_COPY	0x5134

#define	_STARPU_FUT_START_DRIVER_COPY_ASYNC	0x5135
#define	_STARPU_FUT_END_DRIVER_COPY_ASYNC	0x5136

#define	_STARPU_FUT_START_PROGRESS	0x5137
#define	_STARPU_FUT_END_PROGRESS		0x5138

#define _STARPU_FUT_USER_EVENT		0x5139

#define _STARPU_FUT_SET_PROFILING	0x513a

#define _STARPU_FUT_TASK_WAIT_FOR_ALL	0x513b

#ifdef STARPU_USE_FXT
#include <sys/syscall.h> /* pour les définitions de SYS_xxx */
#include <fxt/fxt.h>
#include <fxt/fut.h>

/* Initialize the FxT library. */
void _starpu_start_fxt_profiling(void);

/* Stop the FxT library, and generate the trace file. */
void _starpu_stop_fxt_profiling(void);

/* Associate the current processing unit to the identifier of the LWP that runs
 * the worker. */
void _starpu_fxt_register_thread(unsigned);

/* Sometimes we need something a little more specific than the wrappers from
 * FxT: these macro permit to put add an event with 3 (or 4) numbers followed
 * by a string. */
#define _STARPU_FUT_DO_PROBE3STR(CODE, P1, P2, P3, str)			\
do {									\
	/* No more than FXT_MAX_PARAMS args are allowed */		\
	/* we add a \0 just in case ... */				\
	size_t len = STARPU_MIN(strlen(str)+1, (FXT_MAX_PARAMS - 3)*sizeof(unsigned long));\
	unsigned nbargs_str = (len + sizeof(unsigned long) - 1)/(sizeof(unsigned long));\
	unsigned nbargs = 3 + nbargs_str;				\
	size_t total_len = FUT_SIZE(nbargs);				\
	unsigned long *futargs =					\
		fut_getstampedbuffer(FUT_CODE(CODE, nbargs), total_len);\
	*(futargs++) = (unsigned long)(P1);				\
	*(futargs++) = (unsigned long)(P2);				\
	*(futargs++) = (unsigned long)(P3);				\
	snprintf((char *)futargs, len, "%s", str);			\
	((char *)futargs)[len - 1] = '\0';				\
} while (0);

#define _STARPU_FUT_DO_PROBE4STR(CODE, P1, P2, P3, P4, str)		\
do {									\
	/* No more than FXT_MAX_PARAMS args are allowed */		\
	/* we add a \0 just in case ... */				\
	size_t len = STARPU_MIN(strlen(str)+1, (FXT_MAX_PARAMS - 4)*sizeof(unsigned long));\
	unsigned nbargs_str = (len + sizeof(unsigned long) - 1)/(sizeof(unsigned long));\
	unsigned nbargs = 4 + nbargs_str;				\
	size_t total_len = FUT_SIZE(nbargs);				\
	unsigned long *futargs =						\
		fut_getstampedbuffer(FUT_CODE(CODE, nbargs), total_len);\
	*(futargs++) = (unsigned long)(P1);				\
	*(futargs++) = (unsigned long)(P2);				\
	*(futargs++) = (unsigned long)(P3);				\
	*(futargs++) = (unsigned long)(P4);				\
	snprintf((char *)futargs, len, "%s", str);			\
	((char *)futargs)[len - 1] = '\0';				\
} while (0);

#define _STARPU_FUT_DO_PROBE5STR(CODE, P1, P2, P3, P4, P5, str)		\
do {									\
	/* No more than FXT_MAX_PARAMS args are allowed */		\
	/* we add a \0 just in case ... */				\
	size_t len = STARPU_MIN(strlen(str)+1, (FXT_MAX_PARAMS - 5)*sizeof(unsigned long));\
	unsigned nbargs_str = (len + sizeof(unsigned long) - 1)/(sizeof(unsigned long));\
	unsigned nbargs = 5 + nbargs_str;				\
	size_t total_len = FUT_SIZE(nbargs);				\
	unsigned long *futargs =					\
		fut_getstampedbuffer(FUT_CODE(CODE, nbargs), total_len);\
	*(futargs++) = (unsigned long)(P1);				\
	*(futargs++) = (unsigned long)(P2);				\
	*(futargs++) = (unsigned long)(P3);				\
	*(futargs++) = (unsigned long)(P4);				\
	*(futargs++) = (unsigned long)(P5);				\
	snprintf((char *)futargs, len, "%s", str);			\
	((char *)futargs)[len - 1] = '\0';				\
} while (0);



/* workerkind = _STARPU_FUT_CPU_KEY for instance */
#define _STARPU_TRACE_NEW_MEM_NODE(nodeid)			\
	FUT_DO_PROBE2(_STARPU_FUT_NEW_MEM_NODE, nodeid, syscall(SYS_gettid));

#define _STARPU_TRACE_WORKER_INIT_START(workerkind, devid, memnode)	\
	FUT_DO_PROBE4(_STARPU_FUT_WORKER_INIT_START, workerkind, devid, memnode, syscall(SYS_gettid));

#define _STARPU_TRACE_WORKER_INIT_END				\
	FUT_DO_PROBE1(_STARPU_FUT_WORKER_INIT_END, syscall(SYS_gettid));

#define _STARPU_TRACE_START_CODELET_BODY(job)				\
do {									\
        const char *model_name = _starpu_get_model_name((job));         \
	if (model_name)                                                 \
	{								\
		/* we include the symbol name */			\
		_STARPU_FUT_DO_PROBE3STR(_STARPU_FUT_START_CODELET_BODY, (job), syscall(SYS_gettid), 1, model_name); \
	}								\
	else {                                                          \
		FUT_DO_PROBE3(_STARPU_FUT_START_CODELET_BODY, (job), syscall(SYS_gettid), 0); \
	}								\
} while(0);

#define _STARPU_TRACE_END_CODELET_BODY(job, nimpl, archtype)			\
do {									\
	const size_t job_size = _starpu_job_get_data_size((job)->task->cl?(job)->task->cl->model:NULL, archtype, nimpl, (job));	\
	const uint32_t job_hash = _starpu_compute_buffers_footprint((job)->task->cl?(job)->task->cl->model:NULL, archtype, nimpl, (job));\
	FUT_DO_PROBE5(_STARPU_FUT_END_CODELET_BODY, (job), (job_size), (job_hash), (archtype), syscall(SYS_gettid));	\
} while(0);

#define _STARPU_TRACE_START_CALLBACK(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_START_CALLBACK, job, syscall(SYS_gettid));

#define _STARPU_TRACE_END_CALLBACK(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_END_CALLBACK, job, syscall(SYS_gettid));

#define _STARPU_TRACE_JOB_PUSH(task, prio)	\
	FUT_DO_PROBE3(_STARPU_FUT_JOB_PUSH, task, prio, syscall(SYS_gettid));

#define _STARPU_TRACE_JOB_POP(task, prio)	\
	FUT_DO_PROBE3(_STARPU_FUT_JOB_POP, task, prio, syscall(SYS_gettid));

#define _STARPU_TRACE_UPDATE_TASK_CNT(counter)	\
	FUT_DO_PROBE2(_STARPU_FUT_UPDATE_TASK_CNT, counter, syscall(SYS_gettid))

#define _STARPU_TRACE_START_FETCH_INPUT(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_START_FETCH_INPUT, job, syscall(SYS_gettid));

#define _STARPU_TRACE_END_FETCH_INPUT(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_END_FETCH_INPUT, job, syscall(SYS_gettid));

#define _STARPU_TRACE_START_PUSH_OUTPUT(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_START_PUSH_OUTPUT, job, syscall(SYS_gettid));

#define _STARPU_TRACE_END_PUSH_OUTPUT(job)	\
	FUT_DO_PROBE2(_STARPU_FUT_END_PUSH_OUTPUT, job, syscall(SYS_gettid));

#define _STARPU_TRACE_TAG(tag, job)	\
	FUT_DO_PROBE2(_STARPU_FUT_TAG, tag, (job)->job_id)

#define _STARPU_TRACE_TAG_DEPS(tag_child, tag_father)	\
	FUT_DO_PROBE2(_STARPU_FUT_TAG_DEPS, tag_child, tag_father)

#define _STARPU_TRACE_TASK_DEPS(job_prev, job_succ)	\
	FUT_DO_PROBE2(_STARPU_FUT_TASK_DEPS, (job_prev)->job_id, (job_succ)->job_id)

#define _STARPU_TRACE_GHOST_TASK_DEPS(ghost_prev_id, job_succ_id)		\
	FUT_DO_PROBE2(_STARPU_FUT_TASK_DEPS, (ghost_prev_id), (job_succ_id))

#define _STARPU_TRACE_TASK_DONE(job)						\
do {										\
	unsigned exclude_from_dag = (job)->exclude_from_dag;			\
        const char *model_name = _starpu_get_model_name((job));                       \
	if (model_name)					                        \
	{									\
		_STARPU_FUT_DO_PROBE4STR(_STARPU_FUT_TASK_DONE, (job)->job_id, syscall(SYS_gettid), (long unsigned)exclude_from_dag, 1, model_name);\
	}									\
	else {									\
		FUT_DO_PROBE4(_STARPU_FUT_TASK_DONE, (job)->job_id, syscall(SYS_gettid), (long unsigned)exclude_from_dag, 0);\
	}									\
} while(0);

#define _STARPU_TRACE_TAG_DONE(tag)						\
do {										\
        struct _starpu_job *job = (tag)->job;                                  \
        const char *model_name = _starpu_get_model_name((job));                       \
	if (model_name)                                                         \
	{									\
          _STARPU_FUT_DO_PROBE3STR(_STARPU_FUT_TAG_DONE, (tag)->id, syscall(SYS_gettid), 1, model_name); \
	}									\
	else {									\
		FUT_DO_PROBE3(_STARPU_FUT_TAG_DONE, (tag)->id, syscall(SYS_gettid), 0);\
	}									\
} while(0);

#define _STARPU_TRACE_DATA_COPY(src_node, dst_node, size)	\
	FUT_DO_PROBE3(_STARPU_FUT_DATA_COPY, src_node, dst_node, size)

#define _STARPU_TRACE_START_DRIVER_COPY(src_node, dst_node, size, com_id)	\
	FUT_DO_PROBE4(_STARPU_FUT_START_DRIVER_COPY, src_node, dst_node, size, com_id)

#define _STARPU_TRACE_END_DRIVER_COPY(src_node, dst_node, size, com_id)	\
	FUT_DO_PROBE4(_STARPU_FUT_END_DRIVER_COPY, src_node, dst_node, size, com_id)

#define _STARPU_TRACE_START_DRIVER_COPY_ASYNC(src_node, dst_node)	\
	FUT_DO_PROBE2(_STARPU_FUT_START_DRIVER_COPY_ASYNC, src_node, dst_node)

#define _STARPU_TRACE_END_DRIVER_COPY_ASYNC(src_node, dst_node)	\
	FUT_DO_PROBE2(_STARPU_FUT_END_DRIVER_COPY, src_node, dst_node)

#define _STARPU_TRACE_WORK_STEALING(empty_q, victim_q)		\
	FUT_DO_PROBE2(_STARPU_FUT_WORK_STEALING, empty_q, victim_q)

#define _STARPU_TRACE_WORKER_DEINIT_START			\
	FUT_DO_PROBE1(_STARPU_FUT_WORKER_DEINIT_START, syscall(SYS_gettid));

#define _STARPU_TRACE_WORKER_DEINIT_END(workerkind)		\
	FUT_DO_PROBE2(_STARPU_FUT_WORKER_DEINIT_END, workerkind, syscall(SYS_gettid));

#define _STARPU_TRACE_WORKER_SLEEP_START	\
	FUT_DO_PROBE1(_STARPU_FUT_WORKER_SLEEP_START, syscall(SYS_gettid));

#define _STARPU_TRACE_WORKER_SLEEP_END	\
	FUT_DO_PROBE1(_STARPU_FUT_WORKER_SLEEP_END, syscall(SYS_gettid));

#define _STARPU_TRACE_USER_DEFINED_START	\
	FUT_DO_PROBE1(_STARPU_FUT_USER_DEFINED_START, syscall(SYS_gettid));

#define _STARPU_TRACE_USER_DEFINED_END		\
	FUT_DO_PROBE1(_STARPU_FUT_USER_DEFINED_END, syscall(SYS_gettid));

#define _STARPU_TRACE_START_ALLOC(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_START_ALLOC, memnode, syscall(SYS_gettid));
	
#define _STARPU_TRACE_END_ALLOC(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_END_ALLOC, memnode, syscall(SYS_gettid));

#define _STARPU_TRACE_START_ALLOC_REUSE(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_START_ALLOC_REUSE, memnode, syscall(SYS_gettid));
	
#define _STARPU_TRACE_END_ALLOC_REUSE(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_END_ALLOC_REUSE, memnode, syscall(SYS_gettid));
	
#define _STARPU_TRACE_START_MEMRECLAIM(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_START_MEMRECLAIM, memnode, syscall(SYS_gettid));
	
#define _STARPU_TRACE_END_MEMRECLAIM(memnode)		\
	FUT_DO_PROBE2(_STARPU_FUT_END_MEMRECLAIM, memnode, syscall(SYS_gettid));
	
/* We skip these events becasue they are called so often that they cause FxT to
 * fail and make the overall trace unreadable anyway. */
#define _STARPU_TRACE_START_PROGRESS(memnode)		\
	do {} while (0);
//	FUT_DO_PROBE2(_STARPU_FUT_START_PROGRESS, memnode, syscall(SYS_gettid));

#define _STARPU_TRACE_END_PROGRESS(memnode)		\
	do {} while (0);
	//FUT_DO_PROBE2(_STARPU_FUT_END_PROGRESS, memnode, syscall(SYS_gettid));
	
#define _STARPU_TRACE_USER_EVENT(code)			\
	FUT_DO_PROBE2(_STARPU_FUT_USER_EVENT, code, syscall(SYS_gettid));

#define _STARPU_TRACE_SET_PROFILING(status)		\
	FUT_DO_PROBE2(_STARPU_FUT_SET_PROFILING, status, syscall(SYS_gettid));

#define _STARPU_TRACE_TASK_WAIT_FOR_ALL			\
	FUT_DO_PROBE0(_STARPU_FUT_TASK_WAIT_FOR_ALL)

#else // !STARPU_USE_FXT

/* Dummy macros in case FxT is disabled */
#define _STARPU_TRACE_NEW_MEM_NODE(nodeid)	do {} while(0);
#define _STARPU_TRACE_WORKER_INIT_START(a,b,c)	do {} while(0);
#define _STARPU_TRACE_WORKER_INIT_END		do {} while(0);
#define _STARPU_TRACE_START_CODELET_BODY(job)	do {} while(0);
#define _STARPU_TRACE_END_CODELET_BODY(job, nimpl, a)	do {} while(0);
#define _STARPU_TRACE_START_CALLBACK(job)	do {} while(0);
#define _STARPU_TRACE_END_CALLBACK(job)		do {} while(0);
#define _STARPU_TRACE_JOB_PUSH(task, prio)	do {} while(0);
#define _STARPU_TRACE_JOB_POP(task, prio)	do {} while(0);
#define _STARPU_TRACE_UPDATE_TASK_CNT(counter)	do {} while(0);
#define _STARPU_TRACE_START_FETCH_INPUT(job)	do {} while(0);
#define _STARPU_TRACE_END_FETCH_INPUT(job)	do {} while(0);
#define _STARPU_TRACE_START_PUSH_OUTPUT(job)	do {} while(0);
#define _STARPU_TRACE_END_PUSH_OUTPUT(job)	do {} while(0);
#define _STARPU_TRACE_TAG(tag, job)	do {} while(0);
#define _STARPU_TRACE_TAG_DEPS(a, b)	do {} while(0);
#define _STARPU_TRACE_TASK_DEPS(a, b)		do {} while(0);
#define _STARPU_TRACE_GHOST_TASK_DEPS(a, b)	do {} while(0);
#define _STARPU_TRACE_TASK_DONE(a)		do {} while(0);
#define _STARPU_TRACE_TAG_DONE(a)		do {} while(0);
#define _STARPU_TRACE_DATA_COPY(a, b, c)		do {} while(0);
#define _STARPU_TRACE_START_DRIVER_COPY(a,b,c,d)	do {} while(0);
#define _STARPU_TRACE_END_DRIVER_COPY(a,b,c,d)	do {} while(0);
#define _STARPU_TRACE_START_DRIVER_COPY_ASYNC(a,b)	do {} while(0);
#define _STARPU_TRACE_END_DRIVER_COPY_ASYNC(a,b)	do {} while(0);
#define _STARPU_TRACE_WORK_STEALING(a, b)	do {} while(0);
#define _STARPU_TRACE_WORKER_DEINIT_START	do {} while(0);
#define _STARPU_TRACE_WORKER_DEINIT_END(a)	do {} while(0);
#define _STARPU_TRACE_WORKER_SLEEP_START		do {} while(0);
#define _STARPU_TRACE_WORKER_SLEEP_END		do {} while(0);
#define _STARPU_TRACE_USER_DEFINED_START		do {} while(0);
#define _STARPU_TRACE_USER_DEFINED_END		do {} while(0);
#define _STARPU_TRACE_START_ALLOC(memnode)	do {} while(0);
#define _STARPU_TRACE_END_ALLOC(memnode)		do {} while(0);
#define _STARPU_TRACE_START_ALLOC_REUSE(a)	do {} while(0);
#define _STARPU_TRACE_END_ALLOC_REUSE(a)		do {} while(0);
#define _STARPU_TRACE_START_MEMRECLAIM(memnode)	do {} while(0);
#define _STARPU_TRACE_END_MEMRECLAIM(memnode)	do {} while(0);
#define _STARPU_TRACE_START_PROGRESS(memnode)	do {} while(0);
#define _STARPU_TRACE_END_PROGRESS(memnode)	do {} while(0);
#define _STARPU_TRACE_USER_EVENT(code)		do {} while(0);
#define _STARPU_TRACE_SET_PROFILING(status)	do {} while(0);
#define _STARPU_TRACE_TASK_WAIT_FOR_ALL		do {} while(0);

#endif // STARPU_USE_FXT

#endif // __FXT_H__
