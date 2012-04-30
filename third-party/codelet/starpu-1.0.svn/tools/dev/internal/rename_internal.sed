# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2010  Université de Bordeaux 1
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

s/\bstruct starpu_priority_taskq_s\b/struct _starpu_priority_taskq/g
s/\bSTARPU_FUT_APPS_KEY\b/_STARPU_FUT_APPS_KEY/g
s/\bSTARPU_FUT_CPU_KEY\b/_STARPU_FUT_CPU_KEY/g
s/\bSTARPU_FUT_CUDA_KEY\b/_STARPU_FUT_CUDA_KEY/g
s/\bSTARPU_FUT_DATA_COPY\b/_STARPU_FUT_DATA_COPY/g
s/\bSTARPU_FUT_DO_PROBE3STR\b/_STARPU_FUT_DO_PROBE3STR/g
s/\bSTARPU_FUT_DO_PROBE4STR\b/_STARPU_FUT_DO_PROBE4STR/g
s/\bSTARPU_FUT_DO_PROBE5STR\b/_STARPU_FUT_DO_PROBE5STR/g
s/\bSTARPU_FUT_END_ALLOC\b/_STARPU_FUT_END_ALLOC/g
s/\bSTARPU_FUT_END_ALLOC_REUSE\b/_STARPU_FUT_END_ALLOC_REUSE/g
s/\bSTARPU_FUT_END_CALLBACK\b/_STARPU_FUT_END_CALLBACK/g
s/\bSTARPU_FUT_END_CODELET_BODY\b/_STARPU_FUT_END_CODELET_BODY/g
s/\bSTARPU_FUT_END_DRIVER_COPY\b/_STARPU_FUT_END_DRIVER_COPY/g
s/\bSTARPU_FUT_END_DRIVER_COPY_ASYNC\b/_STARPU_FUT_END_DRIVER_COPY_ASYNC/g
s/\bSTARPU_FUT_END_FETCH_INPUT\b/_STARPU_FUT_END_FETCH_INPUT/g
s/\bSTARPU_FUT_END_MEMRECLAIM\b/_STARPU_FUT_END_MEMRECLAIM/g
s/\bSTARPU_FUT_END_PROGRESS\b/_STARPU_FUT_END_PROGRESS/g
s/\bSTARPU_FUT_END_PUSH_OUTPUT\b/_STARPU_FUT_END_PUSH_OUTPUT/g
s/\bSTARPU_FUT_JOB_POP\b/_STARPU_FUT_JOB_POP/g
s/\bSTARPU_FUT_JOB_PUSH\b/_STARPU_FUT_JOB_PUSH/g
s/\bSTARPU_FUT_NEW_MEM_NODE\b/_STARPU_FUT_NEW_MEM_NODE/g
s/\bSTARPU_FUT_OPENCL_KEY\b/_STARPU_FUT_OPENCL_KEY/g
s/\bSTARPU_FUT_SET_PROFILING\b/_STARPU_FUT_SET_PROFILING/g
s/\bSTARPU_FUT_START_ALLOC\b/_STARPU_FUT_START_ALLOC/g
s/\bSTARPU_FUT_START_ALLOC_REUSE\b/_STARPU_FUT_START_ALLOC_REUSE/g
s/\bSTARPU_FUT_START_CALLBACK\b/_STARPU_FUT_START_CALLBACK/g
s/\bSTARPU_FUT_START_CODELET_BODY\b/_STARPU_FUT_START_CODELET_BODY/g
s/\bSTARPU_FUT_START_DRIVER_COPY\b/_STARPU_FUT_START_DRIVER_COPY/g
s/\bSTARPU_FUT_START_DRIVER_COPY_ASYNC\b/_STARPU_FUT_START_DRIVER_COPY_ASYNC/g
s/\bSTARPU_FUT_START_FETCH_INPUT\b/_STARPU_FUT_START_FETCH_INPUT/g
s/\bSTARPU_FUT_START_MEMRECLAIM\b/_STARPU_FUT_START_MEMRECLAIM/g
s/\bSTARPU_FUT_START_PROGRESS\b/_STARPU_FUT_START_PROGRESS/g
s/\bSTARPU_FUT_START_PUSH_OUTPUT\b/_STARPU_FUT_START_PUSH_OUTPUT/g
s/\bSTARPU_FUT_TAG\b/_STARPU_FUT_TAG/g
s/\bSTARPU_FUT_TAG_DEPS\b/_STARPU_FUT_TAG_DEPS/g
s/\bSTARPU_FUT_TAG_DONE\b/_STARPU_FUT_TAG_DONE/g
s/\bSTARPU_FUT_TASK_DEPS\b/_STARPU_FUT_TASK_DEPS/g
s/\bSTARPU_FUT_TASK_DONE\b/_STARPU_FUT_TASK_DONE/g
s/\bSTARPU_FUT_TASK_WAIT_FOR_ALL\b/_STARPU_FUT_TASK_WAIT_FOR_ALL/g
s/\bSTARPU_FUT_UPDATE_TASK_CNT\b/_STARPU_FUT_UPDATE_TASK_CNT/g
s/\bSTARPU_FUT_USER_DEFINED_END\b/_STARPU_FUT_USER_DEFINED_END/g
s/\bSTARPU_FUT_USER_DEFINED_START\b/_STARPU_FUT_USER_DEFINED_START/g
s/\bSTARPU_FUT_USER_EVENT\b/_STARPU_FUT_USER_EVENT/g
s/\bSTARPU_FUT_WORKER_DEINIT_END\b/_STARPU_FUT_WORKER_DEINIT_END/g
s/\bSTARPU_FUT_WORKER_DEINIT_START\b/_STARPU_FUT_WORKER_DEINIT_START/g
s/\bSTARPU_FUT_WORKER_INIT_END\b/_STARPU_FUT_WORKER_INIT_END/g
s/\bSTARPU_FUT_WORKER_INIT_START\b/_STARPU_FUT_WORKER_INIT_START/g
s/\bSTARPU_FUT_WORKER_SLEEP_END\b/_STARPU_FUT_WORKER_SLEEP_END/g
s/\bSTARPU_FUT_WORKER_SLEEP_START\b/_STARPU_FUT_WORKER_SLEEP_START/g
s/\bSTARPU_FUT_WORK_STEALING\b/_STARPU_FUT_WORK_STEALING/g
s/\bSTARPU_TRACE_DATA_COPY\b/_STARPU_TRACE_DATA_COPY/g
s/\bSTARPU_TRACE_END_ALLOC\b/_STARPU_TRACE_END_ALLOC/g
s/\bSTARPU_TRACE_END_ALLOC_REUSE\b/_STARPU_TRACE_END_ALLOC_REUSE/g
s/\bSTARPU_TRACE_END_CALLBACK\b/_STARPU_TRACE_END_CALLBACK/g
s/\bSTARPU_TRACE_END_CODELET_BODY\b/_STARPU_TRACE_END_CODELET_BODY/g
s/\bSTARPU_TRACE_END_DRIVER_COPY\b/_STARPU_TRACE_END_DRIVER_COPY/g
s/\bSTARPU_TRACE_END_DRIVER_COPY_ASYNC\b/_STARPU_TRACE_END_DRIVER_COPY_ASYNC/g
s/\bSTARPU_TRACE_END_FETCH_INPUT\b/_STARPU_TRACE_END_FETCH_INPUT/g
s/\bSTARPU_TRACE_END_MEMRECLAIM\b/_STARPU_TRACE_END_MEMRECLAIM/g
s/\bSTARPU_TRACE_END_PROGRESS\b/_STARPU_TRACE_END_PROGRESS/g
s/\bSTARPU_TRACE_END_PUSH_OUTPUT\b/_STARPU_TRACE_END_PUSH_OUTPUT/g
s/\bSTARPU_TRACE_GHOST_TASK_DEPS\b/_STARPU_TRACE_GHOST_TASK_DEPS/g
s/\bSTARPU_TRACE_JOB_POP\b/_STARPU_TRACE_JOB_POP/g
s/\bSTARPU_TRACE_JOB_PUSH\b/_STARPU_TRACE_JOB_PUSH/g
s/\bSTARPU_TRACE_NEW_MEM_NODE\b/_STARPU_TRACE_NEW_MEM_NODE/g
s/\bSTARPU_TRACE_SET_PROFILING\b/_STARPU_TRACE_SET_PROFILING/g
s/\bSTARPU_TRACE_START_ALLOC\b/_STARPU_TRACE_START_ALLOC/g
s/\bSTARPU_TRACE_START_ALLOC_REUSE\b/_STARPU_TRACE_START_ALLOC_REUSE/g
s/\bSTARPU_TRACE_START_CALLBACK\b/_STARPU_TRACE_START_CALLBACK/g
s/\bSTARPU_TRACE_START_CODELET_BODY\b/_STARPU_TRACE_START_CODELET_BODY/g
s/\bSTARPU_TRACE_START_DRIVER_COPY\b/_STARPU_TRACE_START_DRIVER_COPY/g
s/\bSTARPU_TRACE_START_DRIVER_COPY_ASYNC\b/_STARPU_TRACE_START_DRIVER_COPY_ASYNC/g
s/\bSTARPU_TRACE_START_FETCH_INPUT\b/_STARPU_TRACE_START_FETCH_INPUT/g
s/\bSTARPU_TRACE_START_MEMRECLAIM\b/_STARPU_TRACE_START_MEMRECLAIM/g
s/\bSTARPU_TRACE_START_PROGRESS\b/_STARPU_TRACE_START_PROGRESS/g
s/\bSTARPU_TRACE_START_PUSH_OUTPUT\b/_STARPU_TRACE_START_PUSH_OUTPUT/g
s/\bSTARPU_TRACE_TAG\b/_STARPU_TRACE_TAG/g
s/\bSTARPU_TRACE_TAG_DEPS\b/_STARPU_TRACE_TAG_DEPS/g
s/\bSTARPU_TRACE_TAG_DONE\b/_STARPU_TRACE_TAG_DONE/g
s/\bSTARPU_TRACE_TASK_DEPS\b/_STARPU_TRACE_TASK_DEPS/g
s/\bSTARPU_TRACE_TASK_DONE\b/_STARPU_TRACE_TASK_DONE/g
s/\bSTARPU_TRACE_TASK_WAIT_FOR_ALL\b/_STARPU_TRACE_TASK_WAIT_FOR_ALL/g
s/\bSTARPU_TRACE_UPDATE_TASK_CNT\b/_STARPU_TRACE_UPDATE_TASK_CNT/g
s/\bSTARPU_TRACE_USER_DEFINED_END\b/_STARPU_TRACE_USER_DEFINED_END/g
s/\bSTARPU_TRACE_USER_DEFINED_START\b/_STARPU_TRACE_USER_DEFINED_START/g
s/\bSTARPU_TRACE_USER_EVENT\b/_STARPU_TRACE_USER_EVENT/g
s/\bSTARPU_TRACE_WORKER_DEINIT_END\b/_STARPU_TRACE_WORKER_DEINIT_END/g
s/\bSTARPU_TRACE_WORKER_DEINIT_START\b/_STARPU_TRACE_WORKER_DEINIT_START/g
s/\bSTARPU_TRACE_WORKER_INIT_END\b/_STARPU_TRACE_WORKER_INIT_END/g
s/\bSTARPU_TRACE_WORKER_INIT_START\b/_STARPU_TRACE_WORKER_INIT_START/g
s/\bSTARPU_TRACE_WORKER_SLEEP_END\b/_STARPU_TRACE_WORKER_SLEEP_END/g
s/\bSTARPU_TRACE_WORKER_SLEEP_START\b/_STARPU_TRACE_WORKER_SLEEP_START/g
s/\bSTARPU_TRACE_WORK_STEALING\b/_STARPU_TRACE_WORK_STEALING/g

#s/\bSTARPU_DEFAULT_ALPHA\b/_STARPU_DEFAULT_ALPHA/g
#s/\bSTARPU_DEFAULT_BETA\b/_STARPU_DEFAULT_BETA/g
#s/\bSTARPU_DEFAULT_GAMMA\b/_STARPU_DEFAULT_GAMMA/g
#s/\bSTARPU_CALIBRATION_MINIMUM\b/_STARPU_CALIBRATION_MINIMUM/g
#s/\bSTARPU_CPU_ALPHA\b/_STARPU_CPU_ALPHA/g
#s/\bSTARPU_CUDA_ALPHA\b/_STARPU_CUDA_ALPHA/g
#s/\bSTARPU_OPENCL_ALPHA\b/_STARPU_OPENCL_ALPHA/g
#s/\bSTARPU_GORDON_ALPHA\b/_STARPU_GORDON_ALPHA/g
#
#s/\bstarpu_memory_node_to_devid\b/_starpu_memory_node_to_devid/g
#s/\bstarpu_memchunk_recently_used\b/_starpu_memchunk_recently_used/g
#
#s/\bstarpu_data_start_reduction_mode\b/_starpu_data_start_reduction_mode/g
#s/\bstarpu_data_end_reduction_mode\b/_starpu_data_end_reduction_mode/g
#s/\bstarpu_data_end_reduction_mode_terminate\b/_starpu_data_end_reduction_mode_terminate/g
#s/\bcreate_request_to_fetch_data\b/_starpu_create_request_to_fetch_data/g
#
#s/\bSTARPU_CPU_MAY_PERFORM\b/_STARPU_CPU_MAY_PERFORM/g
#s/\bSTARPU_CUDA_MAY_PERFORM\b/_STARPU_CUDA_MAY_PERFORM/g
#s/\bSTARPU_SPU_MAY_PERFORM\b/_STARPU_SPU_MAY_PERFORM/g
#s/\bSTARPU_GORDON_MAY_PERFORM\b/_STARPU_GORDON_MAY_PERFORM/g
#s/\bSTARPU_OPENCL_MAY_PERFORM\b/_STARPU_OPENCL_MAY_PERFORM/g
#
#s/\bSTARPU_TAG_SIZE\b/_STARPU_TAG_SIZE/g
#s/\bSTARPU_HTBL_NODE_SIZE\b/_STARPU_HTBL_NODE_SIZE/g
#s/\bSTARPU_HTBL32_NODE_SIZE\b/_STARPU_HTBL32_NODE_SIZE/g
#
#s/\bstarpu_clock_gettime\b/_starpu_clock_gettime/g
#
#s/\bstruct starpu_deque_jobq_s\b/struct _starpu_deque_jobq/g
#s/\bstruct starpu_fifo_taskq_s\b/struct _starpu_fifo_taskq/g
#s/\bstruct starpu_stack_jobq_s\b/struct _starpu_stack_jobq/g
#
#s/\bstarpu_top_mt\b/_starpu_top_mt/g
#s/\bstarpu_top_communications_threads_launcher\b/_starpu_top_communications_threads_launcher/g
#s/\bstruct starpu_top_message_queue_item\b/struct _starpu_top_message_queue_item/g
#s/\bstruct starpu_top_message_queue\b/struct _starpu_top_message_queue/g
#s/\bstarpu_top_message_add\b/_starpu_top_message_add/g
#s/\bstarpu_top_message_remove\b/_starpu_top_message_remove/g
#s/\bstarpu_top_message_queue_new\b/_starpu_top_message_queue_new/g
#s/\bstarpu_top_message_queue_free\b/_starpu_top_message_queue_free/g
#
#s/\bstarpu_fxt_dag_init\b/_starpu_fxt_dag_init/g
#s/\bstarpu_fxt_dag_terminate\b/_starpu_fxt_dag_terminate/g
#s/\bstarpu_fxt_dag_add_tag_deps\b/_starpu_fxt_dag_add_tag_deps/g
#s/\bstarpu_fxt_dag_set_tag_done\b/_starpu_fxt_dag_set_tag_done/g
#s/\bstarpu_fxt_dag_add_task_deps\b/_starpu_fxt_dag_add_task_deps/g
#s/\bstarpu_fxt_dag_set_task_done\b/_starpu_fxt_dag_set_task_done/g
#s/\bstarpu_fxt_dag_add_sync_point\b/_starpu_fxt_dag_add_sync_point/g
#s/\bstarpu_fxt_mpi_find_sync_point\b/_starpu_fxt_mpi_find_sync_point/g
#s/\bstarpu_fxt_mpi_add_send_transfer\b/_starpu_fxt_mpi_add_send_transfer/g
#s/\bstarpu_fxt_mpi_add_recv_transfer\b/_starpu_fxt_mpi_add_recv_transfer/g
#s/\bstarpu_fxt_display_mpi_transfers\b/_starpu_fxt_display_mpi_transfers/g
#s/\bstarpu_fxt_write_paje_header\b/_starpu_fxt_write_paje_header/g
#
#s/\bstruct _cond_and_mutex\b/struct _starpu_cond_and_mutex/g
#s/\bstarpu_mem_node_descr\b/struct _starpu_mem_node_descr/g
#s/\bstarpu_node_kind\b/enum _starpu_node_kind/g
#s/\bstarpu_memory_node_tuple\b/enum _starpu_node_kind/g
#
#s/\bstruct callback_list\b/struct _starpu_callback_list/g
#
#s/\bstarpu_async_channel_event\b/union _starpu_async_channel_event/g
#s/\bstruct starpu_async_channel\b/struct _starpu_async_channel/g
#
#s/\bstarpu_cache_state\b/enum _starpu_cache_state/g
#s/\bstruct starpu_jobid_list\b/struct _starpu_jobid_list/g
#s/\bstruct starpu_task_wrapper_list\b/struct _starpu_task_wrapper_list/g
#
#s/\bstruct starpu_combined_worker_s\b/struct _starpu_combined_worker/g
#s/\bstruct starpu_worker_set_s\b/struct _starpu_worker_set/g
#s/\bstruct starpu_machine_config_s\b/struct _starpu_machine_config/g
#
#s/\bstarpu_tag_state\b/enum _starpu_tag_state/g
#s/\bstruct starpu_tag_s\b/struct _starpu_tag/g
#
#s/\bstruct starpu_htbl_node_s\b/struct _starpu_htbl_node/g
#s/\bstarpu_htbl_node_t\b/struct _starpu_htbl_node/g
#
#s/\bstruct starpu_cg_s\b/struct _starpu_cg/g
#s/\bstarpu_cg_t\b/struct _starpu_cg/g
#s/\bstarpu_cg_list_s\b/_starpu_cg_list/g
#
#s/\bcl_func\b/_starpu_cl_func/g
#s/\bstarpu_worker_status\b/enum _starpu_worker_status/g
#s/\bstarpu_worker_s\b/_starpu_worker/g
#
#s/\bPTHREAD_MUTEX_INIT\b/_STARPU_PTHREAD_MUTEX_INIT/g
#s/\bPTHREAD_MUTEX_DESTROY\b/_STARPU_PTHREAD_MUTEX_DESTROY/g
#s/\bPTHREAD_MUTEX_LOCK\b/_STARPU_PTHREAD_MUTEX_LOCK/g
#s/\bPTHREAD_MUTEX_UNLOCK\b/_STARPU_PTHREAD_MUTEX_UNLOCK/g
#
#s/\bPTHREAD_RWLOCK_INIT\b/_STARPU_PTHREAD_RWLOCK_INIT/g
#s/\bPTHREAD_RWLOCK_RDLOCK\b/_STARPU_PTHREAD_RWLOCK_RDLOCK/g
#s/\bPTHREAD_RWLOCK_WRLOCK\b/_STARPU_PTHREAD_RWLOCK_WRLOCK/g
#s/\bPTHREAD_RWLOCK_UNLOCK\b/_STARPU_PTHREAD_RWLOCK_UNLOCK/g
#s/\bPTHREAD_RWLOCK_DESTROY\b/_STARPU_PTHREAD_RWLOCK_DESTROY/g
#
#s/\bPTHREAD_COND_INIT\b/_STARPU_PTHREAD_COND_INIT/g
#s/\bPTHREAD_COND_DESTROY\b/_STARPU_PTHREAD_COND_DESTROY/g
#s/\bPTHREAD_COND_SIGNAL\b/_STARPU_PTHREAD_COND_SIGNAL/g
#s/\bPTHREAD_COND_BROADCAST\b/_STARPU_PTHREAD_COND_BROADCAST/g
#s/\bPTHREAD_COND_WAIT\b/_STARPU_PTHREAD_COND_WAIT/g
#
#s/\bPTHREAD_BARRIER_INIT\b/_STARPU_PTHREAD_BARRIER_INIT/g
#s/\bPTHREAD_BARRIER_DESTROY\b/_STARPU_PTHREAD_BARRIER_DESTROY/g
#s/\bPTHREAD_BARRIER_WAIT\b/_STARPU_PTHREAD_BARRIER_WAIT/g

