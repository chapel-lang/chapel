// DefaultRectangular.chpl
//
pragma "no use ChapelStandard"
module DefaultRectangular {

use DSIUtil;
config param debugDefaultDist = false;
config param debugDefaultDistBulkTransfer = false;
config param debugDataPar = false;

config param defaultDoRADOpt = true;
config param defaultDisableLazyRADOpt = false;

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

  proc dsiClone() return this;

  proc dsiAssign(other: this.type) { }

  proc dsiCreateReindexDist(newSpace, oldSpace) return this;
  proc dsiCreateRankChangeDist(param newRank, args) return this;
}

//
// Note that the replicated copies are set up in ChapelLocale on the
// other locales.  This just sets it up on this locale.
//
pragma "private" var defaultDist = new dmap(new DefaultDist());

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
    if d == rank - 1 {
      for i in ranges(d) do
        for j in ranges(rank) do
          yield (i, j);
    } else {
      for i in ranges(d) do
        for j in these_help(d+1) do
          yield (i, (...j));
    }
  }

  iter these_help(param d: int, block) {
    if d == block.size - 1 {
      for i in block(d) do
        for j in block(block.size) do
          yield (i, j);
    } else {
      for i in block(d) do
        for j in these_help(d+1, block) do
          yield (i, (...j));
    }
  }

  iter these() {
    if rank == 1 {
      for i in ranges(1) do
        yield i;
    } else {
      for i in these_help(1) do
        yield i;
    }
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    if debugDefaultDist then
      writeln("*** In domain/array leader code:"); // this = ", this);
    const numTasks = if dataParTasksPerLocale==0 then here.numCores
                     else dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    const minIndicesPerTask = dataParMinGranularity;
    if debugDataPar {
      writeln("### numTasks = ", numTasks);
      writeln("### ignoreRunning = ", ignoreRunning);
      writeln("### minIndicesPerTask = ", minIndicesPerTask);
    }

    if debugDefaultDist then
      writeln("    numTasks=", numTasks, " (", ignoreRunning,
              "), minIndicesPerTask=", minIndicesPerTask);

    var (numChunks, parDim) = _computeChunkStuff(numTasks, ignoreRunning,
                                                 minIndicesPerTask, ranges);
    if debugDefaultDist then
      writeln("    numChunks=", numChunks, " parDim=", parDim,
              " ranges(", parDim, ").length=", ranges(parDim).length);

    if debugDataPar then writeln("### numChunks=", numChunks, " (parDim=", parDim, ")");

    if (CHPL_TARGET_PLATFORM != "xmt") {
      if numChunks == 1 {
        if rank == 1 {
          yield tuple(0..ranges(1).length-1);
        } else {
          var block: rank*range(idxType);
          for param i in 1..rank do
            block(i) = 0..ranges(i).length-1;
          yield block;
        }
      } else {
        var locBlock: rank*range(idxType);
        for param i in 1..rank do
          locBlock(i) = 0:ranges(i).low.type..#(ranges(i).length);
        if debugDefaultDist then
          writeln("*** DI: locBlock = ", locBlock);
        coforall chunk in 0..#numChunks {
          var tuple: rank*range(idxType) = locBlock;
          const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                        numChunks, chunk,
                                        locBlock(parDim).high);
          tuple(parDim) = lo..hi;
          if debugDefaultDist then
            writeln("*** DI[", chunk, "]: tuple = ", tuple);
          yield tuple;
        }
      }
    } else {

      var per_stream_i: uint(64) = 0;
      var total_streams_n: uint(64) = 0;

      var locBlock: rank*range(idxType);
      for param i in 1..rank do
        locBlock(i) = 0:ranges(i).low.type..#(ranges(i).length);

      __primitive_loop("xmt pragma forall i in n", per_stream_i,
                       total_streams_n) {

        var tuple: rank*range(idxType) = locBlock;
        const (lo,hi) = _computeBlock(ranges(parDim).length,
                                      total_streams_n, per_stream_i,
                                      (ranges(parDim).length-1));
        tuple(parDim) = lo..hi;
        yield tuple;
      }
    }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    proc anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

    chpl__testPar("default rectangular domain follower invoked on ", followThis);
    if debugDefaultDist then
      writeln("In domain follower code: Following ", followThis);
    param stridable = this.stridable || anyStridable(followThis);
    var block: rank*range(idxType=idxType, stridable=stridable);
    if stridable {
      for param i in 1..rank {
        const rStride = ranges(i).stride:idxType,
              fStride = followThis(i).stride:idxType;
        if ranges(i).stride > 0 {
          const low = ranges(i).low + followThis(i).low*rStride,
                high = ranges(i).low + followThis(i).high*rStride,
                stride = (rStride * fStride):int;
          block(i) = low..high by stride;
        } else {
          const low = ranges(i).high + followThis(i).high*rStride,
                high = ranges(i).high + followThis(i).low*rStride,
                stride = (rStride * fStride): int;
          block(i) = low..high by stride;
        }
      }
    } else {
      for  param i in 1..rank do
        block(i) = ranges(i).low+followThis(i).low:idxType..ranges(i).low+followThis(i).high:idxType;
    }

    if rank == 1 {
      for i in block {
        __primitive("noalias pragma");
        yield i;
      }
    } else {
      for i in these_help(1, block) {
        __primitive("noalias pragma");
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
      if (orderD == -1) then return orderD;
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
  var data : _ddata(eltType);
  var noinit: bool = false;
  //var numelm: int = -1; // for correctness checking

  proc canCopyFromDevice param return true;

  // end class definition here, then defined secondary methods below

  proc dsiDisplayRepresentation() {
    writeln("off=", off);
    writeln("blk=", blk);
    writeln("str=", str);
    writeln("origin=", origin);
    writeln("factoredOffs=", factoredOffs);
    writeln("noinit=", noinit);
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

  iter these() var {
    if rank == 1 {
      // This is specialized to avoid overheads of calling dsiAccess()
      if !dom.stridable {
        // This is specialized because the strided version disables the
        // "single loop iterator" optimization
        var first = getDataIndex(dom.dsiLow);
        var second = getDataIndex(dom.dsiLow+dom.ranges(1).stride:idxType);
        var step = (second-first):chpl__signedType(idxType);
        var last = first + (dom.dsiNumIndices-1) * step:idxType;
        for i in first..last by step do
          yield data(i);
      } else {
        const stride = dom.ranges(1).stride: idxType,
              start  = dom.ranges(1).first,
              first  = getDataIndex(start),
              second = getDataIndex(start + stride),
              step   = (second-first):chpl__signedType(idxType),
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

  iter these(param tag: iterKind) where tag == iterKind.leader {
    for followThis in dom.these(tag) do
      yield followThis;
  }

  iter these(param tag: iterKind, followThis) var where tag == iterKind.follower {
    if debugDefaultDist then
      writeln("*** In array follower code:"); // [\n", this, "]");
    for i in dom.these(tag=iterKind.follower, followThis) {
      __primitive("noalias pragma");
      yield dsiAccess(i);
    }
  }

  proc computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for param i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }
  
  // change name to setup and call after constructor call sites
  // we want to get rid of all initialize functions everywhere
  proc initialize() {
    if noinit == true then return;
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
  }

  inline proc getDataIndex(ind: idxType ...1) where rank == 1
    return getDataIndex(ind);

  inline proc getDataIndex(ind: rank* idxType) {
    var sum = origin;
    if stridable {
      for param i in 1..rank do
        sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
    } else {
      for param i in 1..rank do
        sum += ind(i) * blk(i);
      sum -= factoredOffs;
    }
    return sum;
  }

  // only need second version because wrapper record can pass a 1-tuple
  inline proc dsiAccess(ind: idxType ...1) var where rank == 1
    return dsiAccess(ind);

  inline proc dsiAccess(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.dsiMember(ind) then
        halt("array index out of bounds: ", ind);
    var dataInd = getDataIndex(ind);
    //assert(dataInd >= 0);
    //assert(numelm >= 0); // ensure it has been initialized
    //assert(dataInd: uint(64) < numelm: uint(64));
    return data(dataInd);
  }

  proc dsiReindex(d: DefaultRectangularDom) {
    var alias = new DefaultRectangularArr(eltType=eltType, rank=d.rank,
                                         idxType=d.idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    //alias.numelm = numelm;
    //writeln("DR.dsiReindex blk: ", blk, " stride: ",dom.dsiDim(1).stride," str:",str(1));
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i).low;
      alias.blk(i) = (blk(i) * dom.dsiDim(i).stride / str(i)) : d.idxType;
      alias.str(i) = d.dsiDim(i).stride;
    }
    alias.origin = origin:d.idxType;
    alias.computeFactoredOffs();
    return alias;
  }

  proc dsiSlice(d: DefaultRectangularDom) {
    var alias = new DefaultRectangularArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    //alias.numelm = numelm;
    alias.blk = blk;
    alias.str = str;
    alias.origin = origin;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i).low;
      alias.origin += blk(i) * (d.dsiDim(i).low - off(i)) / str(i);
    }
    alias.computeFactoredOffs();
    return alias;
  }

  proc dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    proc isRange(r: range(?e,?b,?s,?a)) param return 1;
    proc isRange(r) param return 0;

    var alias = new DefaultRectangularArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable,
                                         dom=d, noinit=true);
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
    return alias;
  }

  proc dsiReallocate(d: domain) {
    if (d._value.type == dom.type) {
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
      //numelm = copy.numelm;
      delete copy;
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
    return rad;
  }
}

proc DefaultRectangularDom.dsiSerialReadWrite(f /*: Reader or Writer*/) {
  f 
      <~> 
        new ioLiteral("{") 
            <~> 
                ranges(1);
  for i in 2..rank do
    f <~> new ioLiteral(", ") <~> ranges(i);
  f <~> new ioLiteral("}");
}

proc DefaultRectangularDom.dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
proc DefaultRectangularDom.dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }

proc DefaultRectangularArr.dsiSerialReadWrite(f /*: Reader or Writer*/) {
  proc recursiveArrayWriter(in idx: rank*idxType, dim=1, in last=false) {
    type strType = chpl__signedType(idxType);
    var makeStridePositive = if dom.ranges(dim).stride > 0 then 1:strType else (-1):strType;
    if dim == rank {
      var first = true;
      if debugDefaultDist && f.writing then f.writeln(dom.ranges(dim));
      for j in dom.ranges(dim) by makeStridePositive {
        if first then first = false; else f <~> new ioLiteral(" ");
        idx(dim) = j;
        f <~> dsiAccess(idx);
      }
    } else {
      for j in dom.ranges(dim) by makeStridePositive {
        var lastIdx =  dom.ranges(dim).last;
        idx(dim) = j;
        recursiveArrayWriter(idx, dim=dim+1,
                             last=(last || dim == 1) && (j == lastIdx));
      }
    }
    if !last && dim != 1 then
      f <~> new ioNewline();
  }
  const zeroTup: rank*idxType;
  recursiveArrayWriter(zeroTup);
}

proc DefaultRectangularArr.dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
proc DefaultRectangularArr.dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }

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

proc DefaultRectangularArr.doiCanBulkTransferStride() {
  if debugDefaultDistBulkTransfer then writeln("In DefaultRectangularArr.doiCanBulkTransferStride()");
  //If there is no stride we return false so as to rely in the original BulkTransfer (without stride)
  if getStrideLevels(assertWholeDim(this)) == 0 then return false;    
  return true;
}

proc DefaultRectangularArr.doiBulkTransfer(B) {
    if debugDefaultDistBulkTransfer then
    writeln("In DefaultRectangularArr.doiBulkTransfer");

  const Adims = dom.dsiDims();
  var Alo: rank*dom.idxType;
  for param i in 1..rank do
    Alo(i) = Adims(i).first;

  const Bdims = B.domain.dims();
  var Blo: rank*dom.idxType;
  for param i in 1..rank do
    Blo(i) = Bdims(i).first;

  const len = dom.dsiNumIndices:int(32);
  if debugBulkTransfer {
    const elemSize =sizeof(B._value.eltType);
    //writeln("In doiBulkTransfer(): Alo=", Alo);
//    writeln(", Blo=", Blo);
    //writeln(", len=", len);
//    writeln(", elemSize=", elemSize);
  }

  // NOTE: This does not work with --heterogeneous, but heterogeneous
  // compilation does not work right now.  The calls to chpl_comm_get
  // and chpl_comm_put should be changed once that is fixed.
  if this.data.locale.id==here.id {
    if debugDefaultDistBulkTransfer then //See bug in test/users/alberto/rafatest2.chpl
      writeln("\tlocal get() from ", B._value.locale.id);
    var dest = this.data;
    var src = B._value.data;
    __primitive("chpl_comm_get",
                __primitive("array_get", dest, getDataIndex(Alo)),
                B._value.data.locale.id,
                __primitive("array_get", src, B._value.getDataIndex(Blo)),
                len);
  }else if B._value.data.locale.id==here.id {
    if debugDefaultDistBulkTransfer then
      writeln("\tlocal put() to ", this.locale.id);
    var dest = this.data;
    var src = B._value.data;
    __primitive("chpl_comm_put",
                __primitive("array_get", src, B._value.getDataIndex(Blo)),
                this.data.locale.id,
                __primitive("array_get", dest, getDataIndex(Alo)),
                len);
  } else on this.data.locale {
    if debugDefaultDistBulkTransfer then
      writeln("\tremote get() on ", here.id, " from ", B.locale.id);
    var dest = this.data;
    var src = B._value.data;
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
*/

proc DefaultRectangularArr.doiBulkTransferStride(B,aFromBD=false, bFromBD=false) {
  if debugDefaultDistBulkTransfer then 
    writeln("In DefaultRectangularArr.doiBulkTransferStride ");

  extern proc sizeof(type x): int;
  if debugBulkTransfer then
    writeln("In doiBulkTransferStride: ");
 
  const Adims = dom.dsiDims();
  var Alo: rank*dom.idxType;
  for param i in 1..rank do
    Alo(i) = Adims(i).first;
  
  const Bdims = B.domain.dims();
  var Blo: rank*dom.idxType;
  for param i in 1..rank do
    Blo(i) = Bdims(i).first;
  
  if debugDefaultDistBulkTransfer then
    writeln("\tlocal get() from ", B.locale.id);
  
  var dstCompRank, srcCompRank:[1..rank] bool;
  var stridelevels:int(32);
  
  dstCompRank = assertWholeDim(this);
  srcCompRank = assertWholeDim(B._value);

  /* If the stridelevels in source and destination arrays are different, we take the larger*/
  stridelevels=max(getStrideLevels(dstCompRank),B._value.getStrideLevels(srcCompRank));
  if debugDefaultDistBulkTransfer then 
    writeln("In DefaultRectangularArr.doiBulkTransferStride, stridelevels: ",stridelevels);
  
  //These variables should be actually of size stridelevels+1, but stridelevels is not param...
  
  var srcCount, dstCount:[1..rank+1] int(32);
  var dstAux:bool = false;
  var srcAux:bool = false;
  /* We now obtain the count arrays for source and destination arrays */
    if (dom.dsiDim(rank).stride>1 && B._value.dom.dsiDim(rank).stride==1)
    {
      if stridelevels < rank then stridelevels+=1;
     dstAux = true;
    }
    else if (B._value.dom.dsiDim(rank).stride>1 && dom.dsiDim(rank).stride==1)
    {
      if stridelevels < rank then stridelevels+=1;
      srcAux = true;
    }
  
  dstCount= getCount(stridelevels,dstCompRank,dstAux);
  srcCount= B._value.getCount(stridelevels,srcCompRank,srcAux);
  /*Then the Stride arrays for source and destination arrays*/

  var dstStride, srcStride: [1..rank] int(32);
  /*When the source and destination arrays have different sizes 
    (example: A[1..10,1..10] and B[1..20,1..20]), the count arrays obtained are different,
    so we have to calculate the minimun count array */
  //Note that we use equal function equal instead of dstCount==srcCount due to the latter fails
  //The same for the array assigment (using assing function instead of srcCount=dstCount)
  
  if !equal(dstCount, srcCount, rank+1) //For different size arrays
  {
      for h in 1..stridelevels+1
      {
	if dstCount[h]>srcCount[h]
        {
	  assig(dstCount,srcCount, stridelevels+1);
          break;
        }
        else if dstCount[h]<srcCount[h] then break; 
      }
  }
    
  if (aFromBD && dom.stridable) then dstStride = getStride2(dstCompRank, dstCount,stridelevels);
  else dstStride = getStride(dstCompRank,dstCount,stridelevels);
  
  if (bFromBD && B._value.dom.stridable) then srcStride = B._value.getStride2(dstCompRank,dstCount,stridelevels);
  else srcStride = B._value.getStride(srcCompRank,dstCount,stridelevels);
  
 doiBulkTransferStrideComm(B, stridelevels, dstStride, srcStride, dstCount, srcCount, Alo, Blo);
}

 proc DefaultRectangularArr.doiBulkTransferStrideComm(B, stridelevels, dstStride/*:(rank+1)*int(32)*/, srcStride/*:(rank+1)*int(32)*/, dstCount/*:(rank+1)*int(32)*/, srcCount/*:(rank+1)*int(32)*/, Alo, Blo)
 { 
 //writeln("Locale: ", here.id, " stridelvl: ", stridelevels, " DstStride: ", dstStride," SrcStride: ",srcStride, " Count: ", dstCount, " dst.Blk: ",blk, " src.Blk: ",B._value.blk/*, " dom: ",dom.dsiDims()," B.blk: ",B._value.blk," B.dom: ",B._value.dom.dsiDims()*/);
  if this.data.locale==here // IF 1
  {
    var dest = this.data;
    var src = B._value.data;
    
    var dststr=dstStride._value.data;
    var srcstr=srcStride._value.data;
    var cnt=dstCount._value.data;

    if debugBulkTransfer {
      writeln("Case 1");
      writeln("Locale:",here.id,"stridelevel: ", stridelevels);
      writeln("Locale:",here.id,"Count: ",dstCount);
      writeln("Locale:",here.id," dststrides: ",dstStride);
      writeln("Locale:",here.id,",srcstrides: ",srcStride);
    }
    var srclocale =B._value.data.locale.id : int(32);
       __primitive("chpl_comm_gets",
                    __primitive("array_get",dest, getDataIndex(Alo)),
                    __primitive("array_get",dststr,dstStride._value.getDataIndex(1)), 
		    srclocale,
                    __primitive("array_get",src, B._value.getDataIndex(Blo)),
                    //__primitive("array_get",srcstr,dstStride._value.getDataIndex(1)),
                    __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)),
                    __primitive("array_get",cnt, dstCount._value.getDataIndex(1)),
                    stridelevels);
  } //END IF 1
  else if B._value.data.locale==here //IF 2
  {
    if debugDefaultDistBulkTransfer then
      writeln("\tlocal put() to ", this.locale.id);
    
    var dststr=dstStride._value.data;
    var srcstr=srcStride._value.data;
    var cnt=srcCount._value.data;
    
    if debugBulkTransfer {
      writeln("Case 2");
      writeln("stridelevel: ",stridelevels);
      writeln("Count: ",srcCount);
      writeln("dststrides: ",dstStride);
      writeln("srcstrides: ",srcStride);
      writeln("Blk: ",blk);
    }
    
    var dest = this.data;
    var src = B._value.data;
    var destlocale =this.data.locale.id : int(32);

    __primitive("chpl_comm_puts",
      		  __primitive("array_get",dest,getDataIndex(Alo)),
      		  __primitive("array_get",dststr,dstStride._value.getDataIndex(1)),
                  destlocale,
                  __primitive("array_get",src,B._value.getDataIndex(Blo)),
      		  __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)),
      		  __primitive("array_get",cnt, srcCount._value.getDataIndex(1)),
      		  stridelevels);
  } //END IF 2
  else on this.data.locale
  {   
    var dest = this.data;
    var src = B._value.data;

    //We are in a locale that doesn't store neither A nor B so we need to copy the auxiliarry
    //arrays to the locale that hosts A. This should translate into some more gets...
    //COUNT
    var count:[1..(stridelevels+1)] int(32);
    count=dstCount; //    assig(count,dstCount,stridelevels+1);
  
    //DSTSTRIDES AND SRCSTRIDE
    var dststrides,srcstrides:[1..stridelevels] int(32);
    srcstrides=srcStride; //    assig(srcstrides,srcStride,stridelevels);
    dststrides=dstStride; //    assig(dststrides,dstStride,stridelevels);
    
    var dststr=dststrides._value.data;
    var srcstr=srcstrides._value.data;
    var cnt=count._value.data;
    
    if debugBulkTransfer {
      writeln("Case 3");
      writeln("stridelevel: ", stridelevels);
      writeln("Count: ",count);
      writeln("dststrides: ",dststrides);
      writeln("srcstrides: ",srcstrides);
    }
    
    var srclocale =B._value.data.locale.id : int(32);
       __primitive("chpl_comm_gets",
                    __primitive("array_get",dest, getDataIndex(Alo)),
                    __primitive("array_get",dststr,dststrides._value.getDataIndex(1)), 
                    srclocale,
                    __primitive("array_get",src, B._value.getDataIndex(Blo)),
                    __primitive("array_get",srcstr,dststrides._value.getDataIndex(1)),
                    __primitive("array_get",cnt, count._value.getDataIndex(1)),
                    stridelevels);   
  }//END ELSE1
 // writeln(" Fin Locale ",here.id);
}

proc DefaultRectangularArr.isDefaultRectangular() param{return true;}

/* This function returns the stride level for the default rectangular array. */
proc DefaultRectangularArr.getStrideLevels(rankcomp):int(32) where rank == 1
{
  if dom.dsiStride==1 then return 0;
  else return 1;
}

proc DefaultRectangularArr.getStrideLevels(rankcomp):int(32) where rank > 1 
{
  var stridelevels:int(32);
 
  if (dom.dsiStride(rank)>1 && dom.dsiDim(rank).length>1)||(blk(rank)>1 && dom.dsiDim(rank).length>1) then stridelevels+=1;
 
  for i in 2..rank by -1{ //writeln("distance[",i,"]=",distance[i]);
    if (!rankcomp[i] && dom.dsiDim(i-1).length>1 && !distance(i)) then stridelevels+=1;
    else if(dom.dsiDim(i).length>1 && !distance(i)) then stridelevels+=1;
  }
 if stridelevels==0 then stridelevels=1; 
 return stridelevels; 
}


/* This function returns the count array for the default rectangular array. */
proc DefaultRectangularArr.getCount(stridelevels:int(32), rankcomp:[], aux = false):(rank+1)*int(32) where rank ==1
{
  var c: (rank+1)*int(32);
  c[1]=1;
  c[2]=dom.dsiDim(1).length:int(32); 
  return c;
}

proc DefaultRectangularArr.getCount(stridelevels:int(32), rankcomp:[], aux = false):(rank+1)*int(32) where rank >1
{
  var c: (rank+1)*int(32);
  var cont:int(32)=0;
  var tmp:int(32)=0;
  for i in 1..rank do
  
  if (dom.dsiStride(rank)>1 && dom.dsiDim(rank).length>1)||(blk(rank)>1 && dom.dsiDim(rank).length>1) 
    {c[1]=1; cont=1;}
  tmp = cont;

  if blk(rank)> 1 then if(this.dom.dsiDim(rank).length:int(32)==1) then cont+=1;
  
  for i in 0+tmp..stridelevels do
    if cont == rank then
      if (rankcomp[cont]&&dom.dsiDim(cont).stride==1) then c[i+1]=1;
      else c[i+1]=dom.dsiDim(1).length:int(32); 
    else
      {
        c[i+1]=this.dom.dsiDim(rank-cont+tmp).length:int(32);
        
	for h in 2..rank-cont+tmp by -1:int(32){
	  if( (distance(h)&&dom.dsiDim(h-1+tmp).length>1 && (!aux || h!=rank)) || (dom.dsiDim(h).length==1&&(h)!=rank))
	    {
	      c[i+1]*=dom.dsiDim(h-1).length:int(32);
	      cont=rank-h+2;
	    }
	  else
	    {
	      cont=rank-h+1+tmp;
	      break;
	    }
        }
      }
    if blk(rank)>1 then c[1]=1;
  
  return c;
}

/* This function returns the stride array for the default rectangular array. */
proc DefaultRectangularArr.getStride(rankcomp:[],cnt:[],levels:int(32))
{
  var c: (rank)*int(32); 
  var h=1;
  var acum=1;
  //Special case. See test/users/alberto/test_rank_change2.chpl (example 12)
  if (blk(rank)>1 && dom.dsiDim(rank).length>1)
  {
    c[h]=blk(rank):int(32);//c[1]=1;
    h+=1;
  }
  else if (cnt[1]==1 && dom.dsiDim(rank).length>1)
  {
    c[h]=dom.dsiDim(rank).stride:int(32);
    h+=1;
  }
    
  for i in 2..rank by -1:int(32){
    if (levels>=h)
    {
     if (cnt[h]==dom.dsiDim(i).length*acum )//&& dom.dsiDim(i-1).length>1)
      {
        if(dom.dsiDim(i-1).length==1)
        {
          for t in 1..(i-1) by -1 do
            if dom.dsiDim(t).length!=1
            {
              c[h]=blk(t):int(32)* dom.dsiDim(t).stride:int(32);
              h+=1;
              acum=1;
              break;
            }
        }
        else
        {
           c[h]=blk(i-1):int(32)* dom.dsiDim(i-1).stride:int(32);
          h+=1;
          acum=1;
        }
      }
      else acum=acum*dom.dsiDim(i).length;
    }
  }
  return c;
}


proc DefaultRectangularArr.getStride2(rankcomp:[],cnt:[], levels:int(32)) where rank==1 {return dom.dsiStride:int(32);}
proc DefaultRectangularArr.getStride2(rankcomp:[],cnt:[], levels:int(32)):(rank)*int(32) where rank > 1
{
  var c: rank*int(32); 
  var h=1;
  var acum=1;
  if (cnt[h]==1 || blk[rank]>1) && dom.dsiDim(rank).length>1
  {
    c[h]=blk[rank]:int(32);
    h+=1;
  }
  
  for i in 2..rank by -1:int(32){
    if (levels>=h)
    {
      if (cnt[h]==dom.dsiDim(i).length*acum && dom.dsiDim(i-1).length>1)
      {
        c[h]=blk(i-1):int(32);
        h+=1;
        acum=1;
      }
      else acum=acum*dom.dsiDim(i).length;
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
proc DefaultRectangularArr.assertWholeDim(d) where rank==1
{
  return true;
}

proc DefaultRectangularArr.assertWholeDim(d) where rank>1
{
 var c:d.rank*bool;
  for i in 2..rank do
    if (d.dom.dsiDim(i).length==d.blk(i-1)/d.blk(i) && dom.dsiStride(i)==1) then c[i]=true;

  return c;
}

/* This function checks if the stride in dimension 'x' is the same as the distance
between the last element in dimension 'x' and  the first in dimension 'x-1'.
If the distances are equal, we can aggregate these two dimmensions. 
Example: 
array A, the domain is D=[1..6, 1..6, 1..6]
Let's A[1..6 by 2, 1..6, 1..6 by 2], then, distance(3) returns true, due to when jumping from
row to row (from the last element of one row, to the first element of the next one) the stride
is the same than when jumping from elements inside the row. 
*/
proc DefaultRectangularArr.distance(x: int)
{
  var cont:bool=false;
  if dom.dsiDim(x-1).length==1 then cont=false;//cont=true;
  else
  {
    if blk(rank)==1
    {
      if (((blk(x-1)*dom.dsiStride(x))/blk(x) - (1+dom.dsiDim(x).last - dom.dsiDim(x).first)) == dom.dsiStride(x)-1)
        && dom.dsiDim(x).length>1 && (dom.dsiStride(x-1)==1||dom.dsiDim(x-1).length==1 )
        then cont=true;
    }
    else
    {
      if ((blk(x-1)*dom.dsiStride(x))/blk(x) - (1+dom.dsiDim(x).last - dom.dsiDim(x).first) == dom.dsiStride(x)-1)
        && dom.dsiDim(x).length>1 && (dom.dsiStride(x-1)==1||dom.dsiDim(x-1).length==1 )
        then cont=true;
    }
  }
  return cont;
}

/*This function checks if the first 'tam' elements from array 'd2' are equal than array 'd1' 
This is a pending question: this functions replaces the operator == due to it stopped working
at some point. We will be grateful if anyone can shed light on this problem.
*/
proc DefaultRectangularArr.equal(d1:[]/*(rank+1)*int(32)*/,d2:[]/*(rank+1)*int(32)*/,tam: int(32))
{
  var c:bool=true;
  for i in 1..tam do if d1[i]!=d2[i]{ c=false;break;}
  return c;
}


/*This function assigns the first 'tam' elements from array 'd2' to array 'd1' 
This is a pending question: this functions replaces the operator = due to it stopped working
at some point. We will be grateful if anyone can shed light on this problem.
*/
proc DefaultRectangularArr.assig(d1:[]/*(rank+1)*int(32)*/,d2:[]/*(rank+1)*int(32)*/,tam: int(32))
{
  for i in 1..tam do d1[i]=d2[i];
}

proc DefaultRectangularArr.doiBulkTransferStride(B) where B._value.isBlockDist()
{
  if debugDefaultDistBulkTransfer then 
    writeln("In DefaultRectangularArr.doiBulkTransferStride where B._value.isBlockDist() ");
  var r2: rank * range(stridable = true);
  for j in B._value.dom.dist.targetLocDom
  {
    var inters=B._value.dom.locDoms[j].myBlock;
    if(inters.numIndices>0 && dom.dsiNumIndices >0)
    {
      var sa,ini_src,end_src:rank*int(64);
      if rank==1
      {
        ini_src[1]=corr_inverse(inters.first,B._value,1);
        end_src[1]=corr_inverse(inters.last,B._value,0);
        sa[1] =dom.dsiStride;
      }
      else
      {
        ini_src=corr_inverse(inters.first,B._value,1);
        end_src=corr_inverse(inters.last,B._value,0);
        sa =dom.dsiStride;
      }
      
      for param t in 1..rank do
        r2[t] = (ini_src[t]..end_src[t] by sa[t]);

      const d2 ={(...r2)};
      const slice2 = this.dsiSlice(d2._value);

      slice2.doiBulkTransferStride(B._value.locArr[j].myElems,false,true);
      delete slice2;
    }
  }
}

//These helping function is the main contribution of Juan Lopez, and later improved by Alberto. 
//It is used to compute the low and high values of the ranges of the destination subdomain from 
//the low and high values of the source subdomain and low values of the source whole domain.
//When computing the low bounds, parameter low has to be =1 to account for unaligned low values.

proc DefaultRectangularArr.corr_inverse (b,B,low) where rank ==1
{
  //It finds out the initial domain coordinate and strides of destination Domain (of A)
  var la=dom.ranges(1).low;
  var sa=dom.dsiStride;
  var lb=B.dom.whole.low;
  var sb=B.dom.whole.stride;

  var a=la+((b-lb)/sb)*sa;
  if low==1 then
    if(b-lb)%sb>0 then a=a+sa;
    
  return a;
}

proc DefaultRectangularArr.corr_inverse (b,B,low) where rank > 1
{
  //It finds out the initial domain coordinate and strides of destination Domain (of A)
  var la: rank*int;
  for i in 1..rank do
    la[i]=dom.ranges(i).low;
  
  var sa=dom.dsiStride;
  var lb=B.dom.whole.low;
  var sb=B.dom.whole.stride;
  var a: rank * int; 
  forall i in 1..rank do a[i]=la[i]+((b[i]-lb[i])/sb[i])*sa(i);
  return a;
}

}
