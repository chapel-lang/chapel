// ChapelArray.chpl
//
pragma "no use ChapelStandard"
module ChapelArray {
  
  use ChapelBase; // For opaque type.
  use ChapelTuple;
  use ChapelLocale;

  var privatizeLock$: sync int;
  
  config param debugBulkTransfer = false;
  config param useBulkTransfer = true;
  config param useBulkTransferStride = false;
  
  pragma "privatized class"
  proc _isPrivatized(value) param
    return !_local & ((_privatization & value.dsiSupportsPrivatization()) | value.dsiRequiresPrivatization());
  
  proc _newPrivatizedClass(value) {
    privatizeLock$.writeEF(true);
    var n = __primitive("chpl_numPrivatizedClasses");
    var hereID = here.id;
    const privatizeData = value.dsiGetPrivatizeData();
    on Locales[0] do
      _newPrivatizedClassHelp(value, value, n, hereID, privatizeData);
  
    proc _newPrivatizedClassHelp(parentValue, originalValue, n, hereID, privatizeData) {
      var newValue = originalValue;
      if hereID != here.id {
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
  
  proc _reprivatize(value) {
    var pid = value.pid;
    var hereID = here.id;
    const reprivatizeData = value.dsiGetReprivatizeData();
    on Locales[0] do
      _reprivatizeHelp(value, value, pid, hereID, reprivatizeData);
  
    proc _reprivatizeHelp(parentValue, originalValue, pid, hereID, reprivatizeData) {
      var newValue = originalValue;
      if hereID != here.id {
        newValue = chpl_getPrivatizedCopy(newValue.type, pid);
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
  proc _makeIndexTuple(param rank, t: _tuple, param expand: bool=false) where rank == t.size {
    return t;
  }
  
  proc _makeIndexTuple(param rank, t: _tuple, param expand: bool=false) where rank != t.size {
    compilerError("index rank must match domain rank");
  }
  
  proc _makeIndexTuple(param rank, val:integral, param expand: bool=false) {
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
  
  proc _newArray(value) {
    if _isPrivatized(value) then
      return new _array(_newPrivatizedClass(value), value);
    else
      return new _array(value, value);
  }
  
  proc _newDomain(value) {
    if _isPrivatized(value) then
      return new _domain(_newPrivatizedClass(value), value);
    else
      return new _domain(value, value);
  }
  
  proc _getDomain(value) {
    if _isPrivatized(value) then
      return new _domain(value.pid, value);
    else
      return new _domain(value, value);
  }
  
  proc _newDistribution(value) {
    if _isPrivatized(value) then
      return new _distribution(_newPrivatizedClass(value), value);
    else
      return new _distribution(value, value);
  }
  
  proc _getDistribution(value) {
    if _isPrivatized(value) then
      return new _distribution(value.pid, value);
    else
      return new _distribution(value, value);
  }
  
  
  //
  // Support for domain types
  //
  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(d: _distribution, param rank: int,
                                   type idxType = int,
                                   param stridable: bool = false)
    return _newDomain(d.newRectangularDom(rank, idxType, stridable));
  
  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(d: _distribution, type idxType,
                                    param parSafe: bool = true)
    return _newDomain(d.newAssociativeDom(idxType, parSafe));
  
  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(d: _distribution, type idxType,
                                    param parSafe: bool = true)
   where idxType == _OpaqueIndex
    return _newDomain(d.newOpaqueDom(idxType, parSafe));
  
  // This function has no 'runtime type init fn' pragma since the idxType of
  // opaque domains is _OpaqueIndex, not opaque.  This function is
  // essentially a wrapper around the function that actually builds up
  // the runtime type.
  proc chpl__buildDomainRuntimeType(d: _distribution, type idxType) type
   where idxType == opaque
    return chpl__buildDomainRuntimeType(d, _OpaqueIndex);

  pragma "runtime type init fn"
  proc chpl__buildSparseDomainRuntimeType(d: _distribution, dom: domain) 
    return _newDomain(d.newSparseDom(dom.rank, dom._value.idxType, dom));
  
  proc chpl__convertValueToRuntimeType(dom: domain) type
   where dom._value:BaseRectangularDom
    return chpl__buildDomainRuntimeType(dom.dist, dom._value.rank,
                              dom._value.idxType, dom._value.stridable);
  
  proc chpl__convertValueToRuntimeType(dom: domain) type
   where dom._value:BaseAssociativeDom
    return chpl__buildDomainRuntimeType(dom.dist, dom._value.idxType, dom._value.parSafe);
  
  proc chpl__convertValueToRuntimeType(dom: domain) type
   where dom._value:BaseOpaqueDom
    return chpl__buildDomainRuntimeType(dom.dist, dom._value.idxType);
  
  proc chpl__convertValueToRuntimeType(dom: domain) type
   where dom._value:BaseSparseDom
    return chpl__buildSparseDomainRuntimeType(dom.dist, dom._value.parentDom);
  
  proc chpl__convertValueToRuntimeType(dom: domain) type {
    compilerError("the global domain class of each domain map implementation must be a subclass of BaseRectangularDom, BaseAssociativeDom, BaseOpaqueDom, or BaseSparseDom", 0);
    return 0; // dummy
  }
  
  //
  // Support for array types
  //
  pragma "runtime type init fn"
  proc chpl__buildArrayRuntimeType(dom: domain, type eltType)
    return dom.buildArray(eltType);

  proc _getLiteralType(type t) type {
    if t != c_string then return t;
    else return string;
  }
  /*
   * Support for array literal expressions.
   *
   * Array literals are detected during parsing and converted 
   * to a call expr.  Array values pass through the various  
   * compilation phases as regular parameters. 
   *
   * NOTE:  It would be nice to define a second, less specific, function
   *        to handle the case of multiple types, however this is not 
   *        possible atm due to using var args with a query type. */
  config param CHPL_WARN_DOMAIN_LITERAL = "unset";
  proc chpl__buildArrayExpr( elems:?t ...?k ) {

    if CHPL_WARN_DOMAIN_LITERAL == "true" && chpl__isRange(elems(1)) {
      compilerWarning("Encountered an array literal with range element(s).",
                      " Did you mean a domain literal here?",
                      " If so, use {...} instead of [...]."); 
    }

    // elements of string literals are assumed to be of type string
    type elemType = _getLiteralType(elems(1).type);
    var A : [1..k] elemType;  //This is unfortunate, can't use t here...
  
    for param i in 1..k {
      type currType = _getLiteralType(elems(i).type);

      if currType != elemType {
        compilerError( "Array literal element " + i:c_string +
                       " expected to be of type " + typeToString(elemType) +
                       " but is of type " + typeToString(currType) );
      }

      A(i) = elems(i);
    }

    return A;
  }

  proc chpl__buildAssociativeArrayExpr( elems ...?k ) {
    type keyType = _getLiteralType(elems(1).type);
    type valType = _getLiteralType(elems(2).type);
    var D : domain(keyType);

    //Size the domain appropriately for the number of keys
    //This prevents expensive resizing as keys are added.
    D.requestCapacity(k/2);
    var A : [D] valType;

    for param i in 1..k by 2 {
      var elemKey = elems(i);
      var elemVal = elems(i+1);
      type elemKeyType = _getLiteralType(elemKey.type);
      type elemValType = _getLiteralType(elemVal.type);

      if elemKeyType != keyType {
         compilerError("Associative array key element " + (i+2)/2 + 
                       " expected to be of type " + typeToString(keyType) + 
                       " but is of type " + typeToString(elemKeyType));
      }

      if elemValType != valType {
        compilerError("Associative array value element " + (i+1)/2
                      + " expected to be of type " + typeToString(valType)
                      + " but is of type " + typeToString(elemValType));
      }

      D += elemKey;
      A[elemKey] = elemVal;
    }

    return A;
  }


  proc chpl__convertValueToRuntimeType(arr: []) type
    return chpl__buildArrayRuntimeType(arr.domain, arr.eltType);
  
  proc chpl__getDomainFromArrayType(type arrayType) {
    var A: arrayType;
    pragma "no copy" var D = A.domain;
    pragma "dont disable remote value forwarding"
    proc help() {
      D._value.incRefCount();
    }
    if !noRefCount then
      help();
    return D;
  }


  //
  // These routines increment and decrement the reference count
  // for a domain that is part of an array's element type.
  // Prior to introducing these routines and calls, we would
  // increment/decrement the reference count based on the
  // number of indices in the outer domain instead; this could
  // cause the domain to be deallocated prematurely in the
  // case the the outer domain was empty.  For example:
  //
  //   var D = {1..0};   // start empty; we'll resize later
  //   var A: [D] [1..2] real;
  //
  // The anonymous domain {1..2} must be kept alive as a result
  // of being part of A's type even though D is initially empty.
  // Thus, {1..2} should remain alive as long as A is.  By
  // incrementing and decrementing its reference counts based
  // on A's lifetime rather than the number of elements in domain
  // D, we ensure that is kept alive.  See
  // test/users/bugzilla/bug794133/ for more details and examples.
  //
  proc chpl_incRefCountsForDomainsInArrayEltTypes(type eltType) {
    compilerAssert(!noRefCount);
    if (isArrayType(eltType)) {
      var ev: eltType;
      ev.domain._value.incRefCount();
      //
      // In addition to incrementing the domain's reference, count, we also
      // have to increment the distribution's.  The primary motivation for
      // this at present is:
      //
      //   test/arrays/deitz/part4/test_array_of_associative_arrays.chpl
      //
      // and we suspect that once the reference counting code is cleaned up,
      // this can be too.  See this comment's commit message for more
      // details.
      //
      ev.domain.dist._value.incRefCount();
      chpl_incRefCountsForDomainsInArrayEltTypes(ev.eltType);
    }
  }

  proc chpl_decRefCountsForDomainsInArrayEltTypes(type eltType) {
    compilerAssert(!noRefCount);
    if (isArrayType(eltType)) {
      var ev: eltType;
      const refcount = ev.domain._value.destroyDom();
      if !noRefCount then
        if refcount == 0 then
          delete ev.domain._value;
      chpl_decRefCountsForDomainsInArrayEltTypes(ev.eltType);
    }
  }
  
  //
  // Support for subdomain types
  //
  // Note the domain of a subdomain is not yet part of the runtime type
  //
  proc chpl__buildSubDomainType(dom: domain) type
    return chpl__convertValueToRuntimeType(dom);
  
  //
  // Support for domain expressions, e.g., {1..3, 1..3}
  //

  proc chpl__buildDomainExpr(ranges: range(?) ...?rank) {
    for param i in 2..rank do
      if ranges(1).idxType != ranges(i).idxType then
        compilerError("idxType varies among domain's dimensions");
    for param i in 1..rank do
      if ! isBoundedRange(ranges(i)) then
        compilerError("one of domain's dimensions is not a bounded range");
    var d: domain(rank, ranges(1).idxType, chpl__anyStridable(ranges));
    d.setIndices(ranges);
    return d;
  }
  
  proc chpl__buildDomainExpr(keys: ?t ...?count) {
    // keyType of string literals is assumed to be type string
    type keyType = _getLiteralType(keys(1).type);
    for param i in 2..count do
      if keyType != _getLiteralType(keys(i).type) {
        compilerError("Associative domain element " + i + 
                      " expected to be of type " + typeToString(keyType) + 
                      " but is of type " +
                      typeToString(_getLiteralType(keys(i).type)));
      }

    //Initialize the domain with a size appropriate for the number of keys.
    //This prevents resizing as keys are added.
    var D : domain(keyType);
    D.requestCapacity(count);

    for param i in 1..count do
      D += keys(i);

    return D;
  }

  //
  // Support for domain expressions within array types, e.g. [1..n], [D]
  //
  proc chpl__ensureDomainExpr(x: domain) {
    return x;
  }

  proc chpl__ensureDomainExpr(x...) {
    return chpl__buildDomainExpr((...x));
  }
  
  //
  // Support for distributed domain expression e.g. {1..3, 1..3} dmapped Dist()
  //
  proc chpl__distributed(d: _distribution, dom: domain) {
    if isRectangularDom(dom) {
      var distDom: domain(dom.rank, dom._value.idxType, dom._value.stridable) dmapped d = dom;
      return distDom;
    } else {
      var distDom: domain(dom._value.idxType) dmapped d = dom;
      return distDom;
    }
  }
  
  proc chpl__distributed(d: _distribution, ranges: range(?) ...?rank) {
    return chpl__distributed(d, chpl__buildDomainExpr((...ranges)));
  }
  
  proc chpl__isRectangularDomType(type domainType) param {
    var dom: domainType;
    return isRectangularDom(dom);
  }
  
  proc chpl__isSparseDomType(type domainType) param {
    var dom: domainType;
    return isSparseDom(dom);
  }
  
  proc chpl__distributed(d: _distribution, type domainType) type {
    if !isDomainType(domainType) then
      compilerError("cannot apply 'dmapped' to the non-domain type ",
                    typeToString(domainType));
    if chpl__isRectangularDomType(domainType) {
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
      return chpl__buildDomainRuntimeType(d, dom._value.idxType, dom._value.parSafe);
    }
  }
  
  //
  // Support for index types
  //
  proc chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
    var x: idxType;
    return x.type;
  }
  
  proc chpl__buildIndexType(param rank: int, type idxType) type where rank > 1 {
    var x: rank*idxType;
    return x.type;
  }
  
  proc chpl__buildIndexType(param rank: int) type
    return chpl__buildIndexType(rank, int);
  
  proc chpl__buildIndexType(d: domain) type
    return chpl__buildIndexType(d.rank, d._value.idxType);
  
  proc chpl__buildIndexType(type idxType) type where idxType == opaque
    return _OpaqueIndex;
  
  proc isRectangularDom(d: domain) param {
    proc isRectangularDomClass(dc: BaseRectangularDom) param return true;
    proc isRectangularDomClass(dc) param return false;
    return isRectangularDomClass(d._value);
  }
  
  proc isRectangularArr(a: []) param return isRectangularDom(a.domain);
  
  proc isIrregularDom(d: domain) param {
    return isSparseDom(d) || isAssociativeDom(d) || isOpaqueDom(d);
  }
  
  proc isIrregularArr(a: []) param return isIrregularDom(a.domain);
  
  proc isAssociativeDom(d: domain) param {
    proc isAssociativeDomClass(dc: BaseAssociativeDom) param return true;
    proc isAssociativeDomClass(dc) param return false;
    return isAssociativeDomClass(d._value);
  }
  
  proc isAssociativeArr(a: []) param return isAssociativeDom(a.domain);
  
  proc isEnumDom(d: domain) param {
    return isAssociativeDom(d) && _isEnumeratedType(d._value.idxType);
  }
  
  proc isEnumArr(a: []) param return isEnumDom(a.domain);
  
  proc isOpaqueDom(d: domain) param {
    proc isOpaqueDomClass(dc: BaseOpaqueDom) param return true;
    proc isOpaqueDomClass(dc) param return false;
    return isOpaqueDomClass(d._value);
  }
  
  proc isSparseDom(d: domain) param {
    proc isSparseDomClass(dc: BaseSparseDom) param return true;
    proc isSparseDomClass(dc) param return false;
    return isSparseDomClass(d._value);
  }
  
  proc isSparseArr(a: []) param return isSparseDom(a.domain);
  
  //
  // Support for distributions
  //
  pragma "syntactic distribution"
  record dmap { }
  
  proc chpl__buildDistType(type t) type where t: BaseDist {
    var x: t;
    var y = _newDistribution(x);
    return y.type;
  }
  
  proc chpl__buildDistType(type t) {
    compilerError("illegal domain map type specifier - must be a subclass of BaseDist");
  }
  
  proc chpl__buildDistValue(x) where x: BaseDist {
    return _newDistribution(x);
  }
  
  proc chpl__buildDistValue(x) {
    compilerError("illegal domain map value specifier - must be a subclass of BaseDist");
  }
  
  //
  // Distribution wrapper record
  //
  pragma "distribution"
  record _distribution {
    var _value;
    var _valueType;
  
    proc _distribution(_value, _valueType) { }
  
    inline proc _value {
      if _isPrivatized(_valueType) {
        return chpl_getPrivatizedCopy(_valueType.type, _value);
      } else {
        return _value;
      }
    }
  
    proc ~_distribution() {
     if !noRefCount {
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
    }
  
    proc clone() {
      return _newDistribution(_value.dsiClone());
    }
  
    proc newRectangularDom(param rank: int, type idxType, param stridable: bool) {
      var x = _value.dsiNewRectangularDom(rank, idxType, stridable);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      return x;
    }
  
    proc newAssociativeDom(type idxType, param parSafe: bool=true) {
      var x = _value.dsiNewAssociativeDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      return x;
    }
  
    proc newAssociativeDom(type idxType, param parSafe: bool=true)
    where _isEnumeratedType(idxType) {
      var x = _value.dsiNewAssociativeDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      const enumTuple = _enum_enumerate(idxType);
      for param i in 1..enumTuple.size do
        x.dsiAdd(enumTuple(i));
      return x;
    }
  
    proc newOpaqueDom(type idxType, param parSafe: bool=true) {
      var x = _value.dsiNewOpaqueDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      return x;
    }
  
    proc newSparseDom(param rank: int, type idxType, dom: domain) {
      var x = _value.dsiNewSparseDom(rank, idxType, dom);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      return x;
    }
  
    proc idxToLocale(ind) return _value.dsiIndexToLocale(ind);
  
    proc readWriteThis(f) {
      f <~> _value;
    }
  
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }
  }  // record _distribution
  
  
  //
  // Domain wrapper record
  //
  pragma "domain"
  pragma "has runtime type"
  record _domain {
    var _value;     // stores domain class, may be privatized
    var _valueType; // stores type of privatized domains
    var _promotionType: index(rank, _value.idxType);
  
    inline proc _value {
      if _isPrivatized(_valueType) {
        return chpl_getPrivatizedCopy(_valueType.type, _value);
      } else {
        return _value;
      }
    }
  
    proc ~_domain () {
     if !noRefCount {
      if !_isPrivatized(_valueType) {
        on _value {
          var cnt = _value.destroyDom();
          if cnt == 0 then
            delete _value;
        }
      }
     }
    }
  
    proc dist return _getDistribution(_value.dist);
  
    proc rank param {
      if isRectangularDom(this) || isSparseDom(this) then
        return _value.rank;
      else
        return 1;
    }
  
    proc idxType type {
      if isOpaqueDom(this) then
        compilerError("opaque domains do not currently support .idxType");
      return _value.idxType;
    }
  
    proc stridable param where isRectangularDom(this) {
      return _value.stridable;
    }
  
    proc stridable param where isSparseDom(this) {
      compilerError("sparse domains do not currently support .stridable");
    }
  
    proc stridable param where isOpaqueDom(this) {
      compilerError("opaque domains do not support .stridable");  
    }
  
    proc stridable param where isEnumDom(this) {
      compilerError("enumerated domains do not support .stridable");  
    }
  
    proc stridable param where isAssociativeDom(this) {
      compilerError("associative domains do not support .stridable");  
    }
  
    inline proc these() {
      return _value.these();
    }
  
    // see comments for the same method in _array
    //
    proc this(d: domain) {
      if d.rank == rank then
        return this((...d.getIndices()));
      else
        compilerError("slicing a domain with a domain of a different rank");
    }
  
    proc this(ranges: range(?) ...rank) {
      param stridable = _value.stridable || chpl__anyStridable(ranges);
      var r: rank*range(_value.idxType,
                        BoundedRangeType.bounded,
                        stridable);
  
      for param i in 1..rank {
        r(i) = _value.dsiDim(i)(ranges(i));
      }
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType, stridable, r);
      if !noRefCount then
        if d.linksDistribution() then
          d.dist.incRefCount();
      return _newDomain(d);
    }
  
    proc this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
      var ranges = _getRankChangeRanges(args);
      param newRank = ranges.size, stridable = chpl__anyStridable(ranges);
      var newRanges: newRank*range(idxType=_value.idxType, stridable=stridable);
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
      var d = {(...newRanges)} dmapped newDist;
      return d;
    }
  
    // anything that is not covered by the above
    proc this(args ...?numArgs) {
      if numArgs == rank then
        compilerError("invalid argument types for domain slicing");
      else
        compilerError("a domain slice requires either a single domain argument or exactly one argument per domain dimension");
    }
  
    proc dims() return _value.dsiDims();
  
    proc dim(d : int) return _value.dsiDim(d);
  
    proc dim(param d : int) return _value.dsiDim(d);
  
    iter dimIter(param d, ind) {
      for i in _value.dimIter(d, ind) do yield i;
    }
  
    proc buildArray(type eltType) {
      var x = _value.dsiBuildArray(eltType);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
        if !noRefCount then
          _value.incRefCount();
      }
      help();
      return _newArray(x);
    }
  
    proc clear() {
      _value.dsiClear();
    }
  
    proc create() {
      if _value.idxType != _OpaqueIndex then
        compilerError("domain.create() only applies to opaque domains");
      return _value.dsiCreate();
    }
  
    proc add(i) {
      _value.dsiAdd(i);
    }
  
    proc remove(i) {
      _value.dsiRemove(i);
    }
  
    proc requestCapacity(i) {

      if i < 0 {
        halt("domain.requestCapacity can only be invoked on sizes >= 0");
      }

      if !isAssociativeDom(this) then
        compilerError("domain.requestCapacity only applies to associative domains");

      _value.dsiRequestCapacity(i);
    }

    proc size return numIndices;
    proc numIndices return _value.dsiNumIndices;
    proc low return _value.dsiLow;
    proc high return _value.dsiHigh;
    proc stride return _value.dsiStride;
    proc alignment return _value.dsiAlignment;
    proc first return _value.dsiFirst;
    proc last return _value.dsiLast;
    proc alignedLow return _value.dsiAlignedLow;
    proc alignedHigh return _value.dsiAlignedHigh;
  
    proc member(i: rank*_value.idxType) {
      if isRectangularDom(this) || isSparseDom(this) then
        return _value.dsiMember(_makeIndexTuple(rank, i));
      else
        return _value.dsiMember(i(1));
    }

    proc member(i: _value.idxType ...rank) {
      return member(i);
    }

    // 1/5/10: do we want to support order() and position()?
    proc indexOrder(i) return _value.dsiIndexOrder(_makeIndexTuple(rank, i));
  
    proc position(i) {
      var ind = _makeIndexTuple(rank, i), pos: rank*_value.idxType;
      for d in 1..rank do
        pos(d) = _value.dsiDim(d).indexOrder(ind(d));
      return pos;
    }
  
    proc expand(off: rank*_value.idxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("expand not supported on associative domains");
      else if isOpaqueDom(this) then
        compilerError("expand not supported on opaque domains");
      else if isSparseDom(this) then
        compilerError("expand not supported on sparse domains");
      else
        compilerError("expand not supported on this domain type");
    }
    proc expand(off: _value.idxType ...rank) return expand(off);
    proc expand(off: rank*_value.idxType) {
      var ranges = dims();
      for i in 1..rank do {
        ranges(i) = ranges(i).expand(off(i));
        if (ranges(i).low > ranges(i).high) {
          halt("***Error: Degenerate dimension created in dimension ", i, "***");
        }
      }
  
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
    }
    proc expand(off: _value.idxType) where rank > 1 {
      var ranges = dims();
      for i in 1..rank do
        ranges(i) = dim(i).expand(off);
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
    }
  
    proc exterior(off: rank*_value.idxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("exterior not supported on associative domains");
      else if isOpaqueDom(this) then
        compilerError("exterior not supported on opaque domains");
      else if isSparseDom(this) then
        compilerError("exterior not supported on sparse domains");
      else
        compilerError("exterior not supported on this domain type");
    }
    proc exterior(off: _value.idxType ...rank) return exterior(off);
    proc exterior(off: rank*_value.idxType) {
      var ranges = dims();
      for i in 1..rank do
        ranges(i) = dim(i).exterior(off(i));
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
     }
                    
    proc interior(off: rank*_value.idxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("interior not supported on associative domains");
      else if isOpaqueDom(this) then
        compilerError("interior not supported on opaque domains");
      else if isSparseDom(this) then
        compilerError("interior not supported on sparse domains");
      else
        compilerError("interior not supported on this domain type");
    }
    proc interior(off: _value.idxType ...rank) return interior(off);
    proc interior(off: rank*_value.idxType) {
      var ranges = dims();
      for i in 1..rank do {
        if ((off(i) > 0) && (dim(i).high+1-off(i) < dim(i).low) ||
            (off(i) < 0) && (dim(i).low-1-off(i) > dim(i).high)) {
          halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
        } 
        ranges(i) = _value.dsiDim(i).interior(off(i));
      }
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
    }
  
    //
    // NOTE: We eventually want to support translate on other domain types
    //
    proc translate(off) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("translate not supported on associative domains");
      else if isOpaqueDom(this) then
        compilerError("translate not supported on opaque domains");
      else if isSparseDom(this) then
        compilerError("translate not supported on sparse domains");
      else
        compilerError("translate not supported on this domain type");
    }
    //
    // Notice that the type of the offset does not have to match the
    // index type.  This is handled in the range.translate().
    //
    proc translate(off: ?t ...rank) return translate(off);
    proc translate(off) where isTuple(off) {
      if off.size != rank then
        compilerError("the domain and offset arguments of translate() must be of the same rank");
      var ranges = dims();
      for i in 1..rank do
        ranges(i) = _value.dsiDim(i).translate(off(i));
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
     }
  
    //
    // intended for internal use only:
    //
    proc chpl__unTranslate(off: _value.idxType ...rank) return chpl__unTranslate(off);
    proc chpl__unTranslate(off: rank*_value.idxType) {
      var ranges = dims();
      for i in 1..rank do
        ranges(i) = dim(i).chpl__unTranslate(off(i));
      var d = _value.dsiBuildRectangularDom(rank, _value.idxType,
                                           _value.stridable, ranges);
      if !noRefCount then
        if (d.linksDistribution()) then
          d.dist.incRefCount();
      return _newDomain(d);
    }
  
    proc setIndices(x) {
      _value.dsiSetIndices(x);
      if _isPrivatized(_valueType) {
        _reprivatize(_value);
      }
    }
  
    proc getIndices()
      return _value.dsiGetIndices();
  
    proc writeThis(f: Writer) {
      _value.dsiSerialWrite(f);
    }
    proc readThis(f: Reader) {
      _value.dsiSerialRead(f);
    }
  
    proc localSlice(r: range(?)... rank) where _value.type: DefaultRectangularDom {
      if (_value.locale != here) then
        halt("Attempting to take a local slice of a domain on locale ",
             _value.locale.id, " from locale ", here.id);
      return this((...r));
    }
  
    proc localSlice(r: range(?)... rank) {
      return _value.dsiLocalSlice(chpl__anyStridable(r), r);
    }
  
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }
  
    // associative array interface
  
    iter sorted() {
      for i in _value.dsiSorted() {
        yield i;
      }
    }
  
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }
  }  // record _domain
  
  proc chpl_countDomHelp(dom, counts) {
    var ranges = dom.dims();
    for param i in 1..dom.rank do
      ranges(i) = ranges(i) # counts(i);
    return dom[(...ranges)];
  }  
  
  proc #(dom: domain, counts: integral) where isRectangularDom(dom) && dom.rank == 1 {
    return chpl_countDomHelp(dom, (counts,));
  }
  
  proc #(dom: domain, counts) where isRectangularDom(dom) && isTuple(counts) {
    if (counts.size != dom.rank) then
      compilerError("the domain and tuple arguments of # must have the same rank");
    return chpl_countDomHelp(dom, counts);
  }
  
  proc #(arr: [], counts: integral) where isRectangularArr(arr) && arr.rank == 1 {
    return arr[arr.domain#counts];
  }
  
  proc #(arr: [], counts) where isRectangularArr(arr) && isTuple(counts) {
    if (counts.size != arr.rank) then
      compilerError("the domain and array arguments of # must have the same rank");
    return arr[arr.domain#counts];
  }
  
  
  proc _getNewDist(value) {
    return new dmap(value);
  }
  
  proc +(d: domain, i: index(d)) {
    if isRectangularDom(d) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }
  
  proc +(i, d: domain) where i: index(d) {
    if isRectangularDom(d) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }
  
  proc +(d: domain, i: index(d)) where isIrregularDom(d) {
    d.add(i);
    return d;
  }
  
  proc +(i, d: domain) where i:index(d) && isIrregularDom(d) {
    d.add(i);
    return d;
  }
  
  proc +(d1: domain, d2: domain) where
                                   (d1.type == d2.type) &&
                                   (isIrregularDom(d1) && isIrregularDom(d2)) {
    var d3: d1.type;
    // These should eventually become forall loops
    for e in d1 do d3.add(e);
    for e in d2 do d3.add(e);
    return d3;
  }
  
  proc +(d1: domain, d2: domain) {
    if (isRectangularDom(d1) || isRectangularDom(d2)) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }
  
  proc -(d: domain, i: index(d)) {
    if isRectangularDom(d) then
      compilerError("Cannot remove indices from a rectangular domain");
    else
      compilerError("Cannot remove indices from this domain type");
  }
  
  proc -(d: domain, i: index(d)) where isIrregularDom(d) {
    d.remove(i);
    return d;
  }
  
  proc -(d1: domain, d2: domain) where
                                   (d1.type == d2.type) &&
                                   (isIrregularDom(d1) && isIrregularDom(d2)) {
    var d3: d1.type;
    // These should eventually become forall loops
    for e in d1 do d3.add(e);
    for e in d2 do d3.remove(e);
    return d3;
  }
  
  proc -(d1: domain, d2: domain) {
    if (isRectangularDom(d1) || isRectangularDom(d2)) then
      compilerError("Cannot remove indices from a rectangular domain");
    else
      compilerError("Cannot remove indices from this domain type");
  }
  
  inline proc ==(d1: domain, d2: domain) where isRectangularDom(d1) &&
                                                        isRectangularDom(d2) {
    if d1._value.rank != d2._value.rank then return false;
    if d1._value == d2._value then return true;
    for param i in 1..d1._value.rank do
      if (d1.dim(i) != d2.dim(i)) then return false;
    return true;
  }
  
  inline proc !=(d1: domain, d2: domain) where isRectangularDom(d1) &&
                                                        isRectangularDom(d2) {
    if d1._value.rank != d2._value.rank then return true;
    if d1._value == d2._value then return false;
    for param i in 1..d1._value.rank do
      if (d1.dim(i) != d2.dim(i)) then return true;
    return false;
  }
  
  inline proc ==(d1: domain, d2: domain) where (isAssociativeDom(d1) &&
                                                         isAssociativeDom(d2)) {
    if d1._value == d2._value then return true;
    if d1.numIndices != d2.numIndices then return false;
    for idx in d1 do
      if !d2.member(idx) then return false;
    return true;
  }
  
  inline proc !=(d1: domain, d2: domain) where (isAssociativeDom(d1) &&
                                                         isAssociativeDom(d2)) {
    if d1._value == d2._value then return false;
    if d1.numIndices != d2.numIndices then return true;
    for idx in d1 do
      if !d2.member(idx) then return true;
    return false;
  }
  
  inline proc ==(d1: domain, d2: domain) where (isSparseDom(d1) &&
                                                         isSparseDom(d2)) {
    if d1._value == d2._value then return true;
    if d1.numIndices != d2.numIndices then return false;
    if d1._value.parentDom != d2._value.parentDom then return false;
    for idx in d1 do
      if !d2.member(idx) then return false;
    return true;
  }
  
  inline proc !=(d1: domain, d2: domain) where (isSparseDom(d1) &&
                                                         isSparseDom(d2)) {
    if d1._value == d2._value then return false;
    if d1.numIndices != d2.numIndices then return true;
    if d1._value.parentDom != d2._value.parentDom then return true;
    for idx in d1 do
      if !d2.member(idx) then return true;
    return false;
  }
  
  // any combinations not handled by the above
  
  inline proc ==(d1: domain, d2: domain) param {
    return false;
  }
  
  inline proc !=(d1: domain, d2: domain) param {
    return true;
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
    
    proc initialize() {
     if !noRefCount then
      chpl_incRefCountsForDomainsInArrayEltTypes(_value.eltType);
    }

    inline proc _value {
      if _isPrivatized(_valueType) {
        return chpl_getPrivatizedCopy(_valueType.type, _value);
      } else {
        return _value;
      }
    }

    //
    // Note that the destructor may be called multiple times for
    // a given array, corresponding to cases in which it's
    // autodestroyed multiple times; only the case that brings
    // the reference count to zero is the one that should
    // actually free the array.
    //
    proc ~_array() {
     if !noRefCount {
      if !_isPrivatized(_valueType) {
        on _value {
          var cnt = _value.destroyArr();
          if cnt == 0 then {
            chpl_decRefCountsForDomainsInArrayEltTypes(_value.eltType);
            delete _value;
          }
        }
      }
     }
    }
  
    proc eltType type return _value.eltType;
    proc _dom return _getDomain(_value.dom);
    proc rank param return this.domain.rank;
  
    inline proc this(i: rank*_value.dom.idxType) var {
      if isRectangularArr(this) || isSparseArr(this) then
        return _value.dsiAccess(i);
      else
        return _value.dsiAccess(i(1));
    }
  
    inline proc this(i: _value.dom.idxType ...rank) var
      return this(i);
  
    inline proc localAccess(i: rank*_value.dom.idxType) var {
      if isRectangularArr(this) || isSparseArr(this) then
        return _value.dsiLocalAccess(i);
      else
        return _value.dsiLocalAccess(i(1));
    }
  
    inline proc localAccess(i: _value.dom.idxType ...rank) var
      return localAccess(i);
    //
    // requires dense domain implementation that returns a tuple of
    // ranges via the getIndices() method; domain indexing is difficult
    // in the domain case because it has to be implemented on a
    // domain-by-domain basis; this is not terribly difficult in the
    // dense case because we can represent a domain by a tuple of
    // ranges, but in the sparse case, is there a general
    // representation?
    //
    proc this(d: domain) {
      if d.rank == rank then
        return this((...d.getIndices()));
      else
        compilerError("slicing an array with a domain of a different rank");
    }
  
    proc checkSlice(ranges: range(?) ...rank) {
      for param i in 1.._value.dom.rank do
        if !_value.dom.dsiDim(i).boundsCheck(ranges(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
    }
  
    proc this(ranges: range(?) ...rank) {
      if boundsChecking then
        checkSlice((... ranges));
      var d = _dom((...ranges));
      var a = _value.dsiSlice(d._value);
      a._arrAlias = _value;
      pragma "dont disable remote value forwarding"
      proc help() {
        d._value.incRefCount();
        a._arrAlias.incRefCount();
      }
      if !noRefCount then
        help();
      return _newArray(a);
    }
  
    proc this(args ...rank) where _validRankChangeArgs(args, _value.dom.idxType) {
      if boundsChecking then
        checkRankChange(args);
      var ranges = _getRankChangeRanges(args);
      param rank = ranges.size, stridable = chpl__anyStridable(ranges);
      var d = _dom((...args));
      if !noRefCount then
        d._value.incRefCount();
      var a = _value.dsiRankChange(d._value, rank, stridable, args);
      a._arrAlias = _value;
      if !noRefCount then
        a._arrAlias.incRefCount();
      return _newArray(a);
    }
  
    proc checkRankChange(args) {
      for param i in 1..args.size do
        if !_value.dom.dsiDim(i).boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
    }
  
    // Special cases of local slices for DefaultRectangularArrs because
    // we can't take an alias of the ddata class within that class
    proc localSlice(r: range(?)... rank) where _value.type: DefaultRectangularArr {
      if boundsChecking then
        checkSlice((...r));
      var dom = _dom((...r));
      return chpl__localSliceDefaultArithArrHelp(dom);
    }
  
    proc localSlice(d: domain) where _value.type: DefaultRectangularArr {
      if boundsChecking then
        checkSlice((...d.getIndices()));
  
      return chpl__localSliceDefaultArithArrHelp(d);
    }
  
    proc chpl__localSliceDefaultArithArrHelp(d: domain) {
      if (_value.locale != here) then
        halt("Attempting to take a local slice of an array on locale ",
             _value.locale.id, " from locale ", here.id);
      var A => this(d);
      return A;
    }
  
    proc localSlice(r: range(?)... rank) {
      if boundsChecking then
        checkSlice((...r));
      return _value.dsiLocalSlice(r);
    }
  
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }
  
    inline proc these() var {
      return _value.these();
    }
  
    // 1/5/10: do we need this since it always returns domain.numIndices?
    proc numElements return _value.dom.dsiNumIndices;
    proc size return numElements;
  
    proc newAlias() {
      var x = _value;
      return _newArray(x);
    }
  
    proc reindex(d: domain)
      where isRectangularDom(this.domain) && isRectangularDom(d)
    {
      if rank != d.rank then
        compilerError("illegal implicit rank change");
  
      // Optimization: Just return an alias of this array when
      // reindexing to the same domain. We skip same-ness test
      // if the domain descriptors' types are disjoint.
      if isSubtype(_value.dom.type, d._value.type) ||
         isSubtype(d._value.type, _value.dom.type)
      then
        if _value.dom:object == d._value:object then
          return newAlias();
  
      for param i in 1..rank do
        if d.dim(i).length != _value.dom.dsiDim(i).length then
          halt("extent in dimension ", i, " does not match actual");
  
      var newDist = new dmap(_value.dom.dist.dsiCreateReindexDist(d.dims(),
                                                                  _value.dom.dsiDims()));
      var newDom = {(...d.dims())} dmapped newDist;
      var x = _value.dsiReindex(newDom._value);
      x._arrAlias = _value;
      pragma "dont disable remote value forwarding"
      proc help() {
        newDom._value.incRefCount();
        x._arrAlias.incRefCount();
      }
      if !noRefCount then
        help();
      return _newArray(x);
    }
  
    // reindex for all non-rectangular domain types.
    // See above for the rectangular version.
    proc reindex(d:domain) {
      if this.domain != d then
        halt("Reindexing of non-rectangular arrays is undefined.");
      // Does this need to call newAlias()?
      return newAlias();
    }
  
    proc writeThis(f: Writer) {
      _value.dsiSerialWrite(f);
    }
    proc readThis(f: Reader) {
      _value.dsiSerialRead(f);
    }
  
    // sparse array interface
  
    proc IRV var {
      return _value.IRV;
    }
  
    // associative array interface
  
    iter sorted() {
      for i in _value.dsiSorted() {
        yield i;
      }
    }
  
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    // the locale grid domain
    proc targetLocDom() {
      return _value.dsiTargetLocDom();
    }

    // the locale grid
    proc targetLocales() {
      return _value.dsiTargetLocales();
    }

    // can the subdomain be represented as a single domain?
    proc oneLocalSubdomain() param {
      return _value.dsiOneLocalSubdomain();
    }

    // fetches the subdomain for the current locale
    //
    // Also note that localSlice(dom) produces a slice of a domain/array 
    // that's assumed to be local
    proc getLocalSubdomain() {
      if !_value.dsiOneLocalSubdomain() then
        compilerError("Array's local domain is not a single domain");
      return _value.dsiGetLocalSubdomain();
    }
    
    // if the subdomain cannot be represented as a single domain, 
    // the multiple domains are yielded by an iterator.
    // yield a domain so the user can use procs like expand/exterior/etc.
    iter getLocalSubdomains() {
      if _value.dsiOneLocalSubdomain() then 
        yield _value.dsiGetLocalSubdomain();
      else 
        for d in _value.dsiGetLocalSubdomains() do yield d;
    }

  }  // record _array
  
  //
  // Helper functions
  //
  
  proc isCollapsedDimension(r: range(?e,?b,?s,?a)) param return false;
  proc isCollapsedDimension(r) param return true;
  
  
  // computes || reduction over stridable of ranges
  proc chpl__anyStridable(ranges, param d: int = 1) param {
    for param i in 1..ranges.size do
      if ranges(i).stridable then
        return true;
    return false;
  }
  
  // given a tuple args, returns true if the tuple contains only
  // integers and ranges; that is, it is a valid argument list for rank
  // change
  proc _validRankChangeArgs(args, type idxType) param {
    proc _validRankChangeArg(type idxType, r: range(?)) param return true;
    proc _validRankChangeArg(type idxType, i: idxType) param return true;
    proc _validRankChangeArg(type idxType, x) param return false;
  
    proc help(param dim: int) param {
      if !_validRankChangeArg(idxType, args(dim)) then
        return false;
      else if dim < args.size then
        return help(dim+1);
      else
        return true;
    }
  
    return help(1);
  }
  
  proc chpl__isRange(r: range(?)) param return true;
  proc chpl__isRange(r) param return false;
  
  proc _getRankChangeRanges(args) {
    proc _tupleize(x) {
      var y: 1*x.type;
      y(1) = x;
      return y;
    }
    proc collectRanges(param dim: int) {
      if dim > args.size then
        compilerError("domain slice requires a range in at least one dimension");
      if chpl__isRange(args(dim)) then
        return collectRanges(dim+1, _tupleize(args(dim)));
      else
        return collectRanges(dim+1);
    }
    proc collectRanges(param dim: int, x: _tuple) {
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
  proc chpl__isDomain(x: domain) param return true;
  proc chpl__isDomain(x) param return false;
  
  proc chpl__isArray(x: []) param return true;
  proc chpl__isArray(x) param return false;
  
  proc chpl__isDmap(x: _distribution) param return true;
  proc chpl__isDmap(x) param return false;
  
  //
  // Assignment of domains and arrays
  //
  proc =(ref a: _distribution, b: _distribution) {
    if a._value == nil {
      __primitive("move", a, chpl__autoCopy(b.clone()));
    } else if a._value._doms.length == 0 {
      if a._value.type != b._value.type then
        compilerError("type mismatch in distribution assignment");
      // TODO: This is needed as part of the assign_to_ref implementation to
      // keep _distCnt from going below zero.  It's probably not the "correct"
      // solution, but it resolves the immediate problem in
      // test_distribution_syntax2.chpl (compiled with CHPL_COMM=none).
      if a._value == b._value {
        if !noRefCount then
          a._value.incRefCount();
      } else
        a._value.dsiAssign(b._value);
      if _isPrivatized(a._valueType) then
        _reprivatize(a._value);
    } else {
      halt("assignment to distributions with declared domains is not yet supported");
    }
  }
  
  proc =(ref a: domain, b: domain) {
    if !isIrregularDom(a) && !isIrregularDom(b) {
      for e in a._value._arrs do {
        on e do e.dsiReallocate(b);
      }
      a.setIndices(b.getIndices());
      for e in a._value._arrs do {
        on e do e.dsiPostReallocate();
      }
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
      // NOTE: For the current implementation of associative domains,
      // the domain iteration is parallelized, but modification
      // of the underlying data structures (in particular, the _resize()
      // operation on the table) is not thread-safe.  Something more
      // intelligent will likely be needed before it is worth it to
      // parallelize whole-domain assignment for associative arrays.
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
  }
  
  proc =(ref a: domain, b: _tuple) {
    a._value.clearForIteratableAssign();
    for ind in 1..b.size {
      a.add(b(ind));
    }
  }
  
  proc =(ref d: domain, r: range(?)) {
    d = {r};
  }
  
  //
  // Return true if t is a tuple of ranges that is legal to assign to
  // rectangular domain d
  //
  proc chpl__isLegalRectTupDomAssign(d, t) param {
    proc isRangeTuple(a) param {
      proc isRange(r: range(?e,?b,?s)) param return true;
      proc isRange(r) param return false;
      proc peelArgs(first, rest...) param {
        return if rest.size > 1 then
                 isRange(first) && peelArgs((...rest))
               else
                 isRange(first) && isRange(rest(1));
      }
      proc peelArgs(first) param return isRange(first);
  
      return if !isTuple(a) then false else peelArgs((...a));
    }
  
    proc strideSafe(d, rt, param dim: int=1) param {
      return if dim == d.rank then
               d.dim(dim).stridable || !rt(dim).stridable
             else
               (d.dim(dim).stridable || !rt(dim).stridable) && strideSafe(d, rt, dim+1);
    }
    return isRangeTuple(t) && d.rank == t.size && strideSafe(d, t);
  }
  
  proc =(ref d: domain, rt: _tuple) where chpl__isLegalRectTupDomAssign(d, rt) {
    d = {(...rt)};
  }
  
  proc =(ref a: domain, b) {  // b is iteratable
    a._value.clearForIteratableAssign();
    for ind in b {
      a.add(ind);
    }
  }
  
  proc chpl__serializeAssignment(a: [], b) param {
    if a.rank != 1 && chpl__isRange(b) then
      return true;
  
    // Sparse and Opaque arrays do not yet support parallel iteration.  We
    // could let them fall through, but then we get multiple warnings for a
    // single assignment statement which feels like overkill
    //
    if ((!isRectangularArr(a) && !isAssociativeArr(a) && !isSparseArr(a)) ||
        (chpl__isArray(b) &&
         !isRectangularArr(b) && !isAssociativeArr(b) && !isSparseArr(b))) then
      return true;
    return false;
  }
  
  // This must be a param function
  proc chpl__compatibleForBulkTransfer(a:[], b:[]) param {
    if a.eltType != b.eltType then return false;
    if !chpl__supportedDataTypeForBulkTransfer(a.eltType) then return false;
    if a._value.type != b._value.type then return false;
    if !a._value.dsiSupportsBulkTransfer() then return false;
    return true;
  }
  
  proc chpl__compatibleForBulkTransferStride(a:[], b:[]) param {
    if a.eltType != b.eltType then return false;
    if !chpl__supportedDataTypeForBulkTransfer(a.eltType) then return false;
    if !chpl__supportedDataTypeForBulkTransfer(b.eltType) then return false;
    if !a._value.dsiSupportsBulkTransferInterface() then return false;
    if !b._value.dsiSupportsBulkTransferInterface() then return false;
    return true;
  }
  
  // This must be a param function
  proc chpl__supportedDataTypeForBulkTransfer(type t) param {
    var x:t;
    return chpl__supportedDataTypeForBulkTransfer(x);
  }
  proc chpl__supportedDataTypeForBulkTransfer(x: string) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: sync) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: single) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: domain) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: []) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: _distribution) param return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: ?t) param where _isComplexType(t) return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: ?t) param where t: value return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: object) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x) param return true;
  
  proc chpl__useBulkTransfer(a:[], b:[]) {
    //if debugDefaultDistBulkTransfer then writeln("chpl__useBulkTransfer");
  
    // constraints specific to a particular domain map array type
    if !a._value.doiCanBulkTransfer() then return false;
    if !b._value.doiCanBulkTransfer() then return false;
  
    return true;
  }
  
  //NOTE: This function also checks for equal lengths in all dimensions, 
  //as the previous one (chpl__useBulkTransfer) so depending on the order they
  //are called, this can be factored out.
  proc chpl__useBulkTransferStride(a:[], b:[]) {
    //if debugDefaultDistBulkTransfer then writeln("chpl__useBulkTransferStride");
    
    // constraints specific to a particular domain map array type
    if !a._value.doiCanBulkTransferStride() then return false;
    if !b._value.doiCanBulkTransferStride() then return false;
    
    return true;
  }
  
  inline proc chpl__bulkTransferHelper(a, b) {
    if a._value.isDefaultRectangular() {
      if b._value.isDefaultRectangular() {
        // implemented in DefaultRectangular
        a._value.adjustBlkOffStrForNewDomain(a._value.dom, a._value);
        b._value.adjustBlkOffStrForNewDomain(b._value.dom, b._value);
        a._value.doiBulkTransferStride(b._value);
      }
      else
        // b's domain map must implement this
        b._value.doiBulkTransferToDR(a);
    } else {
      if b._value.isDefaultRectangular() then
        // a's domain map must implement this
        a._value.doiBulkTransferFromDR(b);
      else
        // a's domain map must implement this,
        // possibly using b._value.doiBulkTransferToDR()
        a._value.doiBulkTransferFrom(b);
    }
 }
  
  proc checkArrayShapesUponAssignment(a: [], b: []) {
    if isRectangularArr(a) && isRectangularArr(b) {
      const aDims = a._value.dom.dsiDims(),
            bDims = b._value.dom.dsiDims();
      compilerAssert(aDims.size == bDims.size);
      for param i in 1..aDims.size {
        if aDims(i).length != bDims(i).length then
          halt("assigning between arrays of different shapes in dimension ",
               i, ": ", aDims(i).length, " vs. ", bDims(i).length);
      }
    } else {
      // may not have dsiDims(), so can't check them as above
      // todo: compilerError if one is rectangular and the other isn't?
    }
  }
  
  inline proc =(ref a: [], b:[]) {
    if a.rank != b.rank then
      compilerError("rank mismatch in array assignment");
    
    if b._value == nil then
      // This happens e.g. for 'new' on a record with an array field whose
      // default initalizer is a forall expr. E.g. arrayInClassRecord.chpl.
      return;

    if boundsChecking then
      checkArrayShapesUponAssignment(a, b);
  
    // try bulk transfer
    if !chpl__serializeAssignment(a, b) then
      // Do bulk transfer.
      chpl__bulkTransferArray(a, b);
    else
      // Do non-bulk transfer.
      chpl__transferArray(a, b);
  }
  
  inline proc chpl__bulkTransferArray(a: [], b) {
    if (useBulkTransfer &&
        chpl__compatibleForBulkTransfer(a, b) &&
        chpl__useBulkTransfer(a, b))
    {
      a._value.doiBulkTransfer(b);
    }
    else if (useBulkTransferStride &&
        chpl__compatibleForBulkTransferStride(a, b) &&
        chpl__useBulkTransferStride(a, b))
    {
      chpl__bulkTransferHelper(a, b);
    }
    else {
      if debugBulkTransfer then
        // just writeln() clashes with writeln.chpl
        stdout.writeln("proc =(a:[],b): bulk transfer did not happen");
      chpl__transferArray(a, b);
    }
  }

  inline proc chpl__transferArray(a: [], b) {
    if (a.eltType == b.type ||
        _isPrimitiveType(a.eltType) && _isPrimitiveType(b.type)) {
      forall aa in a do
        aa = b;
    } else if chpl__serializeAssignment(a, b) {
      compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
      for (aa,bb) in zip(a,b) do
        aa = bb;
    } else if chpl__tryToken { // try to parallelize using leader and follower iterators
      forall (aa,bb) in zip(a,b) do
        aa = bb;
    } else {
      compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
      for (aa,bb) in zip(a,b) do
        aa = bb;
    }
  }
  
  inline proc =(ref a: [], b:domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in array assignment");
    chpl__transferArray(a, b);
  }
  
  inline proc =(ref a: [], b) /* b is not an array nor a domain nor a tuple */ {
    chpl__transferArray(a, b);
  }
  
  inline proc =(ref a: [], b: _tuple) where isEnumArr(a) {
    if b.size != a.numElements then
      halt("tuple array initializer size mismatch");
    for (i,j) in zip(chpl_enumerate(index(a.domain)), 1..) {
      a(i) = b(j);
    }
  }
  
  proc =(ref a: [], b: _tuple) where isRectangularArr(a) {
    proc chpl__tupleInit(j, param rank: int, b: _tuple) {
      type idxType = a.domain.idxType,
           strType = chpl__signedType(idxType);
           
      const stride = a.domain.dim(a.rank-rank+1).stride,
            start = a.domain.dim(a.rank-rank+1).first;

      if rank == 1 {
        for param i in 1..b.size {
          j(a.rank-rank+1) = (start:strType + ((i-1)*stride)): idxType;
          a(j) = b(i);
        }
      } else {
        for param i in 1..b.size {
          j(a.rank-rank+1) = (start:strType + ((i-1)*stride)): idxType;
          chpl__tupleInit(j, rank-1, b(i));
        }
      }
    }
    var j: a.rank*a.domain.idxType;
    chpl__tupleInit(j, a.rank, b);
  }
  
  proc _desync(type t) where t: _syncvar || t: _singlevar {
    var x: t;
    return x.value;
  }
  
  proc _desync(type t) {
    var x: t;
    return x;
  }
  
  proc =(ref a: [], b: _desync(a.eltType)) {
    if isRectangularArr(a) {
      forall e in a do
        e = b;
    } else {
      compilerWarning("whole array assignment has been serialized (see note in $CHPL_HOME/STATUS)");
      for e in a do
        e = b;
    }
  }
  
  proc by(a: domain, b) {
    var r: a.rank*range(a._value.idxType,
                      BoundedRangeType.bounded,
                      true);
    var t = _makeIndexTuple(a.rank, b, expand=true);
    for param i in 1..a.rank do
      r(i) = a.dim(i) by t(i);
    var d = a._value.dsiBuildRectangularDom(a.rank, a._value.idxType, true, r);
    if !noRefCount then
      if (d.linksDistribution()) then
        d.dist.incRefCount();
    return _newDomain(d);
  }
  
  //
  // index for all opaque domains
  //
  class _OpaqueIndex { }
  
  //
  // Swap operator for arrays
  //
  inline proc <=>(x: [], y: []) {
    forall (a,b) in zip(x, y) do
      a <=> b;
  }
  
  //
  // reshape function
  //
  proc reshape(A: [], D: domain) {
    if !isRectangularDom(D) then
      compilerError("reshape(A,D) is meaningful only when D is a rectangular domain; got D: ", typeToString(D.type));
    if A.size != D.size then
      halt("reshape(A,D) is invoked when A has ", A.size,
           " elements, but D has ", D.size, " indices");
    var B: [D] A.eltType;
    for (i,a) in zip(D,A) do
      B(i) = a;
    return B;
  }
  
  iter linearize(Xs) {
    for x in Xs do yield x;
  }
  
  //
  // module support for iterators
  //
  proc iteratorIndex(ic: _iteratorClass) {
    ic.advance();
    return ic.getValue();
  }
  
  pragma "expand tuples with values"
  proc iteratorIndex(t: _tuple) {
    pragma "expand tuples with values"
    proc iteratorIndexHelp(t: _tuple, param dim: int) {
      if dim == t.size then
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)));
      else
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                             (...iteratorIndexHelp(t, dim+1)));
    }
  
    return iteratorIndexHelp(t, 1);
  }
  
  proc iteratorIndexType(x) type {
    pragma "no copy" var ic = _getIterator(x);
    pragma "no copy" var i = iteratorIndex(ic);
    _freeIterator(ic);
    return i.type;
  }
  
  proc _iteratorRecord.writeThis(f: Writer) {
    var first: bool = true;
    for e in this {
      if !first then
        f.write(" ");
      else
        first = false;
      f.write(e);
    }
  }
  
  proc =(ref ic: _iteratorRecord, xs) {
    for (e, x) in zip(ic, xs) do
      e = x;
  }
  
  proc =(ref ic: _iteratorRecord, x: iteratorIndexType(ic)) {
    for e in ic do
      e = x;
  }
  
  inline proc _getIterator(x) {
    return _getIterator(x.these());
  }
  
  inline proc _getIterator(ic: _iteratorClass)
    return ic;
  
  proc _getIterator(type t) {
    compilerError("cannot iterate over a type");
  }
  
  inline proc _getIteratorZip(x) {
    return _getIterator(x);
  }
  
  inline proc _getIteratorZip(x: _tuple) {
    inline proc _getIteratorZipInternal(x: _tuple, param dim: int) {
      if dim == x.size then
        return (_getIterator(x(dim)),);
      else
        return (_getIterator(x(dim)), (..._getIteratorZipInternal(x, dim+1)));
    }
    if x.size == 1 then
      return _getIterator(x(1));
    else
      return _getIteratorZipInternal(x, 1);
  }
  
  proc _checkIterator(type t) {
    compilerError("cannot iterate over a type");
  }
  
  inline proc _checkIterator(x) {
    return x;
  }
  
  inline proc _freeIterator(ic: _iteratorClass) {
    chpl_here_free(__primitive("cast_to_void_star", ic));
  }
  
  inline proc _freeIterator(x: _tuple) {
    for param i in 1..x.size do
      _freeIterator(x(i));
  }
  
  pragma "no implicit copy"
  inline proc _toLeader(iterator: _iteratorClass)
    return chpl__autoCopy(__primitive("to leader", iterator));
  
  inline proc _toLeader(ir: _iteratorRecord) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var leader = _toLeader(ic);
    _freeIterator(ic);
    return leader;
  }
  
  inline proc _toLeader(x)
    return _toLeader(x.these());
  
  inline proc _toLeaderZip(x)
    return _toLeader(x);
  
  inline proc _toLeaderZip(x: _tuple)
    return _toLeader(x(1));
  
  //
  // return true if any iterator supports fast followers
  //
  proc chpl__staticFastFollowCheck(x) param {
    pragma "no copy" const lead = x;
    if chpl__isDomain(lead) || chpl__isArray(lead) then
      return chpl__staticFastFollowCheck(x, lead);
    else
      return false;
  }  
  
  proc chpl__staticFastFollowCheck(x, lead) param {
    return false;
  }
  
  proc chpl__staticFastFollowCheck(x: [], lead) param {
    return x._value.dsiStaticFastFollowCheck(lead._value.type);
  }
  
  proc chpl__staticFastFollowCheckZip(x: _tuple) param {
    pragma "no copy" const lead = x(1);
    if chpl__isDomain(lead) || chpl__isArray(lead) then
      return chpl__staticFastFollowCheckZip(x, lead);
    else
      return false;
  } 
  
  proc chpl__staticFastFollowCheckZip(x, lead) param {
    return chpl__staticFastFollowCheck(x, lead);
  }
  
  proc chpl__staticFastFollowCheckZip(x: _tuple, lead, param dim = 1) param {
    if x.size == dim then
      return chpl__staticFastFollowCheckZip(x(dim), lead);
    else
      return chpl__staticFastFollowCheckZip(x(dim), lead) || chpl__staticFastFollowCheckZip(x, lead, dim+1);
  }
  
  //
  // return true if all iterators that support fast followers can use
  // their fast followers
  //
  proc chpl__dynamicFastFollowCheck(x) {
    return chpl__dynamicFastFollowCheck(x, x);
  }
  
  proc chpl__dynamicFastFollowCheck(x, lead) {
    return true;
  }
  
  proc chpl__dynamicFastFollowCheck(x: [], lead) {
    if chpl__staticFastFollowCheck(x, lead) then
      return x._value.dsiDynamicFastFollowCheck(lead);
    else
      return false;
  }
  
  proc chpl__dynamicFastFollowCheckZip(x: _tuple) {
    return chpl__dynamicFastFollowCheckZip(x, x(1));
  }
  
  proc chpl__dynamicFastFollowCheckZip(x, lead) {
    return chpl__dynamicFastFollowCheck(x, lead);
  }
  
  proc chpl__dynamicFastFollowCheckZip(x: _tuple, lead, param dim = 1) {
    if x.size == dim then
      return chpl__dynamicFastFollowCheckZip(x(dim), lead);
    else
      return chpl__dynamicFastFollowCheckZip(x(dim), lead) && chpl__dynamicFastFollowCheckZip(x, lead, dim+1);
  }
  
  pragma "no implicit copy"
  inline proc _toFollower(iterator: _iteratorClass, leaderIndex)
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex));
  
  inline proc _toFollower(ir: _iteratorRecord, leaderIndex) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFollower(ic, leaderIndex);
    _freeIterator(ic);
    return follower;
  }
  
  inline proc _toFollower(x, leaderIndex) {
    return _toFollower(x.these(), leaderIndex);
  }
  
  inline proc _toFollowerZip(x, leaderIndex) {
    return _toFollower(x, leaderIndex);
  }
  
  inline proc _toFollowerZip(x: _tuple, leaderIndex) {
    return _toFollowerZipInternal(x, leaderIndex, 1);
  }

  inline proc _toFollowerZipInternal(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size then
      return (_toFollower(x(dim), leaderIndex),);
    else
      return (_toFollower(x(dim), leaderIndex),
              (..._toFollowerZipInternal(x, leaderIndex, dim+1)));
  }

  pragma "no implicit copy"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex, fast: bool) {
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex, true));
  }
  
  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex, fast: bool) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFastFollower(ic, leaderIndex, fast=true);
    _freeIterator(ic);
    return follower;
  }
  
  pragma "no implicit copy"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex) {
    return _toFollower(iterator, leaderIndex);
  }
  
  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex) {
    return _toFollower(ir, leaderIndex);
  }
  
  inline proc _toFastFollower(x, leaderIndex) {
    if chpl__staticFastFollowCheck(x) then
      return _toFastFollower(x.these(), leaderIndex, fast=true);
    else
      return _toFollower(x.these(), leaderIndex);
  }
  
  inline proc _toFastFollowerZip(x, leaderIndex) {
    return _toFastFollower(x, leaderIndex);
  }
  
  inline proc _toFastFollowerZip(x: _tuple, leaderIndex) {
    return _toFastFollowerZip(x, leaderIndex, 1);
  }
  
  inline proc _toFastFollowerZip(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 then
      return (_toFastFollowerZip(x(dim), leaderIndex),
              _toFastFollowerZip(x(dim+1), leaderIndex));
    else
      return (_toFastFollowerZip(x(dim), leaderIndex),
              (..._toFastFollowerZip(x, leaderIndex, dim+1)));
  }
  
  pragma "init copy fn"
  proc chpl__initCopy(a: _distribution) {
    pragma "no copy" var b = chpl__autoCopy(a.clone());
    return b;
  }
  
  pragma "init copy fn"
  proc chpl__initCopy(a: domain) {
    var b: a.type;
    if isRectangularDom(a) && isRectangularDom(b) {
      b.setIndices(a.getIndices());
    } else {
      // TODO: These should eventually become forall loops, hence the
      // warning
      //
      // NOTE: See above note regarding associative domains
      //
      compilerWarning("whole-domain assignment has been serialized (see note in $CHPL_HOME/STATUS)");
      for i in a do
        b.add(i);
    }
    return b;
  }
  
  pragma "init copy fn"
  proc chpl__initCopy(a: []) {
    var b : [a._dom] a.eltType;

    // Try bulk transfer.
    if !chpl__serializeAssignment(b, a) {
      chpl__bulkTransferArray(b, a);
      return b;
    }

    chpl__transferArray(b, a);
    return b;
  }
  
  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord) {
    iter _ir_copy_recursive(ir) {
      for e in ir do
        yield chpl__initCopy(e);
    }
  
    pragma "no copy" var irc = _ir_copy_recursive(ir);
  
    var i = 1, size = 4;
    pragma "insert auto destroy" var D = {1..size};
  
    // note that _getIterator is called in order to copy the iterator
    // class since for arrays we need to iterate once to get the
    // element type (at least for now); this also means that if this
    // iterator has side effects, we will see them; a better way to
    // handle this may be to get the static type (not initialize the
    // array) and use a primitive to set the array's element; that may
    // also handle skyline arrays
    var A: [D] iteratorIndexType(irc);
  
    for e in irc {
      //pragma "no copy" /*pragma "insert auto destroy"*/ var ee = e;
      if i > size {
        size = size * 2;
        D = {1..size};
      }
      //A(i) = ee;
      A(i) = e;
      i = i + 1;
    }
    D = {1..i-1};
    return A;
  }
  
}
