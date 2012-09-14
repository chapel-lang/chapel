// DefaultRectangular.chpl
//
pragma "no use ChapelStandard"
module DefaultRectangular {

use DSIUtil;
config param noRefCount = false;
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

proc DefaultRectangularArr.doiCanBulkTransfer() {
  if dom.stridable then
    for param i in 1..rank do
      if dom.ranges(i).stride != 1 then return false;

  if !isDataContiguous() then return false;

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
  if debugBulkTransfer {
    const elemSize =sizeof(B._value.eltType);
    writeln("In doiBulkTransfer(): Alo=", Alo, ", Blo=", Blo,
            ", len=", len, ", elemSize=", elemSize);
  }

  // NOTE: This does not work with --heterogeneous, but heterogeneous
  // compilation does not work right now.  The calls to chpl_comm_get
  // and chpl_comm_put should be changed once that is fixed.
  if this.data.locale.id==here.id {
    if debugDefaultDistBulkTransfer then
      writeln("\tlocal get() from ", B.locale.id);
    var dest = this.data;
    var src = B._value.data;
    __primitive("chpl_comm_get",
                __primitive("array_get", dest, getDataIndex(Alo)),
                B._value.data.locale.id,
                __primitive("array_get", src, B._value.getDataIndex(Blo)),
                len);
  } else if B._value.data.locale.id==here.id {
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

}
