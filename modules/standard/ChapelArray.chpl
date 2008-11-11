var privatizeLock$: sync int;

def _supportsPrivatization(value) param
  return _privatization & value.supportsPrivatization();

def _newArrayDomainSharedCode(param isArray: bool, value) {
  if _supportsPrivatization(value) {
    privatizeLock$.writeEF(true);
    var n = __primitive("chpl_numPrivateClasses");
    for loc in Locales {
      if loc != here {
        on loc {
          var mine = value.privatize1();
          __primitive("chpl_setPrivateClass", mine);
          mine.privatize2(n);
        }
      } else {
        __primitive("chpl_setPrivateClass", value);
        value.privatize2(n);
      }
    }
    privatizeLock$.readFE();
    if isArray then
      return new _array(n, value);
    else
      return new _domain(n, value);
  } else {
    if isArray then
      return new _array(value, value);
    else
      return new _domain(value, value);
  }
}

def _newArray(value) {
  return _newArrayDomainSharedCode(true, value);
}

def _newDomain(value) {
  return _newArrayDomainSharedCode(false, value);
}

def _getDomain(value) {
  if _supportsPrivatization(value) {
    return new _domain(value.pid, value);
  } else {
    return new _domain(value, value);
  }
}


//
// Support for domain types
//
pragma "has runtime type"
def chpl__buildDomainRuntimeType(dist, param rank: int, type idxType = int(32),
                       param stridable: bool = false) type
  return _newDomain(dist.newArithmeticDomain(rank, idxType, stridable));

pragma "has runtime type"
def chpl__buildDomainRuntimeType(dist, type idxType) type
 where !__primitive("isEnumType", idxType) && idxType != opaque && idxType != _OpaqueIndex
  return _newDomain(dist.newAssociativeDomain(idxType));

pragma "has runtime type"
def chpl__buildDomainRuntimeType(dist, type idxType) type
 where __primitive("isEnumType", idxType)
  return _newDomain(dist.newEnumeratedDomain(idxType));

pragma "has runtime type"
def chpl__buildDomainRuntimeType(dist, type idxType) type
 where idxType == _OpaqueIndex
  return _newDomain(dist.newOpaqueDomain(idxType));

// This function has no 'has runtime type' pragma since the idxType of
// opaque domains is _OpaqueIndex, not opaque.  This function is
// essentially a wrapper around the function that actually builds up
// the runtime type.
def chpl__buildDomainRuntimeType(dist, type idxType) type
 where idxType == opaque
  return chpl__buildDomainRuntimeType(dist, _OpaqueIndex);

pragma "has runtime type"
def chpl__buildSparseDomainRuntimeType(dist, dom: domain) type
  return _newDomain(dist.newSparseDomain(dom.rank, dom._value.idxType, dom));

def chpl__convertValueToRuntimeType(dom: domain) type
 where dom._value:BaseArithmeticDomain
  return chpl__buildDomainRuntimeType(dom._value.dist, dom._value.rank,
                            dom._value.idxType, dom._value.stridable);

def chpl__convertValueToRuntimeType(dom: domain) type
 where dom._value:BaseSparseDomain
  return chpl__buildSparseDomainRuntimeType(dom._value.dist, dom._value.parentDom);

def chpl__convertValueToRuntimeType(dom: domain) type
where !dom._value:BaseArithmeticDomain && !dom._value:BaseSparseDomain
  return chpl__buildDomainRuntimeType(dom._value.dist, dom._value.idxType);

//
// Support for array types
//
pragma "has runtime type"
def chpl__buildArrayRuntimeType(dom: domain, type eltType) type
  return dom.buildArray(eltType);

def chpl__convertValueToRuntimeType(arr: []) type
  return chpl__buildArrayRuntimeType(arr.domain, arr.eltType);

//
// Support for subdomain types
//
// Note the domain of a subdomain is not yet part of the runtime type
//
def chpl__buildSubDomainType(dom: domain) type
  return chpl__convertValueToRuntimeType(dom);

//
// Support for domain expressions, e.g., [1..3, 1..3]
//
def chpl__buildDomainExpr(x: domain)
  return x;

def chpl__buildDomainExpr(ranges: range(?) ...?rank) {
  for param i in 2..rank {
    if ranges(1).eltType != ranges(i).eltType then
      compilerError("domain has mixed dimensional type");
    if ranges(i).boundedType != BoundedRangeType.bounded then
      compilerError("domain has dimension of unbounded range");
  }
  var d: domain(rank, ranges(1).eltType, chpl__anyStridable(ranges));
  d.setIndices(ranges);
  return d;
}

//
// Support for index types
//
def chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
  var x: idxType;
  return x;
}

def chpl__buildIndexType(param rank: int, type idxType) type where rank > 1 {
  var x: rank*idxType;
  return x;
}

def chpl__buildIndexType(param rank: int) type
  return chpl__buildIndexType(rank, int);

def chpl__buildIndexType(d: domain) type
  return chpl__buildIndexType(d.rank, d._value.idxType);

def chpl__buildIndexType(type idxType) type where idxType == opaque
  return _OpaqueIndex;

def isArithmeticDomain(d: domain) param {
  def isArithmeticDomainClass(dc: BaseArithmeticDomain) param return true;
  def isArithmeticDomainClass(dc) param return false;
  return isArithmeticDomainClass(d._value);
}

def isAssociativeDomain(d: domain) param {
  def isAssociativeDomainClass(dc: BaseAssociativeDomain) param return true;
  def isAssociativeDomainClass(dc) param return false;
  return isAssociativeDomainClass(d._value);
}

def isEnumDomain(d: domain) param {
  def isEnumDomainClass(dc: BaseEnumDomain) param return true;
  def isEnumDomainClass(dc) param return false;
  return isEnumDomainClass(d._value);
}

def isOpaqueDomain(d: domain) param {
  def isOpaqueDomainClass(dc: BaseOpaqueDomain) param return true;
  def isOpaqueDomainClass(dc) param return false;
  return isOpaqueDomainClass(d._value);
}

def isSparseDomain(d: domain) param {
  def isSparseDomainClass(dc: BaseSparseDomain) param return true;
  def isSparseDomainClass(dc) param return false;
  return isSparseDomainClass(d._value);
}

//
// Domain wrapper record
//
pragma "domain"
pragma "has runtime type"
record _domain {
  var _valueField;
  var _valueTypeField; // stores type of privatized domains
  var _promotionType: index(rank, _value.idxType);

  def initialize() { 
    _valueField._count += 1;
  }

  def _value {
    if _supportsPrivatization(_valueTypeField) {
      var tc = _valueTypeField;
      var id = _valueField;
      var pc = __primitive("chpl_getPrivateClass", tc, id);
      return pc;
    } else {
      return _valueField;
    }
  }

  def ~_domain () {
    _value.destroyDom();
  }

  def dist return _value.dist;

  def rank param {
    if isArithmeticDomain(this) || isSparseDomain(this) then
      return _value.rank;
    else
      return 1;
  }

  pragma "inline"
  def these() {
    return _value.these();
  }

  def this(ranges: range(?) ...rank)
    return _newDomain(_value.slice(chpl__anyStridable(ranges), ranges));

  def this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
    var ranges = _getRankChangeRanges(args);
    param rank = ranges.size, stridable = chpl__anyStridable(ranges);
    return _newDomain(_value.rankChange(rank, stridable, args));
  }

  def dim(d : int) return _value.dim(d);

  def dimIter(param d, ind) {
    for i in _value.dimIter(d, ind) do yield i;
  }

  def buildArray(type eltType) {
    var x = _value.buildArray(eltType);
    var cnt = _value._count; // lock
    _value._arrs.append(x);
    _value._count = cnt + 1; // unlock
    return _newArray(x);
  }

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

  def member(i) return _value.member(i);
  def order(i) return _value.order(i);
  def position(i) return _value.position(i);

  def expand(i: _value.idxType ...rank) return expand(i);
  def expand(i: rank*_value.idxType) return _newDomain(_value.expand(i));
  def expand(i: _value.idxType) where rank > 1 return _newDomain(_value.expand(i));

  def exterior(i: _value.idxType ...rank) return exterior(i);
  def exterior(i: rank*_value.idxType) return _newDomain(_value.exterior(i));

  def interior(i: _value.idxType ...rank) return interior(i);
  def interior(i: rank*_value.idxType) return _newDomain(_value.interior(i));

  def translate(i: _value.idxType ...rank) return translate(i);
  def translate(i: rank*_value.idxType) return _newDomain(_value.translate(i));

  def chpl__unTranslate(i: _value.idxType ...rank) return chpl__unTranslate(i);
  def chpl__unTranslate(i: rank*_value.idxType) return _newDomain(_value.chpl__unTranslate(i));

  def subBlocks {
    for d in _value.subBlocks do
      yield d;
  }

  def setIndices(x) {
    _value.setIndices(x);
  }

  def getIndices()
    return _value.getIndices();

  def writeThis(f: Writer) {
    f.write(_value);
  }

  // associative array interface

  def sorted() {
    for i in _value.sorted() {
      yield i;
    }
  }
}

def +(d: domain, i: index(d)) {
  return d.translate(i);
}

def +(i, d: domain) where i: index(d) {
  return d.translate(i);
}

def -(d: domain, i: index(d)) {
  return d.chpl__unTranslate(i);
}


//
// Array wrapper record
//
pragma "array"
pragma "has runtime type"
record _array {
  var _valueField;
  var _valueTypeField; // stores type of privatized arrays
  var _promotionType: _value.eltType;

  def _value {
    if _supportsPrivatization(_valueTypeField) {
      var tc = _valueTypeField;
      var id = _valueField;
      var pc = __primitive("chpl_getPrivateClass", tc, id);
      return pc;
    } else {
      return _valueField;
    }
  }

  def ~_array() {
    delete _value;
  }

  def eltType type return _value.eltType;
  def _dom return _getDomain(_value.dom);
  def rank param return this.domain.rank;

  pragma "inline"
  def this(i: rank*_value.idxType) var where rank > 1
    return _value(i);

  pragma "inline"
  def this(i: _value.idxType ...rank) var where rank > 1
    return _value(i);

  pragma "inline"
  def this(i: _value.idxType) var where rank == 1
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
  def this(d: domain) var where d.rank == rank
    return this((...d.getIndices()));

  pragma "valid var"
  def this(ranges: range(?) ...rank) var {
    if boundsChecking then
      _value.checkSlice(ranges);
    return _newArray(_value.slice(_dom((...ranges))._value));
  }

  pragma "valid var"
  def this(args ...rank) var where _validRankChangeArgs(args, _value.idxType) {
    if boundsChecking then
      _value.checkRankChange(args);
    var ranges = _getRankChangeRanges(args);
    param rank = ranges.size, stridable = chpl__anyStridable(ranges);
    return _newArray(_value.rankChange(rank, stridable, args));
  }

  pragma "inline"
  def these() var {
    return _value.these();
  }

  def numElements return _dom.numIndices; // assume dom name

  def reindex(d: domain) where rank == 1 {
    var x = _value.reindex(d._value);
    return _newArray(x);
  }

  def reindex(d: domain) where rank != 1 {
    var x = _value.reindex(d._value);
    return _newArray(x);
  }

  def writeThis(f: Writer) {
    f.write(_value);
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

//
// Helper functions
//

// computes || reduction over stridable of ranges
def chpl__anyStridable(ranges, param d: int = 1) param {
  for param i in 1..ranges.size do
    if ranges(i).stridable then
      return true;
  return false;
}

// given a tuple args, returns true if the tuple contains only
// integers and ranges; that is, it is a valid argument list for rank
// change
def _validRankChangeArgs(args, type idxType) param {
  def _validRankChangeArg(type idxType, r: range(?)) param return true;
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
  def isRange(r: range(?)) param return 1;
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

//
// Support for += and -= over domains
//
def chpl__isDomain(x: domain) param return true;
def chpl__isDomain(x) param return false;

//
// Assignment of domains and arrays
//
def =(a: domain, b: domain) {
  for e in a._value._arrs do
    e.reallocate(b);
  a._value.setIndices(b._value.getIndices());
  return a;
}

def =(a: domain, b: _tuple) {
  for ind in 1..b.size {
    a.add(b(ind));
  }
  return a;
}

def =(d: domain, r: range(?)) {
  d = [r];
  return d;
}

def =(a: domain, b) {  // b is iteratable
  a._value.clearForIteratableAssign();
  for ind in b {
    a.add(ind);
  }
  return a;
}

def _copy(a: domain) {
  var b: a.type;
  b.setIndices(a.getIndices());
  return b;
}

pragma "inline" def =(a: [], b) {
  for (i,bb) in (a._dom,b) do
    a(i) = bb;
  return a;
}

def =(a: [], b: _tuple) {
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

def =(a: [], b: _desync(a.eltType)) {
  for i in a._dom do
    a(i) = b;
  return a;
}

def _copy(a: []) {
  var b : [a._dom] a.eltType;
  b = a;
  return b;
}



def by(a: domain, b) {
  var x = a._value.strideBy(b);
  return _newDomain(x);
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

def _callSupportsAlignedFollower(A) param where A: BaseArray
  return A.supportsAlignedFollower();

def _callSupportsAlignedFollower(A) param where !(A: BaseArray)
  return false;

def _callSupportsAlignedFollower() param
  return false;

//
// module support for iterators
//
enum iterator {leader, follower};

def iteratorIndex(ic: _iteratorClass) {
  ic.advance();
  return ic.getValue();
}

pragma "expand tuples with values"
def iteratorIndex(t: _tuple) {
  pragma "expand tuples with values"
  def iteratorIndexHelp(t: _tuple, param dim: int) {
    if dim == t.size-1 then
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                           iteratorIndex(t(dim+1)));
    else
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                           (...iteratorIndexHelp(t, dim+1)));
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
def _toLeader(x)
  return _toLeader(x.these());

pragma "inline"
def _toFollower(iterator: _iteratorClass, leaderIndex)
  return __primitive("to follower", iterator, leaderIndex);

//
// If aligned is passed as an argument (true from alignment version of
// _toFollower on tuple) then grab the aligned version of the array's
// iterator if the array supports aligned followers
//
pragma "inline"
def _toFollower(iterator: _iteratorClass, leaderIndex, param aligned: bool) {
  return __primitive("to follower", iterator, leaderIndex, aligned);
}

pragma "inline"
def _toFollower(x: _tuple, leaderIndex) {
  pragma "inline" def _toFollowerHelp(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 then
      return (_toFollower(x(dim), leaderIndex),
              _toFollower(x(dim+1), leaderIndex));
    else
      return (_toFollower(x(dim), leaderIndex),
              (..._toFollowerHelp(x, leaderIndex, dim+1)));
  }
  return _toFollowerHelp(x, leaderIndex, 1);
}

//
// The alignment version uses a compiler analysis to pass a tuple of
// types according to whether the arrays in the tuple x have the same
// distribution as the leader or not.  If they do, the component type
// is passed as Distribution, otherwise as BaseArray.
//
pragma "inline"
def _toFollower(x: _tuple, leaderIndex, type alignment) {
  pragma "inline" def _toFollowerHelp(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 {
      type tdim = alignment(dim);
      type tdimp1 = alignment(dim+1);
      if tdim == Distribution & tdimp1 == Distribution {
        return (_toFollower(x(dim), leaderIndex, true),
                _toFollower(x(dim+1), leaderIndex, true));
      } else if tdim == Distribution {
        return (_toFollower(x(dim), leaderIndex, true),
                _toFollower(x(dim+1), leaderIndex));
      } else if tdimp1 == Distribution {
        return (_toFollower(x(dim), leaderIndex),
                _toFollower(x(dim+1), leaderIndex, true));
      } else {
        return (_toFollower(x(dim), leaderIndex),
                _toFollower(x(dim+1), leaderIndex));
      }
    } else {
      type tdim = alignment(dim);
      if tdim == Distribution {
        return (_toFollower(x(dim), leaderIndex, true),
                (..._toFollowerHelp(x, leaderIndex, dim+1)));
      } else {
        return (_toFollower(x(dim), leaderIndex),
                (..._toFollowerHelp(x, leaderIndex, dim+1)));
      }
    }
  }
  return _toFollowerHelp(x, leaderIndex, 1);
}


// sjd: is this ever called?
pragma "inline"
def _toFollower(x, leaderIndex)
  return _toFollower(x.these(), leaderIndex);
