/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

  .. include:: AutoGpu.rst
     :start-line: 7
     :start-after: Automatically included GPU symbols
*/
@unstable("The GPU module is unstable and its interface is subject to change in the future.")
module GPU
{
  use CTypes;
  use ChplConfig;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_write(const str : c_ptrConst(c_char)) : void;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf0(fmt) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf1(fmt, x1) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf2(fmt, x1, x2) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf3(fmt, x1, x2, x3) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf4(fmt, x1, x2, x3, x4) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf5(fmt, x1, x2, x3, x4, x5) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf6(fmt, x1, x2, x3, x4, x5, x6) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf7(fmt, x1, x2, x3, x4, x5, x6, x7) : void;
  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printf8(fmt, x1, x2, x3, x4, x5, x6, x7, x8) : void;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_clock() : uint;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_printTimeDelta(
    msg : c_ptrConst(c_char), start : uint, stop : uint) : void;

  pragma "codegen for CPU and GPU"
  extern proc chpl_gpu_device_clock_rate(devNum : int(32)) : uint;

  @chpldoc.nodoc
  proc gpuWritef(fmt) do  chpl_gpu_printf0(fmt);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1) do chpl_gpu_printf1(fmt,x1);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2) do chpl_gpu_printf2(fmt.c_str(),x1,x2);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3) do chpl_gpu_printf3(fmt.c_str(),x1,x2,x3);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3, x4) do chpl_gpu_printf4(fmt.c_str(),x1,x2,x3,x4);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3, x4, x5) do chpl_gpu_printf5(fmt.c_str(),x1,x2,x3,x4,x5);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3, x4, x5, x6) do chpl_gpu_printf6(fmt.c_str(),x1,x2,x3,x4,x5,x6);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3, x4, x5, x6, x7) do chpl_gpu_printf7(fmt.c_str(),x1,x2,x3,x4,x5,x6,x7);
  @chpldoc.nodoc
  proc gpuWritef(fmt, x1, x2, x3, x4, x5, x6, x7, x8) do chpl_gpu_printf8(fmt.c_str(),x1,x2,x3,x4,x5,x6,x7,x8);

  /*
     This function is intended to be called from within a GPU kernel and is
     useful for debugging purposes.

     Currently using :proc:`~IO.write` to send output to ``stdout`` will
     make a loop ineligible for GPU execution; use :proc:`gpuWrite` instead.

     Currently this function will only work if values of type
     ``c_ptrConst(c_char)`` are passed.

     On NVIDIA GPUs the written values will be flushed to the terminal after
     the kernel has finished executing.  Note that there is a 1MB limit on the
     size of this buffer.
   */
  proc gpuWrite(const args ...?k) {
    // Right now this function will only work if passed arguments are of type
    // c_ptrConst(c_char).
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
    gpuWrite((...args), "\n":c_ptrConst(c_char));
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
  proc asyncGpuComm(ref dstArr : ?t1, srcArr : ?t2) : GpuAsyncCommHandle
    where isArrayType(t1) && isArrayType(t2)
  {
    extern proc chpl_gpu_comm_async(dstArr : c_ptr(void), srcArr : c_ptr(void),
       n : c_size_t) : c_ptr(void);

    if(dstArr.size != srcArr.size) {
      halt("Arrays passed to asyncGpuComm must have the same number of elements. ",
        "Sizes passed: ", dstArr.size, " and ", srcArr.size);
    }
    return chpl_gpu_comm_async(c_ptrTo(dstArr), c_ptrToConst(srcArr),
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
    Causes the executing thread to wait until all warp lanes named in mask
    have executed a ``syncWarp()`` (with the same mask) before resuming
    execution.
    Each calling thread must have its own bit set in the mask and all
    non-exited threads named in mask must execute a corresponding
    ``syncWarp()`` with the same mask, or the result is undefined.
  */
  inline proc syncWarp(mask : uint(32) = 0xffffffff) {
    pragma "codegen for GPU"
    extern "chpl_gpu_force_warp_sync" proc chpl_syncWarp(mask);

    __primitive("chpl_assert_on_gpu");
    chpl_syncWarp(mask);
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

  @chpldoc.nodoc
  inline proc createSharedCArray(type theType : c_array(?t, ?k)) ref {
    var voidPtr = __primitive("gpu allocShared", numBytes(t) * k);
    var arrayPtr = voidPtr : c_ptr(theType);
    return arrayPtr.deref();
  }

  @chpldoc.nodoc
  proc canAccessPeer(loc1 : locale, loc2 : locale) : bool {
    return chpl_canAccessPeer(loc1, loc2);
  }

  @chpldoc.nodoc
  proc setPeerAccess(loc1 : locale, loc2 : locale, shouldEnable : bool) {
    chpl_setPeerAccess(loc1, loc2, shouldEnable);
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
    extern rtName proc chpl_atomicBinOp(x, val) : T;

    __primitive("chpl_assert_on_gpu");
    return chpl_atomicBinOp(c_ptrTo(x), val);
  }

  private inline proc gpuAtomicTernOp(param opName : string, ref x : ?T, cmp : T, val : T) {
    param rtName = externFunc(opName, T);
    checkValidGpuAtomicOp(opName, rtName, T);

    pragma "codegen for GPU"
    extern rtName proc chpl_atomicTernOp(x, cmp, val) : T;

    __primitive("chpl_assert_on_gpu");
    return chpl_atomicTernOp(c_ptrTo(x), cmp, val);
  }

  /* When run on a GPU, atomically add 'val' to 'x' and store the result in 'x'.
     The operation returns the old value of x. */
  inline proc gpuAtomicAdd(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("add", x, val); }
  /* When run on a GPU, atomically subtract 'val' from 'x' and store the result in 'x'.
     The operation returns the old value of x. */
  inline proc gpuAtomicSub(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("sub", x, val); }
  /* When run on a GPU, atomically exchange the value stored in 'x' with 'val'.
     The operation returns the old value of x. */
  inline proc gpuAtomicExch( ref x : ?T, val : T) : T { return gpuAtomicBinOp("exch", x, val); }
  /* When run on a GPU, atomically compare 'x' and 'val' and store the minimum in 'x'.
     The operation returns the old value of x. */
  inline proc gpuAtomicMin(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("min", x, val); }
  /* When run on a GPU, atomically compare 'x' and 'val' and store the maximum in 'x'.
     The operation returns the old value of x. */
  inline proc gpuAtomicMax(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("max", x, val); }
  /* When run on a GPU, atomically increments x if the original value of x is
     greater-than or equal to val, if so the result is stored in 'x'. Otherwise x is set to 0.
     The operation returns the old value of x. */
  inline proc gpuAtomicInc(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("inc", x, val); }
  /* When run on a GPU, atomically determine if 'x' equals 0 or is greater than 'val'.
     If so store 'val' in 'x' otherwise decrement 'x' by 1. Otherwise x is set to val.
     The operation returns the old value of x. */
  inline proc gpuAtomicDec(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("dec", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'and' operation on 'x' and 'val' and store
     the result in 'x'. The operation returns the old value of x. */
  inline proc gpuAtomicAnd(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("and", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'or' operation on 'x' and 'val' and store
     the result in 'x'. The operation returns the old value of x. */
  inline proc gpuAtomicOr(   ref x : ?T, val : T) : T { return gpuAtomicBinOp("or", x, val); }
  /* When run on a GPU, atomically perform a bitwise 'xor' operation on 'x' and 'val' and store
     the result in 'x'. The operation returns the old value of x. */
  inline proc gpuAtomicXor(  ref x : ?T, val : T) : T { return gpuAtomicBinOp("xor", x, val); }

  /* When run on a GPU, atomically compare the value in 'x' and 'cmp', if they
     are equal store 'val' in 'x'. The operation returns the old value of x. */
  inline proc gpuAtomicCAS(  ref x : ?T, cmp : T, val : T) : T { return gpuAtomicTernOp("CAS", x, cmp, val); }

  // ============================
  // Reductions
  // ============================

  @chpldoc.nodoc
  config param gpuDebugReduce = false;

  @chpldoc.nodoc
  config param gpuAlwaysFallBackToCpuReduce = false;

  private inline proc doGpuReduce(param op: string, const ref A: [] ?t) {
    if op != "sum" && op != "min" && op != "max" &&
       op != "minloc" && op != "maxloc" {

      compilerError("Unexpected reduction kind in doGpuReduce: ", op);
    }

    param cTypeName = if      t==bool     then "chpl_bool"
                      else if t==int(8)   then "int8_t"
                      else if t==int(16)  then "int16_t"
                      else if t==int(32)  then "int32_t"
                      else if t==int(64)  then "int64_t"
                      else if t==uint(8)  then "uint8_t"
                      else if t==uint(16) then "uint16_t"
                      else if t==uint(32) then "uint32_t"
                      else if t==uint(64) then "uint64_t"
                      else if t==real(32) then "_real32"
                      else if t==real(64) then "_real64"
                      else                     "unknown";

    if cTypeName == "unknown" {
      compilerError("Arrays with ", t:string,
                    " elements cannot be reduced with gpu*Reduce functions");
    }

    proc valType(param op: string, const ref A: [] ?t) type {
      if t==bool && op == "sum" then return int;
      else return t;
    }

    proc retType(param op: string, const ref A: [] ?t) type {
      if isValReduce(op) then return valType(op, A);
      if isValIdxReduce(op) then return (valType(op, A), int);
      compilerError("Unknown reduction operation: ", op);
    }


    proc doCpuReduceHelp(param op: string, const ref A: [] ?t) {
      select op {
        when "sum" do return + reduce A;
        when "min" do return min reduce A;
        when "max" do return max reduce A;
        when "minloc" do return minloc reduce zip (A, A.domain);
        when "maxloc" do return maxloc reduce zip (A, A.domain);
        otherwise do compilerError("Unknown reduction operation: ", op);
      }
    }

    proc doCpuReduce(param op: string, const ref A: [] ?t) {
      var res: retType(op, A);
      if CHPL_GPU=="cpu" {
        res = doCpuReduceHelp(op, A): res.type;
      }
      else {
        // I want to do on here.parent but that doesn't work. Note that this
        // caused some issues with `--gpu-specialization`.
        // test/gpu/native/reduction/basic.skipif is a skipif that's added
        // because of this hack.
        extern proc chpl_task_getRequestedSubloc(): int(32);
        const curSubloc = chpl_task_getRequestedSubloc();
        chpl_task_setSubloc(c_sublocid_none);
        var HostArr = A;
        res = doCpuReduceHelp(op, HostArr): res.type;
        chpl_task_setSubloc(curSubloc);
      }
      return res;
    }

    proc getExternFuncName(param op: string, type t) param: string {
      return "chpl_gpu_"+op+"_reduce_"+cTypeName;
    }

    proc isValReduce(param op) param {
      return op=="sum" || op=="min" || op=="max";
    }

    proc isValIdxReduce(param op) param {
      return op=="minloc" || op=="maxloc";
    }

    inline proc subReduceValIdx(param op, const baseOffset, ref accum, val) {
      // do some type checking to be safe
      compilerAssert(isTupleValue(val));
      if isTupleValue(accum) {
        compilerAssert(isValIdxReduce(op));
        compilerAssert(val[0].type == accum[0].type);

      }
      else {
        compilerAssert(isValReduce(op));
        compilerAssert(val[0].type == accum.type);
      }

      select op {
        when "sum" do accum += val[0];
        when "min" do accum = min(accum, val[0]);
        when "max" do accum = max(accum, val[0]);
        when "minloc" do
          if accum[0] > val[0] then accum = (val[0], val[1]+baseOffset);
        when "maxloc" do
          if accum[0] < val[0] then accum = (val[0], val[1]+baseOffset);
        otherwise do compilerError("Unknown reduction operation: ", op);
      }
    }

    iter offsetsThatCanFitIn32Bits(size: int) {
      // Engin: I've tried to get max(int(32)) to work as this bug is about CUB
      // using `int` as the size in the interface. However, getting close to
      // max(int(32)) also triggers the bug. So, I am choosing this as a
      // round/safe value for the time being.
      param chunkSize = 2_000_000_000;

      use Math only divCeil;
      const numChunks = divCeil(size, chunkSize);
      const standardChunkSize = divCeil(size, numChunks);

      if gpuDebugReduce then
        writeln("Will use ", numChunks, " chunks of size ", standardChunkSize);

      foreach chunk in 0..<numChunks {
        const start = chunk*standardChunkSize;
        const curChunkSize = if start+standardChunkSize <= size
                               then standardChunkSize
                               else size-start;
        if gpuDebugReduce then
          writef("Chunk %i: (start=%i, curChunkSize=%i) ", chunk, start,
                 curChunkSize);

        yield (start, curChunkSize);
      }
    }

    if CHPL_GPU=="cpu" || gpuAlwaysFallBackToCpuReduce {
      return doCpuReduce(op, A);
    }

    // find the extern function we'll use
    param externFunc = getExternFuncName(op, t);
    extern externFunc proc reduce_fn(data, size, ref val, ref idx);

    // initialize the return value
    var ret: retType(op, A);;
    if isValReduce(op) {
      if op == "min" then ret = max(t);
      else if op == "max" then ret = min(t);
    }
    else if isValIdxReduce(op) {
      if op == "minloc" then ret[0] = max(t);
      else if op == "maxloc" then ret[0] = min(t);
    }
    else {
      compilerError("Unknown reduction operation: ", op);
      ret = 0;
    }

    // perform the reduction
    const basePtr = c_ptrToConst(A);
    for (offset,size) in offsetsThatCanFitIn32Bits(A.size) {
      var curIdx: int(32) = -1; // should remain -1 for sum, min, max
      var curVal: valType(op, A);
      reduce_fn(basePtr+offset, size, curVal, curIdx);
      subReduceValIdx(op, offset, ret, (curVal, curIdx));
      if gpuDebugReduce then
        writef(" (%s curVal=%i curIdx=%i ret=%?)\n", externFunc, curVal, curIdx,
               ret);
    }

    if isValIdxReduce(op) then
      ret[1] += A.domain.first;

    return ret;
  }

  /*
    Add all elements of an array together on the GPU (that is, perform a
    sum-reduction). The array must be in GPU-accessible memory and the function
    must be called from outside a GPU-eligible loop. Only arrays with int, uint,
    and real types are supported. A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         writeln(gpuSumReduce(Arr)); // 15
       }
  */
  inline proc gpuSumReduce(const ref A: [] ?t) do return doGpuReduce("sum", A);

  /*
    Return the minimum element of an array on the GPU (that is, perform a
    min-reduction). The array must be in GPU-accessible memory and the function
    must be called from outside a GPU-eligible loop. Only arrays with int, uint,
    and real types are supported. A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         writeln(gpuMinReduce(Arr)); // 1
       }
  */
  inline proc gpuMinReduce(const ref A: [] ?t) do return doGpuReduce("min", A);

  /*
    Return the maximum element of an array on the GPU (that is, perform a
    max-reduction). The array must be in GPU-accessible memory and the function
    must be called from outside a GPU-eligible loop. Only arrays with int, uint,
    and real types are supported. A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         writeln(gpuMaxReduce(Arr)); // 5
       }
  */
  inline proc gpuMaxReduce(const ref A: [] ?t) do return doGpuReduce("max", A);

  /*
    For an array on the GPU, return a tuple with the value and the index of the
    minimum element (that is, perform a minloc-reduction). If there are multiple
    elements with the same minimum value, the index of the first one is
    returned. The array must be in GPU-accessible memory and the function must
    be called from outside a GPU-eligible loop.  Only arrays with int, uint, and
    real types are supported. A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         writeln(gpuMinLocReduce(Arr)); // (1, 2). Note that Arr[2]==1.
       }
  */
  inline proc gpuMinLocReduce(const ref A: [] ?t) do return doGpuReduce("minloc", A);

  /*
    For an array on the GPU, return a tuple with the value and the index of the
    maximum element (that is, perform a maxloc-reduction). If there are multiple
    elements with the same maximum value, the index of the first one is
    returned. The array must be in GPU-accessible memory and the function must
    be called from outside a GPU-eligible loop.  Only arrays with int, uint, and
    real types are supported. A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         writeln(gpuMaxLocReduce(Arr)); // (5, 3). Note that Arr[3]==5.
       }
  */
  inline proc gpuMaxLocReduce(const ref A: [] ?t) do return doGpuReduce("maxloc", A);



  // ============================
  // GPU Scans
  // ============================

  // The following functions are used to implement GPU scans. They are
  // intended to be called from a GPU locale.
  private import Math;
  private import BitOps;

  private param DefaultGpuBlockSize = 512;

  /*
    Calculates an exclusive prefix sum (scan) of an array on the GPU.
    The array must be in GPU-accessible memory and the function
    must be called from outside a GPU-eligible loop.
    Arrays of numeric types are supported.
    A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4]; // will be GPU-accessible
         gpuScan(Arr);
         writeln(Arr); // [0, 3, 5, 6, 11]
       }
  */
  proc gpuScan(ref gpuArr : [] ?t) where isNumericType(t) && !isComplexType(t) {
    if !here.isGpu()  then halt("gpuScan must be run on a gpu locale");
    if gpuArr.size==0 then return;
    if gpuArr.rank > 1 then compilerError("gpuScan only supports 1D arrays");

    // Use a simple algorithm for small arrays
    // TODO check the actual thread block size rather than 2*default
    if gpuArr.size <= DefaultGpuBlockSize*2 {
      // The algorithms only works for arrays that are size of a power of two.
      // In case it's not a power of two we pad it out with 0s
      const size = roundToPowerof2(gpuArr.size);
      if size == gpuArr.size {
        // It's already a power of 2 so we don't do copies back and forth
        singleBlockScan(gpuArr);
        return;
      }
      var arr : [0..<size] t;
      arr[0..<gpuArr.size] = gpuArr;

      singleBlockScan(arr);

      // Copy back
      gpuArr = arr[0..<gpuArr.size];
    } else {
      // We use a parallel scan algorithm for large arrays
      parallelArrScan(gpuArr);
    }
  }

  private proc singleBlockScan(ref gpuArr : [] ?t) {
    // Hillis Steele Scan is better if we can scan in
    // a single thread block
    // TODO check the actual thread block size rather than the default
    if gpuArr.size <= DefaultGpuBlockSize then
      hillisSteeleScan(gpuArr);
    else
      blellochScan(gpuArr);
  }

  private proc parallelArrScan(ref gpuArr : [] ?t) where isNumericType(t) && !isComplexType(t) {
    // Divide up the array into chunks of a reasonable size
    // For our default, we choose our default block size which is 512
    const scanChunkSize = DefaultGpuBlockSize;
    const numScanChunks = Math.divCeil(gpuArr.size, scanChunkSize);

    if numScanChunks == 1 {
      hillisSteeleScan(gpuArr);
      return;
    }

    // Allocate an accumulator array
    var gpuScanArr : [0..<numScanChunks] t;
    const low = gpuArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433
    const ceil = gpuArr.domain.high;

    // In parallel: For each chunk we do an in lane serial scan
    @assertOnGpu
    foreach chunk in 0..<numScanChunks {
      const start = low+chunk*scanChunkSize;
      const end = min(ceil, start+scanChunkSize-1);
      gpuScanArr[chunk] = gpuArr[end];  // Save the last element before the scan overwrites it
      serialScan(gpuArr, start, end);   // Exclusive scan in serial
      gpuScanArr[chunk] += gpuArr[end]; // Save inclusive scan in the scan Arr

    }

      // Scan the scanArr and we do it recursively
      gpuScan(gpuScanArr);

      @assertOnGpu
      foreach i in gpuArr.domain {
        // In propagate the right values from scanArr
        // to complete the global scan
        const offset : int = (i-low) / scanChunkSize;
        gpuArr[i] += gpuScanArr[offset];
      }
  }

  private proc roundToPowerof2(const x : uint) {
    // Powers of two only have the highest bit set.
    // Power of two minus one will have all bits set except the highest.
    // & those two together should give us 0;
    // Ex 1000 & 0111 = 0000
    if (x & (x - 1)) == 0 then
      return x; // x is already a power of 2
    // Not a power of two, so we pad it out
    // To the next nearest power of two
    const log_2_x = numBytes(uint)*8 - BitOps.clz(x); // get quick log for uint
    // Next highest power of two is
    return 1 << log_2_x;
  }

  // This function requires that startIdx and endIdx are within the bounds of the array
  // it checks that only if boundsChecking is true (i.e. NOT with --fast or --no-checks)
  private proc serialScan(ref arr : [] ?t, startIdx = arr.domain.low, endIdx = arr.domain.high) {
    // Convert this count array into a prefix sum
    // This is the same as the count array, but each element is the sum of all previous elements
    // This is an exclusive scan
    // Serial implementation
    if boundsChecking then assert(startIdx >= arr.domain.low && endIdx <= arr.domain.high);
    // Calculate the prefix sum
    var sum : t = 0;
    for i in startIdx..endIdx {
      var temp : t = arr[i];
      arr[i] = sum;
      sum += temp;
    }
  }

  private proc hillisSteeleScan(ref arr : [] ?t) where isNumericType(t) && !isComplexType(t) {
    // Hillis Steele Scan
    // This is the same as the count array, but each element is the sum of all previous elements
    // Uses a naive algorithm that does O(nlogn) work
    // Hillis and Steele (1986)
    const x = arr.size;
    if x == 0 then return;
    if (x & (x - 1)) != 0 then {
      halt("Hillis Steele Scan only works for arrays of size a power of two.");
    }

    var offset = 1;
    while offset < arr.size {
        var arrBuffer = arr;
        const low = arr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433
        @assertOnGpu
        foreach idx in offset..<arr.size {
          const i = idx + low;
          arr[i] = arrBuffer[i] + arrBuffer[i-offset];
        }
        offset = offset << 1;
    }

    // Change inclusive scan to exclusive
    var arrBuffer = arr;
    foreach i in arr.domain.low+1..arr.domain.high {
      arr[i] = arrBuffer[i-1];
    }
    arr[arr.domain.low] = 0;
  }

  private proc blellochScan(ref arr : [] ?t) where isNumericType(t) && !isComplexType(t) {
    // Blelloch Scan
    // This is the same as the count array, but each element is the sum of all previous elements
    // Uses a more efficient algorithm that does O(n) work
    // Blelloch (1990)

    const x = arr.size;
    if x == 0 then return;
    if (x & (x - 1)) != 0 then {
      halt("Blelloch Scan only works for arrays of size a power of two.");
    }


    const low = arr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

    // Up-sweep
    var offset = 1;
    while offset < arr.size {
      var arrBuffer = arr;
      const doubleOff = offset << 1;
      @assertOnGpu
      foreach idx in 0..<arr.size/(2*offset) {
        const i = idx*doubleOff + low;
        arr[i+doubleOff-1] = arrBuffer[i+offset-1] + arrBuffer[i+doubleOff-1];
      }
      offset = offset << 1;
    }

    // Down-sweep
    arr[arr.domain.high] = 0;
    offset = arr.size >> 1;
    while offset > 0 {
      var arrBuffer = arr;
      @assertOnGpu
      foreach idx in 0..<arr.size/(2*offset) {
        const i = idx*2*offset+low;
        const t = arrBuffer[i+offset-1];
        arr[i+offset-1] = arrBuffer[i+2*offset-1];
        arr[i+2*offset-1] = arr[i+2*offset-1] + t;
      }
      offset = offset >> 1;
    }
  }





  // ============================
  // GPU Sort
  // ============================

  private import Time;

  // We no doc it so we can test this independently
  @chpldoc.nodoc
  proc gpuExternSort(ref gpuInputArr : [] ?t) {
    param cTypeName = if      t==int(8)   then "int8_t"
                      else if t==int(16)  then "int16_t"
                      else if t==int(32)  then "int32_t"
                      else if t==int(64)  then "int64_t"
                      else if t==uint(8)  then "uint8_t"
                      else if t==uint(16) then "uint16_t"
                      else if t==uint(32) then "uint32_t"
                      else if t==uint(64) then "uint64_t"
                      else if t==real(32) then "float"
                      else if t==real(64) then "double"
                      else                     "unknown";

    if cTypeName == "unknown" {
      compilerError("Arrays with ", t:string,
                    " elements cannot be sorted with gpuExternSort functions");
    }

    // Only useful when calling gpuExternSort directly
    if CHPL_GPU=="cpu" {
      gpuSort(gpuInputArr);
      return;
    }

    proc getExternFuncName(param op: string) param: string {
      return "chpl_gpu_sort_"+op+"_"+cTypeName;
    }

    // find the extern function we'll use
    // (there's only one right now, the infrastructure allows more)
    param externFunc = getExternFuncName("keys");
    extern externFunc proc sort_fn(data, temp, size);

    // Make another array which is needed for the sort
    var temp : gpuInputArr.type;

    const basePtr = c_ptrToConst(gpuInputArr);
    const tempPtr = c_ptrTo(temp);
    // Do the sort
    sort_fn(basePtr, tempPtr, gpuInputArr.size);

    // The sorted values are in temp, so we need to copy them back
    // to the original array
    // TODO: Maybe change this to a  <=> to just swap pointers
    gpuInputArr = temp;
  }

  /*
    Sort an array on the GPU.
    The array must be in GPU-accessible memory and the function must
    be called from outside a GPU-eligible loop.
    Only arrays of numeric types are supported.
    A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4] : uint; // will be GPU-accessible
         gpuSort(Arr);
         writeln(Arr); // [1, 2, 3, 4, 5]
       }
  */
  proc gpuSort(ref gpuInputArr : [] ?t) {
    if !here.isGpu() then halt("gpuSort must be run on a gpu locale");
    // Since we don't have distributed arrays for GPU
    // targetLocales will only return one locale so we just index into that
    // Change this when we add support for sorting distributed GPU arrays
    const loc = gpuInputArr.targetLocales()[0];
    if loc != here then
      halt("gpuSort must be run on the gpu where its argument array lives (array is on ",  loc ,", gpuSort was called on " , here, ")");
    if gpuInputArr.size == 0 then return;

    if CHPL_GPU=="cpu" {
      use Sort only sort;
      sort(gpuInputArr);
    } else {
      gpuExternSort(gpuInputArr);
    }
  }

  // We no doc it so we can test this independently to simulate all cases that can happen with sort
  @chpldoc.nodoc
  proc parallelRadixSort(ref gpuInputArr : [] ?t, const bitsAtATime : int = 8,
                         const chunkSize : int = 512, const noisy : bool = false,
                         const distributed : bool = false) where isCoercible(t, uint){
    // The last argument (distributed) is for multi GPU sort that is pending a
    // patch before it can work
    if !here.isGpu() then halt("parallelRadixSort must be run on a gpu locale");
    if gpuInputArr.size == 0 then return;
    if CHPL_GPU=="cpu" {
      use Sort only sort;
      sort(gpuInputArr);
      return;
    }
    // How many bits we sort at once based on the bitsAtATime
    const buckets = 1 << bitsAtATime; // 2^bitsAtATime, ex: 2^4 = 16 = 0b10000
    const bitMask = buckets - 1; // 2^bitsAtATime - 1, ex: 2^4 - 1 = 15 = 0b1111

    // The size of the chunks the input array is divided into for parallelism
    // Each chunk has it's own count table (hence it's own prefix sum)
    // Which we then intelligently combine together to produce a sorted output
    const numChunks = Math.divCeil(gpuInputArr.size,chunkSize);
    // TODO: make up some heuristic to chose a good chunkSize and numChunks?


    if noisy {
        writeln("Bits at a time: ", bitsAtATime);
        writeln("Buckets: ", buckets);
        writeln("Bit mask: ", bitMask);
        writeln("Chunk Size: ", chunkSize);
        writeln("Num Chunks: ", numChunks);
    }

    // Based on the number of chunks we're gonna divide this input into,
    // we create the prefixSum arrays, one for each chunk
    // And we only create it once
    // This was we can reuse it for each iteration of radix Sort
    var prefixSums : [0..<numChunks*buckets] t;

    var timer : Time.stopwatch;
    // Ceiling on number of iterations based on max element in array
    timer.start();
    var maxVal = max(t);
    // Reduce can be used to do this faster
    // We do this because the preferred ways below either don't work or take too long
    // But I'm leaving this as is due to https://github.com/chapel-lang/chapel/issues/22736
    // var maxVal = max reduce gpuInputArr;
    // var maxVal = gpuInputArr[0]; // Takes toooo long for GPU Arrays
    // for i in 1..<gpuInputArr.size {
    //   if gpuInputArr[i] > maxVal {
    //     maxVal = gpuInputArr[i];
    //   }
    // }
    timer.stop();
    if noisy then writeln("maxVal: ", maxVal);
    if noisy then writeln("Finding Max time: ", timer.elapsed());

    // Number of iterations is the number of bits in the max element divided by number of bits we sort at a time
    var exp = 0;
    while maxVal> 0 {
      if noisy then writeln("       Exp: ", exp);

      // Each chunk maintains it's own count array (here it's called prefixSum)
      // First we do the counts in parallel
      // After this we need to use the prefix sum to get a global offset
      // And finally we will use that offset to do the reorder/scatter step
      // in parallel

      // Count
      timer.clear();
      timer.start();
      if !distributed then
        parallelCount(prefixSums, gpuInputArr, exp, bitMask, chunkSize, numChunks);
      else {
        const numChunksPerGpu = Math.divCeil(numChunks, 2);
        distributedCount(prefixSums, gpuInputArr, exp, bitMask, numChunks, here.gpus.size);
      }
      timer.stop();

      if noisy then writeln("Parallel Count Time: ", timer.elapsed());
      if noisy then writeln("Prefix Sums after count: ", prefixSums);

      // Calculate global offsets for each chunk
      // I can calculate the global offset by simply doing a prefix sum over the array
      // See https://gpuopen.com/download/publications/Introduction_to_GPU_Radix_Sort.pdf
      // Figure 1 and equation 1 for explanation
      timer.clear();
      timer.start();
      gpuScan(prefixSums);
      timer.stop();
      if noisy then writeln("Parallel Scan Time: ", timer.elapsed());
      if noisy then writeln("Prefix Sums after scan: ", prefixSums);

      // Scatter
      // Now we have the offsets for each individual chunk
      // We can use those to scatter back into the output Array
      timer.clear();
      timer.start();
      parallelScatter(prefixSums, gpuInputArr, exp, bitMask, chunkSize, numChunks);
      timer.stop();
      if noisy then writeln("Parallel Scatter Time: ", timer.elapsed());
      if noisy then writeln("Array after scatter: ", gpuInputArr);

      // Increment the exp, decrement maxVal by shifting, and copy arrays
      exp += bitsAtATime;
      maxVal = maxVal >> bitsAtATime;
    }
  }

  private proc parallelCount(ref gpuCounts : [], ref gpuInputArr : [] ?t, const exp : int,
                    const bitMask : int, const chunkSize : int,
                    const numChunks : int,  const numChunksThisGpu : int = numChunks,
                    const startChunk : int = 0,
                    const gpuId : int = 0, const resetCountsArray = true) where isCoercible(t, uint){

    // Instead of using a nested array of arrays, use a simple 1D array of
    // size numChunks*buckets which is a column major representation
    // of the 2D array where each chunk has it's own array of buckets.
    // This way calculating the offset is just a prefix sum
    // And we can use strided iteration to work on them as if they were
    // 2D arrays.
    // writeln("Counting on gpu: ", gpuId);
    if resetCountsArray then
      gpuCounts = 0; // All 0s
    // writeln("Bounds for chunk: ", startChunk, " to ", startChunk+numChunksThisGpu-1);
    const arrSize = gpuInputArr.size;
    const low = gpuInputArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

    @assertOnGpu
    foreach chunk in startChunk..#numChunksThisGpu {
      // Count for each chunk in parallel.
      const startIdx : int = (chunk:int)*chunkSize;
      const endIdx : int = min(arrSize,startIdx+chunkSize);
      for i in startIdx..<endIdx {
        const arrIdx = i+low;
        const tmp = ((gpuInputArr[arrIdx]>>exp) & bitMask):int;
        const tmp2 = chunk+(numChunks*tmp);
        gpuCounts[tmp2] += 1;
      }
    }
  }

  // Multi GPU Experimental function
  // This won't work for now since it wasn't updated with the modularization of radix sort
  private proc distributedCount(ref counts : [], ref gpuInputArr : [] ?t, const exp : int, const bitMask : int,
                        const numChunks : int, const numGpus : int ) where isCoercible(t, uint){
    // Counts should be all 0s
    // counts = 0;

    const numChunksPerGpu = Math.divCeil(numChunks, numGpus);
    const numChunksPerGpuLast = numChunks - (numChunksPerGpu * (numGpus-1));
    var firstRunReset = true;

    // Distribute the counts across the GPUs
    for gpu in 0..<numGpus {
      const numChunksThisGpu = if gpu == numGpus-1 then numChunksPerGpuLast else numChunksPerGpu;
      const startChunk = gpu*numChunksPerGpu;

      // Distributing the counts across the GPUs
      // Each GPU works on a subset of chunks from the array
      // Since each chunk already gets a slot in the counts array
      // We will pass the entire counts array to each GPU
      // But each GPU will only work on a subset of the counts array and not care about the rest
      parallelCount(counts, gpuInputArr, exp, bitMask, numChunks, numChunksThisGpu, startChunk, gpu, firstRunReset);
      // writeln(" Count: ", thisCounts);
      // This works because counts array is all 0s and
      // we're only adding to it. The parallelCount function leaves the
      // rest of the array untouched (i.e. 0s) so for 2 GPUs the add might be:
      // counts1 : 1 0 | 3 0 | 2 0 | 32 0
      // counts2 : 0 3 | 0 2 | 0 12 | 0 2
      // counts      : 1 3 | 3 2 | 2 12 | 32 2
      // I think this is not a race condition for the same reason
      firstRunReset = false; // This is a hacky solution


      // In an ideal world I would like to use a + reduce intent here such that each iteration gets it's own
      // copy of the counts array and it is accumulated into the main counts array
      // And the loop should be a coforall instead of a foreach
      // But I could not get that to work/ don't know how that works
    }
  }

  private proc parallelScatter(ref gpuOffsets : [], ref gpuInputArr : [] ?t,
                       const exp : int, const bitMask : int,
                       const chunkSize : int, const numChunks : int) where isCoercible(t, uint){

    var gpuOutputArr : gpuInputArr.type;
    const arrSize = gpuInputArr.size;
    const low = gpuInputArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

    @assertOnGpu
    foreach chunk in 0..<numChunks {
      // Count for each chunk in parallel.
      const startIdx : int = (chunk:int)*chunkSize;
      const endIdx : int = min(arrSize,startIdx+chunkSize);
      for i in startIdx..<endIdx {
        const arrIdx = i+low;
        const tmp = ((gpuInputArr[arrIdx]>>exp) & bitMask):int; // Where in the counts array to look
        const tmp2 = chunk+(numChunks*tmp); // Index into the offsets array
        const tmp3 = gpuOffsets[tmp2]:int; // Index into the output array
        // This may happen when gpuInputArr.size%chunkSize!=0
        // if tmp3>=gpuInputArr.size then continue;
        gpuOutputArr[low+tmp3] = gpuInputArr[arrIdx];
        gpuOffsets[tmp2] += 1;
      }
    }
    gpuInputArr = gpuOutputArr;
  }

  /*
    Returns a :record:`DeviceAttributes` record containing various properties describing
    the GPU associated with a sublocale.  The available properties reflect a subset
    of those available in both the `CUDA API documentation
    <https://docs.nvidia.com/cuda/cuda-runtime-api/structcudaDeviceProp.html#structcudaDeviceProp>`_
    and `HIP API documentation
    <https://rocm.docs.amd.com/projects/HIP/en/docs-6.0.0/doxygen/html/group___global_defs.html#gacc0acd7b9bda126c6bb3dfd6e2796d7c>`_.

    :arg loc: GPU sublocale to get device attributes on.
   */
  proc deviceAttributes(loc) {
    return new DeviceAttributes(loc);
  }

  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_BLOCK : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_X : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Y : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Z : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_X : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Y : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Z : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_BLOCK : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__TOTAL_CONSTANT_MEMORY : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__WARP_SIZE : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_PITCH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_WIDTH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_WIDTH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_HEIGHT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_WIDTH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_HEIGHT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_DEPTH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_REGISTERS_PER_BLOCK : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__CLOCK_RATE : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__TEXTURE_ALIGNMENT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__TEXTURE_PITCH_ALIGNMENT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MULTIPROCESSOR_COUNT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__KERNEL_EXEC_TIMEOUT : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__INTEGRATED : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__CAN_MAP_HOST_MEMORY : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__COMPUTE_MODE : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__PROCESS : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__CONCURRENT_KERNELS : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__ECC_ENABLED : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__PCI_BUS_ID : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__PCI_DEVICE_ID : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MEMORY_CLOCK_RATE : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__GLOBAL_MEMORY_BUS_WIDTH : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__L2_CACHE_SIZE : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_MULTIPROCESSOR : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MAJOR : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MINOR : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_MULTIPROCESSOR : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MANAGED_MEMORY : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__MULTI_GPU_BOARD : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__CONCURRENT_MANAGED_ACCESS : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES : c_int;
  @chpldoc.nodoc
  extern const CHPL_GPU_ATTRIBUTE__DIRECT_MANAGED_MEM_ACCESS_FROM_HOST : c_int;

  @chpldoc.nodoc
  extern proc chpl_gpu_query_attribute(dev : c_int, attribute : c_int) : c_int;

  /*
    Record returned by :proc:`deviceAttributes` that properties reflect a
    subset of device properties available in both the `CUDA API documentation
    <https://docs.nvidia.com/cuda/cuda-runtime-api/structcudaDeviceProp.html#structcudaDeviceProp>`_
    and `HIP API documentation
    <https://rocm.docs.amd.com/projects/HIP/en/docs-6.0.0/doxygen/html/group___global_defs.html#gacc0acd7b9bda126c6bb3dfd6e2796d7c>`_.
   */
  record DeviceAttributes {
    @chpldoc.nodoc
    var gpuId : int;

    @chpldoc.nodoc
    proc init(loc) {
      if !loc.isGpu() then halt("gpuDeviceInfo must be passed gpu locale");
      this.gpuId = 0; // TODO: Should be loc.gpuId
    }

    proc name : string {
      extern proc chpl_gpu_name(dev : c_int, ref result : c_ptrConst(c_char));
      var ret : string;
      var tmp : c_ptrConst(c_char);

      chpl_gpu_name(this.gpuId : c_int, tmp);
      try! {
        ret = string.createCopyingBuffer(tmp, policy=decodePolicy.escape);
      }
      deallocate(tmp);

      return ret;
    }

    proc maxThreadsPerBlock : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_BLOCK);
    proc maxBlockDimX : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_X);
    proc maxBlockDimY : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Y);
    proc maxBlockDimZ : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_BLOCK_DIM_Z);
    proc MaxGridDimX : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_X);
    proc maxGridDimY : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Y);
    proc maxGridDimZ : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_GRID_DIM_Z);
    proc maxSharedMemoryPerBlock : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_BLOCK);
    proc totalConstantMemory : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__TOTAL_CONSTANT_MEMORY);
    proc warpSize : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__WARP_SIZE);
    proc maxPitch : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_PITCH);
    proc maximumTexture1dWidth : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE1D_WIDTH);
    proc maximumTexture2dWidth : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_WIDTH);
    proc maximumTexture2dHeight : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE2D_HEIGHT);
    proc maximumTexture3dWidth : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_WIDTH);
    proc maximumTexture3dHeight : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_HEIGHT);
    proc maximumTexture3dDepth : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAXIMUM_TEXTURE3D_DEPTH);
    proc maxRegistersPerBlock : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_REGISTERS_PER_BLOCK);
    proc clockRate : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__CLOCK_RATE);
    proc textureAlignment : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__TEXTURE_ALIGNMENT);
    proc texturePitch_alignment : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__TEXTURE_PITCH_ALIGNMENT);
    proc multiprocessorCount : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MULTIPROCESSOR_COUNT);
    proc kernelExecTimeout : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__KERNEL_EXEC_TIMEOUT);
    proc integrated : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__INTEGRATED);
    proc canMapHostMemory : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__CAN_MAP_HOST_MEMORY);
    proc computeMode : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__COMPUTE_MODE);
    proc concurrentKernels : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__CONCURRENT_KERNELS);
    proc eccEnabled : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__ECC_ENABLED);
    proc pciBusId : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__PCI_BUS_ID);
    proc pciDeviceId : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__PCI_DEVICE_ID);
    proc memoryClockRate : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MEMORY_CLOCK_RATE);
    proc globalMemoryBusWidth : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__GLOBAL_MEMORY_BUS_WIDTH);
    proc l2CacheSize : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__L2_CACHE_SIZE);
    proc maxThreadsPerMultiprocessor : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_THREADS_PER_MULTIPROCESSOR);
    proc computeCapabilityMajor : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MAJOR);
    proc computeCapabilityMinor : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__COMPUTE_CAPABILITY_MINOR);
    proc maxSharedMemoryPerMultiprocessor : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MAX_SHARED_MEMORY_PER_MULTIPROCESSOR);
    proc managedMemory : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MANAGED_MEMORY);
    proc multiGpuBoard : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__MULTI_GPU_BOARD);
    proc pageableMemoryAccess : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS);
    proc concurrentManagedAccess : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__CONCURRENT_MANAGED_ACCESS);
    proc pageableMemoryAccessUsesHostPageTables : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES);
    proc directManagedMemAccessFromHost : int do return chpl_gpu_query_attribute(this.gpuId : c_int, CHPL_GPU_ATTRIBUTE__DIRECT_MANAGED_MEM_ACCESS_FROM_HOST);
  }
}
