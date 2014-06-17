extern proc chpl_init_accelerator();
extern proc getThreadID_x() : int(32);
extern proc getThreadID_y() : int(32);
extern proc getThreadID_z() : int(32);
extern proc getBlockID_x() : int(32);
extern proc getBlockID_y() : int(32);
extern proc getBlockID_z() : int(32);
extern proc getBlockSize_x() : int(32);
extern proc getBlockSize_y() : int(32);
extern proc getBlockSize_z() : int(32);

// Initialize the accelerator device from the beginning
chpl_init_accelerator();

pragma "data class"
class _gdata {
  type eltType;
  proc ~_gdata() {
    __primitive("gpu_free", this);
  }
  inline proc init(size: integral) {
    __primitive("gpu_alloc", this, size, eltType);
  }
  inline proc this(i: integral) var {
    return __primitive("get_gpu_value", this, i);
  }
}

config param debugGPUDist = false; // internal development flag (debugging)

class GPUDist: BaseDist {

  const threadPerBlock : int;

  proc GPUDist(param rank : int, 
          type idxType = int(64), 
          threadsPerBlock=128) {
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUDist");
    threadPerBlock = threadsPerBlock;
  }

  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
    return new GPURectangularDom(rank, idxType, stridable, this);
  }
}

//
// Note that the replicated copies are set up in ChapelLocale on the
// other locales.  This just sets it up on this locale.
//

class GPURectangularDom: BaseRectangularDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  var low_val : int(64);

  const threadPerBlock : int;
  const numBlocks : int;

  proc GPURectangularDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
  }

  proc dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    for param i in 1..rank do
      ranges(i) = emptyRange;
  }
  
  proc dsiMyDist() return dist;

  proc dsiGetIndices() return ranges;

  proc dsiSetIndices(x) {
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

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    if rank == 1 {
      yield followThis + low_val;
    } 
    else {
      compilerError("No support for multi-dimensional arrays (yet)");
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

  proc dsiBuildArray(type eltType) {
    return new GPURectangularArr(eltType=eltType, rank=rank, idxType=idxType, 
                    stridable=stridable, dom=this, 
                low_val=ranges(1).low);
  }

  proc dsiRankChange(param rank: int, param stridable: bool, args) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    var d = new GPURectangularDom(rank, idxType, stridable, dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dsiDim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  proc dsiBuildRectangularDom(param rank: int, type idxType,
                            param stridable: bool,
                            ranges: rank*range(idxType,
                            BoundedRangeType.bounded,
                            stridable)) {
  if idxType != dist.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("Block domain rank does not match distribution's");

  var dom = new GPURectangularDom(rank=rank, idxType=idxType,
                                 dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;
}


  proc translate(off: rank*idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).translate(off(i));
    return x;
  }

  proc chpl__unTranslate(off: rank*idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).chpl__unTranslate(off(i));
    return x;
  }

  proc interior(off: rank*idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dsiDim(i).high+1-off(i) < dsiDim(i).low) ||
          (off(i) < 0) && (dsiDim(i).low-1-off(i) > dsiDim(i).high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dsiDim(i).interior(off(i));
    }
    return x;
  }

  proc exterior(off: rank*idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = dsiDim(i).exterior(off(i));
    return x;
  }

  proc expand(off: rank*idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i).expand(off(i));
      if (x.ranges(i).low > x.ranges(i).high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  proc expand(off: idxType) {
    var x = new GPURectangularDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i).expand(off);
    return x;
  }
}

class GPURectangularArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var dom : GPURectangularDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var factoredOffs: idxType;
  var data : _gdata(eltType);
  var size : idxType;
  var low_val : int(64);

  proc canCopyFromHost param return true;

  proc dsiGetBaseDom() return dom;

  proc destroyData() {
    delete data;
  }

  iter these() var {
    for i in dom do
      yield dsiAccess(i);
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
      var nBlocks = dom.numBlocks;
      var threadsPerBlock = dom.threadPerBlock;
      var highval = size;
      on __primitive("chpl_on_gpu",nBlocks,threadsPerBlock) do {
    var tid = getBlockID_x() * getBlockSize_x() + getThreadID_x();
    if tid < highval then
      yield tid;
      }
  }

  iter these(param tag: iterKind, followThis) var where tag == iterKind.follower {
      yield dsiAccess(followThis + low_val);
  }

  proc computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs = factoredOffs + blk(i) * off(i);
    }
  }

  proc initialize() {
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim).low;
      str(dim) = dom.dsiDim(dim).stride;
    }
    blk(rank) = 1:idxType;
    for param dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.dsiDim(dim+1).length;
    computeFactoredOffs();
    size = blk(1) * dom.dsiDim(1).length;
    data = new _gdata(eltType);
    data.init(size);
  }

  inline proc dsiAccess(ind: idxType ...1) var where rank == 1 {
    return dsiAccess(ind);
  }

  inline proc dsiAccess(ind : rank*idxType) var {
    var sum = ind(1) - factoredOffs; // Hardcoded to support only 1D arrays
    return data(sum);
  }

  proc dsiReindex(d: GPURectangularDom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dsiDim(i).length != dom.dsiDim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new GPURectangularArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable, dom=d);
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i).low;
      alias.blk(i) = (blk(i) * dom.dsiDim(i).stride / str(i)) : d.idxType;
      alias.str(i) = d.dsiDim(i).stride;
    }
    alias.computeFactoredOffs();
    return alias;
  }

  proc dsiSlice(d: GPURectangularDom) {
    var alias = new GPURectangularArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d);
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    for param i in 1..rank {
      alias.off(i) = d.dsiDim(i).low;
    }
    alias.computeFactoredOffs();
    return alias;
  }

  proc checkRankChange(args) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    for param i in 1..args.size do
      if isRange(args(i)) then
        if !dom.dsiDim(i).boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  proc dsiRankChange(d, param newRank: int, param newStridable: bool, args) {
    proc isRange(r: range(?e,?b,?s)) param return 1;
    proc isRange(r) param return 0;

    var alias = new GPURectangularArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, dom=d);
    alias.data = data;
    alias.size = size;
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        alias.off(i) = d.dsiDim(i).low;
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      } else {
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }

  proc dsiReallocate(d: domain) {
    if (d._value.type == dom.type) {
      var copy = new GPURectangularArr(eltType=eltType, rank=rank,
                                          idxType=idxType,
                                          stridable=d._value.stridable,
                                          dom=d._value);
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
}

proc GPURectangularDom.dsiSerialReadWrite(f /*: Reader or Writer*/) {
  f <~> new ioLiteral("{") <~> dsiDim(1);
  for i in 2..rank do
    f <~> new ioLiteral(", ") <~> dsiDim(i);
  f <~> new ioLiteral("}");
}

proc GPURectangularDom.dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
proc GPURectangularDom.dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }

proc GPURectangularArr.dsiSerialReadWrite(f /*: Reader or Writer*/) {
  if dom.numIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f <~> dsiAccess(i);
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:idxType) {
      f <~> new ioLiteral(" ");
      i(rank) += dom.dsiDim(rank).stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:idxType) {
          i(dim) += dom.dsiDim(dim).stride:idxType;
          for dim2 in dim+1..rank {
            f <~> new ioNewline();
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

proc GPURectangularArr.dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
proc GPURectangularArr.dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }

