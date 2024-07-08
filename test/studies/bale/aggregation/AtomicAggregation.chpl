module AtomicAggregation {
  use ChplConfig;
  use CTypes;
  use AggregationPrimitives;

  private param defaultBuffSize =
    if CHPL_TARGET_PLATFORM == "hpe-cray-ex" then 1024
    else if CHPL_COMM == "ugni" then 4096
    else 8192;

  private config const yieldFrequency = getEnvInt("AGGREGATION_YIELD_FREQUENCY", 1024);
  private config const amoBuffSize = getEnvInt("AGGREGATION_AMO_BUFF_SIZE", defaultBuffSize*2);

  private config param aggregate = CHPL_COMM != "none";

  proc AggregatedAtomic(type T) type {
    return chpl__processorAtomicType(T);
  }


  /*
   * Aggregates atomic increments. e.g. atomic.add(1). This is a specialization
   * of a non-fetching atomic aggregator that does not have to buffer values.
   * Not parallel safe and is expected to be created on a per-task basis
   * High memory usage since there are per-destination buffers
   */
  record AtomicIncAggregator {
    type elemType;
    @chpldoc.nodoc
    var agg: if aggregate then AtomicIncAggregatorImpl(elemType) else nothing;
    inline proc ref inc(ref dst: AggregatedAtomic(elemType)) {
      if aggregate then agg.inc(dst);
                   else dst.add(1, memoryOrder.relaxed);
    }
    inline proc copy(ref dst: elemType, const in srcVal: elemType) {
      if aggregate then agg.copy(dst, srcVal);
                   else dst = srcVal;
    }
    inline proc flush() {
      if aggregate then agg.flush();
    }
  }

  @chpldoc.nodoc
  record AtomicIncAggregatorImpl {
    type elemType;
    type aggType = c_ptr(AggregatedAtomic(elemType));
    const bufferSize = amoBuffSize;
    const myLocaleSpace = 0..<numLocales;
    var lastLocale: int;
    var opsUntilYield = yieldFrequency;
    var lBuffers:  c_ptr(c_ptr(aggType));
    var rBuffers: [myLocaleSpace] remoteBuffer(aggType);
    var bufferIdxs: c_ptr(int);

    proc ref postinit() {
      lBuffers = allocate(c_ptr(aggType), numLocales:c_size_t);
      bufferIdxs = bufferIdxAlloc();
      for loc in myLocaleSpace {
        lBuffers[loc] = allocate(aggType, bufferSize:c_size_t);
        bufferIdxs[loc] = 0;
        rBuffers[loc] = new remoteBuffer(aggType, bufferSize, loc);
      }
    }

    proc ref deinit() {
      flush();
      for loc in myLocaleSpace {
        deallocate(lBuffers[loc]);
      }
      deallocate(lBuffers);
      deallocate(bufferIdxs);
    }

    proc ref flush() {
      for offsetLoc in myLocaleSpace + lastLocale {
        const loc = offsetLoc % numLocales;
        _flushBuffer(loc, bufferIdxs[loc], freeData=true);
      }
    }

    inline proc ref inc(ref dst: AggregatedAtomic(elemType)) {
      // Get the locale of dst and the local address on that locale
      const loc = dst.locale.id;
      lastLocale = loc;
      const dstAddr = getAddr(dst);

      // Get our current index into the buffer for dst's locale
      ref bufferIdx = bufferIdxs[loc];

      // Buffer the address
      lBuffers[loc][bufferIdx] = dstAddr;
      bufferIdx += 1;

      // Flush our buffer if it's full. If it's been a while since we've let
      // other tasks run, yield so that we're not blocking remote tasks from
      // flushing their buffers.
      if bufferIdx == bufferSize {
        _flushBuffer(loc, bufferIdx, freeData=false);
        opsUntilYield = yieldFrequency;
      } else if opsUntilYield == 0 {
        currentTask.yieldExecution();
        opsUntilYield = yieldFrequency;
      } else {
        opsUntilYield -= 1;
      }
    }

    proc ref _flushBuffer(loc: int, ref bufferIdx, freeData) {
      const myBufferIdx = bufferIdx;
      if myBufferIdx == 0 then return;

      // Allocate a remote buffer
      ref rBuffer = rBuffers[loc];
      const remBufferPtr = rBuffer.cachedAlloc();

      // Copy local buffer to remote buffer
      rBuffer.PUT(lBuffers[loc], myBufferIdx);

      // Process remote buffer
      on Locales[loc] {
        for dstAddr in rBuffer.localIter(remBufferPtr, myBufferIdx) {
          dstAddr.deref().add(1, memoryOrder.relaxed);
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

}
