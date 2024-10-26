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

/* Automatically included GPU symbols */
pragma "module included by default"
@unstable("The module name 'AutoGpu' is unstable.")
module AutoGpu {
  // This module supports GPU-specific attributes like @gpu.assertEligible
  // @assertOnGpu. These attributes are translated into calls to procedures
  // in this module as part of the loop body, which insert various GPU
  // primitives. The primitives are used to configure the GPU execution.

  use ChplConfig;
  use Errors;

  /*
   This attribute can be applied to loops to ensure that they are eligible
   for GPU execution. Unlike :annotation:`@assertOnGpu`, this attribute has no
   execution-time effect. It only asserts that the code `could` be executed on
   the GPU, and not that it `will` be executed.


   .. code-block:: chapel

     @gpu.assertEligible
     foreach i in 1..128 { /* ... */ }

     // variable version (applies to loop expressions and promoted expressions)
     @gpu.assertEligible
     var A = (foreach i in 1..128 do i*i) + 1;
  */
  @chpldoc.attributeSignature("gpu.assertEligible")
  inline proc chpl__gpuAssertEligibleAttr() {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("assert gpu eligible");
  }

  /*
   This configuration parameter is used to disable warnings that are emitted
   when :annotation:`@assertOnGpu` is used in a non-GPU compilation. Since
   :annotation:`@assertOnGpu`'s execution-time semantics are to halt execution
   if it is not on the GPU, it will always halt execution when the program is
   not compiled for the GPU. This is likely an issue, so the warning is emitted
   by default. However, in case the user is aware of this and wants to silence
   the warning, they can set this configuration parameter to ``true``.
  */
  config param silenceAssertOnGpuWarning = false;

  /*
   This attribute can be applied to loops to ensure that they are executed on
   the GPU. It has the effect of :annotation:`@gpu.assertEligible`, halting
   compilation if the construct it is applied to cannot be executed on the GPU.
   In addition, this attribute causes an execution-time check to be performed
   when it is reached, ensuring that the code is executed on the GPU.

   .. code-block:: chapel

     @assertOnGpu
     foreach i in 1..128 { /* ... */ }

     // variable version (applies to loop expressions and promoted expressions)
     @assertOnGpu
     var A = (foreach i in 1..128 do i*i) + 1;
  */
  @chpldoc.attributeSignature("assertOnGpu")
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

  /*
   This attribute can be applied to loops to specify the GPU block size to use
   when executing the loop on the GPU.

   .. code-block:: chapel

     // loop version
     @gpu.blockSize(64)
     foreach i in 1..128 { /* ... */ }

     // variable version (applies to loop expressions and promoted expressions)
     @gpu.blockSize(64)
     var A = (foreach i in 1..128 do i*i) + 1;
  */
  pragma "docs only"
  @chpldoc.attributeSignature("gpu.blockSize")
  inline proc chpl__gpuBlockSizeAttr(blockSize: integral) {}

  inline proc chpl__gpuBlockSizeAttr(param counter: int, arg: integral) {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("gpu set blockSize", counter, arg);
  }

  pragma "last resort"
  inline proc chpl__gpuBlockSizeAttr(args ...) {
    compilerError("'@gpu.blockSize' attribute must have exactly one argument: an integral value for the block size");
  }

  /*
   This attribute requests that the kernel executes each consecutive
   ``numIters`` iterations of the affected loop sequentially within the same
   GPU thread. Users must ensure that the arguments to this attribute are
   positive.

   .. code-block:: chapel

     // loop version
     @gpu.itersPerThread(4)
     foreach i in 1..128 { /* ... */ }

     // variable version (applies to loop expressions and promoted expressions)
     @gpu.itersPerThread(4)
     var A = (foreach i in 1..128 do i*i) + 1;

   Specifying the `cyclic` argument to be `true` distributes the iterations
   across GPU threads in cyclic fashion instead of the default block
   discipline.
  */
  pragma "docs only"
  @chpldoc.attributeSignature("gpu.itersPerThread")
  inline proc chpl__gpuItersPerThreadAttr(itersPerThread: integral,
                                          param cyclic: bool = false) {}

  inline proc chpl__gpuItersPerThreadAttr(param counter: int, ipt: integral,
                                          param cyclic: bool = false) {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("gpu set itersPerThread", counter, ipt, cyclic);
  }

  pragma "last resort"
  inline proc chpl__gpuItersPerThreadAttr(args ...) {
    compilerError("'@gpu.itersPerThread' attribute must have one or two arguments: an integral value for the number of iterations per GPU thread and optionally a param boolean value indicating whether the iterations should be distributed across GPU threads in a cyclic fashion, with block-distributed by default");
  }
}
