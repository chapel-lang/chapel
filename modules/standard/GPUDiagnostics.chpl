/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/* Supports counting and reporting GPU operations.

  .. warning::

    This module is unstable and its interface is subject to change in the
    future.

*/
module GPUDiagnostics
{

  // TODO can we even do this?
  // I am leaving these here, because the runtime already inherits them from the
  // comm diagnostics support, and technically there's support for them. We just
  // don't need or don't know how to use them
  pragma "no doc"
  param gpuDiagsStacktrace = false;

  pragma "no doc"
  param gpuDiagsPrintUnstable = false;

  /*
     Aggregated GPU operation counts.
   */
  extern record chpl_gpuDiagnostics {
    var kernel_launch: uint(64);
  };

  /*
    The Chapel record type inherits the runtime definition of it.
   */
  type gpuDiagnostics = chpl_gpuDiagnostics;

  private extern proc chpl_gpu_startVerbose(stacktrace: bool,
                                             print_unstable: bool);

  private extern proc chpl_gpu_stopVerbose();

  private extern proc chpl_gpu_startVerboseHere(stacktrace: bool,
                                                print_unstable: bool);

  private extern proc chpl_gpu_stopVerboseHere();

  private extern proc chpl_gpu_startDiagnostics(print_unstable: bool);

  private extern proc chpl_gpu_stopDiagnostics();

  private extern proc chpl_gpu_startDiagnosticsHere(print_unstable: bool);

  private extern proc chpl_gpu_stopDiagnosticsHere();

  private extern proc chpl_gpu_resetDiagnosticsHere();

  private extern proc chpl_gpu_getDiagnosticsHere(ref cd: gpuDiagnostics);

  /*
    Start on-the-fly reporting of GPU operations initiated on any locale.
   */
  proc startVerboseGPU() {
    chpl_gpu_startVerbose(gpuDiagsStacktrace, gpuDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of GPU operations initiated on any locale.
   */
  proc stopVerboseGPU() { chpl_gpu_stopVerbose(); }

  /*
    Start counting GPU operations across the whole program.
   */
  proc startGPUDiagnostics() {
    chpl_gpu_startDiagnostics(gpuDiagsPrintUnstable);
  }

  /*
    Stop counting GPU operations across the whole program.
   */
  proc stopGPUDiagnostics() {
    chpl_gpu_stopDiagnostics();
  }

  /*
    Reset aggregate GPU operation counts across the whole program.
   */
  proc resetGPUDiagnostics() {
    for loc in Locales do on loc do
      resetGPUDiagnosticsHere();
  }

  /*
    Retrieve aggregate GPU operation counts for the whole program.

    :returns: array of counts of GPU ops initiated on each locale
    :rtype: `[LocaleSpace] gpuDiagnostics`
   */
  proc getGPUDiagnostics() {
    var D: [LocaleSpace] gpuDiagnostics;
    for loc in Locales do on loc {
      D(loc.id) = getGPUDiagnosticsHere();
    }
    return D;
  }

  /*
    Retrieve aggregate GPU operation counts for this locale.

    :returns: counts of GPU ops initiated on this locale
    :rtype: `gpuDiagnostics`
   */
  private proc getGPUDiagnosticsHere() {
    var cd: gpuDiagnostics;
    chpl_gpu_getDiagnosticsHere(cd);
    return cd;
  }

  /*
    Reset aggregate GPU operation counts on the calling locale.
   */
  private inline proc resetGPUDiagnosticsHere() {
    chpl_gpu_resetDiagnosticsHere();
  }
}
