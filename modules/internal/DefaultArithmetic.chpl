use DSIUtil;
config param debugDefaultDist = false;

class DefaultDist: BaseDist {
  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool)
    return new DefaultArithmeticDom(rank, idxType, stridable, this);

  def dsiNewAssociativeDom(type idxType)
    return new DefaultAssociativeDom(idxType, this);

  def dsiNewOpaqueDom(type idxType)
    return new DefaultOpaqueDom(this);

  def dsiNewSparseDom(param rank: int, type idxType, dom: domain)
    return new DefaultSparseDom(rank, idxType, this, dom);

  def dsiClone() return this;

  def dsiCreateReindexDist(newSpace, oldSpace) return this;
  def dsiCreateRankChangeDist(param newRank, args) return this;
}

//
// Note that the replicated copies are set up in ChapelLocale on the
// other locales.  This just sets it up on this locale.
//
pragma "private" var defaultDist = new dist(new DefaultDist());

class DefaultArithmeticDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: DefaultDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  def linksDistribution() param return false;

  def DefaultArithmeticDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
  }

  def dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    for param i in 1..rank do
      ranges(i) = emptyRange;
  }
  
  // function and iterator versions, also for setIndices
  def dsiGetIndices() return ranges;

  def dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).eltType != x(1).eltType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  def these_help(param d: int) {
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

  def these_help(param d: int, block) {
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

  def these() {
    if rank == 1 {
      for i in ranges(1) do
        yield i;
    } else {
      for i in these_help(1) do
        yield i;
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    if debugDefaultDist then
      writeln("*** In domain leader code:"); // this = ", this);
    // For now we have to set this default here rather than as the
    //  default value in the constructor because we need to create
    //  an array to store the locales (and hence 'here')
    var maxTasks = if maxDataParallelism>0 then maxDataParallelism
                   else here.numCores;
    var limitTasks = limitDataParallelism;
    var minIndicesPerTask = minDataParallelismSize;

    if debugDefaultDist then
      writeln("    maxTasks=", maxTasks, " (", limitTasks,
              "), minIndicesPerTask=", minIndicesPerTask);

    var (numChunks, parDim) = _computeChunkStuff(maxTasks, limitTasks,
                                                 minIndicesPerTask,
                                                 ranges, rank);
    if debugDefaultDist then
      writeln("    numChunks=", numChunks, " parDim=", parDim,
              " ranges(", parDim, ").length=", ranges(parDim).length);

    if debugDefaultDist then
      writeln("*** DI: Using ", numChunks, " chunk(s)");

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
      coforall chunk in 0..numChunks-1 {
	var tuple: rank*range(idxType) = locBlock;
	const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                      numChunks, chunk,
                                      locBlock(parDim).high);
	tuple(parDim) = lo..hi;
        if debugDefaultDist then
          writeln("*** DI: tuple = ", tuple);
	yield tuple;
      }
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    if debugDefaultDist then
      writeln("In domain follower code: Following ", follower);
    var block: ranges.type;
    if stridable {
      for param i in 1..rank do
        block(i) =
          if ranges(i).stride > 0 then
            ranges(i).low+follower(i).low*ranges(i).stride:ranges(i).eltType..ranges(i).low+follower(i).high*ranges(i).stride:ranges(i).eltType by ranges(i).stride
          else
            ranges(i).high+follower(i).high*ranges(i).stride:ranges(i).eltType..ranges(i).high+follower(i).low*ranges(i).stride:ranges(i).eltType by ranges(i).stride;
    } else {
      for  param i in 1..rank do
        block(i) = ranges(i).low+follower(i).low:ranges(i).eltType..ranges(i).low+follower(i).high:ranges(i).eltType;
    }
    if rank == 1 {
      for i in block do
        yield i;
    } else {
      for i in these_help(1, block) do
        yield i;
    }
  }

  def dsiMember(ind: rank*idxType) {
    for param i in 1..rank do
      if !ranges(i).member(ind(i)) then
        return false;
    return true;
  }

  def dsiIndexOrder(ind: rank*idxType) {
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

  def dsiDims()
    return ranges;

  def dsiDim(d : int)
    return ranges(d);

  // optional, is this necesary? probably not now that
  // homogeneous tuples are implemented as C vectors.
  def dsiDim(param d : int)
    return ranges(d);

  def dsiNumIndices {
    var sum = 1:idxType;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  def dsiLow {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  def dsiHigh {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  def dsiBuildArray(type eltType) {
    return new DefaultArithmeticArr(eltType=eltType, rank=rank, idxType=idxType,
                                    stridable=stridable, dom=this);
  }

  def dsiBuildArithmeticDom(param rank: int, type idxType, param stridable: bool,
                            ranges: rank*range(idxType,
                                               BoundedRangeType.bounded,
                                               stridable)) {
    var dom = new DefaultArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      dom.ranges(i) = ranges(i);
    return dom;
  }
}

class DefaultArithmeticArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var dom : DefaultArithmeticDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var size : idxType;
  var data : _ddata(eltType);
  var noinit: bool = false;

  def canCopyFromDevice param return true;

  // end class definition here, then defined secondary methods below

  // can the compiler create this automatically?
  def dsiGetBaseDom() return dom;

  def dsiDestroyData() {
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

  def these() var {
    if rank == 1 {
      var first = getDataIndex(dom.dsiLow);
      var second = getDataIndex(dom.dsiLow+dom.ranges(1).stride:idxType);
      var step = (second-first):int;
      var last = first + (dom.dsiNumIndices-1) * step:idxType;
      for i in first..last by step do
        yield data(i);
    } else {
      for i in dom do
        yield dsiAccess(i);
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    if debugDefaultDist then
      writeln("*** In array leader code:");// [\n", this, "]");
    // For now we have to set this default here rather than as the
    //  default value in the constructor because we need to create
    //  an array to store the locales (and hence 'here')
    var maxTasks = if maxDataParallelism>0 then maxDataParallelism
                   else here.numCores;
    var limitTasks = limitDataParallelism;
    var minElemsPerTask = minDataParallelismSize;

    if debugDefaultDist then
      writeln("    maxTasks=", maxTasks, " (", limitTasks,
              "), minElemsPerTask=", minElemsPerTask);

    var (numChunks, parDim) = _computeChunkStuff(maxTasks, limitTasks,
                                                 minElemsPerTask,
                                                 dom.ranges, rank);
    if debugDefaultDist then
      writeln("    numChunks=", numChunks, " parDim=", parDim,
              " ranges(", parDim, ").length=", dom.ranges(parDim).length);

    if debugDefaultDist then
      writeln("*** AI: Using ", numChunks, " chunk(s)");

    if numChunks == 1 {
      if rank == 1 {
	yield tuple(0..dom.ranges(1).length-1);
      } else {
	var block: rank*range(idxType);
	for param i in 1..rank do
	  block(i) = 0..dom.ranges(i).length-1;
	yield block;
      }
    } else {
      var locBlock: rank*range(idxType);
      for param i in 1..rank do
	locBlock(i) = 0:dom.ranges(i).low.type..#(dom.ranges(i).length);
      if debugDefaultDist then
	writeln("*** AI: locBlock = ", locBlock);
      coforall chunk in 0..numChunks-1 {
	var tuple: rank*range(idxType) = locBlock;
	const (lo,hi) = _computeBlock(locBlock(parDim).length,
                                      numChunks, chunk,
                                      locBlock(parDim).high);
	tuple(parDim) = lo..hi;
	if debugDefaultDist then
	  writeln("*** AI: tuple = ", tuple);
	yield tuple;
      }
    }
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    if debugDefaultDist then
      writeln("*** In array follower code:"); // [\n", this, "]");
    for i in dom.these(tag=iterator.follower, follower) do
      yield dsiAccess(i);
  }

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }
  
  // change name to setup and call after constructor call sites
  // we want to get rid of all initialize functions everywhere
  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim)._low;
      str(dim) = dom.dsiDim(dim)._stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
    computeFactoredOffs();
    size = blk(1) * dom.dsiDim(1).length;
    data = new _ddata(eltType);
    data.init(size);
  }

  pragma "inline"
  def getDataIndex(ind: idxType ...1) where rank == 1
    return getDataIndex(ind);

  pragma "inline"
  def getDataIndex(ind: rank* idxType) {
    var sum = origin;
    if stridable {
      for param i in 1..rank do
        sum += (ind(i) - off(i)) * blk(i) / str(i):idxType;
    } else {
      for param i in 1..rank do
        sum += ind(i) * blk(i);
      sum -= factoredOffs;
    }
    return sum;
  }

  // only need second version because wrapper record can pass a 1-tuple
  pragma "inline"
  def dsiAccess(ind: idxType ...1) var where rank == 1
    return dsiAccess(ind);

  pragma "inline"
  def dsiAccess(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.dsiMember(ind) then
        halt("array index out of bounds: ", ind);
    return data(getDataIndex(ind));
  }

  def dsiReindex(d: DefaultArithmeticDom) {
    var alias = new DefaultArithmeticArr(eltType=eltType, rank=d.rank,
                                         idxType=d.idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i)._low;
      alias.blk(i) = (blk(i) * dom.dsiDim(i)._stride / str(i)) : d.idxType;
      alias.str(i) = d.dsiDim(i)._stride;
    }
    alias.origin = origin:d.idxType;
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiSlice(d: DefaultArithmeticDom) {
    var alias = new DefaultArithmeticArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    alias.origin = origin;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i)._low;
      alias.origin += blk(i) * (d.dsiDim(i)._low - off(i)) / str(i);
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new DefaultArithmeticArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable,
                                         dom=d, noinit=true);
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..args.size {
      if isRange(args(j)) {
        alias.off(i) = d.dsiDim(i)._low;
        alias.origin += blk(j) * (d.dsiDim(i)._low - off(j)) / str(j);
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

  def dsiReallocate(d: domain) {
    if (d._value.type == dom.type) {
      var copy = new DefaultArithmeticArr(eltType=eltType, rank=rank,
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
      size = copy.size;
      dsiDestroyData();
      data = copy.data;
      delete copy;
    } else {
      halt("illegal reallocation");
    }
  }
}

def DefaultArithmeticDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

def DefaultArithmeticArr.dsiSerialWrite(f: Writer) {
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim)._low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank)._high - dom.dsiDim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim)._high - dom.dsiDim(dim)._stride:idxType) {
          i(dim) += dom.dsiDim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}
