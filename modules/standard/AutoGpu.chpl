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
  use ChplConfig;
  use Errors;

  inline proc chpl__gpuAssertEligibleAttr() {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("chpl_assert_on_gpu", true);
  }

  inline proc chpl__gpuBlockSizeAttr(param counter: int, arg: integral) {
    if CHPL_LOCALE_MODEL == "gpu" then
      __primitive("gpu set blockSize", arg, counter);
  }

  pragma "last resort"
  inline proc chpl__gpuBlockSizeAttr(param counter: int, rest ...) {
    compilerError("'@gpu.blockSize' attribute must have exactly one argument: the block size");
  }
}
