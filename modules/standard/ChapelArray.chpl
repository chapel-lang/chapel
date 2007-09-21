def _build_domain_type(dist, param rank : int, type idxType = int,
                       param stridable = false)
  return _domain(rank, dist.buildDomain(rank, idxType, stridable));

def _build_domain_type(dist, type ind) where !__primitive("isEnumType", ind)
  return _domain(1, dist.buildDomain(ind));

def _build_domain_type(dist, type ind) where __primitive("isEnumType", ind)
  return _domain(1, dist.buildEnumDomain(ind));

def _build_subdomain_type(dom)
  return dom.buildSubdomain();

def _build_sparse_subdomain_type(dist, parentDom) {
  var x = dist.buildSparseDomain(parentDom.rank, parentDom._value.idxType, parentDom._value);
  return _domain(parentDom.rank, x);
}

record _ArrayTypeInfo {
  type eltType;
  var dom: _domain;
}

def _init(a: _ArrayTypeInfo)
  return a.dom.buildArray(a.eltType);

def _build_array_type(dom, type eltType)
  return _ArrayTypeInfo(eltType, dom);

def _build_array(dom, type eltType)
  return dom.buildArray(eltType);

def _build_domain(x) // where x:_domain // see test/arrays/bradc/paulslice.chpl
  return x;                             // is * on ranges?

//
// computes && reduction over stridable of ranges
//
def _any_stridable(ranges, param d: int = 1) param {
  if ranges(d).stridable == true then
    return true;
  else if ranges.size > d then
    return _any_stridable(ranges, d+1);
  else
    return false;
}

def _build_domain(ranges: range(?eltType,bounded,?stridable) ...?rank) {
  type t = ranges(1).eltType;
  var d: domain(rank, t, _any_stridable(ranges));
  d.setIndices(ranges);
  return d;
}

def _build_open_interval_upper(x: _domain)
  return x.buildOpenIntervalUpper();

def _build_index_type(param rank: int, type idxType) where rank == 1 {
  var x: idxType;
  return x;
}

def _build_index_type(param rank: int, type idxType) where rank > 1 {
  var x: rank*idxType;
  return x;
}

def _build_index_type(param rank: int)
  return _build_index_type(rank, int);

def _build_index_type(d: _domain)
  return _build_index_type(d.rank, d._value.idxType);

pragma "domain"
record _domain {
  param rank : int;
  var _value;
  var _promotionType: index(rank, _value.idxType);

  def initialize() {
    if _value == nil {
      var x = buildEmptyDomain();
      _value = x._value;
    }
  }

  def these() {
    for i in _value.these() do
      yield i;
  }

  def this(d: _domain)
    return _domain(rank, _value.slice(d._value));

  def this(ranges: range(?eltType,?boundedType,?stridable) ...rank) {
    param stridable = _any_stridable(ranges);
    var newRanges: rank*range(_value.idxType, bounded, stridable);
    for param j in 1..rank {
      newRanges(j) = _value.dim(j)(ranges(j));
    }
    var x = _value.dist.buildDomain(rank, _value.idxType, stridable);
    x.setIndices(newRanges);
    return this(_domain(rank, x));
  }

  def dim(d : int)
    return _value.dim(d);

  def dimIter(param d, ind) {
    for i in _value.dimIter(d, ind) do yield i;
  }

  def buildArray(type eltType) {
    var x = _value.buildArray(eltType);
    _value._arrs.append(x);
    return _array(_value.idxType, eltType, rank, x);
  }

  // buildEmptyDomain is meant to return an uninitialized domain for
  // the given domain type. For example, it should return the same
  // sort of thing that var D: domain(2) dist(yourDist); would result
  // in if you didn't initialize it.  It should be a static method and
  // not refer to any fields that aren't types and params.
  def buildEmptyDomain() {
    var x = _value.buildEmptyDomain();
    return _domain(rank, x);
  }

  def buildSubdomain() {
    var x = _value.buildSubdomain();
    return _domain(rank, x);
  }

  def buildOpenIntervalUpper()
    return _domain(rank, _value.buildOpenIntervalUpper());

  def clear() {
    _value.clear();
  }

  def add(i) {
    _value.add(i);
  }

  def remove(i) {
    _value.remove(i);
  }

  def numIndices return _value.numIndices;
  def low return _value.low;
  def high return _value.high;

  def member(i) {
    return _value.member(i);
  }

  def expand(i: int ...rank)
    return expand(i);

  def expand(i: rank*int)
    return _domain(rank, _value.expand(i));

  def expand(i: int) where rank > 1
    return _domain(rank, _value.expand(i));

  def exterior(i: int ...rank)
    return exterior(i);

  def exterior(i: rank*int) {
    var x = _value.exterior(i);
    return _domain(rank, x);
  }

  def interior(i: int ...rank)
    return interior(i);

  def interior(i: rank*int) {
    var x = _value.interior(i);
    return _domain(rank, x);
  }

  def translate(i: int ...rank)
    return translate(i);

  def translate(i: rank*int) {
    var x = _value.translate(i);
    return _domain(rank, x);
  }

  def subBlocks {
    for d in _value.subBlocks do
      yield d;
  }

  def setIndices(x) {
    _value.setIndices(x);
  }

  def getIndices()
    return _value.getIndices();
}

def _isDomain(x: _domain) param return true;
def _isDomain(x) param return false;

def =(a: _domain, b: _domain) {
  //  if a == nil then return b; // stopgap: why? --sjd
  for e in a._value._arrs do
    e.reallocate(b);
  a._value.setIndices(b._value.getIndices());
  return a;
}

def =(a: _domain, b: _tuple) {
  for ind in 1..b.size {
    a.add(b(ind));
  }
  return a;
}

def =(a: _domain, b) {  // b is iteratable
  a.clear();
  for ind in b {
    a.add(ind);
  }
  return a;
}

pragma "inline" def _pass(a: _domain)
  return a;

def _init(a: _domain)
  return a.buildEmptyDomain();

def _copy(a: _domain) {
  var b: a.buildEmptyDomain();
  b.setIndices(a.getIndices());
  return b;
}

def _domain.writeThis(f: Writer) {
  f.write(_value);
}

def by(a: _domain, b) {
  var x = a._value.strideBy(b);
  return _domain(a.rank, x);
}

// this is a wrapper class for all arrays
pragma "array"
record _array {
  type _dim_index_type;
  type eltType;
  param rank : int;
  var _value;
  var _promotionType : eltType;

  pragma "valid lvalue"
  def _dom
    return _domain(rank, _value.dom);

  pragma "valid lvalue"
  def this(d: _domain) {
    _value.checkSlice(d._value);
    var x = _value.slice(_dom(d)._value);
    return _array(_dim_index_type, eltType, rank, x);
  }

  pragma "valid lvalue"
  def this(rs: range(?_eltType,?boundedType,?stridable) ...rank) {
    _value.checkSlice(rs);
    var x = _value.slice(_dom((...rs))._value);
    return _array(_dim_index_type, eltType, rank, x);
  }

  def this(i: rank*_dim_index_type) var where rank > 1
    return _value(i);

  def this(i: _dim_index_type ...rank) var where rank > 1
    return _value(i);

  def this(i: _dim_index_type) var where rank == 1
    return _value(i);

  def validRankChangeArguments(t) param {
    def validRankChangeArgument(r: range(?eltType,?boundedType,?stridable)) param
      return true;

    def validRankChangeArgument(i: _dim_index_type) param
      return true;

    def validRankChangeArgument(x) param
      return false;

    def help(param dim: int) param {
      if !validRankChangeArgument(t(dim)) then
        return false;
      else if dim < t.size then
        return help(dim+1);
      else
        return true;
    }
    return help(1);
  }

  pragma "valid lvalue"
  def this(irs ...rank) where validRankChangeArguments(irs) {
    def isRange(r: range(?e,?b,?s)) param return 1;
    def isRange(r) param return 0;
    def _tupleize(x) {
      var y: 1*x.type;
      y(1) = x;
      return y;
    }
    def collectRanges(param dim: int) {
      if isRange(irs(dim))
        return collectRanges(dim+1, _tupleize(irs(dim)));
      else
        return collectRanges(dim+1);
    }
    def collectRanges(param dim: int, x: _tuple) {
      if dim > irs.size {
        return x;
      } else if dim < irs.size {
        if isRange(irs(dim)) then
          return collectRanges(dim+1, ((...x), irs(dim)));
        else
          return collectRanges(dim+1, x);
      } else {
        if isRange(irs(dim)) then
          return ((...x), irs(dim));
        else
          return x;
      }
    }
    var rs = collectRanges(1);
    var x = _value.rankChange(rs.size, _any_stridable(rs), irs, rs);
    return _array(_dim_index_type, eltType, rs.size, x);
  }

  def these() var {
    for i in _value.these() do
      yield i;
  }

  def numElements return _dom.numIndices; // assume dom name

  def reindex(d: _domain) where rank == 1 {
    var x = _value.reindex(d._value);
    return _array(x.idxType, eltType, rank, x);
  }

  def reindex(d: _domain) where rank != 1 {
    var x = _value.reindex(d._value);
    type xDimIndexType = x.idxType;       // BLC: hacks to get around
    type xIndexType = rank*xDimIndexType;  // inflexibility of tuple syntax
    return _array(x.idxType, eltType, rank, x);
  }

  def IRV var {
    return _value.IRV;
  }
}

pragma "inline" def =(a: _array, b) {
  for (i,bb) in (a._dom,b) do
    a(i) = bb;
  return a;
}

def =(a: _array, b: _tuple) {
  a._value.tupleInit(b);
  return a;
}

def =(a: _array, b: a.eltType) {
  for i in a._dom do
    a(i) = b;
  return a;
}

def _copy(a: _array) {
  var b : [a._dom] a.eltType;
  b = a;
  return b;
}

pragma "inline" def _pass(a: _array) {
  return a;
}

def _init(a: _array) {
  return a._dom.buildArray(a.eltType);
}

def _array.writeThis(f: Writer) {
  f.write(_value);
}

// this is the class that all array classes are derived from
pragma "BaseArray"
class BaseArray {
  def reallocate(d: _domain) {
    halt("reallocating not support for this array type");
  }

  def purge( ind: int) {
    halt("purging not supported for this array type");
  }

  def _resize( length: int, old_map: _ddata(int)) {
    halt("resizing not supported for this array type");
  }

  def sparseShiftArray(shiftrange, initrange) {
    halt("sparseGrowDomain not supported for non-sparse arrays");
  }
}


class BaseDomain {
  var _arrs: list of BaseArray;

  def member(ind) : bool {
    halt("membership test not supported for this domain type");
    return false;
  }
}


class BaseArithmeticDomain : BaseDomain {
  def bbox(d:int) {
    halt("bbox not supported for this domain type");
    return 1..0;
  }
}


class BaseSparseArithmeticDomain : BaseArithmeticDomain {
}


pragma "inline" def _pass(ic: _iteratorClass)
  return ic;

def _copy(ic: _iteratorClass) {
  var capacity = 4, size = 0;
  var data = _ddata(__primitive("get_ic_type", ic), capacity);
  data.init();

  for e in ic {
    if size == capacity {
      capacity = capacity * 2;
      var tmp = _ddata(__primitive("get_ic_type", ic), capacity);
      tmp.init();
      for i in 0..size-1 do
        tmp(i) = data(i);
      data = tmp;
    }
    data(size) = e;
    size = size + 1;
  }

  var A: [1..size] __primitive("get_ic_type", ic);
  for i in 0..size-1 do
    A(i+1) = data(i);
  return A;
}

//
// print out iterators
//
def _iteratorClass.writeThis(f: Writer) {
  var first: bool = true;
  for e in this {
    if !first then
      f.write(" ");
    else
      first = false;
    f.write(e);
  }
}

//
// Swap operators for arrays and domains
//
pragma "inline" def _chpl_swap(x: [?D1], y:[?D2]) {
  for (i,j) in (D1, D2) do
    x(i) <=> y(j);
}


/*  This might be preferable for domains, but it doesn't work because
    domains apparently can't be modified once sent into a function!? 

pragma "inline" def _chpl_swap(x: domain, y: domain) {
  const t = y;
  y = x;
  x = y;
}
*/
