pragma "data class"
pragma "constant cache array"
class _ccdata {
  type eltType;
  const array_size : int(64);

  def ~_ccdata() {
    __primitive("array_free", this);
  }

  pragma "inline" def init(size: integral) {
    __primitive("array_alloc", this, eltType, size); // Allocate Host Memory
    init_elts(this, size, eltType);
    array_size = size;
  }
  pragma "inline" def this(i: integral) var {
    return __primitive("array_get", this, i);
  }
}

config param debugGPUConstDist = false; // internal development flag (debugging)

class GPUConstDist: BaseDist {

   def GPUConstDist(param rank : int, 
	      type idxType = int(64),
              param constSize) {

    pragma "ccmem" var foo= 555; //CONSTSIZE = constSize;
    //writeln(CONSTSIZE);
    writeln(foo);
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUCCDist");
  }

  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new GPUConstArithmeticDom(rank, idxType, stridable, this);
    //return new GPUConstArithmeticDom(rank, idxType, stridable, constSize);
  }

  def dsiClone() return this;
}

class GPUConstArithmeticDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUConstDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);


  def linksDistribution() param return false;

  def GPUConstArithmeticDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
    writeln(dist);
  }

  def dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    ranges(1) = emptyRange;
  }
  
  def dsiGetIndices() return ranges;

  def dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).idxType != x(1).idxType then
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

  def dsiDims()
    return ranges;

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
      return new GPUConstArithmeticArr(eltType=eltType, rank=rank, idxType=idxType, 
          stridable=stridable, dom=this);
  }

  def dsiBuildArithmeticDom(param rank: int, type idxType, param stridable: bool,
                            ranges: rank*range(idxType,
                                               BoundedRangeType.bounded,
                                               stridable)) {
    var dom = new GPUConstArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      dom.ranges(i) = ranges(i);
    return dom;
  }
}

class GPUConstArithmeticArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;
  var dom : GPUConstArithmeticDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var noinit: bool = false;

  var data : _ccdata(eltType);
  var size : idxType;

  def isGPUExecution param return true;
  def canCopyFromHost param return true;
  def isConstMem param return true;

  def dsiGetBaseDom() return dom;

  def dsiDestroyData() { }

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

  def initialize() {
    if noinit == true then return;
    size = dom.dsiDim(1).length;
    data = new _ccdata(eltType);
    data.init(size);
  }

  pragma "inline"
  def dsiAccess(ind: idxType ...1) var where rank == 1 {
    return dsiAccess(ind);
  }

  pragma "inline"
  def dsiAccess(ind : rank*idxType) var {
    var sum = ind(1);
    return data(sum);
  }

  def dsiReindex(d: GPUConstArithmeticDom) {
    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable, dom=d, noinit=true);
                                     
    alias.data = data;
    alias.size = size: d.idxType;
    return alias;
  }

  def dsiSlice(d: GPUConstArithmeticDom) {
    writeln("slice...");
    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    alias.size = size;
    return alias;
  }

  def dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    writeln("rank change..");
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, dom=d);
    alias.data = data;
    alias.size = size;
    return alias;
  }

  def dsiReallocate(d: domain) {
    writeln("reallocate...");
    //writeln(d._value.type," ", dom.type);
    if (d._value.type == dom.type) {
      var copy = new GPUConstArithmeticArr(eltType=eltType, rank=rank,
                                          idxType=idxType,
                                          stridable=d._value.stridable,
                                          dom=d._value);
      for i in d((...dom.ranges)) do
        copy(i) = dsiAccess(i);
      size = copy.size;
      delete data;
      data = copy.data;
      delete copy;
    } else {
      halt("illegal reallocation");
    }
  }
}

def GPUConstArithmeticDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

def GPUConstArithmeticArr.dsiSerialWrite(f: Writer) {
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
