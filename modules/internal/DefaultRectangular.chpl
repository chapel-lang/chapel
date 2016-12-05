/*
 * Copyright 2004-2016 Cray Inc.
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

// DefaultRectangular.chpl
//
module DefaultRectangular {

  config const dataParTasksPerLocale = 0;
  config const dataParIgnoreRunningTasks = if CHPL_LOCALE_MODEL=="numa" then true
                                           else false;
  config const dataParMinGranularity: int = 1;

  if dataParTasksPerLocale<0 then halt("dataParTasksPerLocale must be >= 0");
  if dataParMinGranularity<=0 then halt("dataParMinGranularity must be > 0");

  use DSIUtil, ChapelArray;
  config param debugDefaultDist = false;
  config param debugDefaultDistBulkTransfer = false;
  config param debugDataPar = false;
  config param debugDataParNuma = false;

  config param defaultDoRADOpt = true;
  config param defaultDisableLazyRADOpt = false;
  config param earlyShiftData = true;

  class DefaultDist: BaseDist {
    proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool)
      return new DefaultRectangularDom(rank, idxType, stridable, this);

    proc dsiNewAssociativeDom(type idxType, param parSafe: bool)
      return new DefaultAssociativeDom(idxType, parSafe, this);

    proc dsiNewOpaqueDom(type idxType, param parSafe: bool)
      return new DefaultOpaqueDom(this, parSafe);

    proc dsiNewSparseDom(param rank: int, type idxType, dom: domain)
      return new DefaultSparseDom(rank, idxType, this, dom);

    proc dsiIndexToLocale(ind) return this.locale;

    // Right now, the default distribution acts like a singleton.
    // So we don't have to copy it when a clone is requested.
    proc dsiClone() return this;

    proc dsiAssign(other: this.type) { }

    proc dsiCreateReindexDist(newSpace, oldSpace) return this;
    proc dsiCreateRankChangeDist(param newRank, args) return this;

    proc dsiEqualDMaps(d:DefaultDist) param return true;
    proc dsiEqualDMaps(d) param return false;

    proc trackDomains() param return false;
    proc dsiTrackDomains()    return false;

    proc singleton() param return true;
  }

  //
  // Replicated copies are set up in chpl_initOnLocales() during locale
  // model initialization
  //
  pragma "locale private"
  var defaultDist = new dmap(new DefaultDist());

  proc chpl_defaultDistInitPrivate() {
    if defaultDist._value==nil {
      // FIXME benharsh: Here's what we want to do:
      //   defaultDist = new dmap(new DefaultDist());
      // The problem is that the LHS of the "proc =" for _distributions
      // loses its ref intent in the removeWrapRecords pass.
      //
      // The code below is copied from the contents of the "proc =".
      const nd = new dmap(new DefaultDist());
      __primitive("move", defaultDist, chpl__autoCopy(nd.clone()));
    }
  }

  class DefaultRectangularDom: BaseRectangularDom {
    param rank : int;
    type idxType;
    param stridable: bool;
    var dist: DefaultDist;
    var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

    proc linksDistribution() param return false;
    proc dsiLinksDistribution()     return false;

    proc DefaultRectangularDom(param rank, type idxType, param stridable, dist) {
      this.dist = dist;
    }

    proc dsiMyDist() {
      return dist;
    }

    proc dsiDisplayRepresentation() {
      writeln("ranges = ", ranges);
    }

    proc dsiClear() {
      var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
      for param i in 1..rank do
        ranges(i) = emptyRange;
    }

    // function and iterator versions, also for setIndices
    proc dsiGetIndices() return ranges;

    proc dsiSetIndices(x) {
      ranges = x;
    }

    iter these_help(param d: int) {
      if d == rank {
        for i in ranges(d) do
          yield i;
      } else if d == rank - 1 {
        for i in ranges(d) do
          for j in these_help(rank) do
            yield (i, j);
      } else {
        for i in ranges(d) do
          for j in these_help(d+1) do
            yield (i, (...j));
      }
    }

    iter these_help(param d: int, block) {
      if d == block.size {
        for i in block(d) do
          yield i;
      } else if d == block.size - 1 {
        for i in block(d) do
          for j in these_help(block.size, block) do
            yield (i, j);
      } else {
        for i in block(d) do
          for j in these_help(d+1, block) do
            yield (i, (...j));
      }
    }

    iter these(tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, idxType, 0:idxType)) {
      if rank == 1 {
        for i in ranges(1) do
          yield i;
      } else {
        for i in these_help(1) do
          yield i;
      }
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, idxType, 0:idxType))
      where tag == iterKind.standalone && !localeModelHasSublocales {
      if chpl__testParFlag then
        chpl__testPar("default rectangular domain standalone invoked on ", ranges);
      if debugDefaultDist then
        chpl_debug_writeln("*** In domain standalone code:");

      const numTasks = if tasksPerLocale == 0 then here.maxTaskPar
                       else tasksPerLocale;
      if debugDefaultDist {
        chpl_debug_writeln("    numTasks=", numTasks, " (", ignoreRunning,
                "), minIndicesPerTask=", minIndicesPerTask);
      }
      const (numChunks, parDim) = if __primitive("task_get_serial") then
                                  (1, -1) else
                                  _computeChunkStuff(numTasks,
                                                     ignoreRunning,
                                                     minIndicesPerTask,
                                                     ranges);
      if debugDefaultDist {
        chpl_debug_writeln("    numChunks=", numChunks, " parDim=", parDim,
                " ranges(", parDim, ").length=", ranges(parDim).length);
      }

      if debugDataPar {
        chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n" +
                "### ignoreRunning = ", ignoreRunning, "\n" +
                "### minIndicesPerTask = ", minIndicesPerTask, "\n" +
                "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n" +
                "### nranges = ", ranges);
      }

      if numChunks <= 1 {
        for i in these_help(1) {
          yield i;
        }
      } else {
        var locBlock: rank*range(idxType);
        for param i in 1..rank {
          locBlock(i) = offset(i)..#(ranges(i).length);
        }
        if debugDefaultDist {
          chpl_debug_writeln("*** DI: locBlock = ", locBlock);
        }
        coforall chunk in 0..#numChunks {
          var followMe: rank*range(idxType) = locBlock;
          const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                        numChunks, chunk,
                                        locBlock(parDim).high,
                                        locBlock(parDim).low,
                                        locBlock(parDim).low);
          followMe(parDim) = lo..hi;
          if debugDefaultDist {
            chpl_debug_writeln("*** DI[", chunk, "]: followMe = ", followMe);
          }
          var block: rank*range(idxType=idxType, stridable=stridable);
          if stridable {
            type strType = chpl__signedType(idxType);
            for param i in 1..rank {
              // Note that a range.stride is signed, even if the range is not
              const rStride = ranges(i).stride;
              const rSignedStride = rStride:strType;
              if rStride > 0 {
                // Since stride is positive, the following line results
                // in a positive number, so casting it to e.g. uint is OK
                const riStride = rStride:idxType;
                const low = ranges(i).alignedLow + followMe(i).low*riStride,
                      high = ranges(i).alignedLow + followMe(i).high*riStride,
                      stride = rSignedStride;
                block(i) = low..high by stride;
              } else {
                // Stride is negative, so the following number is positive.
                const riStride = (-rStride):idxType;
                const low = ranges(i).alignedHigh - followMe(i).high*riStride,
                      high = ranges(i).alignedHigh - followMe(i).low*riStride,
                      stride = rSignedStride;
                block(i) = low..high by stride;
              }
            }
          } else {
            for  param i in 1..rank do
              block(i) = ranges(i).low+followMe(i).low:idxType..ranges(i).low+followMe(i).high:idxType;
          }
          for i in these_help(1, block) {
            yield i;
          }
        }
      }
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, idxType, 0:idxType))
      where tag == iterKind.leader {

      const numSublocs = here.getChildCount();

      if localeModelHasSublocales && numSublocs != 0 {

        const dptpl = if tasksPerLocale==0 then here.maxTaskPar
                      else tasksPerLocale;
        // Make sure we don't use more sublocales than the numbers of
        // tasksPerLocale requested
        const numSublocTasks = min(numSublocs, dptpl);
        // For serial tasks, we will only have a single chunk
        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, -1) else
                                    _computeChunkStuff(numSublocTasks,
                                                       ignoreRunning,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDataParNuma {
          chpl_debug_writeln("### numSublocs = ", numSublocs, "\n" +
                  "### numTasksPerSubloc = ", numSublocTasks, "\n" +
                  "### ignoreRunning = ", ignoreRunning, "\n" +
                  "### minIndicesPerTask = ", minIndicesPerTask, "\n" +
                  "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n" +
                  "### nranges = ", ranges);
        }

        if numChunks == 1 {
          if rank == 1 {
            yield (offset(1)..#ranges(1).length,);
          } else {
            var block: rank*range(idxType);
            for param i in 1..rank do
              block(i) = offset(i)..#ranges(i).length;
            yield block;
          }
        } else {
          coforall chunk in 0..#numChunks { // make sure coforall on can trigger
            local on here.getChild(chunk) {
              if debugDataParNuma {
                if chunk!=chpl_getSubloc() then
                  chpl_debug_writeln("*** ERROR: ON WRONG SUBLOC (should be "+chunk+
                          ", on "+chpl_getSubloc()+") ***");
              }
              // Divide the locale's tasks approximately evenly
              // among the sublocales
              const numSublocTasks = dptpl/numChunks +
                if chunk==numChunks-1 then dptpl%numChunks else 0;
              var locBlock: rank*range(idxType);
              for param i in 1..rank do
                locBlock(i) = offset(i)..#(ranges(i).length);
              var followMe: rank*range(idxType) = locBlock;
              const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                            numChunks, chunk,
                                            locBlock(parDim).high,
                                            locBlock(parDim).low,
                                            locBlock(parDim).low);
              followMe(parDim) = lo..hi;
              const (numChunks2, parDim2) = _computeChunkStuff(numSublocTasks,
                                                               ignoreRunning,
                                                               minIndicesPerTask,
                                                               followMe);
              coforall chunk2 in 0..#numChunks2 {
                var locBlock2: rank*range(idxType);
                for param i in 1..rank do
                  locBlock2(i) = followMe(i).low..followMe(i).high;
                var followMe2: rank*range(idxType) = locBlock2;
                const low  = locBlock2(parDim2).low,
                  high = locBlock2(parDim2).high;
                const (lo,hi) = _computeBlock(locBlock2(parDim2).length,
                                              numChunks2, chunk2,
                                              high, low, low);
                followMe2(parDim2) = lo..hi;
                if debugDataParNuma {
                  chpl_debug_writeln("### chunk = ", chunk, "  chunk2 = ", chunk2, "  " +
                          "followMe = ", followMe, "  followMe2 = ", followMe2);
                }
                yield followMe2;
              }
            }
          }
        }
      } else {

        if debugDefaultDist then
          chpl_debug_writeln("*** In domain/array leader code:"); // this = ", this);
        const numTasks = if tasksPerLocale==0 then here.maxTaskPar
                         else tasksPerLocale;

        if debugDefaultDist then
          chpl_debug_writeln("    numTasks=", numTasks, " (", ignoreRunning,
                  "), minIndicesPerTask=", minIndicesPerTask);

        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, -1) else
                                    _computeChunkStuff(numTasks,
                                                       ignoreRunning,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDefaultDist then
          chpl_debug_writeln("    numChunks=", numChunks, " parDim=", parDim,
                  " ranges(", parDim, ").length=", ranges(parDim).length);

        if debugDataPar {
          chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n" +
                  "### ignoreRunning = ", ignoreRunning, "\n" +
                  "### minIndicesPerTask = ", minIndicesPerTask, "\n" +
                  "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n" +
                  "### nranges = ", ranges);
        }

        if numChunks == 1 {
          if rank == 1 {
            yield (offset(1)..#ranges(1).length,);
          } else {
            var block: rank*range(idxType);
            for param i in 1..rank do
              block(i) = offset(i)..#ranges(i).length;
            yield block;
          }
        } else {
          var locBlock: rank*range(idxType);
          for param i in 1..rank do
            locBlock(i) = offset(i)..#(ranges(i).length);
          if debugDefaultDist then
            chpl_debug_writeln("*** DI: locBlock = ", locBlock);
          coforall chunk in 0..#numChunks {
            var followMe: rank*range(idxType) = locBlock;
            const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                          numChunks, chunk,
                                          locBlock(parDim).high,
                                          locBlock(parDim).low,
                                          locBlock(parDim).low);
            followMe(parDim) = lo..hi;
            if debugDefaultDist then
              chpl_debug_writeln("*** DI[", chunk, "]: followMe = ", followMe);
            yield followMe;
          }
        }
      }
    }

    iter these(param tag: iterKind, followThis,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, idxType, 0:idxType))
      where tag == iterKind.follower {

      proc anyStridable(rangeTuple, param i: int = 1) param
        return if i == rangeTuple.size then rangeTuple(i).stridable
               else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

      if chpl__testParFlag then
        chpl__testPar("default rectangular domain follower invoked on ", followThis);
      if debugDefaultDist then
        chpl_debug_writeln("In domain follower code: Following ", followThis);

      param stridable = this.stridable || anyStridable(followThis);
      var block: rank*range(idxType=idxType, stridable=stridable);
      if stridable {
        type strType = chpl__signedType(idxType);
        for param i in 1..rank {
          // See domain follower for comments about this
          const rStride = ranges(i).stride;
          const rSignedStride = rStride:strType,
                fSignedStride = followThis(i).stride:strType;
          if rStride > 0 {
            const riStride = rStride:idxType;
            const low = ranges(i).alignedLow + followThis(i).low*riStride,
                  high = ranges(i).alignedLow + followThis(i).high*riStride,
                  stride = (rSignedStride * fSignedStride):strType;
            block(i) = low..high by stride;
          } else {
            const irStride = (-rStride):idxType;
            const low = ranges(i).alignedHigh - followThis(i).high*irStride,
                  high = ranges(i).alignedHigh - followThis(i).low*irStride,
                  stride = (rSignedStride * fSignedStride):strType;
            block(i) = low..high by stride;
          }
        }
      } else {
        for  param i in 1..rank do
          block(i) = ranges(i).low+followThis(i).low:idxType..ranges(i).low+followThis(i).high:idxType;
      }

      if rank == 1 {
        for i in zip((...block)) {
          yield i;
        }
      } else {
        for i in these_help(1, block) {
          yield i;
        }
      }
    }

    proc dsiMember(ind: rank*idxType) {
      for param i in 1..rank do
        if !ranges(i).member(ind(i)) then
          return false;
      return true;
    }

    proc dsiIndexOrder(ind: rank*idxType) {
      var totOrder: idxType;
      var blk: idxType = 1;
      for param d in 1..rank by -1 {
        const orderD = ranges(d).indexOrder(ind(d));
        // NOTE: This follows from the implementation of indexOrder()
        if (orderD == (-1):idxType) then return orderD;
        totOrder += orderD * blk;
        blk *= ranges(d).length;
      }
      return totOrder;
    }

    proc dsiDims()
      return ranges;

    proc dsiDim(d : int)
      return ranges(d);

    // optional, is this necessary? probably not now that
    // homogeneous tuples are implemented as C vectors.
    proc dsiDim(param d : int)
      return ranges(d);

    proc dsiNumIndices {
      var sum = 1:idxType;
      for param i in 1..rank do
        sum *= ranges(i).length;
      return sum;
      // WANT: return * reduce (this(1..rank).length);
    }

    proc dsiLow {
      if rank == 1 {
        return ranges(1).low;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).low;
        return result;
      }
    }

    proc dsiHigh {
      if rank == 1 {
        return ranges(1).high;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).high;
        return result;
      }
    }

    proc dsiAlignedLow {
      if rank == 1 {
        return ranges(1).alignedLow;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).alignedLow;
        return result;
      }
    }

    proc dsiAlignedHigh {
      if rank == 1 {
        return ranges(1).alignedHigh;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).alignedHigh;
        return result;
      }
    }

    proc dsiStride {
      if rank == 1 {
        return ranges(1).stride;
      } else {
        var result: rank*chpl__signedType(idxType);
        for param i in 1..rank do
          result(i) = ranges(i).stride;
        return result;
      }
    }

    proc dsiAlignment {
      if rank == 1 {
        return ranges(1).alignment;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).alignment;
        return result;
      }
    }

    proc dsiFirst {
      if rank == 1 {
        return ranges(1).first;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).first;
        return result;
      }
    }

    proc dsiLast {
      if rank == 1 {
        return ranges(1).last;
      } else {
        var result: rank*idxType;
        for param i in 1..rank do
          result(i) = ranges(i).last;
        return result;
      }
    }

    proc dsiBuildArray(type eltType) {
      return new DefaultRectangularArr(eltType=eltType, rank=rank, idxType=idxType,
                                      stridable=stridable, dom=this);
    }

    proc dsiBuildRectangularDom(param rank: int, type idxType, param stridable: bool,
                              ranges: rank*range(idxType,
                                                 BoundedRangeType.bounded,
                                                 stridable)) {
      var dom = new DefaultRectangularDom(rank, idxType, stridable, dist);
      for i in 1..rank do
        dom.ranges(i) = ranges(i);
      return dom;
    }

    proc dsiLocalSlice(ranges) {
      halt("all dsiLocalSlice calls on DefaultRectangulars should be handled in ChapelArray.chpl");
    }
  }

  // TODO: should this include the ranges that represent the domain?
  record _remoteAccessData {
    type eltType;
    param rank : int;
    type idxType;
    var off: rank*idxType;
    var blk: rank*idxType;
    var str: rank*chpl__signedType(idxType);
    var origin: idxType;
    var factoredOffs: idxType;
    var data: _ddata(eltType);
    var shiftedData: _ddata(eltType);
  }

  //
  // Local cache of remote ddata access info
  //
  class LocRADCache {
    type eltType;
    param rank: int;
    type idxType;
    var targetLocDom: domain(rank);
    var RAD: [targetLocDom] _remoteAccessData(eltType, rank, idxType);

    proc LocRADCache(type eltType, param rank: int, type idxType,
                     newTargetLocDom: domain(rank)) {
      // This should resize the arrays
      targetLocDom=newTargetLocDom;
    }
  }

  class DefaultRectangularArr: BaseArr {
    type eltType;
    param rank : int;
    type idxType;
    param stridable: bool;

    var dom : DefaultRectangularDom(rank=rank, idxType=idxType,
                                           stridable=stridable);
    var off: rank*idxType;
    var blk: rank*idxType;
    var str: rank*chpl__signedType(idxType);
    var origin: idxType;
    var factoredOffs: idxType;
    pragma "local field"
    var data : _ddata(eltType);
    pragma "local field"
    var shiftedData : _ddata(eltType);
    var noinit_data: bool = false;

    // 'dataAllocRange' is used by the array-vector operations (e.g. push_back,
    // pop_back, insert, remove) to allow growing or shrinking the data
    // buffer in a doubling/halving style.  If it is used, it will be the
    // actual size of the 'data' buffer, while 'dom' represents the size of
    // the user-level array.
    var dataAllocRange: range(idxType);
    //var numelm: int = -1; // for correctness checking

    // end class definition here, then defined secondary methods below

    proc dsiDisplayRepresentation() {
      writeln("off=", off);
      writeln("blk=", blk);
      writeln("str=", str);
      writeln("origin=", origin);
      writeln("factoredOffs=", factoredOffs);
      writeln("noinit_data=", noinit_data);
    }

    // can the compiler create this automatically?
    proc dsiGetBaseDom() return dom;

    proc dsiDestroyArr(isalias:bool) {

      // data in an array alias will be destroyed when the original array
      // is destroyed.
      if isalias then
        return;

      if dom.dsiNumIndices > 0 {
        pragma "no copy" pragma "no auto destroy" var dr = data;
        pragma "no copy" pragma "no auto destroy" var dv = __primitive("deref", dr);
        pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, 0);
        pragma "no copy" pragma "no auto destroy" var ev = __primitive("deref", er);
        if (chpl__maybeAutoDestroyed(ev)) {
          var numElts = 0;
          // dataAllocRange may be empty or contain a meaningful value
          if rank == 1 && !stridable then
            numElts = dataAllocRange.length;
          if numElts == 0 then
            numElts = dom.dsiNumIndices;

          for i in 0..numElts-1 {
            pragma "no copy" pragma "no auto destroy" var dr = data;
            pragma "no copy" pragma "no auto destroy" var dv = __primitive("deref", dr);
            pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, i);
            pragma "no copy" pragma "no auto destroy" var ev = __primitive("deref", er);
            chpl__autoDestroy(ev);
          }
        }
      }
      _ddata_free(data);
    }

    inline proc theData ref {
      if earlyShiftData && !stridable then
        return shiftedData;
      else
        return data;
    }

    iter these(tasksPerLocale:int = dataParTasksPerLocale,
               ignoreRunning:bool = dataParIgnoreRunningTasks,
               minIndicesPerTask:int = dataParMinGranularity) ref {
      type strType = chpl__signedType(idxType);
      if rank == 1 {
        // This is specialized to avoid overheads of calling dsiAccess()
        if !dom.stridable {
          // Ideally we would like to be able to do something like
          // "for i in first..last by step". However, right now that would
          // result in a strided iterator which isn't as optimized. It would
          // also add a range constructor, which in tight loops is pretty
          // expensive. Instead we use a direct range iterator that is
          // optimized for positively strided ranges. It should be just as fast
          // as directly using a "c for loop", but it contains code check for
          // overflow and invalid strides as well as the ability to use a less
          // optimized iteration method if users are concerned about range
          // overflow.
          const first = getDataIndex(dom.dsiLow);
          const second = getDataIndex(dom.dsiLow+1);
          const step = (second-first);
          const last = first + (dom.dsiNumIndices-1) * step;
          for i in chpl_direct_pos_stride_range_iter(first, last, step) {
            yield theData(i);
          }

        } else {
          const stride = dom.ranges(1).stride: idxType,
                start  = dom.ranges(1).first,
                first  = getDataIndex(start),
                second = getDataIndex(start + stride),
                step   = (second-first):strType,
                last   = first + (dom.ranges(1).length-1) * step:idxType;
          if step > 0 then
            for i in first..last by step do
              yield data(i);
          else
            for i in last..first by step do
              yield data(i);
        }
      } else {
        for i in dom do
          yield dsiAccess(i);
      }
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      ref where tag == iterKind.standalone && !localeModelHasSublocales {
      if debugDefaultDist {
        chpl_debug_writeln("*** In array standalone code");
      }
      for i in dom.these(tag, tasksPerLocale,
                         ignoreRunning, minIndicesPerTask) {
        yield dsiAccess(i);
      }
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      where tag == iterKind.leader {
      for followThis in dom.these(tag,
                                  tasksPerLocale,
                                  ignoreRunning,
                                  minIndicesPerTask) do
        yield followThis;
    }

    iter these(param tag: iterKind, followThis,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      ref where tag == iterKind.follower {
      if debugDefaultDist then
        chpl_debug_writeln("*** In array follower code:"); // [\n", this, "]");
      for i in dom.these(tag=iterKind.follower, followThis,
                         tasksPerLocale,
                         ignoreRunning,
                         minIndicesPerTask) do
        yield dsiAccess(i);
    }

    proc computeFactoredOffs() {
      factoredOffs = 0:idxType;
      for param i in 1..rank do {
        factoredOffs = factoredOffs + blk(i) * off(i);
      }
    }

    inline proc initShiftedData() {
      if earlyShiftData && !stridable {
        // Lydia note 11/04/15: a question was raised as to whether this
        // check on dsiNumIndices added any value.  Performance results
        // from removing this line seemed inconclusive, which may indicate
        // that the check is not necessary, but it seemed like unnecessary
        // work for something with no immediate reward.
        if dom.dsiNumIndices > 0 {
          if isIntType(idxType) then
            shiftedData = _ddata_shift(eltType, data, origin-factoredOffs);
          else
            // Not bothering to check for over/underflow
            shiftedData = _ddata_shift(eltType, data,
                                       origin:chpl__signedType(idxType)-
                                       factoredOffs:chpl__signedType(idxType));
        }
      }
    }

    // change name to setup and call after constructor call sites
    // we want to get rid of all initialize functions everywhere
    proc initialize() {
      if noinit_data == true then return;
      for param dim in 1..rank {
        off(dim) = dom.dsiDim(dim).alignedLow;
        str(dim) = dom.dsiDim(dim).stride;
      }
      blk(rank) = 1:idxType;
      for param dim in 1..(rank-1) by -1 do
        blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
      computeFactoredOffs();
      var size = blk(1) * dom.dsiDim(1).length;
      data = _ddata_allocate(eltType, size);
      initShiftedData();
      if rank == 1 && !stridable then
        dataAllocRange = dom.dsiDim(1);

    }

    inline proc getDataIndex(ind: idxType ...1) where rank == 1
      return getDataIndex(ind);

    inline proc getDataIndex(ind: rank* idxType) {
      if stridable {
        var sum = origin;
        for param i in 1..rank do
          sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
        return sum;
      } else {
        // optimize common case to get cleaner generated code
        if (rank == 1 && earlyShiftData) {
          if __primitive("optimize_array_blk_mult") {
            return ind(1);
          } else {
            return ind(1) * blk(1);
          }
        } else {
          var sum = if earlyShiftData then 0:idxType else origin;

          // If we detect that blk is never changed then then blk(rank) == 1.
          // Knowing this, we need not multiply the final ind(...) by anything.
          // This relies on us marking every function that modifies blk
          if __primitive("optimize_array_blk_mult") {
            for param i in 1..rank-1 {
              sum += ind(i) * blk(i);
            }
            sum += ind(rank);
          } else {
            for param i in 1..rank {
              sum += ind(i) * blk(i);
            }
          }
          if !earlyShiftData then sum -= factoredOffs;
          return sum;
        }
      }
    }

    // These are cloned versions of the getDataIndex except without
    // shifting.  Currently, they are used only in the bulk transfer
    // optimization code.  I originally tried to add an optional param
    // argument to the above functions, but then some of the uses
    // lowerIterators had not be instantiated.
    inline proc getUnshiftedDataIndex(ind: idxType ...1) where rank == 1
      return getUnshiftedDataIndex(ind);

    inline proc getUnshiftedDataIndex(ind: rank* idxType) {
      var sum = origin;
      if stridable {
        for param i in 1..rank do
          sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
        return sum;
      } else {
        for param i in 1..rank do
          sum += ind(i) * blk(i);
        sum -= factoredOffs;
        return sum;
      }
    }

    // only need second version (ind : rank*idxType)
    // because wrapper record can pass a 1-tuple
    inline proc dsiAccess(ind: idxType ...1) ref
    where rank == 1
      return dsiAccess(ind);

    inline proc dsiAccess(ind: idxType ...1)
    where rank == 1 && !shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(ind);

    inline proc dsiAccess(ind: idxType ...1) const ref
    where rank == 1 && shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(ind);


    inline proc dsiAccess(ind : rank*idxType) ref {
      if boundsChecking then
        if !dom.dsiMember(ind) {
          // Note -- because of module load order dependency issues,
          // the multiple-arguments implementation of halt cannot
          // be called at this point. So we call a special routine
          // that does the right thing here.
          halt("array index out of bounds: " + _stringify_tuple(ind));
        }
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }

    inline proc dsiAccess(ind : rank*idxType)
    where !shouldReturnRvalueByConstRef(eltType) {
      if boundsChecking then
        if !dom.dsiMember(ind) {
          halt("array index out of bounds: " + _stringify_tuple(ind));
        }
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }

    inline proc dsiAccess(ind : rank*idxType) const ref
    where shouldReturnRvalueByConstRef(eltType) {
      if boundsChecking then
        if !dom.dsiMember(ind) {
          halt("array index out of bounds: " + _stringify_tuple(ind));
        }
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }


    inline proc dsiLocalAccess(i) ref
      return dsiAccess(i);

    inline proc dsiLocalAccess(i)
    where !shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(i);

    inline proc dsiLocalAccess(i) const ref
    where shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(i);

    proc dsiReindex(d: DefaultRectangularDom) {
      var alias : DefaultRectangularArr(eltType=eltType, rank=d.rank,
                                        idxType=d.idxType,
                                        stridable=d.stridable);
      on this {
      alias = new DefaultRectangularArr(eltType=eltType, rank=d.rank,
                                           idxType=d.idxType,
                                           stridable=d.stridable,
                                           dom=d, noinit_data=true,
                                           str=str,
                                           blk=blk);
      alias.data = data;
      //alias.numelm = numelm;
      //chpl_debug_writeln("DR.dsiReindex blk: ", blk, " stride: ",dom.dsiDim(1).stride," str:",str(1));
      adjustBlkOffStrForNewDomain(d, alias);
      alias.origin = origin:d.idxType;
      alias.computeFactoredOffs();
      alias.initShiftedData();
      }
      return alias;
    }

    pragma "modifies array blk"
    proc adjustBlkOffStrForNewDomain(d: DefaultRectangularDom,
                                     alias: DefaultRectangularArr)
    {
      for param i in 1..rank {
        var s: idxType;
        // NOTE: Not bothering to check to see if this can fit into idxType
        if chpl__signedType(idxType)==idxType {
          s = (dom.dsiDim(i).stride / str(i)) : d.idxType;
        } else { // unsigned type, signed stride
          assert((dom.dsiDim(i).stride<0 && str(i)<0) ||
                 (dom.dsiDim(i).stride>0 && str(i)>0));
          s = (dom.dsiDim(i).stride / str(i)) : d.idxType;
        }
        alias.off(i) = d.dsiDim(i).low;
        alias.blk(i) = blk(i) * s;
        alias.str(i) = d.dsiDim(i).stride;
      }
    }

    proc adjustBlkOffStrForNewDomain(d: DefaultRectangularDom,
                                     alias: DefaultRectangularArr)
      where dom.stridable == false && this.stridable == false
    {
      for param i in 1..rank {
        alias.off(i) = d.dsiDim(i).low;
        alias.blk(i) = blk(i);
        alias.str(i) = d.dsiDim(i).stride;
      }
    }


    proc dsiSlice(d: DefaultRectangularDom) {
      var alias : DefaultRectangularArr(eltType=eltType, rank=rank,
                                        idxType=idxType,
                                        stridable=d.stridable);
      on this {
        alias = new DefaultRectangularArr(eltType=eltType, rank=rank,
                                             idxType=idxType,
                                             stridable=d.stridable,
                                             dom=d, noinit_data=true);
        alias.data = data;
        //alias.numelm = numelm;
        alias.blk = blk;
        alias.str = str;
        alias.origin = origin;
        for param i in 1..rank {
          alias.off(i) = d.dsiDim(i).low;
          // NOTE: Not bothering to check to see if the abs(..) expression
          //  can fit into idxType
          if str(i) > 0 {
            alias.origin += blk(i) * (d.dsiDim(i).low - off(i)) / str(i):idxType;
          } else {
            alias.origin -= blk(i) * (d.dsiDim(i).low - off(i)) / abs(str(i)):idxType;
          }
        }
        alias.computeFactoredOffs();
        alias.initShiftedData();

        // it won't work with this.adjustBlkOffStrForNewDomain(d, alias)
        alias.adjustBlkOffStrForNewDomain(d, alias);
      }
      return alias;
    }

    pragma "modifies array blk"
    proc dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
      var alias : DefaultRectangularArr(eltType=eltType, rank=newRank,
                                        idxType=idxType,
                                        stridable=newStridable);
      on this {
      alias = new DefaultRectangularArr(eltType=eltType, rank=newRank,
                                           idxType=idxType,
                                           stridable=newStridable,
                                           dom=d, noinit_data=true);
      alias.data = data;
      //alias.numelm = numelm;
      var i = 1;
      alias.origin = origin;
      for param j in 1..args.size {
        if isRange(args(j)) {
          alias.off(i) = d.dsiDim(i).low;
          alias.origin += blk(j) * (d.dsiDim(i).low - off(j)) / str(j);
          alias.blk(i) = blk(j);
          alias.str(i) = str(j);
          i += 1;
        } else {
          alias.origin += blk(j) * (args(j) - off(j)) / str(j);
        }
      }
      alias.computeFactoredOffs();
      alias.initShiftedData();
      }
      return alias;
    }

    proc dsiReallocate(d: domain) {
      if (d._value.type == dom.type) {
        on this {
        var copy = new DefaultRectangularArr(eltType=eltType, rank=rank,
                                            idxType=idxType,
                                            stridable=d._value.stridable,
                                            dom=d._value);
        for i in d((...dom.ranges)) do
          copy.dsiAccess(i) = dsiAccess(i);
        off = copy.off;
        blk = copy.blk;
        str = copy.str;
        origin = copy.origin;
        factoredOffs = copy.factoredOffs;
        dsiDestroyArr(false);
        data = copy.data;
        // We can't call initShiftedData here because the new domain
        // has not yet been updated (this is called from within the
        // = function for domains.
        if earlyShiftData && !d._value.stridable then
          // Lydia note 11/04/15: a question was raised as to whether this
          // check on numIndices added any value.  Performance results
          // from removing this line seemed inconclusive, which may indicate
          // that the check is not necessary, but it seemed like unnecessary
          // work for something with no immediate reward.
          if d.numIndices > 0 then
            shiftedData = copy.shiftedData;
        //numelm = copy.numelm;
        delete copy;
        }
      } else {
        halt("illegal reallocation");
      }
    }

    proc dsiLocalSlice(ranges) {
      halt("all dsiLocalSlice calls on DefaultRectangulars should be handled in ChapelArray.chpl");
    }

    proc dsiGetRAD() {
      var rad: _remoteAccessData(eltType, rank, idxType);
      rad.off = off;
      rad.blk = blk;
      rad.str = str;
      rad.origin = origin;
      rad.factoredOffs = factoredOffs;
      rad.data = data;
      if earlyShiftData && !stridable then
        // Lydia note 11/04/15: a question was raised as to whether this
        // check on dsiNumIndices added any value.  Performance results
        // from removing this check seemed inconclusive, which may indicate
        // that the check is not necessary, but it seemed like unnecessary
        // work for something with no immediate reward.
        if dom.dsiNumIndices > 0 then rad.shiftedData = shiftedData;
      return rad;
    }

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by default domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      return _newDomain(dom);
    }
  }

  proc DefaultRectangularDom.dsiSerialReadWrite(f /*: Reader or Writer*/) {
    f <~> new ioLiteral("{") <~> ranges(1);
    for i in 2..rank do
      f <~> new ioLiteral(", ") <~> ranges(i);
    f <~> new ioLiteral("}");
  }

  proc DefaultRectangularDom.dsiSerialWrite(f) { this.dsiSerialReadWrite(f); }
  proc DefaultRectangularDom.dsiSerialRead(f) { this.dsiSerialReadWrite(f); }

  proc DefaultRectangularArr.dsiSerialReadWrite(f /*: Reader or Writer*/) {
    proc writeSpaces(dim:int) {
      for i in 1..dim {
        f <~> new ioLiteral(" ");
      }
    }

    proc recursiveArrayWriter(in idx: rank*idxType, dim=1, in last=false) {
      var binary = f.binary();
      var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
      var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

      type strType = chpl__signedType(idxType);
      var makeStridePositive = if dom.ranges(dim).stride > 0 then 1:strType else (-1):strType;

      if isjson || ischpl {
        if dim != rank {
          f <~> new ioLiteral("[\n");
          writeSpaces(dim); // space for the next dimension
        } else f <~> new ioLiteral("[");
      }

      if dim == rank {
        var first = true;
        if debugDefaultDist && f.writing then f.writeln(dom.ranges(dim));
        for j in dom.ranges(dim) by makeStridePositive {
          if first then first = false;
          else if isspace then f <~> new ioLiteral(" ");
          else if isjson || ischpl then f <~> new ioLiteral(", ");
          idx(dim) = j;
          f <~> dsiAccess(idx);
        }
      } else {
        for j in dom.ranges(dim) by makeStridePositive {
          var lastIdx =  dom.ranges(dim).last;
          idx(dim) = j;

          recursiveArrayWriter(idx, dim=dim+1,
                               last=(last || dim == 1) && (j == dom.ranges(dim).alignedHigh));

          if isjson || ischpl {
            if j != lastIdx {
              f <~> new ioLiteral(",\n");
              writeSpaces(dim);
            }
          }
        }
      }

      if isspace {
        if !last && dim != 1 {
          f <~> new ioLiteral("\n");
        }
      } else if isjson || ischpl {
        if dim != rank {
          f <~> new ioLiteral("\n");
          writeSpaces(dim-1); // space for this dimension
          f <~> new ioLiteral("]");
        }
        else f <~> new ioLiteral("]");
      }

    }

    if false && !f.writing && !f.binary() &&
       rank == 1 && dom.ranges(1).stride == 1 &&
       dom._arrs.length == 1 {

      // resize-on-read implementation, disabled right now
      // until we decide how it should work.

      // Binary reads could also start out with a length.

      // Special handling for reading 1-D stride-1 arrays in order
      // to read them without requiring that the array length be
      // specified ahead of time.

      var binary = f.binary();
      var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
      var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

      if isjson || ischpl {
        f <~> new ioLiteral("[");
      }

      var first = true;

      var offset = dom.ranges(1).low;
      var i = 0;

      var read_end = false;

      while ! f.error() {
        if first {
          first = false;
          // but check for a ]
          if isjson || ischpl {
            f <~> new ioLiteral("]");
          } else if isspace {
            f <~> new ioNewline(skipWhitespaceOnly=true);
          }
          if f.error() == EFORMAT {
            f.clearError();
          } else {
            read_end = true;
            break;
          }
        } else {
          // read a comma or a space.
          if isspace then f <~> new ioLiteral(" ");
          else if isjson || ischpl then f <~> new ioLiteral(",");

          if f.error() == EFORMAT {
            f.clearError();
            // No comma.
            break;
          }
        }

        if i >= dom.ranges(1).size {
          // Create more space.
          var sz = dom.ranges(1).size;
          if sz < 4 then sz = 4;
          sz = 2 * sz;

          // like push_back
          const newDom = {offset..#sz};

          dsiReallocate( newDom );
          // This is different from how push_back does it
          // because push_back might lead to a call to
          // _reprivatize but I don't see how to do that here.
          dom.dsiSetIndices( newDom.getIndices() );
          dsiPostReallocate();
        }

        f <~> dsiAccess(offset + i);

        i += 1;
      }

      if ! read_end {
        if isjson || ischpl {
          f <~> new ioLiteral("]");
        }
      }

      {
        // trim down to actual size read.
        const newDom = {offset..#i};
        dsiReallocate( newDom );
        dom.dsiSetIndices( newDom.getIndices() );
        dsiPostReallocate();
      }

    } else {
      const zeroTup: rank*idxType;
      recursiveArrayWriter(zeroTup);
    }
  }

  proc DefaultRectangularArr.dsiSerialWrite(f) {
    var isNative = f.styleElement(QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER): bool;

    if _isSimpleIoType(this.eltType) && f.binary() &&
       isNative && this.isDataContiguous() {
      // If we can, we would like to write the array out as a single write op
      // since _ddata is just a pointer to the memory location we just pass
      // that along with the size of the array. This is only possible when the
      // byte order is set to native or it's equivalent.
      pragma "no prototype"
      extern proc sizeof(type x): size_t;
      const elemSize = sizeof(eltType);
      const len = dom.dsiNumIndices;
      if boundsChecking then
        assert((len:uint*elemSize:uint) <= max(ssize_t):uint,
               "length of array to write is greater than ssize_t can hold");
      const src = this.theData;
      const idx = getDataIndex(this.dom.dsiLow);
      f.writeBytes(_ddata_shift(eltType, src, idx), len:ssize_t*elemSize:ssize_t);
    } else {
      this.dsiSerialReadWrite(f);
    }
  }

  proc DefaultRectangularArr.dsiSerialRead(f) {
    var isNative = f.styleElement(QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER): bool;

    if _isSimpleIoType(this.eltType) && f.binary() &&
       isNative && this.isDataContiguous() {
      // read the data in one op if possible, same comments as above apply
      pragma "no prototype"
      extern proc sizeof(type x): size_t;
      const elemSize = sizeof(eltType);
      const len = dom.dsiNumIndices;
      if boundsChecking then
        assert((len:uint*elemSize:uint) <= max(ssize_t):uint,
               "length of array to read is greater than ssize_t can hold");
      f.readBytes(data, len:ssize_t*elemSize:ssize_t);
    } else {
      this.dsiSerialReadWrite(f);
    }
  }

  // This is very conservative.
  proc DefaultRectangularArr.isDataContiguous() {
    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("isDataContiguous(): origin=", origin, " off=", off, " blk=", blk);

    for param dim in 1..rank do
      if off(dim)!= dom.dsiDim(dim).first then return false;

    if blk(rank) != 1 then return false;

    for param dim in 1..(rank-1) by -1 do
      if blk(dim) != blk(dim+1)*dom.dsiDim(dim+1).length then return false;

    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("\tYES!");

    return true;
  }

  proc DefaultRectangularArr.dsiSupportsBulkTransfer() param return true;
  proc DefaultRectangularArr.dsiSupportsBulkTransferInterface() param return true;

  proc DefaultRectangularArr.doiCanBulkTransfer() {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiCanBulkTransfer()");
    if dom.stridable then
      for param i in 1..rank do
        if dom.ranges(i).stride != 1 then return false;
    if !isDataContiguous(){
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("isDataContiguous return False");
      return false;
    }
    return true;
  }

  proc DefaultRectangularArr.doiCanBulkTransferStride() param {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiCanBulkTransferStride()");
    // A DefaultRectangular array is always regular, so bulk should be possible.
    return true;
  }

  proc DefaultRectangularArr.doiBulkTransfer(B) {
    const Adims = dom.dsiDims();
    var Alo: rank*dom.idxType;
    for param i in 1..rank do
      Alo(i) = Adims(i).first;

    const Bdims = B.domain.dims();
    var Blo: rank*dom.idxType;
    for param i in 1..rank do
      Blo(i) = Bdims(i).first;

    const len = dom.dsiNumIndices.safeCast(size_t);

    if len == 0 then return;

    if debugBulkTransfer {
      pragma "no prototype"
      extern proc sizeof(type x): int;
      const elemSize =sizeof(B._value.eltType);
      chpl_debug_writeln("In DefaultRectangularArr.doiBulkTransfer():",
              " Alo=", Alo, ", Blo=", Blo,
              ", len=", len, ", elemSize=", elemSize);
    }

    const Adata = _ddata_shift(eltType, this.theData, getDataIndex(Alo));
    const Bdata = _ddata_shift(eltType, B._value.theData, B._value.getDataIndex(Blo));

    if Adata == Bdata then return;

    // NOTE: This does not work with --heterogeneous, but heterogeneous
    // compilation does not work right now.  The calls to chpl_comm_get
    // and chpl_comm_put should be changed once that is fixed.
    if Adata.locale.id==here.id {
      if debugDefaultDistBulkTransfer then //See bug in test/optimizations/bulkcomm/alberto/rafatest2.chpl
        chpl_debug_writeln("\tlocal get() from ", B._value.locale.id);
      __primitive("chpl_comm_array_get", Adata[0], Bdata.locale.id, Bdata[0], len);
    } else if Bdata.locale.id==here.id {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("\tlocal put() to ", this.locale.id);
      __primitive("chpl_comm_array_put", Bdata[0], Adata.locale.id, Adata[0], len);
    } else on Adata.locale {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("\tremote get() on ", here.id, " from ", B.locale.id);
      __primitive("chpl_comm_array_get", Adata[0], Bdata.locale.id, Bdata[0], len);
    }
  }

  /*
  For more details, see: http://upc.lbl.gov/publications/upc_memcpy.pdf
    'Proposal for Extending the UPC Memory Copy Library Functions and
    Supporting Extensions to GASNet, Version 2.0. Author: Dan Bonachea'

  A.doiBulkTransferStride(B) copies B-->A, where 'B' is another
  DefaultRectangular array

  Assumes row-major ordering.

  Depends on adjustBlkOffStrForNewDomain having been called in ChapelArray
  before entering this function.

  TODO: Determine if we can reduce the communication for array metadata

  TODO: Pull simple runtime implementation up into module code
  */
  proc DefaultRectangularArr.doiBulkTransferStride(Barg) {
    if this.data.locale != here && Barg.data.locale != here {
      if debugDefaultDistBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: Both arrays on different locale, moving to locale of destination: LOCALE", this.data.locale.id);
      }
      on this.data do stridedTransferFrom(Barg);
    } else {
      stridedTransferFrom(Barg);
    }
  }

  proc DefaultRectangularArr.stridedTransferFrom(Barg) {
    const A = this, B = Barg;

    // Using the 'first' values is necessary for correctness when the slice is
    // align'd.
    const Adims = A.dom.dsiDims();
    var AFirst : rank*idxType;
    for i in 1..rank do AFirst(i) = if Adims(i).stride < 0 then Adims(i).last else Adims(i).first;

    const Bdims = B.dom.dsiDims();
    var BFirst : rank*idxType;
    for i in 1..rank do BFirst(i) = if Bdims(i).stride < 0 then Bdims(i).last else Bdims(i).first;

    if debugDefaultDistBulkTransfer {
      chpl_debug_writeln("In DefaultRectangularArr.doiBulkTransferStride\n" +
                         "Dest = " + stringify(Adims) + "\n" +
                         "Src  = " + stringify(Bdims));
    }

    // The number of values needed to express the strided region.
    var stridelevels = 0;

    //
    // `count[1]` is the number of elements we can bulk-copy at a time. Each
    // number after that represents the number of times we need to stride at
    // each level. It will ultimately be an array of size `stridelevels+1`.
    //
    var countDom = {1..rank+1};
    var count : [countDom] size_t;
    for c in count do c = 1; // serial to avoid task creation overhead

    //
    // The arrays representing the required stride at each level. Should pull
    // from the values in the 'blk' tuple, though we may skip a dimension if
    // it can be aggregated. Will ultimately be of size `stridelevels`.
    //
    var strideDom = {1..rank};
    var dstStride, srcStride : [strideDom] size_t;

    //
    // If the last dimension is strided then we can only copy one element at a
    // time. `blk(rank)` may be greater than `1` depending on the results of
    // `adjustBlkOffStrForNewDomain` or if there was a rank change.
    //
    // It is incorrect to check the stride of the ranges here because the
    // original domain may have also been strided, like so:
    //   var A : [1..10 by 2, 1..20 by 4] int;
    //
    if A.blk(rank) > 1 || B.blk(rank) > 1 {
      stridelevels += 1;
      count[stridelevels] = 1;
      dstStride[stridelevels] = A.blk(rank).safeCast(size_t);
      srcStride[stridelevels] = B.blk(rank).safeCast(size_t);
    }

    //
    // When at the end of the elements in dimension `i`, `canReuseStride` tells
    // us whether or not we can re-use the current stride value to advance to
    // the next chunk of elements. If either array is unable to re-use its
    // stride, then we need a new stride value.
    //
    for i in 2..rank by -1 {
      // Each corresponding dimension in A and B should have the same length,
      // so it doesn't matter which we use here.
      count[stridelevels+1] *= Adims(i).length.safeCast(size_t);

      const bothReuse = A.canReuseStride(i, stridelevels, count, dstStride)
                     && B.canReuseStride(i, stridelevels, count, srcStride);

      if !bothReuse {
        stridelevels += 1;
        dstStride[stridelevels] = A.blk(i-1).safeCast(size_t);
        srcStride[stridelevels] = B.blk(i-1).safeCast(size_t);
      }
    }
    count[stridelevels+1] *= Adims(1).length.safeCast(size_t);

    assert(stridelevels <= rank, "BulkTransferStride: stride levels greater than rank.");
    if stridelevels == 0 then assert(count[1] == A.dom.dsiNumIndices, "BulkTransferStride: bulk-count incorrect for stride level of 0.");

    countDom = {1..stridelevels+1};
    strideDom = {1..stridelevels};

    doiBulkTransferStrideComm(Barg, stridelevels:int(32), dstStride, srcStride, count, AFirst, BFirst);
  }

  //
  // Invoke the primitives chpl_comm_get_strd/puts, depending on what locale we
  // are on vs. where the source and destination are.
  // The logic mimics that in doiBulkTransfer().
  //
  proc DefaultRectangularArr.doiBulkTransferStrideComm(B, stridelevels:int(32), dstStride, srcStride, count, AFirst, BFirst) {
    if debugDefaultDistBulkTransfer {
      chpl_debug_writeln("BulkTransferStride with values:\n" +
                         "\tLocale        = " + stringify(here.id) + "\n" +
                         "\tStride levels = " + stringify(stridelevels) + "\n" +
                         "\tdstStride     = " + stringify(dstStride) + "\n" +
                         "\tsrcStride     = " + stringify(srcStride) + "\n" +
                         "\tcount         = " + stringify(count) + "\n" +
                         "\tdstBlk        = " + stringify(blk) + "\n" +
                         "\tsrcBlk        = " + stringify(B.blk));
    }

    const A = this;
    const AO = A.getUnshiftedDataIndex(AFirst);
    const BO = B.getUnshiftedDataIndex(BFirst);

    const dest = A.data;
    const src  = B.data;

    const dststr = dstStride._value.data;
    const srcstr = srcStride._value.data;
    const cnt    = count._value.data;
    if dest.locale == here {
      var srclocale = src.locale.id : int(32);

      if debugBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: On LHS - GET from ", srclocale);
      }

      __primitive("chpl_comm_get_strd",
                  dest[AO],
                  dststr[0],
                  srclocale,
                  src[BO],
                  srcstr[0],
                  cnt[0],
                  stridelevels);
    }
    else {
      const destlocale = dest.locale.id : int(32);

      if debugDefaultDistBulkTransfer {
        assert(src.locale == here, "BulkTransferStride: Expected to be on ", src.locale, ", actually on ", here);
      }

      if debugBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: On RHS - PUT to ", destlocale);
      }

      __primitive("chpl_comm_put_strd",
                  dest[AO],
                  dststr[0],
                  destlocale,
                  src[BO],
                  srcstr[0],
                  cnt[0],
                  stridelevels);
    }
  }

  proc DefaultRectangularArr.isDefaultRectangular() param return true;

  /*
  The runtime implementation's loop over the current stride level will look
  something like this:

  ```
  for (i = 0; i < count[levels+1]; i++) {
    src = baseSrc + srcStride[levels] * i;
    dst = baseDst + dstStride[levels] * i;
    // then dimensions curDim+1..rank
  }
  ```
  
  The last value of `src` will be:
    baseSrc + (count[levels+1] - 1) * srcStride[levels];

  To correctly advance to the next chunk of elements, we could simply add a
  stride level and increment `baseSrc` by `blk(curDim-1)`. While correct, this
  would not be the optimal choice if we want to bulk-copy the largest possible
  chunk at a time.

  To do so, we need to determine whether or not we can re-use the current
  stride to advance to the next chunk of elements.

  When the stridelevel is less than 1, we're still calculating the bulk-copy
  size. In this case, we let the stride value be `1` in order to test that the
  next chunk is contiguous in memory.

  When the stridelevel is greater than 1 we're done calculating the bulk-copy
  size. However, it's still convenient to try and re-use the stride if
  possible.
  */
  proc DefaultRectangularArr.canReuseStride(curDim: int, levels, count, stride)
  {
    // TODO: Do we need to return false if the previous dimension has only one
    // element? What if it only has one element in the original domain?
    //
    // So far it seems to 'just work'.

    const lastCount = count[levels+1];
    const curStride = if levels < 1 then 1 else stride[levels];
    
    // Subtract `1` because we don't need a stride for the first element in
    // the current stride level. We could simplify the math here, but I think
    // this is clearer.
    const lastIndex = (lastCount-1) * curStride;
    const nextIndex = lastIndex + curStride;

    return blk(curDim-1) == nextIndex;
  }

  //
  // bulkConvertCoordinate() converts
  //   point 'b' within 'Barr.domain'
  // to
  //   point within 'Aarr.domain'
  // that has the same indexOrder in each dimension.
  //
  // This function was contributed by Juan Lopez and later improved by Alberto.
  // In the SBAC'12 paper it is called m().
  //
  proc bulkCommConvertCoordinate(bArg, Barr, Aarr)
  {
    compilerAssert(Aarr.rank == Barr.rank);
    const b = chpl__tuplify(bArg);
    param rank = Aarr.rank;
    type idxType = Aarr.idxType;
    const AD = Aarr.dom.dsiDims();
    const BD = Barr.dom.dsiDims();
    var result: rank * idxType;
    for param i in 1..rank {
      const ar = AD(i), br = BD(i);
      if boundsChecking then assert(br.member(b(i)));
      result(i) = ar.orderToIndex(br.indexOrder(b(i)));
    }
    return result;
  }

}
