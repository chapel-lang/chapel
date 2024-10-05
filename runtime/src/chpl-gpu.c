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
#include "chpl-atomics.h"
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

static chpl_atomic_spinlock_t* priv_table_lock = NULL;

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

  // TODO these should be freed
  priv_table_lock = chpl_mem_alloc(chpl_gpu_num_devices *
                                   sizeof(chpl_atomic_spinlock_t),
                                   CHPL_RT_MD_GPU_UTIL, 0, 0);

  for (int i=0 ; i<chpl_gpu_num_devices ; i++) {
    atomic_init_spinlock_t(&priv_table_lock[i]);
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

typedef struct priv_inst_s {
  int64_t pid;
  void* dev_instance;
} priv_inst;

typedef struct reduce_var_s {
  void* outer_var;
  size_t elem_size;
  void* buffer;
  reduce_wrapper_fn_t wrapper;
} reduce_var;

typedef struct kernel_cfg_s {
  const char* fn_name;
  int64_t num_threads;

  int grd_dim_x;
  int grd_dim_y;
  int grd_dim_z;
  int blk_dim_x;
  int blk_dim_y;
  int blk_dim_z;

  int dev;

  int ln;
  int32_t fn;

  int n_params;
  int cur_param;
  void*** kernel_params;
  int n_host_registered;
  int cur_host_registered_var;
  void** host_registered_var_boxes; // array of device pointers
  void*** host_registered_vars;     // array of pointers to device pointer
                                    // (stored in host_registered_var_boxes)
  void** host_registered_vars_host_ptrs; // array of host pointers

  // Keep track of kernel parameters we dynamically allocate memory for so
  // later on we know what we need to free.
  bool* param_dyn_allocated;

  // these are used while still collecting pids during kernel launch prologue
  int n_pids;
  int cur_pid;
  int64_t max_pid;
  priv_inst* priv_insts;

  chpl_privateObject_t* priv_table_host; //used for initial staging on the host
  chpl_privateObject_t* priv_table_dev; //actual table on the device

  bool has_priv_table_lock;

  int n_reduce_vars;
  int cur_reduce_var;

  reduce_var* reduce_vars;

  // we need this in the config so that we can offload data using this stream,
  // and in the future allocate/deallocate on this stream, too
  void* stream;
} kernel_cfg;

static void cfg_init(kernel_cfg* cfg, const char* fn_name,
                     int n_params, int n_pids, int n_reduce_vars,
                     int n_host_registered_vars, int ln, int32_t fn) {

  cfg->fn_name = fn_name;

  cfg->dev = chpl_task_getRequestedSubloc();
  cfg->stream = get_stream(cfg->dev);

  cfg->ln = ln;
  cfg->fn = fn;

  // +2 for the ln and fn arguments that we add to the end of the array
  // we pass an additional reduce buffer per reduce variable
  cfg->n_params = n_params+n_reduce_vars+2;
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
  cfg->priv_insts = chpl_mem_alloc(cfg->n_pids * sizeof(priv_inst),
                                   CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF, ln, fn);

  cfg->priv_table_host = NULL;
  cfg->priv_table_dev = NULL;

  cfg->has_priv_table_lock = false;

  cfg->n_reduce_vars = n_reduce_vars;
  cfg->cur_reduce_var = 0;

  cfg->reduce_vars = chpl_mem_alloc(cfg->n_reduce_vars * sizeof(reduce_var),
                                    CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF, ln, fn);

  cfg->n_host_registered = n_host_registered_vars;
  cfg->cur_host_registered_var = 0;
  cfg->host_registered_var_boxes = chpl_mem_alloc(
    cfg->n_host_registered * sizeof(void*), CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF,
    ln, fn);
  cfg->host_registered_vars = chpl_mem_alloc(
    cfg->n_host_registered * sizeof(void**), CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF,
    ln, fn);
  cfg->host_registered_vars_host_ptrs = chpl_mem_alloc(
    cfg->n_host_registered * sizeof(void**), CHPL_RT_MD_GPU_KERNEL_PARAM_BUFF,
    ln, fn);
  for(int i = 0; i < cfg->n_host_registered; i++) {
    cfg->host_registered_vars[i] = &cfg->host_registered_var_boxes[i];
  }
}

static void cfg_init_dims_1d(kernel_cfg* cfg, int64_t num_threads,
                             int blk_dim) {
  cfg->num_threads = num_threads;
  if (num_threads > 0) {
    cfg->grd_dim_x = (num_threads+blk_dim-1)/blk_dim;
    cfg->grd_dim_y = 1;
    cfg->grd_dim_z = 1;
    cfg->blk_dim_x = blk_dim;
    cfg->blk_dim_y = 1;
    cfg->blk_dim_z = 1;
  }
  else {
    cfg->grd_dim_x = 0;
    cfg->grd_dim_y = 0;
    cfg->grd_dim_z = 0;
    cfg->blk_dim_x = 0;
    cfg->blk_dim_y = 0;
    cfg->blk_dim_z = 0;
  }
}

static void cfg_init_dims_3d(kernel_cfg* cfg,
                             int grd_dim_x, int grd_dim_y, int grd_dim_z,
                             int blk_dim_x, int blk_dim_y, int blk_dim_z) {
  cfg->num_threads = grd_dim_x*grd_dim_y*grd_dim_z*
                     blk_dim_x*blk_dim_y*blk_dim_z;
  cfg->grd_dim_x = grd_dim_x;
  cfg->grd_dim_y = grd_dim_y;
  cfg->grd_dim_z = grd_dim_z;
  cfg->blk_dim_x = blk_dim_x;
  cfg->blk_dim_y = blk_dim_y;
  cfg->blk_dim_z = blk_dim_z;
}

static void cfg_deinit_params(kernel_cfg* cfg) {
  // free GPU memory allocated for kernel parameters
  for (int i=0 ; i<cfg->n_params ; i++) {
    if (cfg->param_dyn_allocated[i]) {
      chpl_gpu_mem_free(*(cfg->kernel_params[i]), cfg->ln, cfg->fn);
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

  // the kernel_params array must store the addresses of things that will be
  // copied to the device memory before the kernel launch. For by-offload
  // params, we need it to store the address of the pointer generated by
  // chpl_gpu_mem_array_alloc. We need to allocate dynamic memory for that
  // address-to-pointer as well, because otherwise the address (which is just
  // another void*) will go out of scope at the end of this function.
  cfg->kernel_params[i] = chpl_mem_alloc(sizeof(void*),
                                         CHPL_RT_MD_GPU_KERNEL_PARAM, cfg->ln,
                                         cfg->fn);

  *(cfg->kernel_params[i]) = chpl_gpu_mem_array_alloc(size,
                                                      CHPL_RT_MD_GPU_KERNEL_ARG,
                                                      cfg->ln, cfg->fn);

  chpl_gpu_impl_copy_host_to_device(*(cfg->kernel_params[i]), arg, size,
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
  cfg->priv_insts[i].pid = pid;

  if (pid < 0) {
    // array/domain that is not privatized
    CHPL_GPU_DEBUG("\tIgnoring pid: %" PRId64 "\n", pid);
    cfg->priv_insts[i].dev_instance = NULL;
  }
  else {
    CHPL_GPU_DEBUG("\t offloading pid %" PRId64 "\n", pid);

    // adjust max_pid
    if (pid > cfg->max_pid) {
      cfg->max_pid = pid;
    }

    // create and stash a device instance. Note that we currently don't know how
    // large the privatization table is. So, there's no table on the device to
    // put the instance, yet. cfg_finalize_priv_table will create the actual
    // table
    void* dev_instance = chpl_gpu_mem_array_alloc(size,
                                            CHPL_RT_MD_GPU_KERNEL_ARG,
                                            cfg->ln, cfg->fn);

    CHPL_GPU_DEBUG("\tprivatized device instance %p\n", dev_instance);

    chpl_gpu_impl_copy_host_to_device(dev_instance,
                                      chpl_privateObjects[pid].obj,
                                      size,
                                      cfg->stream);
    cfg->priv_insts[i].dev_instance = dev_instance;

    CHPL_GPU_DEBUG("\t offloaded pid %" PRId64 "\n", pid);
  }

  cfg->cur_pid++;
}

static bool cfg_can_reduce(kernel_cfg* cfg) {
  // reductions are not supported with multi-dimensional kernels, yet
  return (cfg->blk_dim_x > 0 &&
          cfg->blk_dim_y == 1 &&
          cfg->blk_dim_z == 1);
}

static void cfg_finalize_priv_table(kernel_cfg *cfg) {
  // n_pids is what the compiler asks for. There can be some arrays
  // with pid=-1 and n_pids will include those. But we don't need to offload
  // them. Checking for `max_pid` is a more robust thing to do here
  if (cfg->max_pid < 0) {
    CHPL_GPU_DEBUG("No real private instances were found\n");
    return;
  }

  if (cfg->cur_pid != cfg->n_pids) {
    chpl_internal_error("All pids must have been added by now");
  }

  assert(cfg->priv_table_dev == NULL); // we'll create this now.

  const size_t offload_size = (cfg->max_pid+1)*sizeof(chpl_privateObject_t);

  // Engin: Here we can either:
  // 1. Allocate and stage a table on the host, offload it to the device in one
  // go or
  // 2. Allocate a table directly on the device, offload instances one by one
  //
  // My instinct is that 1 is the faster/better approach, but I am not very
  // confident in that. We can revisit that, especially if this turns out to be
  // slower than we'd like.

  // allocate and stage a table on the host side:
  cfg->priv_table_host = chpl_mem_alloc(offload_size,
                                    CHPL_RT_MD_COMM_PRV_OBJ_ARRAY,
                                    cfg->ln, cfg->fn);

  // we use `priv_insts` to populate the actual table
  for (int i=0 ; i<cfg->n_pids ; i++) {
    int64_t pid = cfg->priv_insts[i].pid;
    void* dev_instance = cfg->priv_insts[i].dev_instance;

    cfg->priv_table_host[pid].obj = dev_instance;
  }

  for (int i=0 ; i<cfg->max_pid+1 ; i++) {
    CHPL_GPU_DEBUG("\tpriv_table[%d] = %p\n", i, cfg->priv_table_host[i].obj);
  }

  // allocate a table on the device and copy the staged table there:
  cfg->priv_table_dev = chpl_gpu_mem_array_alloc(offload_size,
                                                 CHPL_RT_MD_COMM_PRV_OBJ_ARRAY,
                                                 cfg->ln, cfg->fn);

  CHPL_GPU_DEBUG("Allocated privatization table %p\n",
                 cfg->priv_table_dev);

  chpl_gpu_impl_copy_host_to_device(cfg->priv_table_dev,
                                    cfg->priv_table_host,
                                    offload_size,
                                    cfg->stream);

  // set the global pointer on the device so the kernels can find the table:
  void* dev_global = NULL;
  size_t priv_table_ptr_size;
  chpl_gpu_impl_load_global("chpl_privateObjects", &dev_global,
                            &priv_table_ptr_size);
  assert(dev_global);

  CHPL_GPU_DEBUG("Global for the device table: %p\n", dev_global);

  chpl_atomic_spinlock_t* lock = &(priv_table_lock[cfg->dev]);
  while(!atomic_try_lock_spinlock_t(lock)) {
    chpl_task_yield();
  }

  cfg->has_priv_table_lock = true;

  if (chpl_gpu_impl_is_device_ptr(dev_global)) {
    chpl_gpu_impl_copy_host_to_device(dev_global,
                                      &(cfg->priv_table_dev),
                                      sizeof(void*), cfg->stream);
  }
  else {
    // the device global is not actually a device pointer. This means we are
    // running in cpu-as-device mode. We shouldn't fiddle with
    // `chpl_privateObjects` as that's the base table.
  }

  CHPL_GPU_DEBUG("Offloaded the new privatization table\n");
}

void* chpl_gpu_init_kernel_cfg(const char* fn_name, int64_t num_threads,
                               int blk_dim, int n_params, int n_pids,
                               int n_reduce_vars, int n_host_registered_vars,
                               int ln, int32_t fn) {
  void* ret = chpl_mem_alloc(sizeof(kernel_cfg),
                             CHPL_RT_MD_GPU_KERNEL_PARAM_META, ln, fn);

  cfg_init((kernel_cfg*)ret, fn_name, n_params, n_pids, n_reduce_vars,
           n_host_registered_vars, ln, fn);
  cfg_init_dims_1d((kernel_cfg*)ret, num_threads, blk_dim);

  CHPL_GPU_DEBUG("Initialized kernel config for %s. num_threads=%"PRId64" blk_dim=%d"
                 " %d params, %d pids and %d reduction buffers\n", fn_name,
                 num_threads, blk_dim, n_params, n_pids, n_reduce_vars);
  CHPL_GPU_DEBUG("%s:%d\n", chpl_lookupFilename(fn), ln);

  return ret;
}

void* chpl_gpu_init_kernel_cfg_3d(const char* fn_name,
                                  int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                  int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                  int n_params, int n_pids, int n_reduce_vars,
                                  int n_host_registered_vars, int ln,
                                  int32_t fn) {

  void* ret = chpl_mem_alloc(sizeof(kernel_cfg),
                             CHPL_RT_MD_GPU_KERNEL_PARAM_META, ln, fn);

  cfg_init((kernel_cfg*)ret, fn_name, n_params, n_pids, n_reduce_vars,
           n_host_registered_vars, ln, fn);
  cfg_init_dims_3d((kernel_cfg*)ret,
                   grd_dim_x, grd_dim_y, grd_dim_z,
                   blk_dim_x, blk_dim_y, blk_dim_z);

  CHPL_GPU_DEBUG("%s:%d\n", chpl_lookupFilename(fn), ln);

  return ret;

}

void chpl_gpu_deinit_kernel_cfg(void* _cfg) {
  kernel_cfg* cfg = (kernel_cfg*)_cfg;

  // unlock the privatization table on the device
  if (cfg->has_priv_table_lock) {
    atomic_unlock_spinlock_t(&(priv_table_lock[cfg->dev]));
  }

  // free GPU memory allocated for privatization
  for (int i=0 ; i<cfg->n_pids ; i++) {
    CHPL_GPU_DEBUG("Freeing privatized instance pid: %" PRId64 ", ptr: %p\n",
                   cfg->priv_insts[i].pid,
                   cfg->priv_insts[i].dev_instance);
    chpl_gpu_mem_free(cfg->priv_insts[i].dev_instance, cfg->ln, cfg->fn);
  }

  if (cfg->priv_table_host) {
    CHPL_GPU_DEBUG("Freeing priv_table_host %p\n", cfg->priv_table_host);
    chpl_mem_free(cfg->priv_table_host, cfg->ln, cfg->fn);
  }

  if (cfg->priv_table_dev) {
    CHPL_GPU_DEBUG("Freeing priv_table_dev %p\n", cfg->priv_table_dev);
    chpl_gpu_mem_free(cfg->priv_table_dev, cfg->ln, cfg->fn);
  }

  for (int i=0 ; i<cfg->n_reduce_vars ; i++) {
    chpl_gpu_mem_free(cfg->reduce_vars[i].buffer, cfg->ln, cfg->fn);
  }
  chpl_mem_free(cfg->reduce_vars, cfg->ln, cfg->fn);

  for (int i=0 ; i<cfg->n_host_registered; i++) {
    chpl_gpu_impl_host_unregister(cfg->host_registered_vars_host_ptrs[i]);
  }
  chpl_mem_free(cfg->host_registered_var_boxes, cfg->ln, cfg->fn);
  chpl_mem_free(cfg->host_registered_vars, cfg->ln, cfg->fn);
  chpl_mem_free(cfg->host_registered_vars_host_ptrs, cfg->ln, cfg->fn);

  chpl_mem_free(cfg, ((kernel_cfg*)cfg)->ln, ((kernel_cfg*)cfg)->fn);
  CHPL_GPU_DEBUG("Deinitialized kernel config\n");
}

void chpl_gpu_pid_offload(void* cfg, int64_t pid, size_t size) {
  CHPL_GPU_DEBUG("\tAdding pid: %" PRId64 " with size %zu\n", pid, size);
  cfg_add_pid((kernel_cfg*)cfg, pid, size);
  CHPL_GPU_DEBUG("\tAdded pid: %" PRId64 " with size %zu\n", pid, size);
}

void chpl_gpu_arg_offload(void* _cfg, void* arg, size_t size) {
  kernel_cfg* cfg = (kernel_cfg*)_cfg;
  cfg_add_offload_param(cfg, arg, size);
  CHPL_GPU_DEBUG("\tAdded by-offload param (at %d): %p\n", cfg->cur_param, arg);
}

void chpl_gpu_arg_pass(void* _cfg, void* arg) {
  kernel_cfg* cfg = (kernel_cfg*)_cfg;
  cfg_add_direct_param(cfg, arg);
  CHPL_GPU_DEBUG("\tAdded by-val param (at %d): %p\n", cfg->cur_param,  arg);
}

void chpl_gpu_arg_reduce(void* _cfg, void* arg, size_t elem_size,
                         reduce_wrapper_fn_t wrapper) {
#ifndef GPU_RUNTIME_CPU
  if (!chpl_gpu_can_reduce()) {
    chpl_internal_error("The runtime is built with a software stack that does "
                        "not support reductions (e.g. ROCm 4.x). `reduce` "
                        "expressions and intents are not supported");
  }
#endif
  kernel_cfg* cfg = (kernel_cfg*)_cfg;
  if (cfg_can_reduce(cfg)) {
    // pass the argument normally
    cfg_add_direct_param(cfg, arg);

    // create the reduction buffer
    const int i = cfg->cur_reduce_var;
    assert(i < cfg->n_reduce_vars);

    void* buf = chpl_gpu_mem_array_alloc((cfg->grd_dim_x)*elem_size,
                                         CHPL_RT_MD_GPU_KERNEL_ARG,
                                         cfg->ln, cfg->fn);
    CHPL_GPU_DEBUG("Allocated reduction buffer: %p num elems:%d elem_size:%zu\n",
                   buf, cfg->grd_dim_x, elem_size);

    cfg->reduce_vars[i].buffer = buf;
    cfg->reduce_vars[i].outer_var = arg;
    cfg->reduce_vars[i].elem_size = elem_size;
    cfg->reduce_vars[i].wrapper = wrapper;

    // pass the reduction buffer normally
    cfg_add_direct_param((kernel_cfg*)cfg, &(cfg->reduce_vars[i].buffer));

    cfg->cur_reduce_var++;

    CHPL_GPU_DEBUG("\tAdded by-reduce param (at %d): %p\n", cfg->cur_param, arg);
  }
  else {
    chpl_internal_error("The runtime is not ready to do reductions with this"
                        " kernel configuration. Using multidimensional launch?\n");
  }
}

void chpl_gpu_arg_host_register(void* _cfg, void* arg, size_t size) {
  kernel_cfg* cfg = (kernel_cfg*)_cfg;

  void *dev_arg = chpl_gpu_impl_host_register(arg, size);
  *(cfg->host_registered_vars[cfg->cur_host_registered_var]) = dev_arg;
  cfg_add_direct_param(cfg, cfg->host_registered_vars[cfg->cur_host_registered_var]);
  cfg->host_registered_vars_host_ptrs[cfg->cur_host_registered_var] = arg;
  cfg->cur_host_registered_var += 1;
  CHPL_GPU_DEBUG("\tAdded ref intent param (at %d): %p\n", cfg->cur_param,  dev_arg);
}

static void cfg_finalize_reductions(kernel_cfg* cfg) {
  if (chpl_gpu_can_reduce()) {
    for (int i=0 ; i<cfg->n_reduce_vars ; i++) {
      CHPL_GPU_DEBUG("Reduce %p into %p. Wrapper: %p\n",
                     cfg->reduce_vars[i].buffer,
                     cfg->reduce_vars[i].outer_var,
                     cfg->reduce_vars[i].wrapper);

      cfg->reduce_vars[i].wrapper(cfg->reduce_vars[i].buffer,
                                  cfg->grd_dim_x,
                                  cfg->reduce_vars[i].outer_var,
                                  NULL); // no minloc/maxloc, yet
    }
  }
  else {
    // we can hit this only with cpu-as-device today, where this function will
    // be called, but the reduction is actually handled by the loop as normal
    // so, we don't do anything
  }
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
    CHPL_GPU_DEBUG("\tArg[%d]: %p\n", i, cfg->kernel_params[i]);
    CHPL_GPU_DEBUG("\t\tVal: %p\n", *(cfg->kernel_params[i]));
  }

  cfg_finalize_priv_table(cfg);

  CHPL_GPU_START_TIMER(kernel_time);

  CHPL_GPU_DEBUG("Calling impl's launcher %s\n", name);
  chpl_gpu_impl_launch_kernel(function,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              cfg->stream, (void**)(cfg->kernel_params));
  CHPL_GPU_DEBUG("\tLauncher returned %s\n", name);

  cfg_finalize_reductions(cfg);

#ifdef CHPL_GPU_ENABLE_PROFILE
  chpl_gpu_impl_stream_synchronize(cfg->stream);
#endif
  CHPL_GPU_STOP_TIMER(kernel_time);

  chpl_task_yield();

  CHPL_GPU_START_TIMER(teardown_time);

  // deinit them before synch as a (premature?) optimization
  // Engin: note that we are not using stream-ordered allocators yet. So, I
  // expect the following to serve as a synchornization unfortunately
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


inline void chpl_gpu_launch_kernel(void* _cfg) {
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
                 cfg->fn_name,
                 cfg->stream,
                 cfg->n_params,
                 cfg->num_threads);

  if (cfg->num_threads > 0){
    launch_kernel(cfg->fn_name,
                  cfg->grd_dim_x, cfg->grd_dim_y, cfg->grd_dim_z,
                  cfg->blk_dim_x, cfg->blk_dim_y, cfg->blk_dim_z,
                  cfg);

  } else {
    CHPL_GPU_DEBUG("No kernel launched since num_threads is <=0\n");
  }

  CHPL_GPU_DEBUG("Kernel launcher returning. (subloc %d)\n"
                 "\tKernel: %s\n", cfg->dev, cfg->fn_name);
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
    src_data_subloc = c_sublocid_none;

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
    dst_buff_subloc = c_sublocid_none;
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

  /* This generates just a lot of output

  CHPL_GPU_DEBUG("chpl_gpu_memcpy of %zu bytes <src subloc:%d, ptr:%p> "
                 "<dst subloc:%d, ptr:%p>\n", n, src_subloc, src, dst_subloc,
                 dst);
  */
  #ifdef CHPL_GPU_MEM_STRATEGY_ARRAY_ON_DEVICE
  if (dst_subloc < 0 && src_subloc < 0) {
    memmove(dst, src, n);
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

  /* This generates just a lot of output

  CHPL_GPU_DEBUG("chpl_gpu_memcpy successful\n");
  */
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

  CHPL_GPU_DEBUG("chpl_gpu_mem_realloc called. Size:%zu Alloc:%p\n", size,
                 memAlloc);

  c_sublocid_t dev_id = chpl_task_getRequestedSubloc();
  chpl_gpu_impl_use_device(dev_id);

#ifdef GPU_RUNTIME_CPU
  return chpl_mem_realloc(memAlloc, size, description, lineno, filename);
#else
  assert(chpl_gpu_is_device_ptr(memAlloc));
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

bool chpl_gpu_can_sort(void) {
  return chpl_gpu_impl_can_sort();
}

#define DEF_ONE_REDUCE(kind, data_type)\
void chpl_gpu_##kind##_reduce_##data_type(void *data, int n, \
                                          void* val, int* idx) { \
  CHPL_GPU_DEBUG("chpl_gpu_" #kind "_reduce_" #data_type " called\n"); \
  \
  int dev = chpl_task_getRequestedSubloc(); \
  chpl_gpu_impl_use_device(dev); \
  void* stream = get_stream(dev); \
  \
  chpl_gpu_impl_##kind##_reduce_##data_type((data_type*)data, n, \
                                            (data_type*)val, idx, stream); \
  \
  if (chpl_gpu_sync_with_host) { \
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream); \
    wait_stream(stream); \
  } \
  \
  CHPL_GPU_DEBUG("chpl_gpu_" #kind "_reduce_" #data_type " returned\n"); \
}

GPU_CUB_WRAP(DEF_ONE_REDUCE, sum)
GPU_CUB_WRAP(DEF_ONE_REDUCE, min)
GPU_CUB_WRAP(DEF_ONE_REDUCE, max)
GPU_CUB_WRAP(DEF_ONE_REDUCE, minloc)
GPU_CUB_WRAP(DEF_ONE_REDUCE, maxloc)

#undef DEF_ONE_REDUCE

#define DEF_ONE_SORT(chpl_kind, data_type)\
void chpl_gpu_sort_##chpl_kind##_##data_type(data_type* data_in, \
                                        data_type* data_out, \
                                        int n) { \
  CHPL_GPU_DEBUG("chpl_gpu_sort_" #chpl_kind "_" #data_type " called\n"); \
  \
  int dev = chpl_task_getRequestedSubloc(); \
  chpl_gpu_impl_use_device(dev); \
  void* stream = get_stream(dev); \
  \
  chpl_gpu_impl_sort_##chpl_kind##_##data_type(data_in, data_out, n, stream); \
  \
  if (chpl_gpu_sync_with_host) { \
    CHPL_GPU_DEBUG("Eagerly synchronizing stream %p\n", stream); \
    wait_stream(stream); \
  } \
  \
  CHPL_GPU_DEBUG("chpl_gpu_sort_" #chpl_kind "_" #data_type " returned\n"); \
}

GPU_CUB_WRAP(DEF_ONE_SORT, keys)

#undef DEF_ONE_SORT

#endif
