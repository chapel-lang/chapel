/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
  config const debugGpu = false;

  extern var chpl_gpu_no_cpu_mode_warning: bool;
  config const gpuNoCpuModeWarning = isEnvSet("CHPL_GPU_NO_CPU_MODE_WARNING");

  /* If true, upon startup, enables peer-to-peer access between all pairs of
     GPUs that are eligible for peer-to-peer access within each locale. */
  @chpldoc.nodoc
  config const enableGpuP2P = false;

  extern proc chpl_gpu_support_module_finished_initializing() : void;

  // by virtue of module initialization:
  chpl_gpu_debug = debugGpu;
  chpl_gpu_no_cpu_mode_warning = gpuNoCpuModeWarning;

  if CHPL_LOCALE_MODEL == 'gpu' {
    if(enableGpuP2P) {
      use GPU;
      for loc in Locales do on loc do
        for gpu1 in here.gpus do
          for gpu2 in here.gpus do
            if canAccessPeer(gpu1,gpu2) then setPeerAccess(gpu1,gpu2,true);
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

  private proc isEnvSet(name: string): bool {
    private use CTypes;
    extern proc getenv(name : c_string) : c_string;
    var env = bytes.createBorrowingBuffer(getenv(c_ptrToConst_helper(name.localize()):c_string));
    return !env.isEmpty();
  }
}
