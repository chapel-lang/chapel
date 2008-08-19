def _build_domain_type(dist, param rank : int, type idxType = int,
                       param stridable = false) type
  return new _domain(rank, dist.buildDomain(rank, idxType, stridable));

def _build_domain_type(dist, type ind) type where !__primitive("isEnumType", ind) && !__primitive("isOpaqueType", ind)
  return new _domain(1, dist.buildDomain(ind));

def _build_domain_type(dist, type ind) type where __primitive("isEnumType", ind)
  return new _domain(1, dist.buildEnumDomain(ind));

def _build_domain_type(dist, type ind) type where __primitive("isOpaqueType", ind)
  return new _domain(1, dist.buildOpaqueDomain());

def _build_subdomain_type(dom) type
  return dom.buildSubdomain();

def _build_sparse_subdomain_type(dist, parentDom)
  return new _domain(parentDom.rank,
                     dist.buildSparseDomain(parentDom.rank,
                                            parentDom._value.idxType,
                                            parentDom._value));

def _build_opaque_domain_type(dist) type
  return new _domain(1, dist.buildOpaqueDomain());

def _build_array_type(dom, type eltType) type
  return dom.buildArray(eltType);

def _build_array(dom, type eltType)
  return dom.buildArray(eltType);

def _build_domain(x: _domain)
  return x;

def _build_domain(ranges: range(?eltType,BoundedRangeType.bounded,?stridable) ...?rank) {
  for param i in 2..rank do
    if eltType(1) != eltType(i) then
      compilerError("domain has mixed dimensional type");
  var d: domain(rank, eltType(1), _any_stridable(ranges));
  d.setIndices(ranges);
  return d;
}

def _wrapDomain(d) return new _domain(d.rank, d);

//
// computes || reduction over stridable of ranges
//
def _any_stridable(ranges, param d: int = 1) param {
  if ranges(d).stridable == true then
    return true;
  else if ranges.size > d then
    return _any_stridable(ranges, d+1);
  else
    return false;
}

def _build_index_type(param rank: int, type idxType) type where rank == 1 {
  var x: idxType;
  return x;
}

def _build_index_type(param rank: int, type idxType) type where rank > 1 {
  var x: rank*idxType;
  return x;
}

def _build_index_type(param rank: int) type
  return _build_index_type(rank, int);

def _build_index_type(d: _domain) type
  return _build_index_type(d.rank, d._value.idxType);

def _build_index_type(type idxType) type where __primitive("isOpaqueType", idxType)
  return _OpaqueIndex;

//
// given a tuple args, returns true if the tuple contains only
// integers and ranges; that is, it is a valid argument list for rank
// change
//
def _validRankChangeArgs(args, type idxType) param {
  def _validRankChangeArg(type idxType, r: range(?e,?b,?s)) param return true;
  def _validRankChangeArg(type idxType, i: idxType) param return true;
  def _validRankChangeArg(type idxType, x) param return false;

  def help(param dim: int) param {
    if !_validRankChangeArg(idxType, args(dim)) then
      return false;
    else if dim < args.size then
      return help(dim+1);
    else
      return true;
  }

  return help(1);
}

def _getRankChangeRanges(args) {
  def isRange(r: range(?e,?b,?s)) param return 1;
  def isRange(r) param return 0;
  def _tupleize(x) {
    var y: 1*x.type;
    y(1) = x;
    return y;
  }
  def collectRanges(param dim: int) {
    if dim > args.size then
      compilerError("domain slice requires a range in at least one dimension");
    if isRange(args(dim)) then
      return collectRanges(dim+1, _tupleize(args(dim)));
    else
      return collectRanges(dim+1);
  }
  def collectRanges(param dim: int, x: _tuple) {
    if dim > args.size {
      return x;
    } else if dim < args.size {
      if isRange(args(dim)) then
        return collectRanges(dim+1, ((...x), args(dim)));
      else
        return collectRanges(dim+1, x);
    } else {
      if isRange(args(dim)) then
        return ((...x), args(dim));
      else
        return x;
    }
  }
  return collectRanges(1);
}

pragma "domain"
record _domain {
  param rank : int;
  var _value;
  var _promotionType: index(rank, _value.idxType);

  def initialize() {
    if _value == nil {
      _value = _value.buildEmptyDomain();
    }
    return this;
  }

  def these() {
    for i in _value.these() do
      yield i;
  }

  def this(ranges: range(?eltType,?boundedType,?stridable) ...rank)
    return new _domain(rank, _value.slice(_any_stridable(ranges), ranges));

  def this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
    var ranges = _getRankChangeRanges(args);
    param rank = ranges.size, stridable = _any_stridable(ranges);
    return new _domain(rank, _value.rankChange(rank, stridable, args));
  }

  def dim(d : int)
    return _value.dim(d);

  def dimIter(param d, ind) {
    for i in _value.dimIter(d, ind) do yield i;
  }

  def buildArray(type eltType) {
    var x = _value.buildArray(eltType);
    if !_value._locked then {
      _value._arrs.append(x);
      _value._locked = false;  // writing to this sync var "unlocks" the lock!
    }
    return new _array(_value.idxType, eltType, rank, x);
  }

  // buildEmptyDomain is meant to return an uninitialized domain for
  // the given domain type. For example, it should return the same
  // sort of thing that var D: domain(2) dist(yourDist); would result
  // in if you didn't initialize it.  It should be a static method and
  // not refer to any fields that aren't types and params.
  def buildEmptyDomain() {
    var x = _value.buildEmptyDomain();
    return new _domain(rank, x);
  }

  def buildSubdomain() {
    var x = _value.buildSubdomain();
    return new _domain(rank, x);
  }

  def buildOpenIntervalUpper()
    return new _domain(rank, _value.buildOpenIntervalUpper());

  def clear() {
    _value.clear();
  }

  def create() {
    if _value.idxType != _OpaqueIndex then
      compilerError("domain.create() only applies to opaque domains");
    return _value.create();
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

  def order(i) {
    return _value.order(i);
  }

  def position(i) {
    return _value.position(i);
  }

  def expand(i: int ...rank)
    return expand(i);

  def expand(i: rank*int)
    return new _domain(rank, _value.expand(i));

  def expand(i: int) where rank > 1
    return new _domain(rank, _value.expand(i));

  def exterior(i: int ...rank)
    return exterior(i);

  def exterior(i: rank*int) {
    var x = _value.exterior(i);
    return new _domain(rank, x);
  }

  def interior(i: int ...rank)
    return interior(i);

  def interior(i: rank*int) {
    var x = _value.interior(i);
    return new _domain(rank, x);
  }

  def translate(i: int ...rank)
    return translate(i);

  def translate(i: rank*int) {
    var x = _value.translate(i);
    return new _domain(rank, x);
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

  // associative array interface

  def sorted() {
    for i in _value.sorted() {
      yield i;
    }
  }
}

def _isDomain(x: _domain) param return true;
def _isDomain(x) param return false;

def =(a: _domain, b: _domain) {
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

def =(d: _domain, r: range(?e,?b,?s)) {
  d = _build_domain(r);
  return d;
}

def =(a: _domain, b) {  // b is iteratable
  a.clear();
  for ind in b {
    a.add(ind);
  }
  return a;
}

def _init(a: _domain)
  return a.buildEmptyDomain();

def _copy(a: _domain) {
  var b: a.buildEmptyDomain().type;
  b.setIndices(a.getIndices());
  return b;
}

def _domain.writeThis(f: Writer) {
  f.write(_value);
}

def by(a: _domain, b) {
  var x = a._value.strideBy(b);
  return new _domain(a.rank, x);
}

// this is a wrapper class for all arrays
pragma "array"
record _array {
  type idxType;
  type eltType;
  param rank : int;
  var _value;
  var _promotionType : eltType;

  def _dom
    return new _domain(rank, _value.dom);

  pragma "inline"
  def this(i: rank*idxType) var where rank > 1
    return _value(i);

  pragma "inline"
  def this(i: idxType ...rank) var where rank > 1
    return _value(i);

  pragma "inline"
  def this(i: idxType) var where rank == 1
    return _value(i);

  //
  // requires dense domain implementation that returns a tuple of
  // ranges via the getIndices() method; domain indexing is difficult
  // in the domain case because it has to be implemented on a
  // domain-by-domain basis; this is not terribly difficult in the
  // dense case because we can represent a domain by a tuple of
  // ranges, but in the sparse case, is there a general
  // representation?
  //
  pragma "valid var"
  def this(d: _domain) var where d.rank == rank
    return this((...d.getIndices()));

  pragma "valid var"
  def this(ranges: range(?_eltType,?boundedType,?stridable) ...rank) var {
    if boundsChecking then
      _value.checkSlice(ranges);
    return new _array(idxType, eltType, rank, _value.slice(_dom((...ranges))._value));
  }

  pragma "valid var"
  def this(args ...rank) var where _validRankChangeArgs(args, idxType) {
    if boundsChecking then
      _value.checkRankChange(args);
    var ranges = _getRankChangeRanges(args);
    param rank = ranges.size, stridable = _any_stridable(ranges);
    return new _array(idxType, eltType, rank, _value.rankChange(rank, stridable, args));
  }

  def these() var {
    for i in _value.these() do
      yield i;
  }

  def numElements return _dom.numIndices; // assume dom name

  def reindex(d: _domain) where rank == 1 {
    var x = _value.reindex(d._value);
    return new _array(x.idxType, eltType, rank, x);
  }

  def reindex(d: _domain) where rank != 1 {
    var x = _value.reindex(d._value);
    return new _array(x.idxType, eltType, rank, x);
  }

  // sparse array interface

  def IRV var {
    return _value.IRV;
  }

  // associative array interface

  def sorted() {
    for i in _value.sorted() {
      yield i;
    }
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

def _desync(type t) where t: _syncvar || t: _singlevar {
  var x: t;
  return x.value;
}

def _desync(type t) where !(t: _syncvar|| t: _singlevar) {
  var x: t;
  return x;
}

def =(a: _array, b: _desync(a.eltType)) {
  for i in a._dom do
    a(i) = b;
  return a;
}

def _copy(a: _array) {
  var b : [a._dom] a.eltType;
  b = a;
  return b;
}

def _init(a: _array) {
  return a._dom.buildArray(a.eltType);
}

def _init(type t) where t: _array {
  return __primitive("build_array", t);
}

def _array.writeThis(f: Writer) {
  f.write(_value);
}

// this is the class that all array classes are derived from
pragma "base array"
class BaseArray {
  def reallocate(d: _domain) {
    halt("reallocating not support for this array type");
  }

  // This method is unsatisfactory -- see bradc's commit entries of
  // 01/02/08 around 14:30 for details
  def _purge( ind: int) {
    halt("purging not supported for this array type");
  }

  def _resize( length: int, old_map) {
    halt("resizing not supported for this array type");
  }

  def sparseShiftArray(shiftrange, initrange) {
    halt("sparseGrowDomain not supported for non-sparse arrays");
  }

  // methods for associative arrays
  def _backupArray() {
    halt("_backupArray() not supported for non-associative arrays");
  }

  def _removeArrayBackup() {
    halt("_removeArrayBackup() not supported for non-associative arrays");
  }

  def _preserveArrayElement(oldslot, newslot) {
    halt("_preserveArrayElement() not supported for non-associative arrays");
  }
}


class BaseDomain {
  var _arrs: list(BaseArray),
      _locked: sync bool = false;

  def member(ind) : bool {
    halt("membership test not supported for this domain type");
    return false;
  }

  // used for associative domains/arrays
  def _backupArrays() {
    for arr in _arrs do
      arr._backupArray();
  }

  def _removeArrayBackups() {
    for arr in _arrs do
      arr._removeArrayBackup();
  }

  def _preserveArrayElement(oldslot, newslot) {
    for arr in _arrs do
      arr._preserveArrayElement(oldslot, newslot);
  }
}


class BaseArithmeticDomain : BaseDomain {
  def bbox(d:int) {
    halt("bbox not supported for this domain type");
    return 1..0;
  }

  def clear() {
    compilerError("Illegal assignment to an arithmetic domain");
  }
}


class BaseSparseArithmeticDomain : BaseArithmeticDomain {
}

//
// index for all opaque domains
//
class _OpaqueIndex { }

//
// Swap operators for arrays and domains
//
pragma "inline" def _chpl_swap(x: [], y: []) {
  for (i,j) in (x.domain, y.domain) do
    x(i) <=> y(j);
}

pragma "inline" def _chpl_swap(x: domain, y: domain) {
  const t = y;
  y = x;
  x = t;
}

//
// reshape function
//
def reshape(A: [], D: domain) {
  var B: [D] A.eltType;
  for (i,a) in (D,A) do
    B(i) = a;
  return B;
}

// This should go away once distributions are implemented
def distributed_warning(d) {
  if (numLocales > 1) {
    __primitive("chpl_warning", "'distributed' domains/arrays are not yet distributed across multiple locales");
  }
  return d;
}

//
// module support for iterators
//
def iteratorIndex(ic: _iteratorClass) {
  ic.advance();
  return ic.getValue();
}

pragma "expand tuples with values"
def iteratorIndex(t: _tuple) {
  pragma "expand tuples with values"
  def iteratorIndexHelp(t: _tuple, param dim: int) {
    if dim == t.size-1 then
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)), iteratorIndex(t(dim+1)));
    else
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)), (...iteratorIndexHelp(t, dim+1)));
  }

  return iteratorIndexHelp(t, 1);
}

def iteratorIndexType(x) type {
  return iteratorIndex(x).type;
}

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

def =(ic: _iteratorClass, xs) {
  for (e, x) in (ic, xs) do
    e = x;
  return ic;
}

def =(ic: _iteratorClass, x: iteratorIndexType(ic)) {
  for e in ic do
    e = x;
  return ic;
}

def _copy(ic: _iteratorClass) {
  return _ic_copy_help(_ic_copy_recursive(ic));

  def _ic_copy_recursive(ic) {
    for e in ic do
      yield _copy(e);
  }

  def _ic_copy_help(ic) {
    var i = 1, size = 4;
    var D = [1..size];

    // note that _getIterator is called in order to copy the iterator
    // class since for arrays we need to iterate once to get the
    // element type (at least for now); this also means that if this
    // iterator has side effects, we will see them; a better way to
    // handle this may be to get the static type (not initialize the
    // array) and use a primitive to set the array's element; that may
    // also handle skyline arrays
    var A: [D] iteratorIndexType(_getIterator(ic));

    for e in ic {
      if i > size {
        size = size * 2;
        D = [1..size];
      }
      A(i) = e;
      i = i + 1;
    }
    D = [1..i-1];
    return A;
  }
}

pragma "inline" pragma "iterator class copy"
def _getIterator(ic: _iteratorClass)
  return ic;

pragma "inline" pragma "ref" def _getIterator(x)
  return x.these();

pragma "inline" def _getIterator(x: _tuple) {
  pragma "inline" def _getIteratorHelp(x: _tuple, param dim: int) {
    if dim == x.size-1 then
      return (_getIterator(x(dim)), _getIterator(x(dim+1)));
    else
      return (_getIterator(x(dim)), (..._getIteratorHelp(x, dim+1)));
  }
  return _getIteratorHelp(x, 1);
}

def _getIterator(type t) {
  compilerError("cannot iterate over a type");
}

pragma "inline"
def _toLeader(iterator: _iteratorClass)
  return __primitive("to leader", iterator);

pragma "inline"
def _toLeader(x: _tuple)
  return _toLeader(x(1));

pragma "inline"
def _toFollower(iterator: _iteratorClass, leaderIndex)
  return __primitive("to follower", iterator, leaderIndex);

pragma "inline"
def _toFollower(x: _tuple, leaderIndex) {
  pragma "inline" def _toFollowerHelp(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 then
      return (_toFollower(x(dim), leaderIndex), _toFollower(x(dim+1), leaderIndex));
    else
      return (_toFollower(x(dim), leaderIndex), (..._toFollowerHelp(x, leaderIndex, dim+1)));
  }
  return _toFollowerHelp(x, leaderIndex, 1);
}
