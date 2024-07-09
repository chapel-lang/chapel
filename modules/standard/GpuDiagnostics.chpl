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
     Aggregated GPU operation counts. ``host_to_device``, ``device_to_host`` and
     ``device_to_device`` will be non-zero only when
     ``CHPL_GPU_MEM_STRATEGY==array_on_device`` and ``CHPL_GPU!=cpu``.
   */
  pragma "chpldoc ignore chpl prefix"
  extern record chpl_gpuDiagnostics {
    var kernel_launch: uint(64);
    var host_to_device: uint(64);
    var device_to_host: uint(64);
    var device_to_device: uint(64);
  }

  /*
    The Chapel record type inherits the runtime definition of it.
   */
  type gpuDiagnostics = chpl_gpuDiagnostics;

  @chpldoc.nodoc
  private operator +=(ref lhs: gpuDiagnostics, rhs: gpuDiagnostics) {
    lhs.kernel_launch += rhs.kernel_launch;
    lhs.host_to_device += rhs.host_to_device;
    lhs.device_to_host += rhs.device_to_host;
    lhs.device_to_device += rhs.device_to_device;
  }

  /*
   * Utility function to assert GPU diagnostic values. First 4 formals have the
   * same names as the fields in chpl_gpuDiagnostics type. `_um` and `_aod`
   * versions of `kernel_launch` are for convenience where we expect different
   * number of launches in different memory modes
   *  - -1 (and really any other negative value) signals that a given diagnostic
   *    should be ignored.
   *  - kernel_launch overrides _um and _aod variants if it is set to something
   *    non-negative.
   */
  @chpldoc.nodoc
  proc assertGpuDiags(kernel_launch=-1,
                      host_to_device=-1,
                      device_to_host=-1,
                      device_to_device=-1,
                      kernel_launch_um=-1,
                      kernel_launch_aod=-1) {

    /* doesn't do any comparison for expected<0. If compared, and
       expected != actual, prints out a message and returns false. Otherwise,
       returns true quietly. */
    proc compare(expected, actual, name) {
      const res = (expected<0) || (actual == expected);

      if res == false {
        writef("Unexpected GPU Diagnostic: observed %i %s instead of %i\n",
               actual, name, expected);
      }

      return res;
    }

    use ChplConfig;

    param isUm = CHPL_GPU_MEM_STRATEGY == "unified_memory";
    param isCpu = CHPL_GPU == "cpu";

    const expectedLaunch;
    if kernel_launch >= 0 {
      assert(kernel_launch_um < 0 && kernel_launch_aod < 0,
             "when kernel_launch is set, kernel_launch* shouldn't be set");

      expectedLaunch = kernel_launch;
    }
    else {
      expectedLaunch = if isUm then kernel_launch_um else kernel_launch_aod;
    }

    var diags: gpuDiagnostics;
    for diag in getGpuDiagnostics() {
      diags += diag;
    }

    var success = compare(expectedLaunch, diags.kernel_launch, "launches");

    if !isUm && !isCpu {
      success &= compare(host_to_device, diags.host_to_device,
                         "host to device communication calls");
      success &= compare(device_to_host, diags.device_to_host,
                         "device to host communication calls");
      success &= compare(device_to_device, diags.device_to_device,
                         "device to device communication calls");
    }

    assert(success, "GPU Diagnostics assertion failed");
  }

  pragma "insert line file info"
  private extern proc chpl_gpu_startVerbose(stacktrace: bool,
                                             print_unstable: bool);

  pragma "insert line file info"
  private extern proc chpl_gpu_stopVerbose();

  pragma "insert line file info"
  private extern proc chpl_gpu_startVerboseHere(stacktrace: bool,
                                                print_unstable: bool);

  pragma "insert line file info"
  private extern proc chpl_gpu_stopVerboseHere();

  pragma "insert line file info"
  private extern proc chpl_gpu_startDiagnostics(print_unstable: bool);

  pragma "insert line file info"
  private extern proc chpl_gpu_stopDiagnostics();

  pragma "insert line file info"
  private extern proc chpl_gpu_startDiagnosticsHere(print_unstable: bool);

  pragma "insert line file info"
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
