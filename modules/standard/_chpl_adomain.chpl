//
// Arrays and domains
//

// this is the class that all array classes are derived from
pragma "abase"
class _abase {
  def reallocate(d: _domain) {
    halt("reallocation not implemented for this array");
  }
}

// this is a wrapper class for all arrays
pragma "array"
class _array {
  type _array_type;
  type elt_type;
  param rank : int;
  var _value : _array_type;
  var dom : _domain;

  def this(d : _domain)
    return _value.slice(d._value);

  def =this(d : _domain, val: elt_type) {
    for i in d do
      this(i) = val;
  }

  def this(i...?k)
    return _value((...i));

  def =this(i...?k, val: elt_type) {
    _value((...i)) = val;
  }

  def getHeadCursor()
    return _value.getHeadCursor();

  def getNextCursor(c)
    return _value.getNextCursor(c);

  def getValue(c)
    return _value.getValue(c);

  def isValidCursor?(c)
    return _value.isValidCursor?(c);

  iterator this() : elt_type {
    forall x in _value
      yield x; 
  }

  def view(d: _domain)
    return _value.view(d._value);
}

def =(a: _array, b: _array) {
  a._value.assign(b._value);
  return a;
}

def =(a: _array, b: _domain) {
  a._value.assign(b._value);
  return a;
}

def =(a: _array, b: seq) {
  a._value.assign(b);
  return a;
}

def =(a: _array, b: _aseq) {
  a._value.assign(b);
  return a;
}

def =(a: _array, b: a.elt_type) {
  a._value.assign(b);
  return a;
}

def _copy(a: _array) {
  var b : [a.dom] a.elt_type;
  b = a;
  return b;
}

def _init(a: _array) {
  var b : [a.dom] a.elt_type;
  return b;
}

def fwrite(f : file, a: _array) {
  fwrite(f, a._value);
}

pragma "domain"
class _domain {
  type _domain_type;
  type _index_type;
  param rank : int;
  var _value : _domain_type;
  var _arrs: seq(_abase);

  def getHeadCursor()
    return _value.getHeadCursor();

  def getNextCursor(c)
    return _value.getNextCursor(c);

  def getValue(c)
    return _value.getValue(c);

  def isValidCursor?(c)
    return _value.isValidCursor?(c);

  iterator this() : _index_type {
    forall x in _value
      yield x; 
  }

  def this(dim : int)
    return _value(dim);

  def _build_array(type elt_type) {
    var x = _value._build_array(elt_type);
    _arrs #= x;
    return _array(x.type, elt_type, rank, x, this);
  }

  def _build_sparse_domain() {
    var x = _value._build_sparse_domain();
    return _domain(x.type, _index_type, rank, x);
  }

  def add(i) {
    _value.add(i);
  }

  def remove(i) {
    _value.remove(i);
  }

  def member?(i) {
    return _value.member?(i);
  }

  def expand(i...?k) {
    var x = _value.expand((...i));
    return _domain(x.type, _index_type, rank, x);
  }

  def exterior(i...?k) {
    var x = _value.exterior((...i));
    return _domain(x.type, _index_type, rank, x);
  }

  def interior(i...?k) {
    var x = _value.interior((...i));
    return _domain(x.type, _index_type, rank, x);
  }

  def translate(i...?k) {
    var x = _value.translate((...i));
    return _domain(x.type, _index_type, rank, x);
  }
}

def =(a: _domain, b: _domain) {
  for e in a._arrs do
    e.reallocate(b);
  a._value = b._value;
  return a;
}

def fwrite(f : file, a: _domain) {
  fwrite(f, a._value);
}

def by(a: _domain, b) {
  var x = a._value by b;
  return _domain(x.type, a._index_type, a.rank, x);
}

////////////////////////////////////////////////////

def _build_domain(x)
  return x;

def _build_domain(ranges : _aseq ...?rank) {
  type t = ranges(1).elt_type;
  var x = _adomain(rank, t, ranges);
  return _domain(x.type, x.getValue(x.getHeadCursor()).type, rank, x);
}

def _build_domain_type(param rank : int, type dimensional_index_type = int) {
  var x = _adomain(rank, dimensional_index_type);
  return _domain(x.type, x.getValue(x.getHeadCursor()).type, rank, x);
}

def _build_domain_type(type ind_type) {
  var x = _idomain(ind_type);
  return _domain(x.type, ind_type, 1, x);
}

def _build_sparse_domain_type(dom)
  return dom._build_sparse_domain();

def _build_array_type(dom, type elt_type)
  return dom._build_array(elt_type);

def _build_index_type(param i: int) where i > 1 {
  var x : i*int;
  return x;
}

def _build_index_type(param i: int) where i == 1 {
  var x : int;
  return x;
}

def _build_index_type(dom) {
  var x: dom._index_type;
  return x;
}

///////////////

def _aseq._translate(i : int) {
  return _low+i.._high+i by _stride;
}

def _aseq._interior(i : int) {
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

def _aseq._exterior(i : int) {
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

def _aseq._expand(i : int) {
  return _low-i.._high+i by _stride;
}


class _adomain {
  param rank : int;
  type dim_type;
  var ranges : rank*_aseq(dim_type);

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

  def _build_array(type elt_type)
    return _aarray(elt_type, rank, dim_type, dom=this);

  def _build_sparse_domain()
    return _sdomain(rank, dim_type, adomain=this);

  def translate(dim : rank*int) {
    var x = _adomain(rank, int);
    for i in 1..rank do
      x.ranges(i) = this(i)._translate(dim(i));
    return x;
  }

  def translate(dim : int ...?numDims) {
    return translate(dim);
  }

  def interior(dim : rank*int) {
    var x = _adomain(rank, int);
    for i in 1..rank do {
      if ((dim(i) > 0) && (this(i)._high+1-dim(i) < this(i)._low) ||
          (dim(i) < 0) && (this(i)._low-1-dim(i) > this(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = this(i)._interior(dim(i));
    }
    return x;
  }

  def interior(dim : int ...?numDims) {
    return interior(dim);
  }

  def exterior(dim : rank*int) {
    var x = _adomain(rank, int);
    for i in 1..rank do
      x.ranges(i) = this(i)._exterior(dim(i));
    return x;
  }
  
  def exterior(dim : int ...?numDims) {
    return exterior(dim);
  }

  def expand(dim : rank*int) {
    var x = _adomain(rank, int);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(dim(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  
  
  def expand(dim : int ...?numDims) {
    var x = _adomain(rank, int);
    if (rank == numDims) {
      -- NOTE: would probably like to get rid of this assignment
      -- since domain assignment is/will eventually be nontrivial
      -- in cost;  yet returning expand(dim) currently causes
      -- problems as captured in trivial/shannon/condReturn3.chpl
      x = expand(dim);
    } else if (numDims == 1) {
      for i in 1..rank do
        x.ranges(i) = ranges(i)._expand(dim(1));
    } else {
      halt("***Error: Rank mismatch between domain and expand() arguments (", rank, " != ", numDims, ")***");
    }
    return x;
  }
}


def by(dom : _adomain, dim : dom.rank*int) {
  var x = _adomain(dom.rank, dom.dim_type);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim(i);
  return x;
}

def by(dom : _adomain, dim : int) {
  var x = _adomain(dom.rank, dom.dim_type);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim;
  return x;
}


class _aarray: _abase {
  type elt_type;
  param rank : int;
  type dim_type;

  var dom : _adomain(rank, dim_type);
  var off: rank*dim_type;
  var blk: rank*dim_type;
  var str: rank*int;
  var orig: rank*dim_type;
  var size : dim_type;
  var data : _ddata(elt_type);
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
    data = _ddata(elt_type, size:int); // ahh!!! can't cast to int here
    data.init();
  }

  def this(d : _adomain) {
    var a : [d] elt_type;
    for i in d do
      a(i) = this(i);
    return a;
  }

  def =this(d : _adomain, v) {
    for i in d do
      this(i) = v;
  }

  def this(ind : rank*dim_type) var {
    if boundsChecking
      for param i in 1..rank do
        if !_in(dom(i), ind(i)) then
          halt("array index out of bounds: ", ind);
    var sum : dim_type;
    for param i in 1..rank do
      sum = sum + (ind(i) - off(i)) * blk(i) / str(i):dim_type - orig(i);
//    write(" [", ind, " = ", sum, "] ");
    return data(sum:int); // !!ahh
  }

  def this(ind : dim_type ...rank) var
    return this(ind);

  def this(ind: _aseq(dim_type) ...rank) var {
    var d = [(...ind)];
    return slice(d._value);
  }

  def view(d: _adomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank do
      if d(i).length != dom(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias = _aarray(elt_type, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = d(i)._low;
      alias.blk(i) = blk(i) * (dom(i)._stride / str(i));
      alias.str(i) = d(i)._stride;
      alias.orig(i) = orig(i) + (off(i) - dom(i)._low) * blk(i);
    }
    return _array(alias.type, elt_type, rank, alias, _domain(d.type, d.getValue(d.getHeadCursor()).type, rank, d));
  }

  def slice(d: _adomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank {
      if !_in(dom(i), d(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", d);
      if d(i)._stride % dom(i)._stride != 0 then
        halt("stride of array slice is not multiple of stride in dimension ", i);
    }
    var alias = _aarray(elt_type, rank, dim_type, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = off(i);
      alias.blk(i) = blk(i);
      alias.str(i) = str(i);
      alias.orig(i) = orig(i);
    }
    return _array(alias.type, elt_type, rank, alias, _domain(d.type, d.getValue(d.getHeadCursor()).type, rank, d));
  }

  def reallocate(d: _domain) {
    if (d.rank == rank) {
      var new = _aarray(elt_type, rank, dim_type, d._value);
      for i in _intersect(d._value, dom) do
        new(i) = this(i);
      dom = new.dom;
      off = new.off;
      blk = new.blk;
      str = new.str;
      orig = new.orig;
      size = new.size;
      data = new.data;
    }
  }

  def assign(y : _aarray) {
    for i,e in dom,y do
      this(i) = e;
  }

  def assign(y : _adomain) {
    for i,e in dom,y do
      this(i) = e;
  }

  def assign(y : seq) {
    for i,e in dom,y do
      this(i) = e;
  }

  def assign(y: _aseq) {
    for i,e in dom,y do
      this(i) = e;
  }

  def assign(y: elt_type) {
    for i in dom do
      this(i) = y;
  }
}

def fwrite(f : file, x : _adomain) {
  fwrite(f, "[", x(1));
  for i in 2..x.rank do
    fwrite(f, ", ", x(i));
  fwrite(f, "]");
}

def fwrite(f : file, x : _aarray) {
  var i : x.rank*x.dim_type;
  for dim in 1..x.rank do
    i(dim) = x.dom(dim)._low;
  label next while true {
    fwrite(f, x(i));
    if i(x.rank) <= (x.dom(x.rank)._high - x.dom(x.rank)._stride:x.dim_type) {
      fwrite(f, " ");
      i(x.rank) += x.dom(x.rank)._stride:x.dim_type;
    } else {
      for dim in 1..x.rank-1 by -1 {
        if i(dim) <= (x.dom(dim)._high - x.dom(dim)._stride:x.dim_type) {
          i(dim) += x.dom(dim)._stride:x.dim_type;
          for dim2 in dim+1..x.rank {
            fwrite(f, "\n");
            i(dim2) = x.dom(dim2)._low;
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
  if abs(a._low - b._low) % g != 0 then
    return 1..0:a.elt_type;
  var low = max(a._low, b._low);
  var high = min(a._high, b._high);
  var stride = a._stride * b._stride / g;
  var alignment = (a._low + (b._low - a._low) * x * a._stride / g):a.elt_type;
  if alignment == 0 then
    alignment = stride:a.elt_type;
  low = low + low % alignment;
  return low..high by stride;
}

def _intersect(a: _adomain, b: _adomain) {
  var c = _adomain(a.rank, a.dim_type);
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


-- Arithmetic sequence

def _build_aseq(low: int, high: int) return _aseq(int, low, high);
def _build_aseq(low: uint, high: uint) return _aseq(uint, low, high);
def _build_aseq(low: int(64), high: int(64)) return _aseq(int(64), low, high);
def _build_aseq(low: uint(64), high: uint(64)) return _aseq(uint(64), low, high);

def _build_aseq(low, high) {
  compilerError("arithmetic sequence bounds are not of integral type");
}

record _aseq {
  type elt_type;
  var _low : elt_type;
  var _high : elt_type;
  var _stride : int = 1;

  iterator this() : elt_type {
    forall x in this
      yield x; 
  }

  def getHeadCursor()
    if _stride > 0 then
      return _low;
    else
      return _high;

  def getNextCursor(c)
    return c + _stride:elt_type;

  def getValue(c)
    return c;

  def isValidCursor?(c)
    return _low <= c && c <= _high;

  def length
    return
      (if _stride > 0
        then (_high - _low + _stride:elt_type) / _stride:elt_type
        else (_low - _high + _stride:elt_type) / _stride:elt_type);
}

def by(s : _aseq, i : int)
  return _aseq(s.elt_type, s._low, s._high, s._stride * i);

def _in(s : _aseq, i : s.elt_type)
  return i >= s._low && i <= s._high &&
    (i - s._low) % abs(s._stride):s.elt_type == 0;

// really slow --- REWRITE
def _in(s1: _aseq, s2: _aseq) {
  for i in s2 do
    if !_in(s1, i) then
      return false;
  return true;
}

def fwrite(f : file, s : _aseq) {
  fwrite(f, s._low, "..", s._high);
  if (s._stride > 1) then
    fwrite(f, " by ", s._stride);
}

pragma "inline" def string.substring(s: _aseq)
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);
