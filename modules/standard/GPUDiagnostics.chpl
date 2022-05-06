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

module GPUDiagnostics
{
  /*
    Print out stack traces for gpu events printed after startVerbosegpu
   */
  config param gpuDiagsStacktrace = false;

  /*
    If this is `false`, a written `gpuDiagnostics` value does not
    include "unstable" fields even when they are non-zero.  Unstable
    fields are those expected to have unpredictable values for multiple
    executions of the same code sequence.  Setting this to `true` causes
    such fields, if non-zero, to be included when a `gpuDiagnostics`
    value is written.  At present the only unstable field is the `amo`
    counter, whose instability is due to the use of atomic reads in spin
    loops that wait for parallelism and on-statements to complete.
   */
  config param gpuDiagsPrintUnstable = false;

  /* Aggregated gpuunication operation counts.  This record type is
     defined in the same way by both the underlying gpu layer(s) and
     this module, because we don't have a good way to inherit types back
     and forth between the two.  This first definition duplicates the
     one in the gpu layer(s).
   */
  extern record chpl_gpuDiagnostics {
    var kernel_launch: uint(64);
  };

  /*
    The Chapel record type inherits the gpu layer definition of it.
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
    Start on-the-fly reporting of gpuunication initiated on any locale.
   */
  proc startVerboseGPU() {
    chpl_gpu_startVerbose(gpuDiagsStacktrace, gpuDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of gpuunication initiated on any locale.
   */
  proc stopVerboseGPU() { chpl_gpu_stopVerbose(); }

  /*
    Start on-the-fly reporting of gpuunication initiated on this locale.
   */
  proc startVerbosegpuHere() {
    compilerError("Not ready yet");
    chpl_gpu_startVerboseHere(gpuDiagsStacktrace, gpuDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of gpuunication initiated on this locale.
   */
  proc stopVerbosegpuHere() {
    compilerError("Not ready yet");
    chpl_gpu_stopVerboseHere();
  }

  /*
    Start counting gpuunication operations across the whole program.
   */
  proc startGPUDiagnostics() {
    chpl_gpu_startDiagnostics(gpuDiagsPrintUnstable);
  }

  /*
    Stop counting gpuunication operations across the whole program.
   */
  proc stopGPUDiagnostics() {
    chpl_gpu_stopDiagnostics();
  }

  /*
    Start counting gpuunication operations initiated on this locale.
   */
  proc startGPUDiagnosticsHere() {
    compilerError("Not ready yet");
    chpl_gpu_startDiagnosticsHere(gpuDiagsPrintUnstable);
  }

  /*
    Stop counting gpuunication operations initiated on this locale.
   */
  proc stopGPUDiagnosticsHere() {
    compilerError("Not ready yet");
    chpl_gpu_stopDiagnosticsHere();
  }

  /*
    Reset aggregate gpuunication counts across the whole program.
   */
  proc resetGPUDiagnostics() {
    for loc in Locales do on loc do
      resetgpuDiagnosticsHere();
  }

  /*
    Reset aggregate gpuunication counts on the calling locale.
   */
  inline proc resetGPUDiagnosticsHere() {
    chpl_gpu_resetDiagnosticsHere();
  }

  /*
    Retrieve aggregate gpuunication counts for the whole program.

    :returns: array of counts of gpu ops initiated on each locale
    :rtype: `[LocaleSpace] gpuDiagnostics`
   */
  proc getGPUDiagnostics() {
    var D: [LocaleSpace] gpuDiagnostics;
    for loc in Locales do on loc {
      D(loc.id) = getgpuDiagnosticsHere();
    }
    return D;
  }

  /*
    Retrieve aggregate gpuunication counts for this locale.

    :returns: counts of gpu ops initiated on this locale
    :rtype: `gpuDiagnostics`
   */
  proc getgpuDiagnosticsHere() {
    var cd: gpuDiagnostics;
    chpl_gpu_getDiagnosticsHere(cd);
    return cd;
  }
}
