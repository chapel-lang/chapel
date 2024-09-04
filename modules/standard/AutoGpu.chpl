/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

pragma "module included by default"
@unstable("The module name 'AutoGpu' is unstable.")
module AutoGpu {
  // This module supports GPU-specific attributes like @gpu.assertEligible
  // @assertOnGpu. These attributes are translated into calls to procedures
  // in this module as part of the loop body, which insert various GPU
  // primitives. The primitives are used to configure the GPU execution.

  use ChplConfig;
  use Errors;

  inline proc chpl__gpuAssertEligibleAttr() {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("assert gpu eligible");
  }

  config param silenceAssertOnGpuWarning = false;

  inline proc chpl__assertOnGpuAttr() {
    if CHPL_LOCALE_MODEL != "gpu" && !silenceAssertOnGpuWarning {
      compilerWarning("@assertOnGpu encountered in non-GPU compilation");
      compilerWarning("this attribute has a runtime component, and will ",
                      "always halt execution in a non-GPU context.");
      compilerWarning("consider using '@gpu.assertEligible' to ensure ",
                      "that the code can be executed on the GPU without ",
                      "runtime checks.");
    }
    __primitive("chpl_assert_on_gpu");
  }

  inline proc chpl__gpuBlockSizeAttr(param counter: int, arg: integral) {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("gpu set blockSize", arg, counter);
  }

  pragma "last resort"
  inline proc chpl__gpuBlockSizeAttr(param counter: int, rest ...) {
    compilerError("'@gpu.blockSize' attribute must have exactly one argument: an integral value for the block size");
  }

  pragma "last resort"
  inline proc chpl__gpuBlockSizeAttr(param counter: int) {
    compilerError("'@gpu.blockSize' attribute must have exactly one argument: an integral value for the block size");
  }

  inline proc chpl__gpuItersPerThreadAttr(param counter: int, arg: integral) {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("gpu set itersPerThread", arg, counter);
  }

  pragma "last resort"
  inline proc chpl__gpuItersPerThreadAttr(param counter: int, rest ...) {
    compilerError("'@gpu.itersPerThread' attribute must have exactly one argument: an integral value for the number of iterations per GPU thread");
  }

  pragma "last resort"
  inline proc chpl__gpuItersPerThreadAttr(param counter: int) {
    compilerError("'@gpu.itersPerThread' attribute must have exactly one argument: an integral value for the number of iterations per GPU thread");
  }
}
