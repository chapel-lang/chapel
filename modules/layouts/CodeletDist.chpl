use DSIUtil;
config param debugCodeletDist = false;
config param debugILDataPar = false;

var ILLow : int;
var ILHigh : int;
var ILStride : int;

class CodeletDist: BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool)
    return new CodletRectangularDom(rank, idxType, stridable, this);

  proc dsiIndexLocale(ind) return this.locale;

  proc dsiClone() return this;

  proc dsiAssign(other: this.type) { }

  proc dsiCreateReindexDist(newSpace, oldSpace) return this;
  proc dsiCreateRankChangeDist(param newRank, args) return this;
}

class CodletRectangularDom: BaseRectangularDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: CodeletDist;
  var ilindex : int;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  proc linksDistribution() param return false;
  proc dsiLinksDistribution()     return false;

  proc CodletRectangularDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
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

    if rank == 1 {
      /*
      var ILLow = ranges(1).low;
      var ILHigh = ranges(1).high;
      var ILStride = ranges(1).stride;
      */
      ILLow = ranges(1).low;
      ILHigh = ranges(1).high;
      ILStride = ranges(1).stride;

      pragma "while index"
      var ILIndex: int;

      on __primitive("chpl_target_il", ILLow, ILHigh, ILStride, ILIndex) do {
        yield ILIndex;
      }
    }
    else 
      compilerError("can't support ranks > 1");
    /* else {
        
      var block: rank*range(idxType);
      for param i in 1..rank do
        block(i) = 0..ranges(i).length-1;
      on __primitive("chpl_target_il", 
    }
    */


/*
     if rank == 1 {
          yield tuple(0..ranges(1).length-1);
        } else {
          var block: rank*range(idxType);
          for param i in 1..rank do
            block(i) = 0..ranges(i).length-1;
          yield block;
        }
        */

    /*
    if debugCodeletDist then
      writeln("*** In domain/array leader code:"); // this = ", this);
    const numTasks = if dataParTasksPerLocale==0 then here.numCores
                     else dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    const minIndicesPerTask = dataParMinGranularity;
    if debugILDataPar {
      writeln("### numTasks = ", numTasks);
      writeln("### ignoreRunning = ", ignoreRunning);
      writeln("### minIndicesPerTask = ", minIndicesPerTask);
    }

    if debugCodeletDist then
      writeln("    numTasks=", numTasks, " (", ignoreRunning,
              "), minIndicesPerTask=", minIndicesPerTask);

    var (numChunks, parDim) = _computeChunkStuff(numTasks, ignoreRunning,
                                                 minIndicesPerTask, ranges);
    if debugCodeletDist then
      writeln("    numChunks=", numChunks, " parDim=", parDim,
              " ranges(", parDim, ").length=", ranges(parDim).length);

    if debugILDataPar then writeln("### numChunks=", numChunks, " (parDim=", parDim, ")");

    if numChunks == 0 then return;	// Avoid problems with 0:uint(64) - 1 below.

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
        if debugCodeletDist then
          writeln("*** DI: locBlock = ", locBlock);
        coforall chunk in 0..numChunks-1 {
          var tuple: rank*range(idxType) = locBlock;
          const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                        numChunks, chunk,
                                        locBlock(parDim).high);
          tuple(parDim) = lo..hi;
          if debugCodeletDist then
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
  */
  }
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {

    if rank == 1 {
      //var block : range(idxType=idxType, stridable=stridable);
      //var block = follower(1).low:idxType..follower(1).high:idxType;
      //for myIndex in follower do
      //for myIndex in block do
       // yield myIndex;
      yield followThis;
    } else {
      compilerError("can't support ranks > 1 (yet)");
    }

    /*
   var block: rank*range(idxType=idxType, stridable=stridable);
     for  param i in 1..rank do {
        block(i) = ranges(i).low+follower(i).low:idxType..ranges(i).low+follower(i).high:idxType;
      }

    if rank == 1 {
      for i in block {
        yield i;
      }
    } else {
      for i in these_help(1, block) {
        yield i;
      }
    }
    */

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
    return new DefaultILRectangularArr(eltType=eltType, rank=rank, idxType=idxType,
                                    stridable=stridable, dom=this);
  }

  proc dsiBuildRectangularDom(param rank: int, type idxType, param stridable: bool,
                            ranges: rank*range(idxType,
                                               BoundedRangeType.bounded,
                                               stridable)) {
    var dom = new CodletRectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      dom.ranges(i) = ranges(i);
    return dom;
  }
}

class DefaultILRectangularArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var dom : CodletRectangularDom(rank=rank, idxType=idxType,
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

  // can the compiler create this automatically?
  proc dsiGetBaseDom() return dom;

  proc dsiDestroyData() {
    if dom.dsiNumIndices > 0 {
      pragma "no copy" pragma "no auto destroy" var dr = data;
      pragma "no copy" pragma "no auto destroy" var dv = __primitive("get ref", dr);
      pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, 0);
      pragma "no copy" pragma "no auto destroy" var ev = __primitive("get ref", er);
      if (chpl__maybeAutoDestroyed(ev)) {
        for i in 0..dom.dsiNumIndices-1 {
          pragma "no copy" pragma "no auto destroy" var dr = data;
          pragma "no copy" pragma "no auto destroy" var dv = __primitive("get ref", dr);
          pragma "no copy" pragma "no auto destroy" var er = __primitive("array_get", dv, i);
          pragma "no copy" pragma "no auto destroy" var ev = __primitive("get ref", er);
          chpl__autoDestroy(ev);
        }
      }
    }
    delete data;
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
    //for follower in dom.these(tag) do
    //  yield follower;
    var follower = dom.these(tag);
    yield follower;
  }

  iter these(param tag: iterKind, followThis) var where tag == iterKind.follower {
    /*
    if debugCodeletDist then
      writeln("*** In array follower code:"); // [\n", this, "]");
    for i in dom.these(tag=iterator.follower, follower) {
      __primitive("noalias pragma");
      yield dsiAccess(i);
    }
    */
    for i in dom.these(tag=iterKind.follower, followThis) {
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

    // Initialize 
    //writeln("About to initialize codelet runtime ...");
    //chpl_codelet_init();

    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim).alignedLow;
      str(dim) = dom.dsiDim(dim).stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
    computeFactoredOffs();
    var size = blk(1) * dom.dsiDim(1).length;
    data = new _ddata(eltType);
    //assert(size >= 0);
    //assert(size:int(64) <= max(int):int(64));
    //numelm = size: int;
    data.init(size);
  }

  pragma "inline"
  proc getDataIndex(ind: idxType ...1) where rank == 1
    return getDataIndex(ind);

  pragma "inline"
  proc getDataIndex(ind: rank* idxType) {
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
  pragma "inline"
  proc dsiAccess(ind: idxType ...1) var where rank == 1
    return dsiAccess(ind);

  pragma "inline"
  proc dsiAccess(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.dsiMember(ind) then
        halt("array index out of bounds: ", ind);
    var dataInd = getDataIndex(ind);
    //assert(dataInd >= 0);
    //assert(numelm >= 0); // ensure it has been initialized
    //assert(dataInd: uint(64) < numelm: uint(64));
    return data(dataInd);
  }

  proc dsiReindex(d: CodletRectangularDom) {
    var alias = new DefaultILRectangularArr(eltType=eltType, rank=d.rank,
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

  proc dsiSlice(d: CodletRectangularDom) {
    var alias = new DefaultILRectangularArr(eltType=eltType, rank=rank,
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

    var alias = new DefaultILRectangularArr(eltType=eltType, rank=newRank,
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
      var copy = new DefaultILRectangularArr(eltType=eltType, rank=rank,
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
    halt("all dsiLocalSlice calls on DefaultILRectangulars should be handled in ChapelArray.chpl");
  }
}

proc CodletRectangularDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

proc DefaultILRectangularArr.dsiSerialWrite(f: Writer) {
  proc recursiveArrayWriter(in idx: rank*idxType, dim=1, in last=false) {
    var makeStridePositive = if dom.ranges(dim).stride > 0 then 1 else -1;
    if dim == rank {
      var first = true;
      if debugCodeletDist then f.writeln(dom.ranges(dim));
      for j in dom.ranges(dim) by makeStridePositive {
        if first then first = false; else f.write(" ");
        idx(dim) = j;
        f.write(dsiAccess(idx));
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
      f.writeln();
  }
  const zeroTup: rank*idxType;
  recursiveArrayWriter(zeroTup);
}
