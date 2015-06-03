/*
 * Copyright 2004-2015 Cray Inc.
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
  config param assertNoSlicing = false;
  
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
  }
  
  //
  // Replicated copies are set up in chpl_initOnLocales() during locale
  // model initialization
  //
  pragma "locale private" var defaultDist = new dmap(new DefaultDist());
  inline proc chpl_defaultDistInitPrivate() {
    if defaultDist._value==nil then defaultDist = new dmap(new DefaultDist());
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
      if ranges.size != x.size then
        compilerError("rank mismatch in domain assignment");
      if ranges(1).idxType != x(1).idxType then
        compilerError("index type mismatch in domain assignment");
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
      if debugDefaultDist {
        writeln("*** In domain standalone code:");
      }
      const numTasks = if tasksPerLocale == 0 then here.maxTaskPar
                       else tasksPerLocale;
      if debugDefaultDist {
        writeln("    numTasks=", numTasks, " (", ignoreRunning,
                "), minIndicesPerTask=", minIndicesPerTask);
      }
      const (numChunks, parDim) = if __primitive("task_get_serial") then
                                  (1, -1) else
                                  _computeChunkStuff(numTasks,
                                                     ignoreRunning,
                                                     minIndicesPerTask,
                                                     ranges);
      if debugDefaultDist {
        writeln("    numChunks=", numChunks, " parDim=", parDim,
                " ranges(", parDim, ").length=", ranges(parDim).length);
      }

      if debugDataPar {
        writeln("### numTasksPerLoc = ", numTasks, "\n" +
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
          writeln("*** DI: locBlock = ", locBlock);
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
            writeln("*** DI[", chunk, "]: followMe = ", followMe);
          }
          var block: rank*range(idxType=idxType, stridable=stridable);
          if stridable {
            type strType = chpl__signedType(idxType);
            for param i in 1..rank {
              const rStride = ranges(i).stride:strType;
              if ranges(i).stride > 0 {
                const low = ranges(i).alignedLow + followMe(i).low*rStride,
                      high = ranges(i).alignedLow + followMe(i).high*rStride,
                      stride = rStride:idxType;
                block(i) = low..high by stride;
              } else {
                const low = ranges(i).alignedHigh + followMe(i).high*rStride,
                      high = ranges(i).alignedHigh + followMe(i).low*rStride,
                      stride = rStride:idxType;
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
        // For serial tasks, we will only have a singel chunk
        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, -1) else
                                    _computeChunkStuff(numSublocTasks,
                                                       ignoreRunning,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDataParNuma {
          writeln("### numSublocs = ", numSublocs, "\n" +
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
            on here.getChild(chunk) {
              if debugDataParNuma {
                if chunk!=chpl_getSubloc() then
                  writeln("*** ERROR: ON WRONG SUBLOC (should be "+chunk+
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
                  writeln("### chunk = ", chunk, "  chunk2 = ", chunk2, "  " +
                          "followMe = ", followMe, "  followMe2 = ", followMe2);
                }
                yield followMe2;
              }
            }
          }
        }
      } else {

        if debugDefaultDist then
          writeln("*** In domain/array leader code:"); // this = ", this);
        const numTasks = if tasksPerLocale==0 then here.maxTaskPar
                         else tasksPerLocale;
  
        if debugDefaultDist then
          writeln("    numTasks=", numTasks, " (", ignoreRunning,
                  "), minIndicesPerTask=", minIndicesPerTask);

        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, -1) else
                                    _computeChunkStuff(numTasks,
                                                       ignoreRunning,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDefaultDist then
          writeln("    numChunks=", numChunks, " parDim=", parDim,
                  " ranges(", parDim, ").length=", ranges(parDim).length);
  
        if debugDataPar { 
          writeln("### numTasksPerLoc = ", numTasks, "\n" +
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
            writeln("*** DI: locBlock = ", locBlock);
          coforall chunk in 0..#numChunks {
            var followMe: rank*range(idxType) = locBlock;
            const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                          numChunks, chunk,
                                          locBlock(parDim).high,
                                          locBlock(parDim).low,
                                          locBlock(parDim).low);
            followMe(parDim) = lo..hi;
            if debugDefaultDist then
              writeln("*** DI[", chunk, "]: followMe = ", followMe);
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
        writeln("In domain follower code: Following ", followThis);
      param stridable = this.stridable || anyStridable(followThis);
      var block: rank*range(idxType=idxType, stridable=stridable);
      if stridable {
        type strType = chpl__signedType(idxType);
        for param i in 1..rank {
          const rStride = ranges(i).stride:strType,
                fStride = followThis(i).stride:strType;
          if ranges(i).stride > 0 {
            const low = ranges(i).alignedLow + followThis(i).low*rStride,
                  high = ranges(i).alignedLow + followThis(i).high*rStride,
                  stride = (rStride * fStride):idxType;
            block(i) = low..high by stride;
          } else {
            const low = ranges(i).alignedHigh + followThis(i).high*rStride,
                  high = ranges(i).alignedHigh + followThis(i).low*rStride,
                  stride = (rStride * fStride):idxType;
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
  
    // optional, is this necesary? probably not now that
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
  
    proc dsiDestroyData() {
      if dom.dsiNumIndices > 0 {
        pragma "no copy" pragma "no auto destroy" var dr = data;
        pragma "no copy" pragma "no auto destroy" var dv = __primitive("deref", dr);
        pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, 0);
        pragma "no copy" pragma "no auto destroy" var ev = __primitive("deref", er);
        if (chpl__maybeAutoDestroyed(ev)) {
          for i in 0..dom.dsiNumIndices-1 {
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
          // results in a strided iterator which isn't as optimized. It also
          // introduces another range creation which in tight loops is
          // unfortunately expensive. Ideally we don't want to be using C for
          // loops outside of ChapelRange. However, since most other array data
          // types are implemented in terms of DefaultRectangular, we think
          // that this will serve as a second base case rather than the
          // beginning of every iterator invoking a primitive C for loop
          var i: idxType;
          const first = getDataIndex(dom.dsiLow);
          const second = getDataIndex(dom.dsiLow+1);
          const step = (second-first);
          const last = first + (dom.dsiNumIndices-1) * step;
          while __primitive("C for loop",
                            __primitive( "=", i, first),
                            __primitive("<=", i, last),
                            __primitive("+=", i, step)) {
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
        writeln("*** In array standalone code");
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
        writeln("*** In array follower code:"); // [\n", this, "]");
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
        var sum = if earlyShiftData then 0:idxType else origin;

        // If the user asserts that there is no slicing in their program,
        // then blk(rank) == 1. Knowing this, we need not multiply the final
        // ind(...) by anything. This may lead to performance improvements for
        // array accesses.
        if assertNoSlicing {
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
  
    // only need second version because wrapper record can pass a 1-tuple
    inline proc dsiAccess(ind: idxType ...1) ref where rank == 1
      return dsiAccess(ind);
  
    inline proc dsiAccess(ind : rank*idxType) ref {
      if boundsChecking then
        if !dom.dsiMember(ind) then
          halt("array index out of bounds: ", ind);
      var dataInd = getDataIndex(ind);
      //assert(dataInd >= 0);
      //assert(numelm >= 0); // ensure it has been initialized
      //assert(dataInd: uint(64) < numelm: uint(64));
      return theData(dataInd);
    }
  
    inline proc dsiLocalAccess(i) ref {
      return dsiAccess(i);
    }
  
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
      //writeln("DR.dsiReindex blk: ", blk, " stride: ",dom.dsiDim(1).stride," str:",str(1));
      adjustBlkOffStrForNewDomain(d, alias);
      alias.origin = origin:d.idxType;
      alias.computeFactoredOffs();
      alias.initShiftedData();
      }
      return alias;
    }
    
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
          s = dom.dsiDim(i).stride / str(i) : d.idxType;
        }
        alias.off(i) = d.dsiDim(i).low;
        alias.blk(i) = blk(i) * s;
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
      }
      return alias;
    }
  
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
        dsiDestroyData();
        data = copy.data;
        // We can't call initShiftedData here because the new domain
        // has not yet been updated (this is called from within the
        // = function for domains.
        if earlyShiftData && !d._value.stridable then
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
  
  proc DefaultRectangularDom.dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
  proc DefaultRectangularDom.dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }

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
                               last=(last || dim == 1) && (j == lastIdx));

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
    const zeroTup: rank*idxType;
    recursiveArrayWriter(zeroTup);
  }

  proc DefaultRectangularArr.dsiSerialWrite(f: Writer) {
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
      f.writeBytes(data, len:ssize_t*elemSize:ssize_t);
    } else {
      this.dsiSerialReadWrite(f);
    }
  }

  proc DefaultRectangularArr.dsiSerialRead(f: Reader) {
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

  // This is very conservative.  For example, it will return false for
  // 1-d array aliases that are shifted from the aliased array.
  proc DefaultRectangularArr.isDataContiguous() {
    if debugDefaultDistBulkTransfer then
      writeln("isDataContiguous(): origin=", origin, " off=", off, " blk=", blk);
    if origin != 0 then return false;
  
    for param dim in 1..rank do
      if off(dim)!= dom.dsiDim(dim).first then return false;
  
    if blk(rank) != 1 then return false;
  
    for param dim in 1..(rank-1) by -1 do
      if blk(dim) != blk(dim+1)*dom.dsiDim(dim+1).length then return false;
  
    if debugDefaultDistBulkTransfer then
      writeln("\tYES!");
  
    return true;
  }
  
  proc DefaultRectangularArr.dsiSupportsBulkTransfer() param return true;
  proc DefaultRectangularArr.dsiSupportsBulkTransferInterface() param return true;
  
  proc DefaultRectangularArr.doiCanBulkTransfer() {
    if debugDefaultDistBulkTransfer then writeln("In DefaultRectangularArr.doiCanBulkTransfer()");
    if dom.stridable then
      for param i in 1..rank do
        if dom.ranges(i).stride != 1 then return false;
    if !isDataContiguous(){ 
      if debugDefaultDistBulkTransfer then
        writeln("isDataContiguous return False"); 
      return false;
    }
    return true;
  }
  
  proc DefaultRectangularArr.doiCanBulkTransferStride() param {
    if debugDefaultDistBulkTransfer then writeln("In DefaultRectangularArr.doiCanBulkTransferStride()");
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
  
    const len = dom.dsiNumIndices:int(32);

    if len == 0 then return;

    if debugBulkTransfer {
      pragma "no prototype"
      extern proc sizeof(type x): int;
      const elemSize =sizeof(B._value.eltType);
      writeln("In DefaultRectangularArr.doiBulkTransfer():",
              " Alo=", Alo, ", Blo=", Blo,
              ", len=", len, ", elemSize=", elemSize);
    }
  
    // NOTE: This does not work with --heterogeneous, but heterogeneous
    // compilation does not work right now.  The calls to chpl_comm_get
    // and chpl_comm_put should be changed once that is fixed.
    if this.data.locale.id==here.id {
      if debugDefaultDistBulkTransfer then //See bug in test/optimizations/bulkcomm/alberto/rafatest2.chpl
        writeln("\tlocal get() from ", B._value.locale.id);
      const dest = this.theData;
      const src = B._value.theData;
      if dest != src {
        __primitive("chpl_comm_get",
                  __primitive("array_get", dest, getDataIndex(Alo)),
                  B._value.data.locale.id,
                  __primitive("array_get", src, B._value.getDataIndex(Blo)),
                  len);
      }
    } else if B._value.data.locale.id==here.id {
      if debugDefaultDistBulkTransfer then
        writeln("\tlocal put() to ", this.locale.id);
      const dest = this.theData;
      const src = B._value.theData;
      __primitive("chpl_comm_put",
                  __primitive("array_get", src, B._value.getDataIndex(Blo)),
                  this.data.locale.id,
                  __primitive("array_get", dest, getDataIndex(Alo)),
                  len);
    } else on this.data.locale {
      if debugDefaultDistBulkTransfer then
        writeln("\tremote get() on ", here.id, " from ", B.locale.id);
      const dest = this.theData;
      const src = B._value.theData;
      __primitive("chpl_comm_get",
                  __primitive("array_get", dest, getDataIndex(Alo)),
                  B._value.data.locale.id,
                  __primitive("array_get", src, B._value.getDataIndex(Blo)),
                  len);
    }
  }
  
  /*
  Data needed to use strided copy of data:
    + Stridelevels: the number of stride level (not really the number of dimensions because:
       - Stridelevels < rank if we can aggregate several dimensions due to they are consecutive 
           -- for exameple, whole rows --
       - Stridelevels == rank if there is a "by X" whith X>1 in the range description for 
           the rightmost dimension)
    + srcCount: slice size in each dimension for the source array. srcCount[0] should be the number of bytes of contiguous data in the rightmost dimension.
    + dstCount: slice size in each dimension for the destination array. dstCount[0] should be the number of bytes of contiguous data in the rightmost dimension.
    + dstStride: destination array of positive stride distances in bytes to move along each dimension
    + srcStrides: source array of positive stride distances in bytes to move along each dimension
    + dest: destination stating the address of the destination data block
    + src: source stating the address of the source data block
  More info in: http://www.escholarship.org/uc/item/5hg5r5fs?display=all
  Proposal for Extending the UPC Memory Copy Library Functions and Supporting 
  Extensions to GASNet, Version 2.0. Author: Dan Bonachea 
  
  A.doiBulkTransferStride(B) copies B-->A.
  */
  
  proc DefaultRectangularArr.doiBulkTransferStride(Barg) {
    const A = this, B = Barg;
  
    if debugDefaultDistBulkTransfer then 
      writeln("In DefaultRectangularArr.doiBulkTransferStride ");
   
    const Adims = A.dom.dsiDims();
    var Alo: rank*dom.idxType;
    for param i in 1..rank do
      Alo(i) = Adims(i).first;
    
    const Bdims = B.dom.dsiDims();
    var Blo: rank*dom.idxType;
    for param i in 1..rank do
      Blo(i) = Bdims(i).first;
    
    if debugDefaultDistBulkTransfer then
      writeln("\tlocal get() from ", B.locale.id);
    
    var dstWholeDim = isWholeDim(A),
        srcWholeDim = isWholeDim(B);
    var stridelevels:int(32);
    
    /* If the stridelevels in source and destination arrays are different, we take the larger*/
    stridelevels=max(A.computeBulkStrideLevels(dstWholeDim),B.computeBulkStrideLevels(srcWholeDim));
    if debugDefaultDistBulkTransfer then 
      writeln("In DefaultRectangularArr.doiBulkTransferStride, stridelevels: ",stridelevels);
    
    //These variables should be actually of size stridelevels+1, but stridelevels is not param...
    
    var srcCount, dstCount:[1..rank+1] int(32);
    
    // Covering the case in which stridelevels has to be incremented after
    // unifying srcCount and dstCount into a single count array. To illustrate the problem:
    
    // config const n = 10 
    // var A: [1..n,1..n] real(64);
    // var B: [1..n,1..n] real(64);
    
    // A[1..10,1..5] = B[1..10, 1..10 by 2]
    
    // Computed variables for chpl_comm_get_strd/puts are:
    //  stridelevels =1
    //  srcCount = (1,50) //In B you read 1 element 50 times
    //  (srcStride=(2) = distance between 1 element and the next one)
    //  dstCount = (5,10) //In A you write a chunk of 5 elments 10
    //  times (dstStride=(10) distance between 1 chunk and the next one)
    
    
    // Since GASNet strided put/get only have a count array, it is
    // necessary to choose the count array that follows the smaller size of
    // the chunks either at the source or destination array. That way the
    // right unified cnt=(1,5,10), which forces to increment stridelevels to
    // 2 and and now srcStride=(2,10) and dstStride=(1,10).
    var dstAux:bool = false;
    var srcAux:bool = false;
    if rank > 1 then
      if (A.dom.dsiDim(rank).stride>1 && B.dom.dsiDim(rank).stride==1)
      {
        if stridelevels < rank then stridelevels+=1;
        dstAux = true;
      }
      else if (B.dom.dsiDim(rank).stride>1 && A.dom.dsiDim(rank).stride==1)
      {
        if stridelevels < rank then stridelevels+=1;
        srcAux = true;
      }
    /* We now obtain the count arrays for source and destination arrays */
    dstCount= A.computeBulkCount(stridelevels,dstWholeDim,dstAux);
    srcCount= B.computeBulkCount(stridelevels,srcWholeDim,srcAux);
    
  /*Then the Stride arrays for source and destination arrays*/
    var dstStride, srcStride: [1..rank] int(32);
    /*When the source and destination arrays have different sizes 
      (example: A[1..10,1..10] and B[1..20,1..20]), the count arrays obtained are different,
      so we have to calculate the minimun count array */
    //Note that we use equal function equal instead of dstCount==srcCount due to the latter fails
    //The same for the array assigment (using assing function instead of srcCount=dstCount)
    
    if !bulkCommEqual(dstCount, srcCount, rank+1) //For different size arrays
    {
        for h in 1..stridelevels+1
        {
    if dstCount[h]>srcCount[h]
          {
      bulkCommAssign(dstCount,srcCount, stridelevels+1);
            break;
          }
          else if dstCount[h]<srcCount[h] then break; 
        }
    }
      
    dstStride = A.computeBulkStride(dstWholeDim,dstCount,stridelevels);
    srcStride = B.computeBulkStride(srcWholeDim,dstCount,stridelevels);
    
    //the same count array, dstCount, is used by both the sender and receiver
    doiBulkTransferStrideComm(Barg, stridelevels, dstStride, srcStride, dstCount, Alo, Blo);
  }
  
  //
  // Invoke the primitives chpl_comm_get_strd/puts, depending on what locale
  // we are on vs. where the source and destination are.
  // The logic mimics that in doiBulkTransfer().
  //
  proc DefaultRectangularArr.doiBulkTransferStrideComm(B, stridelevels:int(32), dstStride, srcStride, count, Alo, Blo)
   {
    if debugDefaultDistBulkTransfer then
      writeln("Locale: ", here.id, " stridelvl: ", stridelevels, " DstStride: ", dstStride," SrcStride: ",srcStride, " Count: ", count, " dst.Blk: ",blk, " src.Blk: ",B.blk);

    const A = this;
    //CASE 1: when the data in destination array is stored "here", it will use "chpl_comm_get_strd". 
    if A.data.locale==here
    {
      const dest = A.data;
      const src = B.data;
      
      const dststr=dstStride._value.theData;
      const srcstr=srcStride._value.theData;
      const cnt=count._value.theData;
  
      if debugBulkTransfer {
        writeln("Case 1");
        writeln("Locale:",here.id,"stridelevel: ", stridelevels);
        writeln("Locale:",here.id,"Count: ",count);
        writeln("Locale:",here.id," dststrides: ",dstStride);
        writeln("Locale:",here.id,",srcstrides: ",srcStride);
      }
      var srclocale = B.data.locale.id : int(32);
         __primitive("chpl_comm_get_strd",
                     __primitive("array_get",dest, A.getUnshiftedDataIndex(Alo)),
                     __primitive("array_get",dststr,dstStride._value.getDataIndex(1)), 
            srclocale,
                     __primitive("array_get",src, B.getUnshiftedDataIndex(Blo)),
                     __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)),
                     __primitive("array_get",cnt, count._value.getDataIndex(1)),
                      stridelevels);
    }
    //CASE 2: when the data in source array is stored "here", it will use "chpl_comm_put_strd". 
    else if B.data.locale==here
    {
      if debugDefaultDistBulkTransfer then
        writeln("\tlocal put() to ", A.locale.id);
      
      const dststr=dstStride._value.theData;
      const srcstr=srcStride._value.theData;
      const cnt=count._value.theData;
      
      if debugBulkTransfer {
        writeln("Case 2");
        writeln("stridelevel: ",stridelevels);
        writeln("Count: ",count);
        writeln("dststrides: ",dstStride);
        writeln("srcstrides: ",srcStride);
        writeln("Blk: ",blk);
      }
      
      const dest = A.data;
      const src = B.data;
      const destlocale =A.data.locale.id : int(32);
  
      __primitive("chpl_comm_put_strd",
                  __primitive("array_get",dest,A.getUnshiftedDataIndex(Alo)),
                  __primitive("array_get",dststr,dstStride._value.getDataIndex(1)),
                    destlocale,
                  __primitive("array_get",src,B.getUnshiftedDataIndex(Blo)),
                  __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)),
                  __primitive("array_get",cnt, count._value.getDataIndex(1)),
                  stridelevels);
    }
    //CASE 3: other case, it will use "chpl_comm_get_strd". 
    else on A.data.locale
    {   
      const dest = A.data;
      const src = B.data;
  
      //We are in a locale that doesn't store neither A nor B so we need to copy the auxiliarry
      //arrays to the locale that hosts A. This should translate into some more gets...
      const countAux=count:int(32);
      const srcstrides=srcStride:int(32);
      const dststrides=dstStride:int(32);

      const dststr=dststrides._value.theData;
      const srcstr=srcstrides._value.theData;
      const cnt=count._value.theData;

      if debugBulkTransfer {
        writeln("Case 3");
        writeln("stridelevel: ", stridelevels);
        writeln("Count: ",countAux);
        writeln("dststrides: ",dststrides);
        writeln("srcstrides: ",srcstrides);
      }
      
      const srclocale =B.data.locale.id : int(32);
         __primitive("chpl_comm_get_strd",
                     __primitive("array_get",dest, A.getUnshiftedDataIndex(Alo)),
                     __primitive("array_get",dststr,dststrides._value.getDataIndex(1)),
                     srclocale,
                     __primitive("array_get",src, B.getUnshiftedDataIndex(Blo)),
                     __primitive("array_get",srcstr,srcstrides._value.getDataIndex(1)),
                     __primitive("array_get",cnt, countAux._value.getDataIndex(1)),
                     stridelevels);

    }
  }
  
  proc DefaultRectangularArr.isDefaultRectangular() param return true;
  
  /* This function returns stridelevels for the default rectangular array.
    + Stridelevels: the number of stride level (not really the number of dimensions because:
       - Stridelevels < rank if we can aggregate several dimensions due to they are consecutive 
           -- for exameple, whole rows --
       - Stridelevels == rank if there is a "by X" whith X>1 in the range description for 
           the rightmost dimension)*/
  proc DefaultRectangularArr.computeBulkStrideLevels(rankcomp):int(32) where rank == 1
  {//To understand the blk(1)==1 condition,
    //see test/optimizations/bulkcomm/alberto/test_rank_change2.chpl(example 4)
    if (dom.dsiStride==1 && blk(1)==1)|| dom.dsiDim(1).length==1 then return 0;
    else return 1;
  }
  
  //Cases for computeBulkStrideLevels where rank>1:
  //Case 1:  
  //  var A: [1..4,1..4,1..4] real; A[1..4,3..4,1..4 by 2] 
  // --> In dimension 3 there is stride, because the elements are not
  //     consecutives, so stridelevels +=1
  //More in test/optimizations/bulkcomm/alberto/2dDRtoBDTest.chpl (example 8)
  
  //Case 2:
  //   Locales = 4
  //   var Dist1 = new dmap(new Block({1..4,1..4,1..4}));
  //   var Dom1: domain(3,int) dmapped Dist1 ={1..4,1..4,1..4};
  //   var A:[Dom1] int;
  //   A[1..4,2..4..3,1..1]--> blk:(8,4,1) 
  //   A[1..4,2..4,1]--> A[1..4,2..4] --> blk:(8,4) --> we need to check the blk array. 
  //More in test/optimizations/bulkcomm/alberto/rankchange.chpl (example 5)
  
  //Example for Case 3:  
  //  var A: [1..4,1..4,1..4] real; A[1..4,3..4,1..4 by 3] 
  //   --> the distance between the elements [1,3,4] and [1,4,1] is 1 element,
  //   while the distance between the elements in the rightmost dimension (rank) 
  //   is 3 positions([1,3,1],[1,3,4]), so the checkStrideDistance(i) for i=2 
  //   will return false, therefore, stridelevels +=1
  //More in test/optimizations/bulkcomm/alberto/2dDRtoBDTest.chpl (example 4)
  proc DefaultRectangularArr.computeBulkStrideLevels(rankcomp):int(32) where rank > 1 
  {
    var stridelevels:int(32) = 0;
    if (dom.dsiStride(rank)>1 && dom.dsiDim(rank).length>1) //CASE 1 
    || (blk(rank)>1 && dom.dsiDim(rank).length>1) //CASE 2   
    then stridelevels+=1; //In many tests, both cases are true
   
    for param i in 2..rank by -1 do
      if (dom.dsiDim(i-1).length>1 && !checkStrideDistance(i)) //CASE 3
        then stridelevels+=1; 
    
    return stridelevels:int(32);
  }
  
  /* This function returns the count array for the default rectangular array. */
  proc DefaultRectangularArr.computeBulkCount(stridelevels:int(32), rankcomp, aux = false):(rank+1)*int(32) where rank ==1
  {
    var c: (rank+1)*int(32);
    //To understand the blk(1)>1 condition,
    //see test/optimizations/bulkcomm/alberto/test_rank_change2.chpl(example 4)
    if dom.dsiStride > 1 || blk(1)>1 {
      c[1]=1;
      c[2]=dom.dsiDim(1).length:int(32);
    }
    else
      c[1]=dom.dsiDim(1).length:int(32);
    return c;
  }
  
  //Cases for computeBulkCount where rank>1:
  //Case 1:  
  //  var A: [1..4,1..4,1..4] real; A[1..4,3..4,1..4 by 2] 
  //    --> In dimension 3 there is stride, because the elements 
  //        are not consecutive, so c[1] = 1
  //More in test/optimizations/bulkcomm/alberto/perfTest.chpl (DR <- DR example 8)
  
  //Case 2:
  //    Locales = 4
  //    var Dist1 = new dmap(new Block({1..4,1..4,1..4}));
  //    var Dom1: domain(3,int) dmapped Dist1 ={1..4,1..4,1..4};
  //    var A:[Dom1] int;
  //    A[1..4,2..4,1..1]--> blk:(8,4,1) 
  //    A[1..4,2..4,1]--> A[1..4,2..4] --> blk:(8,4) --> c[1] = 1;
  //More in test/optimizations/bulkcomm/alberto/rankchange.chpl (example 5)
  
  //Case 3:  
  //    var A: [1..4,1..4,1..4] real; A[1..4,3..4,1..4 by 2] 
  //      --> the distance between the elements [1,3,3] and [1,4,1] is 2 elements,
  //          and the distance between the elements in the rightmost dimension(rank) 
  //          is also 2 positions([1,3,1],[1,3,3]), so it is possible to join both 
  //          dimensions, and the new count value is count[2] = 4 (2 x 2).
  //
  //Case 4:  
  //    var A: [1..4,1..4,1..4] real; A[1..4,4..4,1..4 by 3] 
  //      --> There is only 1 element in dimension 2, so it's possible to 
  //        join to dimension 1, and the value of count[3] will be the number 
  //        of elements in dimension 2 x number of elements in dimesion 1 (1 x 4).
  //More in test/optimizations/bulkcomms/alberto/3dAgTestStride.chpl (example 6 and 7)
  proc DefaultRectangularArr.computeBulkCount(stridelevels:int(32), rankcomp, aux = false):(rank+1)*int(32) where rank >1
  {
    var c: (rank+1)*int(32) ;
    var init:int(32)=1;
  //var dim is used to point to the analyzed dimension at each iteration
  //due to the same stride can be valid across two contiguous dimensions
    var dim:int =rank;
    var tmp:int(32)=1;
    if (dom.dsiStride(rank)>1 && dom.dsiDim(rank).length>1) //CASE 1
      ||(blk(rank)>1 && dom.dsiDim(rank).length>1) //CASE 2
      {c[1]=1; init=2;}
  //If the first value of count, c[1] have been already computed then
  //compute the rest of count starting at dim 2 (init=2)  
    for i in init..stridelevels+1 do
    {
      if dim == 0 then c[i]=1;//the leftmost dimension 
      else
        {
          c[i]=this.dom.dsiDim(dim).length:int(32);
  //find the next dimension for which the next different stride arises
    for h in 2..dim by -1:int(32) 
          {
  //The aux variable is to cover the case in which stridelevels has to be
  // incremented after unifying srcCount and dstCount into a single count array,
  // and the condition h!=rank is because the new count value it's always in the 
  // rightmost dimension.
  // See lines 850-871
      if( (checkStrideDistance(h) && (!aux || h!=rank))//CASE 3
               || (dom.dsiDim(h).length==1&& h!=rank)) //CASE 4
        {
          c[i]*=dom.dsiDim(h-1).length:int(32);
                dim -= 1;
        }
      else break;  
          }
          dim -= 1;
        }
    }
    return c;
  }
  
  /* This function returns the stride array for the default rectangular array. */
  
  //Case 1:
  //    Locales = 4
  //    var Dist1 = new dmap(new Block({1..4,1..4,1..4}));
  //    var Dom1: domain(3,int) dmapped Dist1 ={1..4,1..4,1..4};
  //    var A:[Dom1] int;
  //    A[1..4,2..4,1..1]--> blk:(8,4,1) 
  //    A[1..4,2..4,1]--> A[1..4,2..4] --> blk:(8,4) --> Stride[1] = blk(2) = 4;
  //More in test/optimizations/bulkcomm/alberto/rankchange.chpl (example 5)
  
  //Case 2:
  //    Locales = 2
  //    var Dist1 = new dmap(new Block({1..4,1..4,1..4}));
  //    var Dom1: domain(3,int) dmapped Dist1 ={1..4,1..4,1..4};
  //    var A:[Dom1] int;
  //    A[1..4 by 2,2..4 by 2,1..4 by 3]--> blk:(32,8,3)
  //    --> To get the value in Stride[2] we only need to check if the actual
  //        dimension has enough number of elements.
  //        To do this, we use a cumulative variable until the number of elements 
  //        are equal to count[3]. Then Stride[2] = blk(2) = 8, because this DR
  //        is part of a BD array.
  //More in test/optimizations/bulkcomm/alberto/perfTest_v2.chpl (BD <- BD Example 13)
  
  proc DefaultRectangularArr.computeBulkStride(rankcomp,cnt:[],levels:int(32)/*, aFromBD=false*/)
  {
    var c: rank*int(32); 
    var h=1; //Stride array index
    var cum=1; //cumulative variable
    
    if (cnt[h]==1 && dom.dsiDim(rank).length>1)
    {//To understand the blk[rank]==1 condition,
    //see test/optimizations/bulkcomm/alberto/test_rank_change2.chpl(example 12)
      c[h]=blk[rank]:int(32); //CASE 1
      h+=1;
    }
   
    for param i in 2..rank by -1:int(32){
      if (levels>=h)
      {
        if (cnt[h]==dom.dsiDim(i).length*cum && dom.dsiDim(i-1).length>1) //CASE 2
        {//now, we are in the right dimension (i dimension) to obtain the stride value
          c[h]=blk(i-1):int(32);
          h+=1; //Increment the index
          cum=1; //reset the cumulative variable
        }
        else cum=cum*dom.dsiDim(i).length;
      }
      
    }
    return c;
  }
  
  /*
  This function is used to help in the aggregation of data from different array dimensions.
  The function returns an array in which each position is associated to one of the dimensions
  of the domain. Each array component can be true or false, pointing out whether or not the rank
  for that particular dimension covers the whole dimension. 
  For instance, if for one array A, the domain is D=[1..4, 2..6, 1..6] and we refer to 
  A[1..3, 3..5,1..6] the resulting array will be [false, false, true].
  Note that the leftmost dimension is really not necessary due to the first dimension can not be 
  aggregated with any other one (there is no more dimensions beyond the first one). Therefore, a 
  subtle possible optimization would be to declare the resulting array 2..rank instead of 1..rank
  */ 
  proc DefaultRectangularArr.isWholeDim(d) where rank==1
  {
    return true;
  }
  
  proc DefaultRectangularArr.isWholeDim(d) where rank>1
  {
   var c:d.rank*bool;
    for param i in 2..rank do
      if (d.dom.dsiDim(i).length==d.blk(i-1)/d.blk(i) && dom.dsiStride(i)==1) then c[i]=true;
  
    return c;
  }
  
  /* This function checks if the stride in dimension 'x' is the same as the distance
  between the last element in dimension 'x' and  the first in dimension 'x-1'.
  If the distances are equal, we can aggregate these two dimmensions. 
  Example: 
  array A, the domain is D=[1..6, 1..6, 1..6]
  Let's A[1..6 by 2, 1..6, 1..6 by 2], then, checkStrideDistance(3) returns true, 
  due to when jumping from row to row (from the last element of one row, to the 
  first element of the next one) the stride is the same than when jumping from 
  elements inside the row. 
  */
  proc DefaultRectangularArr.checkStrideDistance(x: int)
  {
    if dom.dsiDim(x-1).length==1 then return false;
  
    if (blk(x-1)*dom.dsiStride(x))/blk(x) - (1+dom.dsiDim(x).last - dom.dsiDim(x).first) == dom.dsiStride(x)-1
    && dom.dsiDim(x).length>1
    && (dom.dsiStride(x-1)==1 || dom.dsiDim(x-1).length==1)
    then return true;
  
    return false;
  }
  
  //
  // Check whether the first 'tam' elements in arrays 'd1' and 'd2' are equal.
  // This is better than 'd1==d2' because it does not result in a forall.
  // TODO: convert to 'for' for rank 1..5. (The caller must ensure that
  // d1 and d2 are always equal at indices tam+1..rank.)
  // Ideally, d1 and d2 will become tuples.
  //
  proc bulkCommEqual(d1:[], d2:[], tam:int)
  {
    var c:bool = true;
    for i in 1..tam do if d1[i]!=d2[i] then c=false;
    return c;
  }
  
  //
  // Assign the first 'tam' elements from array 'd2' to array 'd1'.
  // This is better than 'd1=d2' because it does not result in a forall.
  // TODO: convert to 'for' for rank 1..5.
  // Ideally, d1 and d2 will become tuples.
  //
  proc bulkCommAssign(d1:[], d2:[], tam: int)
  {
    for i in 1..tam do d1[i]=d2[i];
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
    const AD = Aarr.dom.dsiDims();
    const BD = Barr.dom.dsiDims();
    var result: rank * int;
    for param i in 1..rank {
      const ar = AD(i), br = BD(i);
      if boundsChecking then assert(br.member(b(i)));
      result(i) = ar.orderToIndex(br.indexOrder(b(i)));
    }
    return result;
  }
  
}
