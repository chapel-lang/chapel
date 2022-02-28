module CommAggregation {
  use SysCTypes;
  use CPtr;
  use UnorderedCopy;
  private use CommPrimitives;

  // TODO these parameters need to be tuned and size should be user-settable at
  // creation time. iters before yield should be based on numLocales & buffSize
  private config const maxItersBeforeYield = 4096;
  private config const dstBuffSize = getEnvInt("ARKOUDA_SERVER_AGGREGATION_DST_BUFF_SIZE", 4096);
  private config const srcBuffSize = getEnvInt("ARKOUDA_SERVER_AGGREGATION_SRC_BUFF_SIZE", 4096);

  record AtomicAggregator {
    type elemType;
    type aggType = (c_ptr(chpl__processorAtomicType(elemType)), elemType);
    const bufferSize = dstBuffSize;
    const myLocaleSpace = 0..<numLocales;
    var itersSinceYield: int;
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

    inline proc xor(ref dst: chpl__processorAtomicType(elemType), const in srcVal: elemType) {
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
      } else if itersSinceYield % maxItersBeforeYield == 0 {
        chpl_task_yield();
        itersSinceYield = 0;
      }
      itersSinceYield += 1;
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
          dstAddr.deref().xor(srcVal, memoryOrder.relaxed);
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
      // TODO align up to 8-byte boundary to avoid misaligned comm perf hit
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
      // TODO align up to 8-byte boundary to avoid misaligned comm perf hit
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

  private proc getEnvInt(name: string, default: int): int {
    extern proc getenv(name : c_string) : c_string;
    var strval = getenv(name.localize().c_str()): string;
    if strval.isEmpty() { return default; }
    return try! strval: int;
  }
}
