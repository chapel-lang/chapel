/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  config const dataParIgnoreRunningTasks = false;
  config const dataParMinGranularity: int = 1;

  if dataParTasksPerLocale<0 then halt("dataParTasksPerLocale must be >= 0");
  if dataParMinGranularity<=0 then halt("dataParMinGranularity must be > 0");

  use DSIUtil, ChapelArray;
  private use ChapelDistribution, ChapelRange, SysBasic, SysError, SysCTypes;
  private use ChapelDebugPrint, ChapelLocks, OwnedObject, IO;
  private use DefaultSparse, DefaultAssociative;
  use ExternalArray;

  config param debugDefaultDist = false;
  config param debugDefaultDistBulkTransfer = false;
  config param debugDataPar = false;
  config param debugDataParNuma = false;

  config param defaultDoRADOpt = true;
  config param defaultDisableLazyRADOpt = false;
  config param earlyShiftData = true;
  config param usePollyArrayIndex = false;

  enum ArrayStorageOrder { RMO, CMO }
  config param defaultStorageOrder = ArrayStorageOrder.RMO;
  // would like to move this into DefaultRectangularArr and
  // DefaultRectangularDom so each instance can choose individually
  param storageOrder = defaultStorageOrder;

  // A function which help to compute the final index
  // to be used for DefaultRectangularArr access. This function
  // helps Polly to effectively communicate the array dimension
  // sizes and the index subscripts to Polly.
  pragma "lineno ok"
  pragma "llvm readnone"
  proc polly_array_index(arguments:int ...):int {
    param rank = (arguments.size - 1) / 2;
    param blkStart = 2;
    param blkEnd = 2 + rank - 1;
    param indStart = blkEnd + 1;
    param indEnd = indStart + rank - 1;
    var offset = arguments(1);
    var blk:rank*int;
    var ind:rank*int;

    blk(rank) = 1;
    for param i in 1..(rank-1) by -1 do
      blk(i) = blk(i+1) * arguments(blkStart+i);

    for param j in 1..rank {
      ind(j) = arguments(indStart+j-1);
    }

    var ret:int = offset;
    for param i in 1..rank {
      ret += ind(i) * blk(i);
    }

    return ret;
  }

  class DefaultDist: BaseDist {
    override proc dsiNewRectangularDom(param rank: int, type idxType,
                                       param stridable: bool, inds) {
      const dom = new unmanaged DefaultRectangularDom(rank, idxType, stridable,
                                                      _to_unmanaged(this));
      dom.dsiSetIndices(inds);
      return dom;
    }

    override proc dsiNewAssociativeDom(type idxType, param parSafe: bool)
      return new unmanaged DefaultAssociativeDom(idxType, parSafe, _to_unmanaged(this));

    override proc dsiNewSparseDom(param rank: int, type idxType, dom: domain)
      return new unmanaged DefaultSparseDom(rank, idxType, _to_unmanaged(this), dom);

    proc dsiIndexToLocale(ind) return this.locale;

    // Right now, the default distribution acts like a singleton.
    // So we don't have to copy it when a clone is requested.
    proc dsiClone() return _to_unmanaged(this);

    proc dsiAssign(other: unmanaged this.type) { }

    proc dsiEqualDMaps(d:unmanaged DefaultDist) param return true;
    proc dsiEqualDMaps(d) param return false;

    proc trackDomains() param return false;
    override proc dsiTrackDomains()    return false;

    proc singleton() param return true;

    proc dsiIsLayout() param return true;
  }

  //
  // Replicated copies are set up in chpl_initOnLocales() during locale
  // model initialization
  //
  pragma "locale private"
  var defaultDist = new dmap(new unmanaged DefaultDist());

  proc chpl_defaultDistInitPrivate() {
    if defaultDist._value==nil {
      // FIXME benharsh: Here's what we want to do:
      //   defaultDist = new dmap(new DefaultDist());
      // The problem is that the LHS of the "proc =" for _distributions
      // loses its ref intent in the removeWrapRecords pass.
      //
      // The code below is copied from the contents of the "proc =".
      const nd = new dmap(new unmanaged DefaultDist());
      __primitive("move", defaultDist, chpl__autoCopy(nd.clone()));
    }
  }

  class DefaultRectangularDom: BaseRectangularDom {
    var dist: unmanaged DefaultDist;
    var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

    proc linksDistribution() param return false;
    override proc dsiLinksDistribution()     return false;

    proc type isDefaultRectangular() param return true;
    proc isDefaultRectangular() param return true;

    proc init(param rank, type idxType, param stridable, dist) {
      super.init(rank, idxType, stridable);
      this.dist = dist;
    }

    proc intIdxType type {
      return chpl__idxTypeToIntIdxType(idxType);
    }

    override proc dsiMyDist() {
      return dist;
    }

    pragma "no doc"
    record _serialized_domain {
      param rank;
      type idxType;
      param stridable;
      var dims;
      param isDefaultRectangular;
    }

    proc chpl__serialize() {
      return new _serialized_domain(rank, idxType, stridable, dsiDims(), true);
    }

    proc type chpl__deserialize(data) {
      return defaultDist.newRectangularDom(data.rank,
                                           data.idxType,
                                           data.stridable,
                                           data.dims);

    }

    override proc dsiDisplayRepresentation() {
      writeln("ranges = ", ranges);
    }

    // function and iterator versions, also for setIndices
    proc dsiGetIndices() return ranges;

    proc dsiSetIndices(x) {
      ranges = x;
    }

    proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
      chpl_assignDomainWithGetSetIndices(this, rhs);
    }

    iter these_help(param d: int) /*where storageOrder == ArrayStorageOrder.RMO*/ {
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
/*
    iter these_help(param d: int) where storageOrder == ArrayStorageOrder.CMO {
      param rd = rank - d + 1;
      if rd == 1 {
        for i in ranges(rd) do
          yield i;
      } else if rd == 2 {
        for i in ranges(rd) do
          for j in these_help(rank) do
            yield (j, i);
      } else {
        for i in ranges(rd) do
          for j in these_help(d+1) do
            yield ((...j), i);
      }
    }
*/

    iter these_help(param d: int, block) /*where storageOrder == ArrayStorageOrder.RMO*/ {
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

/*
    iter these_help(param d: int, block) where storageOrder == ArrayStorageOrder.CMO {
      param rd = rank - d + 1;
      if rd == 1 {
        for i in block(rd) do
          yield i;
      } else if rd == 2 {
        for i in block(rd) do
          for j in these_help(block.size, block) do
            yield (j, i);
      } else {
        for i in block(rd) do
          for j in these_help(d+1, block) do
            yield ((...j), i);
      }
    }
*/

    iter these(tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, intIdxType, 0:intIdxType)) {
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
               offset=createTuple(rank, intIdxType, 0:intIdxType))
      where tag == iterKind.standalone {
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
                           " ranges(", parDim, ").size=", ranges(parDim).size);
      }
      if debugDataPar {
        chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n",
                           "### ignoreRunning = ", ignoreRunning, "\n",
                           "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                           "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n",
                           "### nranges = ", ranges);
      }
      if numChunks <= 1 {
        for i in these_help(1) {
          yield i;
        }
      } else {
        if debugDefaultDist {
          chpl_debug_writeln("*** DI: ranges = ", ranges);
        }
        // TODO: The following is somewhat of an abuse of what
        // _computeBlock() was designed for (dense ranges only; I
        // multiplied by the stride as a white lie to make it work
        // reasonably.  We should switch to using the RangeChunk
        // library...
        coforall chunk in 0..#numChunks {
          var block = ranges;
          const len = if (!ranges(parDim).stridable) then ranges(parDim).size
              else ranges(parDim).size:uint * abs(ranges(parDim).stride):uint;
          const (lo,hi) = _computeBlock(len,
                                        numChunks, chunk,
                                        ranges(parDim)._high,
                                        ranges(parDim)._low,
                                        ranges(parDim)._low);
          if block(parDim).stridable then
            block(parDim) = lo..hi by block(parDim).stride align chpl__idxToInt(block(parDim).alignment);
          else
            block(parDim) = lo..hi;
          if debugDefaultDist {
            chpl_debug_writeln("*** DI[", chunk, "]: block = ", block);
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
               offset=createTuple(rank, intIdxType, 0:intIdxType))
      where tag == iterKind.leader {

      const numSublocs = here.getChildCount();

      if localeModelHasSublocales && numSublocs != 0 {
        var dptpl = if tasksPerLocale==0 then here.maxTaskPar
                    else tasksPerLocale;
        if !ignoreRunning {
          const otherTasks = here.runningTasks() - 1; // don't include self
          dptpl = if otherTasks < dptpl then (dptpl-otherTasks):int else 1;
        }
        // Make sure we don't use more sublocales than the numbers of
        // tasksPerLocale requested
        const numSublocTasks = min(numSublocs, dptpl);
        // For serial tasks, we will only have a single chunk
        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, -1) else
                                    _computeChunkStuff(numSublocTasks,
                                                       ignoreRunning=true,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDataParNuma {
          chpl_debug_writeln("### numSublocs = ", numSublocs, "\n",
                 "### numTasksPerSubloc = ", numSublocTasks, "\n",
                  "### ignoreRunning = ", ignoreRunning, "\n",
                  "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                  "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n",
                  "### nranges = ", ranges);
        }

        if numChunks == 1 {
          if rank == 1 {
            yield (offset(1)..#ranges(1).size,);
          } else {
            var block: rank*range(intIdxType);
            for param i in 1..rank do
              block(i) = offset(i)..#ranges(i).size;
            yield block;
          }
        } else {
          coforall chunk in 0..#numChunks { // make sure coforall on can trigger
            local do on here.getChild(chunk) {
              if debugDataParNuma {
                if chunk!=chpl_getSubloc() then
                  chpl_debug_writeln("*** ERROR: ON WRONG SUBLOC (should be ", chunk,
                                     ", on ", chpl_getSubloc(), ") ***");
              }
              // Divide the locale's tasks approximately evenly
              // among the sublocales
              const numSublocTasks = (if chunk < dptpl % numChunks
                                      then dptpl / numChunks + 1
                                      else dptpl / numChunks);
              var locBlock: rank*range(intIdxType);
              for param i in 1..rank do
                locBlock(i) = offset(i)..#(ranges(i).size);
              var followMe: rank*range(intIdxType) = locBlock;
              const (lo,hi) = _computeBlock(locBlock(parDim).size,
                                            numChunks, chunk,
                                            locBlock(parDim)._high,
                                            locBlock(parDim)._low,
                                            locBlock(parDim)._low);
              followMe(parDim) = lo..hi;
              const (numChunks2, parDim2) = _computeChunkStuff(numSublocTasks,
                                                               ignoreRunning=true,
                                                               minIndicesPerTask,
                                                               followMe);
              coforall chunk2 in 0..#numChunks2 {
                var locBlock2: rank*range(intIdxType);
                for param i in 1..rank do
                  locBlock2(i) = followMe(i).low..followMe(i).high;
                var followMe2: rank*range(intIdxType) = locBlock2;
                const low  = locBlock2(parDim2)._low,
                  high = locBlock2(parDim2)._high;
                const (lo,hi) = _computeBlock(locBlock2(parDim2).size,
                                              numChunks2, chunk2,
                                              high, low, low);
                followMe2(parDim2) = lo..hi;
                if debugDataParNuma {
                  chpl_debug_writeln("### chunk = ", chunk, "  chunk2 = ", chunk2, "  ",
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
                  " ranges(", parDim, ").size=", ranges(parDim).size);

        if debugDataPar {
          chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n",
                 "### ignoreRunning = ", ignoreRunning, "\n",
                 "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                 "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n",
                  "### nranges = ", ranges);
        }

        if numChunks == 1 {
          if rank == 1 {
            yield (offset(1)..#ranges(1).size,);
          } else {
            var block: rank*range(intIdxType);
            for param i in 1..rank do
              block(i) = offset(i)..#ranges(i).size;
            yield block;
          }
        } else {
          var locBlock: rank*range(intIdxType);
          for param i in 1..rank do
            locBlock(i) = offset(i)..#(ranges(i).size);
          if debugDefaultDist then
            chpl_debug_writeln("*** DI: locBlock = ", locBlock);
          coforall chunk in 0..#numChunks {
            var followMe: rank*range(intIdxType) = locBlock;
            const (lo,hi) = _computeBlock(locBlock(parDim).size,
                                          numChunks, chunk,
                                          locBlock(parDim)._high,
                                          locBlock(parDim)._low,
                                          locBlock(parDim)._low);
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
               offset=createTuple(rank, intIdxType, 0:intIdxType))
      where tag == iterKind.follower {

      proc anyStridable(rangeTuple, param i: int = 1) param
        return if i == rangeTuple.size then rangeTuple(i).stridable
               else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

      if chpl__testParFlag then
        chpl__testPar("default rectangular domain follower invoked on ", followThis);
      if debugDefaultDist then
        chpl_debug_writeln("In domain follower code: Following ", followThis);

      param stridable = this.stridable || anyStridable(followThis);
      var block: rank*range(idxType=intIdxType, stridable=stridable);
      if stridable {
        type strType = chpl__signedType(intIdxType);
        for param i in 1..rank {
          // See domain follower for comments about this
          const rStride = ranges(i).stride;
          const rSignedStride = rStride:strType,
                fSignedStride = followThis(i).stride:strType;
          if rStride > 0 {
            const riStride = rStride:intIdxType;
            const low = ranges(i).alignedLowAsInt + followThis(i).low*riStride,
                  high = ranges(i).alignedLowAsInt + followThis(i).high*riStride,
                  stride = (rSignedStride * fSignedStride):strType;
            block(i) = low..high by stride;
          } else {
            const irStride = (-rStride):intIdxType;
            const low = ranges(i).alignedHighAsInt - followThis(i).high*irStride,
                  high = ranges(i).alignedHighAsInt - followThis(i).low*irStride,
                  stride = (rSignedStride * fSignedStride):strType;
            block(i) = low..high by stride;
          }
        }
      } else {
        for  param i in 1..rank do
          block(i) = ranges(i)._low+followThis(i).low:intIdxType..ranges(i)._low+followThis(i).high:intIdxType;
      }

      if rank == 1 {
        for i in zip((...block)) {
          yield chpl_intToIdx(i);
        }
      } else {
        for i in these_help(1, block) {
          yield chpl_intToIdx(i);
        }
      }
    }

    proc dsiMember(ind: rank*idxType) {
      for param i in 1..rank do
        if !ranges(i).contains(ind(i)) then
          return false;
      return true;
    }

    proc dsiIndexOrder(ind: rank*idxType) {
      var totOrder: intIdxType;
      var blk: intIdxType = 1;
      for param d in 1..rank by -1 {
        const orderD = ranges(d).indexOrder(ind(d));
        // NOTE: This follows from the implementation of indexOrder()
        if (orderD == (-1):intIdxType) then return orderD;
        totOrder += orderD * blk;
        blk *= ranges(d).size;
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
      var sum = 1:intIdxType;
      for param i in 1..rank do
        sum *= ranges(i).size;
      return sum;
      // WANT: return * reduce (this(1..rank).size);
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
        var result: rank*chpl__signedType(intIdxType);
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
      return new unmanaged DefaultRectangularArr(eltType=eltType, rank=rank,
                                                 idxType=idxType,
                                                 stridable=stridable,
                                                 dom=_to_unmanaged(this));
    }

    proc dsiBuildArrayWith(type eltType, data:_ddata(eltType), allocSize:int) {

      var allocRange:range(idxType) = (ranges(1).low)..#allocSize;
      return new unmanaged DefaultRectangularArr(eltType=eltType,
                                       rank=rank,
                                       idxType=idxType,
                                       stridable=stridable,
                                       dom=_to_unmanaged(this),
                                       data=data);
    }


    proc dsiLocalSlice(ranges) {
      halt("all dsiLocalSlice calls on DefaultRectangulars should be handled in ChapelArray.chpl");
    }

    proc dsiTargetLocales() {
      return [this.locale, ];
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if (this.locale == loc) {
        return _getDomain(_to_unmanaged(this));
      } else {
        var a: domain(rank, idxType, stridable);
        return a;
      }
    }

    iter dsiLocalSubdomains(loc: locale) {
      yield dsiLocalSubdomain(loc);
    }

    // convenience routine for turning an int (tuple) into an index (tuple)
    inline proc chpl_intToIdx(i) {
      return chpl__intToIdx(this.idxType, i);
    }
  }

  // helper routines for converting tuples of integers into tuple indices
  
  inline proc chpl__intToIdx(type idxType, i: integral, j ...) {
    const first = chpl__intToIdx(idxType, i);
    const rest = chpl__intToIdx(idxType, (...j));
    return (first, (...rest));
  }

  inline proc chpl__intToIdx(type idxType, i: integral, j: integral) {
    return (chpl__intToIdx(idxType, i), chpl__intToIdx(idxType, j));
  }

  inline proc chpl__intToIdx(type idxType, i: _tuple) {
    return chpl__intToIdx(idxType, (...i));
  }

  // TODO: should this include the ranges that represent the domain?
  record _remoteAccessData {
    type eltType;
    param rank : int;
    type idxType;
    param stridable: bool;
    param blkChanged : bool = false;

    var off: rank*idxType;
    var blk: rank*chpl__idxTypeToIntIdxType(idxType);
    var str: rank*chpl__signedType(chpl__idxTypeToIntIdxType(idxType));
    var origin: chpl__idxTypeToIntIdxType(idxType);
    var factoredOffs: chpl__idxTypeToIntIdxType(idxType);

    var data: _ddata(eltType);
    var shiftedData: _ddata(eltType);


    inline proc theData ref {
      if stridable {
        return data;
      } else {
        return shiftedData;
      }
    }

    inline proc getDataElem(i) ref {
      if stridable {
        return dataElem(i);
      } else {
        return shiftedDataElem(i);
      }
    }

    inline proc dataElem(i) ref {
      return data(i);
    }

    inline proc shiftedDataElem(i) ref
      return shiftedData(i);
  }

  inline proc _remoteAccessData.getDataIndex(ind : idxType) {
    return this.getDataIndex(chpl__tuplify(ind));
  }

  //
  // Copied from DefaultRectangularArr.getDataIndex
  //
  inline proc _remoteAccessData.getDataIndex(ind: rank*idxType) {
    if stridable {
      var sum = origin;
      for param i in 1..rank do
        sum += (chpl__idxToInt(ind(i)) - chpl__idxToInt(off(i))) * blk(i) / abs(str(i)):chpl__idxTypeToIntIdxType(idxType);
      return sum;
    } else {
      // optimize common case to get cleaner generated code
      if (rank == 1 && earlyShiftData) {
        if blkChanged {
          return chpl__idxToInt(ind(1)) * blk(1);
        } else {
          return chpl__idxToInt(ind(1));
        }
      } else {
        var sum = if earlyShiftData then 0:chpl__idxTypeToIntIdxType(idxType) else origin;

        if blkChanged {
          for param i in 1..rank {
            sum += chpl__idxToInt(ind(i)) * blk(i);
          }
        } else {
          if storageOrder == ArrayStorageOrder.RMO {
            for param i in 1..rank-1 {
              sum += chpl__idxToInt(ind(i)) * blk(i);
            }
            sum += chpl__idxToInt(ind(rank));
          } else {
            for param i in 2..rank {
              sum += chpl__idxToInt(ind(i)) * blk(i);
            }
            sum += chpl__idxToInt(ind(1));
          }
        }

        if !earlyShiftData then sum -= factoredOffs;
        return sum;
      }
    }
  }

  proc _remoteAccessData.computeFactoredOffs() {
    factoredOffs = 0;
    for param i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * chpl__idxToInt(off(i));
    }
  }

  proc _remoteAccessData.initShiftedData() {
    if earlyShiftData && !stridable {
      type idxSignedType = chpl__signedType(chpl__idxTypeToIntIdxType(idxType));
      const shiftDist = if isIntType(idxType) then origin - factoredOffs
                        else origin:idxSignedType - factoredOffs:idxSignedType;
      shiftedData = _ddata_shift(eltType, data, shiftDist);
    }
  }

  proc _remoteAccessData.strideAlignUp(lo, r)
    return r.low + (lo - r.low + abs(r.stride):idxType - 1)
           / abs(r.stride):idxType * abs(r.stride):idxType;

  proc _remoteAccessData.strideAlignDown(hi, r)
    return hi - (hi - r.low) % abs(r.stride):idxType;

  proc _remoteAccessData.initDataFrom(other : _remoteAccessData) {
    this.data = other.data;
  }

  //
  // Based on the old 'dsiSlice' method
  //
  proc _remoteAccessData.toSlice(newDom) {
    compilerAssert(this.rank == newDom.rank);

    // NB: Sets 'blkChanged' if the new domain is stridable.
    var rad : _remoteAccessData(eltType, newDom.rank, newDom.idxType, newDom.stridable, newDom.stridable || this.blkChanged);

    rad.initDataFrom(this);

    rad.shiftedData = if newDom.stridable then this.data else this.shiftedData;
    rad.origin      = this.origin:newDom.idxType;
    rad.off         = chpl__tuplify(newDom.dsiLow);
    rad.str         = chpl__tuplify(newDom.dsiStride);

    for param i in 1..rank {
      const shift = this.blk(i) * (chpl__idxToInt(newDom.dsiDim(i).low) - chpl__idxToInt(this.off(i))) / abs(this.str(i)) : rad.idxType;
      if this.str(i) > 0 {
        rad.origin += shift;
      } else {
        rad.origin -= shift;
      }

      const mult = (newDom.dsiDim(i).stride / this.str(i)) : rad.idxType;
      rad.blk(i) = this.blk(i) * mult;
    }

    rad.computeFactoredOffs();
    rad.initShiftedData();

    return rad;
  }

  //
  // Based on the old 'dsiReindex' method
  //
  proc _remoteAccessData.toReindex(newDom) {
    compilerAssert(this.rank == newDom.rank);

    // NB: Only sets 'blkChanged' if underlying RADs have it set
    var rad : _remoteAccessData(eltType, newDom.rank, newDom.idxType, newDom.stridable, blkChanged);

    rad.initDataFrom(this);

    rad.shiftedData  = if newDom.stridable then this.data else this.shiftedData;
    rad.origin       = this.origin:newDom.intIdxType;
    rad.blk          = this.blk;
    rad.off          = chpl__tuplify(newDom.dsiLow);
    rad.str          = chpl__tuplify(newDom.dsiStride);
    rad.factoredOffs = 0:newDom.intIdxType;

    rad.computeFactoredOffs();
    rad.initShiftedData();

    return rad;
  }

  //
  // Based on the old 'dsiRankChange' method
  //
  proc _remoteAccessData.toRankChange(newDom, cd, idx) {
    compilerAssert(this.rank == idx.size && this.rank != newDom.rank);
    type intIdxType = newDom.intIdxType;
    type idxSignedType = chpl__signedType(intIdxType);

    // Unconditionally sets 'blkChanged'
    //
    // TODO: If 'collapsedDims' were param, we would know if blk(rank) was 1 or not.
    var rad : _remoteAccessData(eltType, newDom.rank, newDom.idxType, newDom.stridable, true);
    const collapsedDims = chpl__tuplify(cd);

    rad.initDataFrom(this);

    rad.shiftedData = if newDom.stridable then this.data else this.shiftedData;
    rad.origin      = this.origin:newDom.intIdxType;

    var curDim      = 1;
    for param j in 1..idx.size {
      if !collapsedDims(j) {
        rad.off(curDim) = newDom.dsiDim(curDim).low;
        const off       = (chpl__idxToInt(rad.off(curDim)) - chpl__idxToInt(this.off(j))):idxSignedType;
        rad.origin     += ((this.blk(j):idxSignedType) * off / this.str(j)):intIdxType;
        rad.blk(curDim) = this.blk(j);
        rad.str(curDim) = this.str(j);

        curDim += 1;
      } else {
        const off   = (chpl__idxToInt(idx(j)) - chpl__idxToInt(this.off(j))):idxSignedType;
        rad.origin += (this.blk(j):idxSignedType *  off / this.str(j)):intIdxType;
      }
    }

    rad.computeFactoredOffs();
    rad.initShiftedData();

    return rad;
  }

  //
  // Local cache of remote ddata access info
  //
  class LocRADCache {
    type eltType;
    param rank: int;
    type idxType;
    param stridable: bool;
    var targetLocDom: domain(rank);
    var RAD: [targetLocDom] _remoteAccessData(eltType, rank, idxType,
                                              stridable);
    var RADLocks: [targetLocDom] chpl_LocalSpinlock;

    pragma "dont disable remote value forwarding"
    proc init(type eltType, param rank: int, type idxType,
              param stridable: bool, newTargetLocDom: domain(rank)) {
      this.eltType = eltType;
      this.rank = rank;
      this.idxType = idxType;
      this.stridable = stridable;
      // This should resize the arrays
      targetLocDom=newTargetLocDom;
    }

    inline proc lockRAD(rlocIdx) {
      RADLocks[rlocIdx].lock();
    }

    inline proc unlockRAD(rlocIdx) {
      RADLocks[rlocIdx].unlock();
    }
  }

  class DefaultRectangularArr: BaseRectangularArr {
    /*type eltType;
    param rank : int;
    type idxType;
    param stridable: bool;*/

    type idxSignedType = chpl__signedType(chpl__idxTypeToIntIdxType(idxType));

    var dom : unmanaged DefaultRectangularDom(rank=rank, idxType=idxType,
                                           stridable=stridable);
    var off: rank*idxType;
    var blk: rank*chpl__idxTypeToIntIdxType(idxType);
    var sizesPerDim: rank*chpl__idxTypeToIntIdxType(idxType);
    var str: rank*idxSignedType;
    var factoredOffs: chpl__idxTypeToIntIdxType(idxType);

    pragma "alias scope from this"
    pragma "local field"
    var data : _ddata(eltType) = nil;

    pragma "alias scope from this"
    pragma "local field"
    var shiftedData : _ddata(eltType);

    // note: used by pychapel
    var noinit_data: bool = false;

    // note: used for external array support
    var externArr: bool = false;
    var _borrowed: bool = true;
    var externFreeFunc: c_void_ptr;

    //var numelm: int = -1; // for correctness checking

    // end class definition here, then defined secondary methods below

    proc intIdxType type {
      return chpl__idxTypeToIntIdxType(idxType);
    }

    override proc dsiDisplayRepresentation() {
      writeln("off=", off);
      writeln("blk=", blk);
      writeln("str=", str);
      writeln("factoredOffs=", factoredOffs);
      writeln("noinit_data=", noinit_data);
    }

    // can the compiler create this automatically?
    override proc dsiGetBaseDom() return dom;

    proc dsiDestroyDataHelper(dd, ddiNumIndices) {
      compilerAssert(chpl_isDdata(dd.type));
      for i in 0..ddiNumIndices-1 {
        chpl__autoDestroy(dd[i]);
      }
    }

    override proc dsiDestroyArr() {
      if (externArr) {
        if (!_borrowed) {
          chpl_call_free_func(externFreeFunc, c_ptrTo(data));
        }
      } else {
        var numElts:intIdxType = 0;
        if dom.dsiNumIndices > 0 {
          param needsDestroy = __primitive("needs auto destroy",
                                           __primitive("deref", data[0]));
          numElts = dom.dsiNumIndices;

          if needsDestroy {
            dsiDestroyDataHelper(data, numElts);
          }
        }
        _ddata_free(data, numElts);
      }
    }

    inline proc theData ref {
      if earlyShiftData && !stridable then
        return shiftedData;
      else
        return data;
    }

    //
    // Simple-ddata iterators (locale models without sublocales)
    //
    iter these(tasksPerLocale:int = dataParTasksPerLocale,
               ignoreRunning:bool = dataParIgnoreRunningTasks,
               minIndicesPerTask:int = dataParMinGranularity) ref {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr simple-dd serial iterator");
      }
      for elem in chpl__serialViewIter(this, dom) do yield elem;
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      ref where tag == iterKind.standalone {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr simple-dd standalone iterator");
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
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr simple-dd leader iterator");
      }
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
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr simple-dd follower iterator: ",
                           followThis);
      }

      for i in dom.these(tag=iterKind.follower, followThis,
                         tasksPerLocale,
                         ignoreRunning,
                         minIndicesPerTask) do
        yield dsiAccess(i);
    }

    proc computeFactoredOffs() {
      factoredOffs = 0:intIdxType;
      for param i in 1..rank do {
        factoredOffs = factoredOffs + blk(i) * chpl__idxToInt(off(i));
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
          const shiftDist = if isIntType(idxType) then
                              0:idxType - factoredOffs
                            else
                              // Not bothering to check for over/underflow
                              0:idxSignedType - factoredOffs:idxSignedType;
          shiftedData = _ddata_shift(eltType, data, shiftDist);
        }
      }
    }

    proc postinit() {
      if noinit_data == true then return;
      for param dim in 1..rank {
        off(dim) = dom.dsiDim(dim).alignedLow;
        str(dim) = dom.dsiDim(dim).stride;
      }
      if storageOrder == ArrayStorageOrder.RMO {
        blk(rank) = 1:intIdxType;
        for param dim in 1..(rank-1) by -1 do
          blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).size;
      } else if storageOrder == ArrayStorageOrder.CMO {
        blk(1) = 1:intIdxType;
        for param dim in 2..rank {
          blk(dim) = blk(dim-1) * dom.dsiDim(dim-1).size;
        }
      } else {
        halt("unknown array storage order");
      }
      computeFactoredOffs();
      const size = if storageOrder == ArrayStorageOrder.RMO
                   then blk(1) * dom.dsiDim(1).size
                   else blk(rank) * dom.dsiDim(rank).size;

      if usePollyArrayIndex {
        for param dim in 1..rank {
         sizesPerDim(dim) = dom.dsiDim(dim).size;
        }
      }

      // Allow DR array initialization to pass in existing data
      if data == nil {
        if !localeModelHasSublocales {
          data = _ddata_allocate(eltType, size);
        } else {
          data = _ddata_allocate(eltType, size,
                                 subloc = (if here.getChildCount() > 1
                                           then c_sublocid_all
                                           else c_sublocid_none));
        }
      }

      initShiftedData();
    }

    inline proc getDataIndex(ind: idxType ...1,
                             param getShifted = true)
      where rank == 1
      return getDataIndex(ind, getShifted=getShifted);

    inline proc getDataIndex(ind: rank*idxType,
                             param getShifted = true) {
      if stridable {
        var sum = 0:intIdxType;
        for param i in 1..rank do
          sum += (chpl__idxToInt(ind(i)) - chpl__idxToInt(off(i))) * blk(i) / abs(str(i)):intIdxType;
        return sum;
      } else {
        param wantShiftedIndex = getShifted && earlyShiftData;

        // optimize common case to get cleaner generated code
        if (rank == 1 && wantShiftedIndex) {
          return chpl__idxToInt(ind(1));
        } else {
          var sum = 0:intIdxType;
          var useInd = ind;
          var useOffset:int = 0;
          var useSizesPerDim = sizesPerDim;

          if usePollyArrayIndex {
            // Polly works better if we provide 0-based indices from the start.
            // So instead of using factoredOffs at the end, we initially subtract
            // the dimension offsets from the index subscripts beforehand.
            if !wantShiftedIndex {
             for param i in 1..rank do {
               useInd(i) = chpl__idxToInt(useInd(i)) - chpl__idxToInt(off(i));
             }
           }
           return polly_array_index(useOffset, (...useSizesPerDim), (...useInd));
          } else {
            if storageOrder == ArrayStorageOrder.RMO {
              for param i in 1..rank-1 {
                sum += chpl__idxToInt(ind(i)) * blk(i);
              }
              sum += chpl__idxToInt(ind(rank));
            } else {
              for param i in 2..rank {
                sum += chpl__idxToInt(ind(i)) * blk(i);
              }
              sum += chpl__idxToInt(ind(1));
            }
            if !wantShiftedIndex then sum -= factoredOffs;
            return sum;
          }
        }
      }
    }

    // only need second version (ind : rank*idxType)
    // because wrapper record can pass a 1-tuple
    inline proc dsiAccess(ind: idxType ...1) ref
    where rank == 1
      return dsiAccess(ind);

    inline proc dsiAccess(ind: idxType ...1)
    where rank == 1 && shouldReturnRvalueByValue(eltType)
      return dsiAccess(ind);

    inline proc dsiAccess(ind: idxType ...1) const ref
    where rank == 1 && shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(ind);

    inline proc dsiAccess(ind : rank*idxType) ref {
      // Note: bounds checking occurs in ChapelArray for this type.
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }

    inline proc dsiAccess(ind : rank*idxType)
    where shouldReturnRvalueByValue(eltType) {
      // Note: bounds checking occurs in ChapelArray for this type.
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }

    inline proc dsiAccess(ind : rank*idxType) const ref
    where shouldReturnRvalueByConstRef(eltType) {
      // Note: bounds checking occurs in ChapelArray for this type.
      var dataInd = getDataIndex(ind);
      return theData(dataInd);
    }


    inline proc dsiLocalAccess(i) ref
      return dsiAccess(i);

    inline proc dsiLocalAccess(i)
    where shouldReturnRvalueByValue(eltType)
      return dsiAccess(i);

    inline proc dsiLocalAccess(i) const ref
    where shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(i);

    inline proc dsiBoundsCheck(i) {
      return dom.dsiMember(i);
    }

    proc adjustBlkOffStrForNewDomain(d: unmanaged DefaultRectangularDom,
                                     alias: unmanaged DefaultRectangularArr)
    {
      for param i in 1..rank {
        var s: idxType;
        // NOTE: Not bothering to check to see if this can fit into idxType
        if idxSignedType==idxType {
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

    proc adjustBlkOffStrForNewDomain(d: unmanaged DefaultRectangularDom,
                                     alias: unmanaged DefaultRectangularArr)
      where dom.stridable == false && this.stridable == false
    {
      for param i in 1..rank {
        alias.off(i) = d.dsiDim(i).low;
        alias.blk(i) = blk(i);
        alias.str(i) = d.dsiDim(i).stride;
      }
    }

    pragma "ignore transfer errors"
    override proc dsiReallocate(allocBound: range(idxType,
                                                  BoundedRangeType.bounded,
                                                  stridable),
                                arrayBound: range(idxType,
                                                  BoundedRangeType.bounded,
                                                  stridable)) where rank == 1 {
      on this {
        const allocD = {allocBound};
        var copy = new unmanaged DefaultRectangularArr(eltType=eltType,
                                                       rank=rank,
                                                       idxType=idxType,
                                                       stridable=allocD._value.stridable,
                                                       dom=allocD._value);

        forall i in arrayBound(dom.ranges(1)) do
          copy.dsiAccess(i) = dsiAccess(i);

        off = copy.off;
        blk = copy.blk;
        str = copy.str;
        factoredOffs = copy.factoredOffs;

        dsiDestroyArr();
        data = copy.data;
        // We can't call initShiftedData here because the new domain
        // has not yet been updated (this is called from within the
        // = function for domains.
        if earlyShiftData && !allocD._value.stridable {
          // Lydia note 11/04/15: a question was raised as to whether this
          // check on numIndices added any value.  Performance results
          // from removing this line seemed inconclusive, which may indicate
          // that the check is not necessary, but it seemed like unnecessary
          // work for something with no immediate reward.
          if allocD.numIndices > 0 {
            shiftedData = copy.shiftedData;
          }
        }
        delete copy;
      }
    }


    // Reallocate the array to have space for elements specified by `bounds`
    pragma "ignore transfer errors"
    override proc dsiReallocate(bounds: rank*range(idxType,
                                                   BoundedRangeType.bounded,
                                                   stridable)) {
      on this {
        const allocD = {(...bounds)};

        var copy = new unmanaged DefaultRectangularArr(eltType=eltType,
                                            rank=rank,
                                            idxType=idxType,
                                            stridable=allocD._value.stridable,
                                            dom=allocD._value);

        forall i in allocD((...dom.ranges)) do
          copy.dsiAccess(i) = dsiAccess(i);

        off = copy.off;
        blk = copy.blk;
        str = copy.str;
        factoredOffs = copy.factoredOffs;

        dsiDestroyArr();
        data = copy.data;
        // We can't call initShiftedData here because the new domain
        // has not yet been updated (this is called from within the
        // = function for domains.
        if earlyShiftData && !allocD._value.stridable {
          // Lydia note 11/04/15: a question was raised as to whether this
          // check on numIndices added any value.  Performance results
          // from removing this line seemed inconclusive, which may indicate
          // that the check is not necessary, but it seemed like unnecessary
          // work for something with no immediate reward.
          if allocD.numIndices > 0 {
            shiftedData = copy.shiftedData;
          }
        }
        delete copy;
      }
    }

    override proc dsiPostReallocate() {
      // No action necessary here
    }

    proc dsiLocalSlice(ranges) {
      halt("all dsiLocalSlice calls on DefaultRectangulars should be handled in ChapelArray.chpl");
    }

    proc dsiGetRAD() {
      var rad: _remoteAccessData(eltType, rank, idxType, stridable);
      rad.off = off;
      rad.blk = blk;
      rad.str = str;
      rad.origin = 0;
      rad.factoredOffs = factoredOffs;
      rad.data = data;
      rad.shiftedData = shiftedData;
      return rad;
    }

    proc dsiTargetLocales() {
      return [this.data.locale, ];
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.data.locale == loc {
        return _getDomain(dom);
      } else {
        var a: domain(rank, idxType, stridable);
        return a;
      }
    }

    iter dsiLocalSubdomains(loc: locale) {
      yield dsiLocalSubdomain(loc);
    }
  }

  iter chpl__serialViewIter(arr, viewDom) ref
    where chpl__isDROrDRView(arr) {
    param useCache = chpl__isArrayView(arr) && arr.shouldUseIndexCache();
    var info = if useCache then arr.indexCache
               else if arr.isSliceArrayView() then arr.arr
               else arr;
    if arr.rank == 1 {
      // This is specialized to avoid overheads of calling dsiAccess()
      if !viewDom.stridable {
        // Ideally we would like to be able to do something like
        // "for i in first..last by step". However, right now that would
        // result in a strided iterator which isn't as optimized. It would
        // also add a range initializer, which in tight loops is pretty
        // expensive. Instead we use a direct range iterator that is
        // optimized for positively strided ranges. It should be just as fast
        // as directly using a "c for loop", but it contains code check for
        // overflow and invalid strides as well as the ability to use a less
        // optimized iteration method if users are concerned about range
        // overflow.

        const first  = info.getDataIndex(viewDom.dsiLow);
        const second = info.getDataIndex(viewDom.chpl_intToIdx(chpl__idxToInt(viewDom.dsiLow)+1));
        const step   = (second-first);
        const last   = first + (viewDom.dsiNumIndices-1) * step;
        for i in chpl_direct_pos_stride_range_iter(first, last, step) {
          yield info.theData(i);
        }
      } else {
        const viewDomDim = viewDom.dsiDim(1),
              stride = viewDomDim.stride: viewDom.intIdxType,
              start  = viewDomDim.first,
              second = info.getDataIndex(viewDom.chpl_intToIdx(viewDomDim.firstAsInt + stride));

        var   first  = info.getDataIndex(start);
        const step   = (second-first):chpl__signedType(viewDom.intIdxType);
        var   last   = first + (viewDomDim.size-1) * step:viewDom.intIdxType;

        if step < 0 then
          last <=> first;

        var data = info.theData;
        for i in first..last by step do
          yield data(i);
      }
    } else if useCache {
      for i in viewDom {
        const dataIdx = info.getDataIndex(i);
        yield info.getDataElem(dataIdx);
      }
    } else {
      for elem in chpl__serialViewIterHelper(arr, viewDom) do yield elem;
    }
  }

  iter chpl__serialViewIter(arr, viewDom) ref {
    for elem in chpl__serialViewIterHelper(arr, viewDom) do yield elem;
  }

  iter chpl__serialViewIterHelper(arr, viewDom) ref {
    for i in viewDom {
      const dataIdx = if arr.isReindexArrayView() then chpl_reindexConvertIdx(i, arr.dom, arr.downdom)
                      else if arr.isRankChangeArrayView() then chpl_rankChangeConvertIdx(i, arr.collapsedDim, arr.idx)
                      else i;
      const info = if chpl__isArrayView(arr) then arr.arr else arr;
      yield info.dsiAccess(dataIdx);
    }
  }

  proc DefaultRectangularDom.dsiSerialReadWrite(f /*: Reader or Writer*/) throws {
    f <~> new ioLiteral("{") <~> ranges(1);
    for i in 2..rank do
      f <~> new ioLiteral(", ") <~> ranges(i);
    f <~> new ioLiteral("}");
  }

  proc DefaultRectangularDom.dsiSerialWrite(f) throws { this.dsiSerialReadWrite(f); }
  proc DefaultRectangularDom.dsiSerialRead(f) throws { this.dsiSerialReadWrite(f); }

  proc DefaultRectangularArr.dsiSerialReadWrite(f /*: Reader or Writer*/) throws {
    chpl_serialReadWriteRectangular(f, this);
  }

  // Why can the following two functions not be collapsed into one
  // where 'dom = arr.dom'?  Because this puts a type constraint on
  // what 'dom' can be passed that is too strict in some callchains
  // (e.g., if arr.dom is non-stridable but the 'dom' passed in is
  // stridable).
  //
  proc chpl_serialReadWriteRectangular(f, arr) throws {
    chpl_serialReadWriteRectangular(f, arr, arr.dom);
  }

  // ReplicatedDist declares a version of this method with a 'where'
  // clause, but current resolution rules prefer the more visible
  // function (rather than the one with a 'where' clause) and this
  // call is more visible (e.g. ArrayViewSlice uses DefaultRectangular
  // but not ReplicatedDist). Applying "last resort" to this function
  // requests the compiler prefer another overload if one is applicable.
  // Overload sets (or a similar idea) would be a better user-facing
  // way to solve this problem (see CHIP 20).
  pragma "last resort"
  proc chpl_serialReadWriteRectangular(f, arr, dom) throws {
    chpl_serialReadWriteRectangularHelper(f, arr, dom);
  }

  proc chpl_serialReadWriteRectangularHelper(f, arr, dom) throws {
    param rank = arr.rank;
    type idxType = arr.idxType;
    type idxSignedType = chpl__signedType(chpl__idxTypeToIntIdxType(idxType));

    const isNative = f.styleElement(QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER): bool;

    proc writeSpaces(dim:int) throws {
      for i in 1..dim {
        f <~> new ioLiteral(" ");
      }
    }

    proc recursiveArrayWriter(in idx: rank*idxType, dim=1, in last=false) throws {
      var binary = f.binary();
      var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
      var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

      type strType = idxSignedType;
      var makeStridePositive = if dom.dsiDim(dim).stride > 0 then 1:strType else (-1):strType;

      if isjson || ischpl {
        if dim != rank {
          f <~> new ioLiteral("[\n");
          writeSpaces(dim); // space for the next dimension
        } else f <~> new ioLiteral("[");
      }

      if dim == rank {
        var first = true;
        if debugDefaultDist && f.writing then f.writeln(dom.dsiDim(dim));
        for j in dom.dsiDim(dim) by makeStridePositive {
          if first then first = false;
          else if isspace then f <~> new ioLiteral(" ");
          else if isjson || ischpl then f <~> new ioLiteral(", ");
          idx(dim) = j;
          f <~> arr.dsiAccess(idx);
        }
      } else {
        for j in dom.dsiDim(dim) by makeStridePositive {
          var lastIdx =  dom.dsiDim(dim).last;
          idx(dim) = j;

          recursiveArrayWriter(idx, dim=dim+1,
                               last=(last || dim == 1) && (j == dom.dsiDim(dim).alignedHigh));

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
       rank == 1 && dom.dsiDim(1).stride == 1 &&
       dom._arrs.size == 1 {

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

      var offset = dom.dsiDim(1).low;
      var i = 0;

      var read_end = false;

      while true {
        if first {
          first = false;
          // but check for a ]
          try {
            if isjson || ischpl {
              f <~> new ioLiteral("]");
            } else if isspace {
              f <~> new ioNewline(skipWhitespaceOnly=true);
            }
            read_end = true;
            break;
          } catch err: BadFormatError {
            // Continue on if we didn't read a closing bracket.
          }
        } else {

          // Try reading a comma/space. Break if we don't read one.
          try {
            if isspace then f <~> new ioLiteral(" ");
            else if isjson || ischpl then f <~> new ioLiteral(",");
          } catch err: BadFormatError {
            break;
          } 
        }

        if i >= dom.dsiDim(1).size {
          // Create more space.
          var sz = dom.dsiDim(1).size;
          if sz < 4 then sz = 4;
          sz = 2 * sz;

          // like push_back
          const newDom = {offset..#sz};

          arr.dsiReallocate( newDom );
          // This is different from how push_back does it
          // because push_back might lead to a call to
          // _reprivatize but I don't see how to do that here.
          dom.dsiSetIndices( newDom.getIndices() );
          arr.dsiPostReallocate();
        }

        f <~> arr.dsiAccess(offset + i);

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
        arr.dsiReallocate( newDom );
        dom.dsiSetIndices( newDom.getIndices() );
        arr.dsiPostReallocate();
      }

    } else if arr.isDefaultRectangular() && !chpl__isArrayView(arr) &&
              _isSimpleIoType(arr.eltType) && f.binary() &&
              isNative && arr.isDataContiguous(dom) {
      // If we can, we would like to read/write the array as a single write op
      // since _ddata is just a pointer to the memory location we just pass
      // that along with the size of the array. This is only possible when the
      // byte order is set to native or its equivalent.
      const elemSize = c_sizeof(arr.eltType);
      if boundsChecking {
        var rw = if f.writing then "write" else "read";
        assert((dom.dsiNumIndices:uint*elemSize:uint) <= max(ssize_t):uint,
               "length of array to ", rw, " is greater than ssize_t can hold");
      }

      const len = dom.dsiNumIndices;
      const src = arr.theData;
      const idx = arr.getDataIndex(dom.dsiLow);
      const size = len:ssize_t*elemSize:ssize_t;
      try {
        if f.writing {
          f.writeBytes(_ddata_shift(arr.eltType, src, idx), size);
        } else {
          f.readBytes(_ddata_shift(arr.eltType, src, idx), size);
        }
      } catch err {
        // Setting errors in channels has no effect, so just rethrow.
        throw err;
      }
    } else {
      const zeroTup: rank*idxType;
      recursiveArrayWriter(zeroTup);
    }
  }

  proc DefaultRectangularArr.dsiSerialWrite(f) throws {
    dsiSerialReadWrite(f);
  }

  proc DefaultRectangularArr.dsiSerialRead(f) throws {
    dsiSerialReadWrite(f);
  }

  // This is very conservative.
  proc DefaultRectangularArr.isDataContiguous(dom) {
    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("isDataContiguous(): off=", off, " blk=", blk);

    if blk(rank) != 1 then return false;

    for param dim in 1..(rank-1) by -1 do
      if blk(dim) != blk(dim+1)*dom.dsiDim(dim+1).size then return false;

    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("\tYES!");

    return true;
  }

  private proc _canDoSimpleTransfer(A, aView, B, bView) {
    if !A.isDataContiguous(aView._value) || !B.isDataContiguous(bView._value) {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("isDataContiguous return False");
      return false;
    }
    return true;
  }

  private proc _canDoComplexTransfer(A, aView, B, bView) {
    return useBulkTransferStride;
  }

  proc DefaultRectangularArr.doiCanBulkTransferRankChange() param return true;

  proc DefaultRectangularArr.doiBulkTransferToKnown(srcDom, destClass:DefaultRectangularArr, destDom) : bool {
    return transferHelper(destClass, destDom, this, srcDom);
  }

  proc DefaultRectangularArr.doiBulkTransferFromKnown(destDom, srcClass:DefaultRectangularArr, srcDom) : bool {
    return transferHelper(this, destDom, srcClass, srcDom);
  }

  private proc transferHelper(A, aView, B, bView) : bool {
    if A.rank == B.rank &&
       (aView.stridable == false && bView.stridable == false) &&
       _canDoSimpleTransfer(A, aView, B, bView) {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("Performing simple DefaultRectangular transfer");

      _simpleTransfer(A, aView, B, bView);
    } else if _canDoComplexTransfer(A, aView, B, bView) {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("Performing complex DefaultRectangular transfer");

      complexTransfer(A, aView, B, bView);
    } else {
      return false;
    }

    return true;
  }

  private proc _simpleTransfer(A, aView, B, bView) {
    param rank     = A.rank;
    type idxType   = A.idxType;

    const Adims = aView.dims();
    var Alo: rank*aView.idxType;
    for param i in 1..rank do
      Alo(i) = Adims(i).first;

    const Bdims = bView.dims();
    var Blo: rank*B.idxType;
    for param i in 1..rank do
      Blo(i) = Bdims(i).first;

    const len = aView.numIndices.safeCast(size_t);

    if len == 0 then return;

    if debugBulkTransfer {
      pragma "fn synchronization free"
      extern proc sizeof(type x): int;
      const elemSize =sizeof(B.eltType);
      chpl_debug_writeln("In DefaultRectangular._simpleTransfer():",
              " Alo=", Alo, ", Blo=", Blo,
              ", len=", len, ", elemSize=", elemSize);
    }

    const Aidx = A.getDataIndex(Alo);
    const Adata = _ddata_shift(A.eltType, A.theData, Aidx);
    const Bidx = B.getDataIndex(Blo);
    const Bdata = _ddata_shift(B.eltType, B.theData, Bidx);
    _simpleTransferHelper(A, B, Adata, Bdata, len);
  }

  private proc _simpleTransferHelper(A, B, Adata, Bdata, len) {
    if Adata == Bdata then return;

    // NOTE: This does not work with --heterogeneous, but heterogeneous
    // compilation does not work right now.  The calls to chpl_comm_get
    // and chpl_comm_put should be changed once that is fixed.
    if Adata.locale.id==here.id {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("\tlocal get() from ", B.locale.id);
      __primitive("chpl_comm_array_get", Adata[0], Bdata.locale.id, Bdata[0], len);
    } else if Bdata.locale.id==here.id {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("\tlocal put() to ", A.locale.id);
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

  This function transfers B into A, where A and B are both
  DefaultRectangularArr classes.

  `aView` and `bView` represent the indices to be transferred to/from. They
  might be different if this is a transfer involving an Array View.

  Assumes row-major ordering.

  Depends on adjustBlkOffStrForNewDomain having been called in ChapelArray
  before entering this function.

  TODO: Determine if we can reduce the communication for array metadata

  TODO: Pull simple runtime implementation up into module code
  */
  private proc complexTransfer(A, aView, B, bView) {
    if (A.data.locale.id != here.id &&
        B.data.locale.id != here.id) {
      if debugDefaultDistBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: Both arrays on different locale, moving to locale of destination: LOCALE", A.data.locale.id);
      }
      on A.data do
        complexTransferCore(A, aView, B, bView);
    } else {
      complexTransferCore(A, aView, B, bView);
    }
  }


  private proc complexTransferCore(LHS, LViewDom, RHS, RViewDom) {
    param minRank = min(LHS.rank, RHS.rank);
    type  idxType = LHS.idxType;
    type  intIdxType = LHS.intIdxType;

    if debugDefaultDistBulkTransfer {
      writeln("Transferring views :", LViewDom, " <-- ", RViewDom);
      writeln("Original domains   :", LHS.dom.dsiDims(), " <-- ", RHS.dom.dsiDims());
    }

    const LeftDims  = LViewDom.dims();
    const RightDims = RViewDom.dims();

    const (LeftActives, RightActives, inferredRank) = bulkCommComputeActiveDims(LeftDims, RightDims);

    var DimSizes: [1..inferredRank] LeftDims(1).size.type;
    for i in 1..inferredRank {
      const dimIdx = LeftActives(i);
      DimSizes[i] = LeftDims(dimIdx).size;
    }

    if debugDefaultDistBulkTransfer {
      writeln("inferredRank = ", inferredRank);
    }
    assert(inferredRank <= minRank, "complex DR transfer: computed rank greater than minimum rank!");

    // Compute a 'blk' tuple for the LHS and RHS based on their view-domains
    var LBlk, RBlk : minRank*intIdxType;

    {
      // For each array, compute a valid 'blk' with 'inferredRank' values
      // over the array's original data by skipping over rank-changed dims.
      for idx in 1..inferredRank by -1 {
        const li = LeftActives(idx);
        LBlk(idx) = LHS.blk(li) * (LeftDims(li).stride / LHS.dom.dsiDim(li).stride):intIdxType;

        const ri = RightActives(idx);
        RBlk(idx) = RHS.blk(ri) * (RightDims(ri).stride / RHS.dom.dsiDim(ri).stride):intIdxType;
      }
    }

    if debugDefaultDistBulkTransfer {
      writeln("LBlk = ", LBlk);
      writeln("RBlk = ", RBlk);
    }

    // The number of values needed to express the strided region.
    var stridelevels = 0;

    //
    // `count[1]` is the number of elements we can bulk-copy at a time. Each
    // number after that represents the number of times we need to stride at
    // each level. It will ultimately be an array of size `stridelevels+1`.
    //
    var countDom = {1..inferredRank+1};
    var count : [countDom] size_t;
    for c in count do c = 1; // serial to avoid task creation overhead

    //
    // The arrays representing the required stride at each level. Should pull
    // from the values in the 'blk' tuple, though we may skip a dimension if
    // it can be aggregated. Will ultimately be of size `stridelevels`.
    //
    var strideDom = {1..inferredRank};
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
    if LBlk(inferredRank) > 1 || RBlk(inferredRank) > 1 {
      stridelevels           += 1;
      count[stridelevels]     = 1;
      dstStride[stridelevels] = LBlk(inferredRank).safeCast(size_t);
      srcStride[stridelevels] = RBlk(inferredRank).safeCast(size_t);
    }

    //
    // When at the end of the elements in dimension `i`, `canReuseStride` tells
    // us whether or not we can re-use the current stride value to advance to
    // the next chunk of elements. If either array is unable to re-use its
    // stride, then we need a new stride value.
    //
    for i in 2..inferredRank by -1 {
      // Each corresponding dimension in A and B should have the same length,
      // so it doesn't matter which we use here.
      count[stridelevels+1] *= DimSizes(i).safeCast(size_t);

      const bothReuse = canReuseStride(LBlk, i, stridelevels, count, dstStride) &&
                        canReuseStride(RBlk, i, stridelevels, count, srcStride);

      if !bothReuse {
        stridelevels += 1;
        dstStride[stridelevels] = LBlk(i-1).safeCast(size_t);
        srcStride[stridelevels] = RBlk(i-1).safeCast(size_t);
      }
    }
    count[stridelevels+1] *= DimSizes(1).safeCast(size_t);

    assert(stridelevels <= inferredRank, "BulkTransferStride: stride levels greater than rank.");
    if stridelevels == 0 then assert(count[1] == LViewDom.numIndices, "BulkTransferStride: bulk-count incorrect for stride level of 0: ", count[1], " != ", LViewDom.numIndices);

    countDom  = {1..stridelevels+1};
    strideDom = {1..stridelevels};

    proc getFirstIdx(dims) {
      var ret : dims.size * dims(1).idxType;
      for param i in 1..dims.size do
        ret(i) = if dims(i).stride < 0 then dims(i).last else dims(i).first;
      return ret;
    }

    // Build up the index tuples to calculate the offset for the first element
    const LFirst = getFirstIdx(LeftDims);
    const RFirst = getFirstIdx(RightDims);

    complexTransferComm(LHS, RHS, stridelevels:int(32), dstStride, srcStride, count, LFirst, RFirst);
  }

  //
  // Invoke the primitives chpl_comm_get_strd/puts, depending on what locale we
  // are on vs. where the source and destination are.
  //
  private proc complexTransferComm(A, B, stridelevels:int(32), dstStride, srcStride, count, AFirst, BFirst) {
    if debugDefaultDistBulkTransfer {
      chpl_debug_writeln("BulkTransferStride with values:\n",
                         "\tLocale        = ", stringify(here.id), "\n",
                         "\tStride levels = ", stringify(stridelevels), "\n",
                         "\tdstStride     = ", stringify(dstStride), "\n",
                         "\tsrcStride     = ", stringify(srcStride), "\n",
                         "\tcount         = ", stringify(count));
    }

    const AO = A.getDataIndex(AFirst, getShifted = false);
    const BO = B.getDataIndex(BFirst, getShifted = false);

    const dest = A.data;
    const src  = B.data;

    const dststr = dstStride._value.data;
    const srcstr = srcStride._value.data;
    const cnt    = count._value.data;

    if dest.locale.id == here.id {
      const srclocale = src.locale.id : int(32);

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
        assert(src.locale.id == here.id,
               "BulkTransferStride: Expected to be on ", src.locale.id,
               ", actually on ", here.id);
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
  proc type DefaultRectangularArr.isDefaultRectangular() param return true;

  config param debugDRScan = false;

  /* This computes a 1D scan in parallel on the array, for 1D arrays only */
  proc DefaultRectangularArr.doiScan(op, dom) where (rank == 1) &&
                                                chpl__scanStateResTypesMatch(op) {
    use RangeChunk;

    type resType = op.generate().type;
    var res: [dom] resType;

    // Take first pass, computing per-task partial scans, stored in 'state'
    var (numTasks, rngs, state, _) = this.chpl__preScan(op, res, dom);

    // Take second pass updating result based on the scanned 'state'
    this.chpl__postScan(op, res, numTasks, rngs, state);

    // Clean up and return
    delete op;
    return res;
  }

  // A helper routine to take the first parallel scan over a vector
  // yielding the number of tasks used, the ranges computed by each
  // task, and the scanned results of each task's scan.  This is
  // broken out into a helper function in order to be made use of by
  // distributed array scans.
  proc DefaultRectangularArr.chpl__preScan(op, res: [] ?resType, dom) {
    import RangeChunk;
    // Compute who owns what
    const rng = dom.dim(1);
    const numTasks = if __primitive("task_get_serial") then
                      1 else _computeNumChunks(rng.size);
    const rngs = RangeChunk.chunks(rng, numTasks);
    if debugDRScan {
      writeln("Using ", numTasks, " tasks");
      writeln("Whose chunks are: ", rngs);
    }

    var state: [1..numTasks] resType;

    // Take first pass over data doing per-chunk scans

    // optimize for the single-task case
    if numTasks == 1 {
      preScanChunk(1);
    } else {
      coforall tid in 1..numTasks {
        preScanChunk(tid);
      }
    }

    proc preScanChunk(tid) {
      const current: resType;
      const myop = op.clone();
      for i in rngs[tid] {
        ref elem = dsiAccess(i);
        myop.accumulate(elem);
        res[i] = myop.generate();
      }
      state[tid] = res[rngs[tid].high];
      delete myop;
    }
    if debugDRScan {
      writeln("res = ", res);
      writeln("state = ", state);
    }

    // Scan state vector itself
    const metaop = op.clone();
    var next: resType = metaop.identity;
    for i in 1..numTasks {
      state[i] <=> next;
      metaop.accumulateOntoState(next, state[i]);
    }
    delete metaop;
    if debugDRScan then
      writeln("state = ", state);

    return (numTasks, rngs, state, next);
  }

  // A second helper routine that does the second parallel pass over
  // the result vector adding the prefix state computed by the earlier
  // tasks.  This is broken out into a helper function in order to be
  // made use of by distributed array scans.
  proc DefaultRectangularArr.chpl__postScan(op, res, numTasks, rngs, state) {
    // optimize for the single-task case
    if numTasks == 1 {
      postScanChunk(1);
    } else {
      coforall tid in 1..numTasks {
        postScanChunk(tid);
      }
    }

    proc postScanChunk(tid) {
      const myadjust = state[tid];
      for i in rngs[tid] {
        op.accumulateOntoState(res[i], myadjust);
      }
    }
    
    if debugDRScan then
      writeln("res = ", res);
  }



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
  private proc canReuseStride(blk, curDim: int, levels, count, stride)
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

}
