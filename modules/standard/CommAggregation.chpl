module CommAggregation {
  use SysCTypes;
  use CPtr;
  use UnorderedCopy;
  use CommPrimitives;

  config param verboseAggregation = true;

  // TODO should tune these values at startup
  private config const yieldFrequency = getEnvInt("ARKOUDA_SERVER_AGGREGATION_YIELD_FREQUENCY", 1024);
  private config const dstBuffSize = getEnvInt("ARKOUDA_SERVER_AGGREGATION_DST_BUFF_SIZE", 4096);
  private config const srcBuffSize = getEnvInt("ARKOUDA_SERVER_AGGREGATION_SRC_BUFF_SIZE", 4096);


  /* Creates a new destination aggregator (dst/lhs will be remote). */
  proc newDstAggregator(type elemType, param useUnorderedCopy=false) {
    if CHPL_COMM == "none" || useUnorderedCopy {
      return new DstUnorderedAggregator(elemType);
    } else {
      return new DstAggregator(elemType);
    }
  }

  /* Creates a new source aggregator (src/rhs will be remote). */
  proc newSrcAggregator(type elemType, param useUnorderedCopy=false) {
    if CHPL_COMM == "none" || useUnorderedCopy {
      return new SrcUnorderedAggregator(elemType);
    } else {
      return new SrcAggregator(elemType);
    }
  }

  /*
   * Aggregates copy(ref dst, src). Optimized for when src is local.
   * Not parallel safe and is expected to be created on a per-task basis
   * High memory usage since there are per-destination buffers
   */
  record DstAggregator {
    type elemType;
    type aggType = (c_ptr(elemType), elemType);
    const bufferSize = dstBuffSize;
    const myLocaleSpace = LocaleSpace;
    var opsUntilYield = yieldFrequency;
    var lBuffers: [myLocaleSpace] [0..#bufferSize] aggType;
    var rBuffers: [myLocaleSpace] remoteBuffer(aggType);
    var bufferIdxs: [myLocaleSpace] int;

    proc postinit() {
      for loc in myLocaleSpace {
        rBuffers[loc] = new remoteBuffer(aggType, bufferSize, loc);
      }
    }

    proc deinit() {
      flush();
    }

    proc flush() {
      for loc in myLocaleSpace {
        _flushBuffer(loc, bufferIdxs[loc], freeData=true);
      }
    }

    inline proc copy(ref dst: elemType, const in srcVal: elemType) {
      if verboseAggregation {
        writeln("DstAggregator.copy is called");
      }
      // Get the locale of dst and the local address on that locale
      const loc = dst.locale.id;
      const dstAddr = getAddr(dst);

      // Get our current index into the buffer for dst's locale
      ref bufferIdx = bufferIdxs[loc];

      // Buffer the address and desired value
      lBuffers[loc][bufferIdx] = (dstAddr, srcVal);
      bufferIdx += 1;

      // Flush our buffer if it's full. If it's been a while since we've let
      // other tasks run, yield so that we're not blocking remote tasks from
      // flushing their buffers.
      if bufferIdx == bufferSize {
        _flushBuffer(loc, bufferIdx, freeData=false);
        opsUntilYield = yieldFrequency;
      } else if opsUntilYield == 0 {
        chpl_task_yield();
        opsUntilYield = yieldFrequency;
      } else {
        opsUntilYield -= 1;
      }
    }

    proc _flushBuffer(loc: int, ref bufferIdx, freeData) {
      const myBufferIdx = bufferIdx;
      if myBufferIdx == 0 then return;

      // Allocate a remote buffer
      ref rBuffer = rBuffers[loc];
      const remBufferPtr = rBuffer.cachedAlloc();

      // Copy local buffer to remote buffer
      rBuffer.PUT(lBuffers[loc], myBufferIdx);

      // Process remote buffer
      on Locales[loc] {
        for (dstAddr, srcVal) in rBuffer.localIter(remBufferPtr, myBufferIdx) {
          dstAddr.deref() = srcVal;
        }
        if freeData {
          rBuffer.localFree(remBufferPtr);
        }
      }
      if freeData {
        rBuffer.markFreed();
      }
      bufferIdx = 0;
    }
  }



  /* "Aggregator" that uses unordered copy instead of actually aggregating */
  pragma "no doc"
  record DstUnorderedAggregator {
    type elemType;

    proc deinit() {
      flush();
    }
    proc flush() {
      unorderedCopyTaskFence();
    }
    inline proc copy(ref dst: elemType, const in srcVal: elemType) {
      unorderedCopyWrapper(dst, srcVal);
    }
  }


  /*
   * Aggregates copy(ref dst, const ref src). Only works when dst is local.
   * Not parallel safe and is expected to be created on a per task basis
   * High memory usage since there are per-destination buffers
   */
  record SrcAggregator {
    type elemType;
    type aggType = c_ptr(elemType);
    const bufferSize = srcBuffSize;
    const myLocaleSpace = LocaleSpace;
    var opsUntilYield = yieldFrequency;
    var dstAddrs: [myLocaleSpace][0..#bufferSize] aggType;
    var lSrcAddrs: [myLocaleSpace][0..#bufferSize] aggType;
    var lSrcVals: [myLocaleSpace][0..#bufferSize] elemType;
    var rSrcAddrs: [myLocaleSpace] remoteBuffer(aggType);
    var rSrcVals: [myLocaleSpace] remoteBuffer(elemType);

    var bufferIdxs: [myLocaleSpace] int;

    proc postinit() {
      for loc in myLocaleSpace {
        rSrcAddrs[loc] = new remoteBuffer(aggType, bufferSize, loc);
        rSrcVals[loc] = new remoteBuffer(elemType, bufferSize, loc);
      }
    }

    proc deinit() {
      flush();
    }

    proc flush() {
      for loc in myLocaleSpace {
        _flushBuffer(loc, bufferIdxs[loc], freeData=true);
      }
    }

    inline proc copy(ref dst: elemType, const ref src: elemType) {
      if verboseAggregation {
        writeln("SrcAggregator.copy is called");
      }
      assert(dst.locale.id == here.id);
      const dstAddr = getAddr(dst);

      const loc = src.locale.id;
      const srcAddr = getAddr(src);

      ref bufferIdx = bufferIdxs[loc];
      lSrcAddrs[loc][bufferIdx] = srcAddr;
      dstAddrs[loc][bufferIdx] = dstAddr;
      bufferIdx += 1;

      if bufferIdx == bufferSize {
        _flushBuffer(loc, bufferIdx, freeData=false);
        opsUntilYield = yieldFrequency;
      } else if opsUntilYield == 0 {
        chpl_task_yield();
        opsUntilYield = yieldFrequency;
      } else {
        opsUntilYield -= 1;
      }
    }

    proc _flushBuffer(loc: int, ref bufferIdx, freeData) {
      const myBufferIdx = bufferIdx;
      if myBufferIdx == 0 then return;

      ref myLSrcVals = lSrcVals[loc];
      ref myRSrcAddrs = rSrcAddrs[loc];
      ref myRSrcVals = rSrcVals[loc];

      // Allocate remote buffers
      const rSrcAddrPtr = myRSrcAddrs.cachedAlloc();
      const rSrcValPtr = myRSrcVals.cachedAlloc();

      // Copy local addresses to remote buffer
      myRSrcAddrs.PUT(lSrcAddrs[loc], myBufferIdx);

      // Process remote buffer, copying the value of our addresses into a
      // remote buffer
      on Locales[loc] {
        for i in 0..<myBufferIdx {
          rSrcValPtr[i] = rSrcAddrPtr[i].deref();
        }
        if freeData {
          myRSrcAddrs.localFree(rSrcAddrPtr);
        }
      }
      if freeData {
        myRSrcAddrs.markFreed();
      }

      // Copy remote values into local buffer
      myRSrcVals.GET(myLSrcVals, myBufferIdx);

      // Assign the srcVal to the dstAddrs
      var dstAddrPtr = c_ptrTo(dstAddrs[loc][0]);
      var srcValPtr = c_ptrTo(myLSrcVals[0]);
      for i in 0..<myBufferIdx {
        dstAddrPtr[i].deref() = srcValPtr[i];
      }

      bufferIdx = 0;
    }
  }

  /* "Aggregator" that uses unordered copy instead of actually aggregating */
  pragma "no doc"
  record SrcUnorderedAggregator {
    type elemType;

    proc deinit() {
      flush();
    }
    proc flush() {
      unorderedCopyTaskFence();
    }
    inline proc copy(ref dst: elemType, const ref src: elemType) {
      assert(dst.locale.id == here.id);
      unorderedCopyWrapper(dst, src);
    }
  }


  // A remote buffer with lazy allocation
  record remoteBuffer {
    type elemType;
    var size: int;
    var loc: int;
    var data: c_ptr(elemType);

    // Allocate a buffer on loc if we haven't already. Return a c_ptr to the
    // remote locales buffer
    proc cachedAlloc(): c_ptr(elemType) {
      if data == c_nil {
        const rvf_size = size;
        on Locales[loc] do {
          data = c_malloc(elemType, rvf_size);
        }
      }
      return data;
    }

    // Iterate through buffer elements, must be running on loc. data is passed
    // in to avoid communication.
    iter localIter(data: c_ptr(elemType), size: int) ref : elemType {
      if boundsChecking {
        assert(this.loc == here.id);
        assert(this.data == data);
        assert(data != c_nil);
      }
      for i in 0..<size {
        yield data[i];
      }
    }

    // Free the data, must be running on the owning locale, data is passed in
    // to avoid communication. Data is freed'd automatically when this record
    // goes out of scope, but this is an optimization to free when already
    // running on loc
    inline proc localFree(data: c_ptr(elemType)) {
      if boundsChecking {
        assert(this.loc == here.id);
        assert(this.data == data);
        assert(data != c_nil);
      }
      c_free(data);
    }

    // After free'ing the data, need to nil out the records copy of the pointer
    // so we don't double-free on deinit
    inline proc markFreed() {
      if boundsChecking {
        assert(this.locale.id == here.id);
      }
      data = c_nil;
    }

    // Copy size elements from lArr to the remote buffer. Must be running on
    // lArr's locale.
    proc PUT(lArr: [] elemType, size: int) where lArr.isDefaultRectangular() {
      if boundsChecking {
        assert(size <= this.size);
        assert(this.size == lArr.size);
        assert(lArr.domain.low == 0);
        assert(lArr.locale.id == here.id);
      }
      const byte_size = size:size_t * c_sizeof(elemType);
      CommPrimitives.PUT(c_ptrTo(lArr[0]), loc, data, byte_size);
    }

    proc GET(lArr: [] elemType, size: int) where lArr.isDefaultRectangular() {
      if boundsChecking {
        assert(size <= this.size);
        assert(this.size == lArr.size);
        assert(lArr.domain.low == 0);
        assert(lArr.locale.id == here.id);
      }
      const byte_size = size:size_t * c_sizeof(elemType);
      CommPrimitives.GET(c_ptrTo(lArr[0]), loc, data, byte_size);
    }

    proc deinit() {
      if data != c_nil {
        const rvf_data=data;
        on Locales[loc] {
          localFree(rvf_data);
        }
        markFreed();
      }
    }
  }

  //
  // Helper routines
  //

  // Unordered copy wrapper that also supports tuples. In 1.20 this will call
  // unorderedCopy for each tuple element, but in 1.21 it is a single call.
  private inline proc unorderedCopyWrapper(ref dst, const ref src): void {
    use Reflection;
    // Always resolves in 1.21, only resolves for numeric/bool types in 1.20
    if canResolve("unorderedCopy", dst, src) {
      unorderedCopy(dst, src);
    } else if isTuple(dst) && isTuple(src) {
      for param i in 1..dst.size {
        unorderedCopyWrapper(dst(i), src(i));
      }
    } else {
      compilerWarning("Missing optimized unorderedCopy for " + dst.type:string);
      dst = src;
    }
  }

  private proc getEnvInt(name: string, default: int): int {
    extern proc getenv(name : c_string) : c_string;
    var strval = getenv(name.localize().c_str()): string;
    if strval.isEmpty() { return default; }
    return try! strval: int;
  }
}
