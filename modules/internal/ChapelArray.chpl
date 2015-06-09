/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ChapelArray.chpl
//
module ChapelArray {

  use ChapelBase; // For opaque type.
  use ChapelTuple;
  use ChapelLocale;

  // Explicitly use a processor atomic, as most calls to this function are
  // likely be on locale 0
  var numPrivateObjects: atomic_int64;

  config param debugBulkTransfer = false;
  config param useBulkTransfer = true;
  config param useBulkTransferStride = false;

  pragma "privatized class"
  proc _isPrivatized(value) param
    return !_local && ((_privatization && value.dsiSupportsPrivatization()) || value.dsiRequiresPrivatization());

  proc _newPrivatizedClass(value) {

    var n = numPrivateObjects.fetchAdd(1);

    var hereID = here.id;
    const privatizeData = value.dsiGetPrivatizeData();
    on Locales[0] do
      _newPrivatizedClassHelp(value, value, n, hereID, privatizeData);

    proc _newPrivatizedClassHelp(parentValue, originalValue, n, hereID, privatizeData) {
      var newValue = originalValue;
      if hereID != here.id {
        newValue = parentValue.dsiPrivatize(privatizeData);
        __primitive("chpl_newPrivatizedClass", newValue, n);
        newValue.pid = n;
      } else {
        __primitive("chpl_newPrivatizedClass", newValue, n);
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
  proc chpl__buildSparseDomainRuntimeType(dom: domain) {
    compilerWarning("***"+typeToString(dom._value.type));
    return _newDomain(dom._value.dsiNewSpsSubDom(dom));
  }

  /*
  //  pragma "has runtime type"
  pragma "runtime type init fn"
    proc chpl__buildSparseDomainRuntimeType(dom: domain)  {
    return _newDomain(d.newSparseDom(dom.rank, dom._value.idxType, dom));
  }
  */
  
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
    return chpl__buildSparseDomainRuntimeType(dom._value.parentDom);
  
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

    if CHPL_WARN_DOMAIN_LITERAL == "true" && isRange(elems(1)) {
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
      return chpl__buildSparseDomainRuntimeType(dom._value.parentDom);
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
    return isAssociativeDom(d) && isEnumType(d._value.idxType);
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
  pragma "ignore noinit"
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
    where isEnumType(idxType) {
      var x = _value.dsiNewAssociativeDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
        if !noRefCount then
          _value.incRefCount();
      }
      const enumTuple = chpl_enum_enumerate(idxType);
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
  
  // The following method is called by the compiler to determine the default
  // value of a given type.
  /* Need new <alias>() for this to function
  proc _defaultOf(type t) where t:_distribution {
    var ret: t = noinit;
    type valType = __primitive("query type field", t, "_valueType");
    var typeInstance = new <valType>();
    ret = chpl__buildDistValue(typeInstance);
    return ret;
  }
  */
  
  //
  // Domain wrapper record
  //
  pragma "domain"
  pragma "has runtime type"
  pragma "ignore noinit"
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

    pragma "reference to const when const this"
    proc newAlias() {
      var x = _value;
      return _newDomain(x);
    }

    /*
       Returns true if this domain is a subset of 'super'
    */
    proc isSubset(super : domain) {
      if !isAssociativeDom(this) {
        if isRectangularDom(this) then
          compilerError("isSubset not supported on rectangular domains");
        else if isOpaqueDom(this) then
          compilerError("isSubset not supported on opaque domains");
        else if isSparseDom(this) then
          compilerError("isSubset not supported on sparse domains");
        else
          compilerError("isSubset not supported on this domain type");
      }
      if super.type != this.type then
        compilerError("isSuper called with different associative domain types");

      return && reduce forall i in this do super.member(i);
    }

    /*
       Returns true if this domain is a superset of 'sub'
    */
    proc isSuper(sub : domain) {
      if !isAssociativeDom(this) {
        if isRectangularDom(this) then
          compilerError("isSuper not supported on rectangular domains");
        else if isOpaqueDom(this) then
          compilerError("isSuper not supported on opaque domains");
        else if isSparseDom(this) then
          compilerError("isSuper not supported on sparse domains");
        else
          compilerError("isSuper not supported on the domain type ", this.type);
      }
      if sub.type != this.type then
        compilerError("isSuper called with different associative domain types");

      return && reduce forall i in sub do this.member(i);
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
                                   (isSparseDom(d1) || isOpaqueDom(d1)) {
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
  pragma "ignore noinit"
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
    proc idxType type return _value.idxType;
    proc _dom return _getDomain(_value.dom);
    proc rank param return this.domain.rank;
  
    // When 'this' is 'const', so is the returned l-value.
    pragma "reference to const when const this"
    inline proc this(i: rank*_value.dom.idxType) ref {
      if isRectangularArr(this) || isSparseArr(this) then
        return _value.dsiAccess(i);
      else
        return _value.dsiAccess(i(1));
    }
  
    pragma "reference to const when const this"
    inline proc this(i: _value.dom.idxType ...rank) ref
      return this(i);
  
    pragma "reference to const when const this"
    inline proc localAccess(i: rank*_value.dom.idxType) ref {
      if isRectangularArr(this) || isSparseArr(this) then
        return _value.dsiLocalAccess(i);
      else
        return _value.dsiLocalAccess(i(1));
    }
  
    pragma "reference to const when const this"
    inline proc localAccess(i: _value.dom.idxType ...rank) ref
      return localAccess(i);
  
    //
    // requires dense domain implementation that returns a tuple of
    // ranges via the getIndices() method; domain indexing is difficult
    // in the domain case because it has to be implemented on a
    // domain-by-domain basis; this is not terribly difficult in the
    // dense case because we can represent a domain by a tuple of
    // ranges, but in the sparse case, is there a general representation?
    //
    pragma "reference to const when const this"
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
  
    pragma "reference to const when const this"
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
  
    pragma "reference to const when const this"
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
    pragma "reference to const when const this"
    proc localSlice(r: range(?)... rank) where _value.type: DefaultRectangularArr {
      if boundsChecking then
        checkSlice((...r));
      var dom = _dom((...r));
      return chpl__localSliceDefaultArithArrHelp(dom);
    }
  
    pragma "reference to const when const this"
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
  
    pragma "reference to const when const this"
    proc localSlice(r: range(?)... rank) {
      if boundsChecking then
        checkSlice((...r));
      return _value.dsiLocalSlice(r);
    }
  
    pragma "reference to const when const this"
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }
  
    inline proc these() ref {
      return _value.these();
    }
  
    // 1/5/10: do we need this since it always returns domain.numIndices?
    proc numElements return _value.dom.dsiNumIndices;
    proc size return numElements;
  
    pragma "reference to const when const this"
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
  
    proc IRV ref {
      return _value.IRV;
    }
  
    // associative array interface
  
    iter sorted() {
      for i in _value.dsiSorted() {
        yield i;
      }
    }
  
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    // the locale grid
    proc targetLocales() {
      return _value.dsiTargetLocales();
    }

    // can the subdomain be represented as a single domain?
    proc hasSingleLocalSubdomain() param {
      return _value.dsiHasSingleLocalSubdomain();
    }

    // fetches the subdomain for the current locale
    //
    // Also note that localSlice(dom) produces a slice of a domain/array 
    // that's assumed to be local
    proc localSubdomain() {
      if !_value.dsiHasSingleLocalSubdomain() then
        compilerError("Array's local domain is not a single domain");
      return _value.dsiLocalSubdomain();
    }
    
    // if the subdomain cannot be represented as a single domain, 
    // the multiple domains are yielded by an iterator.
    // yield a domain so the user can use procs like expand/exterior/etc.
    iter localSubdomains() {
      if _value.dsiHasSingleLocalSubdomain() then 
        yield _value.dsiLocalSubdomain();
      else 
        for d in _value.dsiLocalSubdomains() do yield d;
    }

    proc chpl__isDense1DArray() param {
      return isRectangularArr(this) &&
             this.rank == 1 &&
             !this._value.stridable;
    }

    inline proc chpl__assertSingleArrayDomain(fnName: string) {
      if this.domain._value._arrs.length != 1 then
        halt("cannot call " + fnName +
             " on an array defined over a domain with multiple arrays");
    }

    /* The following methods are intended to provide a list or vector style
       interface to 1D unstridable rectangular arrays.  They are only intended
       for use on arrays that have a 1:1 correspondence with their domains.
       All methods here that modify the array's domain assert that this 1:1
       property holds.

       These are currently not parallel safe, and cannot safely be called by
       multiple tasks simultaneously on the same array.

       The current implementation reallocates the array every time the domain
       is modified.  This could be improved with a size doubling/halving
       strategy.
     */

    /* Return true if the array has no elements */
    proc isEmpty(): bool {
      return this.numElements == 0;
    }

    /* Return the first value in the array */
    proc head(): this._value.eltType {
      return this[this.domain.low];
    }

    /* Return the last value in the array */
    proc tail(): this._value.eltType {
      return this[this.domain.high];
    }

    /* Add element 'val' to the back of the array, extending the array's
       domain by one. If the domain was {1..5} it will become {1..6}.
       val: the value to add to the back of the array
     */
    proc push_back(val: this.eltType) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("push_back");
      const lo = this.domain.low,
            hi = this.domain.high+1;
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
      this[hi] = val;
    }

    /* Remove the last element from the array, reducing the size of the
       domain by one. If the domain was {1..5} it will become {1..4}
     */
    proc pop_back() where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("pop_back");
      const lo = this.domain.low,
            hi = this.domain.high-1;
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
    }

    /* Add element 'val' to the front of the array, extending the array's
       domain by one. If the domain was {1..5} it will become {0..5}.
       val: the value to add to the front of the array
     */
    proc push_front(val: this.eltType) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("push_front");
      const lo = this.domain.low-1,
            hi = this.domain.high;
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
      this[lo] = val;
    }

    /* Remove the first element of the array reducing the size of the
       domain by one.  If the domain was {1..5} it will become {2..5}.
     */
    proc pop_front() where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("pop_front");
      const lo = this.domain.low+1,
            hi = this.domain.high;
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
    }

    /* Insert element 'val' into the array at index 'pos'. Shift the array
       elements above 'pos' up one index. If the domain was {1..5} it will
       become {1..6}.
       val: the value to add to the array
       pos: the index at which the value should be added
     */
    proc insert(pos: this.idxType, val: this.eltType) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("insert");
      const lo = this.domain.low,
            hi = this.domain.high+1;
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
      for i in pos..hi-1 by -1 do this[i+1] = this[i];
      this[pos] = val;
    }

    /* Remove the element at index 'pos' from the array and shift the array
       elements above 'pos' down one index. If the domain was {1..5} it will
       become {1..4}.
       pos: the index at which the value should be removed
     */
    proc remove(pos: this.idxType) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("remove");
      const lo = this.domain.low,
            hi = this.domain.high-1;
      const newDom = {lo..hi};
      for i in pos..hi {
        this[i] = this[i+1];
      }
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
    }

    /* Remove 'count' elements from the array starting at index 'pos' and
       shift elements above 'pos+count' down by 'count' indices.
       pos: The index to start removing from
       count: The number of array elements to remove
     */
    proc remove(pos: this.idxType, count: this.idxType) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("remove count");
      const lo = this.domain.low,
            hi = this.domain.high-count;
      if pos > hi then
        halt("index ", pos+count, " is outside the supported range");
      const newDom = {lo..hi};
      for i in pos..hi {
        this[i] = this[i+count];
      }
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
    }

    /* Remove the elements at the indices in the 'pos' range and shift the
       array elements down by 'pos.size' elements. If the domain was {1..5}
       and this is called with 2..3 as an argument, the new domain would be
       {1..3} and the array would contain the elements formerly at positions
       1, 4, 5.
       pos: a dense range of indices to remove
     */
    proc remove(pos: range(this.idxType, stridable=false)) where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("remove range");
      remove(pos.low, pos.size);
    }

    /* Reverse the order of the values in the array. */
    proc reverse() where chpl__isDense1DArray() {
      const lo = this.domain.low,
            mid = this.domain.size / 2,
            hi = this.domain.high;
      for i in 0..#mid {
        this[lo + i] <=> this[hi - i];
      }
    }

    /* Remove all elements from the array leaving the domain empty. If the
       domain was {5..10} it will become {5..4}.
     */
    proc clear() where chpl__isDense1DArray() {
      chpl__assertSingleArrayDomain("clear");
      const lo = this.domain.low,
            hi = this.domain.low-1;
      assert(hi < lo, "overflow occured subtracting 1 from low bound in clear");
      const newDom = {lo..hi};
      on this._value {
        this._value.dsiReallocate(newDom);
        this.domain.setIndices(newDom.getIndices());
        this._value.dsiPostReallocate();
      }
    }

    /* Return a tuple containing true and the index of the first instance of
       'val' in the array, or if 'val' is not found, a tuple containing false
       and an unspecified value is returned.
       val: the value to locate
     */
    proc find(val: this.eltType): (bool, this.idxType) {
      for i in this.domain {
        if this[i] == val then return (true, i);
      }
      return (false, 0);
    }

    /* Return the number of times 'val' occurs in the array.
       val: the value to count
     */
    proc count(val: this.eltType): int {
      var total: int = 0;
      for i in this do if i == val then total += 1;
      return total;
    }
  }  // record _array


  //
  // isXxxType, isXxxValue
  //

  proc isDmapType(type t) param {
    proc isDmapHelp(type t: _distribution) param  return true;
    proc isDmapHelp(type t)                param  return false;
    return isDmapHelp(t);
  }

  proc isDmapValue(e: _distribution) param  return true;
  proc isDmapValue(e)                param  return false;

  proc isDomainType(type t) param {
    proc isDomainHelp(type t: _domain) param  return true;
    proc isDomainHelp(type t)          param  return false;
    return isDomainHelp(t);
  }
  
  proc isDomainValue(e: domain) param  return true;
  proc isDomainValue(e)         param  return false;

  proc isArrayType(type t) param {
    proc isArrayHelp(type t: _array) param  return true;
    proc isArrayHelp(type t)         param  return false;
    return isArrayHelp(t);
  }

  proc isArrayValue(e: []) param  return true;
  proc isArrayValue(e)     param  return false;

  
  /*
     The following functions define set operations on associative arrays.

     The supported functions (and their op= variations) are:
       + or |    Union
       &         Intersection
       -         Difference
       ^         Symmetric Difference

     Consider the following code where 'a' and 'b' are associative arrays:

     var c = a op b;

     The result 'c' is a new associative array backed by a new associative
     domain. The domains of 'a' and 'b' are not modified by 'op'.

     Consider the following code where 'a' and 'b' are associative arrays:

     a op= b;

     'a' must not share its domain with another array, otherwise the program
     will halt with an error message.
     
     For the += and |= operators, the value from 'b' will overwrite the
     existing value when indices overlap.
  */

  // promototion for associative array addition doesn't really make sense. instead,
  // we really just want a union
  proc +(a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    return a | b;
  }

  proc +=(ref a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    a.chpl__assertSingleArrayDomain("+=");
    a |= b;
  }

  proc |(a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    var newDom : a.domain.type;
    var ret : [newDom] a.eltType;
    serial !newDom._value.parSafe {
      forall (k,v) in zip(a.domain, a) do ret[k] = v;
      ret |= b;
    }
    return ret;
  }

  proc |=(ref a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    a.chpl__assertSingleArrayDomain("|=");
    serial !a.domain._value.parSafe do forall (k,v) in zip(b.domain, b) do a[k] = v;
  }

  proc &(a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    var newDom : a.domain.type;
    var ret : [newDom] a.eltType;

    serial !newDom._value.parSafe do
      forall k in a.domain do
        if b.domain.member(k) then ret[k] = a[k];
    return ret;
  }

  proc &=(ref a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    a.chpl__assertSingleArrayDomain("&=");
    serial !a.domain._value.parSafe do
      forall k in a.domain do
        if !b.domain.member(k) then a.domain.remove(k);
  }

  proc -(a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    var newDom : a.domain.type;
    var ret : [newDom] a.eltType;

    serial !newDom._value.parSafe do
      forall k in a.domain do
        if !b.domain.member(k) then ret[k] = a[k];

    return ret;
  }

  proc -=(ref a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    a.chpl__assertSingleArrayDomain("-=");
    serial !a.domain._value.parSafe do
      forall k in a.domain do
        if b.domain.member(k) then a.domain.remove(k);
  }


  proc ^(a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    var newDom : a.domain.type;
    var ret : [newDom] a.eltType;

    serial !newDom._value.parSafe {
      forall k in a.domain do
        if !b.domain.member(k) then ret[k] = a[k];
      forall k in b.domain do
        if !a.domain.member(k) then ret[k] = b[k];
    }

    return ret;
  }

  proc ^=(ref a :_array, b: _array) where (a._value.type == b._value.type) && isAssociativeArr(a) {
    a.chpl__assertSingleArrayDomain("^=");
    serial !a.domain._value.parSafe do
      forall k in b.domain do
        if a.domain.member(k) then a.domain.remove(k);
        else a[k] = b[k];
  }

  /*
     The following functions define set operations on associative domains.

     The supported functions are:
       + or |    Union
       &         Intersection
       -         Difference
       ^         Symmetric Difference

     For each operator, a new domain is returned.
  */

  proc -(a :domain, b :domain) where (a.type == b.type) && isAssociativeDom(a) {
    var newDom : a.type;
    serial !newDom._value.parSafe do
      forall e in a do
        if !b.member(e) then newDom.add(e);
    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  proc -=(ref a :domain, b :domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in b do
      if a.member(e) then
        a.remove(e);
  }
  
  proc |(a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    return a + b;
  }

  proc |=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in b do
      a.add(e);
  }

  proc +=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    a |= b;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  proc &(a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    var newDom : a.type;

    serial !newDom._value.parSafe do 
      forall k in a with (ref newDom) do // no race - in 'serial'
        if b.member(k) then newDom += k;
    return newDom;
  }

  proc &=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in a do
      if !b.member(e) then
        a.remove(e);
  }

  proc ^(a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    var newDom : a.type;

    serial !newDom._value.parSafe {
      forall k in a do
        if !b.member(k) then newDom.add(k);
      forall k in b do
        if !a.member(k) then newDom.add(k);
    }

    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, it is
     added to the LHS.
  */
  proc ^=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in a do
      if b.member(e) then
        a.remove(e);
      else
        a.add(e);
  }
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
  
  proc _getRankChangeRanges(args) {
    proc _tupleize(x) {
      var y: 1*x.type;
      y(1) = x;
      return y;
    }
    proc collectRanges(param dim: int) {
      if dim > args.size then
        compilerError("domain slice requires a range in at least one dimension");
      if isRange(args(dim)) then
        return collectRanges(dim+1, _tupleize(args(dim)));
      else
        return collectRanges(dim+1);
    }
    proc collectRanges(param dim: int, x: _tuple) {
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
    if a.rank != 1 && isRange(b) then
      return true;
  
    // Sparse and Opaque arrays do not yet support parallel iteration.  We
    // could let them fall through, but then we get multiple warnings for a
    // single assignment statement which feels like overkill
    //
    if ((!isRectangularArr(a) && !isAssociativeArr(a) && !isSparseArr(a)) ||
        (isArray(b) &&
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
  proc chpl__supportedDataTypeForBulkTransfer(x: ?t) param where isComplexType(t) return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: ?t) param where isRecordType(t) return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: ?t) param where isUnionType(t) return false;
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

    if a._value == b._value {
      // Do nothing for A = A but we could generate a warning here
      // since it is probably unintended. We need this check here in order
      // to avoid memcpy(x,x) which happens inside doiBulkTransfer.
      return;
    }

    if a.size == 0 && b.size == 0 then
      // Do nothing for zero-length assignments
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
