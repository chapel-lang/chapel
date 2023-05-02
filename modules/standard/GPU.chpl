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

/* Supports utility functions for operating with GPUs.

  .. warning::

    This module is unstable and its interface is subject to change in the
    future.

    GPU support is a relatively new feature to Chapel and is under active
    development.

    For the most up-to-date information about GPU support see the
    :ref:`technical note <readme-gpu>` about it.
*/
@unstable("The GPU module is unstable and its interface is subject to change in the future.")
module GPU
{
  use CTypes;

  pragma "no doc"
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_write(const str : c_string) : void;

  pragma "no doc"
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_clock() : uint;

  pragma "no doc"
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printTimeDelta(
    msg : c_string, start : uint, stop : uint) : void;

  pragma "no doc"
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_device_clock_rate(devNum : int(32)) : uint;

  /*
     This function is intended to be called from within a GPU kernel and is
     useful for debugging purposes.

     Currently using :proc:`~ChapelIO.write` to send output to ``stdout`` will
     make a loop ineligible for GPU execution; use :proc:`gpuWrite` instead.

     Currently this function will only work if values of type ``c_string`` are
     passed.

     On NVIDIA GPUs the written values will be flushed to the terminal after
     the kernel has finished executing.  Note that there is a 1MB limit on the
     size of this buffer.
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
    // somewhere uses the "outside variable" "nil", which
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
     Pass arguments to :proc:`gpuWrite` and follow with a newline.
  */
  proc gpuWriteln(const args ...?k) {
    gpuWrite((...args), "\n".c_str());
  }

  /*
    Will halt execution at runtime if called from outside a GPU.  If used on
    first line in ``foreach`` or ``forall`` loop will also do a compile time
    check that the loop is eligible for execution on a GPU.
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  inline proc assertOnGpu() {
    __primitive("chpl_assert_on_gpu");
  }

  /*
    Returns value of a per-multiprocessor counter that increments every clock cycle.
    This function is meant to be called to time sections of code within a GPU
    enabled loop.
  */
  proc gpuClock() : uint {
    return chpl_gpu_clock();
  }

  /*
    Prints 'msg' followed by the difference between 'stop' and 'start'. Meant to
    print the time elapsed between subsequent calls to 'gpuClock()'.
    To convert to seconds divide by 'gpuClocksPerSec()'
  */
  pragma "no doc"
  proc gpuPrintTimeDelta(msg : c_string, start : uint, stop : uint) : void {
    chpl_gpu_printTimeDelta(msg, start, stop);
  }

  /*
    Returns the number of clock cycles per second of a GPU multiprocessor.
    Note: currently we don't support calling this function from within a kernel.
   */
  proc gpuClocksPerSec(devNum : int) {
    return chpl_gpu_device_clock_rate(devNum : int(32));
  }

  pragma "no doc"
  type GpuAsyncCommHandle = c_void_ptr;

  /*
    Copy srcArr to dstArr, at least one array must be on a GPU; this function
    can be used for either communication to or from the GPU

    Returns a handle that can be passed to `waitGpuComm` to pause execution
    until completion of this asynchronous transfer
  */
  pragma "no doc"
  proc asyncGpuComm(dstArr : ?t1, srcArr : ?t2) : GpuAsyncCommHandle
    where isArrayType(t1) && isArrayType(t2)
  {
    extern proc chpl_gpu_comm_async(dstArr : c_void_ptr, srcArr : c_void_ptr,
       n : c_size_t) : c_void_ptr;

    if(dstArr.size != srcArr.size) {
      halt("Arrays passed to asyncGpuComm must have the same number of elements. ",
        "Sizes passed: ", dstArr.size, " and ", srcArr.size);
    }
    return chpl_gpu_comm_async(c_ptrTo(dstArr), c_ptrTo(srcArr),
      dstArr.size * numBytes(dstArr.eltType));
  }

  /*
     Wait for communication to complete, the handle passed in should be from the return
     value of a previous call to `asyncGpuComm`.
  */
  pragma "no doc"
  proc gpuCommWait(gpuHandle : GpuAsyncCommHandle) {
    extern proc chpl_gpu_comm_wait(stream : c_void_ptr);

    chpl_gpu_comm_wait(gpuHandle);
  }

  /*
     Synchronize threads within a GPU block.
   */
  inline proc syncThreads() {
    __primitive("gpu syncThreads");
  }

  /*
    Allocate block shared memory, enough to store ``size`` elements of
    ``eltType``. Returns a :type:`CTypes.c_ptr` to the allocated array. Note that
    although every thread in a block calls this procedure, the same shared array
    is returned to all of them.

    :arg eltType: the type of elements to allocate the array for.

    :arg size: the number of elements in each GPU thread block's copy of the array.
   */
  inline proc createSharedArray(type eltType, param size): c_ptr(eltType) {
    const voidPtr = __primitive("gpu allocShared", numBytes(eltType)*size);
    return voidPtr : c_ptr(eltType);
  }

  /*
    Set the block size for kernels launched on the GPU.
   */
  inline proc setBlockSize(blockSize: int) {
    __primitive("gpu set blockSize", blockSize);
  }

  pragma "no doc"
  proc canAccessPeer(loc1 : locale, loc2 : locale) : bool {
    extern proc chpl_gpu_can_access_peer(i : c_int, j : c_int) : bool;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    return chpl_gpu_can_access_peer(loc1Sid, loc2Sid);
  }

  pragma "no doc"
  proc setPeerAccess(loc1 : locale, loc2 : locale, shouldEnable : bool) {
    extern proc chpl_gpu_set_peer_access(
      i : c_int, j : c_int, shouldEnable : bool) : void;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    chpl_gpu_set_peer_access(loc1Sid, loc2Sid, shouldEnable);
  }
}
