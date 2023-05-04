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

/* Provides support for aggregated copies/assignments for trivial types.

   .. warning::
     This module represents work in progress. The API is unstable and likely to
     change over time.

   This module provides an aggregated version of copy/assignment for trivially
   copyable types. Trivially copyable types require no special behavior to be
   copied and merely copying their representation is sufficient. They include
   ``numeric`` and ``bool`` types as well as tuples or records consisting only
   of ``numeric``/``bool``. Records cannot have user-defined copy-initializers,
   deinitializers, or assignment overloads.

   Aggregated copies can provide a significant speedup for batch assignment
   operations that do not require ordering of operations. The results of
   aggregated operations are not visible until the aggregator is deinitialized
   or an explicit ``flush()`` call is made. The current implementation requires
   that one side of the copy is always local. When the source is always local
   and the destination may be remote a :record:`DstAggregator` should be used.
   When the destination is always local and the source may be remote, a
   :record:`SrcAggregator` should be used. Note that aggregators are not
   parallel safe and are expected to be created on a per-task basis. Each
   aggregator has a per-peer buffer, so memory usage increases with number of
   locales. The following example demonstrates  using :record:`SrcAggregator`
   to reverse an array:

   .. code-block:: chapel

     use BlockDist, CopyAggregation;

     const size = 10000;
     const space = {0..size};
     const D = space dmapped Block(space);
     var A, reversedA: [D] int = D;

     forall (rA, i) in zip(reversedA, D) with (var agg = new SrcAggregator(int)) do
       agg.copy(rA, A[size-i]);

     // no flush required, flushed when aggregator is deinitialized

     forall (rA, i) in zip(reversedA, D) do
       assert(rA == size-i);


   It's important to be aware that aggregated operations are not consistent with
   regular operations and updates may not be visible until the aggregator is
   deinitialized or an explicit call to ``flush()`` is made.

   .. code-block:: chapel

     use CopyAggregation;

     var a = 0;
     on Locales[numLocales-1] {
       var agg = new DstAggregator(int);
       var b = 1;
       agg.copy(a, b);
       writeln(a);   // can print 0 or 1
       agg.flush();
       writeln(a);   // must print 1
     }

   Generally speaking aggregators are useful for when you have a large batch of
   remote assignments to perform and the order of those operations doesn't
   matter.
 */
module CopyAggregation {
  use ChplConfig;
  use CTypes;
  use AggregationPrimitives;

  private config param verboseAggregation = false;

  private param defaultBuffSize = if CHPL_COMM == "ugni" then 4096 else 8192;
  private const yieldFrequency = getEnvInt("CHPL_AGGREGATION_YIELD_FREQUENCY", 1024);
  private const dstBuffSize = getEnvInt("CHPL_AGGREGATION_DST_BUFF_SIZE", defaultBuffSize);
  private const srcBuffSize = getEnvInt("CHPL_AGGREGATION_SRC_BUFF_SIZE", defaultBuffSize);

  private config param aggregate = CHPL_COMM != "none";

  /*
     Aggregates ``copy(ref dst, src)``. Optimized for when src is local.
     Not parallel safe and is expected to be created on a per-task basis.
     High memory usage since there are per-destination buffers.
   */
  record DstAggregator {
    type elemType;
    pragma "no doc"
    var agg: if aggregate then DstAggregatorImpl(elemType) else nothing;
    inline proc copy(ref dst: elemType, const in srcVal: elemType) {
      if aggregate then agg.copy(dst, srcVal);
                   else dst = srcVal;
    }
    inline proc flush() {
      if aggregate then agg.flush();
    }
  }

  /*
     Aggregates ``copy(ref dst, const ref src)``. Only works when dst is local.
     Not parallel safe and is expected to be created on a per task basis.
     High memory usage since there are per-destination buffers.
   */
  record SrcAggregator {
    type elemType;
    pragma "no doc"
    var agg: if aggregate then SrcAggregatorImpl(elemType) else nothing;
    inline proc copy(ref dst: elemType, const ref src: elemType) {
      if aggregate then agg.copy(dst, src);
                   else dst = src;
    }
    inline proc flush() {
      if aggregate then agg.flush();
    }
  }

  pragma "no doc"
  record DstAggregatorImpl {
    type elemType;
    type aggType = (c_ptr(elemType), elemType);
    const bufferSize = dstBuffSize;
    const myLocaleSpace = 0..<numLocales;
    var lastLocale: int;
    var opsUntilYield = yieldFrequency;
    var lBuffers: c_ptr(c_ptr(aggType));
    var rBuffers: [myLocaleSpace] remoteBuffer(aggType);
    var bufferIdxs: c_ptr(int);

    proc postinit() {
      lBuffers = c_malloc(c_ptr(aggType), numLocales);
      bufferIdxs = bufferIdxAlloc();
      for loc in myLocaleSpace {
        lBuffers[loc] = c_malloc(aggType, bufferSize);
        bufferIdxs[loc] = 0;
        rBuffers[loc] = new remoteBuffer(aggType, bufferSize, loc);
      }
    }

    proc deinit() {
      flush();
      for loc in myLocaleSpace {
        c_free(lBuffers[loc]);
      }
      c_free(lBuffers);
      c_free(bufferIdxs);
    }

    proc flush() {
      for offsetLoc in myLocaleSpace + lastLocale {
        const loc = offsetLoc % numLocales;
        _flushBuffer(loc, bufferIdxs[loc], freeData=true);
      }
    }

    inline proc copy(ref dst: elemType, const in srcVal: elemType) {
      if verboseAggregation {
        writeln("DstAggregator.copy is called");
      }
      // Get the locale of dst and the local address on that locale
      const loc = dst.locale.id;
      lastLocale = loc;
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

  pragma "no doc"
  record SrcAggregatorImpl {
    type elemType;
    type aggType = c_ptr(elemType);
    const bufferSize = srcBuffSize;
    const myLocaleSpace = 0..<numLocales;
    var lastLocale: int;
    var opsUntilYield = yieldFrequency;
    var dstAddrs: c_ptr(c_ptr(aggType));
    var lSrcAddrs: c_ptr(c_ptr(aggType));
    var lSrcVals: [myLocaleSpace][0..#bufferSize] elemType;
    var rSrcAddrs: [myLocaleSpace] remoteBuffer(aggType);
    var rSrcVals: [myLocaleSpace] remoteBuffer(elemType);
    var bufferIdxs: c_ptr(int);

    proc postinit() {
      dstAddrs = c_malloc(c_ptr(aggType), numLocales);
      lSrcAddrs = c_malloc(c_ptr(aggType), numLocales);
      bufferIdxs = bufferIdxAlloc();
      for loc in myLocaleSpace {
        dstAddrs[loc] = c_malloc(aggType, bufferSize);
        lSrcAddrs[loc] = c_malloc(aggType, bufferSize);
        bufferIdxs[loc] = 0;
        rSrcAddrs[loc] = new remoteBuffer(aggType, bufferSize, loc);
        rSrcVals[loc] = new remoteBuffer(elemType, bufferSize, loc);
      }
    }

    proc deinit() {
      flush();
      for loc in myLocaleSpace {
        c_free(dstAddrs[loc]);
        c_free(lSrcAddrs[loc]);
      }
      c_free(dstAddrs);
      c_free(lSrcAddrs);
      c_free(bufferIdxs);
    }

    proc flush() {
      for offsetLoc in myLocaleSpace + lastLocale {
        const loc = offsetLoc % numLocales;
        _flushBuffer(loc, bufferIdxs[loc], freeData=true);
      }
    }

    inline proc copy(ref dst: elemType, const ref src: elemType) {
      if verboseAggregation {
        writeln("SrcAggregator.copy is called");
      }
      if boundsChecking {
        assert(dst.locale.id == here.id);
      }
      const dstAddr = getAddr(dst);

      const loc = src.locale.id;
      lastLocale = loc;
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
      var dstAddrPtr = c_addrOf(dstAddrs[loc][0]);
      var srcValPtr = c_addrOf(myLSrcVals[0]);
      for i in 0..<myBufferIdx {
        dstAddrPtr[i].deref() = srcValPtr[i];
      }

      bufferIdx = 0;
    }
  }
}


pragma "no doc"
module AggregationPrimitives {
  use CTypes;
  use Communication;
  public import Communication.get as GET;
  public import Communication.put as PUT;

  inline proc getAddr(const ref p): c_ptr(p.type) {
    // TODO can this use c_ptrTo?
    return __primitive("_wide_get_addr", p): c_ptr(p.type);
  }

  // Cacheline aligned and padded allocation to avoid false-sharing
  inline proc bufferIdxAlloc() {
    const cachePaddedLocales = (numLocales + 7) & ~7;
    return c_aligned_alloc(int, 64, cachePaddedLocales);
  }

  proc getEnvInt(name: string, default: int): int {
    extern proc getenv(name : c_string) : c_string;
    var strval = getenv(name.localize().c_str()): string;
    if strval.isEmpty() { return default; }
    return try! strval: int;
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
      foreach i in 0..<size {
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
        assert(lArr.domain.lowBound == 0);
        assert(lArr.locale.id == here.id);
      }
      const byte_size = size:c_size_t * c_sizeof(elemType);
      AggregationPrimitives.PUT(data, c_ptrTo(lArr[0]), loc, byte_size);
    }

    proc PUT(lArr: c_ptr(elemType), size: int) {
      if boundsChecking {
        assert(size <= this.size);
      }
      const byte_size = size:c_size_t * c_sizeof(elemType);
      AggregationPrimitives.PUT(data, lArr, loc, byte_size);
    }

    proc GET(lArr: [] elemType, size: int) where lArr.isDefaultRectangular() {
      if boundsChecking {
        assert(size <= this.size);
        assert(this.size == lArr.size);
        assert(lArr.domain.lowBound == 0);
        assert(lArr.locale.id == here.id);
      }
      const byte_size = size:c_size_t * c_sizeof(elemType);
      AggregationPrimitives.GET(c_ptrTo(lArr[0]), data, loc, byte_size);
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
}
