class CMODist : BaseDist {
  def dsiNewArithmeticDom(param rank: int, type dimensional_index_type, param stridable: bool, param alias: bool=false) {
    return new CMODom(rank=rank, idxType=dimensional_index_type, stridable=stridable, alias=alias, dist=this);
  }
  def dsiCreateRankChangeDist(param newRank, args) {
    return this;
  }
  def dsiClone() return this;
}

class CMODom: BaseArithmeticDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var dist: CMODist;
  param alias: bool = false;
  var ranges : rank*range(idxType,BoundedRangeType.bounded,stridable);

  def dsiGetIndices() return ranges;

  def dsiSetIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).idxType != x(1).idxType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  def these_help(param dim: int) {
    if dim == rank - 1 {
      for j in ranges(rank) do
        for i in ranges(dim) do
          yield (i, j);
    } else {
      for j in these_help(dim+1) do
        for i in ranges(dim) do
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

  def dsiAccess(dim : int)
    return ranges(dim);

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

  def dsiDim(d : int)
    return ranges(d);

  def bbox(d: int) {
    const r: range(idxType,BoundedRangeType.bounded,false) = ranges(d);
    return r;
  }

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
    return new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  }
 
  def buildSubdomain() 
    return new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);

  def dsiRankChange(param rank: int, param stridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var d = new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dsiDim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  def translate(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable = stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._translate(off(i));
    return x;
  }

  def interior(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do {
      if ((off(i) > 0) && (dim(i)._high+1-dim(i) < dim(i)._low) ||
          (off(i) < 0) && (dim(i)._low-1-dim(i) > dim(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = dim(i)._interior(off(i));
    }
    return x;
  }

  def exterior(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = dim(i)._exterior(off(i));
    return x;
  }

  def expand(off: rank*int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(off(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(off: int) {
    var x = new CMODom(rank=rank, idxType=int, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(off);
    return x;
  }

  def dsiStrideBy(str : rank*int) {
    var x = new CMODom(rank=rank, idxType=idxType, stridable=stridable, dist=dist);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }
}

class CMOArr:BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  param reindexed: bool = false;

  var dom: CMODom(rank=rank,idxType=idxType, stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var size: idxType;
  var D1: domain(1, idxType);
  var data: [D1] eltType;
  var noinit: bool = false;

  def dsiGetBaseDom() return dom;
  
  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs += blk(i) * off(i);
    }
  }

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dsiDim(dim)._low;
      str(dim) = dom.dsiDim(dim)._stride;
    }
    blk(1) = 1:idxType;
    for dim in 2..rank do
      blk(dim) = blk(dim-1) * dom.dsiDim(dim-1).length;
    computeFactoredOffs();
    size = blk(rank) * dom.dsiDim(rank).length;
    D1 = [0:idxType..#size:idxType];
    data = 0:eltType;
  }

  def these() var {
    for i in dom do {
      yield dsiAccess(i);
    }
  }

  def dsiAccess(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.dsiMember(ind) then
        halt("array index out of bounds: ", ind);
    var sum = origin;  
    if stridable {
      for param i in 1..rank do
        sum = sum + (ind(i) - off(i)) * blk(i) / str(i):idxType;
    } else {
      if reindexed {
        for param i in 1..rank do
          sum += ind(i) * blk(i);
      } else {
        for param i in 1..rank do
          sum += ind(i) * blk(i);
      }
      sum -= factoredOffs;
    }
    return data(sum); 
  }

  def dsiReindex(d: CMODom) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dim(i).length != dom.dim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = new CMOArr(eltType=eltType, rank=d.rank, idxType=d.idxType, stridable=d.stridable, reindexed=true, dom=d, noinit=true);
    //    was:  (eltType, rank, idxType, d.stridable, true, d, noinit=true);
    alias.D1 = [0:idxType..#size:idxType];
    alias.data = data;
    alias.size = size: d.idxType;
    for param i in 1..rank {
      alias.off(i) = d.dim(i)._low;
      alias.blk(i) = (blk(i) * dom.dim(i)._stride / str(i)) : d.idxType;
      alias.str(i) = d.dim(i)._stride;
    }
    alias.origin = origin:d.idxType;
    alias.computeFactoredOffs();
    return alias;
  }

  def dsiCheckSlice(d) {
    for param i in 1..rank {
      if d(i).boundedType == BoundedRangeType.bounded then
        if !dom.dim(i).member(d(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", d(i));
    }
  }

  def dsiSlice(d: CMODom) {
    var alias = new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, reindexed=reindexed, dom=d, noinit=true);
    alias.D1 = [0:idxType..#size:idxType];
    alias.data = data;
    alias.size = size;
    alias.blk = blk;
    alias.str = str;
    alias.origin = origin;
    for param i in 1..rank {
      alias.off(i) = d.dim(i)._low;
      alias.origin += blk(i) *(d.dim(i)._low - off(i))/str(i);
    }
    alias.computeFactoredOffs();
    return alias;
  }

  def checkRankChange(args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    for param i in 1..args.size do
      if isRange(args(i)) then
        if args(i).boundedType == BoundedRangeType.bounded then
          if !dom.dsiDim(i).member(args(i)) then
            halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  def dsiRankChange(d, param newRank: int, param newStridable: bool, irs) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;
    var alias = new CMOArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=newStridable, reindexed=true, dom=d, noinit=true);
    alias.D1 = [0:idxType..#size:idxType];
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..irs.size {
      if isRange(irs(j)) {
        alias.off(i) = d.dsiDim(i)._low;
        alias.origin += blk(j) * (d.dsiDim(i)._low - off(j)) / str(j);
        alias.blk(i) = blk(j);
        alias.str(i) = str(j);
        i += 1;
      } else {
        alias.origin += blk(j) * (irs(j) - off(j)) / str(j);
      }
    }
    alias.computeFactoredOffs();
    return alias;
  }


  def dsiReallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var copy = new CMOArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d._value.stridable, reindexed=reindexed, dom=d._value);
      for i in _intersect(d._value, dom) do
        copy(i) = this(i);
      off = copy.off;
      blk = copy.blk;
      str = copy.str;
      origin = copy.origin;
      factoredOffs = copy.factoredOffs;
      size = copy.size;
      data = copy.data;
    } else {
      halt("illegal reallocation");
    }
  }

  def tupleInit(b: _tuple) {
    def _tupleInitHelp(j, param rank: int, b: _tuple) {
      if rank == 1 {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dim(this.rank-rank+1).low + i - 1;
          this(j) = b(i);
        }
      } else {
        for param i in 1..b.size {
          j(this.rank-rank+1) = dom.dim(this.rank-rank+1).low + i - 1;
          _tupleInitHelp(j, rank-1, b(i));
        }
      }
    }

    if rank == 1 {
      for param i in 1..b.size do
        this(this.dom.dim(1).low + i - 1) = b(i);
    } else {
      var j: rank*int;
      _tupleInitHelp(this, j, rank, b);
    }
  }
}

def CMODom.dsiSerialWrite(f: Writer) {
  f.write("[", this(1));
  for i in 2..rank do
    f.write(", ", this(i));
  f.write("]");
}

def CMOArr.dsiSerialWrite(f: Writer) {
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

def _intersect(a: CMODom, b: CMODom) {
  var c = new CMODom(a.rank, a.idxType, stridable=a.stridable, dist=b.dist);
  for param i in 1..a.rank do
    c.ranges(i) = a.dim(i)(b.dim(i));
  return c;
}

def main() {
  param n = 5;
  const D = [1..n,1..n];
  const D2: domain(2) dmapped new dmap(new CMODist()) = [1..n,1..n];
  var A: [D2] real = [ij in D2] ij(1) + (ij(2) - 1)*n;
  var B: [D] real = [ij in D] ij(2) + (ij(1) - 1)*n;

  writeln('A by columns');
  for j in 1..n {
    writeln(A(1..n,j));
  }
  writeln();
  writeln('B by rows');
  for i in 1..n {
    writeln(B(i,1..n));
  }

  writeln();
  write('A = ');
  writeln(A);
  writeln();
  write('B = ');
  writeln(B);
}
