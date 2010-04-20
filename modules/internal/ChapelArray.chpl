var privatizeLock$: sync int;

pragma "privatized class"
def _isPrivatized(value) param
  return !_local & ((_privatization & value.dsiSupportsPrivatization()) | value.dsiRequiresPrivatization());

def _newPrivatizedClass(value) {
  privatizeLock$.writeEF(true);
  var n = __primitive("chpl_numPrivatizedClasses");
  var hereID = here.uid;
  const privatizeData = value.dsiGetPrivatizeData();
  on Realms(0) do
    _newPrivatizedClassHelp(value, value, n, hereID, privatizeData);

  def _newPrivatizedClassHelp(parentValue, originalValue, n, hereID, privatizeData) {
    var newValue = originalValue;
    if hereID != here.uid {
      newValue = parentValue.dsiPrivatize(privatizeData);
      __primitive("chpl_newPrivatizedClass", newValue);
      newValue.pid = n;
    } else {
      __primitive("chpl_newPrivatizedClass", newValue);
      newValue.pid = n;
    }
    cobegin {
      if chpl_localeTree.left then
        on chpl_localeTree.left do
          _newPrivatizedClassHelp(newValue, originalValue, n, hereID, privatizeData);
      if chpl_localeTree.right then
        on chpl_localeTree.right do
          _newPrivatizedClassHelp(newValue, originalValue, n, hereID, privatizeData);
    }
  }

  privatizeLock$.readFE();
  return n;
}

def _reprivatize(value) {
  var pid = value.pid;
  var hereID = here.uid;
  const reprivatizeData = value.dsiGetReprivatizeData();
  on Realms(0) do
    _reprivatizeHelp(value, value, pid, hereID, reprivatizeData);
  def _reprivatizeHelp(parentValue, originalValue, pid, hereID, reprivatizeData) {
    var newValue = originalValue;
    if hereID != here.uid {
      newValue = __primitive("chpl_getPrivatizedClass", newValue, pid);
      newValue.dsiReprivatize(parentValue, reprivatizeData);
    }
    cobegin {
      if chpl_localeTree.left then
        on chpl_localeTree.left do
          _reprivatizeHelp(newValue, originalValue, pid, hereID, reprivatizeData);
      if chpl_localeTree.right then
        on chpl_localeTree.right do
          _reprivatizeHelp(newValue, originalValue, pid, hereID, reprivatizeData);
    }
  }
}

//
// Take a rank and value and check that the value is a rank-tuple or not a
// tuple. If the value is not a tuple and expand is true, copy the value into
// a rank-tuple. If the value is a scalar and rank is 1, copy it into a 1-tuple.
//
pragma "inline"
def _makeIndexTuple(param rank, t: _tuple, param expand: bool=false) where rank == t.size {
  return t;
}

def _makeIndexTuple(param rank, t: _tuple, param expand: bool=false) where rank != t.size {
  compilerError("index rank must match domain rank");
}

def _makeIndexTuple(param rank, val:integral, param expand: bool=false) {
  if expand || rank == 1 {
    var t: rank*val.type;
    for param i in 1..rank do
      t(i) = val;
    return t;
  } else {
    compilerWarning(typeToString(val.type));
    compilerError("index rank must match domain rank");
    return val;
  }
}

def _newArray(value) {
  if _isPrivatized(value) then
    return new _array(_newPrivatizedClass(value), value);
  else
    return new _array(value, value);
}

def _newDomain(value) {
  if _isPrivatized(value) then
    return new _domain(_newPrivatizedClass(value), value);
  else
    return new _domain(value, value);
}

def _getDomain(value) {
  if _isPrivatized(value) then
    return new _domain(value.pid, value);
  else
    return new _domain(value, value);
}

def _newDistribution(value) {
  if _isPrivatized(value) then
    return new _distribution(_newPrivatizedClass(value), value);
  else
    return new _distribution(value, value);
}

def _getDistribution(value) {
  if _isPrivatized(value) then
    return new _distribution(value.pid, value);
  else
    return new _distribution(value, value);
}


//
// Support for domain types
//
pragma "has runtime type"
def chpl__buildDomainRuntimeType(d: _distribution, param rank: int,
                                 type idxType = int(32),
                                 param stridable: bool = false) type
  return _newDomain(d.newArithmeticDom(rank, idxType, stridable));

pragma "has runtime type"
def chpl__buildDomainRuntimeType(d: _distribution, type idxType) type
  return _newDomain(d.newAssociativeDom(idxType));

pragma "has runtime type"
def chpl__buildDomainRuntimeType(d: _distribution, type idxType) type
 where idxType == _OpaqueIndex
  return _newDomain(d.newOpaqueDom(idxType));

// This function has no 'has runtime type' pragma since the idxType of
// opaque domains is _OpaqueIndex, not opaque.  This function is
// essentially a wrapper around the function that actually builds up
// the runtime type.
def chpl__buildDomainRuntimeType(d: _distribution, type idxType) type
 where idxType == opaque
  return chpl__buildDomainRuntimeType(d, _OpaqueIndex);

pragma "has runtime type"
def chpl__buildSparseDomainRuntimeType(d: _distribution, dom: domain) type
  return _newDomain(d.newSparseDom(dom.rank, dom._value.idxType, dom));

def chpl__convertValueToRuntimeType(dom: domain) type
 where dom._value:BaseArithmeticDom
  return chpl__buildDomainRuntimeType(dom.dist, dom._value.rank,
                            dom._value.idxType, dom._value.stridable);

def chpl__convertValueToRuntimeType(dom: domain) type
 where dom._value:BaseSparseDom
  return chpl__buildSparseDomainRuntimeType(dom.dist, dom._value.parentDom);

def chpl__convertValueToRuntimeType(dom: domain) type
  return chpl__buildDomainRuntimeType(dom.dist, dom._value.idxType);

//
// Support for array types
//
pragma "has runtime type"
def chpl__buildArrayRuntimeType(dom: domain, type eltType) type
  return dom.buildArray(eltType);

def chpl__convertValueToRuntimeType(arr: []) type
  return chpl__buildArrayRuntimeType(arr.domain, arr.eltType);

def chpl__getDomainFromArrayType(type arrayType) {
  var A: arrayType;
  pragma "no copy" var D = A.domain;
  pragma "dont disable remote value forwarding"
  def help() {
    D._value._domCnt$ += 1;
  }
  help();
  return D;
}

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
// Support for distributed domain expression, e.g., [1..3, 1..3] distributed Dist
//
def chpl__distributed(d: _distribution, dom: domain) {
  if isArithmeticDom(dom) {
    var distDom: domain(dom.rank, dom._value.idxType, dom._value.stridable) dmapped d = dom;
    return distDom;
  } else {
    var distDom: domain(dom._value.idxType) dmapped d = dom;
    return distDom;
  }
}

def chpl__distributed(d: _distribution, ranges: range(?) ...?rank) {
  return chpl__distributed(d, chpl__buildDomainExpr((...ranges)));
}

def chpl__isArithmeticDomType(type domainType) param {
  var dom: domainType;
  return isArithmeticDom(dom);
}

def chpl__isSparseDomType(type domainType) param {
  var dom: domainType;
  return isSparseDom(dom);
}

def chpl__distributed(d: _distribution, type domainType) type {
  if chpl__isArithmeticDomType(domainType) {
    var dom: domainType;
    return chpl__buildDomainRuntimeType(d, dom._value.rank, dom._value.idxType,
                                        dom._value.stridable);
  } else if chpl__isSparseDomType(domainType) {
    //
    // this "no auto destroy" pragma is necessary as of 1/20 because
    // otherwise the parentDom gets destroyed in the sparse case; see
    // sparse/bradc/CSR/sparse.chpl as an example
    //
    pragma "no auto destroy" var dom: domainType;
    return chpl__buildSparseDomainRuntimeType(d, dom._value.parentDom);
  } else {
    var dom: domainType;
    return chpl__buildDomainRuntimeType(d, dom._value.idxType);
  }
}

//
// Support for index types
//
def chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
  var x: idxType;
  return x.type;
}

def chpl__buildIndexType(param rank: int, type idxType) type where rank > 1 {
  var x: rank*idxType;
  return x.type;
}

def chpl__buildIndexType(param rank: int) type
  return chpl__buildIndexType(rank, int);

def chpl__buildIndexType(d: domain) type
  return chpl__buildIndexType(d.rank, d._value.idxType);

def chpl__buildIndexType(type idxType) type where idxType == opaque
  return _OpaqueIndex;

def isArithmeticDom(d: domain) param {
  def isArithmeticDomClass(dc: BaseArithmeticDom) param return true;
  def isArithmeticDomClass(dc) param return false;
  return isArithmeticDomClass(d._value);
}

def isArithmeticArr(a: []) param return isArithmeticDom(a.domain);

def isAssociativeDom(d: domain) param {
  def isAssociativeDomClass(dc: BaseAssociativeDom) param return true;
  def isAssociativeDomClass(dc) param return false;
  return isAssociativeDomClass(d._value);
}

def isEnumDom(d: domain) param {
  return isAssociativeDom(d) && __primitive("isEnumType", d._value.idxType);
}

def isEnumArr(a: []) param return isEnumDom(a.domain);

def isOpaqueDom(d: domain) param {
  def isOpaqueDomClass(dc: BaseOpaqueDom) param return true;
  def isOpaqueDomClass(dc) param return false;
  return isOpaqueDomClass(d._value);
}

def isSparseDom(d: domain) param {
  def isSparseDomClass(dc: BaseSparseDom) param return true;
  def isSparseDomClass(dc) param return false;
  return isSparseDomClass(d._value);
}

def isSparseArr(a: []) param return isSparseDom(a.domain);

//
// Support for distributions
//
pragma "syntactic distribution"
record dmap { }

def chpl__buildDistType(type t) type where t: BaseDist {
  var x: t;
  var y = _newDistribution(x);
  return y.type;
}

def chpl__buildDistType(type t) {
  compilerError("illegal distribution type specifier");
}

def chpl__buildDistValue(x) where x: BaseDist {
  return _newDistribution(x);
}

def chpl__buildDistValue(x) {
  compilerError("illegal distribution value specifier");
}

//
// Distribution wrapper record
//
pragma "distribution"
record _distribution {
  var _value;
  var _valueType;

  def _distribution(_value, _valueType) { }

  pragma "inline"
  def _value {
    if _isPrivatized(_valueType) {
      var tc = _valueType;
      var id = _value;
      var pc = __primitive("chpl_getPrivatizedClass", tc, id);
      return pc;
    } else {
      return _value;
    }
  }

  def ~_distribution() {
    if !_isPrivatized(_valueType) {
      on _value {
        var cnt = _value.destroyDist();
        if cnt == 0 {
          _value.dsiDestroyDistClass();
          delete _value;
        }
      }
    }
  }

  def clone() {
    return _newDistribution(_value.dsiClone());
  }

  def newArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    var x = _value.dsiNewArithmeticDom(rank, idxType, stridable);
    if x.linksDistribution() {
      var cnt = _value._distCnt$;
      _value._doms.append(x);
      _value._distCnt$ = cnt + 1;
    }
    return x;
  }

  def newAssociativeDom(type idxType) {
    var x = _value.dsiNewAssociativeDom(idxType);
    if x.linksDistribution() {
      var cnt = _value._distCnt$;
      _value._doms.append(x);
      _value._distCnt$ = cnt + 1;
    }
    return x;
  }

  def newAssociativeDom(type idxType) where __primitive("isEnumType", idxType) {
    var x = _value.dsiNewAssociativeDom(idxType);
    if x.linksDistribution() {
      var cnt = _value._distCnt$;
      _value._doms.append(x);
      _value._distCnt$ = cnt + 1;
    }
    const enumTuple = _enum_enumerate(idxType);
    for param i in 1..enumTuple.size do
      x.dsiAdd(enumTuple(i));
    return x;
  }

  def newOpaqueDom(type idxType) {
    var x = _value.dsiNewOpaqueDom(idxType);
    if x.linksDistribution() {
      var cnt = _value._distCnt$;
      _value._doms.append(x);
      _value._distCnt$ = cnt + 1;
    }
    return x;
  }

  def newSparseDom(param rank: int, type idxType, dom: domain) {
    var x = _value.dsiNewSparseDom(rank, idxType, dom);
    if x.linksDistribution() {
      var cnt = _value._distCnt$;
      _value._doms.append(x);
      _value._distCnt$ = cnt + 1;
    }
    return x;
  }

  def idxToLocale(ind) return _value.dsiIndexLocale(ind);

  def writeThis(x: Writer) {
    _value.writeThis(x);
  }

  def displayRepresentation() { _value.dsiDisplayRepresentation(); }
}


//
// Domain wrapper record
//
pragma "domain"
pragma "has runtime type"
record _domain {
  var _value;     // stores domain class, may be privatized
  var _valueType; // stores type of privatized domains
  var _promotionType: index(rank, _value.idxType);

  pragma "inline"
  def _value {
    if _isPrivatized(_valueType) {
      var tc = _valueType;
      var id = _value;
      var pc = __primitive("chpl_getPrivatizedClass", tc, id);
      return pc;
    } else {
      return _value;
    }
  }

  def ~_domain () {
    if !_isPrivatized(_valueType) {
      on _value {
        var cnt = _value.destroyDom();
        if cnt == 0 then
          delete _value;
      }
    }
  }

  def dist return _getDistribution(_value.dist);

  def rank param {
    if isArithmeticDom(this) || isSparseDom(this) then
      return _value.rank;
    else
      return 1;
  }

  pragma "inline"
  def these() {
    return _value.these();
  }

  def this(ranges: range(?) ...rank) {
    param stridable = _value.stridable || chpl__anyStridable(ranges);
    var r: rank*range(_value.idxType,
                      BoundedRangeType.bounded,
                      stridable);

    for param i in 1..rank {
      r(i) = _value.dsiDim(i)(ranges(i));
    }
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType, stridable, r);
    if d.linksDistribution() then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
  }

  def this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
    var ranges = _getRankChangeRanges(args);
    param newRank = ranges.size, stridable = chpl__anyStridable(ranges);
    var newRanges: newRank*range(eltType=_value.idxType, stridable=stridable);
    var newDistVal = _value.dist.dsiCreateRankChangeDist(newRank, args);
    var newDist = _getNewDist(newDistVal);
    var j = 1;
    var makeEmpty = false;

    for param i in 1..rank {
      if !isCollapsedDimension(args(i)) {
        newRanges(j) = dim(i)(args(i));
        j += 1;
      } else {
        if !dim(i).member(args(i)) then
          makeEmpty = true;
      }
    }
    if makeEmpty {
      for param i in 1..newRank {
        newRanges(i) = 1..0;
      }
    }
    var d = [(...newRanges)] dmapped newDist;
    return d;
  }

  def dims() return _value.dsiDims();

  def dim(d : int) return _value.dsiDim(d);

  pragma "inline"
  def dim(param d : int) return _value.dsiDim(d);

  def dimIter(param d, ind) {
    for i in _value.dimIter(d, ind) do yield i;
  }

  def buildArray(type eltType) {
    var x = _value.dsiBuildArray(eltType);
    pragma "dont disable remote value forwarding"
    def help() {
      var cnt = _value._domCnt$; // lock
      _value._arrs.append(x);
      _value._domCnt$ = cnt + 1; // unlock
    }
    help();
    return _newArray(x);
  }

  def clear() {
    _value.dsiClear();
  }

  def create() {
    if _value.idxType != _OpaqueIndex then
      compilerError("domain.create() only applies to opaque domains");
    return _value.dsiCreate();
  }

  def add(i) {
    _value.dsiAdd(i);
  }

  def remove(i) {
    _value.dsiRemove(i);
  }

  def numIndices return _value.dsiNumIndices;
  def low return _value.dsiLow;
  def high return _value.dsiHigh;

  // SS: adding pragma inline 
  pragma "inline"
  def member(i) {
    if isArithmeticDom(this) then
      return _value.dsiMember(_makeIndexTuple(rank, i));
    else
      return _value.dsiMember(i);
  }

  // 1/5/10: do we want to support order() and position()?
  def indexOrder(i) return _value.dsiIndexOrder(_makeIndexTuple(rank, i));

  def position(i) {
    var ind = _makeIndexTuple(rank, i), pos: rank*_value.idxType;
    for d in 1..rank do
      pos(d) = _value.dsiDim(d).indexOrder(ind(d));
    return pos;
  }

  def expand(off: _value.idxType ...rank) return expand(off);
  def expand(off: rank*_value.idxType) {
    var ranges = dims();
    for i in 1..rank do {
      ranges(i) = ranges(i).expand(off(i));
      if (ranges(i)._low > ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }

    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
  }
  def expand(off: _value.idxType) where rank > 1 {
    var ranges = dims();
    for i in 1..rank do
      ranges(i) = dim(i).expand(off);
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
  }

  def exterior(off: _value.idxType ...rank) return exterior(off);
  def exterior(off: rank*_value.idxType) {
    var ranges = dims();
    for i in 1..rank do
      ranges(i) = dim(i).exterior(off(i));
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
   }
                  
  def interior(off: _value.idxType ...rank) return interior(off);
  def interior(off: rank*_value.idxType) {
    var ranges = dims();
    for i in 1..rank do {
      if ((off(i) > 0) && (dim(i)._high+1-off(i) < dim(i)._low) ||
          (off(i) < 0) && (dim(i)._low-1-off(i) > dim(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      ranges(i) = _value.dsiDim(i).interior(off(i));
    }
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
   }

  def translate(off: _value.idxType ...rank) return translate(off);
  def translate(off: rank*_value.idxType) {
    var ranges = dims();
    for i in 1..rank do
      ranges(i) = _value.dsiDim(i).translate(off(i));
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
   }

  def chpl__unTranslate(off: _value.idxType ...rank) return chpl__unTranslate(off);
  def chpl__unTranslate(off: rank*_value.idxType) {
    var ranges = dims();
    for i in 1..rank do
      ranges(i) = dim(i).chpl__unTranslate(off(i));
    var d = _value.dsiBuildArithmeticDom(rank, _value.idxType,
                                         _value.stridable, ranges);
    if (d.linksDistribution()) then
      d.dist._distCnt$ += 1;
    return _newDomain(d);
   }

  def setIndices(x) {
    _value.dsiSetIndices(x);
    if _isPrivatized(_valueType) {
      _reprivatize(_value);
    }
  }

  def getIndices()
    return _value.dsiGetIndices();

  def writeThis(f: Writer) {
    _value.dsiSerialWrite(f);
  }

  def localSlice(r: range(?)... rank) {
    return _value.localSlice(chpl__anyStridable(r), r);
  }

  // associative array interface

  def sorted() {
    for i in _value.dsiSorted() {
      yield i;
    }
  }

  def displayRepresentation() { _value.dsiDisplayRepresentation(); }
}

def _getNewDist(value) {
  return new dmap(value);
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
  var _value;     // stores array class, may be privatized
  var _valueType; // stores type of privatized arrays
  var _promotionType: _value.eltType;

  pragma "inline"
  def _value {
    if _isPrivatized(_valueType) {
      var tc = _valueType;
      var id = _value;
      var pc = __primitive("chpl_getPrivatizedClass", tc, id);
      return pc;
    } else {
      return _value;
    }
  }

  def ~_array() {
    if !_isPrivatized(_valueType) {
      on _value {
        var cnt = _value.destroyArr();
        if cnt == 0 then
          delete _value;
      }
    }
  }

  def eltType type return _value.eltType;
  def _dom return _getDomain(_value.dom);
  def rank param return this.domain.rank;

  pragma "inline"
  def this(i: rank*_value.idxType) var {
    if isArithmeticArr(this) || isSparseArr(this) then
      return _value.dsiAccess(i);
    else
      return _value.dsiAccess(i(1));
  }

  pragma "inline"
  def this(i: _value.idxType ...rank) var
    return this(i);

  //
  // requires dense domain implementation that returns a tuple of
  // ranges via the getIndices() method; domain indexing is difficult
  // in the domain case because it has to be implemented on a
  // domain-by-domain basis; this is not terribly difficult in the
  // dense case because we can represent a domain by a tuple of
  // ranges, but in the sparse case, is there a general
  // representation?
  //
  def this(d: domain) where d.rank == rank
    return this((...d.getIndices()));

  def checkSlice(ranges: range(?) ...rank) {
    for param i in 1.._value.rank do
      if !_value.dom.dsiDim(i).boundsCheck(ranges(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
  }

  def this(ranges: range(?) ...rank) {
    if boundsChecking then
      checkSlice((... ranges));
    var d = _dom((...ranges));
    var a = _value.dsiSlice(d._value);
    a._arrAlias = _value;
    pragma "dont disable remote value forwarding"
    def help() {
      d._value._domCnt$ += 1;
      a._arrAlias._arrCnt$ += 1;
    }
    help();
    return _newArray(a);
  }

  def this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
    if boundsChecking then
      checkRankChange(args);
    var ranges = _getRankChangeRanges(args);
    param rank = ranges.size, stridable = chpl__anyStridable(ranges);
    var d = _dom((...args));
    d._value._domCnt$ += 1;
    var a = _value.dsiRankChange(d._value, rank, stridable, args);
    a._arrAlias = _value;
    a._arrAlias._arrCnt$ += 1;
    return _newArray(a);
  }

  def checkRankChange(args) {
    for param i in 1..args.size do
      if !_value.dom.dsiDim(i).boundsCheck(args(i)) then
        halt("array slice out of bounds in dimension ", i, ": ", args(i));
  }

  def localSlice(r: range(?)... rank) {
    return _value.localSlice(r);
  }

  pragma "inline"
  def these() var {
    return _value.these();
  }

  // 1/5/10: do we need this since it always returns domain.numIndices?
  def numElements return _value.dom.dsiNumIndices;

  def newAlias() {
    var x = _value.dsiReindex(_value.dom);
    x._arrAlias = _value;
    pragma "dont disable remote value forwarding"
    def help() {
      _value.dom._domCnt$ += 1;
      x._arrAlias._arrCnt$ += 1;
    }
    help();
    return _newArray(x);
  }

  def reindex(d: domain) {
    if rank != d.rank then
      compilerError("illegal implicit rank change");
    for param i in 1..rank do
      if d.dim(i).length != _value.dom.dsiDim(i).length then
        halt("extent in dimension ", i, " does not match actual");

    var newDist = new dmap(_value.dom.dist.dsiCreateReindexDist(d.dims(),
                                                                _value.dom.dsiDims()));
    var newDom = [(...d.dims())] dmapped newDist;
    var x = _value.dsiReindex(newDom._value);
    x._arrAlias = _value;
    pragma "dont disable remote value forwarding"
    def help() {
      newDom._value._domCnt$ += 1;
      x._arrAlias._arrCnt$ += 1;
    }
    help();
    return _newArray(x);
  }

  def writeThis(f: Writer) {
    _value.dsiSerialWrite(f);
  }

  // sparse array interface

  def IRV var {
    return _value.IRV;
  }

  // associative array interface

  def sorted() {
    for i in _value.dsiSorted() {
      yield i;
    }
  }

  def displayRepresentation() { _value.dsiDisplayRepresentation(); }
}

//
// Helper functions
//

def isCollapsedDimension(r: range(?e,?b,?s)) param return false;
def isCollapsedDimension(r) param return true;


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

def chpl__isRange(r: range(?)) param return true;
def chpl__isRange(r) param return false;

def _getRankChangeRanges(args) {
  def _tupleize(x) {
    var y: 1*x.type;
    y(1) = x;
    return y;
  }
  def collectRanges(param dim: int) {
    if dim > args.size then
      compilerError("domain slice requires a range in at least one dimension");
    if chpl__isRange(args(dim)) then
      return collectRanges(dim+1, _tupleize(args(dim)));
    else
      return collectRanges(dim+1);
  }
  def collectRanges(param dim: int, x: _tuple) {
    if dim > args.size {
      return x;
    } else if dim < args.size {
      if chpl__isRange(args(dim)) then
        return collectRanges(dim+1, ((...x), args(dim)));
      else
        return collectRanges(dim+1, x);
    } else {
      if chpl__isRange(args(dim)) then
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

def chpl__isArray(x: []) param return true;
def chpl__isArray(x) param return false;

//
// Assignment of domains and arrays
//
def =(a: _distribution, b: _distribution) {
  if a._value == nil {
    return chpl__autoCopy(b.clone());
  } else if a._value._doms.length == 0 {
    if a._value.type != b._value.type then
      compilerError("type mismatch in distribution assignment");
    a._value.dsiAssign(b._value);
    if _isPrivatized(a._value) then
      _reprivatize(a._value);
  } else {
    halt("assignment to distributions with declared domains is not yet supported");
  }
  return a;
}

def =(a: domain, b: domain) {
  if isArithmeticDom(a) && isArithmeticDom(b) {
    var bc = b;
    for e in a._value._arrs do {
      on e do e.dsiReallocate(bc);
    }
    a.setIndices(bc.getIndices());
  } else {
    //
    // BLC: It's tempting to do a clear + add here, but because
    // we need to preserve array values that are in the intersection
    // between the old and new index sets, we use the following
    // instead.
    //
    // TODO: These should eventually become forall loops, hence the
    // warning
    //
    compilerWarning("whole-domain assignment has been serialized (see note in $CHPL_HOME/STATUS)");
    for i in a._value.dsiIndsIterSafeForRemoving() {
      if !b.member(i) {
        a.remove(i);
      }
    }
    for i in b {
      if !a.member(i) {
        a.add(i);
      }
    }
  }
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

pragma "inline" def =(a: [], b : []) where (a._value.canCopyFromHost && b._value.canCopyFromHost) {
  if a.rank != b.rank then
    compilerError("rank mismatch in array assignment");
  compilerError("GPU to GPU transfers not yet implemented");
}

pragma "inline" def =(a: [], b : []) where (a._value.canCopyFromDevice && b._value.canCopyFromHost) {
  if a.rank != b.rank then
    compilerError("rank mismatch in array assignment");
  __primitive("copy_gpu_to_host", 
              a._value.data, b._value.data, b._value.eltType, b._value.size);
  return a;
}

pragma "inline" def =(a: [], b : []) where (a._value.canCopyFromHost && b._value.canCopyFromDevice) {
  if a.rank != b.rank then
    compilerError("rank mismatch in array assignment");
  __primitive("copy_host_to_gpu", 
              a._value.data, b._value.data, b._value.eltType, b._value.size);
  return a;
}

def chpl__serializeAssignment(a: [], b) param {
  if a.rank != 1 && chpl__isRange(b) then
    return true;

  // Sparse, Associative, Opaque do not yet support parallel iteration.  We
  // could let them fall through, but then we get multiple warnings for a
  // single assignment statement which feels like overkill
  //
  if (!isArithmeticArr(a) || (chpl__isArray(b) && !isArithmeticArr(b))) then
    return true;
  return false;
}

pragma "inline" def =(a: [], b) {
  if (chpl__isArray(b) || chpl__isDomain(b)) && a.rank != b.rank then
    compilerError("rank mismatch in array assignment");
  if chpl__isArray(b) && b._value == nil then
    return a;
  if chpl__serializeAssignment(a, b) {
    compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
    for (aa,bb) in (a,b) do
      aa = bb;
  } else if chpl__tryToken { // try to parallelize using leader and follower iterators
    forall (aa,bb) in (a,b) do
      aa = bb;
  } else {
    compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
    for (aa,bb) in (a,b) do
      aa = bb;
  }
  return a;
}

def =(a: [], b: _tuple) where isEnumArr(a) || isArithmeticArr(a) {
  if isEnumArr(a) {
    if b.size != a.numElements then
      halt("tuple array initializer size mismatch");
    for (i,j) in (chpl_enumerate(index(a.domain)), 1..) {
      a(i) = b(j);
    }
  } else {
    def chpl__tupleInit(j, param rank: int, b: _tuple) {
      if rank == 1 {
        for param i in 1..b.size {
          j(a.rank-rank+1) = a.domain.dim(a.rank-rank+1).low + i - 1;
          a(j) = b(i);
        }
      } else {
        for param i in 1..b.size {
          j(a.rank-rank+1) = a.domain.dim(a.rank-rank+1).low + i - 1;
          chpl__tupleInit(j, rank-1, b(i));
        }
      }
    }

    if a.rank == 1 {
      for param i in 1..b.size do
        a(a.domain.dim(1).low + i - 1) = b(i);
    } else {
      var j: a.rank*int;
      chpl__tupleInit(j, a.rank, b);
    }
  }
  return a;
}

def _desync(type t) where t: _syncvar || t: _singlevar {
  var x: t;
  return x.value;
}

def _desync(type t) {
  var x: t;
  return x;
}

def =(a: [], b: _desync(a.eltType)) {
  if isArithmeticArr(a) {
    forall e in a do
      e = b;
  } else {
    compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
    for e in a do
      e = b;
  }
  return a;
}

def by(a: domain, b) {
  var r: a.rank*range(a._value.idxType,
                    BoundedRangeType.bounded,
                    true);
  var t = _makeIndexTuple(a.rank, b, expand=true);
  for param i in 1..a.rank do
    r(i) = a.dim(i) by t(i);
  var d = a._value.dsiBuildArithmeticDom(a.rank, a._value.idxType, true, r);
  if (d.linksDistribution()) then
    d.dist._distCnt$ += 1;
  return _newDomain(d);
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

def linearize(Xs) {
  for x in Xs do yield x;
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
    if dim == t.size then
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)));
    else
      return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                           (...iteratorIndexHelp(t, dim+1)));
  }

  return iteratorIndexHelp(t, 1);
}

def iteratorIndexType(x) type {
  pragma "no copy" var ic = _getIterator(x);
  pragma "no copy" var i = iteratorIndex(ic);
  _freeIterator(ic);
  return i.type;
}

def _iteratorRecord.writeThis(f: Writer) {
  var first: bool = true;
  for e in this {
    if !first then
      f.write(" ");
    else
      first = false;
    f.write(e);
  }
}

def =(ic: _iteratorRecord, xs) {
  for (e, x) in (ic, xs) do
    e = x;
  return ic;
}

def =(ic: _iteratorRecord, x: iteratorIndexType(ic)) {
  for e in ic do
    e = x;
  return ic;
}

pragma "inline" def _getIterator(x) {
  return _getIterator(x.these());
}

pragma "inline" def _getIterator(ic: _iteratorClass)
  return ic;

pragma "inline" def _getIterator(x: _tuple) {
  pragma "inline" def _getIteratorHelp(x: _tuple, param dim: int) {
    if dim == x.size then
      return tuple(_getIterator(x(dim)));
    else
      return (_getIterator(x(dim)), (..._getIteratorHelp(x, dim+1)));
  }
  if x.size == 1 then
    return _getIterator(x(1));
  else
    return _getIteratorHelp(x, 1);
}

def _getIterator(type t) {
  compilerError("cannot iterate over a type");
}

def _checkIterator(type t) {
  compilerError("cannot iterate over a type");
}

pragma "inline" def _checkIterator(x) {
  return x;
}

pragma "inline"
def _freeIterator(ic: _iteratorClass) {
  __primitive("chpl_free", ic);
}

pragma "inline"
def _freeIterator(x: _tuple) {
  for param i in 1..x.size do
    _freeIterator(x(i));
}

pragma "inline"
pragma "no implicit copy"
def _toLeader(iterator: _iteratorClass)
  return chpl__autoCopy(__primitive("to leader", iterator));

pragma "inline"
def _toLeader(ir: _iteratorRecord) {
  pragma "no copy" var ic = _getIterator(ir);
  pragma "no copy" var leader = _toLeader(ic);
  _freeIterator(ic);
  return leader;
}

pragma "inline"
def _toLeader(x: _tuple)
  return _toLeader(x(1));

pragma "inline"
def _toLeader(x)
  return _toLeader(x.these());

//
// returns lead entity
//
def chpl__lead(x: _tuple) return chpl__lead(x(1));
def chpl__lead(x) return x;

//
// return true if any iterator supports fast followers
//
def chpl__staticFastFollowCheck(x) param {
  pragma "no copy" const lead = chpl__lead(x);
  if chpl__isDomain(lead) || chpl__isArray(lead) then
    return chpl__staticFastFollowCheck(x, lead);
  else
    return false;
}  

def chpl__staticFastFollowCheck(x: _tuple, lead, param dim = 1) param {
  if x.size == dim then
    return chpl__staticFastFollowCheck(x(dim), lead);
  else
    return chpl__staticFastFollowCheck(x(dim), lead) || chpl__staticFastFollowCheck(x, lead, dim+1);
}

def chpl__staticFastFollowCheck(x, lead) param {
  return false;
}

def chpl__staticFastFollowCheck(x: [], lead) param {
  return x._value.dsiStaticFastFollowCheck(lead._value.type);
}

//
// return true if all iterators that support fast followers can use
// their fast followers
//
def chpl__dynamicFastFollowCheck(x) {
  return chpl__dynamicFastFollowCheck(x, chpl__lead(x));
}

def chpl__dynamicFastFollowCheck(x: _tuple, lead, param dim = 1) {
  if x.size == dim then
    return chpl__dynamicFastFollowCheck(x(dim), lead);
  else
    return chpl__dynamicFastFollowCheck(x(dim), lead) && chpl__dynamicFastFollowCheck(x, lead, dim+1);
}

def chpl__dynamicFastFollowCheck(x, lead) {
  return true;
}

def chpl__dynamicFastFollowCheck(x: [], lead) {
  if chpl__staticFastFollowCheck(x, lead) then
    return x._value.dsiDynamicFastFollowCheck(lead);
  else
    return false;
}

pragma "inline"
pragma "no implicit copy"
def _toFollower(iterator: _iteratorClass, leaderIndex)
  return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex));

pragma "inline"
def _toFollower(ir: _iteratorRecord, leaderIndex) {
  pragma "no copy" var ic = _getIterator(ir);
  pragma "no copy" var follower = _toFollower(ic, leaderIndex);
  _freeIterator(ic);
  return follower;
}

pragma "inline"
def _toFollower(x, leaderIndex) {
  return _toFollower(x.these(), leaderIndex);
}

pragma "inline"
def _toFollowerHelp(x: _tuple, leaderIndex, param dim: int) {
  if dim == x.size-1 then
    return (_toFollower(x(dim), leaderIndex),
            _toFollower(x(dim+1), leaderIndex));
  else
    return (_toFollower(x(dim), leaderIndex),
            (..._toFollowerHelp(x, leaderIndex, dim+1)));
}

pragma "inline"
def _toFollower(x: _tuple, leaderIndex) {
  return _toFollowerHelp(x, leaderIndex, 1);
}

pragma "inline"
pragma "no implicit copy"
def _toFastFollower(iterator: _iteratorClass, leaderIndex, fast: bool) {
  return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex, true));
}

pragma "inline"
def _toFastFollower(ir: _iteratorRecord, leaderIndex, fast: bool) {
  pragma "no copy" var ic = _getIterator(ir);
  pragma "no copy" var follower = _toFastFollower(ic, leaderIndex, fast=true);
  _freeIterator(ic);
  return follower;
}

pragma "inline"
pragma "no implicit copy"
def _toFastFollower(iterator: _iteratorClass, leaderIndex) {
  return _toFollower(iterator, leaderIndex);
}

pragma "inline"
def _toFastFollower(ir: _iteratorRecord, leaderIndex) {
  return _toFollower(ir, leaderIndex);
}

pragma "inline"
def _toFastFollower(x, leaderIndex) {
  if chpl__staticFastFollowCheck(x) then
    return _toFastFollower(x.these(), leaderIndex, fast=true);
  else
    return _toFollower(x.these(), leaderIndex);
}

pragma "inline"
def _toFastFollowerHelp(x: _tuple, leaderIndex, param dim: int) {
  if dim == x.size-1 then
    return (_toFastFollower(x(dim), leaderIndex),
            _toFastFollower(x(dim+1), leaderIndex));
  else
    return (_toFastFollower(x(dim), leaderIndex),
            (..._toFastFollowerHelp(x, leaderIndex, dim+1)));
}

pragma "inline"
def _toFastFollower(x: _tuple, leaderIndex) {
  return _toFastFollowerHelp(x, leaderIndex, 1);
}

def chpl__initCopy(a: _distribution) {
  pragma "no copy" var b = chpl__autoCopy(a.clone());
  return b;
}

def chpl__initCopy(a: domain) {
  var b: a.type;
  if isArithmeticDom(a) && isArithmeticDom(b) {
    b.setIndices(a.getIndices());
  } else {
    // TODO: These should eventually become forall loops, hence the
    // warning
    //
    compilerWarning("whole-domain assignment has been serialized (see note in $CHPL_HOME/STATUS)");
    for i in a do
      b.add(i);
  }
  return b;
}

def chpl__initCopy(a: []) {
  var b : [a._dom] a.eltType;
  b = a;
  return b;
}

def chpl__initCopy(ir: _iteratorRecord) {
  def _ir_copy_recursive(ir) {
    for e in ir do
      yield chpl__initCopy(e);
  }

  pragma "no copy" var irc = _ir_copy_recursive(ir);

  var i = 1, size = 4;
  pragma "insert auto destroy" var D = [1..size];

  // note that _getIterator is called in order to copy the iterator
  // class since for arrays we need to iterate once to get the
  // element type (at least for now); this also means that if this
  // iterator has side effects, we will see them; a better way to
  // handle this may be to get the static type (not initialize the
  // array) and use a primitive to set the array's element; that may
  // also handle skyline arrays
  var A: [D] iteratorIndexType(irc);

  for e in irc {
    pragma "no copy" pragma "insert auto destroy" var ee = e;
    if i > size {
      size = size * 2;
      D = [1..size];
    }
    A(i) = ee;
    i = i + 1;
  }
  D = [1..i-1];
  return A;
}
