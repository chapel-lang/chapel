pragma "data class"
pragma "shared array"
class _sdata {
  type eltType;
  pragma "inline" def this(i: integral) var {
    return __primitive("get_gpu_value", this, i);
  }
}

config param debugGPUSharedDist = false; // internal development flag (debugging)

class GPUSharedMemDist: BaseDist {

  def GPUSharedMemDist(param rank : int, 
	      type idxType = int(64)) {
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUSharedDist");
  }

  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new GPUSharedMemDom(rank, idxType, stridable, this);
  }

  def dsiClone() return this;
  def dsiAssign(other: this.type) { }
  def dsiCreateReindexDist(newSpace, oldSpace) return this;
  def dsiCreateRankChangeDist(param newRank, args) return this;
}

class GPUSharedMemDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUSharedMemDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  def linksDistribution() param return false;

  def GPUSharedMemDom(param rank, type idxType, param stridable, dist) {
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
    if ranges(1).idxType != x(1).idxType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
    if rank > 3 then
      compilerError("Can't support dimensions higher than 3 currently");
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
      compilerError("Not supported");
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
      compilerError("Not supported");
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
    return new GPUSharedMemArr(eltType=eltType, rank=rank, idxType=idxType, 
        stridable=stridable, dom=this);
  }

  def dsiRankChange(param rank: int, param stridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var d = new GPUSharedMemDom(rank, idxType, stridable, dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dsiDim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  def dsiBuildArithmeticDom(param rank: int, type idxType,
      param stridable: bool,
      ranges: rank*range(idxType,
        BoundedRangeType.bounded,
        stridable)) {
    if idxType != dist.idxType then
      compilerError("Block domain index type does not match distribution's");
    if rank != dist.rank then
      compilerError("Block domain rank does not match distribution's");

    var dom = new GPUSharedMemDom(rank=rank, idxType=idxType,
        dist=dist, stridable=stridable);
    dom.dsiSetIndices(ranges);
    return dom;
  }

}

class GPUSharedMemArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var dom : GPUSharedMemDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var noinit: bool = false;

  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var factoredOffs: idxType;
  var data : _sdata(eltType);
  var size : idxType;

  def canCopyFromHost param return true;

  def dsiGetBaseDom() return dom;

  def these() var {
    for i in dom do
      yield dsiAccess(i);
  }

  def these(param tag: iterator) where tag == iterator.leader {
    halt("Not supposed to happen");
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    halt("Not supposed to happen");
  }

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim)._low;
      str(dim) = dom.dsiDim(dim)._stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do {
      blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
    }
    computeFactoredOffs();
    size = blk(1) * dom.dsiDim(1).length;
    data = new _sdata(eltType);
  }

  pragma "inline"
  def dsiAccess(ind: idxType ...1) var where rank == 1 {
    return dsiAccess(ind);
  }

  pragma "inline"
  def dsiAccess(ind : rank*idxType) var {
    var sum : int(64);
    for param i in 1..rank do {
      sum += ind(i) * blk(i);
    }
    sum -= factoredOffs;
    return data(sum);
  }

  def dsiReindex(d: GPUSharedMemDom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dsiDim(i).length != dom.dsiDim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new GPUSharedMemArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable, dom=d, noinit=true);
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

  def dsiSlice(d: GPUSharedMemDom) {
    var alias = new GPUSharedMemArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);

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

  def dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new GPUSharedMemArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, dom=d);
    alias.data = data;
    alias.size = size;
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        alias.off(i) = d.dsiDim(i)._low;
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiReallocate(d: domain) {
      halt("reallocation of shared mem arrays not allowed");
  }
}

def GPUSharedMemDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

def GPUSharedMemArr.dsiSerialWrite(f: Writer) {
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
