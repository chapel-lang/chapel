/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
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

module ChapelGpuSupport {
  use ChapelBase;
  use ChplConfig;

  extern var chpl_gpu_debug : bool;
  @unstable("The variable 'debugGpu' is unstable and its interface is subject to change in the future")
  config const debugGpu = false;

  extern var chpl_gpu_no_cpu_mode_warning: bool;
  @unstable("The variable 'gpuNoCpuModeWarning' is unstable and its interface is subject to change in the future")
  config const gpuNoCpuModeWarning = isEnvSet("CHPL_GPU_NO_CPU_MODE_WARNING");

  extern var chpl_gpu_sync_with_host : bool;
  @unstable("The variable 'gpuSyncWithHostAfterGpuOp' is unstable and its interface is subject to change in the future")
  config const gpuSyncWithHostAfterGpuOp = true;

  extern var chpl_gpu_use_stream_per_task : bool;
  @unstable("The variable 'gpuUseStreamPerTask' is unstable and its interface is subject to change in the future")
  config var gpuUseStreamPerTask = CHPL_GPU_MEM_STRATEGY!="unified_memory";

  /* If true, upon startup, enables peer-to-peer access between all pairs of
     GPUs that are eligible for peer-to-peer access within each locale. */
  @chpldoc.nodoc
  @unstable("The variable 'enableGpuP2P' is unstable and its interface is subject to change in the future")
  config const enableGpuP2P = false;

  extern proc chpl_gpu_support_module_finished_initializing() : void;

  // by virtue of module initialization:
  chpl_gpu_debug = debugGpu;
  chpl_gpu_no_cpu_mode_warning = gpuNoCpuModeWarning;
  chpl_gpu_sync_with_host = gpuSyncWithHostAfterGpuOp;

  if (CHPL_GPU_MEM_STRATEGY=="unified_memory") {
    if (gpuUseStreamPerTask) {
      // the user must have set this
      warning("gpuUseDefaultStream can't be set to false with ",
              "CHPL_GPU_MEM_STRATEGY=unified_memory. Assuming ",
              "gpuUseDefaultStream=true.");

      gpuUseStreamPerTask = false;
    }
  }
  chpl_gpu_use_stream_per_task = gpuUseStreamPerTask;

  if CHPL_LOCALE_MODEL == 'gpu' {
    if(enableGpuP2P) {
      for loc in Locales do on loc do
        for gpu1 in here.gpus do
          for gpu2 in here.gpus do
            if chpl_canAccessPeer(gpu1,gpu2) then
              chpl_setPeerAccess(gpu1,gpu2,true);
    }

    // There are some allocations that occur in the locale model's
    // `helpSetupLocaleGPU` where we
    // set the current sublocale to a GPU but we still want the allocations
    // to actually be performed on the CPU. To do this we start the GPU
    // runtime library in a mode where all allocations will be done on the
    // host until this "finished_initializing" function is called.
    // We also want the value of 'chpl_gpu_debug' to be assigned before
    // calling this so we make this call here in ChapelGpuSupport.
    coforall loc in Locales do on loc do
      chpl_gpu_support_module_finished_initializing();
  }

  // chpl_canAccessPeer() and chpl_setPeerAccess() are in this module
  // so that they can be invoked above without use-ing the GPU module.
  // Otherwise even non-GPU compilations would include the GPU module.

  @chpldoc.nodoc
  proc chpl_canAccessPeer(loc1 : locale, loc2 : locale) : bool {
    extern proc chpl_gpu_can_access_peer(i : c_int, j : c_int) : bool;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    return chpl_gpu_can_access_peer(loc1Sid, loc2Sid);
  }

  @chpldoc.nodoc
  proc chpl_setPeerAccess(loc1 : locale, loc2 : locale, shouldEnable : bool) {
    extern proc chpl_gpu_set_peer_access(
      i : c_int, j : c_int, shouldEnable : bool) : void;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    chpl_gpu_set_peer_access(loc1Sid, loc2Sid, shouldEnable);
  }

  private proc isEnvSet(name: string): bool {
    private use CTypes;
    extern proc getenv(name : c_ptrConst(c_char)) : c_ptrConst(c_char);
    const env = bytes.createBorrowingBuffer(getenv(name.localize().c_str()));
    return !env.isEmpty();
  }

  use CTypes;
  export proc chpl_gpu_comm_on_put(dst_subloc: int(32), addr: c_ptr(void),
                                   src_node: int(32), src_subloc: int(32),
                                   raddr: c_ptr(void), size: c_size_t) {
    // this is a way to implement remote GET, where host initiates a GET to a
    // data that's on a remote device. Because we can't register memory with the
    // comm layer yet, we do an `on` to the remote's device sublocale and
    // initiate a PUT from there.
    extern const chpl_nodeID: int(32);
    const dst_node = chpl_nodeID;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(src_node, src_subloc)) {

      // communicate to the primitive using a reference rather than a ptr
      // the primitive will copy data using these references
      const ref addrRef = (addr:c_ptr(uint(8))).deref();
      ref raddrRef = (raddr:c_ptr(uint(8))).deref();
      __primitive("chpl_comm_put", raddrRef, dst_node,
                  dst_subloc, addrRef, size);

    }
  }

  export proc chpl_gpu_comm_on_get(src_subloc: int(32), addr: c_ptr(void),
                                   dst_node: int(32), dst_subloc: int(32),
                                   raddr: c_ptr(void), size: c_size_t) {
    // this is a way to implement remote PUT, where host initiates a PUT to an
    // address that's on a remote device. Because we can't register memory with
    // the comm layer yet, we do an `on` to the remote's device sublocale and
    // initiate a GET from there.
    extern const chpl_nodeID: int(32);
    const src_node = chpl_nodeID;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(dst_node, dst_subloc)) {

      // communicate to the primitive using a reference rather than a ptr
      // the primitive will copy data using these references
      ref addrRef = (addr:c_ptr(uint(8))).deref();
      const ref raddrRef = (raddr:c_ptr(uint(8))).deref();
      __primitive("chpl_comm_get", raddrRef, src_node,
                  src_subloc, addrRef, size);

    }
  }
}
