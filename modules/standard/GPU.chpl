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
  use ChplConfig;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_write(const str : c_ptrConst(c_char)) : void;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_clock() : uint;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printTimeDelta(
    msg : c_ptrConst(c_char), start : uint, stop : uint) : void;

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
  @chpldoc.nodoc
  proc gpuPrintTimeDelta(msg : c_ptrConst(c_char), start : uint, stop : uint) : void {
    chpl_gpu_printTimeDelta(msg, start, stop);
  }

  /*
    Returns the number of clock cycles per second of a GPU multiprocessor.
    Note: currently we don't support calling this function from within a kernel.
   */
  proc gpuClocksPerSec(devNum : int) {
    return chpl_gpu_device_clock_rate(devNum : int(32));
  }

  @chpldoc.nodoc
  type GpuAsyncCommHandle = c_ptr(void);

  /*
    Copy srcArr to dstArr, at least one array must be on a GPU; this function
    can be used for either communication to or from the GPU

    Returns a handle that can be passed to `waitGpuComm` to pause execution
    until completion of this asynchronous transfer
  */
  @chpldoc.nodoc
  proc asyncGpuComm(dstArr : ?t1, srcArr : ?t2) : GpuAsyncCommHandle
    where isArrayType(t1) && isArrayType(t2)
  {
    extern proc chpl_gpu_comm_async(dstArr : c_ptr(void), srcArr : c_ptr(void),
       n : c_size_t) : c_ptr(void);

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
  @chpldoc.nodoc
  proc gpuCommWait(gpuHandle : GpuAsyncCommHandle) {
    extern proc chpl_gpu_comm_wait(stream : c_ptr(void));

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
    if !__primitive("call and fn resolves", "numBits", eltType) {
      compilerError("attempting to allocate a shared array of '",
                    eltType : string,
                    "', which does not have a known size. Is 'numBits(",
                    eltType : string,
                    ")' supported?");
    }
    else if CHPL_GPU != "cpu" {
      const voidPtr = __primitive("gpu allocShared", numBytes(eltType)*size);
      return voidPtr : c_ptr(eltType);
    }
    else {
      // this works because the function is inlined.
      var alloc = new c_array(eltType, size);
      return c_ptrTo(alloc[0]);
    }
  }

  /*
    Set the block size for kernels launched on the GPU.
   */
  inline proc setBlockSize(blockSize: int) {
    __primitive("gpu set blockSize", blockSize);
  }

  @chpldoc.nodoc
  proc canAccessPeer(loc1 : locale, loc2 : locale) : bool {
    extern proc chpl_gpu_can_access_peer(i : c_int, j : c_int) : bool;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    return chpl_gpu_can_access_peer(loc1Sid, loc2Sid);
  }

  @chpldoc.nodoc
  proc setPeerAccess(loc1 : locale, loc2 : locale, shouldEnable : bool) {
    extern proc chpl_gpu_set_peer_access(
      i : c_int, j : c_int, shouldEnable : bool) : void;

    if(!loc1.isGpu() || !loc2.isGpu()) then
      halt("Non GPU locale passed to 'canAccessPeer'");
    const loc1Sid = chpl_sublocFromLocaleID(loc1.chpl_localeid());
    const loc2Sid = chpl_sublocFromLocaleID(loc2.chpl_localeid());

    chpl_gpu_set_peer_access(loc1Sid, loc2Sid, shouldEnable);
  }

  // ============================
  // Atomics
  // ============================

  // In the runtime library we have various type specific wrappers to call out
  // to the CUDA/ROCM atomic operation functions.  Note that the various
  // CUDA/ROCM atomic functions are defined in terms of the various "minimum
  // width" C types (like int, long, etc.) rather than fixed width types (like
  // int32_t, int64_t, etc.) thus we need to figure out which of these C types
  // makes the "best fit" for a corresponding Chapel type.
  private proc atomicExternTString(type T) param {
    param nb = if isNumeric(T) then numBits(T) else -1;
    param nbInt = numBits(c_int);
    param nbShort = numBits(c_short);
    param nbFloat = numBits(c_float);

    if nb == -1 then return "unknown";
    if isUint(T) && nb <= nbShort then return "short";
    if isInt(T)  && nb <= nbInt   then return "int";
    if isInt(T)                   then return "longlong";
    if isUint(T) && nb <= nbInt   then return "uint";
    if isUint(T)                  then return "ulonglong";
    if isReal(T) && nb <= nbFloat then return "float";
    if isReal(T)                  then return "double";
    return "unknown";
  }

  private proc externFunc(param opName : string, type T) param {
    return "chpl_gpu_atomic_" + opName + "_" + atomicExternTString(T);
  }

  // used to indicate that although a given atomic operation
  // is supported by other SDKs these particular ones are not
  // supported by ROCm.
  private proc invalidGpuAtomicOpForRocm(param s : string) param {
    select s { when
      "chpl_gpu_atomic_min_longlong",
      "chpl_gpu_atomic_max_longlong"
      do return true; }
    return false;
  }

  private proc validGpuAtomicOp(param s : string) param {
    select s { when
      "chpl_gpu_atomic_add_int",       "chpl_gpu_atomic_add_uint",
      "chpl_gpu_atomic_add_ulonglong", "chpl_gpu_atomic_add_float",
      "chpl_gpu_atomic_add_double",

      "chpl_gpu_atomic_sub_int", "chpl_gpu_atomic_sub_uint",

      "chpl_gpu_atomic_exch_int",       "chpl_gpu_atomic_exch_uint",
      "chpl_gpu_atomic_exch_ulonglong", "chpl_gpu_atomic_exch_float",

      "chpl_gpu_atomic_min_int",       "chpl_gpu_atomic_min_uint",
      "chpl_gpu_atomic_min_ulonglong", "chpl_gpu_atomic_min_longlong",

      "chpl_gpu_atomic_max_int",       "chpl_gpu_atomic_max_uint",
      "chpl_gpu_atomic_max_ulonglong", "chpl_gpu_atomic_max_longlong",

      "chpl_gpu_atomic_inc_uint",

      "chpl_gpu_atomic_dec_uint",

      "chpl_gpu_atomic_and_int",       "chpl_gpu_atomic_and_uint",
      "chpl_gpu_atomic_and_ulonglong",

      "chpl_gpu_atomic_or_int",       "chpl_gpu_atomic_or_uint",
      "chpl_gpu_atomic_or_ulonglong",

      "chpl_gpu_atomic_xor_int",       "chpl_gpu_atomic_xor_uint",
      "chpl_gpu_atomic_xor_ulonglong",

      "chpl_gpu_atomic_CAS_int",       "chpl_gpu_atomic_CAS_uint",
      "chpl_gpu_atomic_CAS_ulonglong"

      // Before adding support for this I would want better capabilities
      // to process CHPL_GPU (this is only supported when compiling for
      // CUDA with CC >= 7.0
      //"chpl_gpu_atomic_CAS_ushort"

      do return true; }
    return false;
  }

  private proc checkValidGpuAtomicOp(param opName, param rtFuncName, type T) param {
    if CHPL_GPU == "amd" && invalidGpuAtomicOpForRocm(rtFuncName) then
      compilerError("Chapel does not support atomic ", opName, " operation on type ", T : string,
        " when using 'CHPL_GPU=amd'.");

    if(!validGpuAtomicOp(rtFuncName)) then
      compilerError("Chapel does not support atomic ", opName, " operation on type ", T : string, ".");
  }

  private inline proc gpuAtomicBinOp(param opName : string, ref x : ?T, val : T) {
    param rtName = externFunc(opName, T);
    checkValidGpuAtomicOp(opName, rtName, T);

    pragma "codegen for GPU"
    extern rtName proc chpl_atomicBinOp(x, val);

    assertOnGpu();
    chpl_atomicBinOp(c_ptrTo(x), val);
  }

  private inline proc gpuAtomicTernOp(param opName : string, ref x : ?T, cmp : T, val : T) {
    param rtName = externFunc(opName, T);
    checkValidGpuAtomicOp(opName, rtName, T);

    pragma "codegen for GPU"
    extern rtName proc chpl_atomicTernOp(x, cmp, val);

    assertOnGpu();
    chpl_atomicTernOp(c_ptrTo(x), cmp, val);
  }

  /* When run on a GPU, atomically add 'val' to 'x' (result is stored in 'x'). */
  inline proc gpuAtomicAdd(  ref x : ?T, val : T) : void { gpuAtomicBinOp("add", x, val); }
  /* When run on a GPU, atomically subtract 'val' from 'x' (result is stored in 'x'). */
  inline proc gpuAtomicSub(  ref x : ?T, val : T) : void { gpuAtomicBinOp("sub", x, val); }
  @chpldoc.nodoc
  inline proc gpuAtomicExch( ref x : ?T, val : T) : void { gpuAtomicBinOp("exch", x, val); }
  /* When run on a GPU, atomically compare 'x' and 'val' and store the minimum in 'x'. */
  inline proc gpuAtomicMin(  ref x : ?T, val : T) : void { gpuAtomicBinOp("min", x, val); }
  /* When run on a GPU, atomically compare 'x' and 'val' and store the maximum in 'x'. */
  inline proc gpuAtomicMax(  ref x : ?T, val : T) : void { gpuAtomicBinOp("max", x, val); }
  /* When run on a GPU, atomically increments x if the original value of x is
     greater-than or equal to val, if so the result is stored in 'x'. */
  inline proc gpuAtomicInc(  ref x : ?T, val : T) : void { gpuAtomicBinOp("inc", x, val); }
  /* When run on a GPU, atomically determine if 'x' equals 0 or is greater than 'val'.
     If so store 'val' in 'x' otherwise decrement 'x' by 1. */
  inline proc gpuAtomicDec(  ref x : ?T, val : T) : void { gpuAtomicBinOp("dec", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'and' operation on 'x' and 'val' and store
     the result in 'x'. */
  inline proc gpuAtomicAnd(  ref x : ?T, val : T) : void { gpuAtomicBinOp("and", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'or' operation on 'x' and 'val' and store
     the result in 'x'.  */
  inline proc gpuAtomicOr(   ref x : ?T, val : T) : void { gpuAtomicBinOp("or", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'xor' operation on 'x' and 'val' and store
     the result in 'x'. */
  inline proc gpuAtomicXor(  ref x : ?T, val : T) : void { gpuAtomicBinOp("xor", x, val); }

  /* When run on a GPU, atomically compare the value in 'x' and 'cmp', if they
     are equal store 'val' in 'x'.  */
  inline proc gpuAtomicCAS(  ref x : ?T, cmp : T, val : T) : void { gpuAtomicTernOp("CAS", x, cmp, val); }
}
