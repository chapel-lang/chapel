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

/* Supports utility functions for operating with GPUs.

  .. warning::

    This module is unstable and its interface is subject to change in the
    future.

*/
module GPU
{
  extern proc chpl_gpu_write(const str : c_string) : void;

  /*
     This function is intended to be called from within a GPU
     kernel and is useful for debugging purposes.

     Currently writeln() will make a loop ineligable for
     gpuization. This function is here as a stopgap measure
     until we fix that.

     Currently this function will only work if values of type
     c_string are passed.

     On NVIDIA GPUs the written values will be flushed to the
     terminal after the kernel has finished executing.
     Note that there is (by default) a 1MB limit on the size
     of this buffer.
   */
  proc gpuWrite(const args ...?k) {
    // Right now this function will only work if passed in c_strings.
    // I would prefer to do some string processing within the
    // function so I could pass in arguments other than C types.
    //
    // One thing I tried was changing the call to chpl_gpu_write
    // to look like this:
    //
    //    chpl_gpu_write((args[i] : string).c_str());
    //
    // Unfortunately that made things un-gpuizable as I believe
    // it ends up calling the constructor for string which
    // somewhere uses the the "outside variable" "nil", which
    // fails our gpuization check.
    //
    // I also explored making `printf` an extern proc
    // and calling it directly but that resulted in this error:
    //   ptxas fatal   : Unresolved extern function 'printf
    for param i in 0..<k {
      chpl_gpu_write(args[i]);
    }
  }

  /*
     Pass arguments to gpuWrite and follow with a newline
  */
  proc gpuWriteln(const args ...?k) {
    gpuWrite((...args), "\n".c_str());
  }

  /*
    Will halt execution if called from outside a GPU.
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  inline proc assertOnGpu() {
    __primitive("chpl_assert_on_gpu");
  }
}
