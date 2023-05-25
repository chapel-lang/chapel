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

/* Supports counting and reporting GPU operations.

  .. warning::

    This module is unstable and its interface is subject to change in the
    future.

    GPU support is a relatively new feature to Chapel and is under active
    development.

    For the most up-to-date information about GPU support see the
    :ref:`technical note <readme-gpu>` about it.
*/
@unstable("The GpuDiagnostics module is unstable and its interface is subject to change in the future.")
module GpuDiagnostics
{

  // TODO can we even do this?
  // I am leaving these here, because the runtime already inherits them from the
  // comm diagnostics support, and technically there's support for them. We just
  // don't need or don't know how to use them
  @chpldoc.nodoc
  param gpuDiagsStacktrace = false;

  @chpldoc.nodoc
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
  proc startVerboseGpu() {
    chpl_gpu_startVerbose(gpuDiagsStacktrace, gpuDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of GPU operations initiated on any locale.
   */
  proc stopVerboseGpu() { chpl_gpu_stopVerbose(); }

  /*
    Start counting GPU operations across the whole program.
   */
  proc startGpuDiagnostics() {
    chpl_gpu_startDiagnostics(gpuDiagsPrintUnstable);
  }

  /*
    Stop counting GPU operations across the whole program.
   */
  proc stopGpuDiagnostics() {
    chpl_gpu_stopDiagnostics();
  }

  /*
    Reset aggregate GPU operation counts across the whole program.
   */
  proc resetGpuDiagnostics() {
    for loc in Locales do on loc do
      resetGpuDiagnosticsHere();
  }

  /*
    Retrieve aggregate GPU operation counts for the whole program.

    :returns: array of counts of GPU ops initiated on each locale
    :rtype: `[LocaleSpace] gpuDiagnostics`
   */
  proc getGpuDiagnostics() {
    var D: [LocaleSpace] gpuDiagnostics;
    for loc in Locales do on loc {
      D(loc.id) = getGpuDiagnosticsHere();
    }
    return D;
  }

  /*
    Retrieve aggregate GPU operation counts for this locale.

    :returns: counts of GPU ops initiated on this locale
    :rtype: `gpuDiagnostics`
   */
  private proc getGpuDiagnosticsHere() {
    var cd: gpuDiagnostics;
    chpl_gpu_getDiagnosticsHere(cd);
    return cd;
  }

  /*
    Reset aggregate GPU operation counts on the calling locale.
   */
  private inline proc resetGpuDiagnosticsHere() {
    chpl_gpu_resetDiagnosticsHere();
  }
}
