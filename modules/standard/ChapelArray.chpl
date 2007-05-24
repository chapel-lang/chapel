use List;

def _build_domain_type(dist, param rank : int,
                       type dimensional_index_type = int) {
  var x = dist.buildDomain(rank, dimensional_index_type);
  if rank > 1 {
    type index_type = rank*dimensional_index_type;
    return _domain(x.type, index_type, dimensional_index_type, rank, x);
  } else {
    return _domain(x.type, dimensional_index_type, dimensional_index_type, rank, x);
  }
}

def _build_domain_type(dist, type ind) where !__primitive("isEnumType", ind) {
  var x = dist.buildDomain(ind);
  return _domain(x.type, ind, ind, 1, x);
}

def _build_domain_type(dist, type ind) where __primitive("isEnumType", ind) {
  var x = dist.buildEnumDomain(ind);
  return _domain(x.type, ind, ind, 1, x);
}

def _build_subdomain_type(dom)
  return dom.buildSubdomain();

def _build_sparse_subdomain_type(dist, parentDom) {
  var x = dist.buildSparseDomain(parentDom.rank, parentDom._dim_index_type, parentDom._value);
  // This index(2) is lame/incorrect, but the technique of using the
  // iterator to find out the index type is lame too -- all of which
  // could be fixed if we could have tuple types within classes.
  return _domain(x.type, index(2), parentDom._dim_index_type, parentDom.rank, x);
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
  param domain_stridable = _any_stridable(ranges);
  var d: domain(rank, t)
           distributed (SingleLocaleDistribution(stridable=domain_stridable));
  d.setIndices(ranges);
  return d;
}

def _build_open_interval_upper(x: _domain)
  return x.buildOpenIntervalUpper();

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

pragma "domain"
record _domain {
  type _domain_type;
  type _index_type;
  type _dim_index_type;
  param rank : int;
  var _value : _domain_type;
  var _promotionType : _index_type;

  def initialize() {
    if _value == nil {
      var x = buildEmptyDomain();
      _value = x._value;
    }
  }

  iterator ault() {
    for i in _value.ault() do
      yield i;
  }

  // slicing a domain using a varargs set of ranges of rank elements
  def this(i: range(_dim_index_type,?bnd,?str) ... rank) {
    // assuming stridable dependent on incoming ranges for now;
    // really should check stridablility of current domain as well to be
    // correct.
    var newRanges: rank*range(_dim_index_type, bounded, stridable=_any_stridable(i));
    for param d in 1..rank {
      newRanges(d) = _value.dim(d)(i(d));
    }
    // this should really be something like:
    //           var subdomainClass = dist.buildDomain(...)
    // but it seems that domains don't store a pointer to their
    // distribution currently, so we build a SingleLocaleArithmetic
    // for now.  Ugh!
    return [(...newRanges)];
    // And then we would do:
    //    return _domain(_domain_type, _index_type, _dim_index_type, rank, 
    //                   subdomainClass);
  }

  def dim(d : int)
    return _value.dim(d);

  iterator dimIter(param d, ind) {
    for i in _value.dimIter(d, ind) do yield i;
  }

  def buildArray(type eltType) {
    var x = _value.buildArray(eltType);
    _value._arrs.append(x);
    return _array(x.type, _index_type, _dim_index_type, eltType, rank, x);
  }

  // buildEmptyDomain is meant to return an uninitialized domain for
  // the given domain type. For example, it should return the same
  // sort of thing that var D: domain(2) dist(yourDist); would result
  // in if you didn't initialize it.  It should be a static method and
  // not refer to any fields that aren't types and params.
  def buildEmptyDomain() {
    var x = _value.buildEmptyDomain();
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def buildSubdomain() {
    var x = _value.buildSubdomain();
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def buildOpenIntervalUpper()
    return _domain(_domain_type, _index_type, _dim_index_type, rank, _value.buildOpenIntervalUpper());

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

  def expand(i: rank*int) {
    var x = _value.expand(i);
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def expand(i: int) where rank > 1 {
    var x = _value.expand(i);
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def exterior(i: int ...rank)
    return exterior(i);

  def exterior(i: rank*int) {
    var x = _value.exterior(i);
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def interior(i: int ...rank)
    return interior(i);

  def interior(i: rank*int) {
    var x = _value.interior(i);
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  def translate(i: int ...rank)
    return translate(i);

  def translate(i: rank*int) {
    var x = _value.translate(i);
    return _domain(x.type, _index_type, _dim_index_type, rank, x);
  }

  iterator subBlocks {
    for d in _value.subBlocks do
      yield d;
  }

  def setIndices(x) {
    _value.setIndices(x);
  }

  def getIndices()
    return _value.getIndices();
}

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
  return _domain(x.type, a._index_type, a._dim_index_type, a.rank, x);
}

// this is a wrapper class for all arrays
pragma "array"
record _array {
  type _array_type;
  type _index_type;
  type _dim_index_type;
  type eltType;
  param rank : int;
  var _value : _array_type;
  var _promotionType : eltType;

  def _dom var {
    var x : _domain(_value.dom.type, _index_type, _dim_index_type, rank, _value.dom);
    //    x._value = _value.dom;
    return x;
  }

  def this(d: _domain) {
    var x = _value.slice(d._value);
    return _array(x.type, _index_type, _dim_index_type, eltType, rank, x);
  }

  def this(i: range(_dim_index_type,?bnd,?stridable) ...rank) {
    var d = _dom[(...i)];
    return this(d);
  }

  def =this(d: _domain, val) {
    var y = _array(_array_type, _index_type, _dim_index_type, eltType, rank);
    y._value = _value.slice(d._value);
    y = val;
  }

  def =this(d: _domain, val: eltType) {
    var y = _array(_array_type, _index_type, _dim_index_type, eltType, rank);
    y._value = _value.slice(d._value);
    y = val;
  }

  def =this(i: range(_dim_index_type,?bnd,?stridable) ...rank, val) where rank > 0 {
    var d = _dom[(...i)];
    var y = _array(_array_type, _index_type, _dim_index_type, eltType, rank);
    y._value = _value.slice(d._value);
    y = val;
  }

  def =this(i: range(_dim_index_type,?bnd,?stridable) ...rank, val: eltType) where rank > 0 {
    var d = _dom[(...i)];
    var y = _array(_array_type, _index_type, _dim_index_type, eltType, rank);
    y._value = _value.slice(d._value);
    y = val;
  }

  def this(i: _index_type)
    return _value(i);

  def this(i: _dim_index_type ...rank) where rank > 1
    return _value(i);

  def =this(i: _index_type, val: eltType) {
    _value(i) = val;
  }

  def =this(i: _dim_index_type ...rank, val: eltType) where rank > 1 {
    _value(i) = val;
  }

  iterator ault() {
    for i in _value.ault() do
      yield i;
  }

  def numElements return _dom.numIndices; // assume dom name

  def reindex(d: _domain) {
    var x = _value.reindex(d._value);
    return _array(x.type, _index_type, _dim_index_type, eltType, rank, x);
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

iterator _tupleIterate(param rank: int, b: _tuple) {
  if rank == 1 {
    for param i in 1..b.size do
      yield b(i);
  } else {
    for param i in 1..b.size do
      for bb in _tupleIterate(rank-1, b(i)) do
        yield bb;
  }
}

def =(a: _array, b: _tuple) {
  for (i,bb) in (a._dom,_tupleIterate(a.rank,b)) do
    a(i) = bb;
  return a;
}

def =(a: _array, b: a.eltType) {
  for i in a._dom do
    a(i) = b;
  return a;
}

def _copy(a: _array) {
  if a._value == nil then return a;
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
