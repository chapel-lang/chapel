class SingleLocaleDistribution {
  def buildDomain(param rank: int, type dimensional_index_type) {
    return SingleLocaleArithmeticDomain(rank, dimensional_index_type);
  }
  def buildDomain(type ind_type) {
    return SingleLocaleAssociativeDomain(ind_type);
  }
}

var Block = SingleLocaleDistribution();

class SingleLocaleArithmeticDomain: BaseDomain {
  param rank : int;
  type dim_type;
  var ranges : rank*_aseq(dim_type);

  def _exclusive_upper {
    var x = SingleLocaleArithmeticDomain(rank, dim_type);
    for param i in 1..rank {
      if ranges(i)._stride != 1 then
        halt("syntax [domain-specification) requires a stride of one");
      x.ranges(i) = ranges(i)._low..ranges(i)._high-1;
    }
    return x;
  }

  def getHeadCursor() {
    var c : rank*dim_type;
    for param i in 1..rank do
      c(i) = ranges(i).getHeadCursor();
    return c;
  }

  def getNextCursor(c) {
    for param i in 1..rank {
      c(rank-i+1) = ranges(rank-i+1).getNextCursor(c(rank-i+1));
      if ranges(rank-i+1).isValidCursor?(c(rank-i+1)) then
        return c;
      else
        c(rank-i+1) = ranges(rank-i+1).getHeadCursor();
    }
    c(1) = ranges(1)._high+1;
    return c;
  }

  def getValue(c) {
    if rank == 1 then
      return c(1);
    else
      return c;
  }

  def isValidCursor?(c) {
    return c(1) != ranges(1)._high+1;
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
    if (rank == 1) then 
      return ranges(1)._low;
    else
      compilerError("low currently implemented only for rank-1 domains");
  }

  def high {
    if (rank == 1) then
      return ranges(1)._high;
    else
      commpilerError("high currently implemented only for rank-1 domains");
  }

  def _build_array(type eltType) {
    return SingleLocaleArithmeticArray(eltType, rank, dim_type, dom=this);
  }

  def _build_sparse_domain()
    return SingleLocaleSparseDomain(rank, dim_type, adomain=this);

  def _build_subdomain()
    return SingleLocaleArithmeticDomain(rank, dim_type);

  def translate(dim: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank, int);
    for i in 1..rank do
      x.ranges(i) = this(i)._translate(dim(i));
    return x;
  }

  def interior(dim: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank, int);
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
    var x = SingleLocaleArithmeticDomain(rank, int);
    for i in 1..rank do
      x.ranges(i) = this(i)._exterior(dim(i));
    return x;
  }

  def expand(dim: rank*int) {
    var x = SingleLocaleArithmeticDomain(rank, int);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(dim(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  

  def expand(dim: int) {
    var x = SingleLocaleArithmeticDomain(rank, int);
    for i in 1..rank do
      x.ranges(i) = ranges(i)._expand(dim);
    return x;
  }

  // this should eventually...
  //  ...be in the distribtion's implementation of the blocked domain
  // ...yield a subBlock of the domain per thread per locale
  iterator subBlocks {
    yield this;
  }
}


def by(dom : SingleLocaleArithmeticDomain, dim : dom.rank*int) {
  var x = SingleLocaleArithmeticDomain(dom.rank, dom.dim_type);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim(i);
  return x;
}

def by(dom : SingleLocaleArithmeticDomain, dim : int) {
  var x = SingleLocaleArithmeticDomain(dom.rank, dom.dim_type);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim;
  return x;
}


class SingleLocaleArithmeticArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;

  var dom : SingleLocaleArithmeticDomain(rank, dim_type);
  var off: rank*dim_type;
  var blk: rank*dim_type;
  var str: rank*int;
  var orig: rank*dim_type;
  var size : dim_type;
  var data : _ddata(eltType);
  var noinit: bool = false;

  def getHeadCursor()
    return dom.getHeadCursor();

  def getNextCursor(c)
    return dom.getNextCursor(c);

  def getValue(c)
    return this(dom.getValue(c));

  def isValidCursor?(c)
    return dom.isValidCursor?(c);

  def initialize() {
    if noinit == true then return;
    for param dim in 1..rank {
      off(dim) = dom(dim)._low;
      str(dim) = dom(dim)._stride;
      orig(dim) = 0:dim_type;
    }
    blk(rank) = 1:dim_type;
    for dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom(dim+1).length;
    size = blk(1) * dom(1).length;
    data = _ddata(eltType, size:int); // ahh!!! can't cast to int here
    data.init();
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
    return data(sum:int); // !!ahh
  }

  def this(ind: _aseq(dim_type) ...rank) var {
    var d = [(...ind)];
    return slice(d._value);
  }

  def view(d: SingleLocaleArithmeticDomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank do
      if d(i).length != dom(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = SingleLocaleArithmeticArray(eltType, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = d(i)._low;
      alias.blk(i) = blk(i) * (dom(i)._stride / str(i));
      alias.str(i) = d(i)._stride;
      alias.orig(i) = orig(i) + (off(i) - dom(i)._low) * blk(i);
    }
    return _array(alias.type, eltType, rank, alias, _domain(d.type, d.getValue(d.getHeadCursor()).type, dim_type, rank, d));
  }

  def checkSlice(d: SingleLocaleArithmeticDomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank {
      if !_in(dom(i), d(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", d);
      if d(i)._stride % dom(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
  }

  def slice(d: SingleLocaleArithmeticDomain) {
    checkSlice(d);
    var alias = SingleLocaleArithmeticArray(eltType, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = off(i);
      alias.blk(i) = blk(i);
      alias.str(i) = str(i);
      alias.orig(i) = orig(i);
    }
    return _array(alias.type, eltType, rank, alias, _domain(d.type, d.getValue(d.getHeadCursor()).type, dim_type, rank, d));
  }

  def =slice(d: SingleLocaleArithmeticDomain, val) {
    checkSlice(d);
    var alias = SingleLocaleArithmeticArray(eltType, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = off(i);
      alias.blk(i) = blk(i);
      alias.str(i) = str(i);
      alias.orig(i) = orig(i);
    }
    alias.assign(val);
  }

  // when condition compares d.type to dom.type
  // when parameter folding is done in function resolution
  // then rank of indefinite domain can be changed back to 1
  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var new = SingleLocaleArithmeticArray(eltType, rank, dim_type, d._value);
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

  def assign(y) {
    for (i,e) in (dom,y) do
      this(i) = e;
  }

  def assign(y: eltType) {
    for i in dom do
      this(i) = y;
  }
}

def SingleLocaleArithmeticDomain.writeThis(f: Writer) {
  f.write("[", this(1));
  for i in 2..rank do
    f.write(", ", this(i));
  f.write("]");
}

def SingleLocaleArithmeticArray.writeThis(f: Writer) {
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

def _intersect(a: _aseq, b: _aseq) {
  var g, x: int;
  (g, x) = _extended_euclid(a._stride, b._stride);
  var gg = g:a.eltType;
  var xx = x:a.eltType;
  var as = (a._stride):a.eltType;
  if abs(a._low - b._low) % gg != 0 then
    return 1..0:a.eltType;
  var low = max(a._low, b._low);
  var high = min(a._high, b._high);
  var stride = a._stride * b._stride / g;
  var alignment = a._low + (b._low - a._low) * xx * as / gg;
  if alignment == 0 then
    alignment = stride:a.eltType;
  low = low + low % alignment;
  return low..high by stride;
}

def _intersect(a: SingleLocaleArithmeticDomain, b: SingleLocaleArithmeticDomain) {
  var c = SingleLocaleArithmeticDomain(a.rank, a.dim_type);
  for param i in 1..a.rank do
    c.ranges(i) = _intersect(a(i), b(i));
  return c;
}

// Extended-Euclid (Knuth Volume 2 --- Section 4.5.2)
// given two non-negative integers u and v
// returns (gcd(u, v), x) where x is set such that u*x + v*y = gcd(u, v)
def _extended_euclid(u: int, v: int) {
  var u1 = 1;
  var u2 = 0;
  var u3 = u;
  var v1 = 0;
  var v2 = 1;
  var v3 = v;
  while v3 != 0 {
    var q = u3 / v3;
    var t1 = u1 - v1 * q;
    var t2 = u2 - v2 * q;
    var t3 = u3 - v3 * q;
    u1 = v1;
    u2 = v2;
    u3 = v3;
    v1 = t1;
    v2 = t2;
    v3 = t3;
  }
  return (u3, u1);
}
