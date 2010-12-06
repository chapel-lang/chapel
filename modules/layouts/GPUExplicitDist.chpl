_extern def chpl_init_accelerator();
_extern def thread_barrier();
_extern def getThreadID_x() : int(32);
_extern def getThreadID_y() : int(32);
_extern def getThreadID_z() : int(32);
_extern def getBlockID_x() : int(32);
_extern def getBlockID_y() : int(32);
_extern def getBlockID_z() : int(32);
_extern def getBlockSize_x() : int(32);
_extern def getBlockSize_y() : int(32);
_extern def getBlockSize_z() : int(32);
_extern def getGridSize_x() : int(32);
_extern def getGridSize_y() : int(32);

// Initialize the accelerator device from the beginning
chpl_init_accelerator();

pragma "data class"
pragma "explicit gpu array"
class _gdata {
  type eltType;
  def ~_gdata() {
    __primitive("gpu_free", this);
  }
  pragma "inline" def init(size: integral) {
    __primitive("gpu_alloc", this, size, eltType);   // Allocate GPU Memory
  }
  pragma "inline" def this(i: integral) var {
    return __primitive("get_gpu_value", this, i);
  }
}

config param debugGPUExplicitDist = false; // internal development flag (debugging)

pragma "use gpu reduction"
class GPUExplicitDist: BaseDist {

  const tbsizeX, tbsizeY, tbsizeZ : int;
  const gridsizeX, gridsizeY : int;

  def GPUExplicitDist(param rank : int, 
	      type idxType = int(64),
              const gridSizeX = 1,
              const gridSizeY = 1,
              const tbSizeX = 1,
              const tbSizeY = 1,
              const tbSizeZ = 1) {
    if (CHPL_TARGET_COMPILER != "nvidia") then 
      compilerError("Support for non-nvidia compilers not yet defined for GPUExplicitDist");
    tbsizeX = tbSizeX;
    tbsizeY = tbSizeY;
    tbsizeZ = tbSizeZ;
    gridsizeX = gridSizeX;
    gridsizeY = gridSizeY;
  }

  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new GPUExplicitArithmeticDom(rank, idxType, stridable, this);
  }

  def dsiDestroyDistClass() { }
  def dsiClone() return this;

  def dsiCreateReindexDist(newSpace, oldSpace) return this;
  def dsiCreateRankChangeDist(param newRank, args) return this;
}

pragma "use gpu reduction"
class GPUExplicitArithmeticDom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: GPUExplicitDist;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  var low_val : rank*idxType;

  var sharedsize : int;

  const tbsizeX : int;
  const tbsizeY : int;
  const tbsizeZ : int;
  
  const tbGridX : int = 1;
  const tbGridY : int = 1;

  def linksDistribution() param return false;

  def GPUExplicitArithmeticDom(param rank, type idxType, param stridable, dist) {
    this.dist = dist;
  }

  def dsiClear() {
    var emptyRange: range(idxType, BoundedRangeType.bounded, stridable);
    for param i in 1..rank do
      ranges(i) = emptyRange;
  }
  
  def dsiGetIndices() return ranges;

  def setTBsizeX(x) { tbsizeX = x; }
  def setTBsizeY(x) { tbsizeY = x; }
  def setTBsizeZ(x) { tbsizeZ = x; }

  def getTBsizeX() return tbsizeX;
  def getTBsizeY() return tbsizeY;
  def getTBsizeZ() return tbsizeZ;

  def setTBGridX(x) { tbGridX = x; }
  def setTBGridY(x) { tbGridY = x; }

  def dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).idxType != x(1).idxType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;

    for param i in 1..rank do
      low_val(i) = ranges(i).low;
    tbsizeX = dist.tbsizeX;
    tbsizeY = dist.tbsizeY;
    tbsizeZ = dist.tbsizeZ;
    tbGridX = dist.gridsizeX;
    tbGridY = dist.gridsizeY;
    if rank == 1 then {
      if tbGridX == 1 then
        tbGridX = (ranges(1).length:int + tbsizeX - 1) / tbsizeX;
    }
    else if rank == 2 then {
      if tbGridX == 1 then
        tbGridX = (ranges(1).length:int + tbsizeX - 1) / tbsizeX;
      if tbGridY == 1 then
        tbGridY = (ranges(2).length:int + tbsizeY - 1) / tbsizeY;
    }
    else if rank == 3 then {
      if tbGridX == 1 then
        tbGridX = (((ranges(1).length:int + tbsizeX - 1) / tbsizeX):int * 
                   ((ranges(2).length:int + tbsizeY - 1) / tbsizeY):int *
                   ((ranges(3).length:int + tbsizeZ - 1) / tbsizeZ) : int);
    }
    else
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
    if rank == 1 {
      var gridX = tbGridX;
      var gridY = tbGridY;
      var sizeX = tbsizeX;
      var sizeY = tbsizeY;
      var sizeZ = tbsizeZ;
      var sharedSize = sharedsize;
      var size = ranges(1).length;
      on __primitive("chpl_on_gpu",gridX, gridY, sizeX, sizeY, sizeZ, sharedSize) do {
        var tid = getBlockID_x() * getBlockSize_x() + getThreadID_x();
        if tid < size then
          yield tid;
      }
    } 
    else if rank == 2 {
      var gridX = tbGridX;
      var gridY = tbGridY;
      var sizeX = tbsizeX;
      var sizeY = tbsizeY;
      var sizeZ = tbsizeZ;
      var sharedSize = sharedsize;
      var lenX = ranges(1).length;
      var lenY = ranges(2).length;
      on __primitive("chpl_on_gpu",gridX, gridY, sizeX, sizeY, sizeZ, sharedSize) do {
        var i = getBlockID_x() * getBlockSize_x() + getThreadID_x();
        var j = getBlockID_y() * getBlockSize_y() + getThreadID_y();
        if (i < lenX && j < lenY) then
          yield (i,j);
      }
    } 
    else if rank == 3 {
      var gridX = tbGridX;
      var gridY = tbGridY;
      var sizeX = tbsizeX;
      var sizeY = tbsizeY;
      var sizeZ = tbsizeZ;
      var sharedSize = sharedsize;
      var lenX = ranges(1).length;
      var lenY = ranges(2).length;
      var lenZ = ranges(3).length;
      var chunki = ((lenX  + sizeX - 1) / sizeX) : int;
      var chunkj = ((lenY  + sizeY - 1) / sizeY) : int;
      var chunkk = ((lenZ  + sizeZ - 1) / sizeZ) : int;
      on __primitive("chpl_on_gpu",gridX, gridY, sizeX, sizeY, sizeZ, sharedSize) do {
        var r1 = getBlockID_x() % chunki;
        var b1 = getBlockID_x() / chunki;
        var r2 = b1 % chunkj;
        var b2 = b1 / chunkj;
        var r3 = b2 % chunkk;

        var i = getThreadID_x() + getBlockSize_x() * r1;
        var j = getThreadID_y() + getBlockSize_y() * r2;
        var k = getThreadID_z() + getBlockSize_z() * r3;

        if (i < lenX && j < lenY && k < lenZ) then
          yield (i,j,k);
        
      }
    }
    else {
      compilerError("Can't support dimensions higher than 3 (yet)");
    }
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    if rank == 1 then
      yield follower + low_val(1);
    else
      yield follower + low_val;
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

  pragma "use gpu reduction"
  def dsiBuildArray(type eltType) {
      return new GPUExplicitArithmeticArr(eltType=eltType, rank=rank, idxType=idxType, 
          stridable=stridable, dom=this, off=low_val);
  }

  def dsiBuildArithmeticDom(param rank: int, type idxType, param stridable: bool,
                            ranges: rank*range(idxType,
                                               BoundedRangeType.bounded,
                                               stridable)) {
    var dom = new GPUExplicitArithmeticDom(rank, idxType, stridable, dist);
    for i in 1..rank do
      dom.ranges(i) = ranges(i);
    return dom;
  }
}

pragma "use gpu reduction"
class GPUExplicitArithmeticArr: BaseArr {
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var dom : GPUExplicitArithmeticDom(rank=rank, idxType=idxType,
                                         stridable=stridable);
  var noinit: bool = false;
  var contiguousMem : bool = true;

  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var data : _gdata(eltType);
  var size : idxType;

  def isGPUExecution param return true;

  def canCopyFromHost param return true;
  def isGPUExplicit param return true;

  def dsiGetBaseDom() return dom;

  def dsiDestroyData() { }

  def these() var {
    for i in dom do
      yield dsiAccess(i);
  }

  def these(param tag: iterator) where tag == iterator.leader {
      var tbgridX = dom.tbGridX;
      var tbgridY = dom.tbGridY;
      var tbsizeX = dom.tbsizeX;
      var tbsizeY = dom.tbsizeY;
      var tbsizeZ = dom.tbsizeZ;
      var highval = size;
      var sharedSize = 0;
      on __primitive("chpl_on_gpu",tbgridX,tbgridY,tbsizeX, tbsizeY, tbsizeZ, sharedSize) do {
        var tid = getBlockID_x() * getBlockSize_x() + getThreadID_x();
        if tid < highval then
          yield tid;
      }
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    if rank == 1 then
      yield dsiAccess(follower + off(1));
    else
      yield dsiAccess(follower + off);
  }

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for param i in 1..rank do
      factoredOffs = factoredOffs - blk(i) * off(i);
  }

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank do
      str(dim) = dom.dsiDim(dim)._stride;
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
  /*
    var sum = factoredOffs;
    for param i in 1..rank do
      sum += ind(i) * blk(i);
    return data(sum);
  */
    var sum = origin;
    if stridable {
      for param i in 1..rank do
        sum += (ind(i) - off(i)) * blk(i) / str(i):idxType;
    } else {
      for param i in 1..rank do
        sum += ind(i) * blk(i);
      sum += factoredOffs;
    }
    return data(sum);
  }

  def dsiReindex(d: GPUExplicitArithmeticDom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dsiDim(i).length != dom.dsiDim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new GPUExplicitArithmeticArr(eltType=eltType, rank=d.rank,
                                     idxType=d.idxType,
                                     stridable=d.stridable, dom=d, noinit=true, 
                                     contiguousMem=true);
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

  def dsiSlice(d: GPUExplicitArithmeticDom) {
    var alias = new GPUExplicitArithmeticArr(eltType=eltType, rank=rank,
                                         idxType=idxType,
                                         stridable=d.stridable,
                                         dom=d, contiguousMem=true, noinit=true);

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
    writeln("rank change..");
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var alias = new GPUExplicitArithmeticArr(eltType=eltType, rank=newRank,
                                         idxType=idxType,
                                         stridable=newStridable, contiguousMem=true, dom=d);
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
    writeln("reallocate...");
    if (d._value.type == dom.type) {
      var copy = new GPUExplicitArithmeticArr(eltType=eltType, rank=rank,
                                          idxType=idxType,
                                          stridable=d._value.stridable,
                                          dom=d._value);
      for i in d((...dom.ranges)) do
        copy(i) = dsiAccess(i);
      off = copy.off;
      blk = copy.blk;
      str = copy.str;
      origin = copy.origin;
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

def GPUExplicitArithmeticDom.dsiSerialWrite(f: Writer) {
  f.write("[", dsiDim(1));
  for i in 2..rank do
    f.write(", ", dsiDim(i));
  f.write("]");
}

def GPUExplicitArithmeticArr.dsiSerialWrite(f: Writer) {
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
