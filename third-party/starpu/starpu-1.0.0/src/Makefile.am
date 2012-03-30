# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
#
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

SUBDIRS =

CLEANFILES = *.gcno *.gcda *.linkinfo

EXTRA_DIST = dolib.c

ldflags =

if STARPU_HAVE_WINDOWS

LC_MESSAGES=C
export LC_MESSAGES

ldflags += -Xlinker --output-def -Xlinker .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.def

if STARPU_HAVE_MS_LIB
.libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.lib: libstarpu-@STARPU_EFFECTIVE_VERSION@.la dolib
	./dolib "$(STARPU_MS_LIB)" $(STARPU_MS_LIB_ARCH) .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.def libstarpu-0 .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.lib
all-local: .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.lib
endif STARPU_HAVE_MS_LIB

install-exec-hook:
	$(INSTALL) .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.def $(DESTDIR)$(libdir)
if STARPU_HAVE_MS_LIB
	$(INSTALL) .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.lib $(DESTDIR)$(libdir)
	$(INSTALL) .libs/libstarpu-@STARPU_EFFECTIVE_VERSION@.exp $(DESTDIR)$(libdir)
endif STARPU_HAVE_MS_LIB

endif STARPU_HAVE_WINDOWS

lib_LTLIBRARIES = libstarpu-@STARPU_EFFECTIVE_VERSION@.la

libstarpu_@STARPU_EFFECTIVE_VERSION@_la_CPPFLAGS = -I$(top_srcdir)/include/ -DBUILDING_STARPU

libstarpu_@STARPU_EFFECTIVE_VERSION@_la_CFLAGS = $(GLOBAL_AM_CFLAGS) $(HWLOC_CFLAGS) $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS) $(FXT_CFLAGS)
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_LIBADD = -lm $(HWLOC_LIBS) $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS) $(FXT_LIBS) $(STARPU_GLPK_LDFLAGS)
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_LDFLAGS = $(ldflags) -no-undefined									\
  -version-info $(LIBSTARPU_INTERFACE_CURRENT):$(LIBSTARPU_INTERFACE_REVISION):$(LIBSTARPU_INTERFACE_AGE)

noinst_HEADERS = 						\
	core/dependencies/data_concurrency.h			\
	core/dependencies/cg.h					\
	core/dependencies/tags.h				\
	core/dependencies/htable.h				\
	core/dependencies/implicit_data_deps.h			\
	core/sched_policy.h					\
	core/perfmodel/perfmodel.h				\
	core/perfmodel/regression.h				\
	core/jobs.h						\
	core/task.h						\
	core/workers.h						\
	core/topology.h						\
	core/debug.h						\
	core/errorcheck.h					\
	core/combined_workers.h					\
	core/parallel_task.h					\
	core/task_bundle.h					\
	sched_policies/detect_combined_workers.h		\
	sched_policies/fifo_queues.h				\
	sched_policies/deque_queues.h				\
	sched_policies/stack_queues.h				\
	datawizard/footprint.h					\
	datawizard/datawizard.h					\
	datawizard/data_request.h				\
	datawizard/filters.h					\
	datawizard/write_back.h					\
	datawizard/datastats.h					\
	datawizard/memalloc.h					\
	datawizard/copy_driver.h				\
	datawizard/coherency.h					\
	datawizard/sort_data_handles.h				\
	datawizard/memory_nodes.h				\
	datawizard/interfaces/data_interface.h			\
	common/barrier.h					\
	common/timing.h						\
	common/htable32.h					\
	common/list.h						\
	common/rwlock.h						\
	common/starpu_spinlock.h				\
	common/fxt.h						\
	common/utils.h						\
	common/barrier.h					\
	common/uthash.h						\
	drivers/driver_common/driver_common.h			\
	drivers/cpu/driver_cpu.h				\
	drivers/gordon/driver_gordon.h				\
	drivers/gordon/gordon_interface.h			\
	drivers/cuda/driver_cuda.h				\
	drivers/opencl/driver_opencl.h				\
	drivers/opencl/driver_opencl_utils.h			\
	debug/starpu_debug_helpers.h				\
	debug/traces/starpu_fxt.h				\
	profiling/bound.h					\
	profiling/profiling.h					\
	util/starpu_insert_task_utils.h				\
	util/starpu_data_cpy.h					\
	starpu_parameters.h					\
	top/starpu_top_message_queue.h				\
	top/starpu_top_connection.h				\
	top/starpu_top_core.h

libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES = 						\
	common/barrier.c					\
	common/hash.c 						\
	common/htable32.c					\
	common/rwlock.c						\
	common/starpu_spinlock.c				\
	common/timing.c						\
	common/fxt.c						\
	common/utils.c						\
	core/jobs.c						\
	core/task.c						\
	core/task_bundle.c					\
	core/workers.c						\
	core/combined_workers.c					\
	core/topology.c						\
	core/debug.c						\
	core/errorcheck.c					\
	core/progress_hook.c					\
	core/dependencies/cg.c					\
	core/dependencies/dependencies.c			\
	core/dependencies/implicit_data_deps.c			\
	core/dependencies/tags.c				\
	core/dependencies/task_deps.c				\
	core/dependencies/htable.c				\
	core/dependencies/data_concurrency.c			\
	core/perfmodel/perfmodel_history.c			\
	core/perfmodel/perfmodel_bus.c				\
	core/perfmodel/perfmodel.c				\
	core/perfmodel/regression.c				\
	core/sched_policy.c					\
	core/priorities.c					\
	core/parallel_task.c					\
	sched_policies/eager_central_policy.c			\
	sched_policies/eager_central_priority_policy.c		\
	sched_policies/work_stealing_policy.c			\
	sched_policies/deque_modeling_policy_data_aware.c	\
	sched_policies/heft.c					\
	sched_policies/random_policy.c				\
	sched_policies/stack_queues.c				\
	sched_policies/deque_queues.c				\
	sched_policies/fifo_queues.c				\
	sched_policies/detect_combined_workers.c		\
	sched_policies/parallel_heft.c				\
	sched_policies/parallel_greedy.c			\
	drivers/driver_common/driver_common.c			\
	datawizard/memory_nodes.c				\
	datawizard/write_back.c					\
	datawizard/coherency.c					\
	datawizard/data_request.c				\
	datawizard/progress.c					\
	datawizard/copy_driver.c				\
	datawizard/filters.c					\
	datawizard/sort_data_handles.c				\
	datawizard/memalloc.c					\
	datawizard/footprint.c					\
	datawizard/datastats.c					\
	datawizard/user_interactions.c				\
	datawizard/reduction.c					\
	datawizard/interfaces/data_interface.c			\
	datawizard/interfaces/bcsr_interface.c			\
	datawizard/interfaces/csr_interface.c			\
	datawizard/interfaces/matrix_filters.c			\
	datawizard/interfaces/matrix_interface.c		\
	datawizard/interfaces/block_filters.c			\
	datawizard/interfaces/block_interface.c			\
	datawizard/interfaces/vector_interface.c		\
	datawizard/interfaces/bcsr_filters.c			\
	datawizard/interfaces/csr_filters.c			\
	datawizard/interfaces/vector_filters.c			\
	datawizard/interfaces/variable_interface.c		\
	datawizard/interfaces/void_interface.c			\
	datawizard/interfaces/multiformat_interface.c           \
	util/malloc.c						\
	util/execute_on_all.c					\
	util/starpu_create_sync_task.c				\
	util/starpu_cublas.c					\
	util/file.c						\
	util/starpu_data_cpy.c					\
	util/starpu_insert_task.c				\
	util/starpu_insert_task_utils.c				\
	util/starpu_task_list.c					\
	debug/traces/starpu_fxt.c				\
	debug/traces/starpu_fxt_mpi.c				\
	debug/traces/starpu_fxt_dag.c				\
	debug/traces/starpu_paje.c				\
	debug/latency.c						\
	debug/structures_size.c					\
	profiling/profiling.c					\
	profiling/bound.c					\
	profiling/profiling_helpers.c				\
	top/starpu_top.c					\
	top/starpu_top_task.c					\
	top/starpu_top_message_queue.c				\
	top/starpu_top_connection.c

if STARPU_USE_CPU
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += drivers/cpu/driver_cpu.c
endif

if STARPU_USE_CUDA
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += drivers/cuda/driver_cuda.c
endif

if STARPU_USE_GORDON
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += drivers/gordon/driver_gordon.c
endif

if STARPU_USE_OPENCL
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += drivers/opencl/driver_opencl.c
libstarpu_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += drivers/opencl/driver_opencl_utils.c
endif

showcheck:
	-cat /dev/null
