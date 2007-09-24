class CMODist {
  def buildDomain(param rank: int, type dimensional_index_type, param stridable: bool) {
    return CMODomain(rank=rank, idxType=dimensional_index_type, stridable=stridable);
  }
}

class CMODomain: BaseDomain {
  param rank : int;
  type idxType;
  param stridable: bool;
  var ranges : rank*range(idxType,bounded,stridable);

  def getIndices() return ranges;

  def setIndices(x) {
    if ranges.size != x.size then
      compilerError("rank mismatch in domain assignment");
    if ranges(1).eltType != x(1).eltType then
      compilerError("index type mismatch in domain assignment");
    ranges = x;
  }

  def buildEmptyDomain()
    return CMODomain(rank=rank, idxType=idxType, stridable=stridable);

  def buildOpenIntervalUpper() {
    var x = CMODomain(rank=rank, idxType=idxType, stridable=stridable);
    for param i in 1..rank {
      if ranges(i)._stride != 1 then
        halt("syntax [domain-specification) requires a stride of one");
      x.ranges(i) = ranges(i)._low..ranges(i)._high-1;
    }
    return x;
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

  def this(dim : int)
    return ranges(dim);

  def member(ind: idxType) where rank == 1 {
    if !_in(ranges(1), ind) then
      return false;
    return true;
  }

  def member(ind: rank*idxType) {
    for param i in 1..rank do
      if !_in(ranges(i), ind(i)) then
        return false;
    return true;
  }

  def dim(d : int)
    return ranges(d);

  def bbox(d: int) {
    const r: range(idxType,bounded,false) = ranges(d);
    return r;
  }

  def numIndices {
    var sum = 1:idxType;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  def low {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  def high {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*idxType;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  def buildArray(type eltType) {
    return CMOArray(eltType, rank, idxType, stridable, dom=this);
  }
 
  def buildSubdomain() 
    return CMODomain(rank=rank, idxType=idxType, stridable=stridable);

  def rankChange(param rank: int, param stridable: bool, args) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;

    var d = CMODomain(rank=rank, idxType=idxType, stridable=stridable);
    var i = 1;
    for param j in 1..args.size {
      if isRange(args(j)) {
        d.ranges(i) = dim(j)(args(j));
        i += 1;
      }
    }
    return d;
  }

  def translate(off: rank*int) {
    var x = CMODomain(rank=rank, idxType=int, stridable = stridable);
    for i in 1..rank do
      x.ranges(i) = dim(i)._translate(off(i));
    return x;
  }

  def interior(off: rank*int) {
    var x = CMODomain(rank=rank, idxType=int, stridable=stridable);
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
    var x = CMODomain(rank=rank, idxType=int, stridable=stridable);
    for i in 1..rank do
      x.ranges(i) = dim(i)._exterior(off(i));
    return x;
  }

  def expand(off: rank*int) {
    var x = CMODomain(rank=rank, idxType=int, stridable=stridable);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(off(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(off: int) {
    var x = CMODomain(rank=rank, idxType=int, stridable=stridable);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(off);
    return x;
  }

  // this should eventually...
  //  ...be in the distribtion's implementation of the blocked domain
  // ...yield a subBlock of the domain per thread per locale
  def subBlocks {
    yield this;
  }

  def strideBy(str : rank*int) {
    var x = CMODomain(rank=rank, idxType=idxType, stridable=stridable);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str(i);
    return x;
  }

  def strideBy(str : int) {
    var x = CMODomain(rank=rank, idxType=idxType, stridable=stridable);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by str;
    return x;
  }
}

class CMOArray:BaseArray {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  param reindexed: bool = false;

  var dom: CMODomain(rank=rank,idxType=idxType, stridable=stridable);
  var off: rank*idxType;
  var blk: rank*idxType;
  var str: rank*int;
  var origin: idxType;
  var factoredOffs: idxType;
  var size: idxType;
  var D1: domain(1, idxType);
  var data: [D1] eltType;
  var noinit: bool = false;

  def computeFactoredOffs() {
    factoredOffs = 0:idxType;
    for i in 1..rank do {
      factoredOffs += blk(i) * off(i);
    }
  }

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom.dim(dim)._low;
      str(dim) = dom.dim(dim)._stride;
    }
    blk(1) = 1:idxType;
    for dim in 2..rank do
      blk(dim) = blk(dim-1) * dom.dim(dim-1).length;
    computeFactoredOffs();
    size = blk(rank) * dom.dim(rank).length;
    D1 = [0:idxType..size:idxType);
    data = 0:eltType;
  }

  def these() {
    for i in dom do {
      yield this(i);
    }
  }

  def this(ind: idxType ...1) var where rank == 1
    return this(ind);

  def this(ind : rank*idxType) var {
    if boundsChecking then
      if !dom.member(ind) then
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

  def reindex(d: CMODomain) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dim(i).length != dom.dim(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = CMOArray(eltType, d.rank, d.idxType, d.stridable, true, d, noinit=true);
    //    was:  (eltType, rank, idxType, d.stridable, true, d, noinit=true);
    alias.D1 = [0:idxType..size:idxType);
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

  def checkSlice(d: CMODomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank {
      if !_in(dom.dim(i), d.dim(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", d);
      if d.dim(i)._stride % dom.dim(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def checkSlice(d) {
    if rank != d.size then
      halt("array rank change not supported");
    for param i in 1..rank {
      if d(i).boundedType == bounded then
        if !_in(dom.dim(i), d(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", d(i));
      if d(i)._stride % dom.dim(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def slice(d: CMODomain) {
    var alias = CMOArray(eltType, rank, idxType, d.stridable, reindexed, d, noinit=true);
    alias.D1 = [0:idxType..size:idxType);
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

  def rankChange(param newRank: int, param newStridable: bool, irs) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;
    var d = dom.rankChange(newRank, newStridable, irs);
    var alias = CMOArray(eltType, newRank, idxType, newStridable, true, d, noinit=true);
    alias.D1 = [0:idxType..size:idxType);
    alias.data = data;
    alias.size = size;
    var i = 1;
    alias.origin = origin;
    for param j in 1..irs.size {
      if isRange(irs(j)) {
        alias.off(i) = d.dim(i)._low;
        alias.origin += blk(j) * (d.dim(i)._low - off(j)) / str(j);
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


  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var new = CMOArray(eltType, rank, idxType, d._value.stridable, reindexed, d._value);
      for i in _intersect(d._value, dom) do
        new(i) = this(i);
      off = new.off;
      blk = new.blk;
      str = new.str;
      origin = new.origin;
      factoredOffs = new.factoredOffs;
      size = new.size;
      data = new.data;
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

def CMODomain.writeThis(f: Writer) {
  f.write("[", this(1));
  for i in 2..rank do
    f.write(", ", this(i));
  f.write("]");
}

def CMOArray.writeThis(f: Writer) {
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dim(dim)._low;
  label next while true {
    f.write(this(i));
    if i(rank) <= (dom.dim(rank)._high - dom.dim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dim(dim)._high - dom.dim(dim)._stride:idxType) {
          i(dim) += dom.dim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

def _intersect(a: CMODomain, b: CMODomain) {
  var c = CMODomain(a.rank, a.idxType, stridable=a.stridable);
  for param i in 1..a.rank do
    c.ranges(i) = a.dim(i)(b.dim(i));
  return c;
}

def main() {
  param n = 5;
  const D = [1..n,1..n];
  const D2: domain(2) distributed(CMODist()) = [1..n,1..n];
  var A: [ij in D2] real = ij(1) + (ij(2) - 1)*n;
  var B: [ij in D] real = ij(2) + (ij(1) - 1)*n;

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
