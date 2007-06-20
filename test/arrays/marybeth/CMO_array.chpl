class CMODist {
  def buildDomain(param rank: int, type dimensional_index_type) {
    return CMODomain(rank=rank, dim_type=dimensional_index_type);
  }
}

class CMODomain: BaseDomain {
  param rank : int;
  type dim_type;
  var ranges : rank*range(dim_type,bounded,true);

  def getIndices() return ranges;

  def setIndices(x) {
    ranges = x;
  }

  def buildEmptyDomain()
    return CMODomain(rank=rank, dim_type=dim_type);

  def buildOpenIntervalUpper() {
    var x = CMODomain(rank=rank, dim_type=dim_type);
    for param i in 1..rank {
      if ranges(i)._stride != 1 then
        halt("syntax [domain-specification) requires a stride of one");
      x.ranges(i) = ranges(i)._low..ranges(i)._high-1;
    }
    return x;
  }

  def these_help(param dim: int) {
    if dim == rank - 1 {
      for i in ranges(dim) do
        for j in ranges(rank) do
          yield (i, j);
    } else {
      for i in ranges(dim) do
        for j in these_help(dim+1) do
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

  def numIndices {
    var sum = 1:dim_type;
    for param i in 1..rank do
      sum *= ranges(i).length;
    return sum;
    // WANT: return * reduce (this(1..rank).length);
  }

  def low {
    if rank == 1 {
      return ranges(1)._low;
    } else {
      var result: rank*dim_type;
      for param i in 1..rank do
        result(i) = ranges(i)._low;
      return result;
    }
  }

  def high {
    if rank == 1 {
      return ranges(1)._high;
    } else {
      var result: rank*dim_type;
      for param i in 1..rank do
        result(i) = ranges(i)._high;
      return result;
    }
  }

  def buildArray(type eltType) {
    return CMOArray(eltType, rank, dim_type, dom=this);
  }

  def translate(dim: rank*int) {
    var x = CMODomain(rank=rank, dim_type=int);
    for i in 1..rank do
      x.ranges(i) = this(i)._translate(dim(i));
    return x;
  }

  def interior(dim: rank*int) {
    var x = CMODomain(rank=rank, dim_type=int);
    for i in 1..rank do {
      if ((dim(i) > 0) && (this(i)._high+1-dim(i) < this(i)._low) ||
          (dim(i) < 0) && (this(i)._low-1-dim(i) > this(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = this(i)._interior(dim(i));
    }
    return x;
  }

  def exterior(dim: rank*int) {
    var x = CMODomain(rank=rank, dim_type=int);
    for i in 1..rank do
      x.ranges(i) = this(i)._exterior(dim(i));
    return x;
  }

  def expand(dim: rank*int) {
    var x = CMODomain(rank=rank, dim_type=int);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(dim(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(dim: int) {
    var x = CMODomain(rank=rank, dim_type=int);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(dim);
    return x;
  }

  // this should eventually...
  //  ...be in the distribtion's implementation of the blocked domain
  // ...yield a subBlock of the domain per thread per locale
  def subBlocks {
    yield this;
  }

  def strideBy(dim : rank*int) {
    var x = CMODomain(rank=rank, dim_type=dim_type);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by dim(i);
    return x;
  }

  def strideBy(dim : int) {
    var x = CMODomain(rank=rank, dim_type=dim_type);
    for i in 1..rank do
      x.ranges(i) = ranges(i) by dim;
    return x;
  }
}

class CMOArray:BaseArray {
  type eltType;
  param rank: int;
  type dim_type;

  var dom: CMODomain(rank=rank,dim_type=dim_type);
  var off: rank*dim_type;
  var blk: rank*dim_type;
  var str: rank*int;
  var orig: rank*dim_type;
  var size: dim_type;
  var D1: domain(1, dim_type);
  var data: [D1] eltType;
  var noinit: bool = false;

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom(dim)._low;
      str(dim) = dom(dim)._stride;
      orig(dim) = 0:dim_type;
    }
    blk(1) = 1:dim_type;
    for dim in 2..rank do
      blk(dim) = blk(dim-1) * dom(dim-1).length;
    size = blk(rank) * dom(rank).length;
    D1 = [0:dim_type..size:dim_type);
    data = 0:eltType;
  }

  def these() {
    for i in dom do
      yield this(i);
  }

  def this(ind: dim_type ...1) var where rank == 1
    return this(ind);

  def this(ind : rank*dim_type) var {
    if boundsChecking
      for param i in 1..rank do
        if !_in(dom(i), ind(i)) then
          halt("array index out of bounds: ", ind);
    var sum : dim_type;
    for param i in 1..rank do
      sum = sum + (ind(i) - off(i)) * blk(i) / str(i):dim_type - orig(i);
    return data(sum); 
  }

    def view(d: CMODomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank do
      if d(i).length != dom(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = CMOArray(eltType, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = d(i)._low;
      alias.blk(i) = blk(i) * (dom(i)._stride / str(i));
      alias.str(i) = d(i)._stride;
      alias.orig(i) = orig(i) + (off(i) - dom(i)._low) * blk(i);
    }
    return alias;
  }

  def checkSlice(d: CMODomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank {
      if !_in(dom(i), d(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", d);
      if d(i)._stride % dom(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def slice(d: CMODomain) {
    checkSlice(d);
    var alias = CMOArray(eltType, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = off(i);
      alias.blk(i) = blk(i);
      alias.str(i) = str(i);
      alias.orig(i) = orig(i);
    }
    return alias;
  }

  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var new = CMOArray(eltType, rank, dim_type, d._value);
      for i in _intersect(d._value, dom) do
        new(i) = this(i);
      dom = new.dom;
      off = new.off;
      blk = new.blk;
      str = new.str;
      orig = new.orig;
      size = new.size;
      data = new.data;
    } else {
      halt("illegal reallocation");
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
  var i : rank*dim_type;
  for dim in 1..rank do
    i(dim) = dom(dim)._low;
  label next while true {
    f.write(this(i));
    if i(rank) <= (dom(rank)._high - dom(rank)._stride:dim_type) {
      f.write(" ");
      i(rank) += dom(rank)._stride:dim_type;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom(dim)._high - dom(dim)._stride:dim_type) {
          i(dim) += dom(dim)._stride:dim_type;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

def _intersect(a: CMODomain, b: CMODomain) {
  var c = CMODomain(a.rank, a.dim_type);
  for param i in 1..a.rank do
    c.ranges(i) = _intersect(a(i), b(i));
  return c;
}

def main() {
  param n = 5;
  const D = [1..n,1..n];
  const D2: domain(2) distributed(CMODist()) = [1..n,1..n];
  var A: [D2] real;
  var B: [D] real;

  for ij in D2 {
    A(ij) = ij(1) + (ij(2) - 1)*n;
    B(ij) = ij(2) + (ij(1) - 1)*n;
  }

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
