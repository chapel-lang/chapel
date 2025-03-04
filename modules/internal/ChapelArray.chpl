/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

  public use ChapelBase;
  use ChapelTuple;
  use ChapelLocale;
  use ArrayViewSlice;
  use ArrayViewRankChange;
  use ArrayViewReindex;
  import Reflection;
  use ChapelDebugPrint;
  use CTypes;
  use ChapelPrivatization;
  use ChplConfig only compiledForSingleLocale, CHPL_LOCALE_MODEL;
  use ChapelArrayViewElision;
  use ChapelNumLocales;
  public use ChapelDomain;

  // Explicitly use a processor atomic, as most calls to this function are
  // likely be on locale 0
  @chpldoc.nodoc
  var numPrivateObjects: chpl__processorAtomicType(int);
  @chpldoc.nodoc
  param nullPid = -1;

  @chpldoc.nodoc
  config param debugBulkTransfer = false;
  @chpldoc.nodoc
  config param useBulkTransfer = true;
  @chpldoc.nodoc
  config param useBulkTransferStride = true;
  @chpldoc.nodoc
  config param useBulkPtrTransfer = useBulkTransfer;

  @chpldoc.nodoc
  config param disableConstDomainOpt = false;

  @chpldoc.nodoc
  config param debugOptimizedSwap = false;

  // Return POD values from arrays as values instead of const ref?
  @chpldoc.nodoc
  config param PODValAccess = true;

  // Toggles the functionality to perform strided bulk transfers involving
  // distributed arrays.
  //
  // Currently disabled due to observations of higher communication counts
  // compared to element-by-element assignment.
  @chpldoc.nodoc
  config param useBulkTransferDist = false;

  @chpldoc.nodoc // no doc unless we decide to expose this
  config param arrayAsVecGrowthFactor = 1.5;
  @chpldoc.nodoc
  config param debugArrayAsVec = false;

  @chpldoc.nodoc
  config param logDistArrEltAccess = false;
  @chpldoc.nodoc
  config param logAllArrEltAccess = false;

  proc _isPrivatized(value) param do
    return (!compiledForSingleLocale() || CHPL_LOCALE_MODEL=="gpu") &&
           ((_privatization && value!.dsiSupportsPrivatization()) ||
            value!.dsiRequiresPrivatization());
    // _privatization is controlled by --[no-]privatization
    // privatization required, not optional, for PrivateDist

  // MPF 2016-10-02: This simple implementation of privatization has some
  // drawbacks:
  // 1) Creating a new privatized object necessarily does something on all
  //    locales; this would be surprising if the user explicitly requested a
  //    Block array on 2 locales for example.
  // 2) Privatized object ids are managed by Locale 0 in a way that, while
  //    relatively low overhead, adds work to Locale 0 that is not present on
  //    the other locales, and again would be surprising if a Block array were
  //    created over other locales only (say, Locales[2] and Locales[3]).

  // Given a dsi Dist/Dom/Array, create an pid integer identifying the
  // privatized version on all locales; and populate each locale
  // with a privatized value that can be retrieved by the pid
  // without communication.
  proc _newPrivatizedClass(value) : int {
    const pid = numPrivateObjects.fetchAdd(1);
    const hereID = here.id;
    const privatizeData = value.dsiGetPrivatizeData();
    const definedConst = if isSubtype(value.type, BaseDom) then value.definedConst
                                                           else none;

    coforall loc in Locales do on loc {
      if hereID != here.id {
        var newValue = value.dsiPrivatize(privatizeData);
        if isSubtype(value.type, BaseDom) {
          newValue.definedConst = definedConst;
        }
        __primitive("chpl_newPrivatizedClass", newValue, pid);
        newValue.pid = pid;
      } else {
        __primitive("chpl_newPrivatizedClass", value, pid);
        value.pid = pid;
      }
    }
    return pid;
  }

  // original is the value this method shouldn't free, because it's the
  // canonical version. The rest are copies on other locales.
  proc _freePrivatizedClass(pid:int, original:RootClass) : void  {
    // Do nothing for null pids.
    if pid == nullPid then return;

    coforall loc in Locales do on loc {
      var prv = chpl_getPrivatizedCopy(unmanaged RootClass, pid);
      if prv != original then
        delete prv;

      extern proc chpl_clearPrivatizedClass(pid:int);
      chpl_clearPrivatizedClass(pid);

    }
  }

  proc _reprivatize(value) : void {
    const pid = value.pid;
    const hereID = here.id;
    const reprivatizeData = value.dsiGetReprivatizeData();

    coforall loc in Locales do on loc {
      var newValue = value;
      if hereID != here.id {
        newValue = chpl_getPrivatizedCopy(newValue.type, pid);
        newValue.dsiReprivatize(value, reprivatizeData);
      }
    }
  }

  //
  // Take a rank and value and check that the value is a rank-tuple or not a
  // tuple. If the value is not a tuple and expand is true, copy the value into
  // a rank-tuple. If the value is a scalar and rank is 1, copy it into a 1-tuple.
  //
  proc _makeIndexTuple(param rank, t: _tuple, param concept: string,
                       param expand: bool=false) {
   if rank == t.size then
    return t;
   else
    compilerError("rank of the ", concept, " must match domain rank");
  }

  proc _makeIndexTuple(param rank, val:integral, param concept: string,
                       param expand: bool=false) {
    if expand || rank == 1 {
      var t: rank*val.type;
      for param i in 0..rank-1 do
        t(i) = val;
      return t;
    } else {
      compilerWarning(concept, " is of type ", val.type:string);
      compilerError("rank of the ", concept, " must match domain rank");
      return val;
    }
  }

  pragma "last resort"
  proc _makeIndexTuple(param rank, val, param concept: string,
                       param expand: bool=false) {
    compilerError("cannot use ", concept, " of type ", val.type:string);
  }

  pragma "no copy return"
  proc _newArray(value) {
    if _isPrivatized(value) then
      return new _array(_newPrivatizedClass(value), value);
    else
      return new _array(nullPid, value);
  }

  // It is intentional that there is no _getArray.
  // It would have implications for alias analysis
  // of arrays.

  proc _getDistribution(value) {
    if _isPrivatized(value) then
      return new _distribution(value.pid, value, _unowned=true);
    else
      return new _distribution(nullPid, value, _unowned=true);
  }

  // Run-time type support
  //
  // NOTE: the bodies of functions marked with runtime type init fn (such as
  // chpl__buildArrayRuntimeType) are replaced by the compiler to just create a
  // record storing the arguments.  The return type of
  // chpl__build...RuntimeType is what tells the compiler which runtime type it
  // is creating. These functions are written to return a value even though
  // they are marked as type functions.

  //
  // Support for array types
  //
  pragma "runtime type init fn"
  proc chpl__buildArrayRuntimeType(dom: domain, type eltType) type {
    return dom.buildArray(eltType, false);
  }

  pragma "no copy returns owned" // workaround for order of resolution issue
  proc chpl__convertRuntimeTypeToValue(dom: domain,
                                       type eltType,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return dom.buildArray(eltType, !isNoInit);
  }

  proc chpl__convertValueToRuntimeType(arr: []) type {
    return chpl__buildArrayRuntimeType(arr.domain, arr.eltType);
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
  @chpldoc.nodoc
  config param CHPL_WARN_DOMAIN_LITERAL = "unset";

  // the low bound to use for array literals
  @chpldoc.nodoc
  config param arrayLiteralLowBound = defaultLowBound;
  @chpldoc.nodoc
  config param capturedIteratorLowBound = defaultLowBound;

  pragma "ignore transfer errors"
  proc chpl__buildArrayExpr( pragma "no auto destroy" in elems ...?k ) {

    if CHPL_WARN_DOMAIN_LITERAL == "true" && isRange(elems(0)) {
      compilerWarning("Encountered an array literal with range element(s).",
                      " Did you mean a domain literal here?",
                      " If so, use {...} instead of [...].");
    }

    param homog = isHomogeneousTuple(elems);

    // elements of string literals are assumed to be of type string
    type eltType = if homog then elems(0).type
                            else chpl_computeUnifiedType(elems);

    var dom = {arrayLiteralLowBound..#k};
    var arr = dom.buildArray(eltType, initElts=false);

    if homog {
      for i in 0..<k {
        ref dst = arr(i+arrayLiteralLowBound);
        ref src = elems(i);
        __primitive("=", dst, src);
      }
    } else {
      for param i in 0..k-1 {
        ref dst = arr(i+arrayLiteralLowBound);
        ref src = elems(i);
        type currType = src.type;

        if (currType == eltType ||
            Reflection.canResolve("=", dst, src)) {
          __primitive("=", dst, src);
        } else {
          compilerError( "Array literal element " + i:string +
                         " expected to be of type " + eltType:string +
                         " but is of type " + currType:string );
        }
      }
    }

    arr.dsiElementInitializationComplete();

    return arr;
  }

  // For a given tuple, compute whether it has a potential unified
  // type by leaning on return type unification via the following
  // helper routine.  Ultimately, it should be the compiler doing this
  // rather than this module code, but this is a nice short-term
  // workaround until 'dyno' is ready for it.
  proc chpl_computeUnifiedType(x: _tuple) type {
    if isHomogeneousTuple(x) {
      return x(0).type;
    } else {
      return chpl_computeUnifiedTypeHelp(x).type;
    }
  }

  // For a given tuple, set up a return per element in order to
  // leverage return type unification to determine whether there is a
  // common type that can be used.  Note that the purpose of the
  // seemingly unused 'j' argument is to avoid having the compiler
  // fold it down to a single return statement if we relied on a param
  // check against 0.
  pragma "compute unified type helper"
  proc chpl_computeUnifiedTypeHelp(x: _tuple, j: int=0) {
    for param i in 0..<x.size {
      if i == j then
        return x(i);
    }
    halt("Should never get here");
  }

  proc chpl__buildAssociativeArrayExpr( const elems ...?k ) {
    type keyType = elems(0).type;
    type valType = elems(1).type;
    var D : domain(keyType);

    //Size the domain appropriately for the number of keys
    //This prevents expensive resizing as keys are added.
    // Note that k/2 is the number of keys, since the tuple
    // passed to this function has 2 elements (key and value)
    // for each array element.
    D.requestCapacity(k/2);
    var A : [D] valType;

    for param i in 0..k-1 by 2 {
      var elemKey = elems(i);
      var elemVal = elems(i+1);
      type elemKeyType = elemKey.type;
      type elemValType = elemVal.type;

      if elemKeyType != keyType {
        compilerError("Associative array key element " + (i/2):string +
                       " expected to be of type " + keyType:string +
                       " but is of type " + elemKeyType:string);
      }

      if elemValType != valType {
        compilerError("Associative array value element " + (i/2):string
                      + " expected to be of type " + valType:string
                      + " but is of type " + elemValType:string);
      }

      D += elemKey;
      A[elemKey] = elemVal;
    }

    return A;
  }

  //
  // These routines increment and decrement the reference count
  // for a domain that is part of an array's element type.
  // Prior to introducing these routines and calls, we would
  // increment/decrement the reference count based on the
  // number of indices in the outer domain instead; this could
  // cause the domain to be deallocated prematurely in the
  // case the outer domain was empty.  For example:
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
  proc chpl_incRefCountsForDomainsInArrayEltTypes(arr:unmanaged BaseArr, type eltType) {
    if isArrayType(eltType) {
      arr._decEltRefCounts = true;

      type arrayEltType = chpl__eltTypeFromArrayRuntimeType(eltType);
      const ref dom = chpl__domainFromArrayRuntimeType(eltType);
      const dv = dom._instance;
      dv.add_containing_arr(arr);
      chpl_incRefCountsForDomainsInArrayEltTypes(arr, arrayEltType);
    }
  }

  proc chpl_decRefCountsForDomainsInArrayEltTypes(arr:unmanaged BaseArr, type eltType) {
    if isArrayType(eltType) {
      if arr._decEltRefCounts == false then
        halt("Decrementing array's elements' ref counts without having incremented first!");

      type arrayEltType = chpl__eltTypeFromArrayRuntimeType(eltType);
      const ref dom = chpl__domainFromArrayRuntimeType(eltType);

      chpl_decRefCountsForDomainsInArrayEltTypes(arr, arrayEltType);
      var removeDom = dom._instance.remove_containing_arr(arr);
      if removeDom {
        on dom._instance {
          const inst = dom._instance;
          var (domToFree, distToRemove) = inst.remove();
          var distToFree:unmanaged BaseDist? = nil;
          if distToRemove != nil {
            distToFree = distToRemove!.remove();
          }
          if domToFree != nil then
            _delete_dom(inst, _isPrivatized(inst));
          if distToFree != nil then
            _delete_dist(distToFree!, _isPrivatized(inst.dist));
        }
      }
    }
  }

  //
  // Support for distributed domain expression e.g. {1..3, 1..3} dmapped Dist()
  //
  proc chpl__distributed(d, dom: domain,
                         definedConst: bool) {
    if definedConst {
      if dom.isRectangular() {
        const distDom = new _domain(d.newRectangularDom(
                              dom.rank,
                              dom._value.idxType,
                              dom._value.strides,
                              dom.dims(),
                              definedConst));
        return distDom;
      } else {
        const distDom: domain(dom._value.idxType) dmapped d = dom;
        return distDom;
      }
    }
    else {
      if dom.isRectangular() {
        var distDom = new _domain(d.newRectangularDom(
                              dom.rank,
                              dom._value.idxType,
                              dom._value.strides,
                              dom.dims(),
                              definedConst));

        return distDom;
      } else {
        var distDom: domain(dom._value.idxType) dmapped d = dom;
        return distDom;
      }
    }
  }

  pragma "last resort"
  proc chpl__distributed(d, expr, definedConst: bool) {
    compilerError("'dmapped' can currently only be applied to domains.");
  }

  //
  // Array-view utility functions
  //
  proc chpl__isArrayView(arr) param {
    const value = if isArray(arr) then arr._value else arr;

    param isSlice      = value.isSliceArrayView();
    param isRankChange = value.isRankChangeArrayView();
    param isReindex    = value.isReindexArrayView();

    return isSlice || isRankChange || isReindex;
  }

  //
  // Return the innermost array class (e.g., a DefaultRectangular).
  //
  // 'arr' can be a full-fledged array or a BaseArr-inheriting class
  //
  proc chpl__getActualArray(arr) {
    var value = if isArray(arr) then arr._value else arr;
    var ret = if chpl__isArrayView(value) then value._getActualArray() else value;
    return ret;
  }

  //
  // Return true if 'arg' is a DefaultRectangular array or domain
  // or is an ArrayView over a DefaultRectangular array or domain.
  //
  // 'arg' can be a full-fledged array/domain type or a class that inherits
  // from BaseArr or BaseDom
  //
  proc chpl__isDROrDRView(arg) param {
    if isDomain(arg) || isSubtype(arg.type, BaseDom) {
      const value = if isDomain(arg) then arg._value else arg;
      param isDR  = value.isDefaultRectangular();
      param isDRView = chpl__isDomainView(value) && chpl__getActualDomain(value).isDefaultRectangular();
      return isDR || isDRView;
    } else if isArray(arg) || isSubtype(arg.type, BaseArr) {
      const value = if isArray(arg) then arg._value else arg;
      param isDR = value.isDefaultRectangular();
      param isDRView = chpl__isArrayView(value) && chpl__getActualArray(value).isDefaultRectangular();
      return isDR || isDRView;
    } else {
      return false;
    }
  }

  //
  // End of array-view utility functions
  //

  //
  // DomainView utility functions
  //
  proc chpl__isDomainView(dom) param {
    const value = if isDomain(dom) then dom._value else dom;

    param isSlice      = value.isSliceDomainView();
    param isRankChange = value.isRankChangeDomainView();
    param isReindex    = value.isReindexDomainView();

    return isSlice || isRankChange || isReindex;
  }

  proc chpl__getActualDomain(dom) {
    var value = if isDomain(dom) then dom._value else dom;
    var ret = if chpl__isDomainView(value) then value._getActualDomain() else value;
    return ret;
  }

  // note: chpl__isDROrDRView defined above also applies to domains

  //
  // End of DomainView utility functions
  //
  // this is a type function and as such, definedConst has no effect
  proc chpl__distributed(d, type domainType,
                         definedConst: bool) type {
    if !isDomainType(domainType) then
      compilerError("cannot apply 'dmapped' to the non-domain type ",
                    domainType:string);
    if chpl__isRectangularDomType(domainType) {
      var dom: domainType;
      return chpl__buildDomainRuntimeType(d, dom._value.rank, dom._value.idxType,
                                          dom._value.strides);
    } else if chpl__isSparseDomType(domainType) {
      const ref parentDom = chpl__parentDomainFromDomainRuntimeType(domainType);
      return chpl__buildSparseDomainRuntimeType(d, parentDom);
    } else {
      var dom: domainType;
      return chpl__buildDomainRuntimeType(d, dom._value.idxType, dom._value.parSafe);
    }
  }

  pragma "return not owned"
  proc chpl__distributionFromDomainRuntimeType(type rtt) {
    pragma "no copy"
    pragma "no auto destroy"
    var dist = __primitive("get runtime type field", rtt, "dist");

    return _getDistribution(dist._value);
  }

  proc chpl__eltTypeFromArrayRuntimeType(type rtt) type {
    pragma "no copy"
    pragma "no auto destroy"
    type eltType = __primitive("get runtime type field", rtt, "eltType");

    return eltType;
  }

  pragma "ignore runtime type"
  proc chpl__instanceTypeFromArrayRuntimeType(type rtt) type {
    // this function is compile-time only and should not be run
    __primitive("chpl_warning",
                "chpl__instanceTypeFromArrayRuntimeType should not be run");
    return __primitive("static field type", rtt, "_instance");
  }

  pragma "ignore runtime type"
  proc chpl__instanceTypeFromDomainRuntimeType(type rtt) type {
    // this function is compile-time only and should not be run
    __primitive("chpl_warning",
                "chpl__instanceTypeFromArrayRuntimeType should not be run");
    return __primitive("static field type", rtt, "_instance");
  }

  //
  // Support for index types
  //
  proc chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
    return idxType;
  }

  proc chpl__buildIndexType(param rank: int, type idxType) type where rank > 1 {
    return rank*idxType;
  }

  proc chpl__buildIndexType(param rank: int) type do
    return chpl__buildIndexType(rank, int);

  proc chpl__buildIndexType(d: domain) type do
    return chpl__buildIndexType(d.rank, d._value.idxType);

  // Helper function used to ensure a returned array matches the declared
  // return type when the declared return type specifies a particular element
  // type but not the domain
  proc chpl__checkRetEltTypeMatch(a: [], type b) {
    if (a.eltType != b) {
      compilerError("array element type mismatch in return from ",
                    a.eltType: string,
                    " to ",
                    b: string);
    }
  }

  proc chpl__checkRetEltTypeMatch(a: _iteratorRecord, type b) {
    type eltType = iteratorToArrayElementType(a.type);
    if (eltType != b) {
      compilerError("array element type mismatch in return from ",
                    eltType: string,
                    " to ",
                    b: string);
    }
  }
  proc chpl__checkOutEltTypeMatch(a: [], type b) {
    if (a.eltType != b) {
      compilerError("array element type mismatch in initializing out formal ",
                    "with element type ",
                    b: string,
                    " from array with element type ",
                    a.eltType: string);
    }
  }

  proc chpl__checkOutEltTypeMatch(a: _iteratorRecord, type b) {
    type eltType = iteratorToArrayElementType(a.type);
    if (eltType != b) {
      compilerError("array element type mismatch in initializing out formal ",
                    "with element type ",
                    b: string,
                    " from iterator with element type ",
                    eltType: string);
    }
  }

  //
  // Support for distributions
  //
  pragma "syntactic distribution"
  @chpldoc.nodoc
  @unstable("the type 'dmap' is unstable, instead please use distribution factory functions when available")
  record dmap { }

  proc chpl__buildDistType(type t) type where isSubtype(_to_borrowed(t), BaseDist) {
    var x: _to_unmanaged(t)?;
    var y = new _distribution(x!);
    return y.type;
  }

  proc chpl__buildDistType(type t: record) type {
    compilerWarning("The use of 'dmap' is deprecated for this distribution; please replace 'dmap(<DistName>(<args>))' with '<DistName>(<args>)'");
    return t;
  }

  proc chpl__buildDistType(type t) {
    compilerError("illegal domain map type specifier - must be a subclass of BaseDist");
  }

  proc chpl__buildDistValue(x:unmanaged) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(x);
  }
  proc chpl__buildDistValue(in x:owned) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(owned.release(x));
  }
  proc chpl__buildDistValue(const ref x:record) const ref {
    return x;
  }

  proc chpl__buildDistValue(x) {
    compilerError("illegal domain map value specifier - must be a subclass of BaseDist");
  }

  proc chpl__buildDistDMapValue(const ref x: record) const ref {
    compilerWarning("The use of 'dmap' is deprecated for this distribution; please replace 'new dmap(new <DistName>(<args>))' with 'new <DistName>(<args>)'");
    return chpl__buildDistValue(x);
  }

  proc chpl__buildDistDMapValue(x:unmanaged) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(x);
  }
  proc chpl__buildDistDMapValue(in x:owned) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(owned.release(x));
  }


  @chpldoc.nodoc
  inline operator ==(d1: _distribution(?), d2: _distribution(?)) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  // This is temporary while we work on retiring _distribution
  @chpldoc.nodoc
  inline operator ==(d1: _distribution(?), d2: record) param {
    return false;
  }

  // This is temporary while we work on retiring _distribution
  @chpldoc.nodoc
  inline operator ==(d1: record, d2: _distribution(?)) param {
    return false;
  }

  @chpldoc.nodoc
  inline operator !=(d1: _distribution(?), d2: _distribution(?)) {
    return !(d1 == d2);
  }

  @chpldoc.nodoc
  inline operator !=(d1: _distribution(?), d2: record) param {
    return true;
  }

  @chpldoc.nodoc
  inline operator !=(d1: record, d2: _distribution(?)) param {
    return true;
  }

  // This alternative usage of Sort.defaultComparator
  // prevents transitive use of module Sort.
  proc chpl_defaultComparator() {
    use Sort;
    return new defaultComparator();
  }

  @chpldoc.nodoc
  proc shouldReturnRvalueByValue(type t) param {
    if !PODValAccess then return false;
    if isPODType(t) then return true;
    return false;
  }

  // Array wrapper record
  pragma "array"
  pragma "has runtime type"
  pragma "ignore noinit"
  pragma "default intent is ref if modified"
  // this has always been true, but hard-coded into the compiler; here
  // we put it into the module code to remove a special-case and force
  // the serialize routines to fire, when their where-clause permits.
  pragma "always RVF"
  /* The array type */
  record _array : writeSerializable, readDeserializable {
    var _pid:int;  // only used when privatized
    pragma "owned"
    pragma "alias scope from this"
    var _instance; // generic, but an instance of a subclass of BaseArr
    var _unowned:bool;

    proc chpl__serialize() where _instance.chpl__rvfMe() {
      return _instance.chpl__serialize();
    }

    pragma "no copy return"
    proc type chpl__deserialize(data) {
      var arrinst = _to_borrowed(__primitive("static field type", this, "_instance")).chpl__deserialize(data);
      return new _array(nullPid, arrinst, _unowned=true);
    }

    proc chpl__promotionType() type {
      return _value.eltType;
    }

    pragma "alias scope from this"
    inline proc _value {
      if _isPrivatized(_instance) {
        return chpl_getPrivatizedCopy(_instance.type, _pid);
      } else {
        return _instance;
      }
    }

    forwarding _value except doiBulkTransferFromKnown, doiBulkTransferToKnown,
                             doiBulkTransferFromAny,  doiBulkTransferToAny,
                             chpl__serialize, chpl__deserialize;

    @chpldoc.nodoc
    proc deinit() {
      _do_destroy_array(this);
    }

    /* The type of elements contained in the array */
    proc eltType type do return _value.eltType;

    /* The type used to represent the array's indices.  For a
       multidimensional array, this is the per-dimension type used. */
    proc idxType type do return _value.idxType;

    /* The type used to represent the array's indices.  For a
       1-dimensional or associative array, this will be the same as
       :proc:`idxType` above.  For a multidimensional array, it will be
       :proc:`rank` * :proc:`idxType`. */
    proc fullIdxType type do return this.domain.fullIdxType;

    pragma "no copy return"
    pragma "return not owned"
    proc _dom do return _getDomain(_value.dom);

    /* The number of dimensions in the array */
    proc rank param do return this.domain.rank;

    /* The strides value of the underlying domain */
    proc strides param do return this.domain.strides;

    @chpldoc.nodoc proc hasUnitStride() param do return strides.isOne();
    @chpldoc.nodoc proc hasPosNegUnitStride() param do return strides.isPosNegOne();

    /*
      Return a dense rectangular array's indices as a default domain.
    */
    proc indices where !this.isSparse() && !this.isAssociative() {
      return {(..._dom.getIndices())};
    }

    /*
      Yield an irregular array's indices.
    */
    iter indices where (this.isSparse() || this.isAssociative()) {
      for i in _dom do
        yield i;
    }

    // bounds checking helpers
    pragma "insert line file info"
    pragma "always propagate line file info"
    @chpldoc.nodoc
    proc checkAccess(indices, value) {
      if this.isRectangular() {
        if !value.dsiBoundsCheck(indices) {
          if rank == 1 {
            halt("array index out of bounds\n",
                 "note: index was ", indices(0),
                 " but array bounds are ", value.dom.dsiDim(0));
          } else {
            var istr = "";
            var bstr = "";
            for param i in 0..rank-1 {
              if i != 0 {
                istr += ", ";
                bstr += ", ";
              }
              istr += indices(i):string;
              bstr += value.dom.dsiDim(i):string;
            }
            var dimstr = "";
            for param i in 0..rank-1 {
              if !value.dom.dsiDim(i).contains(indices(i)) {
                if dimstr == "" {
                  dimstr = "out of bounds in dimension " + i:string +
                           " because index " + indices(i):string +
                           " is not in " + value.dom.dsiDim(i):string;
                }
              }
            }
            halt("array index out of bounds\n",
                 "note: index was (", istr, ") ",
                 "but array bounds are (", bstr, ")\n",
                 "note: ", dimstr);
          }
        }
      }
    }

    pragma "insert line file info"
    pragma "always propagate line file info"
    @chpldoc.nodoc
    proc checkSlice(d: domain, value) {
      if (d.isRectangular() || d.isSparse()) {
        checkSlice((...d.dsiDims()), value=value);
      } else if (d.isAssociative()) {
        use HaltWrappers;
        forall i in d do
          if !this.domain.dsiMember(i) then
            HaltWrappers.boundsCheckHalt("index '" + i:string + "' is out-of-bounds");
      } else {
        compilerError("This style of array slicing is not yet supported");
      }
    }

    pragma "insert line file info"
    pragma "always propagate line file info"
    @chpldoc.nodoc
    proc checkSlice(ranges...rank, value) where chpl__isTupleOfRanges(ranges) {
      if this.isRectangular() {
        var ok = true;
        for param i in 0..rank-1 {
          ok &&= value.dom.dsiDim(i).chpl_boundsCheck(ranges(i));
        }
        if ok == false {
          if rank == 1 {
            halt("array slice out of bounds\n",
                 "note: slice index was ", ranges(0),
                 " but array bounds are ", value.dom.dsiDim(0));
          } else {
            var istr = "";
            var bstr = "";
            for param i in 0..rank-1 {
              if i != 0 {
                istr += ", ";
                bstr += ", ";
              }
              istr += ranges(i):string;
              bstr += value.dom.dsiDim(i):string;
            }
            var dimstr = "";
            for param i in 0..rank-1 {
              if !value.dom.dsiDim(i).chpl_boundsCheck(ranges(i)) {
                if dimstr == "" {
                  dimstr = "out of bounds in dimension " + i:string +
                           " because slice index " + ranges(i):string +
                           " is not in " + value.dom.dsiDim(i):string;
                }
              }
            }
            halt("array slice out of bounds\n",
                 "note: slice index was (", istr, ") ",
                 "but array bounds are (", bstr, ")\n",
                 "note: ", dimstr);
          }
        }
      }
    }

    // array element access
    // When 'this' is 'const', so is the returned l-value.

    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "removable array access"
    pragma "alias scope from this"
    @chpldoc.nodoc // ref version
    inline proc ref this(const i: rank*_value.dom.idxType) ref {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("default _array accessor was called");

      if this.isRectangular() || this.isSparse() then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }
    pragma "alias scope from this"
    @chpldoc.nodoc // value version, for POD types
    inline proc const this(const i: rank*_value.dom.idxType)
    where shouldReturnRvalueByValue(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("default _array accessor was called");

      if this.isRectangular() || this.isSparse() then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }
    pragma "alias scope from this"
    @chpldoc.nodoc // const ref version, for not-POD types
    inline proc const this(const i: rank*_value.dom.idxType) const ref
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("default _array accessor was called");

      if this.isRectangular() || this.isSparse() then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }

    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "removable array access"
    pragma "alias scope from this"
    @chpldoc.nodoc // ref version
    inline proc ref this(const i: _value.dom.idxType ...rank) ref do
      return this(i);

    pragma "alias scope from this"
    @chpldoc.nodoc // value version, for POD types
    inline proc const this(const i: _value.dom.idxType ...rank)
    where shouldReturnRvalueByValue(_value.eltType) do
      return this(i);

    pragma "alias scope from this"
    @chpldoc.nodoc // const ref version, for not-POD types
    inline proc const this(const i: _value.dom.idxType ...rank) const ref do
      return this(i);


    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "alias scope from this"
    @chpldoc.nodoc // ref version
    inline proc ref localAccess(i: rank*_value.dom.idxType) ref
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("local _array accessor was called");

      if chpl_isNonDistributedArray() then
        return this(i);
      else
        if this.isRectangular() || this.isSparse() then
          return value.dsiLocalAccess(i);
        else
          return value.dsiLocalAccess(i(0));
    }
    pragma "alias scope from this"
    @chpldoc.nodoc // value version, for POD types
    inline proc const localAccess(i: rank*_value.dom.idxType)
    where shouldReturnRvalueByValue(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("local _array accessor was called");

      if chpl_isNonDistributedArray() then
        return this(i);
      else
        if this.isRectangular() || this.isSparse() then
          return value.dsiLocalAccess(i);
        else
          return value.dsiLocalAccess(i(0));
    }
    pragma "alias scope from this"
    @chpldoc.nodoc // const ref version, for not-POD types
    inline proc const localAccess(i: rank*_value.dom.idxType) const ref
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if logAllArrEltAccess ||
        (logDistArrEltAccess && !chpl_isNonDistributedArray()) then
        chpl_debug_writeln("local _array accessor was called");

      if chpl_isNonDistributedArray() then
        return this(i);
      else
        if this.isRectangular() || this.isSparse() then
          return value.dsiLocalAccess(i);
        else
          return value.dsiLocalAccess(i(0));
    }

    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "alias scope from this"
    @chpldoc.nodoc // ref version
    inline proc ref localAccess(i: _value.dom.idxType ...rank) ref do
      return localAccess(i);

    pragma "alias scope from this"
    @chpldoc.nodoc // value version, for POD types
    inline proc const localAccess(i: _value.dom.idxType ...rank)
    where shouldReturnRvalueByValue(_value.eltType) do
      return localAccess(i);

    pragma "alias scope from this"
    @chpldoc.nodoc // const ref version, for not-POD types
    inline proc const localAccess(i: _value.dom.idxType ...rank) const ref do
      return localAccess(i);


    // array slicing by a domain
    //
    // requires dense domain implementation that returns a tuple of
    // ranges via the getIndices() method; domain indexing is difficult
    // in the domain case because it has to be implemented on a
    // domain-by-domain basis; this is not terribly difficult in the
    // dense case because we can represent a domain by a tuple of
    // ranges, but in the sparse case, is there a general representation?
    //
    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc this(d: domain) {
      if d.rank != rank then
        compilerError("slicing an array with a domain of a different rank");

      if boundsChecking then
        checkSlice(d, _value);
      d.chpl_checkNegativeStride();

      //
      // If this is already a slice array view, we can short-circuit
      // down to the underlying array.
      //
      const (arr, arrpid) = if (_value.isSliceArrayView())
                              then (this._value.arr, this._value._ArrPid)
                              else (this._value, this._pid);

      var a = new unmanaged ArrayViewSliceArr(eltType=this.eltType,
                                              _DomPid=d._pid,
                                              dom=d._instance,
                                              _ArrPid=arrpid,
                                              _ArrInstance=arr);

      // lock since we're referring to an existing domain; but don't
      // add this array to the domain's list of arrays since resizing
      // a slice's domain shouldn't generate a reallocate call for the
      // underlying array
      d._value.add_arr(a, locking=true, addToList=false);
      return _newArray(a);
    }

    // array slicing by a tuple of ranges
    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc this(ranges...rank) where chpl__isTupleOfRanges(ranges) {
      if boundsChecking then
        checkSlice((... ranges), value=_value);

      pragma "no auto destroy" var d = _dom((...ranges));
      d._value._free_when_no_arrs = true;

      // this domain is not exposed to the user in any way, so it is actually
      // constant
      d._value.definedConst = true;
      d.chpl_checkNegativeStride();

      //
      // If this is already a slice array view, we can short-circuit
      // down to the underlying array.
      //
      const (arr, arrpid) = if (_value.isSliceArrayView())
                              then (this._value.arr, this._value._ArrPid)
                              else (this._value, this._pid);

      var a = new unmanaged ArrayViewSliceArr(eltType=this.eltType,
                                    _DomPid=d._pid,
                                    dom=d._instance,
                                    _ArrPid=arrpid,
                                    _ArrInstance=arr);

      // this doesn't need to lock since we just created the domain d
      // don't add this array to the domain's list of arrays since
      // resizing a slice's domain shouldn't generate a reallocate
      // call for the underlying array
      d._value.add_arr(a, locking=false, addToList=false);
      return _newArray(a);
    }

    // array rank change
    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc this(args ...rank) where _validRankChangeArgs(args, _value.dom.idxType) {
      if boundsChecking then
        checkRankChange(args);

      // as we are making this a "no copy", we won't get chpl__initCopy, and
      // thus no chance to adjust constness. So, define the variable var, but
      // set its constness manually
      pragma "no copy"
      var rcdom = this.domain[(...args)];
      rcdom.definedConst = true;
      rcdom.chpl_checkNegativeStride();

      // TODO: With additional effort, we could collapse rank changes of
      // rank-change array views to a single array view, similar to what
      // we do for slices.
      const (arr, arrpid)  = (this._value, this._pid);

      var a = new unmanaged ArrayViewRankChangeArr(eltType=this.eltType,
                                         _DomPid = rcdom._pid,
                                         dom = rcdom._instance,
                                         _ArrPid=arrpid,
                                         _ArrInstance=arr,
                                         // TODO: Should the array really store
                                         // these redundantly?
                                         collapsedDim=rcdom._value.collapsedDim,
                                         idx=rcdom._value.idx,
                                         ownsArrInstance=false);

      // this doesn't need to lock since we just created the domain d
      rcdom._value.add_arr(a, locking=false);
      return _newArray(a);
    }

    /*
       Return a tuple of ranges describing the bounds of a rectangular domain.
       For a sparse domain, return the bounds of the parent domain.
     */
    proc dims() do return this.domain.dims();

    /*
       Return a range representing the boundary of this
       domain in a particular dimension.
     */
    proc dim(d : int) {
      return this.domain.dim(d);
    }

    @chpldoc.nodoc
    proc dim(param d : int) {
      return this.domain.dim(d);
    }

    @chpldoc.nodoc
    proc checkRankChange(args) {
      for param i in 0..args.size-1 do
        if !_value.dom.dsiDim(i).chpl_boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
    }

    // Special cases of local slices for DefaultRectangularArrs because
    // we can't take an alias of the ddata class within that class
    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc localSlice(r... rank)
    where chpl__isDROrDRView(this) && chpl__isTupleOfRanges(r) {
      if boundsChecking then
        checkSlice((...r), value=_value);
      var dom = _dom((...r));
      return chpl__localSliceDefaultArithArrHelp(dom);
    }

    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc localSlice(d: domain) where chpl__isDROrDRView(this) {
      if boundsChecking then
        checkSlice((...d.getIndices()), value=_value);

      return chpl__localSliceDefaultArithArrHelp(d);
    }

    @unstable("tryCopy() is subject to change in the future.")
    proc tryCopy() throws {
      use Reflection;
      if !(__primitive("resolves", this.domain.
                       tryCreateArray(this.eltType))) then
        compilerError("cannot call 'tryCopy' on arrays that do not" +
                      " support a 'tryCreateArray' method.");
      var res = this.domain.tryCreateArray(this.eltType);
      res = this;
      return res;
    }

    pragma "no copy return"
    proc chpl__localSliceDefaultArithArrHelp(d: domain) {
      if (_value.locale != here) then
        halt("Attempting to take a local slice of an array on locale ",
             _value.locale.id, " from locale ", here.id);
      return this(d);
    }
    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc localSlice(r... rank)
    where chpl__isTupleOfRanges(r) && !chpl__isDROrDRView(this) {
      if boundsChecking then
        checkSlice((...r), value=_value);
      return _value.dsiLocalSlice(r);
    }

    pragma "no promotion when by ref"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }

    /* Yield the array elements */
    pragma "reference to const when const this"
    iter these() ref {
      for i in _value.these() {
        yield i;
      }
    }

    pragma "reference to const when const this"
    @chpldoc.nodoc
    iter these(param tag: iterKind) ref
      where tag == iterKind.standalone &&
            __primitive("resolves", _value.these(tag=tag)) {
      for i in _value.these(tag) do
        yield i;
    }
    @chpldoc.nodoc
    iter these(param tag: iterKind)
      where tag == iterKind.leader {
      for followThis in _value.these(tag) do
        yield followThis;
    }
    pragma "reference to const when const this"
    @chpldoc.nodoc
    iter these(param tag: iterKind, followThis, param fast: bool = false) ref
      where tag == iterKind.follower {
      if __primitive("resolves", _value.these(tag=tag, followThis, fast=fast)) {
        for i in _value.these(tag=tag, followThis, fast=fast) do
          yield i;
      } else {
        for i in _value.these(tag, followThis) do
          yield i;
      }
    }

    /* Return the number of elements in the array */
    proc size: int do return _dom.size;

    /* Return the number of elements in the array as the specified type. */
    proc sizeAs(type t: integral): t do return _dom.sizeAs(t);

    //
    // This routine determines whether an actual array argument
    // ('this')'s domain is appropriate for a formal array argument
    // that specifies a domain ('formalDom').  It does this using a
    // mix of static checks (do the ranks match, are the domain map
    // types the same if the formal isn't the default dist?) and
    // runtime checks (are the domains' index sets the same; are
    // the domain maps/distributions equivalent?)
    //
    // The 'runtimeChecks' argument indicates whether or not runtime
    // checks should be performed and is set based on the value of
    // the --no-formal-domain-checks flag.
    //
    inline proc chpl_checkArrArgDoms(formalDom: domain, param runtimeChecks: bool) {
      //
      // It's a compile-time error if the ranks don't match
      //
      if (formalDom.rank != this.domain.rank) then
        compilerError("Rank mismatch passing array argument: expected " +
                      formalDom.rank:string + " but got " + this.domain.rank:string, errorDepth=2);

      //
      // If the formal domain specifies a domain map other than the
      // default one, then we're putting a constraint on the domain
      // map of the actual that's being passed in.  If it's the
      // default, we take that as an indication that the routine is
      // generic w.r.t. domain map for now (though we may wish to
      // change this in the future when we have better syntax for
      // indicating a generic domain map)..
      //
      if (formalDom.distribution._value.type != unmanaged DefaultDist) {
        //
        // First, at compile-time, check that the domain's types are
        // the same:
        //
        if (formalDom.type != this.domain.type) then
          compilerError("Domain type mismatch in passing array argument", errorDepth=2);

        //
        // Then, at run-time, check that the domain map's values are
        // the same (do this only if the runtime checks argument is true).
        //
        if (runtimeChecks && formalDom.distribution != this.domain.distribution) then
          halt("Domain map mismatch passing array argument:\n",
               "  Formal domain map is: ", formalDom.distribution, "\n",
               "  Actual domain map is: ", this.domain.distribution);
      }

      //
      // If we pass those checks, verify at runtime that the index
      // sets of the formal and actual match (do this only if the
      // runtime checks argument is true).
      //
      if (runtimeChecks && formalDom != this.domain) then
        halt("Domain mismatch passing array argument:\n",
             "  Formal domain is: ", formalDom, "\n",
             "  Actual domain is: ", this.domain);
    }

    // keep in sync with test/arrays/reindex/from-reindex-chpldocs.chpl
    /*
       Return an array view over a new domain. The new domain must be
       of the same rank and size as the original array's domain.

       For example:

       .. code-block:: chapel

          var A: [1..10] int;
          const D = {6..15};
          ref reA = A.reindex(D);
          reA[6] = 1; // updates A[1]
    */
    pragma "fn returns aliasing array"
    inline proc reindex(newDomain: domain)
      where this.domain.isRectangular() && newDomain.isRectangular() do
    return reindex((...newDomain.dims()));

    // The reason `newDims` arg is untyped is that it needs to allow
    // ranges of various types, ex. a mix of stridable and not.
    //
    // keep in sync with test/arrays/reindex/from-reindex-chpldocs.chpl
    /*
       Return an array view over a new domain defined implicitly
       by one or more `newDims`, which must be ranges. The new domain must be
       of the same rank and size as the original array's domain.

       For example:

       .. code-block:: chapel

          var A: [3..4, 5..6] int;
          ref reA = A.reindex(13..14, 15..16);
          reA[13,15] = 1; // updates A[3,5]
    */
    pragma "fn returns aliasing array"
    proc reindex(newDims...)
      where this.domain.isRectangular()
    {
      for param i in 0..newDims.size-1 do
        if !isRange(newDims(i)) then
          compilerError("cannot reindex() a rectangular array to a tuple containing non-ranges");

      if this.rank != newDims.size then
        compilerError("rank mismatch: cannot reindex() from " + this.rank:string +
                      " dimension(s) to " + newDims.size:string);

      const dom = this._value.dom;
      const origDims = dom.dsiDims();

      for param i in 0..rank-1 {
        if newDims(i).sizeAs(uint) != origDims(i).sizeAs(uint) then
          halt("extent mismatch in dimension ", i+1, ": cannot reindex() from ",
               origDims(i), " to ", newDims(i));
        if ! noNegativeStrideWarnings && origDims(i).hasPositiveStride()
           && ! newDims(i).hasPositiveStride() then
          warning("arrays and array slices with negatively-strided dimensions are currently unsupported and may lead to unexpected behavior; compile with -snoNegativeStrideWarnings to suppress this warning; in reindex() from ", origDims, " to ", newDims);
      }

      pragma "no auto destroy"
      const updom = {(...newDims)};

      const redist = new unmanaged ArrayViewReindexDist(downDistPid = this.domain.distribution._pid,
                                              downDistInst=this.domain.distribution._instance,
                                              updom = updom._value,
                                              downdomPid = dom.pid,
                                              downdomInst = dom);
      const redistRec = new _distribution(redist);
      // redist._free_when_no_doms = true;

      pragma "no copy"
      pragma "no auto destroy"
      const newDom = new _domain(redistRec, rank, updom.idxType,
                                 updom.strides, updom.dims(),
                                 definedConst=true);
      newDom._value._free_when_no_arrs = true;

      // TODO: With additional effort, we could collapse reindexings of
      // reindexed array views to a single array view, similar to what
      // we do for slices.
      const (arr, arrpid) = (this._value, this._pid);

      var x = new unmanaged ArrayViewReindexArr(eltType=this.eltType,
                                      _DomPid = newDom._pid,
                                      dom = newDom._instance,
                                      _ArrPid=arrpid,
                                      _ArrInstance=arr,
                                      ownsArrInstance=false);
      // this doesn't need to lock since we just created the domain d
      newDom._value.add_arr(x, locking=false);
      return _newArray(x);
    }

    // reindex for all non-rectangular domain types.
    // See above for the rectangular version.
    pragma "fn returns aliasing array"
    @chpldoc.nodoc
    proc reindex(d:domain) {
      compilerError("Reindexing non-rectangular arrays is not permitted.");
    }

    // Note: This 'serialize' is required at the moment because the compiler
    // generated 'serialize' is considered to be a last resort. Without this
    // method we would incur promotion when trying to print arrays.
    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      var arrayStyle = writer.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !writer._binary();
      if rank > 1 && ischpl {
        throw new owned IllegalArgumentError("Cannot perform Chapel write of multidimensional array.");
      }

      _value.dsiSerialWrite(writer);
    }

    @chpldoc.nodoc
    proc ref deserialize(reader, ref deserializer) throws {
      var arrayStyle = reader.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !reader._binary();
      if rank > 1 && ischpl {
        throw new owned IllegalArgumentError("Cannot perform Chapel read of multidimensional array.");
      }

      _value.dsiSerialRead(reader);
    }

    // TODO: Can we convert this to an initializer despite the potential issues
    // with runtime types?
    @chpldoc.nodoc
    proc type deserializeFrom(reader, ref deserializer) throws {
      var ret : this;
      ret.deserialize(reader, deserializer);
      return ret;
    }

    // sparse array interface
    /* Return the Implicitly Represented Value for sparse arrays */
    proc IRV where !this.isSparse() {
      compilerError("only sparse arrays have an IRV");
    }

    @chpldoc.nodoc
    proc IRV ref where this.isSparse() {
      return _value.IRV;
    }

    @chpldoc.nodoc
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    /*
       Return an array of locales over which this array has been distributed.
    */
    proc targetLocales() const ref {
      //
      // TODO: Is it really appropriate that the array should provide
      // this dsi routine rather than having this call forward to the
      // domain[.dist] here?  Do any of the array implementations do
      // anything other than that with it?
      //
      return _value.dsiTargetLocales();
    }

    /* Return true if the local subdomain can be represented as a single
       domain. Otherwise return false. */
    @unstable("'hasSingleLocalSubdomain' on arrays is unstable and may change in the future")
    proc hasSingleLocalSubdomain() param {
      return _value.dsiHasSingleLocalSubdomain();
    }

    /*
       Return the subdomain that is local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    proc localSubdomain(loc: locale = here) {
      if !_value.dsiHasSingleLocalSubdomain() then
        compilerError("the array may have multiple local subdomains");

      return _value.dsiLocalSubdomain(loc);
    }

    /*
       Yield the subdomains that are local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    @unstable("'localSubdomains' on arrays is unstable and may change in the future")
    iter localSubdomains(loc: locale = here) {
      if _value.dsiHasSingleLocalSubdomain() {
        yield localSubdomain(loc);
      } else {
        for d in _value.dsiLocalSubdomains(loc) do yield d;
      }
    }

    proc chpl__isDense1DArray() param {
      return this.isRectangular() &&
             this.rank == 1 &&
             this._value.hasUnitStride();
    }

    /* The following methods are intended to provide a list or vector style
       interface to 1D unstridable rectangular arrays.  They are only intended
       for use on arrays that have a 1:1 correspondence with their domains.
       All methods here that modify the array's domain assert that this 1:1
       property holds.

       These are currently not parallel safe, and cannot safely be called by
       multiple tasks simultaneously on the same array.
     */

    /* Return true if the array has no elements */
    proc isEmpty(): bool {
      return this.sizeAs(uint) == 0;
    }

    /* Return the last element in the array. The array must be a
       rectangular 1-D array.
     */
    proc last {
      if !this.isRectangular() || this.rank != 1 then
        compilerError("last is only supported on 1D rectangular arrays");

      if boundsChecking && isEmpty() then
        halt("last called on an empty array");

      return this(this.domain.last);
    }

    /* Return the first element in the array. The array must be a
       rectangular 1-D array.
     */
    proc first {
      if !this.isRectangular() || this.rank != 1 then
        compilerError("first is only supported on 1D rectangular arrays");

      if boundsChecking && isEmpty() then
        halt("first called on an empty array");

      return this(this.domain.first);
    }

    /*

      Search an array for ``val``, returning whether or not it is
      found.  If the value is found, the index storing it is returned
      in ``idx``.  If multiple copies of it are found, the
      lexicographically earliest index will be returned.  If it is not
      found, the resulting value of ``idx`` is unspecified.

    */
    proc find(val: eltType, ref idx: fullIdxType): bool {
      // Workaround for issue #21748
      proc max(type e: enum) {
        return chpl__orderToEnum(e.size-1, e);
      }

      // For the sparse case, start by seeing if the IRV is what we're
      // looking for.  If so, iterate over the parent domain to look
      // for the value or an index not represented in the array.  This
      // assumes that we're likely to find a case of the IRV fast.
      // Otherwise, fall through to the normal case to just search the
      // explicitly represented values.
      if this.isSparse() && val == this.IRV {
        // This is serial, but that's reasonable because the value
        // we're looking for is the IRV, so we'll likely bump into it
        // very quickly if this array is truly sparse.
        for i in this.domain._value.parentDom {
          // If we find an index representing the IRV, return it; if
          // it has an explicit value, it may still be the IRV, so
          // check it.
          if !this.domain.contains(i) || this[i] == val {
            idx = i;
            return true;
          }
        }
        // We didn't find it, so return false.
        return false;

      // Should we run the parallel version of the algorithm?
      // - if idxType is bool, then no b/c there's unlikely to be
      //   enough data to warrant a parallel loop and because of
      //   issue #21791
      // - if there's no max() for our 'fullIdxType' then 'no' b/c
      //   we rely on the max() value in the current parallel
      //   implementation.  Note that this calls the primitive to
      //   check the resolution directly rather than Reflection
      //   as a workaround for issue #21749
      } else if this.idxType != bool && __primitive("call and fn resolves", "max", fullIdxType) {
        var foundIt = false;
        var locIdx = max(fullIdxType);
        forall i in this.domain with (min reduce locIdx, max reduce foundIt) {
          if this[i] == val && (!foundIt || i < locIdx) {
            locIdx = i;
            foundIt = true;
          }
        }
        if foundIt then
          idx = locIdx;

        return foundIt;
      } else {
        // This is a serial fallback case, currently used by idxTypes
        // that don't support a 'max(idxType)' call (such as
        // associative arrays with string indices).  In such cases, we
        // need to use a serial iteration since we can't be guaranteed
        // that the first element found will have the
        // lexicographically earliest index.
        var foundIt = false;
        for i in this.domain {
          if this[i] == val {
            if foundIt {
              if i < idx then
                idx = i;
            } else {
              idx = i;
              foundIt = true;
            }
          }
        }
        return foundIt;
      }
    }

    /*

      Search a rectangular array with integral indices for ``val``,
      returning the index where it is found.  If the array contains
      multiple copies of ``val``, the lexicographically earliest index
      will be returned.  If ``val`` is not found,
      ``domain.lowBound-1`` will be returned instead.

      Note that for arrays with ``idxType=int(?w)`` (signed ``int``
      indices), if the low bound in a dimension is ``min(int(w))``,
      the result will not be well-defined.

    */
    proc find(val: eltType): fullIdxType {
      if !(this.isRectangular() || this.isSparse()) ||
         !isIntegralType(this.idxType) then
        compilerError("This array type does not currently support the 1-argument '.find()' method; try using the 2-argument version'");

      var idx: fullIdxType;
      if find(val, idx) then
        return idx;
      else
        // The following relies on tuple promotion for multidimensional arrays.
        return this.domain.lowBound - 1;
    }

    /* Return the number of times ``val`` occurs in the array. */
    proc count(val: this.eltType): int {
      return + reduce (this == val);
    }

   /* Return a tuple of integers describing the size of each dimension.
      For a sparse array, returns the shape of the parent domain.*/
    proc shape: rank*int where this.isRectangular() || this.isSparse() {
      return this.domain.shape;
    }

    @chpldoc.nodoc
    /* Associative domains assumed to be 1-D. */
    proc shape where this.isAssociative() {
      var s: (size.type,);
      s[0] = size;
      return s;
    }

    @chpldoc.nodoc
    /* Unsupported case */
    proc shape {
      compilerError(".shape not supported on this array");
    }

    @chpldoc.nodoc
    proc _scan(op) where Reflection.canResolveMethod(_value, "doiScan", op, this.domain) {
      return _value.doiScan(op, this.domain);
    }

    @chpldoc.nodoc
    proc iteratorYieldsLocalElements() param {
      return _value.dsiIteratorYieldsLocalElements();
    }

    proc chpl_isNonDistributedArray() param {
      return chpl_domainDistIsLayout(_getDomain(chpl__getActualArray(_value).dom));
    }

    /* Return true if the argument ``a`` is an array with a rectangular
       domain.  Otherwise return false. */
    proc isRectangular() param do return this.domain.isRectangular();

    /* Return true if ``a`` is an array with an irregular domain; e.g. not
       rectangular. Otherwise return false. */
    proc isIrregular() param do return this.domain.isIrregular();

    /* Return true if ``a`` is an array with an associative domain. Otherwise
       return false. */
    proc isAssociative() param do return this.domain.isAssociative();

    /* Return true if ``a`` is an array with a sparse domain. Otherwise
       return false. */
    proc isSparse() param do return this.domain.isSparse();
  }  // record _array

  // _instance is a subclass of BaseArr.  LYDIA NOTE: moved this from
  // being a method on _array so that it could be called on unwrapped
  // _instance fields
  inline proc _do_destroy_arr(_unowned: bool, _instance, deinitElts=true) {
    if ! _unowned {
      on _instance {
        param arrIsInList = !_instance.isSliceArrayView();
        var (arrToFree, domToRemove) = _instance.remove(arrIsInList);
        var domToFree:unmanaged BaseDom? = nil;
        var distToRemove:unmanaged BaseDist? = nil;
        var distToFree:unmanaged BaseDist? = nil;
        // The dead code to access the fields of _instance are left in the
        // generated code with --baseline on. This means that these
        // statements cannot come after the _delete_arr call.
        var domIsPrivatized = false;
        // Store the instance's dom class before the instance is destroyed
        const instanceDom = domToRemove;
        if domToRemove != nil {
          // remove that domain
          (domToFree, distToRemove) = domToRemove!.remove();
          domIsPrivatized = _privatization && (domToRemove!.pid != nullPid);
        }
        var distIsPrivatized = false;
        if distToRemove != nil {
          distToFree = distToRemove!.remove();
          distIsPrivatized = _privatization && (distToRemove!.pid != nullPid);
        }
        if arrToFree != nil then
          _delete_arr(_instance, _isPrivatized(_instance),
                      deinitElts=deinitElts);
        if domToFree != nil then
          _delete_dom(instanceDom!, domIsPrivatized);
        if distToFree != nil then
          _delete_dist(distToFree!, distIsPrivatized);
      }
    }
    else {
      // we have _unowned set, so we don't have data to free up, but our
      // instance might be an rvf'ed array view that itself needs to be cleaned
      // up
      if chpl__isArrayView(_instance) {
        delete _instance;
      }
    }
  }
  inline proc _do_destroy_array(array: _array, deinitElts=true) {
    _do_destroy_arr(array._unowned, array._instance, deinitElts);
  }

  proc _deinitElementsIsParallel(type eltType, size: integral) {
    return init_elts_method(size, eltType) == ArrayInit.parallelInit;
  }

  proc _deinitElements(ref array: _array) {
    param needsDestroy = __primitive("needs auto destroy", array.eltType);
    if needsDestroy {
      if _deinitElementsIsParallel(array.eltType, array.size) {
        forall elt in array {
          chpl__autoDestroy(elt);
        }
      } else {
        for elt in array {
          chpl__autoDestroy(elt);
        }
      }
    }
    array._instance.dsiElementDeinitializationComplete();
  }

  //
  // A helper function to check array equality (== on arrays promotes
  // to an array of booleans)
  //
  /* Return true if all this array is the same size and shape
     as argument ``that`` and all elements of this array are
     equal to the corresponding element in ``that``. Otherwise
     return false. */
  @unstable("the 'Array.equals()' method is unstable")
  proc _array.equals(that: _array): bool {
    //
    // quick path for identical arrays
    //
    if this._value == that._value then
      return true;
    //
    // quick path for rank mismatches
    //
    if this.rank != that.rank then
      return false;

    if this.sizeAs(uint) != that.sizeAs(uint) then
      return false;

    //
    // check that size/shape are the same to permit legal zippering
    //
    if this.domain.isRectangular() && that.domain.isRectangular() {
      for d in 0..#this.rank do
        if this.domain.dim(d).sizeAs(uint) != that.domain.dim(d).sizeAs(uint) then
          return false;
    }

    //
    // if all the above tests match, see if zippered equality is
    // true everywhere
    //
    if isArrayType(this.eltType) {
      var ret = true;
      forall (thisArr, thatArr) in zip(this, that) with (&& reduce ret) do
        ret &&= thisArr.equals(thatArr);
      return ret;
    } else {
      return && reduce (this == that);
    }
  }

  @unstable("casting an array to an array type is unstable due to being a new feature — please share any feedback you might have")
  operator :(arr: [], type t: []) {
    // Would like to write:
    //   var a: t = [elem in arr] elem:t.eltType;
    // but we don't support type queries on array types (yet)
    // (see https://github.com/chapel-lang/chapel/issues/8543)
    var a: t;

    if !arr.isRectangular() || !a.isRectangular() {
      compilerError("Casts between arrays only support rectangular arrays");
    } else if arr.rank != a.rank {
      compilerError("Casts between arrays require matching ranks");
    } else {
      a = [elem in arr] elem:a.eltType;
    }

    return a;
  }

  // The same as the built-in _cast, except accepts a param arg.
  @chpldoc.nodoc
  operator :(param arg, type t:_array) {
    var result: t;
    // The would-be param version of proc =, inlined.
    chpl__transferArray(result, arg);
    return result;
  }

  // How to cast arrays to strings
  @chpldoc.nodoc
  @deprecated(notes="casting arrays to string is deprecated; please use 'try! \"%?\".format()' from IO.FormattedIO instead")
  operator :(x: [], type t:string) {
    import IO.FormattedIO.string;
    return try! "%?".format(x);
  }

  pragma "last resort"
  @chpldoc.nodoc
  operator :(in x: [] ?et, type t: et) where t == et {
    return x;
  }

  pragma "fn returns aliasing array"
  @chpldoc.nodoc
  operator #(arr: [], counts: integral) {
    return arr[arr.domain#counts];
  }

  pragma "fn returns aliasing array"
  @chpldoc.nodoc
  operator #(arr: [], counts: _tuple) {
    return arr[arr.domain#counts];
  }

  pragma "last resort"
  @chpldoc.nodoc
  operator #(arr: [], counts) {
    compilerError("cannot apply '#' to '", arr.type:string,
                  "' using count(s) of type ", counts.type:string);
  }

  //
  // Helper functions
  //

  @chpldoc.nodoc
  proc isCollapsedDimension(r: range(?e,?b,?s,?a)) param do return false;
  @chpldoc.nodoc
  proc isCollapsedDimension(r) param do return true;

  // the following pair of routines counts the number of ranges in its
  // argument list and is used for rank-change slices
  proc chpl__countRanges(arg) param {
    return isRangeValue(arg):int;
  }

  proc chpl__countRanges(arg, args...) param {
    return chpl__countRanges(arg) + chpl__countRanges((...args));
  }

  // Workaround: moved these helper procs out of the body of
  // _validRankChangeArgs to avoid issue with generic parent procs.
  // Anna, 2025-03-04
  proc _validRankChangeArg(type idxType, r: range(?)) param do return true;
  proc _validRankChangeArg(type idxType, i: idxType) param do return true;
  pragma "last resort"
  proc _validRankChangeArg(type idxType, x) param do return false;

  // given a tuple args, returns true if the tuple contains only
  // integers and ranges; that is, it is a valid argument list for rank
  // change
  proc _validRankChangeArgs(args, type idxType) param {
    /*
    proc help(param dim: int) param {
      if !_validRankChangeArg(idxType, args(dim)) then
        return false;
      else if dim < args.size then
        return help(dim+1);
      else
        return true;
    }*/

    // Workaround: Added explicit args and/or idxType formals to these helpers
    // to work around lack of nested proc variable capture.
    // Anna, 2025-03-04
    proc allValid(args, type idxType) param {
      for param dim in 0.. args.size-1 {
        if !_validRankChangeArg(idxType, args(dim)) then
          return false;
      }
      return true;
    }
    proc oneRange(args) param {
      for param dim in 0.. args.size-1 {
        if isRange(args(dim)) then
          return true;
      }
      return false;
    }
    proc oneNonRange(args) param {
      for param dim in 0.. args.size-1 {
        if !isRange(args(dim)) then
          return true;
      }
      return false;
    }

    return allValid(args, idxType) && oneRange(args) && oneNonRange(args);
    //return help(0);
  }

  //
  // Assignment of distributions and arrays
  //
  @chpldoc.nodoc
  @unstable(category="experimental", reason="assignment between distributions is currently unstable due to lack of testing")
  operator =(ref a: _distribution, b: _distribution) {
    if a._value == nil {
      __primitive("move", a, chpl__autoCopy(b.clone(), definedConst=false));
    } else {
      if a._value.type != b._value.type then
        compilerError("type mismatch in distribution assignment");
      if a._value == b._value {
        // do nothing
      } else
        a._value.dsiAssign(b._value);
      if _isPrivatized(a._instance) then
        _reprivatize(a._value);
    }
  }

  proc chpl__serializeAssignment(a, b) param {
    if a.rank != 1 && isRange(b) then
      return true;

    // Sparse arrays do not yet support parallel iteration.  We
    // could let them fall through, but then we get multiple warnings for a
    // single assignment statement which feels like overkill
    //
    if ((!a.isRectangular() && !a.isAssociative() && !a.isSparse()) ||
        (isArray(b) &&
         !b.isRectangular() && !b.isAssociative() && !b.isSparse())) then
      return true;
    return false;
  }

  // these functions avoid spurious warnings related to sync variable
  // deprecation, so once the deprecation has completed these can be removed.
  private proc isCopyableOrSyncSingle(type t) param {
    return isSyncType(t) || isCopyableType(t);
  }
  private proc isConstCopyableOrSyncSingle(type t) param {
    return isSyncType(t) || isConstCopyableType(t);
  }

  // This must be a param function
  proc chpl__compatibleForBulkTransfer(a, b, param kind:_tElt) param {
    if !useBulkTransfer then return false;
    if a.eltType != b.eltType then return false;
    if kind==_tElt.move then return true;
    if kind==_tElt.initCopy && isConstCopyableOrSyncSingle(a.eltType) then return true;
    if !chpl__supportedDataTypeForBulkTransfer(a.eltType) then return false;
    return true;
  }

  /*proc chpl__supportedDataTypeForBulkTransfer(type t: c_array) param { return true;}*/

  // This must be a param function
  proc chpl__supportedDataTypeForBulkTransfer(type t) param {
    // These types cannot be default initialized
    if isSubtype(t, borrowed) || isSubtype(t, unmanaged) {
      return false;
    } else if isRecordType(t) || isTupleType(t) {
      // TODO: The current implementations of isPODType and
      //       supportedDataTypeForBulkTransfer do not completely align. I'm
      //       leaving it as future work to enable bulk transfer for other
      //       types that are POD. In the long run it seems like we should be
      //       able to have only one method for supportedDataType that just
      //       calls isPODType.

      // We can bulk transfer any record or tuple that is 'Plain Old Data'
      // ie. a bag of bits
      return isPODType(t);
    } else if (isUnionType(t)) {
      return false;
    } else {
      pragma "unsafe" var x:t;
      return chpl__supportedDataTypeForBulkTransfer(x);
    }
  }

  // TODO: should this be returning true for atomic types?
  proc chpl__supportedDataTypeForBulkTransfer(x: string) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: bytes) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: sync) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: domain) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: []) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: _distribution) param do return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: locale) param do return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: chpl_anycomplex) param do return true;
  // TODO -- why is the below line here?
  proc chpl__supportedDataTypeForBulkTransfer(x: borrowed RootClass) param do return false;
  proc chpl__supportedDataTypeForBulkTransfer(x) param do return true;

  @chpldoc.nodoc
  proc checkArrayShapesUponAssignment(a, b, forSwap = false) {
    if a.isRectangular() && b.isRectangular() {
      const aDims = if isProtoSlice(a) then a.dims()
                                       else a._value.dom.dsiDims();
      const bDims = if isProtoSlice(b) then b.dims()
                                       else b._value.dom.dsiDims();
      compilerAssert(aDims.size == bDims.size);
      for param i in 0..aDims.size-1 {
        if aDims(i).sizeAs(uint) != bDims(i).sizeAs(uint) then
          halt(if forSwap then "swapping" else "assigning",
               " between arrays of different shapes in dimension ",
               i, ": ", aDims(i).sizeAs(uint), " vs. ", bDims(i).sizeAs(uint));
      }
    } else {
      // may not have dsiDims(), so can't check them as above
      // todo: compilerError if one is rectangular and the other isn't?
    }
  }

  pragma "find user line"
  private inline proc arrayOrProtoSliceAssign(ref a, b) {
    if a.rank != b.rank then
      compilerError("rank mismatch in array assignment");

    if b._value == nil then
      // This happens e.g. for 'new' on a record with an array field whose
      // default initializer is a forall expr. E.g. arrayInClassRecord.chpl.
      return;

    var eqVals: bool;
    if isArray(a) && isArray(b) {
      eqVals = (a._value == b._value);
    }
    else if isProtoSlice(a) && isProtoSlice(b) {
      eqVals = (a == b);
    }
    else {
      compilerError("Internal error: cross-type assignments are not supported");
    }

    if eqVals then {
      // Do nothing for A = A but we could generate a warning here
      // since it is probably unintended. We need this check here in order
      // to avoid memcpy(x,x) which happens inside doiBulkTransfer.
      return;
    }

    if a.sizeAs(uint) == 0 && b.sizeAs(uint) == 0 then
      // Do nothing for zero-length assignments
      return;

    if boundsChecking then
      checkArrayShapesUponAssignment(a, b);

    chpl__uncheckedArrayTransfer(a, b, kind=_tElt.assign);
  }


  pragma "find user line"
  @chpldoc.nodoc
  inline operator =(ref a: [], b: []) {
    arrayOrProtoSliceAssign(a, b);
  }

  pragma "find user line"
  @chpldoc.nodoc
  inline operator =(ref a: chpl__protoSlice, b: chpl__protoSlice) {
    arrayOrProtoSliceAssign(a, b);
  }

  // what kind of transfer to do for each element?
  @chpldoc.nodoc
  enum _tElt {
    move,
    initCopy,
    assign
  }

  // This is a workaround for an issue with replaceRecordWrappedRefs
  // when initializing arrays of domains (or arrays or distributions).
  // replaceRecordWrappedRefs assumes that the wrapper records (e.g. _array)
  // are immutable. That's not true before they are initialized
  // which can happen in chpl__transferArray, so chpl__transferArray
  // uses a different pattern to avoid the problem for those types.
  private proc needsInitWorkaround(type t) param {
    return isSubtype(t, _array) ||
           isSubtype(t, _domain) ||
           isSubtype(t, _distribution);
  }

  pragma "ignore transfer errors"
  private proc initCopyAfterTransfer(ref a: []) {
    if needsInitWorkaround(a.eltType) {
      forall ai in a.domain with (ref a) {
        ref aa = a[ai];
        pragma "no auto destroy"
        var copy: a.eltType = aa; // run copy initializer
        // move it into the array
        __primitive("=", aa, copy);
      }
    } else if isSyncType(a.eltType) {
      forall aa in a {
        pragma "no auto destroy"
        var copy: a.eltType = aa.readFE(); // run copy initializer
        // move it into the array
        __primitive("=", aa, copy);
      }
    } else {
      forall aa in a {
        pragma "no auto destroy"
        var copy: a.eltType = aa; // run copy initializer
        // move it into the array
        __primitive("=", aa, copy);
      }
    }
  }

  // If we move initialized an array element from another value,
  // there is the potential for the runtime type to need adjustment
  // (if the element type is an array/domain type). In that event,
  // check that the runtime type components match. If not, copy-initialize.
  private proc fixRuntimeType(type eltType, ref elt) {
    var runtimeTypesDiffer = false;
    if isSubtype(eltType, _array) || isSubtype(eltType, _domain) {
      if isSubtype(eltType, _array) {
        const ref lhsDomain = chpl__domainFromArrayRuntimeType(eltType);
        const ref rhsDomain = elt.domain;
        if lhsDomain._instance != rhsDomain._instance {
          runtimeTypesDiffer = true;
        }
      }
      if isSubtype(eltType, _domain) {
        const ref lhsDist = chpl__distributionFromDomainRuntimeType(eltType);
        const ref rhsDist = elt.distribution;
        if lhsDist._instance != rhsDist._instance {
          runtimeTypesDiffer = true;
        }
      }
      // the element has a runtime type
      if runtimeTypesDiffer {
        // if the runtime type does not match, re-initialize the element

        // copy-initialize the element to a variable with the right type
        // (this should call chpl__coerceCopy)
        pragma "no auto destroy"
        var copy: eltType = elt;

        // deinitialize the old element
        chpl__autoDestroy(elt);

        // set the element to the copy
        __primitive("=", elt, copy);
      }
    }
  }

  private proc fixEltRuntimeTypesAfterTransfer(ref a: []) {
    if needsInitWorkaround(a.eltType) {
      forall ai in a.domain with (ref a) {
        ref aa = a[ai];
        fixRuntimeType(a.eltType, aa);
      }
    } else {
      forall aa in a {
        fixRuntimeType(a.eltType, aa);
      }
    }
  }

  pragma "find user line"
  inline proc chpl__uncheckedArrayTransfer(ref a, b, param kind) {
    use ChapelShortArrayTransfer;

    if chpl__serializeAssignment(a, b) {
      chpl__transferArray(a, b, kind);
    }
    else if chpl__staticCheckShortArrayTransfer(a, b) &&
            chpl__dynamicCheckShortArrayTransfer(a, b) {
      chpl__transferArray(a, b, kind, alwaysSerialize=true);
    }
    else if chpl__compatibleForBulkTransfer(a, b, kind) {
      if chpl__bulkTransferArray(a, b) {
        chpl__initAfterBulkTransfer(a, kind);
      }
      else {
        chpl__transferArray(a, b, kind);
      }
    }
    else if chpl__compatibleForWidePtrBulkTransfer(a, b, kind) {
      if chpl__bulkTransferPtrArray(a, b) {
        chpl__initAfterBulkTransfer(a, kind);
      }
      else {
        chpl__transferArray(a, b, kind);
      }
    }
    else {
      chpl__transferArray(a, b, kind);
    }
  }

  inline proc chpl__initAfterBulkTransfer(ref a, param kind) {
    if kind==_tElt.initCopy && !isPODType(a.eltType) {
      initCopyAfterTransfer(a);
    } else if kind==_tElt.move && (isSubtype(a.eltType, _array) ||
        isSubtype(a.eltType, _domain)) {
      fixEltRuntimeTypesAfterTransfer(a);
    }
  }

  proc chpl__compatibleForWidePtrBulkTransfer(a, b,
                                              param kind=_tElt.assign) param {
    if !useBulkPtrTransfer then return false;

    // TODO: for now we are limiting ourselves to default rectangulars
    if !(a._value.isDefaultRectangular() &&
         b._value.isDefaultRectangular()) then return false;

    if a.eltType != b.eltType then return false;

    // only classes have pointer assignment semantics
    if !isClass(a.eltType) then return false;

    // ownership transfer is complicated
    if isOwnedClass(a.eltType) then return false;

    // shared array assignment seems to be handled differently, but prevent them
    // here, too, just in case.
    if isSharedClass(a.eltType) then return false;

    return true;
  }

  inline proc chpl__bulkTransferPtrArray(ref a: [], b: []) {
    // for now assume they are both local arrays, that have the same bounds
    const aDom = a.domain;
    const bDom = b.domain;
    if aDom != bDom then return false;

     // TODO can we omit the following check and bulk transfer narrow
     // pointers, too
    if __primitive("is wide pointer", a[aDom.low]) {
      return chpl__bulkTransferArray(a, aDom, b, bDom);
    }
    return false;
  }

  inline proc chpl__bulkTransferArray(ref a: [?AD], b : [?BD]) {
    return chpl__bulkTransferArray(a, AD, b, BD);
  }

  inline proc chpl__bulkTransferArray(ref a: chpl__protoSlice,
                                      b: chpl__protoSlice) {
    if debugBulkTransfer {
      chpl_debug_writeln("Performing protoSlice bulk transfer");
    }
    return chpl__bulkTransferArray(a.ptrToArr.deref(), a.domOrRange,
                                   b.ptrToArr.deref(), b.domOrRange);
  }
  inline proc chpl__bulkTransferArray(ref a: [], AD, const ref b: [], BD) {
    return chpl__bulkTransferArray(a._value, AD, b._value, BD);
  }


  inline proc chpl__bulkTransferArray(destClass, destView, srcClass, srcView) {
    var success = false;

    inline proc bulkTransferDebug(msg:string) {
      if debugBulkTransfer then chpl_debug_writeln("operator =(a:[],b:[]): ", msg);
    }

    bulkTransferDebug("in chpl__bulkTransferArray");

    //
    // BHARSH TODO: I would prefer to hoist these 'canResolveMethod' calls into
    // param bools before the if/else chain, but the compiler complains about
    // hitting the instantiation limit for 'canResolveMethod'...
    //
    // TODO: should we attempt other bulk transfer methods if one fails?
    //
    if Reflection.canResolveMethod(destClass, "doiBulkTransferFromKnown",
                                   destView, srcClass, srcView) {
      bulkTransferDebug("attempting doiBulkTransferFromKnown");
      success = destClass.doiBulkTransferFromKnown(destView, srcClass, srcView);
    } else if Reflection.canResolveMethod(srcClass, "doiBulkTransferToKnown",
                                          srcView, destClass, destView) {
      bulkTransferDebug("attempting doiBulkTransferToKnown");
      success = srcClass.doiBulkTransferToKnown(srcView, destClass, destView);
    } else if Reflection.canResolveMethod(destClass, "doiBulkTransferFromAny",
                                          destView, srcClass, srcView) {
      bulkTransferDebug("attempting doiBulkTransferFromAny");
      success = destClass.doiBulkTransferFromAny(destView, srcClass, srcView);
    } else if Reflection.canResolveMethod(srcClass, "doiBulkTransferToAny",
                                          srcView, destClass, destView) {
      bulkTransferDebug("attempting doiBulkTransferToAny");
      success = srcClass.doiBulkTransferToAny(srcView, destClass, destView);
    }

    if success then
      bulkTransferDebug("successfully completed bulk transfer");
    else
      bulkTransferDebug("bulk transfer did not happen");

    return success;
  }

  pragma "find user line"
  pragma "ignore transfer errors"
  inline proc chpl__transferArray(ref a, const ref b,
                           param kind=_tElt.assign,
                           param alwaysSerialize=false) lifetime a <= b {
    if (a.eltType == b.type ||
        _isPrimitiveType(a.eltType) && _isPrimitiveType(b.type)) {

      if kind==_tElt.move || kind==_tElt.initCopy {
        // need to copy if "move"ing from 1 element
        if needsInitWorkaround(a.eltType) {
          forall ai in a.domain with (in b) {
            ref aa = a[ai];
            pragma "no auto destroy"
            var copy: a.eltType = b; // make a copy for this iteration
            // move it into the array
            __primitive("=", aa, copy);
          }

        } else {
          forall aa in a with (in b) {
            pragma "no auto destroy"
            var copy: a.eltType = b; // make a copy for this iteration
            // move it into the array
            __primitive("=", aa, copy);
          }
        }
      } else if kind==_tElt.assign {
        forall aa in a with (in b) {
          aa = b;
        }
      }
    } else if alwaysSerialize || chpl__serializeAssignment(a, b) {
      if kind==_tElt.move {
        if needsInitWorkaround(a.eltType) {
          for (ai, bb) in zip(a.domain, b) {
            ref aa = a[ai];
            __primitive("=", aa, __primitive("steal", bb));
            fixRuntimeType(a.eltType, aa);
          }

        } else {
          for (aa,bb) in zip(a,b) {
            __primitive("=", aa, __primitive("steal", bb));
            fixRuntimeType(a.eltType, aa);
          }
        }

      } else if kind==_tElt.initCopy {
        if needsInitWorkaround(a.eltType) {
          for (ai, bb) in zip(a.domain, b) {
            ref aa = a[ai];
            pragma "no auto destroy"
            var copy: a.eltType = b; // init copy
            // move it into the array
            __primitive("=", aa, copy);
          }

        } else {
          for (aa,bb) in zip(a,b) {
            pragma "no auto destroy"
            var copy: a.eltType = b; // init copy
            // move it into the array
            __primitive("=", aa, copy);
          }
        }

      } else if kind==_tElt.assign {
        for (aa,bb) in zip(a,b) {
          aa = bb;
        }
      }
    } else {
      if kind==_tElt.move {
        if needsInitWorkaround(a.eltType) {
          [ (ai, bb) in zip(a.domain, b) with (ref a) ] {
            ref aa = a[ai];
            __primitive("=", aa, __primitive("steal", bb));
            fixRuntimeType(a.eltType, aa);
          }

        } else {
          [ (aa,bb) in zip(a,b) ] {
            __primitive("=", aa, __primitive("steal", bb));
            fixRuntimeType(a.eltType, aa);
          }
        }
      } else if kind==_tElt.initCopy {
        if needsInitWorkaround(a.eltType) {
          [ (ai, bb) in zip(a.domain, b) with (ref a) ] {
            ref aa = a[ai];
            pragma "no auto destroy"
            var copy: a.eltType = bb; // init copy
            // move it into the array
            __primitive("=", aa, copy);
          }
        } else {
          [ (aa,bb) in zip(a,b) ] {
            if isSyncType(bb.type) {
              pragma "no auto destroy"
              var copy: a.eltType = bb.readFE(); // init copy
              // move it into the array
              __primitive("=", aa, copy);
            } else {
              pragma "no auto destroy"
              var copy: a.eltType = bb; // init copy
              // move it into the array
              __primitive("=", aa, copy);
            }
          }
        }
      } else if kind==_tElt.assign {
        [ (aa,bb) in zip(a,b) ] {
          aa = bb;
        }
      }
    }
  }

  // assigning from a param
  pragma "find user line"
  inline proc chpl__transferArray(ref a: [], param b,
                                  param kind=_tElt.assign) {
    forall aa in a do
      aa = b;
  }

  @chpldoc.nodoc
  inline operator =(ref a: [], b:domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in array assignment");
    if b.isAssociative() && a.isRectangular() then
      compilerError("cannot assign to rectangular arrays from associative domains");
    chpl__transferArray(a, b);
  }

  @chpldoc.nodoc
  inline operator =(ref a: [], b: range(?)) {
    if a.rank == 1 then
      chpl__transferArray(a, b);
    else
      compilerError("cannot assign from ranges to multidimensional arrays");
  }

  @chpldoc.nodoc
  inline operator =(ref a: [], b: _iteratorRecord) {
    // e.g. b might be a list
    chpl__transferArray(a, b);
  }

  // TODO Jade 6/29/23: this should be deprecated/removed with _desync
  pragma "last resort"
  @chpldoc.nodoc
  inline operator =(ref a: [], b: ?t)
  where !(isTupleType(t) || isCoercible(t, _desync(a.eltType))) {
    // e.g. b might be a list
    chpl__transferArray(a, b);
  }

/* Does not work: compiler expects assignments to have 2 formals,
   whereas the below becomes a 1-argument function after resolution.
  inline operator =(ref a: [], param b) {
    chpl__transferArray(a, b);
  }
*/

  private proc helpInitArrFromTuple(ref j, param rank: int,
                                    ref a: [], b: _tuple,
                                    param kind:_tElt)
  lifetime a < b {

      type idxType = a.domain.idxType,
           strType = chpl__signedType(a.domain.chpl_integralIdxType);

      const stride = a.domain.dim(a.rank-rank).stride,
      start = a.domain.dim(a.rank-rank).firstAsInt;

      if rank == 1 {
        for param i in 0..b.size-1 {
          j(a.rank-rank) = chpl__intToIdx(idxType, start:strType + (i*stride));
          ref dst = a(j);
          const ref src = b(i);
          if kind == _tElt.move {
            if isArray(dst) {
              pragma "no auto destroy" pragma "no copy"
              var newArr = chpl__coerceMove(a.eltType, src, definedConst=false);
              __primitive("=", dst, newArr);
            } else {
              __primitive("=", dst, src);
              fixRuntimeType(a.eltType, dst);
            }
          } else if kind == _tElt.initCopy {
            pragma "no auto destroy"
            var copy: a.eltType = src; // init copy
            __primitive("=", dst, copy);
          } else if kind == _tElt.assign {
            dst = src;
          }
        }
      } else {
        for param i in 0..b.size-1 {
          j(a.rank-rank) = chpl__intToIdx(idxType, start:strType + (i*stride));
          helpInitArrFromTuple(j, rank-1, a, b(i), kind);
        }
      }
  }
  private proc initArrFromTuple(ref a: [], b: _tuple, param kind:_tElt) {
    var j: a.rank*a.domain.idxType;
    helpInitArrFromTuple(j, a.rank, a, b, kind);
  }

  @chpldoc.nodoc
  operator =(ref a: [], b: _tuple) where a.isRectangular() {
    initArrFromTuple(a, b, _tElt.assign);
  }

  // TODO Jade 6/29/23: post implicit sync/single reads, we should be able to
  // make these cases give a nice error
  proc _desync(type t:_syncvar) type {
    var x: t;
    return x.valType;
  }

  proc _desync(type t) type where isAtomicType(t) {
    var x: t;
    return x.read().type;
  }

  /* Or, we could explicitly overload for each atomic type since there
     are a fixed number
  proc _desync(type t: atomic int) type {
    return int;
  } */

  proc _desync(type t:_array) type {
    type eltType = chpl__eltTypeFromArrayRuntimeType(t);
    const ref dom = chpl__domainFromArrayRuntimeType(t);
    return [dom] _desync(eltType);
  }

  proc _desync(type t) type {
    return t;
  }

  private proc desyncEltType(type t:_array) type {
    type eltType = chpl__eltTypeFromArrayRuntimeType(t);
    return _desync(eltType);
  }

  @chpldoc.nodoc
  operator =(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e = b;
  }

  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated(notes="Direct assignment to 'sync' variables is deprecated; apply a 'write??()' method to modify one")
  operator =(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e = b;
  }

  //
  // op= overloads for array/scalar pairs
  //
  @chpldoc.nodoc
  operator +=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e += b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator +=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e += b;
  }

  @chpldoc.nodoc
  operator -=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e -= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator -=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e -= b;
  }

  @chpldoc.nodoc
  operator *=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e *= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator *=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e *= b;
  }

  @chpldoc.nodoc
  operator /=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e /= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator /=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e /= b;
  }

  @chpldoc.nodoc
  operator %=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e %= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator %=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e %= b;
  }

  @chpldoc.nodoc
  operator **=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e **= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator **=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e **= b;
  }

  @chpldoc.nodoc
  operator &=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e &= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator &=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e &= b;
  }

  @chpldoc.nodoc
  operator |=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e |= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator |=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e |= b;
  }

  @chpldoc.nodoc
  operator ^=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e ^= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator ^=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e ^= b;
  }

  @chpldoc.nodoc
  operator >>=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e >>= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator >>=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e >>= b;
  }

  @chpldoc.nodoc
  operator <<=(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e <<= b;
  }
  // required to prevent deprecation warnings being related to internal modules
  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator <<=(ref a: [], b: _desync(a.eltType)) where isSyncType(a.eltType) {
    forall e in a do
      e <<= b;
  }

  //
  // Swap operator for arrays
  //
  @chpldoc.nodoc
  inline operator <=>(ref x: [?xD], ref y: [?yD]) {
    if x.rank != y.rank then
      compilerError("rank mismatch in array swap");

    if boundsChecking then
      checkArrayShapesUponAssignment(x, y, forSwap=true);

    var hasSwapped: bool = false;

    // we don't want to do anything optimized for arrays with different element
    // types, if their eltTypes can coerce to one another let the forall handle
    // it
    if x.eltType == y.eltType {
      // Check if array can use optimized pointer swap
      if Reflection.canResolveMethod(x._value, "doiOptimizedSwap", y._value) {
        hasSwapped = x._value.doiOptimizedSwap(y._value);
      }
    }

    if !hasSwapped {
      forall (a,b) in zip(x, y) do
        a <=> b;
    }
  }

  /* Return a copy of the array ``A`` containing the same values but
     in the shape of the domain ``D``. The number of indices in the
     domain must equal the number of elements in the array. The
     elements of ``A`` are copied into the new array using the
     default iteration orders over ``D`` and ``A``.  */
  proc reshape(A: [], D: domain) {
    if !D.isRectangular() then
      compilerError("reshape(A,D) is meaningful only when D is a rectangular domain; got D: ", D.type:string);
    if A.sizeAs(int) != D.sizeAs(int) then
      halt("reshape(A,D) is invoked when A has ", A.sizeAs(int),
           " elements, but D has ", D.sizeAs(int), " indices");
    var B: [D] A.eltType = for (i,a) in zip(D, A) do a;
    return B;
  }

  @chpldoc.nodoc
  proc reshape(A: _iteratorRecord, D: domain) {
    if !D.isRectangular() then
      compilerError("reshape(A,D) is meaningful only when D is a rectangular domain; got D: ", D.type:string);
    var B = for (i,a) in zip(D, A) do a;
    return B;
  }

  @chpldoc.nodoc
  iter linearize(Xs) {
    for x in Xs do yield x;
  }

  pragma "init copy fn"
  proc chpl__initCopy(const ref rhs: domain, definedConst: bool)
      where rhs.isRectangular() {

    var lhs = new _domain(rhs.distribution, rhs.rank, rhs.idxType, rhs.strides,
                          rhs.dims(), definedConst=definedConst);
    return lhs;
  }

  pragma "init copy fn"
  proc chpl__initCopy(const ref rhs: domain, definedConst: bool)
      where rhs.isAssociative() {

    var lhs = new _domain(rhs.distribution, rhs.idxType, rhs.parSafe,
                          definedConst=definedConst);
    // No need to lock this domain since it's not exposed anywhere yet.
    // No need to handle arrays over this domain either for the same reason.
    lhs._instance.dsiAssignDomain(rhs, lhsPrivate=true);
    return lhs;
  }

  pragma "init copy fn"
  proc chpl__initCopy(const ref rhs: domain, definedConst: bool)
      where rhs.isSparse() {

    var lhs = new _domain(rhs.distribution, rhs.parentDom, definedConst=definedConst);
    // No need to lock this domain since it's not exposed anywhere yet.
    // No need to handle arrays over this domain either for the same reason.
    lhs._instance.dsiAssignDomain(rhs, lhsPrivate=true);
    return lhs;
  }

  // TODO: why is the compiler calling chpl__autoCopy on a domain at all?
  pragma "auto copy fn"
  proc chpl__autoCopy(const ref x:domain, definedConst: bool) {
    pragma "no copy" var b = chpl__initCopy(x, definedConst);
    return b;
  }

  //
  // These control an optimization in which copying an array from one
  // locale to another will also copy the array's domain if we believe
  // it's safe to do so.  The optimization is currently on by default.
  //
  config param localizeConstDomains = true,
               debugLocalizedConstDomains = false;

  pragma "init copy fn"
  proc chpl__initCopy(const ref rhs: [], definedConst: bool) {
    //
    // create a local copy of a remote array's domain if...
    // - the optimization is enabled
    // - we're running using multiple locales
    // - the array copy we're declaring is 'const' (implying that
    //   its domain can't be re-assigned while it's alive) or the
    //   original domain is 'const' (implying that it can never
    //   be re-assigned)
    // - the domain is on a remote locale
    //
    const localize = (localizeConstDomains &&
                      numLocales > 1 &&
                      (definedConst || rhs.domain.definedConst) &&
                      rhs.domain._value.locale != here);
    if debugLocalizedConstDomains then
      writeln("In initCopy(definedConst=", definedConst,
              "), domain definedConst: ", rhs.domain.definedConst, "; ",
              if localize then "localizing" else "taking normal path");
    if localize {
      // localize domain for efficiency since it's OK to do so
      const lhsDom = rhs.domain;
      pragma "no copy"
      var lhs: [lhsDom] rhs.eltType = rhs;
      return lhs;
    } else {
      // otherwise, do what we traditionally have done
      pragma "no copy"
      var lhs = chpl__coerceCopy(rhs.type, rhs, definedConst);
      return lhs;
    }
  }

  // TODO: why is the compiler calling chpl__autoCopy on an array at all?
  pragma "auto copy fn" proc chpl__autoCopy(x: [], definedConst: bool) {
    pragma "no copy" var b = chpl__initCopy(x, definedConst);
    return b;
  }

  inline proc chpl__coerceHelp(type dstType: domain, definedConst: bool) {
    pragma "no copy"
    pragma "no auto destroy"
    const ref dist = __primitive("get runtime type field", dstType, "dist");
    type instanceType = chpl__instanceTypeFromDomainRuntimeType(dstType);
    if chpl__isRectangularDomType(dstType) {
      return chpl__convertRuntimeTypeToValue(dist=dist,
                                             rank=instanceType.rank,
                                             idxType=instanceType.idxType,
                                             strides=instanceType.strides,
                                             isNoInit=false,
                                             definedConst=definedConst);
    }
    else if chpl__isSparseDomType(dstType) {
      pragma "no copy"
      pragma "no auto destroy"
      const ref parentDom = __primitive("get runtime type field", dstType,
                                        "parentDom");
      return chpl__convertRuntimeTypeToValue(dist=dist,
                                             parentDom=parentDom,
                                             isNoInit=false,
                                             definedConst=definedConst);
    }
    else {
      return chpl__convertRuntimeTypeToValue(dist=dist,
                                             idxType=instanceType.idxType,
                                             parSafe=instanceType.parSafe,
                                             isNoInit=false,
                                             definedConst=definedConst);
    }
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:_array, definedConst: bool) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if !isCopyableOrSyncSingle(eltType) then
      compilerError("Cannot copy-initialize array because element type '",
                    eltType:string, "' cannot be copy-initialized");

    if rhs._value == nil {
      // This happens e.g. for 'new' on a record with an array field whose
      // default initializer is a forall expr. E.g. arrayInClassRecord.chpl.
    } else if lhs._value == rhs._value {
      // do nothing (assert?)
    } else if lhs.sizeAs(int) == 0 && rhs.sizeAs(int) == 0 {
      // Do nothing for zero-length assignments
    } else {
      if boundsChecking then
        checkArrayShapesUponAssignment(lhs, rhs);

      chpl__uncheckedArrayTransfer(lhs, rhs, kind=_tElt.initCopy);
    }

    // provide distributions a hook to call _ddata_allocate_postalloc etc.
    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array,
                        pragma "no auto destroy" in rhs:_array,
                        definedConst: bool) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    // type mismatch important because RHS could be a slice, e.g.
    param typeMismatch = rhs._instance.type !=
                         chpl__instanceTypeFromArrayRuntimeType(dstType);

    param moveElts = !typeMismatch;

    if moveElts {
      // If the domains point to the same thing (and aren't just identical),
      // then we can simply return the RHS array.
      if dom._instance == rhs.domain._instance {
        return rhs;
      }
      // If the domains have the same value but aren't pointing to the same
      // thing, we can ask the array implementation to steal the buffer
      if Reflection.canResolveMethod(dom._value,
                                     "doiBuildArrayMoving",
                                     rhs._value) {
        if dom == rhs.domain &&
           dom.distribution == rhs.domain.distribution &&
           eltType == rhs.eltType && // note: eltType might have runtime part
           !isArrayType(eltType) && // arrays-of-arrays not working yet
           !rhs._unowned {
          // ask the array implementation to steal the buffer
          pragma "no copy" // avoid error about recursion for initCopy
          pragma "unsafe" // when eltType is non-nilable
          var lhs = dom.buildArrayMoving(rhs._value);
          // destroy the husk that we moved from
          _do_destroy_arr(rhs._unowned, rhs._instance, deinitElts=false);
          return lhs;
        }
      }
    }

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");

    if rhs._value == nil {
      // This happens e.g. for 'new' on a record with an array field whose
      // default initializer is a forall expr. E.g. arrayInClassRecord.chpl.
    } else if lhs._value == rhs._value {
      // do nothing (assert?)
    } else if lhs.sizeAs(int) == 0 && rhs.sizeAs(int) == 0 {
      // Do nothing for zero-length assignments
    } else {
      if boundsChecking then
        checkArrayShapesUponAssignment(lhs, rhs);

      param kind = if moveElts then _tElt.move else _tElt.initCopy;
      chpl__uncheckedArrayTransfer(lhs, rhs, kind=kind);
    }

    // we might have already moved the elements out
    // and so should not try to deinit them.
    // We still need to free any array memory.
    _do_destroy_arr(rhs._unowned, rhs._instance, deinitElts=!moveElts);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:_domain, definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if rhs.isAssociative() && lhs.isRectangular() then
      compilerError("cannot assign to rectangular arrays from associative domains");
    if !isPODType(eltType) then
      compilerError("cannot assign to array from domain of non-POD indices");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array, in rhs:_domain, definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if rhs.isAssociative() && lhs.isRectangular() then
      compilerError("cannot assign to rectangular arrays from associative domains");
    if !isPODType(eltType) then
      compilerError("cannot assign to array from domain of non-POD indices");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:range(?), definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != 1 then
      compilerError("cannot assign from ranges to multidimensional arrays");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array, in rhs:range(?), definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != 1 then
      compilerError("cannot assign from ranges to multidimensional arrays");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:_tuple, definedConst: bool) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if !lhs.isRectangular() then
      compilerError("Cannot assign from tuple to non-rectangular array");

    initArrFromTuple(lhs, rhs, _tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array,
                        pragma "no auto destroy" in rhs:_tuple,
                        definedConst: bool) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if !lhs.isRectangular() then
      compilerError("Cannot assign from tuple to non-rectangular array");

    initArrFromTuple(lhs, rhs, _tElt.move);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:desyncEltType(dstType),
                        definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    forall e in lhs with (in rhs) {
      pragma "no auto destroy"
      var copy: lhs.eltType = rhs; // make a copy for this iteration
      // move it into the array
      __primitive("=", e, copy);
    }

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array, in rhs:desyncEltType(dstType), definedConst: bool) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    // always copies because there is 1 rhs but many array elts (generally)
    forall e in lhs with (in rhs) {
      pragma "no auto destroy"
      var copy: eltType = rhs; // make a copy for this iteration
      // move it into the array
      __primitive("=", e, copy);
    }

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs: _iteratorRecord, definedConst: bool) {
    // assumes rhs is iterable

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array, rhs: _iteratorRecord, definedConst: bool) {
    // assumes rhs is iterable

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    chpl__transferArray(lhs, rhs, kind=_tElt.move);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  pragma "last resort"
  proc chpl__coerceCopy(type dstType:_array, rhs, definedConst: bool) {
    // assumes rhs is iterable (e.g. list)

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  pragma "last resort"
  proc chpl__coerceMove(type dstType:_array, in rhs, definedConst: bool) {
    // assumes rhs is iterable (e.g. list)

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }


  // Used to implement the copy-out language semantics
  // Relies on the return types being different to detect an ArrayView at
  // compile-time
  //pragma "fn returns infinite lifetime"
  pragma "no copy return"
  pragma "unref fn"
  inline proc chpl__unref(x: []) where chpl__isArrayView(x._value) {
    // intended to call initCopy
    pragma "no auto destroy" var ret = x;
    return ret;
  }

  //pragma "fn returns infinite lifetime"
  pragma "no copy return"
  pragma "unref fn"
  proc chpl__unref(ir: _iteratorRecord) {
    pragma "no auto destroy"
    pragma "no copy"
    // TODO: definedConst can be added to `chpl__unref`, too
    var toArray = chpl__initCopy(ir, definedConst=false); // call iterator -> array copy fn
    return toArray;
  }


  // Intended to return whatever it gets without copying
  // Not marked with "unref fn" because this version shouldn't
  // actually remain in the AST - it's just added temporarily
  // during resolution.
  pragma "no copy return"
  inline proc chpl__unref(x: []) {
    pragma "no copy" var ret = x;
    return ret;
  }


  // chpl__initCopy(ir: _iteratorRecord, definedConst: bool) is used to create
  // an array out of for-expressions, forall-expressions, promoted expressions.
  // The 'ir' iterator - or its standalone/leader/follower counterpart - is
  // invoked to generate the desired array elements.

  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord, definedConst: bool)
    where chpl_iteratorHasDomainShape(ir)
  {

    // ENGIN: here ir._shape_ could be a privatized domain. Make sure that the
    // initializer we call here does not create another set of privatized
    // instances
    // TODO: can we make this domain constant by passing an argument to the
    // initializer?
    var shape = new _domain(ir._shape_);

    // Important: ir._shape_ points to a domain class for a domain
    // that is owned by the forall-expression or the leader in the
    // promoted expression.
    shape._unowned = true;

    return chpl__initCopy_shapeHelp(shape, ir);
  }

  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord, definedConst: bool)
    where chpl_iteratorHasRangeShape(ir) && !chpl_iteratorFromForExpr(ir)
  {
    // Need this pragma in the range case to avoid leaking 'shape'.
    // If we use it in the domain case, we get one autoDestroy too many.
    pragma "insert auto destroy"
    var shape = {ir._shape_};

    return chpl__initCopy_shapeHelp(shape, ir);
  }

  pragma "no copy returns owned"
  pragma "ignore transfer errors"
  proc chpl__initCopy_shapeHelp(shape: domain, ir: _iteratorRecord)
  {
    pragma "unsafe" pragma "no copy"
    var result = shape.buildArray(iteratorToArrayElementType(ir.type),
                                  initElts=false);

    if isArray(result.eltType) then
      compilerError("creating an array of arrays using a for- or forall-expression is not supported, except when using a for-expression over a range");

    if chpl_iteratorFromForExpr(ir) {
      if needsInitWorkaround(result.eltType) {
        pragma "loop indices mutable"
        for (ri, src) in zip(result.domain, ir) {
          ref r = result[ri];
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      } else {
        pragma "loop indices mutable"
        for (r, src) in zip(result, ir) {
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      }
    } else if chpl_iteratorFromForeachExpr(ir) {
      if needsInitWorkaround(result.eltType) {
        pragma "loop indices mutable"
        foreach (ri, src) in zip(result.domain, ir) {
          ref r = result[ri];
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      } else {
        pragma "loop indices mutable"
        foreach (r, src) in zip(result, ir) {
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      }
    } else {
      if needsInitWorkaround(result.eltType) {
        pragma "loop indices mutable"
        forall (ri, src) in zip(result.domain, ir) with (ref result) {
          ref r = result[ri];
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      } else {
        pragma "loop indices mutable"
        forall (r, src) in zip(result, ir) {
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      }
    }

    result._value.dsiElementInitializationComplete();

    return result;
  }

  pragma "unchecked throws"
  proc chpl__throwErrorUnchecked(in e: owned Error) throws {
    throw e;
  }

  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord, definedConst: bool) {
    // We'd like to know the yielded type of the record, but we can't
    // access the (runtime) component of that until we actually yield
    // something.
    //
    // The below is a work-around that creates a 1-D array by
    // filling in a ddata and then handing the data to DefaultRectangularArr.
    // Besides breaking the catch-22 with the runtime type, the other
    // advantage of this approach is that it moves the yielded value
    // in to the array instead of assigning it. Alternatives to this strategy
    // include creating a no-inited array with the static type that the
    // iterator yields.
    //
    // The resulting array grows dynamically. That limits parallelism here.

    param shapeful = chpl_iteratorHasRangeShape(ir);
    var r = if shapeful then ir._shape_ else 1..0;

    var i  = 0;
    var size = r.sizeAs(c_size_t);
    type elemType = iteratorToArrayElementType(ir.type);
    var data:_ddata(elemType) = nil;

    var callPostAlloc: bool;
    var subloc = c_sublocid_none;

    if size > 0 {
      data = _ddata_allocate_noinit(elemType, size, callPostAlloc);
    }

    try {
      pragma "loop indices mutable"
      for elt in ir {

        // Future: we should generally remove this copy.
        // Note though that in some cases it invokes this function
        // recursively - in that case it shouldn't be removed!
        pragma "no auto destroy"
        pragma "no copy"
        var eltCopy = try chpl__initCopy(elt, definedConst);

        if i >= size {
          // Allocate a new buffer and then copy.
          var oldSize = size;
          var oldData = data;

          if size == 0 then
            size = 4;
          else
            size = 2 * size;

          data = _ddata_allocate_noinit(elemType, size, callPostAlloc);

          // Now copy the data over
          for i in 0..#oldSize {
            // this is a move, transferring ownership
            __primitive("=", data[i], oldData[i]);
          }

          // Then, free the old data
          _ddata_free(oldData, oldSize);
        }

        // Now move the element to the array
        // The intent here is to transfer ownership to the array.
        __primitive("=", data[i], eltCopy);

        i += 1;
      }
    } catch e {
      // Deinitialize any elements that have been initialized.
      for j in 0..i-1 {
        chpl__autoDestroy(data[j]);
      }
      // Free the allocated memory.
      _ddata_free(data, size);
      // Propagate the thrown error, but don't consider this
      // function throwing just because of this call.
      chpl__throwErrorUnchecked(e);
    }

    // Create the domain of the array that we will return.

    // This is a workaround for #11301, whereby we can get here even when
    // there is an exception in the above 'for elt' loop.
    // If not for that, we could probably assert(r.size == i).
    // Todo: what if _shape_ is unbounded? Can we reach this point somehow?
    if shapeful && i < r.sizeAs(uint) then
      r = r #i;

    if !shapeful then
      // return 1..0 (the empty range value) if the size is 0; 0.. #i otherwise
      // (unless capturedIteratorLowBound is overridden)
      r = if i == 0 then 1..0 else capturedIteratorLowBound .. #i;

    pragma "insert auto destroy"
    var D = { r };

    if data != nil {

      // let the comm layer adjust array allocation
      if callPostAlloc then
        _ddata_allocate_postalloc(data, size);

      // Now construct a DefaultRectangular array using the data
      pragma "no copy"
      var A = D.buildArrayWith(data[0].type, data, size:int);

      // Normally, the sub-arrays share a domain with the
      // parent, but that's not the case for the arrays created
      // by this routine. Instead, each sub-array may have its own domain.
      // That allows them to have different runtime sizes.
      chpl_decRefCountsForDomainsInArrayEltTypes(A._value, data[0].type);
      A._value._decEltRefCounts = false;

      // in lieu of automatic memory management for runtime types
      __primitive("auto destroy runtime type", elemType);

      return A;

    } else {
      // Construct and return an empty array.

      // Create space for 1 element as a placeholder.
      data = _ddata_allocate_noinit(elemType, size, callPostAlloc);
      if callPostAlloc then
        _ddata_allocate_postalloc(data, size);

      pragma "no copy"
      var A = D.buildArrayWith(elemType, data, size:int);

      // in lieu of automatic memory management for runtime types
      __primitive("auto destroy runtime type", elemType);

      return A;
    }
  }

  // used for passing arrays to extern procs, e.g.
  //   extern proc foo(X: []);
  //   var A: [1..3] real;
  //   foo(A);
  // 'castToVoidStar' says whether we should cast the result to c_ptr(void)

  proc chpl_arrayToPtrErrorHelper(const ref arr: []) {
    if (!arr.isRectangular() || !chpl_domainDistIsLayout(arr.domain)) then
      compilerError("Only single-locale rectangular arrays can be passed to an external routine argument with array type", errorDepth=3);

    if (arr._value.locale != here) then
      halt("An array can only be passed to an external routine from the locale on which it lives (array is on locale " + arr._value.locale.id:string + ", call was made on locale " + here.id:string + ")");
  }

  proc chpl_arrayToPtr(ref arr: [], param castToVoidStar: bool = false) {
    chpl_arrayToPtrErrorHelper(arr);

    use CTypes;
    const ptr = c_pointer_return(arr[arr.domain.low]);
    if castToVoidStar then
      return ptr: c_ptr(void);
    else
      return ptr;
  }
  proc chpl_arrayToPtrConst(const ref arr: [], param castToVoidStar: bool = false) {
    chpl_arrayToPtrErrorHelper(arr);

    use CTypes;
    const ptr = c_pointer_return_const(arr[arr.domain.low]);
    if castToVoidStar then
      return ptr: c_ptrConst(void);
    else
      return ptr;
  }
}
