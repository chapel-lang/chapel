/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.  *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// We need to define this variable outside of the commented out HAS_GPU_LOCALE
// section due to the fact that GpuDiagnostics module accesses it (and this
// module can be used despite what locale model you're using).
#include <stdbool.h>
bool chpl_gpu_debug = false;
bool chpl_gpu_no_cpu_mode_warning = false;
int chpl_gpu_num_devices = -1;
bool chpl_gpu_sync_with_host = true;
bool chpl_gpu_use_stream_per_task = true;

#ifdef HAS_GPU_LOCALE

// #define CHPL_GPU_ENABLE_PROFILE

#include "chplrt.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-gpu-diags.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"
#include "chpl-linefile-support.h"
#include "chpl-env-gen.h"
#include "chpl-env.h"
#include "chpl-comm-compiler-macros.h"
#include "chpl-privatization.h"

#include "gpu/chpl-gpu-reduce-util.h"

#include <inttypes.h>

void chpl_gpu_init(void) {
  chpl_gpu_impl_init(&chpl_gpu_num_devices);

  assert(chpl_gpu_num_devices >= 0);

  // override number of devices if applicable
  const char* env;
  int32_t num = -1;
  if ((env = chpl_env_rt_get("NUM_GPUS_PER_LOCALE", NULL)) != NULL) {
    if (sscanf(env, "%" SCNi32, &num) != 1) {
      chpl_error("Cannot parse CHPL_RT_NUM_GPUS_PER_LOCALE environment "
                 "variable", 0, 0);
    }

    if (num < 0) {
      chpl_error("CHPL_RT_NUM_GPUS_PER_LOCALE must be >= 0", 0, 0);
    }

#ifndef GPU_RUNTIME_CPU
    if (chpl_gpu_num_devices > 0 && num > chpl_gpu_num_devices) {
      char msg[200];
      snprintf(msg, sizeof(msg),
          "CHPL_RT_NUM_GPUS_PER_LOCALE = %" PRIi32 " is too large; "
          "it must be less than or equal to the number of GPUs per node. "
          "Detected %" PRIi32 " GPUs in node %" PRIi32 ". "
          "Ignoring this environment variable.",
          num, chpl_gpu_num_devices, chpl_nodeID);
      chpl_warning(msg, 0, 0);
    }
    else {
#endif
      assert(num!=-1);
      chpl_gpu_num_devices = num;
#ifndef GPU_RUNTIME_CPU
    }
#endif
  }
}

// With very limited and artificial benchmarking, we observed that yielding
// while waiting for a stream to finish actually hurts performance. So,
// currently, this is hard-wired for `false`. As we explore overlap patterns
// more, we might want to change it. I expect a begin-based parallelism where
// multiple tasks are likely to be bunched up in a core, we might prefer this
// yield.
static bool yield_in_stream_sync = false;

static inline void wait_stream(void* stream) {
  if (yield_in_stream_sync) {
    while (!chpl_gpu_impl_stream_ready(stream)) {
      chpl_task_yield();
    }
  }
  chpl_gpu_impl_stream_synchronize(stream);
}

static inline bool has_stream_per_task(void) {
  return chpl_gpu_use_stream_per_task && chpl_gpu_impl_stream_supported();
}

static chpl_gpu_taskPrvData_t* get_gpu_task_private_data(void) {
  if (!has_stream_per_task()) return NULL;

  chpl_task_infoRuntime_t* infoRuntime = chpl_task_getInfoRuntime();
  if (infoRuntime != NULL) return &infoRuntime->gpu_data;
  return NULL;
}

void chpl_gpu_task_end(void) {
  if (!has_stream_per_task()) return;

  chpl_gpu_taskPrvData_t* prvData = get_gpu_task_private_data();
  assert(prvData);

  if (prvData->streams != NULL) {
    int i;
    for (i=0 ; i<chpl_gpu_num_devices ; i++) {
      if (prvData->streams[i] != NULL) {
        CHPL_GPU_DEBUG("Destroying stream %p (subloc %d)\n",
                       prvData->streams[i], i);
        wait_stream(prvData->streams[i]);
        chpl_gpu_impl_stream_destroy(prvData->streams[i]);
        prvData->streams[i] = NULL;
      }
    }
  }

}

void chpl_gpu_task_fence(void) {
  if (!has_stream_per_task()) return;

  int dev = chpl_task_getRequestedSubloc();
  if (dev<0) {
    return;
  }

  chpl_gpu_taskPrvData_t* prvData = get_gpu_task_private_data();
  assert(prvData);

  if (prvData->streams != NULL) {
    int i;
    for (i=0 ; i<chpl_gpu_num_devices ; i++) {
      if (prvData->streams[i] != NULL) {
        CHPL_GPU_DEBUG("Synchronizing stream %p (subloc %d)\n", prvData->streams[i], i);
        wait_stream(prvData->streams[i]);
      }
    }
  }
}

static void* get_stream(int dev) {
  if (!has_stream_per_task()) return NULL;

  CHPL_GPU_START_TIMER(stream_time);

  // assumes that device has been set correctly with chpl_gpu_impl_use_device
  chpl_gpu_taskPrvData_t* prvData = get_gpu_task_private_data();

  assert(prvData);

  if (prvData->streams == NULL) {
    CHPL_GPU_DEBUG("here %p\n", prvData);
    CHPL_GPU_DEBUG("allocating stream array (subloc %d)\n", dev);

    prvData->streams = chpl_mem_calloc(chpl_gpu_num_devices, sizeof(void*),
                                       CHPL_RT_MD_GPU_UTIL, 0, 0);
  }
  void** stream = &(prvData->streams[dev]);
  if (*stream == NULL) {
    *stream = chpl_gpu_impl_stream_create();
    CHPL_GPU_DEBUG("Stream created: %p (subloc %d)\n", *stream, dev);
  }

  CHPL_GPU_STOP_TIMER(stream_time);
  CHPL_GPU_PRINT_TIMERS("Stream obtained in %.1Lf us\n", stream_time*1000);

  CHPL_GPU_DEBUG("Using stream: %p (subloc %d)\n", *stream, dev);

  return *stream;
}

void chpl_gpu_support_module_finished_initializing(void) {
  // we can't use `CHPL_GPU_DEBUG` before the support module is finished
  // initializing. This call back is used to signal the runtime that that module
  // has finished initializing.

  CHPL_GPU_DEBUG("GPU layer initialized.\n");
  CHPL_GPU_DEBUG("  Memory allocation strategy for ---\n");
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
    CHPL_GPU_DEBUG("    array data: device memory\n");
    CHPL_GPU_DEBUG("         other: page-locked host memory\n");
  #else
    CHPL_GPU_DEBUG("    array data: unified memory\n");
    CHPL_GPU_DEBUG("         other: unified memory\n");
  #endif

  CHPL_GPU_DEBUG("  Stream per task: %s\n", has_stream_per_task() ? "enabled" :
                                                                    "disabled");
  CHPL_GPU_DEBUG("  Always sync with host: %s\n",
                 chpl_gpu_sync_with_host ? "enabled" : "disabled");
}

typedef struct kernel_cfg_s {
  int dev;

  int ln;
  int32_t fn;

  int n_params;
  int cur_param;
  void*** kernel_params;

  int n_pids;
  int cur_pid;
  int64_t max_pid;
  int64_t* pids;
  size_t* instance_sizes;

  chpl_privateObject_t* priv_table;
  chpl_privateObject_t* host_mirror; //used for initial staging

  // Keep track of kernel parameters we dynamically allocate memory for so
  // later on we know what we need to free.
  bool* param_dyn_allocated;

  // we need this in the config so that we can offload data using this stream,
  // and in the future allocate/deallocate on this stream, too
  void* stream;
} kernel_cfg;

static void cfg_init(kernel_cfg* cfg, int n_params, int n_pids, int ln,
                     int32_t fn) {
  cfg->dev = chpl_task_getRequestedSubloc();
  cfg->stream = get_stream(cfg->dev);

  cfg->ln = ln;
  cfg->fn = fn;

  //+2 for the ln and fn arguments that we add to the end of the array
  cfg->n_params = n_params+2;
  cfg->cur_param = 0;

  cfg->kernel_params = chpl_mem_alloc(cfg->n_params * sizeof(void **),
                                      CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF, ln, fn);
  assert(cfg->kernel_params);


  cfg->param_dyn_allocated = chpl_mem_alloc(cfg->n_params * sizeof(bool),
                                            CHPL_RT_MD_GPU_KERNEL_PARAM_META,
                                            ln, fn);
  assert(cfg->param_dyn_allocated);

  // add the ln and fn arguments to the end of the array These arguments only
  // make sense when the kernel lives inside of standard module code and
  // CHPL_DEVELOPER is not set since the generated kernel function will have two
  // extra formals to account for the line and file num.  If CHPL_DEVELOPER is
  // set, these arguments are dropped on the floor
  cfg->kernel_params[cfg->n_params-2] = (void**)(&ln);
  cfg->kernel_params[cfg->n_params-1] = (void**)(&fn);

  cfg->param_dyn_allocated[cfg->n_params-2] = false;
  cfg->param_dyn_allocated[cfg->n_params-1] = false;

  cfg->n_pids = n_pids;
  cfg->cur_pid = 0;
  cfg->max_pid = -1;
  cfg->pids = chpl_mem_alloc(cfg->n_pids * sizeof(int64_t),
                             CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF, ln, fn);
  cfg->instance_sizes = chpl_mem_alloc(cfg->n_pids * sizeof(size_t),
                             CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF, ln, fn);
  cfg->priv_table = NULL;
}

static void cfg_deinit_params(kernel_cfg* cfg) {
  // free GPU memory allocated for kernel parameters
  for (int i=0 ; i<cfg->n_params ; i++) {
    if (cfg->param_dyn_allocated[i]) {
      chpl_gpu_mem_free(*cfg->kernel_params[i], cfg->ln, cfg->fn);
      chpl_mem_free(cfg->kernel_params[i], cfg->ln, cfg->fn);
    }
  }

  // deallocate these two in reverse order for ease of verbose mem debugging
  chpl_mem_free(cfg->param_dyn_allocated, cfg->ln, cfg->fn);
  chpl_mem_free(cfg->kernel_params, cfg->ln, cfg->fn);
}

static void cfg_add_offload_param(kernel_cfg* cfg, void* arg, size_t size) {
  const int i = cfg->cur_param;
  assert(i < cfg->n_params-2); // -2 because last two params are always ln and fn

  cfg->param_dyn_allocated[i] = true;

  (cfg->kernel_params)[i] = chpl_mem_alloc(sizeof(void*),
                                           CHPL_RT_MD_GPU_KERNEL_PARAM, cfg->ln,
                                           cfg->fn);
  //
  // TODO this doesn't work on EX, why?
  // *kernel_params[i] = chpl_gpu_impl_mem_array_alloc(cur_arg_size, stream);
  *(cfg->kernel_params)[i] = chpl_gpu_mem_alloc(size, CHPL_RT_MD_GPU_KERNEL_ARG,
                                                cfg->ln, cfg->fn);

  chpl_gpu_impl_copy_host_to_device(*(cfg->kernel_params)[i], arg, size,
                                    cfg->stream);

  cfg->cur_param++;
}

static void cfg_add_direct_param(kernel_cfg* cfg, void* arg) {
  const int i = cfg->cur_param;
  assert(i < cfg->n_params-2); // -2 because last two params are always ln and fn

  cfg->param_dyn_allocated[i] = false;
  cfg->kernel_params[i] = arg;

  cfg->cur_param++;
}

static void cfg_add_pid(kernel_cfg* cfg, int64_t pid, size_t size) {
  const int i = cfg->cur_pid;
  assert(i < cfg->n_pids);

  cfg->pids[i] = pid;
  if (pid > cfg->max_pid) {
    cfg->max_pid = pid;
  }

  cfg->instance_sizes[i] = size;

  cfg->cur_pid++;
}

static void cfg_finalize_priv_table(kernel_cfg *cfg,
                                    chpl_privateObject_t* host_table) {
  // TODO why host_table doesn't work?
  if (cfg->cur_pid != cfg->n_pids) {
    chpl_internal_error("All pids must have been added by now");
  }

  if (cfg->priv_table == NULL) {
    const size_t priv_table_size = (cfg->max_pid+1)*sizeof(chpl_privateObject_t);

    // TODO free this
    cfg->host_mirror = chpl_mem_alloc(priv_table_size,
                                      CHPL_RT_MD_COMM_PRV_OBJ_ARRAY,
                                      cfg->ln, cfg->fn);

    // TODO free this
    cfg->priv_table = chpl_gpu_mem_array_alloc((cfg->max_pid+1)*sizeof(chpl_privateObject_t),
                                               CHPL_RT_MD_COMM_PRV_OBJ_ARRAY,
                                               cfg->ln, cfg->fn);

    CHPL_GPU_DEBUG("Allocated privatization table %p\n", cfg->priv_table);

    for (int i=0 ; i<cfg->n_pids ; i++) {
      const int64_t pid_to_copy = cfg->pids[i];
      const size_t instance_size = cfg->instance_sizes[i];

      CHPL_GPU_DEBUG("\t offloading pid %ld\n", pid_to_copy);

      void* gpu_instance = chpl_gpu_mem_alloc(instance_size,
                                                   CHPL_RT_MD_GPU_KERNEL_ARG,
                                                   cfg->ln, cfg->fn);

      CHPL_GPU_DEBUG("\t gpu instance %p\n", gpu_instance);

      /*CHPL_GPU_DEBUG("\t starting h2d %p %p\n", host_table[pid_to_copy].obj,*/
      CHPL_GPU_DEBUG("\t starting h2d %p %p\n", chpl_privateObjects[pid_to_copy].obj,
                                       gpu_instance);
      chpl_gpu_impl_copy_host_to_device(gpu_instance,
                                        chpl_privateObjects[pid_to_copy].obj,
                                        instance_size,
                                        cfg->stream);

      CHPL_GPU_DEBUG("\t h2d ended %p %p\n", chpl_privateObjects[pid_to_copy].obj,
                                       gpu_instance);
      cfg->host_mirror[pid_to_copy].obj = gpu_instance;
    }

    chpl_gpu_impl_copy_host_to_device(cfg->priv_table, cfg->host_mirror,
                                      priv_table_size, cfg->stream);

    CHPL_GPU_DEBUG("Offloaded the new privatization table");
  }
}

void* chpl_gpu_init_kernel_cfg(int n_params, int n_pids, int ln, int32_t fn) {
  void* ret = chpl_mem_alloc(sizeof(kernel_cfg),
                             CHPL_RT_MD_GPU_KERNEL_PARAM_META, ln, fn);
  cfg_init((kernel_cfg*)ret, n_params, n_pids, ln, fn);

  CHPL_GPU_DEBUG("Initialized kernel config for %d params and %d pids\n",
                 n_params, n_pids);

  return ret;
}

void chpl_gpu_deinit_kernel_cfg(void* cfg) {
  chpl_mem_free(cfg, ((kernel_cfg*)cfg)->ln, ((kernel_cfg*)cfg)->fn);
  CHPL_GPU_DEBUG("Deinitialized kernel config\n");
}

void chpl_gpu_pid_offload(void* cfg, int64_t pid, size_t size) {
  CHPL_GPU_DEBUG("\tAdding pid: %ld with size %zu\n", pid, size);
  cfg_add_pid((kernel_cfg*)cfg, pid, size);
  CHPL_GPU_DEBUG("\tAdded pid: %ld with size %zu\n", pid, size);
}

void chpl_gpu_arg_offload(void* cfg, void* arg, size_t size) {
  cfg_add_offload_param((kernel_cfg*)cfg, arg, size);
  CHPL_GPU_DEBUG("\tAdded by-offload param: %p\n", arg);
}

void chpl_gpu_arg_pass(void* cfg, void* arg) {
  cfg_add_direct_param((kernel_cfg*)cfg, arg);
  CHPL_GPU_DEBUG("\tAdded by-val param: %p\n", arg);
}

void chpl_gpu_arg_privtable(void* cfg, void* arg) {
  cfg_finalize_priv_table((kernel_cfg*)cfg, arg);
  cfg_add_direct_param((kernel_cfg*)cfg, &(((kernel_cfg*)cfg)->priv_table));
}

static void launch_kernel(const char* name,
                          int grd_dim_x, int grd_dim_y, int grd_dim_z,
                          int blk_dim_x, int blk_dim_y, int blk_dim_z,
                          kernel_cfg* cfg) {
  chpl_gpu_impl_use_device(cfg->dev);

  chpl_gpu_diags_verbose_launch(cfg->ln, cfg->fn, cfg->dev,
                                blk_dim_x, blk_dim_y, blk_dim_z);
  chpl_gpu_diags_incr(kernel_launch);

  CHPL_GPU_START_TIMER(load_time);

  CHPL_GPU_DEBUG("Kernel configuration %p\n", cfg);

  CHPL_GPU_DEBUG("Loading function named %s\n", name);
  void* function = chpl_gpu_impl_load_function(name);
  assert(function);
  CHPL_GPU_DEBUG("\tFunction Address: %p\n", function);

  CHPL_GPU_STOP_TIMER(load_time);

  CHPL_GPU_DEBUG("Creating kernel parameters\n");
  CHPL_GPU_DEBUG("\tgridDims=(%d, %d, %d), blockDims(%d, %d, %d)\n",
      grd_dim_x, grd_dim_y, grd_dim_z,
      blk_dim_x, blk_dim_y, blk_dim_z);

  for (int i = 0; i < cfg->n_params ; i++) {
    CHPL_GPU_DEBUG("\tArg: %p\n", cfg->kernel_params[i]);
  }

  CHPL_GPU_START_TIMER(kernel_time);

  CHPL_GPU_DEBUG("Calling impl's launcher %s\n", name);
  chpl_gpu_impl_launch_kernel(function,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              cfg->stream, (void**)(cfg->kernel_params));
  CHPL_GPU_DEBUG("\tLauncher returned %s\n", name);


#ifdef CHPL_GPU_ENABLE_PROFILE
  chpl_gpu_impl_stream_synchronize(cfg->stream);
#endif
  CHPL_GPU_STOP_TIMER(kernel_time);

  chpl_task_yield();

  CHPL_GPU_START_TIMER(teardown_time);

  // deinit them before synch as a (premature?) optimization
  cfg_deinit_params(cfg);

  CHPL_GPU_STOP_TIMER(teardown_time);

  CHPL_GPU_PRINT_TIMERS("<%20s> Load: %Lf, "
      "Prep: %Lf, "
      "Kernel: %Lf, "
      "Teardown: %Lf\n",
      name, load_time, prep_time, kernel_time, teardown_time);

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  if (chpl_gpu_sync_with_host) {
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", cfg->stream);
    wait_stream(cfg->stream);
  }
#else
  chpl_gpu_impl_synchronize();
#endif

}


inline void chpl_gpu_launch_kernel(const char* name,
                                   int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                   int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                   void* _cfg) {

  kernel_cfg* cfg = (kernel_cfg*)_cfg;

  chpl_gpu_impl_use_device(cfg->dev);

  CHPL_GPU_DEBUG("Kernel launcher called. (subloc %d)\n"
                 "\tLocation: %s:%d\n"
                 "\tKernel: %s\n"
                 "\tNumArgs: %d\n",
                 cfg->dev,
                 chpl_lookupFilename(cfg->fn),
                 cfg->ln,
                 name,
                 cfg->n_params);

  launch_kernel(name,
                grd_dim_x, grd_dim_y, grd_dim_z,
                blk_dim_x, blk_dim_y, blk_dim_z,
                cfg);

#ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  if (chpl_gpu_sync_with_host) {
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", cfg->stream);
    wait_stream(cfg->stream);
  }
#else
  chpl_gpu_impl_synchronize();
#endif

  CHPL_GPU_DEBUG("Kernel launcher returning. (subloc %d)\n"
                 "\tKernel: %s\n", cfg->dev, name);
}

inline void chpl_gpu_launch_kernel_flat(const char* name,
                                        int64_t num_threads, int blk_dim,
                                        void* _cfg) {
  kernel_cfg* cfg = (kernel_cfg*)_cfg;

  chpl_gpu_impl_use_device(cfg->dev);

  CHPL_GPU_DEBUG("Kernel launcher called. (subloc %d)\n"
                 "\tLocation: %s:%d\n"
                 "\tKernel: %s\n"
                 "\tStream: %p\n"
                 "\tNumArgs: %d\n"
                 "\tNumThreads: %"PRId64"\n",
                 cfg->dev,
                 chpl_lookupFilename(cfg->fn),
                 cfg->ln,
                 name,
                 cfg->stream,
                 cfg->n_params,
                 num_threads);

  if (num_threads > 0){
    int grd_dim_x = (num_threads+blk_dim-1)/blk_dim;
    int grd_dim_y = 1;
    int grd_dim_z = 1;
    int blk_dim_x = blk_dim;
    int blk_dim_y = 1;
    int blk_dim_z = 1;

    launch_kernel(name,
                  grd_dim_x, grd_dim_y, grd_dim_z,
                  blk_dim_x, blk_dim_y, blk_dim_z,
                  cfg);

  } else {
    CHPL_GPU_DEBUG("No kernel launched since num_threads is <=0\n");
  }

  CHPL_GPU_DEBUG("Kernel launcher returning. (subloc %d)\n"
                 "\tKernel: %s\n", cfg->dev, name);
}

extern void chpl_gpu_comm_on_put(c_sublocid_t dst_subloc, void *addr,
                                 c_nodeid_t src_node, c_sublocid_t src_subloc,
                                 void* raddr, size_t size);

extern void chpl_gpu_comm_on_get(c_sublocid_t src_subloc, void* addr,
                                 c_nodeid_t dst_node, c_sublocid_t dst_subloc,
                                 void* raddr, size_t size);


void chpl_gpu_comm_put(c_nodeid_t dst_node, c_sublocid_t dst_subloc, void *dst,
                       c_sublocid_t src_subloc, void *src,
                       size_t size, int32_t commID, int ln, int32_t fn)
{
  void* src_data = src;
  c_sublocid_t src_data_subloc = src_subloc;
  if (src_subloc >= 0) {
    // source is on device, we can't pass device pointers to comm layer. We'll
    // create a copy of the source on the local host.
    src_data = chpl_malloc(size);
    src_data_subloc = c_sublocid_any;

    chpl_gpu_memcpy(src_data_subloc, src_data, src_subloc, src, size, commID,
                    ln, fn);
  }

  if (dst_subloc >= 0) {
    // destination is on device, we can't write to remote GPU memory yet. So,
    // we'll use on+get instead
    chpl_gpu_comm_on_get(src_data_subloc, src_data, dst_node, dst_subloc, dst,
                         size);
  }
  else {
    // destination is on the host, we can do a direct put
    chpl_gen_comm_put(src_data, dst_node, dst, size, commID, ln, fn);
  }

  if (src_subloc >= 0) {
    chpl_free(src_data);
  }
}

void chpl_gpu_comm_get(c_sublocid_t dst_subloc, void *dst,
                       c_nodeid_t src_node, c_sublocid_t src_subloc, void *src,
                       size_t size, int32_t commID, int ln, int32_t fn)
{
  void* dst_buff = dst;
  c_sublocid_t dst_buff_subloc = dst_subloc;
  if (dst_subloc >= 0) {
    // destination is on device, we can't pass device pointers to comm layer.
    // We'll create a buffer on the local host.
    dst_buff = chpl_malloc(size);
    dst_buff_subloc = c_sublocid_any;
  }

  if (src_subloc >= 0) {
    // source is on device, we can't read from remote GPU memory yet. So,
    // we'll use on+put instead
    chpl_gpu_comm_on_put(dst_buff_subloc, dst_buff, src_node, src_subloc, src,
                         size);
  }
  else {
    // source is on the host, we can do a direct put
    chpl_gen_comm_get(dst_buff, src_node, src, size, commID, ln, fn);
  }

  if (dst_subloc >= 0) {
    chpl_gpu_memcpy(dst_subloc, dst, dst_buff_subloc, dst_buff, size,
                    commID, ln, fn);
    chpl_free(dst_buff);
  }
}

void chpl_gpu_comm_get_strd(c_sublocid_t dst_subloc,
                          void* dstaddr_arg, size_t* dststrides,
                          c_nodeid_t srclocale, c_sublocid_t src_subloc,
                          void* srcaddr_arg, size_t* srcstrides,
                          size_t* count, int32_t stridelevels, size_t elemSize,
                          int32_t commID, int ln, int32_t fn) {
  // TODO: Re-use code from chpl-comm-strd-xfer.h instead of copying it here.
  //
  // Note: This function differs from the original in chpl-comm-strd-xfer.h by
  // not supporting non-blocking communication calls.
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];


  // TODO: Communication diagnostics

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]=count[0] * elemSize;
  if(strlvls>0){
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch(strlvls) {
  case 0:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    chpl_gpu_comm_get(dst_subloc, dstaddr,
                      srclocale, src_subloc, srcaddr, cnt[0],
                      commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      chpl_gpu_comm_get(dst_subloc, dstaddr,
                        srclocale, src_subloc, srcaddr, cnt[0],
                        commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        chpl_gpu_comm_get(dst_subloc, dstaddr,
                          srclocale, src_subloc, srcaddr, cnt[0],
                          commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          chpl_gpu_comm_get(dst_subloc, dstaddr,
                            srclocale, src_subloc, srcaddr, cnt[0],
                            commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_gpu_comm_get operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          chpl_gpu_comm_get(dst_subloc, dstaddr+dstdisp[j],
                            srclocale, src_subloc,
                            srcaddr+srcdisp[j], cnt[0],
                            commID, ln, fn);
          break;

        } else {  //ELSE 1
          carry=carry*cnt[t];
        }
      }
    }
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }
}

void chpl_gpu_comm_put_strd(c_sublocid_t src_subloc,
                          void* dstaddr_arg, size_t* dststrides,
                          c_nodeid_t dstlocale, c_sublocid_t dst_subloc,
                          void* srcaddr_arg, size_t* srcstrides,
                          size_t* count, int32_t stridelevels, size_t elemSize,
                          int32_t commID, int ln, int32_t fn) {
  // TODO: Re-use code from chpl-comm-strd-xfer.h instead of copying it here.
  //
  // Note: This function differs from the original in chpl-comm-strd-xfer.h by
  // not supporting non-blocking communication calls.
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];


  // TODO: Communication diagnostics

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]=count[0] * elemSize;
  if(strlvls>0){
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch(strlvls) {
  case 0:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    chpl_gpu_comm_put(dstlocale, dst_subloc, dstaddr,
                      src_subloc, srcaddr, cnt[0],
                      commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      chpl_gpu_comm_put(dstlocale, dst_subloc, dstaddr,
                        src_subloc, srcaddr, cnt[0],
                        commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        chpl_gpu_comm_put(dstlocale, dst_subloc, dstaddr,
                          src_subloc, srcaddr, cnt[0],
                          commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          chpl_gpu_comm_put(dstlocale, dst_subloc, dstaddr,
                            src_subloc, srcaddr, cnt[0],
                            commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_gpu_comm_put operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          chpl_gpu_comm_put(dstlocale, dst_subloc, dstaddr+dstdisp[j],
                            src_subloc, srcaddr+srcdisp[j], cnt[0],
                            commID, ln, fn);
          break;

        } else {  //ELSE 1
          carry=carry*cnt[t];
        }
      }
    }
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }
}


void chpl_gpu_memcpy(c_sublocid_t dst_subloc, void* dst,
                     c_sublocid_t src_subloc, const void* src, size_t n,
                     int32_t commID, int ln, int32_t fn) {
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  if (dst_subloc < 0 && src_subloc < 0) {
    chpl_memmove(dst, src, n);
  }
  else {
    bool dst_on_host = chpl_gpu_impl_is_host_ptr(dst);
    bool src_on_host = chpl_gpu_impl_is_host_ptr(src);

    if (!dst_on_host && !src_on_host) {
      chpl_gpu_copy_device_to_device(dst_subloc, dst, src_subloc, src, n,
                                     commID, ln, fn);
    }
    else if (!dst_on_host) {
      chpl_gpu_copy_host_to_device(dst_subloc, dst, src, n, commID, ln, fn);
    }
    else if (!src_on_host) {
      chpl_gpu_copy_device_to_host(dst, src_subloc, src, n, commID, ln, fn);
    }
    else {
      // Note: this is the case where both source and destination have been
      // created on a GPU sublocale. Therefore, the wide pointers that refer to
      // them have non-negative sublocale. However, not everything created on a
      // GPU sublocale is allocated on the GPU memory. Think of this as a copy
      // between two ints that happen to hav been created on a GPU sublocale.
      assert(dst_on_host && src_on_host);
      memmove(dst, src, n);
    }
  }
  #else

  // for unified memory strategy we don't want to generate calls to copy
  // data from the device to host (since it can just be accessed directly)
  // TODO however, the code path above could be more efficient.
  memmove(dst, src, n);
  #endif

}

void* chpl_gpu_memset(void* addr, const uint8_t val, size_t n) {
  CHPL_GPU_DEBUG("Doing GPU memset of %zu bytes from %p. Val=%d\n\n", n, addr,
                 val);
  int dev = chpl_task_getRequestedSubloc();
  chpl_gpu_impl_use_device(dev);
  void* stream = get_stream(dev);

  void* ret = chpl_gpu_impl_memset(addr, val, n, stream);
  if (chpl_gpu_sync_with_host) {
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream);
    wait_stream(stream);
  }

  CHPL_GPU_DEBUG("chpl_gpu_memset successful\n");
  return ret;
}

void chpl_gpu_copy_device_to_device(c_sublocid_t dst_dev, void* dst,
                                    c_sublocid_t src_dev, const void* src,
                                    size_t n, int32_t commID, int ln,
                                    int32_t fn) {
  assert(chpl_gpu_is_device_ptr(src));

  CHPL_GPU_DEBUG("Copying %zu bytes from device to device\n", n);

  chpl_gpu_impl_use_device(dst_dev);

  chpl_gpu_diags_verbose_device_to_device_copy(ln, fn, dst_dev, src_dev, n,
                                               commID);
  chpl_gpu_diags_incr(device_to_device);

  void* stream = get_stream(dst_dev);
  chpl_gpu_impl_copy_device_to_device(dst, src, n, stream);
  if (dst_dev != src_dev) {
    // going to a device that maybe used by a different task, synchronize
    wait_stream(stream);
  }
  else if (chpl_gpu_sync_with_host) {
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream);
    wait_stream(stream);
  }

  CHPL_GPU_DEBUG("Copy successful\n");
}

void chpl_gpu_copy_device_to_host(void* dst, c_sublocid_t src_dev,
                                  const void* src, size_t n, int32_t commID,
                                  int ln, int32_t fn) {
  assert(chpl_gpu_is_device_ptr(src));


  chpl_gpu_impl_use_device(src_dev);
  void* stream = get_stream(src_dev);

  CHPL_GPU_DEBUG("Copying %zu bytes from device to host on stream %p\n", n, stream);

  chpl_gpu_diags_verbose_device_to_host_copy(ln, fn, src_dev, n, commID);
  chpl_gpu_diags_incr(device_to_host);

  chpl_gpu_impl_copy_device_to_host(dst, src, n, stream);

  // data is going to host, synchronize
  wait_stream(stream);

  CHPL_GPU_DEBUG("Copy successful\n");
}

void chpl_gpu_copy_host_to_device(c_sublocid_t dst_dev, void* dst,
                                  const void* src, size_t n, int32_t commID,
                                  int ln, int32_t fn) {
  assert(chpl_gpu_is_device_ptr(dst));

  CHPL_GPU_DEBUG("Copying %zu bytes from host to device\n", n);

  chpl_gpu_impl_use_device(dst_dev);
  void* stream = get_stream(dst_dev);

  chpl_gpu_diags_verbose_host_to_device_copy(ln, fn, dst_dev, n, commID);
  chpl_gpu_diags_incr(host_to_device);

  chpl_gpu_impl_copy_host_to_device(dst, src, n, stream);
  if (chpl_gpu_sync_with_host) {
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream);
    wait_stream(stream);
  }

  CHPL_GPU_DEBUG("Copy successful\n");
}

void* chpl_gpu_comm_async(void *dst, void *src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst) || chpl_gpu_is_device_ptr(src));

  CHPL_GPU_DEBUG("Copying %zu bytes asynchronously between host and device\n", n);

  return chpl_gpu_impl_comm_async(dst, src, n);
}

void chpl_gpu_comm_wait(void *stream) {
  chpl_gpu_impl_comm_wait(stream);
}

size_t chpl_gpu_get_alloc_size(void* ptr) {
  return chpl_gpu_impl_get_alloc_size(ptr);
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {

  CHPL_GPU_DEBUG("chpl_gpu_mem_alloc called. Size:%zu file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  void *ptr = NULL;
  if (size > 0) {
    chpl_gpu_impl_use_device(chpl_task_getRequestedSubloc());

    chpl_memhook_malloc_pre(1, size, description, lineno, filename);
    ptr = chpl_gpu_impl_mem_alloc(size);
    chpl_memhook_malloc_post((void*)ptr, 1, size, description, lineno, filename);

    CHPL_GPU_DEBUG("chpl_gpu_mem_alloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_DEBUG("chpl_gpu_mem_alloc returning NULL (size was 0)\n");
  }

  return ptr;
}

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc called. Size:%zu file:%s line:%d\n",
                 size, chpl_lookupFilename(filename), lineno);

  int dev = chpl_task_getRequestedSubloc();
  chpl_gpu_impl_use_device(dev);

  void* ptr = 0;
  if (size > 0) {
    chpl_memhook_malloc_pre(1, size, description, lineno, filename);
    ptr = chpl_gpu_impl_mem_array_alloc(size);
    chpl_memhook_malloc_post((void*)ptr, 1, size, description, lineno, filename);

    CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_DEBUG("chpl_gpu_mem_array_alloc returning NULL (size was 0)\n");
  }

  return ptr;
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  CHPL_GPU_DEBUG("chpl_gpu_mem_free is called. Ptr %p\n", memAlloc);

  int dev = chpl_task_getRequestedSubloc();
  chpl_gpu_impl_use_device(dev);

  chpl_memhook_free_pre(memAlloc, 0, lineno, filename);
  chpl_gpu_impl_mem_free(memAlloc);

  CHPL_GPU_DEBUG("chpl_gpu_mem_free is returning\n");
}


void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {

  CHPL_GPU_DEBUG("chpl_gpu_mem_calloc called. Size:%zu file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  void *ptr = NULL;

  if (size > 0) {
    // TODO this is a really poor implementation -- CUDA has memset, that can
    // help a bit, but omp doesn't expose it. I don't know whether performance
    // here matters.
    const size_t total_size = number*size;

    void *host_mem = chpl_mem_calloc(number, size, description, lineno,
                                     filename);

    c_sublocid_t dev_id = chpl_task_getRequestedSubloc();
    chpl_gpu_impl_use_device(dev_id);

    chpl_memhook_malloc_pre(1, total_size, description, lineno, filename);
    ptr = chpl_gpu_impl_mem_alloc(total_size);
    chpl_memhook_malloc_post((void*)ptr, 1, total_size, description, lineno, filename);

    chpl_gpu_impl_copy_host_to_device(ptr, host_mem, total_size, NULL);

    chpl_mem_free(host_mem, lineno, filename);

    CHPL_GPU_DEBUG("chpl_gpu_mem_calloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_DEBUG("chpl_gpu_mem_calloc returning NULL (size was 0)\n");
  }

  return ptr;
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {

  CHPL_GPU_DEBUG("chpl_gpu_mem_realloc called. Size:%zu\n", size);

  assert(chpl_gpu_is_device_ptr(memAlloc));

  c_sublocid_t dev_id = chpl_task_getRequestedSubloc();
  chpl_gpu_impl_use_device(dev_id);

#ifdef GPU_RUNTIME_CPU
    return chpl_mem_realloc(memAlloc, size, description, lineno, filename);
#else
  size_t cur_size = chpl_gpu_get_alloc_size(memAlloc);
  assert(cur_size >= 0);

  if (size == cur_size) {
    return memAlloc;
  }

  // TODO we could probably do something smarter, especially for the case where
  // the new allocation size is smaller than the original allocation size.
  void* new_alloc = chpl_gpu_mem_alloc(size, description, lineno, filename);

  const size_t copy_size = size < cur_size ? size : cur_size;
  chpl_gpu_impl_copy_device_to_device(new_alloc, memAlloc, copy_size,
                                      /*stream=*/NULL); // for now, keep it on
                                                        // the default stream
  chpl_gpu_mem_free(memAlloc, lineno, filename);

  return new_alloc;
#endif
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  // ENGIN: I don't know if it is possible to allocate memory with custom
  // alignment on GPU. It looks like GPUs typically have a default alignment
  // (512?) that cannot be changed. I don't think we'd need more than that
  // today, and if we want, we can play some pointer games to return something
  // with a larger alignment here.

  chpl_internal_error("Allocating aligned GPU memory is not supported yet");
  return NULL;
}

void chpl_gpu_hostmem_register(void *memAlloc, size_t size) {
  CHPL_GPU_DEBUG("chpl_gpu_hostmem_register is called. Ptr %p, size: %zu\n", memAlloc, size);
  chpl_gpu_impl_hostmem_register(memAlloc, size);
}

bool chpl_gpu_is_device_ptr(const void* ptr) {
  return chpl_gpu_impl_is_device_ptr(ptr);
}

bool chpl_gpu_is_host_ptr(const void* ptr) {
  return chpl_gpu_impl_is_host_ptr(ptr);
}

bool chpl_gpu_can_access_peer(int dev1, int dev2) {
  return chpl_gpu_impl_can_access_peer(dev1, dev2);
}

void chpl_gpu_set_peer_access(int dev1, int dev2, bool enable) {
  chpl_gpu_impl_set_peer_access(dev1, dev2, enable);
}

bool chpl_gpu_can_reduce(void) {
  return chpl_gpu_impl_can_reduce();
}

#define DEF_ONE_REDUCE(kind, data_type)\
void chpl_gpu_##kind##_reduce_##data_type(data_type *data, int n, \
                                          data_type* val, int* idx) { \
  CHPL_GPU_DEBUG("chpl_gpu_" #kind "_reduce_" #data_type " called\n"); \
  \
  int dev = chpl_task_getRequestedSubloc(); \
  chpl_gpu_impl_use_device(dev); \
  void* stream = get_stream(dev); \
  \
  chpl_gpu_impl_##kind##_reduce_##data_type(data, n, val, idx, stream); \
  \
  if (chpl_gpu_sync_with_host) { \
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream); \
    wait_stream(stream); \
  } \
  \
  CHPL_GPU_DEBUG("chpl_gpu_" #kind "_reduce_" #data_type " returned\n"); \
}

GPU_REDUCE(DEF_ONE_REDUCE, sum)
GPU_REDUCE(DEF_ONE_REDUCE, min)
GPU_REDUCE(DEF_ONE_REDUCE, max)
GPU_REDUCE(DEF_ONE_REDUCE, minloc)
GPU_REDUCE(DEF_ONE_REDUCE, maxloc)

#undef DEF_ONE_REDUCE

#endif
