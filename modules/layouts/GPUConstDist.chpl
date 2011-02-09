pragma "data class"
pragma "constant cache array"
class _ccdata {
  type eltType;
  const array_size : int(64);

  proc ~_ccdata() {
    __primitive("array_free", this);
  }

  pragma "inline" proc init(size: integral) {
    __primitive("array_alloc", this, eltType, size); // Allocate Host Memory
    init_elts(this, size, eltType);
    array_size = size;
  }
  pragma "inline" proc this(i: integral) var {
    return __primitive("array_get", this, i);
  }
}

config param debugGPUConstDist = false; // internal development flag (debugging)

class GPUConstDist: BaseDist {

   proc GPUConstDist(param rank : int, 
	      type idxType = int(64),
              param constSize) {

    pragma "ccmem" var foo= 555; //CONSTSIZE = constSize;
    //writeln(CONSTSIZE);
    writeln(foo);
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUCCDist");
  }

  proc dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new GPUConstArithmeticDom(rank, idxType, stridable, this);
    //return new GPUConstArithmeticDom(rank, idxType, stridable, constSize);
  }

  proc dsiClone() return this;
}

class GPUConstArithmeticDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUConstDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);


  proc linksDistribution() param return false;

  proc GPUConstArithmeticDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
    writeln(dist);
  }

  proc dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    ranges(1) = emptyRange;
  }
  
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

  proc these(param tag: iterator) where tag == iterator.leader {
      compilerError("Not supported");
  }

  proc these(param tag: iterator, follower) where tag == iterator.follower {
      compilerError("Not supported");
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

  proc dsiNumIndices {
    var sum = 1:idxType;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  proc dsiLow {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  proc dsiHigh {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  proc dsiBuildArray(type eltType) {
      return new GPUConstArithmeticArr(eltType=eltType, rank=rank, idxType=idxType, 
          stridable=stridable, dom=this);
  }

  proc dsiBuildArithmeticDom(param rank: int, type idxType, param stridable: bool,
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

  proc isGPUExecution param return true;
  proc canCopyFromHost param return true;
  proc isConstMem param return true;

  proc dsiGetBaseDom() return dom;

  proc dsiDestroyData() { }

  iter these() var {
    for i in dom do
      yield dsiAccess(i);
  }

  proc these(param tag: iterator) where tag == iterator.leader {
    halt("Not supposed to happen");
  }


  proc these(param tag: iterator, follower) var where tag == iterator.follower {
    halt("Not supposed to happen");
  }

  proc initialize() {
    if noinit == true then return;
    size = dom.dsiDim(1).length;
    data = new _ccdata(eltType);
    data.init(size);
  }

  pragma "inline"
  proc dsiAccess(ind: idxType ...1) var where rank == 1 {
    return dsiAccess(ind);
  }

  pragma "inline"
  proc dsiAccess(ind : rank*idxType) var {
    var sum = ind(1);
    return data(sum);
  }

  proc dsiReindex(d: GPUConstArithmeticDom) {
    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable, dom=d, noinit=true);
                                     
    alias.data = data;
    alias.size = size: d.idxType;
    return alias;
  }

  proc dsiSlice(d: GPUConstArithmeticDom) {
    writeln("slice...");
    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, noinit=true);
    alias.data = data;
    alias.size = size;
    return alias;
  }

  proc dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    writeln("rank change..");
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    var alias = new GPUConstArithmeticArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, dom=d);
    alias.data = data;
    alias.size = size;
    return alias;
  }

  proc dsiReallocate(d: domain) {
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

proc GPUConstArithmeticDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

proc GPUConstArithmeticArr.dsiSerialWrite(f: Writer) {
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
