/*
 * Copyright 2004-2017 Cray Inc.
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
  config const dataParIgnoreRunningTasks = if CHPL_LOCALE_MODEL!="flat" then true
                                           else false;
  config const dataParMinGranularity: int = 1;

  if dataParTasksPerLocale<0 then halt("dataParTasksPerLocale must be >= 0");
  if dataParMinGranularity<=0 then halt("dataParMinGranularity must be > 0");

  use DSIUtil, ChapelArray;
  config param debugDefaultDist = false;
  config param debugDefaultDistBulkTransfer = false;
  config param debugDataPar = false;
  config param debugDataParNuma = false;
  config param debugDataParMultiDData = false;

  config param defaultDoRADOpt = true;
  config param defaultDisableLazyRADOpt = false;
  config param earlyShiftData = true;

  config param defRectArrMultiDDataSizeThreshold = 2**20;

  // Used for the size of 'mData' tuples. Hopefully one day this
  // will be a query supported by the LocaleModel.
  config param experimentalMaxSublocales = 4;

  inline proc defRectSimpleDData param return !localeModelHasSublocales;

  // helper function to set the types of multi-ddata specific fields
  // to 'void' when they are not needed
  proc mdType(type baseType) type {
    if defRectSimpleDData then
      return void;
    else
      return baseType;
  }

  class DefaultDist: BaseDist {
    proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
      const dom = new DefaultRectangularDom(rank, idxType, stridable, this);
      dom.dsiSetIndices(inds);
      return dom;
    }

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

    proc isDefaultRectangular() param return true;

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
      where tag == iterKind.standalone && defRectSimpleDData {
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

    proc dsiLocalSlice(ranges) {
      halt("all dsiLocalSlice calls on DefaultRectangulars should be handled in ChapelArray.chpl");
    }

    proc dsiTargetLocales() {
      return [this.locale, ];
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      if (this.locale == here) {
        return _getDomain(this);
      } else {
        var a: domain(rank, idxType, stridable);
        return a;
      }
    }

    iter dsiLocalSubdomains() {
      yield dsiLocalSubdomain();
    }
  }

  record _multiData {
    type eltType;
    type idxType;
    var pdr: range(idxType,BoundedRangeType.bounded,true);
    var dataOff: idxType;
    //
    // I would like to leave these pragmas here, in the belief that they
    // should do the same good as they do in DefaultRectangularArr.  But
    // when uncommented they cause this error in quite a few tests:
    //   error: Attempted to assign to local class field with remote class
    // Example: arrays/deitz/test_block_array_of_syncs with numa+gasnet
    //
    //pragma "local field"
    var data : _ddata(eltType);
    //pragma "local field"
    var shiftedData : _ddata(eltType);
  };

  // TODO: should this include the ranges that represent the domain?
  record _remoteAccessData {
    type eltType;
    param rank : int;
    type idxType;
    param stridable: bool;
    param blkChanged : bool = false;

    var off: rank*idxType;
    var blk: rank*idxType;
    var str: rank*chpl__signedType(idxType);
    var origin: idxType;
    var factoredOffs: idxType;

    var data: _ddata(eltType);
    var shiftedData: _ddata(eltType);

    inline proc oneDData return defRectSimpleDData || mdNumChunks < 2;

    var mdParDim: mdType(int);
    var mdNumChunks: mdType(int);
    var mdRLo: mdType(idxType);
    var mdRHi: mdType(idxType);
    var mdRStr: mdType(idxType);
    var mdRLen: mdType(idxType);
    var mdBlk: mdType(idxType);
    var mdAlias: mdType(bool);

    var mData : mdType(experimentalMaxSublocales*_multiData(eltType=eltType,
                                                     idxType=idxType));

    inline proc dataChunk(i) ref {
      if defRectSimpleDData then
        return data;
      else {
        return mData(i+1).data;
      }
    }

    inline proc shiftedDataChunk(i) ref {
      if defRectSimpleDData then
        return shiftedData;
      else {
        return mData(i+1).shiftedData;
      }
    }

    inline proc theDataChunk(i) ref {
      if stridable {
        return dataChunk(i);
      } else {
        return shiftedDataChunk(i);
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
      if defRectSimpleDData then
        return data(i);
      else {
        return mData(i(1)+1).data(i(2));
      }
    }

    inline proc shiftedDataElem(i) ref where defRectSimpleDData
      return shiftedData(i);

    inline proc shiftedDataElem(i) ref where !defRectSimpleDData {
      return mData(i(1) + 1).shiftedData(i(2));
    }

    // duplicates DefaultRectangularArr.mdInd2Chunk except for mdBlk,
    // handling alias with different mdParDim'th stride than original 
    inline proc mdInd2Chunk(ind)
      where !defRectSimpleDData {
      if stridable then
        return (((ind - mdRLo) / mdRStr * mdBlk * mdNumChunks:idxType)
                / mdRLen):int;
      else
        return (((ind - mdRLo) * mdBlk * mdNumChunks:idxType) / mdRLen):int;
    }

    inline proc mdChunk2Ind(chunk)
      where !defRectSimpleDData {
      if stridable {
        var (lo, hi) = _computeBlock(mdRLen, mdNumChunks, chunk,
                                     (mdRHi - mdRLo) / mdRStr, 0, 0);
        lo = lo * mdRStr + mdRLo;
        hi = hi * mdRStr + mdRLo;
        return (lo, hi);
      } else {
        return _computeBlock(mdRLen, mdNumChunks, chunk, mdRHi, mdRLo, mdRLo);
      }
    }
  }

  inline proc _remoteAccessData.getDataIndex(ind : idxType,
                                             param getChunked=!defRectSimpleDData) {
    return this.getDataIndex(chpl__tuplify(ind), getChunked);
  }

  //
  // Copied from DefaultRectangularArr.getDataIndex
  //
  inline proc _remoteAccessData.getDataIndex(ind: rank*idxType,
                                             param getChunked=!defRectSimpleDData) {
      param chunkify = !defRectSimpleDData && getChunked;

      if stridable {
        inline proc chunked_dataIndex(sum, str) {
          if mdNumChunks == 1 {
            return (0, sum);
          } else {
            const chunk = mdInd2Chunk(ind(mdParDim));
            return (chunk, sum);
          }
        }

        var sum = origin;
        for param i in 1..rank do
          sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
        if chunkify then
          return chunked_dataIndex(sum, str=abs(str(mdParDim)):idxType);
        else
          return sum;
      } else {
        inline proc chunked_dataIndex(sum) {
          if mdNumChunks == 1 {
            return (0, sum);
          } else {
            const chunk = mdInd2Chunk(ind(mdParDim));
            return (chunk, sum);
          }
        }

        // optimize common case to get cleaner generated code
        if (rank == 1 && earlyShiftData) {
          if blkChanged {
            if chunkify then
              return chunked_dataIndex(ind(1) * blk(1));
            else
              return ind(1) * blk(1);
          } else {
            if chunkify then
              return chunked_dataIndex(ind(1));
            else
              return ind(1);
          }
        } else {
          var sum = if earlyShiftData then 0:idxType else origin;

          if blkChanged {
            for param i in 1..rank {
              sum += ind(i) * blk(i);
            }
          } else {
            for param i in 1..rank-1 {
              sum += ind(i) * blk(i);
            }
            sum += ind(rank);
          }

          if !earlyShiftData then sum -= factoredOffs;
          if chunkify then
            return chunked_dataIndex(sum);
          else
            return sum;
        }
      }
  }

  proc _remoteAccessData.computeFactoredOffs() {
    factoredOffs = 0;
    for param i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }

  proc _remoteAccessData.initShiftedData() {
    if earlyShiftData && !stridable {
      type idxSignedType = chpl__signedType(idxType);
      const shiftDist = if isIntType(idxType) then origin - factoredOffs
                        else origin:idxSignedType - factoredOffs:idxSignedType;
      if defRectSimpleDData {
        shiftedData = _ddata_shift(eltType, data, shiftDist);
      } else {
        for i in 1..#mdNumChunks {
          mData(i).shiftedData = _ddata_shift(eltType, mData(i).data, shiftDist);
        }
      }
    }
  }

  proc _remoteAccessData.strideAlignUp(lo, r)
    return r.low + (lo - r.low + abs(r.stride):idxType - 1)
           / abs(r.stride):idxType * abs(r.stride):idxType;

  proc _remoteAccessData.strideAlignDown(hi, r)
    return hi - (hi - r.low) % abs(r.stride):idxType;

  proc _remoteAccessData.initDataFrom(other : _remoteAccessData) {
    if defRectSimpleDData {
      this.data = other.data;
    } else {
      if other.mdNumChunks > experimentalMaxSublocales {
        halt("mdNumChunks > experimentalMaxSublocales: ", mdNumChunks, " > ", experimentalMaxSublocales);
      }
      for i in 1..#other.mdNumChunks {
        this.mData(i).dataOff = other.mData(i).dataOff;
        this.mData(i).data    = other.mData(i).data;
      }
    }
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
      const shift = this.blk(i) * (newDom.dsiDim(i).low - this.off(i)) / abs(this.str(i)) : rad.idxType;
      if this.str(i) > 0 {
        rad.origin += shift;
      } else {
        rad.origin -= shift;
      }

      const mult = (newDom.dsiDim(i).stride / this.str(i)) : rad.idxType;
      rad.blk(i) = this.blk(i) * mult;
    }

    if !defRectSimpleDData {
      rad.mdParDim    = this.mdParDim;
      rad.mdNumChunks = this.mdNumChunks;
      rad.mdRLo       = this.mdRLo;
      rad.mdRHi       = this.mdRHi;
      rad.mdRStr      = this.mdRStr;
      rad.mdRLen      = this.mdRLen;
      rad.mdBlk       = this.mdBlk;

      for i in 1..#mdNumChunks {
        var low = max(this.mData(i).pdr.low, newDom.dsiDim(mdParDim).low);
        low = if rad.stridable then strideAlignUp(low, newDom.dsiDim(mdParDim)) else low;

        var high = min(this.mData(i).pdr.high, newDom.dsiDim(mdParDim).high);
        high = if rad.stridable then strideAlignDown(high, newDom.dsiDim(mdParDim)) else high;

        const rng = low..high;
        rad.mData(i).pdr = if !rad.stridable then rng else rng by newDom.dsiDim(mdParDim).stride align newDom.dsiDim(mdParDim).alignment;
      }

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
    rad.origin       = this.origin:newDom.idxType;
    rad.blk          = this.blk;
    rad.off          = chpl__tuplify(newDom.dsiLow);
    rad.str          = chpl__tuplify(newDom.dsiStride);
    rad.factoredOffs = 0:idxType;

    if !defRectSimpleDData {
      rad.mdParDim    = this.mdParDim;
      rad.mdNumChunks = this.mdNumChunks;

      const thisStr   = abs(this.str(mdParDim)):idxType;
      const radStr    = abs(rad.str(mdParDim)):idxType;

      rad.mdRLo       = rad.off(mdParDim) - (this.off(mdParDim) - this.mdRLo) / thisStr * radStr;
      rad.mdRHi       = rad.off(mdParDim) + (this.mdRLen - 1) * radStr;
      rad.mdRStr      = abs(rad.str(mdParDim)):rad.idxType;
      rad.mdRLen      = this.mdRLen;
      rad.mdBlk       = thisStr / radStr;

      const thisLo    = this.off(mdParDim);
      const radLo     = rad.off(mdParDim);
      for i in 1..#mdNumChunks {
        var low = (this.mData(i).pdr.low - thisLo) / thisStr;
        low = if rad.stridable then low * radStr else low;
        low += radLo;

        var high = (this.mData(i).pdr.high - thisLo) /thisStr;
        high = if rad.stridable then high * radStr else high;
        high += radLo;

        const rng = low..high;
        rad.mData(i).pdr = if !rad.stridable then rng else rng by radStr align newDom.dsiDim(mdParDim).alignment;
      }
    }

    rad.computeFactoredOffs();
    rad.initShiftedData();

    return rad;
  }

  //
  // Based on the old 'dsiRankChange' method
  //
  proc _remoteAccessData.toRankChange(newDom, cd, idx) {
    compilerAssert(this.rank == idx.size && this.rank != newDom.rank);
    type idxSignedType = chpl__signedType(idxType);

    // Unconditionally sets 'blkChanged'
    //
    // TODO: If 'collapsedDims' were param, we would know if blk(rank) was 1 or not.
    var rad : _remoteAccessData(eltType, newDom.rank, newDom.idxType, newDom.stridable, true);
    const collapsedDims = chpl__tuplify(cd);

    rad.initDataFrom(this);

    rad.shiftedData = if newDom.stridable then this.data else this.shiftedData;
    rad.origin      = this.origin:newDom.idxType;

    var mdpdIsRange : bool;
    var mdpdJ       : this.idxType;
    var mdpdJVal    : this.idxType;
    var curDim      = 1;
    for param j in 1..idx.size {
      if !collapsedDims(j) {
        rad.off(curDim) = newDom.dsiDim(curDim).low;
        const off       = (rad.off(curDim) - this.off(j)):idxSignedType;
        rad.origin     += ((this.blk(j):idxSignedType) * off / this.str(j)):idxType;
        rad.blk(curDim) = this.blk(j);
        rad.str(curDim) = this.str(j);

        if !defRectSimpleDData && j == mdParDim {
          mdpdIsRange  = true;
          rad.mdParDim = curDim;
        }

        curDim += 1;
      } else {
        const off   = (idx(j) - this.off(j)):idxSignedType;
        rad.origin += (this.blk(j):idxSignedType *  off / this.str(j)):idxType;

        if !defRectSimpleDData && j == mdParDim {
          mdpdIsRange = false;
          mdpdJ       = j;
          mdpdJVal    = idx(j);
        }
      }
    }

    rad.computeFactoredOffs();

    if !defRectSimpleDData {
      if mdpdIsRange {
        rad.mdNumChunks = this.mdNumChunks;
        rad.mdRLo       = this.mdRLo;
        rad.mdRHi       = this.mdRHi;
        rad.mdRStr      = this.mdRStr;
        rad.mdRLen      = this.mdRLen;
        rad.mdBlk       = this.mdBlk;

        for i in 1..#mdNumChunks {
          const rng = max(this.mData(i).pdr.low, newDom.dsiDim(rad.mdParDim).low)
                      ..min(this.mData(i).pdr.high, newDom.dsiDim(rad.mdParDim).high);
          rad.mData(i).pdr = if !rad.stridable then rng else rng by newDom.dsiDim(rad.mdParDim).stride align newDom.dsiDim(rad.mdParDim).alignment;
        }
      } else {
        // If the mdParDim'th dimension is removed, then we switch to
        // a synthesized mdParDim==1.
        const blkRatio  = this.blk(1) / rad.blk(1);
        rad.mdParDim    = 1;
        rad.mdNumChunks = this.mdNumChunks;
        rad.mdRLen      = this.mdRLen * this.mdBlk * blkRatio;
        rad.mdRStr      = abs(newDom.dsiDim(1).stride):rad.idxType;
        rad.mdRLo       = newDom.dsiDim(1).alignedLow - (mdpdJVal - this.mdRLo) * blkRatio;
        rad.mdRHi       = rad.mdRLo + (rad.mdRLen - 1) * rad.mdRStr;
        rad.mdBlk       = 1;

        for i in 1..#mdNumChunks {
          const (lo, hi) = rad.mdChunk2Ind(i-1);
          const rng = max(lo, newDom.dsiDim(1).low) .. min(hi, newDom.dsiDim(1).high);
          rad.mData(i).pdr = if !rad.stridable then rng else rng by newDom.dsiDim(1).stride align newDom.dsiDim(1).alignment;
        }
      }
    }


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
    var RADLocks: [targetLocDom] atomicbool; // only accessed locally
                                             // force processor atomics

    proc LocRADCache(type eltType, param rank: int, type idxType,
                     param stridable: bool, newTargetLocDom: domain(rank)) {
      // This should resize the arrays
      targetLocDom=newTargetLocDom;
    }

    // These functions must always be called locally, because the lock
    // is a (local) processor one.
    inline proc lockRAD(rlocIdx) {
      while RADLocks(rlocIdx).testAndSet() do chpl_task_yield();
    }

    inline proc unlockRAD(rlocIdx) {
      RADLocks(rlocIdx).clear();
    }
  }

  class DefaultRectangularArr: BaseArr {
    type eltType;
    param rank : int;
    type idxType;
    param stridable: bool;

    type idxSignedType = chpl__signedType(idxType);

    var dom : DefaultRectangularDom(rank=rank, idxType=idxType,
                                           stridable=stridable);
    var off: rank*idxType;
    var blk: rank*idxType;
    var str: rank*idxSignedType;
    var origin: idxType;
    var factoredOffs: idxType;

    pragma "local field"
    var data : _ddata(eltType);

    pragma "local field"
    var shiftedData : _ddata(eltType);

    inline proc oneDData return defRectSimpleDData || mdNumChunks < 2;

                                  // these are only used if !defRectSimpleDData
    var mdParDim: mdType(int);    //   array is chunked on this dimension
    var mdNumChunks: mdType(int); //   number of chunks
    var mdRLo: mdType(idxType);   //   chunking dim .low
    var mdRHi: mdType(idxType);   //       "     "  .high
    var mdRStr: mdType(idxType);  //       "     "  .stride
    var mdRLen: mdType(idxType);  //       "     "  .length
    var mdAlias: mdType(bool);    //   is this an alias of another array?

    pragma "local field"
      var mData : mdType(_ddata(_multiData(eltType=eltType,
                                    idxType=idxType)));

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
      if !defRectSimpleDData then {
        writeln("mdParDim=", mdParDim);
        writeln("mdNumChunks=", mdNumChunks);
        writeln("mdRLo=", mdRLo);
        writeln("mdRHi=", mdRHi);
        writeln("mdRStr=", mdRStr);
        writeln("mdRLen=", mdRLen);
        for i in 0..#mdNumChunks {
          writeln("chunk (", mData(i).pdr, ') @', mData(i).dataOff);
        }
      }
      writeln("noinit_data=", noinit_data);
    }

    // can the compiler create this automatically?
    proc dsiGetBaseDom() return dom;

    proc dsiDestroyDataHelper(dd, ddiNumIndices) {
      pragma "no copy" pragma "no auto destroy" var dr = dd;
      pragma "no copy" pragma "no auto destroy" var dv = __primitive("deref", dr);
      for i in 0..ddiNumIndices-1 {
        pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, i);
        pragma "no copy" pragma "no auto destroy" var ev = __primitive("deref", er);
        chpl__autoDestroy(ev);
      }
    }

    proc dsiDestroyArr(isalias:bool) {

      // data in an array alias will be destroyed when the original array
      // is destroyed.
      if isalias {
        // A multi-ddata alias nevertheless has its own mData.
        if !defRectSimpleDData {
          _ddata_free(mData);
        }

        return;
      }

      if dom.dsiNumIndices > 0 {
        pragma "no copy" pragma "no auto destroy" var dr = dataChunk(0);
        pragma "no copy" pragma "no auto destroy" var dv = __primitive("deref", dr);
        pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, 0);
        pragma "no copy" pragma "no auto destroy" var ev = __primitive("deref", er);
        if (chpl__maybeAutoDestroyed(ev)) {
          var numElts:idxType = 0;
          // dataAllocRange may be empty or contain a meaningful value
          if rank == 1 && !stridable then
            numElts = dataAllocRange.length;
          if numElts == 0 then
            numElts = dom.dsiNumIndices;

          if defRectSimpleDData {
            dsiDestroyDataHelper(dataChunk(0), numElts);
          } else {
            for chunk in 0..#mdNumChunks {
              const chunkSize = if mdRLen == 0 then 0
                                else numElts / mdRLen * mData(chunk).pdr.length;
              if chunkSize > 0 {
                dsiDestroyDataHelper(_ddata_shift(eltType, dataChunk(chunk),
                                                  mData(chunk).dataOff),
                                     chunkSize);
              }
            }
          }
        }
      }

      if defRectSimpleDData {
        _ddata_free(dataChunk(0));
      } else {
        for chunk in 0..#mdNumChunks {
          _ddata_free(_ddata_shift(eltType,
                                   dataChunk(chunk),
                                   mData(chunk).dataOff));
        }
        _ddata_free(mData);
      }
    }

    inline proc dataChunk(i) ref {
      if defRectSimpleDData then
        return data;
      else {
        return mData(i).data;
      }
    }

    inline proc theDataChunk(i: idxType) ref {
      if defRectSimpleDData {
        if earlyShiftData && !stridable then
          return shiftedData;
        else
          return data;
      } else {
        if earlyShiftData && !stridable then
          return mData(i).shiftedData;
        else
          return mData(i).data;
      }
    }

    inline proc theDataChunk(i: integral) ref {
      return theDataChunk(i: idxType);
    }

    inline proc theData(i: idxType) ref where defRectSimpleDData
      return theDataChunk(0)(i);

    inline proc theData(i: (int, idxType)) ref where !defRectSimpleDData
      return theDataChunk(i(1))(i(2));

    inline proc theData(chunk: int, i: idxType) ref where !defRectSimpleDData
      return theDataChunk(chunk)(i);

    //
    // Simple-ddata iterators (locale models without sublocales)
    //
    iter these(tasksPerLocale:int = dataParTasksPerLocale,
               ignoreRunning:bool = dataParIgnoreRunningTasks,
               minIndicesPerTask:int = dataParMinGranularity)
      ref where defRectSimpleDData {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr simple-dd serial iterator");
      }
      for elem in chpl__serialViewIter(this, dom) do yield elem;
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      ref where tag == iterKind.standalone && defRectSimpleDData {
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
      where tag == iterKind.leader && defRectSimpleDData {
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
      ref where tag == iterKind.follower && defRectSimpleDData {
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

    //
    // Potentially multi-ddata iterators (locale models with sublocales)
    //
    iter these(tasksPerLocale:int = dataParTasksPerLocale,
               ignoreRunning:bool = dataParIgnoreRunningTasks,
               minIndicesPerTask:int = dataParMinGranularity)
      ref where !defRectSimpleDData {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr multi-dd serial iterator");
      }
      for elem in chpl__serialViewIter(this, dom) do yield elem;
    }

    iter these(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      where tag == iterKind.leader && !defRectSimpleDData {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr multi-dd leader iterator");
      }
      // This was adapted from the DefaultRectangularDom leader.
      const dptpl = if tasksPerLocale==0 then here.maxTaskPar
                    else tasksPerLocale;
      if debugDataParMultiDData {
        chpl_debug_writeln("### mdNumChunks = ", mdNumChunks, "\n",
                           "### ignoreRunning = ", ignoreRunning, "\n",
                           "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                           "### mdParDim = ", mdParDim, "\n",
                           "### dom.dsiDims = ", dom.dsiDims());
      }
      coforall chunk in 0..#mdNumChunks { // make sure coforall on can trigger
        local on here.getChild(chunk) { // eventually, on dataChunk(chunk).locale
          if debugDataParMultiDData {
            if chunk != chpl_getSubloc() then
              writeln("*** ERROR: multiDD:  ON WRONG SUBLOC (should be ",
                      chunk, ", on ", chpl_getSubloc(), ") ***");
          }
          // Divide the locale's tasks approximately evenly
          // among the chunks.
          const numSublocTasks = (if chunk < dptpl % mdNumChunks
                                  then dptpl / mdNumChunks + 1
                                  else dptpl / mdNumChunks);
          var locBlock: rank*range(idxType);
          for param i in 1..rank do
            locBlock(i) = 0:idxType..#(dom.dsiDim(i).length);
          var followMe = locBlock;
          const (lo,hi) = _computeBlock(locBlock(mdParDim).length,
                                        mdNumChunks, chunk,
                                        locBlock(mdParDim).high,
                                        locBlock(mdParDim).low,
                                        locBlock(mdParDim).low);
          followMe(mdParDim) = lo..hi;
          const (numChunks2, parDim2) = _computeChunkStuff(numSublocTasks,
                                                           ignoreRunning,
                                                           minIndicesPerTask,
                                                           followMe);
          if debugDataParMultiDData then
            chpl_debug_writeln("### multiDD: chunk ", chunk,
                               ", followMe ", followMe,
                               ", numChunks2 ", numChunks2);
          coforall chunk2 in 0..#numChunks2 do local on here {
            var locBlock2: rank*range(idxType);
            for param i in 1..rank do
              locBlock2(i) = followMe(i).low..followMe(i).high;
            var followMe2 = locBlock2;
            const low  = locBlock2(parDim2).low;
            const high = locBlock2(parDim2).high;
            const (lo, hi) = _computeBlock(locBlock2(parDim2).length,
                                          numChunks2, chunk2,
                                          high, low, low);
            followMe2(parDim2) = lo..hi;
            if debugDataParMultiDData {
              if chunk != chpl_getSubloc():idxType then
                writeln("*** ERROR: multiDD leaf: ON WRONG SUBLOC (should be ",
                        chunk, ", on ", chpl_getSubloc(), ") ***");
              chpl_debug_writeln("### multiDD: chunk ", chunk,
                                 ", chunk2 ", chunk2,
                                 ", followMe ", followMe,
                                 ", followMe2 ", followMe2);
            }
            yield followMe2;
          }
        }
      }
    }

    iter these(param tag: iterKind, followThis,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity)
      ref where tag == iterKind.follower && !defRectSimpleDData {
      if debugDefaultDist {
        chpl_debug_writeln("*** In defRectArr multi-dd follower iterator: ",
                           followThis);
      }

      proc anyStridable(rangeTuple, param i: int = 1) param
        return if i == rangeTuple.size then rangeTuple(i).stridable
               else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

      param stridable = this.stridable || anyStridable(followThis);
      if stridable {
        for i in dom.these(tag=iterKind.follower, followThis,
                           tasksPerLocale,
                           ignoreRunning,
                           minIndicesPerTask) do
          yield dsiAccess(i);
      } else {
        // TODO: why does 'followThis' have a different idxType?
        const mdPDLow = dom.dsiDim(mdParDim).low;
        const chunk = mdInd2Chunk(mdPDLow + followThis(mdParDim).low:mdPDLow.type);
        if boundsChecking {
          // the code here assumes followThis spans but a single chunk
          assert(mdPDLow + followThis(mdParDim).high:mdPDLow.type <= mData(chunk).pdr.high);
        }

        //
        // shiftedData{Vec} is offset from data{Vec}, forward by
        // origin and backward by dom.dsiLow.  The domain follower
        // offsets forward by dom.dsiLow.  Combining these lets us
        // reference data{Vec} using the 0-based indexes passed in.
        //
        // gbt TODO: change to using .data here
        //
        var dd = mData(chunk).shiftedData;
        for ind in dom.these(tag=iterKind.follower, followThis,
                             tasksPerLocale,
                             ignoreRunning,
                             minIndicesPerTask) {
          var dataInd: idxType;
          if rank == 1 {
            dataInd = ind;
          } else {
            dataInd = 0;
            for param i in 1..rank-1 {
              dataInd += ind(i) * blk(i);
            }
            dataInd += ind(rank);  // blk(rank) == 1, so no need to multiply it
          }
          yield dd(dataInd);
        }
      }
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
          const shiftDist = if isIntType(idxType) then
                              origin - factoredOffs
                            else
                              // Not bothering to check for over/underflow
                              origin:idxSignedType - factoredOffs:idxSignedType;
          if defRectSimpleDData {
            shiftedData = _ddata_shift(eltType, dataChunk(0), shiftDist);
          } else {
            for i in 0..#mdNumChunks {
              mData(i).shiftedData = _ddata_shift(eltType, mData(i).data,
                                                  shiftDist);
            }
          }
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

      if defRectSimpleDData {
        data = _ddata_allocate(eltType, size);
      } else {
        //
        // Checking the size first (and having a large-ish size hurdle)
        // prevents us from calling the pure virtual getChildCount() in
        // ChapelLocale, when we're setting up arrays in the locale model
        // and thus here.getChildCount() isn't available yet.
        //
        if (size < defRectArrMultiDDataSizeThreshold
            || here.getChildCount() < 2) {
          mdParDim = 1;
          mdNumChunks = 1;
        }
        else {
          const (numChunks, parDim) =
            _computeChunkStuff(here.getChildCount(), ignoreRunning=true,
                               minSize=1, ranges=dom.ranges);
          if numChunks == 0 {
            mdParDim = 1;
            mdNumChunks = 1;
          } else {
            mdParDim = parDim;
            mdNumChunks = numChunks;
          }
        }
        mdRLo = dom.dsiDim(mdParDim).alignedLow;
        mdRHi = dom.dsiDim(mdParDim).alignedHigh;
        mdRStr = abs(dom.dsiDim(mdParDim).stride):idxType;
        mdRLen = dom.dsiDim(mdParDim).length;
        mData = _ddata_allocate(_multiData(eltType=eltType,
                                           idxType=idxType),
                                mdNumChunks);

        //
        // If just a single chunk then get memory from anywhere but if
        // more then get each chunk's memory from the corresponding
        // sublocale.
        //
        if mdNumChunks == 1 {
          if stridable then
            mData(0).pdr = dom.dsiDim(mdParDim).low..dom.dsiDim(mdParDim).high
                           by dom.dsiDim(mdParDim).stride align dom.dsiDim(mdParDim).alignment;
          else
            mData(0).pdr = dom.dsiDim(mdParDim).low..dom.dsiDim(mdParDim).high;
          mData(0).data =
            _ddata_allocate(eltType, size,
                            locStyle = if here.maxTaskPar < 2
                                       then localizationStyle_t.locNone
                                       else localizationStyle_t.locSubchunks);
        } else {
          var dataOff: idxType = 0;
          for i in 0..#mdNumChunks do local on here.getChild(i) {
            mData(i).dataOff = dataOff;
            const (lo, hi) = mdChunk2Ind(i);
            if stridable then
              mData(i).pdr = lo..hi by dom.dsiDim(mdParDim).stride align dom.dsiDim(mdParDim).alignment;
            else
              mData(i).pdr = lo..hi;
            const chunkSize = size / mdRLen * mData(i).pdr.length;
            const dd = _ddata_allocate(eltType, chunkSize,
                                       locStyle = localizationStyle_t.locWhole,
                                       subloc = i:chpl_sublocID_t);
            mData(i).data = _ddata_shift(eltType, dd, -dataOff:idxSignedType);
            dataOff += chunkSize;
          }
        }
      }

      initShiftedData();
      if rank == 1 && !stridable then
        dataAllocRange = dom.dsiDim(1);
    }

    inline proc mdInd2Chunk(ind)
      where !defRectSimpleDData {
      if stridable then
        return (((ind - mdRLo) / mdRStr * mdNumChunks:idxType)
                / mdRLen):int;
      else
        return (((ind - mdRLo) * mdNumChunks:idxType) / mdRLen):int;
    }

    inline proc mdChunk2Ind(chunk)
      where !defRectSimpleDData {
      if stridable {
        var (lo, hi) = _computeBlock(mdRLen, mdNumChunks, chunk,
                                     (mdRHi - mdRLo) / mdRStr, 0, 0);
        lo = lo * mdRStr + mdRLo;
        hi = hi * mdRStr + mdRLo;
        return (lo, hi);
      } else {
        return _computeBlock(mdRLen, mdNumChunks, chunk, mdRHi, mdRLo, mdRLo);
      }
    }

    inline proc getDataIndex(ind: idxType ...1,
                             param getShifted = true,
                             param getChunked = !defRectSimpleDData)
      where rank == 1
      return getDataIndex(ind, getShifted=getShifted, getChunked=getChunked);

    inline proc getDataIndex(ind: rank*idxType,
                             param getShifted = true,
                             param getChunked = !defRectSimpleDData) {
      param chunkify = !defRectSimpleDData && getChunked;

      if stridable {
        inline proc chunked_dataIndex(sum, str) {
          if mdNumChunks == 1 {
            return (0, sum);
          } else {
            const chunk = mdInd2Chunk(ind(mdParDim));
            return (chunk, sum);
          }
        }

        var sum = origin;
        for param i in 1..rank do
          sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
        if chunkify then
          return chunked_dataIndex(sum, str=abs(str(mdParDim)):idxType);
        else
          return sum;
      } else {
        inline proc chunked_dataIndex(sum) {
          if mdNumChunks == 1 {
            return (0, sum);
          } else {
            const chunk = mdInd2Chunk(ind(mdParDim));
            return (chunk, sum);
          }
        }

        param wantShiftedIndex = getShifted && earlyShiftData;

        // optimize common case to get cleaner generated code
        if (rank == 1 && wantShiftedIndex) {
          if chunkify then
            return chunked_dataIndex(ind(1));
          else
            return ind(1);
        } else {
          var sum = if wantShiftedIndex then 0:idxType else origin;

          for param i in 1..rank-1 {
            sum += ind(i) * blk(i);
          }
          sum += ind(rank);

          if !wantShiftedIndex then sum -= factoredOffs;
          if chunkify then
            return chunked_dataIndex(sum);
          else
            return sum;
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
    where shouldReturnRvalueByValue(eltType) {
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
    where shouldReturnRvalueByValue(eltType)
      return dsiAccess(i);

    inline proc dsiLocalAccess(i) const ref
    where shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(i);

    proc adjustBlkOffStrForNewDomain(d: DefaultRectangularDom,
                                     alias: DefaultRectangularArr)
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
        if defRectSimpleDData {
          data = copy.data;
        } else {
          mdParDim = copy.mdParDim;
          mdNumChunks = copy.mdNumChunks;
          mdRLo = copy.mdRLo;
          mdRHi = copy.mdRHi;
          mdRStr = copy.mdRStr;
          mdRLen = copy.mdRLen;
          mData = copy.mData;
        }
        // We can't call initShiftedData here because the new domain
        // has not yet been updated (this is called from within the
        // = function for domains.
        if earlyShiftData && !d._value.stridable {
          // Lydia note 11/04/15: a question was raised as to whether this
          // check on numIndices added any value.  Performance results
          // from removing this line seemed inconclusive, which may indicate
          // that the check is not necessary, but it seemed like unnecessary
          // work for something with no immediate reward.
          if d.numIndices > 0 {
            if defRectSimpleDData {
              shiftedData = copy.shiftedData;
            }
          }
        }
        // also set dataAllocRange
        dataAllocRange = copy.dataAllocRange;
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
      var rad: _remoteAccessData(eltType, rank, idxType, stridable);
      rad.off = off;
      rad.blk = blk;
      rad.str = str;
      rad.origin = origin;
      rad.factoredOffs = factoredOffs;
      if defRectSimpleDData {
        rad.data = data;
        rad.shiftedData = shiftedData;
      }
      else {
        rad.mdParDim = mdParDim;
        rad.mdNumChunks = mdNumChunks;
        rad.mdRLo = mdRLo;
        rad.mdRHi = mdRHi;
        rad.mdRStr = mdRStr;
        rad.mdRLen = mdRLen;
        rad.mdBlk = 1;
        for i in 1..#mdNumChunks {
          rad.mData(i).data        = mData(i - 1).data;
          rad.mData(i).shiftedData = mData(i - 1).shiftedData;
          rad.mData(i).pdr         = mData(i - 1).pdr;
          rad.mData(i).dataOff     = mData(i - 1).dataOff;
        }
      }
      return rad;
    }

    proc dsiTargetLocales() {
      return [this.data.locale, ];
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      if (this.data.locale == here) {
        return _getDomain(dom);
      } else {
        var a: domain(rank, idxType, stridable);
        return a;
      }
    }

    iter dsiLocalSubdomains() {
      yield dsiLocalSubdomain();
    }
  }

  iter chpl__serialViewIter(arr, viewDom) ref
    where chpl__isDROrDRView(arr) && !defRectSimpleDData {
    param useCache = chpl__isArrayView(arr) && arr.shouldUseIndexCache();
    var info = if useCache then arr.indexCache
               else if arr.isSliceArrayView() then arr.arr
               else arr;

    if arr.rank == 1 && !viewDom.stridable {
      const first  = info.getDataIndex(viewDom.dsiLow, getChunked=false);
      const second = info.getDataIndex(viewDom.dsiLow+1, getChunked=false);
      const step   = (second-first);
      const lo     = viewDom.dsiDim(info.mdParDim).low;
      const hi     = viewDom.dsiDim(info.mdParDim).high;

      param chunkOffset = if useCache then 1 else 0;
      var (chunk, idx) = info.getDataIndex(viewDom.dsiLow);
      var dd           = info.theDataChunk(chunk);
      chunk += chunkOffset;
      var lastChunkInd = info.mData(chunk).pdr.high;

      for ind in chpl_direct_pos_stride_range_iter(lo, hi, 1:viewDom.idxType) {
        if ind > lastChunkInd { // traverse to next chunk
          (chunk, idx) = info.getDataIndex(ind);
          dd           = info.theDataChunk(chunk);
          chunk += chunkOffset;
          lastChunkInd = info.mData(chunk).pdr.high;
        }
        yield dd(idx);
        idx += step;
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

  iter chpl__serialViewIter(arr, viewDom) ref
    where chpl__isDROrDRView(arr) && defRectSimpleDData {
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
        // also add a range constructor, which in tight loops is pretty
        // expensive. Instead we use a direct range iterator that is
        // optimized for positively strided ranges. It should be just as fast
        // as directly using a "c for loop", but it contains code check for
        // overflow and invalid strides as well as the ability to use a less
        // optimized iteration method if users are concerned about range
        // overflow.
        const first  = info.getDataIndex(viewDom.dsiLow);
        const second = info.getDataIndex(viewDom.dsiLow+1);
        const step   = (second-first);
        const last   = first + (viewDom.dsiNumIndices-1) * step;
        for i in chpl_direct_pos_stride_range_iter(first, last, step) {
          yield info.theDataChunk(0)(i);
        }
      } else {
        const viewDomDim = viewDom.dsiDim(1),
              stride = viewDomDim.stride: viewDom.idxType,
              start  = viewDomDim.first,
              second = info.getDataIndex(start + stride);

        var   first  = info.getDataIndex(start);
        const step   = (second-first):chpl__signedType(viewDom.idxType);
        var   last   = first + (viewDomDim.length-1) * step:viewDom.idxType;

        if step < 0 then
          last <=> first;

        for i in first..last by step do
          yield info.data(i);
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
      const dataIdx = if arr.isReindexArrayView() then arr.chpl_reindexConvertIdx(i)
                      else if arr.isRankChangeArrayView() then chpl_rankChangeConvertIdx(i, arr.collapsedDim, arr.idx)
                      else i;
      const info = if chpl__isArrayView(arr) then arr.arr else arr;
      yield info.dsiAccess(dataIdx);
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
    chpl_serialReadWriteRectangular(f, this);
  }

  // Why can the following two functions not be collapsed into one
  // where 'dom = arr.dom'?  Because this puts a type constraint on
  // what 'dom' can be passed that is too strict in some callchains
  // (e.g., if arr.dom is non-stridable but the 'dom' passed in is
  // stridable).
  //
  proc chpl_serialReadWriteRectangular(f, arr) {
    chpl_serialReadWriteRectangular(f, arr, arr.dom);
  }

  proc chpl_serialReadWriteRectangular(f, arr, dom) {
    chpl_serialReadWriteRectangularHelper(f, arr, dom);
  }

  proc chpl_serialReadWriteRectangularHelper(f, arr, dom) {
    param rank = arr.rank;
    type idxType = arr.idxType;
    type idxSignedType = chpl__signedType(idxType);

    const isNative = f.styleElement(QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER): bool;

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

      var offset = dom.dsiDim(1).low;
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
      pragma "no prototype"
      extern proc sizeof(type x): size_t;
      const elemSize = sizeof(arr.eltType);
      if boundsChecking {
        var rw = if f.writing then "write" else "read";
        assert((dom.dsiNumIndices:uint*elemSize:uint) <= max(ssize_t):uint,
               "length of array to ", rw, " is greater than ssize_t can hold");
      }
      if defRectSimpleDData {
        const len = dom.dsiNumIndices;
        const src = arr.theDataChunk(0);
        const idx = arr.getDataIndex(dom.dsiLow);
        const size = len:ssize_t*elemSize:ssize_t;
        if f.writing {
          f.writeBytes(_ddata_shift(arr.eltType, src, idx), size);
        } else {
          f.readBytes(_ddata_shift(arr.eltType, src, idx), size);
        }
      } else {
        var indLo = dom.dsiLow;
        for chunk in 0..#arr.mdNumChunks {
          if arr.mData(chunk).pdr.length >= 0 {
            const src = arr.theDataChunk(chunk);
            const cmp = if isTuple(indLo) then indLo(arr.mdParDim) else indLo;
            const newLow = max(arr.mData(chunk).pdr.low, cmp);
            if isTuple(indLo) then
              indLo(arr.mdParDim) = newLow;
            else
              indLo = newLow;
            const (_, idx) = arr.getDataIndex(indLo);
            const blkLen = if arr.mdParDim == arr.rank
                           then 1
                           else arr.blk(arr.mdParDim) / arr.blk(arr.mdParDim+1);
            const outer = dom.dsiDim(arr.mdParDim);
            const inner = arr.mData(chunk).pdr;
            const len = outer[inner].length * blkLen;
            const size = len:ssize_t*elemSize:ssize_t;
            if f.writing {
              f.writeBytes(_ddata_shift(arr.eltType, src, idx), size);
            } else {
              f.readBytes(_ddata_shift(arr.eltType, src, idx), size);
            }
          }
        }
      }
    } else {
      const zeroTup: rank*idxType;
      recursiveArrayWriter(zeroTup);
    }
  }

  proc DefaultRectangularArr.dsiSerialWrite(f) {
    dsiSerialReadWrite(f);
  }

  proc DefaultRectangularArr.dsiSerialRead(f) {
    dsiSerialReadWrite(f);
  }

  // This is very conservative.
  proc DefaultRectangularArr.isDataContiguous(dom) {
    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("isDataContiguous(): origin=", origin, " off=", off, " blk=", blk);

    if blk(rank) != 1 then return false;

    for param dim in 1..(rank-1) by -1 do
      if blk(dim) != blk(dim+1)*dom.dsiDim(dim+1).length then return false;

    // Strictly speaking a multi-ddata array isn't contiguous, but
    // nevertheless we do support bulk transfer on such arrays, so
    // here we ignore single- vs. multi-ddata.

    if debugDefaultDistBulkTransfer then
      chpl_debug_writeln("\tYES!");

    return true;
  }

  proc DefaultRectangularArr.dsiSupportsBulkTransfer() param return true;
  proc DefaultRectangularArr.dsiSupportsBulkTransferInterface() param return true;

  proc DefaultRectangularArr.doiCanBulkTransfer(viewDom) {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiCanBulkTransfer()");
    if viewDom.stridable then
      for param i in 1..rank do
        if viewDom.dim(i).stride != 1 then return false;
    if !isDataContiguous(viewDom._value) {
      if debugDefaultDistBulkTransfer then
        chpl_debug_writeln("isDataContiguous return False");
      return false;
    }
    return true;
  }

  proc DefaultRectangularArr.doiCanBulkTransferStride(viewDom) param {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiCanBulkTransferStride()");
    // A DefaultRectangular array is always regular, so bulk should be possible.
    return true;
  }

  proc DefaultRectangularArr.doiUseBulkTransfer(B) {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiUseBulkTransfer()");

    //
    // With multi-ddata, at least for now if the arrays aren't chunked
    // exactly the same way we don't do direct bulk transfer.
    //
    if this.rank != B.rank {
      return false;
    } else if defRectSimpleDData {
      return true;
    } else {
      const actual = chpl__getActualArray(B);
      return mdParDim == actual.mdParDim
             && mdNumChunks == actual.mdNumChunks
             && mdRLen == actual.mdRLen;
    }
  }

  proc DefaultRectangularArr.doiUseBulkTransferStride(B) {
    if debugDefaultDistBulkTransfer then chpl_debug_writeln("In DefaultRectangularArr.doiUseBulkTransferStride()");

    //
    // For now, strided bulk transfer is only supported on single-ddata
    // arrays.
    // gbt TODO
    //
    var actual = chpl__getActualArray(B);
    return oneDData && actual.oneDData;
  }

  proc DefaultRectangularArr.doiBulkTransfer(B, viewDom) {
    var actual = chpl__getActualArray(B);
    bulkTransferFrom(viewDom, actual, chpl__getViewDom(B));
  }

  proc DefaultRectangularArr.bulkTransferFrom(viewDom, B, bView) {
    const Adims = viewDom.dims();
    var Alo: rank*viewDom.idxType;
    for param i in 1..rank do
      Alo(i) = Adims(i).first;

    const Bdims = bView.dims();
    var Blo: rank*idxType;
    for param i in 1..rank do
      Blo(i) = Bdims(i).first;

    const len = viewDom.numIndices.safeCast(size_t);

    if len == 0 then return;

    if debugBulkTransfer {
      pragma "no prototype"
      extern proc sizeof(type x): int;
      const elemSize =sizeof(B.eltType);
      chpl_debug_writeln("In DefaultRectangularArr.doiBulkTransfer():",
              " Alo=", Alo, ", Blo=", Blo,
              ", len=", len, ", elemSize=", elemSize);
    }

    if defRectSimpleDData {
      const Aidx = getDataIndex(Alo);
      const Adata = _ddata_shift(eltType, this.theDataChunk(0), Aidx);
      const Bidx = B.getDataIndex(Blo);
      const Bdata = _ddata_shift(eltType, B.theDataChunk(0), Bidx);
      doiBulkTransferHelper(B, Adata, Bdata, len);
    }
    else {
      //
      // Some prefix of the transfer is in the first involved chunk.
      // The multi-ddata chunking must be the same here, so we only
      // need to compute the starting chunk for A[].
      //
      const (chunk0, Aidx) = getDataIndex(Alo);
      const Adata = _ddata_shift(eltType, this.theDataChunk(chunk0), Aidx);
      var len0 = ((mData(chunk0).pdr.high - Alo(mdParDim) + 1) * blk(mdParDim))
                 .safeCast(size_t);
      const (_, Bidx) = getDataIndex(Blo);
      const Bdata = _ddata_shift(eltType, B.theDataChunk(chunk0), Bidx);
      doiBulkTransferHelper(B, Adata, Bdata, min(len0, len));

      if len > len0 {
        var lenRemain = len;
        var chunkLen = len0;
        var chunk = chunk0 + 1;
        do {
          lenRemain -= chunkLen;
          chunkLen = (mData(chunk).pdr.length * blk(mdParDim)).safeCast(size_t);
          doiBulkTransferHelper(B,
                                _ddata_shift(eltType,
                                             dataChunk(chunk),
                                             mData(chunk).dataOff),
                                _ddata_shift(B.eltType,
                                             B.dataChunk(chunk),
                                             mData(chunk).dataOff),
                                min(chunkLen, lenRemain));
        } while lenRemain > chunkLen;
      }
    }
  }

  proc DefaultRectangularArr.doiBulkTransferHelper(B, Adata, Bdata, len) {
    if Adata == Bdata then return;

    // NOTE: This does not work with --heterogeneous, but heterogeneous
    // compilation does not work right now.  The calls to chpl_comm_get
    // and chpl_comm_put should be changed once that is fixed.
    if Adata.locale.id==here.id {
      if debugDefaultDistBulkTransfer then //See bug in test/optimizations/bulkcomm/alberto/rafatest2.chpl
        chpl_debug_writeln("\tlocal get() from ", B.locale.id);
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
  DefaultRectangular _array record (or array-view to default-rectangular array)

  `viewDom` is a DefaultRectangularDom class representing a view into the array
  data for 'A'. If 'B' is an ArrayView, its own view-domain will be computed
  within this function.

  Assumes row-major ordering.

  Depends on adjustBlkOffStrForNewDomain having been called in ChapelArray
  before entering this function.

  TODO: Determine if we can reduce the communication for array metadata

  TODO: Pull simple runtime implementation up into module code
  */
  proc DefaultRectangularArr.doiBulkTransferStride(B, viewDom) {
    if debugDefaultDistBulkTransfer {
      writeln();
      writeln("In DefaultRectangularArr.doiBulkTransferStride");
    }
    var actual = chpl__getActualArray(B);
    if (this.dataChunk(0).locale.id != here.id
        && actual.dataChunk(0).locale.id != here.id) {
      if debugDefaultDistBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: Both arrays on different locale, moving to locale of destination: LOCALE", this.dataChunk(0).locale.id);
      }
      on this.dataChunk(0) do stridedTransferFrom(viewDom, B);
    } else {
      stridedTransferFrom(viewDom, B);
    }
  }

  proc DefaultRectangularArr.stridedTransferFrom(LViewDom, B) {
    const SizeDims   = B.domain.dims();
    const LHS        = this;
    const RHS        = chpl__getActualArray(B);
    const RViewDom   = chpl__getViewDom(B);
    param targetRank = B.rank;

    if debugDefaultDistBulkTransfer {
      writeln("Transferring views :", LViewDom, " <-- ", RViewDom);
      writeln("Original domains   :", this.dom.dsiDims(), " <-- ", RHS.dom.dsiDims());
    }

    const LViewDims = LViewDom.dims();
    const RViewDims  = RViewDom.dims();

    // Build up the index tuples to calculate the offset for the first element
    var LFirst : LHS.rank*idxType;
    for param i in 1..LHS.rank do
      LFirst(i) = if LViewDims(i).stride < 0 then LViewDims(i).last else LViewDims(i).first;

    var RFirst : RHS.rank*idxType;
    for param i in 1..RHS.rank do
      RFirst(i) = if RViewDims(i).stride < 0 then RViewDims(i).last else RViewDims(i).first;

    // Compute a 'blk' tuple for the LHS and RHS based on their view-domains
    var LBlk, RBlk : targetRank*idxType;

    // The current index into the LHS or RHS
    var li = LHS.rank, ri = RHS.rank;


    // If the dimension is of size one, it may be representing a rank-change.
    // If ``li`` or ``ri`` are greater than 'idx', there is at least one
    // rank-changed dimension between li/ri and idx. In that case, walk
    // backward until you either find a non-length-one dimension or li/ri ==
    // idx.
    for idx in 1..targetRank by -1 {
      if LViewDims(li).size == 1 && li > idx {
        while LViewDims(li).size == 1 && li > idx do li -= 1;
      }
      if RViewDims(ri).size == 1 && ri > idx {
        while RViewDims(ri).size == 1 && ri > idx do ri -= 1;
      }
      LBlk(idx) = LHS.blk(li) * (LViewDims(li).stride / LHS.dom.dsiDim(li).stride):idxType;
      RBlk(idx) = RHS.blk(ri) * (RViewDims(ri).stride / RHS.dom.dsiDim(ri).stride):idxType;
      li -= 1;
      ri -= 1;
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
    var countDom = {1..targetRank+1};
    var count : [countDom] size_t;
    for c in count do c = 1; // serial to avoid task creation overhead

    //
    // The arrays representing the required stride at each level. Should pull
    // from the values in the 'blk' tuple, though we may skip a dimension if
    // it can be aggregated. Will ultimately be of size `stridelevels`.
    //
    var strideDom = {1..targetRank};
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
    if LBlk(targetRank) > 1 || RBlk(targetRank) > 1 {
      stridelevels += 1;
      count[stridelevels] = 1;
      dstStride[stridelevels] = LBlk(targetRank).safeCast(size_t);
      srcStride[stridelevels] = RBlk(targetRank).safeCast(size_t);
    }

    //
    // When at the end of the elements in dimension `i`, `canReuseStride` tells
    // us whether or not we can re-use the current stride value to advance to
    // the next chunk of elements. If either array is unable to re-use its
    // stride, then we need a new stride value.
    //
    for i in 2..targetRank by -1 {
      // Each corresponding dimension in A and B should have the same length,
      // so it doesn't matter which we use here.
      count[stridelevels+1] *= SizeDims(i).length.safeCast(size_t);

      const bothReuse = canReuseStride(LBlk, i, stridelevels, count, dstStride)
                     && canReuseStride(RBlk, i, stridelevels, count, srcStride);

      if !bothReuse {
        stridelevels += 1;
        dstStride[stridelevels] = LBlk(i-1).safeCast(size_t);
        srcStride[stridelevels] = RBlk(i-1).safeCast(size_t);
      }
    }
    count[stridelevels+1] *= SizeDims(1).length.safeCast(size_t);

    assert(stridelevels <= targetRank, "BulkTransferStride: stride levels greater than rank.");
    if stridelevels == 0 then assert(count[1] == LViewDom.numIndices, "BulkTransferStride: bulk-count incorrect for stride level of 0.");

    countDom = {1..stridelevels+1};
    strideDom = {1..stridelevels};

    doiBulkTransferStrideComm(RHS, stridelevels:int(32), dstStride, srcStride, count, LFirst, RFirst);
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
                         "\tcount         = " + stringify(count));
    }

    const A = this;
    const AO = A.getDataIndex(AFirst, getShifted=false);
    const AOChunk: int = if defRectSimpleDData then 0 else AO(1);
    const AOIdx: idxType = if defRectSimpleDData then AO else AO(2);
    const BO = B.getDataIndex(BFirst, getShifted=false);
    const BOChunk: int = if defRectSimpleDData then 0 else BO(1);
    const BOIdx: idxType = if defRectSimpleDData then BO else BO(2);

    const dest = A.dataChunk(AOChunk);
    const src  = B.dataChunk(BOChunk);

    assert(dstStride._value.oneDData);
    const dststr = dstStride._value.dataChunk(0);
    assert(srcStride._value.oneDData);
    const srcstr = srcStride._value.dataChunk(0);
    assert(count._value.oneDData);
    const cnt = count._value.dataChunk(0);

    if dest.locale.id == here.id {
      const srclocale = src.locale.id : int(32);

      if debugBulkTransfer {
        chpl_debug_writeln("BulkTransferStride: On LHS - GET from ", srclocale);
      }

      __primitive("chpl_comm_get_strd",
                  dest[AOIdx],
                  dststr[0],
                  srclocale,
                  src[BOIdx],
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
                  dest[AOIdx],
                  dststr[0],
                  destlocale,
                  src[BOIdx],
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
  proc canReuseStride(blk, curDim: int, levels, count, stride)
  {
    // TODO: implement for multi-ddata

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
