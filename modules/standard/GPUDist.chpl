_extern def chpl_init_accelerator();
_extern def getThreadID_x() : int(32);
_extern def getThreadID_y() : int(32);
_extern def getThreadID_z() : int(32);
_extern def getBlockID_x() : int(32);
_extern def getBlockID_y() : int(32);
_extern def getBlockID_z() : int(32);
_extern def getBlockSize_x() : int(32);
_extern def getBlockSize_y() : int(32);
_extern def getBlockSize_z() : int(32);

// Initialize the accelerator device from the beginning
chpl_init_accelerator();

pragma "data class"
class _gdata {
  type eltType;
  def ~_gdata() {
    __primitive("gpu_free", this);
  }
  pragma "inline" def init(size: integral) {
    __primitive("gpu_alloc", this, size, eltType);
  }
  pragma "inline" def this(i: integral) var {
    return __primitive("get_gpu_value", this, i);
  }
}

config param debugGPUDist = false; // internal development flag (debugging)

class GPUDist: BaseDist {

  const threadPerBlock : int;

  def GPUDist(param rank : int, 
	      type idxType = int(64), 
	      threadsPerBlock=128) {
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUDist");
    threadPerBlock = threadsPerBlock;
  }

  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new GPUArithmeticDom(rank, idxType, stridable, this);
  }
}

//
// Note that the replicated copies are set up in ChapelLocale on the
// other locales.  This just sets it up on this locale.
//

class GPUArithmeticDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  var low_val : int(64);

  const threadPerBlock : int;
  const numBlocks : int;

  def GPUArithmeticDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
  }

  def dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    for param i in 1..rank do
      ranges(i) = emptyRange;
  }
  
  def dsiGetIndices() return ranges;

  def dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).eltType != x(1).eltType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;

    low_val = ranges(1).low;
    threadPerBlock = dist.threadPerBlock;
    if rank == 1 then {
      numBlocks = (ranges(1).length:int + threadPerBlock - 1) / threadPerBlock;
    }
    else if rank == 2 then {
      numBlocks = (ranges(1).length:int + threadPerBlock - 1) / threadPerBlock;
    }
    else
      compilerError("Can't support dimensions higher than 1 currently");
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
    if rank == 1 {
      var nBlocks = numBlocks;
      var threadsPerBlock = threadPerBlock;
      var size = ranges(1).length;
      on __primitive("chpl_on_gpu",nBlocks,threadsPerBlock) do {
        var tid = getBlockID_x() * getBlockSize_x() + getThreadID_x();
        if tid < size then
          yield tid;
      }
    } 
    else {
      compilerError("No support for multi-dimensional arrays (yet)");
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    if rank == 1 {
      yield follower + low_val;
    } 
    else {
      compilerError("No support for multi-dimensional arrays (yet)");
    }
  }

  def dsiMember(ind: idxType) where rank == 1 {
    if !ranges(1).member(ind) then
      return false;
    return true;
  }

  def dsiMember(ind: rank*idxType) {
    for param i in 1..rank do
      if !ranges(i).member(ind(i)) then
        return false;
    return true;
  }

  def dsiIndexOrder(ind: idxType) where rank == 1 {
    return ranges(1).order(ind);
  }

  def dsiIndexOrder(ind: rank*idxType) {
    var totOrder: idxType;
    var blk: idxType = 1;
    for param d in 1..rank by -1 {
      const orderD = ranges(d).order(ind(d));
      if (orderD == -1) then return orderD;
      totOrder += orderD * blk;
      blk *= ranges(d).length;
    }
    return totOrder;
  }

  def dsiPosition(ind: idxType) where rank == 1 {
    var pos: 1*idxType;
    pos(1) = dsiIndexOrder(ind);
    return pos;
  }

  def dsiPosition(ind: rank*idxType) {
    var pos: rank*idxType;
    for d in 1..rank {
      pos(d) = ranges(d).order(ind(d));
    }
    return pos;
  }

  def dsiDim(d : int)
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
    return new GPUArithmeticArr(eltType=eltType, rank=rank, idxType=idxType, 
		    		stridable=stridable, dom=this, 
				low_val=ranges(1).low);
  }

  def dsiSlice(param stridable: bool, ranges) {
    var d = new GPUArithmeticDom(rank, idxType,
                                     stridable || this.stridable, dist);
    for param i in 1..rank do
      d.ranges(i) = dsiDim(i)(ranges(i));
    return d;
  }

  def dsiRankChange(param rank: int, param stridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var d = new GPUArithmeticDom(rank, idxType, stridable, dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dsiDim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  def translate(off: rank*idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).translate(off(i));
    return x;
  }

  def chpl__unTranslate(off: rank*idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).chpl__unTranslate(off(i));
    return x;
  }

  def interior(off: rank*idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dsiDim(i)._high+1-off(i) < dsiDim(i)._low) ||
          (off(i) < 0) && (dsiDim(i)._low-1-off(i) > dsiDim(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dsiDim(i).interior(off(i));
    }
    return x;
  }

  def exterior(off: rank*idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).exterior(off(i));
    return x;
  }

  def expand(off: rank*idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i).expand(off(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(off: idxType) {
    var x = new GPUArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i).expand(off);
    return x;
  }

  def dsiStrideBy(str : rank*int) {
    var x = new GPUArithmeticDom(rank, idxType, true, dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }

  def dsiStrideBy(str : int) {
    var x = new GPUArithmeticDom(rank, idxType, true, dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str;
    return x;
  }
}

class GPUArithmeticArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;
  param reindexed: bool = true; // may have blk(rank) != 1

  var dom : GPUArithmeticDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var factoredOffs: idxType;
  var data : _gdata(eltType);
  var size : idxType;
  var low_val : int(64);

  def canCopyFromHost param return true;

  def dsiGetBaseDom() return dom;

  def destroyData() {
    delete data;
  }

  def dsiCreateAlias(B: GPUArithmeticArr) {
    var A = B.reindex(dom);
    off = A.off;
    blk = A.blk;
    str = A.str;
    factoredOffs = A.factoredOffs;
    data = A.data;
    delete A;
  }

  def these() var {
    for i in dom do
      yield dsiAccess(i);
  }

  def these(param tag: iterator) where tag == iterator.leader {
      var nBlocks = dom.numBlocks;
      var threadsPerBlock = dom.threadPerBlock;
      var highval = size;
      on __primitive("chpl_on_gpu",nBlocks,threadsPerBlock) do {
	var tid = getBlockID_x() * getBlockSize_x() + getThreadID_x();
	if tid < highval then
	  yield tid;
      }
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
      yield dsiAccess(follower + low_val);
  }

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }

  def initialize() {
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim)._low;
      str(dim) = dom.dsiDim(dim)._stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
    computeFactoredOffs();
    size = blk(1) * dom.dsiDim(1).length;
    data = new _gdata(eltType);
    data.init(size);
  }

  pragma "inline"
  def dsiAccess(ind: idxType ...1) var where rank == 1 {
    return dsiAccess(ind);
  }

  pragma "inline"
  def dsiAccess(ind : rank*idxType) var {
    var sum = ind(1) - factoredOffs; // Hardcoded to support only 1D arrays
    return data(sum);
  }

  def dsiReindex(d: GPUArithmeticDom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dsiDim(i).length != dom.dsiDim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new GPUArithmeticArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable,
                                     reindexed=true, dom=d);
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i)._low;
      alias.blk(i) = (blk(i) * dom.dsiDim(i)._stride / str(i)) : d.idxType;
      alias.str(i) = d.dsiDim(i)._stride;
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiCheckSlice(ranges) {
    for param i in 1..rank do
      if !dom.dsiDim(i).boundsCheck(ranges(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
  }

  def dsiSlice(d: GPUArithmeticDom) {
    var alias = new GPUArithmeticArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         reindexed=reindexed,
                                         dom=d);
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i)._low;
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def checkRankChange(args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    for param i in 1..args.size do
      if isRange(args(i)) then
        if !dom.dsiDim(i).boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  def dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new GPUArithmeticArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, reindexed=true,
                                         dom=d);
    alias.data = data;
    alias.size = size;
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        alias.off(i) = d.dsiDim(i)._low;
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      } else {
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiReallocate(d: domain) {
    if (d._value.type == dom.type) {
      var copy = new GPUArithmeticArr(eltType=eltType, rank=rank,
                                          idxType=idxType,
                                          stridable=d._value.stridable,
                                          reindexed=reindexed, dom=d._value);
      for i in d((...dom.ranges)) do
        copy(i) = dsiAccess(i);
      off = copy.off;
      blk = copy.blk;
      str = copy.str;
      factoredOffs = copy.factoredOffs;
      size = copy.size;
      delete data;
      data = copy.data;
      delete copy;
    } else {
      halt("illegal reallocation");
    }
  }

  def tupleInit(b: _tuple) {
    def _tupleInitHelp(j, param rank: int, b: _tuple) {
      if rank == 1 {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dsiDim(this.rank-rank+1).low + i - 1;
          this(j) = b(i);
        }
      } else {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dsiDim(this.rank-rank+1).low + i - 1;
          _tupleInitHelp(j, rank-1, b(i));
        }
      }
    }

    if rank == 1 {
      for param i in 1..b.size do
        this(this.dom.dsiDim(1).low + i - 1) = b(i);
    } else {
      var j: rank*int;
      _tupleInitHelp(j, rank, b);
    }
  }
}

def GPUArithmeticDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

def GPUArithmeticArr.dsiSerialWrite(f: Writer) {
  if dom.numIndices == 0 then return;
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
