//
// Arrays and domains
//

pragma "array"
class _array {
  type _array_type;
  type elt_type;
  param rank : int;
  var _value : _array_type;
  var dom : _domain;

  def this(d : _domain) {
    var a = d._build_array(elt_type);
    for i in d do
      a(i) = this(i);
    return a;
  }

  def =this(d : _domain, v) {
    for i in d do
      this(i) = v;
  }

  // need this function to compete with string indexing function
  // (scalar promotion on array of strings or scalar promotion and
  // coercion on array of ints/floats)
  def this(i:int...?k)
    return _value((...i));

  def =this(i:int...?k, val: elt_type) {
    _value((...i)) = val;
  }

  def this(i) where !(i.type:_domain)
    return _value(i);

  def =this(i, val: elt_type) where !(i.type:_domain) {
    _value(i) = val;
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
  a._value = b._value;
  return a;
}

def =(a: _array, b: seq) {
  a._value = b;
  return a;
}

def =(a: _array, b: _aseq) {
  a._value = b;
  return a;
}

def =(a: _array, b: a.elt_type) {
  a._value = b;
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

class _domain {
  type _domain_type;
  param rank : int;
  var _value : _domain_type;

  def getHeadCursor()
    return _value.getHeadCursor();

  def getNextCursor(c)
    return _value.getNextCursor(c);

  def getValue(c)
    return _value.getValue(c);

  def isValidCursor?(c)
    return _value.isValidCursor?(c);

  def this(dim : int)
    return _value(dim);

  def _build_array(type elt_type) {
    var x = _value._build_array(elt_type);
    return _array(x.type, elt_type, rank, x, this);
  }

  def _build_index()
    return _value._build_index();

  def _build_sparse_domain() {
    var x = _value._build_sparse_domain();
    return _domain(x.type, rank, x);
  }

  def add(i) {
    _value.add(i);
  }

  def expand(i...?k) {
    var x = _value.expand((...i));
    return _domain(x.type, rank, x);
  }

  def exterior(i...?k) {
    var x = _value.exterior((...i));
    return _domain(x.type, rank, x);
  }

  def interior(i...?k) {
    var x = _value.interior((...i));
    return _domain(x.type, rank, x);
  }

  def translate(i...?k) {
    var x = _value.translate((...i));
    return _domain(x.type, rank, x);
  }
}

def =(a: _domain, b: _domain) {
  a._value = b._value;
  return a;
}

def fwrite(f : file, a: _domain) {
  fwrite(f, a._value);
}

def by(a: _domain, b) {
  var x = a._value by b;
  return _domain(x.type, a.rank, x);
}

////////////////////////////////////////////////////

def _build_domain(x)
  return x;

def _build_domain(ranges : _aseq ...?rank) {
  var x = _adomain(rank, ranges);
  return _domain(x.type, rank, x);
}

def _build_domain_type(param rank : int) {
  var x = _adomain(rank);
  return _domain(x.type, rank, x);
}

def _build_domain_type(type ind_type) {
  var x = _idomain(ind_type);
  return _domain(x.type, 1, x);
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
  return dom._build_index();
}

///////////////

def _aseq._translate(i : int) : _aseq {
  return _low+i.._high+i by _stride;
}

def _aseq._interior(i : int) : _aseq {
  var x : _aseq = _low.._high by _stride;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

def _aseq._exterior(i : int) : _aseq {
  var x : _aseq = _low.._high by _stride;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

def _aseq._expand(i : int) : _aseq {
  return _low-i.._high+i by _stride;
}


class _adomain {
  param rank : int;
  var ranges : rank*_aseq;

  def getHeadCursor() {
    var c : rank*int;
    for param i in 1..rank do
      c(i) = ranges(i).getHeadCursor();
    return c;
  }

  def getNextCursor(c) {
    for param i in 1..rank {
      c(i) = ranges(i).getNextCursor(c(i));
      if ranges(i).isValidCursor?(c(i)) then
        return c;
      else
        c(i) = ranges(i).getHeadCursor();
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
    for param i in 1..rank do
      if !ranges(i).isValidCursor?(c(i)) then
        return false;
    return true;
  }

  def this(dim : int)
    return ranges(dim);

  def _build_array(type elt_type)
    return _aarray(elt_type, rank, dom=this);

  def _build_sparse_domain()
    return _sdomain(rank, adomain=this);

  def _build_index() {
    var x : rank*int;
    return x;
  }

  def translate(dim : rank*int) {
    var x = _adomain(rank);
    for i in 1..rank do
      x.ranges(i) = this(i)._translate(dim(i));
    return x;
  }

  def translate(dim : int ...?numDims) {
    return translate(dim);
  }

  def interior(dim : rank*int) {
    var x = _adomain(rank);
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
    var x = _adomain(rank);
    for i in 1..rank do
      x.ranges(i) = this(i)._exterior(dim(i));
    return x;
  }
  
  def exterior(dim : int ...?numDims) {
    return exterior(dim);
  }

  def expand(dim : rank*int) {
    var x = _adomain(rank);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(dim(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  
  
  def expand(dim : int ...?numDims) {
    var x = _adomain(rank);
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
  var x = _adomain(dom.rank);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim(i);
  return x;
}

def by(dom : _adomain, dim : int) {
  var x = _adomain(dom.rank);
  for i in 1..dom.rank do
    x.ranges(i) = dom.ranges(i) by dim;
  return x;
}


record _aarray {
  type elt_type;
  param rank : int;

  var dom : _adomain(rank);
  var info : rank*2*int;
  var size : int;
  var data : _ddata(elt_type);
  var noinit: bool = false;

  def getHeadCursor()
    return 0;

  def getNextCursor(c)
    return c + 1;

  def getValue(c)
    return data(c);

  def isValidCursor?(c)
    return c < size;

  iterator this() : elt_type {
    forall x in dom
      yield x; 
  }

  def off(dim : int) var
    return info(dim)(1);

  def blk(dim : int) var
    return info(dim)(2);

  def initialize() {
    if dom == nil then return;
    if noinit == true then return;
    for param dim in 1..rank do
      off(dim) = dom(dim)._low;
    blk(rank) = 1;
    for dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom(dim+1).length;
    size = blk(1) * dom(1).length;
    data = _ddata(elt_type, size);
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

  def this(ind : rank*int) var {
    for param i in 1..rank do
      if !_in(dom(i), ind(i)) {
        halt("array index out of bounds: ", ind);
      }
    var sum : int;
    for param i in 1..rank do
      sum = sum + (ind(i) - off(i)) * blk(i);
    return data(sum);
  }

  def this(ind : int ...rank) var
    return this(ind);

  def view(d: _adomain) {
    if rank != d.rank then
      halt("array rank change not supported");
    for param i in 1..rank do
      if d(i).length != dom(i).length then
        halt("extent in dimension ", i, " does not match actual");
    var alias: _aarray(elt_type, rank, d, noinit=true);
    alias.data = data;
    alias.size = size;
    for param i in 1..rank {
      alias.off(i) = d(i)._low;
      alias.blk(i) = blk(i);
    }
    return _array(alias.type, elt_type, rank, alias, _domain(d.type, rank, d));
  }
}

def =(x : _aarray, y : _aarray) {
  var j : int;
  for e in y {
    x.data(j) = e;
    j = j + 1;
  }
  return x;
}

def =(x : _aarray, y : seq) {
  var j : int;
  for e in y {
    x.data(j) = e;
    j = j + 1;
  }
  return x;
}

def =(x: _aarray, y: _aseq) {
  var j : int;
  for e in y {
    x.data(j) = e;
    j = j + 1;
  }
  return x;
}

def =(x : _aarray, y) {
  for i in 0..x.size-1 do
    x.data(i) = y;
  return x;
}

def fwrite(f : file, x : _adomain) {
  fwrite(f, "[", x(1));
  for i in 2..x.rank do
    fwrite(f, ", ", x(i));
  fwrite(f, "]");
}

def fwrite(f : file, x : _aarray) {
  var i : x.rank*int;
  for dim in 1..x.rank do
    i(dim) = x.dom(dim)._low;
  label next while true {
    fwrite(f, x(i));
    if i(x.rank) < x.dom(x.rank)._high {
      fwrite(f, " ");
      i(x.rank) += x.dom(x.rank)._stride;
    } else {
      for dim in 1..x.rank-1 by -1 {
        if i(dim) < x.dom(dim)._high {
          i(dim) += x.dom(dim)._stride;
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
