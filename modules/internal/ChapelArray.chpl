/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
/* Operations on Domains and Arrays.

   =================================================
   Distribution, Domain and Array Equality operators
   =================================================

   Equality operators are defined to test if two distributions
   are equivalent or not:

   .. code-block:: chapel

     dist1 == dist2
     dist1 != dist2

   Or to test if two domains are equivalent or not:

   .. code-block:: chapel

     dom1 == dom2
     dom1 != dom2

   Arrays are promoted, so the result of the equality operators is
   an array of booleans.  To get a single result use the ``equals``
   method instead.

   .. code-block:: chapel

     arr1 == arr2 // compare each element resulting in an array of booleans
     arr1 != arr2 // compare each element resulting in an array of booleans
     arr1.equals(arr2) // compare entire arrays resulting in a single boolean

   ========================================
   Miscellaneous Domain and Array Operators
   ========================================

   The domain count operator ``#``
   -------------------------------

   The ``#`` operator can be applied to dense rectangular domains
   with a tuple argument whose size matches the rank of the domain
   (or optionally an integer in the case of a 1D domain). The operator
   is equivalent to applying the ``#`` operator to the component
   ranges of the domain and then using them to slice the domain.

   The array count operator ``#``
   ------------------------------
   The ``#`` operator can be applied to dense rectangular arrays
   with a tuple argument whose size matches the rank of the array
   (or optionally an integer in the case of a 1D array). The operator
   is equivalent to applying the ``#`` operator to the array's domain
   and using the result to slice the array.

   The array swap operator ``<=>``
   -------------------------------
   The ``<=>`` operator can be used to swap the contents of two arrays
   with the same shape.

   ================================================
   Set Operations on Associative Domains and Arrays
   ================================================

   Associative domains and arrays support a number of operators for
   set manipulations.  The supported set operators are:

     =======  ====================
     \+ , \|  Union
     &        Intersection
     \-       Difference
     ^        Symmetric Difference
     =======  ====================

   Consider the following code where ``A`` and ``B`` are associative arrays:

   .. code-block:: chapel

     var C = A op B;

   The result ``C`` is a new associative array backed by a new associative
   domain. The domains of ``A`` and ``B`` are not modified by ``op``.

   There are also ``op=`` variants that store the result into the first operand.

   Consider the following code where ``A`` and ``B`` are associative arrays:

   .. code-block:: chapel

     A op= B;

   ``A`` must not share its domain with another array, otherwise the program
   will halt with an error message.

   For the ``+=`` and ``|=`` operators, the value from ``B`` will overwrite
   the existing value in ``A`` when indices overlap.

   ==================================================
   Parallel Safety with respect to Arrays and Domains
   ==================================================

   Users must take care when applying operations to arrays and domains
   concurrently from distinct tasks.  For instance, if one task is
   modifying the index set of a domain while another task is operating
   on either the domain itself or an array declared over that domain,
   this represents a race and could have arbitrary consequences
   including incorrect results and program crashes.  While making
   domains and arrays safe with respect to such concurrent operations
   would be appealing, Chapel's current position is that such safety
   guarantees would be prohibitively expensive.

   Chapel arrays do support concurrent reads, writes, iterations, and
   operations as long as their domains are not being modified
   simultaneously.  Such operations are subject to Chapel's memory
   consistency model like any other memory accesses.  Similarly, tasks
   may make concurrent queries and iterations on a domain as long as
   another task is not simultaneously modifying the domain's index
   set.

   By default, associative domains permit multiple tasks
   to modify their index sets concurrently.  This adds some amount of
   overhead to these operations.  If the user knows that all such
   modifications will be done serially or in a parallel-safe context,
   the overheads can be avoided by setting ``parSafe`` to ``false`` in
   the domain's type declaration.  For example, the following
   declaration creates an associative domain of strings where the
   implementation will do nothing to ensure that simultaneous
   modifications to the domain are parallel-safe:

     .. code-block:: chapel

       var D: domain(string, parSafe=false);

   As with any other domain type, it is not safe to access an
   associative array while its domain is changing, regardless of
   whether ``parSafe`` is set to ``true`` or ``false``.

   ===========================================
   Functions and Methods on Arrays and Domains
   ===========================================

 */
module ChapelArray {

  use ChapelBase; // For opaque type.
  use ChapelTuple;
  use ChapelLocale;
  use ArrayViewSlice;
  use ArrayViewRankChange;
  use ArrayViewReindex;
  import Reflection;
  private use ChapelDebugPrint;
  private use SysCTypes;

  // Explicitly use a processor atomic, as most calls to this function are
  // likely be on locale 0
  pragma "no doc"
  var numPrivateObjects: chpl__processorAtomicType(int);
  pragma "no doc"
  param nullPid = -1;

  pragma "no doc"
  config param debugBulkTransfer = false;
  pragma "no doc"
  config param useBulkTransfer = true;
  pragma "no doc"
  config param useBulkTransferStride = true;
  pragma "no doc"
  config param useBulkPtrTransfer = useBulkTransfer;

  // Return POD values from arrays as values instead of const ref?
  pragma "no doc"
  config param PODValAccess = true;

  // Toggles the functionality to perform strided bulk transfers involving
  // distributed arrays.
  //
  // Currently disabled due to observations of higher communication counts
  // compared to element-by-element assignment.
  pragma "no doc"
  config param useBulkTransferDist = false;

  pragma "no doc" // no doc unless we decide to expose this
  config param arrayAsVecGrowthFactor = 1.5;
  pragma "no doc"
  config param debugArrayAsVec = false;

  proc _isPrivatized(value) param
    return !_local && ((_privatization && value!.dsiSupportsPrivatization()) ||
                       value!.dsiRequiresPrivatization());
    // Note - _local=true means --local / single locale
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

    const n = numPrivateObjects.fetchAdd(1);

    const hereID = here.id;
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
        if chpl_localeTree.left._instance != nil then
          on chpl_localeTree.left do
            _newPrivatizedClassHelp(newValue, originalValue, n, hereID, privatizeData);
        if chpl_localeTree.right._instance != nil then
          on chpl_localeTree.right do
            _newPrivatizedClassHelp(newValue, originalValue, n, hereID, privatizeData);
      }
    }

    return n;
  }

  // original is the value this method shouldn't free, because it's the
  // canonical version. The rest are copies on other locales.
  proc _freePrivatizedClass(pid:int, original:object):void
  {
    // Do nothing for null pids.
    if pid == nullPid then return;

    on Locales[0] {
      _freePrivatizedClassHelp(pid, original);
    }

    proc _freePrivatizedClassHelp(pid, original) {
      var prv = chpl_getPrivatizedCopy(unmanaged object, pid);
      if prv != original then
        delete prv;

      extern proc chpl_clearPrivatizedClass(pid:int);
      chpl_clearPrivatizedClass(pid);

      cobegin {
        if chpl_localeTree.left._instance != nil then
          on chpl_localeTree.left do
            _freePrivatizedClassHelp(pid, original);
        if chpl_localeTree.right._instance != nil then
          on chpl_localeTree.right do
            _freePrivatizedClassHelp(pid, original);
      }
    }
  }

  proc _reprivatize(value) {
    const pid = value.pid;
    const hereID = here.id;
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
        if chpl_localeTree.left._instance != nil then
          on chpl_localeTree.left do
            _reprivatizeHelp(newValue, originalValue, pid, hereID, reprivatizeData);
        if chpl_localeTree.right._instance != nil then
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
      for param i in 0..rank-1 do
        t(i) = val;
      return t;
    } else {
      compilerWarning(val.type:string);
      compilerError("index rank must match domain rank");
      return val;
    }
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

  proc _getDomain(value) {
    if _to_unmanaged(value.type) != value.type then
      compilerError("Domain on borrow created");

    if _isPrivatized(value) then
      return new _domain(value.pid, value, _unowned=true);
    else
      return new _domain(nullPid, value, _unowned=true);
  }

  proc _getDistribution(value) {
    if _isPrivatized(value) then
      return new _distribution(value.pid, value, _unowned=true);
    else
      return new _distribution(nullPid, value, _unowned=true);
  }

  // Run-time type support
  //
  // NOTE: the bodies of functions marked with runtime type init fn such as
  // chpl__buildDomainRuntimeType and chpl__buildArrayRuntimeType are replaced
  // by the compiler to just create a record storing the arguments.
  // The return type of chpl__build...RuntimeType is what tells the
  // compiler which runtime type it is creating.
  // These functions are considered type functions early in compilation.

  //
  // Support for domain types
  //
  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(dist: _distribution, param rank: int,
                                    type idxType = int,
                                    param stridable: bool = false) {
    return new _domain(dist, rank, idxType, stridable);
  }

  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(dist: _distribution, type idxType,
                                    param parSafe: bool = true) {
    return new _domain(dist, idxType, parSafe);
  }

  pragma "runtime type init fn"
  proc chpl__buildSparseDomainRuntimeType(dist: _distribution,
                                          parentDom: domain) {
    return new _domain(dist, parentDom);
  }

  proc chpl__convertRuntimeTypeToValue(dist: _distribution, param rank: int,
                                       type idxType = int,
                                       param stridable: bool) {
    return new _domain(dist, rank, idxType, stridable);
  }

  proc chpl__convertRuntimeTypeToValue(dist: _distribution, type idxType,
                                       param parSafe: bool) {
    return new _domain(dist, idxType, parSafe);
  }

  proc chpl__convertRuntimeTypeToValue(dist: _distribution, parentDom: domain) {
    return new _domain(dist, parentDom);
  }

  proc chpl__convertRuntimeTypeToValue(type t: domain) {
    compilerError("the global domain class of each domain map implementation must be a subclass of BaseRectangularDom, BaseAssociativeDom, or BaseSparseDom", 0);
    return 0; // dummy
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseRectangularDom) {
    return chpl__buildDomainRuntimeType(dom.dist, dom._value.rank,
                              dom._value.idxType, dom._value.stridable);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseAssociativeDom) {
    return chpl__buildDomainRuntimeType(dom.dist, dom._value.idxType, dom._value.parSafe);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseSparseDom) {
    return chpl__buildSparseDomainRuntimeType(dom.dist, dom._value.parentDom);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type {
    compilerError("the global domain class of each domain map implementation must be a subclass of BaseRectangularDom, BaseAssociativeDom, or BaseSparseDom", 0);
    return 0; // dummy
  }

  //
  // Support for array types
  //
  pragma "runtime type init fn"
  proc chpl__buildArrayRuntimeType(dom: domain, type eltType) {
    return dom.buildArray(eltType, true);
  }

  proc chpl__convertRuntimeTypeToValue(dom: domain, type eltType) {
    // TODO: add initElts argument
    return dom.buildArray(eltType, true);
  }

  proc chpl__convertValueToRuntimeType(arr: []) type {
    return chpl__buildArrayRuntimeType(arr.domain, arr.eltType);
  }


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
  pragma "no doc"
  config param CHPL_WARN_DOMAIN_LITERAL = "unset";

  // the low bound to use for array literals
  pragma "no doc"
  config param arrayLiteralLowBound = defaultLowBound;
  pragma "no doc"
  config param capturedIteratorLowBound = defaultLowBound;

  pragma "ignore transfer errors"
  proc chpl__buildArrayExpr( elems ...?k ) {

    if CHPL_WARN_DOMAIN_LITERAL == "true" && isRange(elems(0)) {
      compilerWarning("Encountered an array literal with range element(s).",
                      " Did you mean a domain literal here?",
                      " If so, use {...} instead of [...].");
    }

    // elements of string literals are assumed to be of type string
    type elemType = _getLiteralType(elems(0).type);
    pragma "unsafe" // 'elemType' can be non-nilable
    var A : [arrayLiteralLowBound..#k] elemType;  //This is unfortunate, can't use t here...

    for param i in 0..k-1 {
      type currType = _getLiteralType(elems(i).type);

      if currType != elemType {
        compilerError( "Array literal element " + i:string +
                       " expected to be of type " + elemType:string +
                       " but is of type " + currType:string );
      }

      A(i+arrayLiteralLowBound) = elems(i);
    }

    return A;
  }

  proc chpl__buildAssociativeArrayExpr( elems ...?k ) {
    type keyType = _getLiteralType(elems(0).type);
    type valType = _getLiteralType(elems(1).type);
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
      type elemKeyType = _getLiteralType(elemKey.type);
      type elemValType = _getLiteralType(elemVal.type);

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
  // Support for subdomain types
  //
  // Note the domain of a subdomain is not yet part of the runtime type
  //
  proc chpl__buildSubDomainType(dom: domain) type
    return chpl__convertValueToRuntimeType(dom);

  //
  // Support for domain expressions, e.g., {1..3, 1..3}
  //

  proc chpl__isTupleOfRanges(tup) param {
    for param i in 0..tup.size-1 {
      if !isRangeType(tup(i).type) then
        return false;
    }
    return true;
  }

  proc chpl__buildDomainExpr(ranges...)
  where chpl__isTupleOfRanges(ranges) {
    param rank = ranges.size;
    for param i in 1..rank-1 do
      if ranges(0).idxType != ranges(i).idxType then
        compilerError("idxType varies among domain's dimensions");
    for param i in 0..rank-1 do
      if ! isBoundedRange(ranges(i)) then
        compilerError("one of domain's dimensions is not a bounded range");
    var d: domain(rank, ranges(0).idxType, chpl__anyStridable(ranges));
    d.setIndices(ranges);
    return d;
  }

  proc chpl__buildDomainExpr(keys...) {
    param count = keys.size;
    // keyType of string literals is assumed to be type string
    type keyType = _getLiteralType(keys(0).type);
    for param i in 1..count-1 do
      if keyType != _getLiteralType(keys(i).type) {
        compilerError("Associative domain element " + i:string +
                      " expected to be of type " + keyType:string +
                      " but is of type " +
                      _getLiteralType(keys(i).type):string);
      }

    //Initialize the domain with a size appropriate for the number of keys.
    //This prevents resizing as keys are added.
    var D : domain(keyType);
    D.requestCapacity(count);

    for param i in 0..count-1 do
      D += keys(i);

    return D;
  }

  //
  // Support for domain expressions within array types, e.g. [1..n], [D]
  //
  proc chpl__ensureDomainExpr(const ref x: domain) const ref {
    return x;
  }

  // pragma is a workaround for ref-pair vs generic/specific overload resolution
  pragma "compiler generated"
  pragma "last resort"
  proc chpl__ensureDomainExpr(x...) {
    return chpl__buildDomainExpr((...x));
  }

  pragma "compiler generated"
  pragma "last resort"
  proc chpl__ensureDomainExpr(type t) {
    compilerError("Domain expression was a type ('", t:string, "') rather than a domain value or range list as expected");
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

  proc chpl__distributed(d: _distribution, ranges...)
  where chpl__isTupleOfRanges(ranges) {
    return chpl__distributed(d, chpl__buildDomainExpr((...ranges)));
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
      compilerError("Invalid argument for chpl__isDROrDRView");
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

  proc chpl__isRectangularDomType(type domainType) param {
    var dom: domainType;
    return isDomainType(domainType) && isRectangularDom(dom);
  }

  proc chpl__isSparseDomType(type domainType) param {
    var dom: domainType;
    return isSparseDom(dom);
  }

  pragma "return not owned"
  proc chpl__parentDomainFromDomainRuntimeType(type domainType) {
    pragma "no copy"
    pragma "no auto destroy"
    var parentDom = __primitive("get runtime type field",
                                domainType, "parentDom");

    return _getDomain(parentDom._value);
  }

  proc chpl__distributed(d: _distribution, type domainType) type {
    if !isDomainType(domainType) then
      compilerError("cannot apply 'dmapped' to the non-domain type ",
                    domainType:string);
    if chpl__isRectangularDomType(domainType) {
      var dom: domainType;
      return chpl__buildDomainRuntimeType(d, dom._value.rank, dom._value.idxType,
                                          dom._value.stridable);
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

  pragma "return not owned"
  proc chpl__domainFromArrayRuntimeType(type rtt) {
    pragma "no copy"
    pragma "no auto destroy"
    var dom = __primitive("get runtime type field", rtt, "dom");

    return _getDomain(dom._value);
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

  //
  // Support for index types
  //
  proc chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
    return idxType;
  }

  proc chpl__buildIndexType(param rank: int, type idxType) type where rank > 1 {
    return rank*idxType;
  }

  proc chpl__buildIndexType(param rank: int) type
    return chpl__buildIndexType(rank, int);

  proc chpl__buildIndexType(d: domain) type
    return chpl__buildIndexType(d.rank, d._value.idxType);

  /* Return true if the argument ``d`` is a rectangular domain.
     Otherwise return false.  */
  proc isRectangularDom(d: domain) param {
    return isSubtype(_to_borrowed(d._value.type), BaseRectangularDom);
  }

  /* Return true if the argument ``a`` is an array with a rectangular
     domain.  Otherwise return false. */
  proc isRectangularArr(a: []) param return isRectangularDom(a.domain);

  /* Return true if ``d`` is an irregular domain; e.g. is not rectangular.
     Otherwise return false. */
  proc isIrregularDom(d: domain) param {
    return isSparseDom(d) || isAssociativeDom(d);
  }

  /* Return true if ``a`` is an array with an irregular domain; e.g. not
     rectangular. Otherwise return false. */
  proc isIrregularArr(a: []) param return isIrregularDom(a.domain);

  /* Return true if ``d`` is an associative domain. Otherwise return false. */
  proc isAssociativeDom(d: domain) param {
    proc isAssociativeDomClass(dc: BaseAssociativeDom) param return true;
    proc isAssociativeDomClass(dc) param return false;
    return isAssociativeDomClass(_to_borrowed(d._value));
  }

  /* Return true if ``a`` is an array with an associative domain. Otherwise
     return false. */
  proc isAssociativeArr(a: []) param return isAssociativeDom(a.domain);

  /* Return true if ``d`` is a sparse domain. Otherwise return false. */
  proc isSparseDom(d: domain) param {
    proc isSparseDomClass(dc: BaseSparseDom) param return true;
    proc isSparseDomClass(dc) param return false;
    return isSparseDomClass(d._value);
  }

  /* Return true if ``a`` is an array with a sparse domain. Otherwise
     return false. */
  proc isSparseArr(a: []) param return isSparseDom(a.domain);

  // Helper function used to ensure a returned array matches the declared
  // return type when the declared return type specifies a particular domain
  // but not the element type.
  proc chpl__checkDomainsMatch(a: [], b) {
    import HaltWrappers;
    if (boundsChecking) {
      if (a.domain != b) {
        HaltWrappers.boundsCheckHalt("domain mismatch on return");
      }
    }
  }

  proc chpl__checkDomainsMatch(a: _iteratorRecord, b) {
    import HaltWrappers;
    if (boundsChecking) {
      // Should use iterator.shape here to avoid copy
      var tmp = a;
      if (tmp.domain != b) {
        HaltWrappers.boundsCheckHalt("domain mismatch on return");
      }
    }
  }

  // Helper function used to ensure a returned array matches the declared
  // return type when the declared return type specifies a particular element
  // type but not the domain
  proc chpl__checkEltTypeMatch(a: [], type b) {
    if (a.eltType != b) {
      compilerError("array element type mismatch in return from ",
                    a.eltType: string,
                    " to ",
                    b: string);
    }
  }

  proc chpl__checkEltTypeMatch(a: _iteratorRecord, type b) {
    type eltType = iteratorToArrayElementType(a.type);
    if (eltType != b) {
      compilerError("array element type mismatch in return from ",
                    eltType: string,
                    " to ",
                    b: string);
    }
  }

  //
  // Support for distributions
  //
  pragma "no doc"
  pragma "syntactic distribution"
  record dmap { }

  proc chpl__buildDistType(type t) type where isSubtype(_to_borrowed(t), BaseDist) {
    var x: _to_unmanaged(t)?;
    var y = new _distribution(x!);
    return y.type;
  }

  proc chpl__buildDistType(type t) {
    compilerError("illegal domain map type specifier - must be a subclass of BaseDist");
  }

  proc chpl__buildDistValue(x:unmanaged) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(x);
  }
  proc chpl__buildDistValue(in x:owned) where isSubtype(x.borrow().type, BaseDist) {
    return new _distribution(x.release());
  }

  proc chpl__buildDistValue(x) {
    compilerError("illegal domain map value specifier - must be a subclass of BaseDist");
  }

  //
  // Distribution wrapper record
  //
  pragma "distribution"
  pragma "ignore noinit"
  pragma "no doc"
  record _distribution {
    var _pid:int;  // only used when privatized
    pragma "owned"
    var _instance; // generic, but an instance of a subclass of BaseDist
    var _unowned:bool; // 'true' for the result of 'getDistribution',
                       // in which case, the record destructor should
                       // not attempt to delete the _instance.

    proc init(_pid : int, _instance, _unowned : bool) {
      this._pid      = _pid;
      this._instance = _instance;
      this._unowned  = _unowned;
    }

    proc init(value) {
      this._pid = if _isPrivatized(value) then _newPrivatizedClass(value) else nullPid;
      this._instance = _to_unmanaged(value);
    }

    // Note: This does not handle the case where the desired type of 'this'
    // does not match the type of 'other'. That case is handled by the compiler
    // via coercions.
    proc init=(const ref other : _distribution) {
      var value = other._value.dsiClone();
      this.init(value);
    }

    inline proc _value {
      if _isPrivatized(_instance) {
        return chpl_getPrivatizedCopy(_instance.type, _pid);
      } else {
        return _instance;
      }
    }

    forwarding _value except targetLocales;

    inline proc _do_destroy() {
      if ! _unowned && ! _instance.singleton() {
        on _instance {
          // Count the number of domains that refer to this distribution.
          // and mark the distribution to be freed when that number reaches 0.
          // If the number is 0, .remove() returns the distribution
          // that should be freed.
          var distToFree = _instance.remove();
          if distToFree != nil {
            _delete_dist(distToFree!, _isPrivatized(_instance));
          }
        }
      }
    }

    proc deinit() {
      _do_destroy();
    }

    proc clone() {
      return new _distribution(_value.dsiClone());
    }

    proc newRectangularDom(param rank: int, type idxType, param stridable: bool,
                           ranges: rank*range(idxType, BoundedRangeType.bounded,stridable)) {
      var x = _value.dsiNewRectangularDom(rank, idxType, stridable, ranges);
      if x.linksDistribution() {
        _value.add_dom(x);
      }
      return x;
    }

    proc newRectangularDom(param rank: int, type idxType, param stridable: bool) {
      var ranges: rank*range(idxType, BoundedRangeType.bounded, stridable);
      return newRectangularDom(rank, idxType, stridable, ranges);
    }

    proc newAssociativeDom(type idxType, param parSafe: bool=true) {
      var x = _value.dsiNewAssociativeDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
      }
      return x;
    }

    proc newSparseDom(param rank: int, type idxType, dom: domain) {
      var x = _value.dsiNewSparseDom(rank, idxType, dom);
      if x.linksDistribution() {
        _value.add_dom(x);
      }
      return x;
    }

    proc idxToLocale(ind) return _value.dsiIndexToLocale(ind);

    proc readWriteThis(f) throws {
      f <~> _value;
    }

    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    /*
       Return an array of locales over which this distribution was declared.
    */
    proc targetLocales() {
      return _value.dsiTargetLocales();
    }
  }  // record _distribution

  inline proc ==(d1: _distribution(?), d2: _distribution(?)) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  inline proc !=(d1: _distribution(?), d2: _distribution(?)) {
    if (d1._value == d2._value) then
      return false;
    return !d1._value.dsiEqualDMaps(d2._value);
  }


  // This alternative declaration of Sort.defaultComparator
  // prevents transitive use of module Sort.
  proc chpl_defaultComparator() {
    use Sort;
    return defaultComparator;
  }


  //
  // Domain wrapper record.
  //
  /* The domain type */
  pragma "domain"
  pragma "has runtime type"
  pragma "ignore noinit"
  record _domain {
    var _pid:int; // only used when privatized
    pragma "owned"
    var _instance; // generic, but an instance of a subclass of BaseDom
    var _unowned:bool; // 'true' for the result of 'getDomain'
                       // in which case, the record destructor should
                       // not attempt to delete the _instance.

    proc chpl__promotionType() type {
      return index(rank, _value.idxType);
    }

    proc init(_pid: int, _instance, _unowned: bool) {
      this._pid = _pid;
      this._instance = _instance;
      this._unowned = _unowned;
    }

    proc init(value) {
      if _to_unmanaged(value.type) != value.type then
        compilerError("Domain on borrow created");

      if _isPrivatized(value) {
        // the below check is necessary for iterator records that have domain
        // shapes would create another set of privatized instances otherwise
        if value.pid == nullPid {
          this._pid = _newPrivatizedClass(value);
        }
        else {
          this._pid = value.pid;
        }
      }
      else {
        this._pid = nullPid;
      }

      this._instance = value;
    }

    proc init(d: _distribution,
              param rank : int,
              type idxType = int,
              param stridable: bool = false) {
      this.init(d.newRectangularDom(rank, idxType, stridable));
    }

    proc init(d: _distribution,
              param rank : int,
              type idxType = int,
              param stridable: bool = false,
              ranges: rank*range(idxType, BoundedRangeType.bounded,stridable)) {
      this.init(d.newRectangularDom(rank, idxType, stridable, ranges));
    }

    proc init(d: _distribution,
              type idxType,
              param parSafe: bool = true) {
      this.init(d.newAssociativeDom(idxType, parSafe));
    }

    proc init(d: _distribution,
              dom: domain) {
      this.init(d.newSparseDom(dom.rank, dom._value.idxType, dom));
    }

    // Note: init= does not handle the case where the type of 'this' does not
    // handle the type of 'other'. That case is currently managed by the
    // compiler and various helper functions involving runtime types.
    proc init=(const ref other : domain) where isRectangularDom(other) {
      this.init(other.dist, other.rank, other.idxType, other.stridable, other.dims());
    }

    proc init=(const ref other : domain) {
      if isAssociativeDom(other) {
        this.init(other.dist, other.idxType, other.parSafe);
      } else if isSparseDom(other) {
        this.init(other.dist, other.parentDom);
      } else {
        compilerError("Cannot initialize '", this.type:string, "' from '", other.type:string, "'");
        this.init(nil);
      }

      // No need to lock this domain since it's not exposed anywhere yet.
      // No need to handle arrays over this domain either for the same reason.
      _instance.dsiAssignDomain(other, lhsPrivate=true);
    }

    inline proc _value {
      if _isPrivatized(_instance) {
        return chpl_getPrivatizedCopy(_instance.type, _pid);
      } else {
        return _instance;
      }
    }

    forwarding _value except chpl__serialize, chpl__deserialize;

    pragma "no doc"
    proc chpl__serialize()
      where this._value.isDefaultRectangular() {

      return this._value.chpl__serialize();
    }

    // TODO: we *SHOULD* be allowed to query the type of '_instance' directly
    // This function may not use any run-time type information passed to it
    // in the form of the 'this' argument. Static/param information is OK.
    pragma "no doc"
    proc type chpl__deserialize(data) {
      type valueType = __primitive("static field type", this, "_instance");
      return new _domain(_to_borrowed(valueType).chpl__deserialize(data));
    }

    pragma "no doc"
    proc _do_destroy () {
      if ! _unowned {
        on _instance {
          // Count the number of arrays that refer to this domain,
          // and mark the domain to be freed when that number reaches 0.
          // Additionally, if the number is 0, remove the domain from
          // the distribution and possibly get the distribution to free.
          const inst = _instance;
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
      else {
        // Engin 4/14/20: We don't have any arrayview domain instances that we
        // RVF today. If/when we do have them, we need to clean them up here.
        // See _do_destroy_arr for a similar cleanup we do today for arrayview
        // arrays.
      }
    }
    pragma "no doc"
    proc deinit () {
      _do_destroy();
    }

    /* Return the domain map that implements this domain */
    pragma "return not owned"
    proc dist return _getDistribution(_value.dist);

    /* Return the number of dimensions in this domain */
    proc rank param {
      if isRectangularDom(this) || isSparseDom(this) then
        return _value.rank;
      else
        return 1;
    }

    /* Return the type of the indices of this domain */
    proc idxType type {
      return _value.idxType;
    }

    /* The ``idxType`` as represented by an integer type.  When
       ``idxType`` is an enum type, this evaluates to ``int``.
       Otherwise, it evaluates to ``idxType``. */
    proc intIdxType type {
      return chpl__idxTypeToIntIdxType(_value.idxType);
    }

    /* Return true if this is a stridable domain */
    proc stridable param where isRectangularDom(this) {
      return _value.stridable;
    }

    pragma "no doc"
    proc stridable param where isSparseDom(this) {
      return _value.parentDom.stridable;
    }

    pragma "no doc"
    proc stridable param where isAssociativeDom(this) {
      compilerError("associative domains do not support .stridable");
    }

    /* Yield the domain indices */
    iter these() {
      for i in _value.these() {
        yield i;
      }
    }
    pragma "no doc"
    iter these(param tag: iterKind)
      where tag == iterKind.standalone &&
            __primitive("method call resolves", _value, "these", tag=tag) {
      for i in _value.these(tag) do
        yield i;
    }
    pragma "no doc"
    iter these(param tag: iterKind)
      where tag == iterKind.leader {
      // If I use forall here, it says
      //   error: user invocation of a parallel iterator should not supply tag
      //   arguments -- they are added implicitly by the compiler
      for followThis in _value.these(tag) do
        yield followThis;
    }
    pragma "no doc"
    iter these(param tag: iterKind, followThis, param fast: bool = false)
      where tag == iterKind.follower {

      if __primitive("method call resolves", _value, "these",
                     tag=tag, followThis, fast=fast) {
        for i in _value.these(tag=tag, followThis, fast=fast) do
          yield i;
      } else {
        for i in _value.these(tag, followThis) do
          yield i;
      }
    }


    // see comments for the same method in _array
    //
    // domain slicing by domain
    pragma "no doc"
    proc this(d: domain) {
      if d.rank == rank then
        return this((...d.getIndices()));
      else
        compilerError("slicing a domain with a domain of a different rank");
    }

    // domain slicing by tuple of ranges
    pragma "no doc"
    proc this(ranges...rank)
    where chpl__isTupleOfRanges(ranges) {
      param stridable = _value.stridable || chpl__anyStridable(ranges);
      var r: rank*range(_value.idxType,
                        BoundedRangeType.bounded,
                        stridable);

      for param i in 0..rank-1 {
        r(i) = _value.dsiDim(i)(ranges(i));
      }
      return new _domain(dist, rank, _value.idxType, stridable, r);
    }

    // domain rank change
    pragma "no doc"
    proc this(args ...rank) where _validRankChangeArgs(args, _value.idxType) {
      //
      // Compute which dimensions are collapsed and what the index
      // (idx) is in the event that it is.  These will be stored in
      // the array view to convert from lower-D indices to higher-D.
      // Also compute the upward-facing rank and tuple of ranges.
      //
      var collapsedDim: rank*bool;
      var idx: rank*idxType;
      param uprank = chpl__countRanges((...args));
      param upstridable = this.stridable || chpl__anyRankChangeStridable(args);
      var upranges: uprank*range(idxType=_value.idxType,
                                 stridable=upstridable);
      var updim = 0;

      for param i in 0..rank-1 {
        if (isRange(args(i))) {
          collapsedDim(i) = false;
          idx(i) = dim(i).alignedLow;
          upranges(updim) = this._value.dsiDim(i)[args(i)]; // intersect ranges
          updim += 1;
        } else {
          collapsedDim(i) = true;
          idx(i) = args(i);
        }
      }

      // Create distribution, domain, and array objects representing
      // the array view
      const emptyrange: upranges(0).type;
      //
      // If idx isn't in the original domain, we need to generate an
      // empty upward facing domain (intersection is empty)
      //
      if !contains(idx) {
        for param d in 0..uprank-1 do
          upranges(d) = emptyrange;
      }

      const rcdist = new unmanaged ArrayViewRankChangeDist(downDistPid=dist._pid,
                                                 downDistInst=dist._instance,
                                                 collapsedDim=collapsedDim,
                                                 idx = idx);
      // TODO: Should this be set?
      //rcdist._free_when_no_doms = true;

      const rcdistRec = new _distribution(rcdist);

      return new _domain(rcdistRec, uprank,
                                    upranges(0).idxType,
                                    upranges(0).stridable,
                                    upranges);
    }

    // error case for all-int access
    proc this(i: integral ... rank) {
      compilerError("domain slice requires a range in at least one dimension");
    }

    // anything that is not covered by the above
    pragma "no doc"
    proc this(args ...?numArgs) {
      if numArgs == rank {
        compilerError("invalid argument types for domain slicing");
      } else
        compilerError("a domain slice requires either a single domain argument or exactly one argument per domain dimension");
    }

    /*
       Return a tuple of ranges describing the bounds of a rectangular domain.
       For a sparse domain, return the bounds of the parent domain.
     */
    proc dims() return _value.dsiDims();

    /*
       Return a range representing the boundary of this
       domain in a particular dimension.
     */
    proc dim(d : int) {
      use HaltWrappers;
      if boundsChecking then
        if (d < 0 || d >= rank) then
          HaltWrappers.boundsCheckHalt("dim(" + d:string + ") is out-of-bounds; must be 0.." + (rank-1):string);
      return _value.dsiDim(d);
    }

    pragma "no doc"
    proc dim(param d : int) {
      if (d < 0 || d > rank-1) then
        compilerError("dim() must take a value from 0..", rank-1);
      return _value.dsiDim(d);
    }

    pragma "no doc"
    iter dimIter(param d, ind) {
      for i in _value.dimIter(d, ind) do yield i;
    }

   /* Return a tuple of :proc:`intIdxType` describing the size of each dimension.
      For a sparse domain, return the shape of the parent domain.*/
    proc shape where isRectangularDom(this) || isSparseDom(this) {
      var s: rank*(dim(0).intIdxType);
      for (i, r) in zip(0..#s.size, dims()) do
        s(i) = r.size;
      return s;
    }

    pragma "no doc"
    /* Associative domains assumed to be 1-D. */
    proc shape where isAssociativeDom(this) {
      var s: (size.type,);
      s[0] = size;
      return s;
    }

    pragma "no doc"
    /* Unsupported case */
    proc shape {
      compilerError(".shape not supported on this domain");
    }

    pragma "no doc"
    pragma "no copy return"
    proc buildArray(type eltType, param initElts:bool) {
      if eltType == void {
        compilerError("array element type cannot be void");
      }
      if isGenericType(eltType) {
        compilerWarning("creating an array with element type " +
                        eltType:string);
        if isClassType(eltType) && !isGenericType(borrowed eltType) {
          compilerWarning("which now means class type with generic management");
        }
        compilerError("array element type cannot currently be generic");
        // In the future we might support it if the array is not default-inited
      } else if isSparseDom(this) && !isDefaultInitializable(eltType) {
        if isNonNilableClass(eltType) {
          compilerError("sparse arrays of non-nilable classes are not currently supported");
        } else {
          compilerError("sparse arrays of non-default-initializable types are not currently supported");
        }
      }

      if chpl_warnUnstable then
        if isRectangularDom(this) && this.stridable then
          if rank == 1 {
            if this.stride < 0 then
              warning("arrays with negatively strided dimensions are not particularly stable");
          } else {
            for s in this.stride do
              if s < 0 then
                warning("arrays with negatively strided dimensions are not particularly stable");
          }

      var x = _value.dsiBuildArray(eltType, initElts);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);
    }

    pragma "no doc"
    pragma "no copy return"
    proc buildArrayWith(type eltType, data:_ddata(eltType), allocSize:int) {
      if eltType == void {
        compilerError("array element type cannot be void");
      }
      var x = _value.dsiBuildArrayWith(eltType, data, allocSize);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);
    }

    /* Remove all indices from this domain, leaving it empty */
    proc clear() where isRectangularDom(this) {
      // For rectangular domains, create an empty domain and assign it to this
      // one to make sure that we leverage all of the array's normal resizing
      // machinery.

      var emptyDom: this.type;
      this = emptyDom;
    }

    // For other domain types, the implementation probably knows the most
    // efficient way to clear its index set, so make a dsiClear() call.
    pragma "no doc"
    proc clear() {
      _value.dsiClear();
    }

    /* Add index ``i`` to this domain. This method is also available
       as the ``+=`` operator.

       The domain must be irregular.
     */
    proc add(in i) {
      return _value.dsiAdd(i);
    }

    pragma "no doc"
    proc bulkAdd(inds: [] _value.idxType, dataSorted=false,
        isUnique=false, preserveInds=true, addOn=nilLocale)
        where isSparseDom(this) && _value.rank==1 {

      if inds.size == 0 then return 0;

      return _value.dsiBulkAdd(inds, dataSorted, isUnique, preserveInds, addOn);
    }

    /*
     Creates an index buffer which can be used for faster index addition.

     For example, instead of:

       .. code-block:: chapel

          var spsDom: sparse subdomain(parentDom);
          for i in someIndexIterator() do
            spsDom += i;

     You can use `SparseIndexBuffer` for better performance:

       .. code-block:: chapel

          var spsDom: sparse subdomain(parentDom);
          var idxBuf = spsDom.makeIndexBuffer(size=N);
          for i in someIndexIterator() do
            idxBuf.add(i);
          idxBuf.commit();

     The above snippet will create a buffer of size N indices, and will
     automatically commit indices to the sparse domain as the buffer fills up.
     Indices are also committed when the buffer goes out of scope.

       .. note::

          The interface and implementation is not stable and may change in the
          future.

     :arg size: Size of the buffer in number of indices.
     :type size: int
    */
    inline proc makeIndexBuffer(size: int) {
      return _value.dsiMakeIndexBuffer(size);
    }

    /*
       Adds indices in ``inds`` to this domain in bulk.

       For sparse domains, an operation equivalent to this method is available
       with the ``+=`` operator, where the right-hand-side is an array. However,
       in that case, default values will be used for the flags ``dataSorted``,
       ``isUnique``, and ``preserveInds``. This method is available because in
       some cases, expensive operations can be avoided by setting those flags.
       To do so, ``bulkAdd`` must be called explicitly (instead of ``+=``).

       .. note::

         Right now, this method and the corresponding ``+=`` operator are
         only available for sparse domains. In the future, we expect that
         these methods will be available for all irregular domains.

       .. note::

         ``nilLocale`` is a sentinel value to denote that the locale where this
         addition should occur is unknown. We expect this to change in the
         future.

       :arg inds: Indices to be added. ``inds`` can be an array of
                  ``rank*idxType`` or an array of ``idxType`` for
                  1-D domains.

       :arg dataSorted: ``true`` if data in ``inds`` is sorted.
       :type dataSorted: bool

       :arg isUnique: ``true`` if data in ``inds`` has no duplicates.
       :type isUnique: bool

       :arg preserveInds: ``true`` if data in ``inds`` needs to be preserved.
       :type preserveInds: bool

       :arg addOn: The locale where the indices should be added. Default value
                   is ``nil`` which indicates that locale is unknown or there
                   are more than one.
       :type addOn: locale

       :returns: Number of indices added to the domain
       :rtype: int
    */
    proc bulkAdd(inds: [] _value.rank*_value.idxType,
        dataSorted=false, isUnique=false, preserveInds=true, addOn=nilLocale)
        where isSparseDom(this) && _value.rank>1 {

      if inds.size == 0 then return 0;

      return _value.dsiBulkAdd(inds, dataSorted, isUnique, preserveInds, addOn);
    }

    /* Remove index ``i`` from this domain */
    proc remove(i) {
      return _value.dsiRemove(i);
    }

    /* Request space for a particular number of values in an
       domain.

       Currently only applies to associative domains.
     */
    proc requestCapacity(i) {

      if i < 0 {
        halt("domain.requestCapacity can only be invoked on sizes >= 0");
      }

      if !isAssociativeDom(this) then
        compilerError("domain.requestCapacity only applies to associative domains");

      _value.dsiRequestCapacity(i);
    }

    /* Return the number of indices in this domain */
    proc size return _value.dsiNumIndices;
    /* Deprecated - please use :proc:`size`. */
    proc numIndices {
      compilerWarning("'domain.numIndices' is deprecated - " +
                      "please use 'domain.size' instead");
      return size;
    }
    /* Return the lowest index in this domain */
    proc low return _value.dsiLow;
    /* Return the highest index in this domain */
    proc high return _value.dsiHigh;
    /* Return the stride of the indices in this domain */
    proc stride return _value.dsiStride;
    /* Return the alignment of the indices in this domain */
    proc alignment return _value.dsiAlignment;
    /* Return the first index in this domain */
    proc first return _value.dsiFirst;
    /* Return the last index in this domain */
    proc last return _value.dsiLast;
    /* Return the low index in this domain factoring in alignment */
    proc alignedLow return _value.dsiAlignedLow;
    /* Return the high index in this domain factoring in alignment */
    proc alignedHigh return _value.dsiAlignedHigh;

    /* This error overload is here because without it, the domain's
       indices tend to be promoted across the `.indices` calls of
       their idxType which can be very confusing. */
    pragma "no doc"
    proc indices {
      compilerError("domains do not support '.indices'");
    }

    pragma "no doc"
    proc contains(i: rank*_value.idxType) {
      if isRectangularDom(this) || isSparseDom(this) then
        return _value.dsiMember(_makeIndexTuple(rank, i));
      else
        return _value.dsiMember(i(0));
    }

    /* Return true if this domain contains ``i``. Otherwise return false.
       For sparse domains, only indices with a value are considered
       to be contained in the domain.
     */
    inline proc contains(i: _value.idxType ...rank) {
      return contains(i);
    }

    pragma "no doc"
    inline proc member(i: rank*_value.idxType) {
      compilerWarning("domain.member is deprecated - " +
                      "please use domain.contains instead");
      return this.contains(i);
    }

    /* Deprecated - please use :proc:`contains`. */
    inline proc member(i: _value.idxType ...rank) {
      compilerWarning("domain.member is deprecated - " +
                      "please use domain.contains instead");
      return this.contains(i);
    }

    /* Return true if this domain is a subset of ``super``. Otherwise
       returns false. */
    proc isSubset(super : domain) {
      if !(isAssociativeDom(this) || isRectangularDom(this)) {
        if isSparseDom(this) then
          compilerError("isSubset not supported on sparse domains");
        else
          compilerError("isSubset not supported on this domain type");
      }
      if super.type != this.type then
        if isRectangularDom(this) {
          if super.rank != this.rank then
            compilerError("rank mismatch in domain.isSubset()");
          else if super.low.type != this.low.type then
            compilerError("isSubset called with different index types");
        } else
          compilerError("isSubset called with different associative domain types");

      if isRectangularDom(this) {
        var contains = true;
        for i in 0..this.dims().size-1 {
          contains &&= super.dims()[i].contains(this.dims()[i]);
          if contains == false then break;
        }
        return contains;
      }

      return && reduce forall i in this do super.contains(i);
    }

    /* Return true if this domain is a superset of ``sub``. Otherwise
       returns false. */
    proc isSuper(sub : domain) {
      if !(isAssociativeDom(this) || isRectangularDom(this)) {
        if isSparseDom(this) then
          compilerError("isSuper not supported on sparse domains");
        else
          compilerError("isSuper not supported on the domain type ", this.type);
      }
      if sub.type != this.type then
        if isRectangularDom(this) {
          if sub.rank != this.rank then
            compilerError("rank mismatch in domain.isSuper()");
          else if sub.low.type != this.low.type then
            compilerError("isSuper called with different index types");
        } else
          compilerError("isSuper called with different associative domain types");

      if isRectangularDom(this) {
        var contains = true;
        for i in 0..this.dims().size-1 {
          contains &&= this.dims()[i].contains(sub.dims()[i]);
          if contains == false then break;
        }
        return contains;
      }

      return && reduce forall i in sub do this.contains(i);
    }

    // 1/5/10: do we want to support order() and position()?
    pragma "no doc"
    proc indexOrder(i) return _value.dsiIndexOrder(_makeIndexTuple(rank, i));

    pragma "no doc"
    proc position(i) {
      var ind = _makeIndexTuple(rank, i), pos: rank*intIdxType;
      for d in 0..rank-1 do
        pos(d) = _value.dsiDim(d).indexOrder(ind(d));
      return pos;
    }

    pragma "no doc"
    proc expand(off: rank*intIdxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("expand not supported on associative domains");
      else if isSparseDom(this) then
        compilerError("expand not supported on sparse domains");
      else
        compilerError("expand not supported on this domain type");
    }

    pragma "no doc"
    proc expand(off: intIdxType ...rank) return expand(off);

    /* Return a new domain that is the current domain expanded by
       ``off(d)`` in dimension ``d`` if ``off(d)`` is positive or
       contracted by ``off(d)`` in dimension ``d`` if ``off(d)``
       is negative. */
    proc expand(off: rank*intIdxType) {
      var ranges = dims();
      for i in 0..rank-1 do {
        ranges(i) = ranges(i).expand(off(i));
        if (ranges(i).low > ranges(i).high) {
          halt("***Error: Degenerate dimension created in dimension ", i, "***");
        }
      }

      return new _domain(dist, rank, _value.idxType, stridable, ranges);
    }

    /* Return a new domain that is the current domain expanded by
       ``off`` in all dimensions if ``off`` is positive or contracted
       by ``off`` in all dimensions if ``off`` is negative. */
    proc expand(off: intIdxType) where rank > 1 {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).expand(off);
      return new _domain(dist, rank, _value.idxType, stridable, ranges);
    }

    pragma "no doc"
    proc exterior(off: rank*intIdxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("exterior not supported on associative domains");
      else if isSparseDom(this) then
        compilerError("exterior not supported on sparse domains");
      else
        compilerError("exterior not supported on this domain type");
    }

    pragma "no doc"
    proc exterior(off: intIdxType ...rank) return exterior(off);

    /* Return a new domain that is the exterior portion of the
       current domain with ``off(d)`` indices for each dimension ``d``.
       If ``off(d)`` is negative, compute the exterior from the low
       bound of the dimension; if positive, compute the exterior
       from the high bound. */
    proc exterior(off: rank*intIdxType) {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).exterior(off(i));
      return new _domain(dist, rank, _value.idxType, stridable, ranges);
    }

    /* Return a new domain that is the exterior portion of the
       current domain with ``off`` indices for each dimension.
       If ``off`` is negative, compute the exterior from the low
       bound of the dimension; if positive, compute the exterior
       from the high bound. */
    proc exterior(off:intIdxType) where rank != 1 {
      var offTup: rank*intIdxType;
      for i in 0..rank-1 do
        offTup(i) = off;
      return exterior(offTup);
    }

    pragma "no doc"
    proc interior(off: rank*intIdxType) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("interior not supported on associative domains");
      else if isSparseDom(this) then
        compilerError("interior not supported on sparse domains");
      else
        compilerError("interior not supported on this domain type");
    }

    pragma "no doc"
    proc interior(off: intIdxType ...rank) return interior(off);

    /* Return a new domain that is the interior portion of the
       current domain with ``off(d)`` indices for each dimension
       ``d``. If ``off(d)`` is negative, compute the interior from
       the low bound of the dimension; if positive, compute the
       interior from the high bound. */
    proc interior(off: rank*intIdxType) {
      var ranges = dims();
      for i in 0..rank-1 do {
        if ((off(i) > 0) && (dim(i)._high+1-off(i) < dim(i)._low) ||
            (off(i) < 0) && (dim(i)._low-1-off(i) > dim(i)._high)) {
          halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
        }
        ranges(i) = _value.dsiDim(i).interior(off(i));
      }
      return new _domain(dist, rank, _value.idxType, stridable, ranges);
    }

    /* Return a new domain that is the interior portion of the
       current domain with ``off`` indices for each dimension.
       If ``off`` is negative, compute the interior from the low
       bound of the dimension; if positive, compute the interior
       from the high bound. */
    proc interior(off: intIdxType) where rank != 1 {
      var offTup: rank*intIdxType;
      for i in 0..rank-1 do
        offTup(i) = off;
      return interior(offTup);
    }

    //
    // NOTE: We eventually want to support translate on other domain types
    //
    pragma "no doc"
    proc translate(off) where !isRectangularDom(this) {
      if isAssociativeDom(this) then
        compilerError("translate not supported on associative domains");
      else if isSparseDom(this) then
        compilerError("translate not supported on sparse domains");
      else
        compilerError("translate not supported on this domain type");
    }

    //
    // Notice that the type of the offset does not have to match the
    // index type.  This is handled in the range.translate().
    //
    pragma "no doc"
    proc translate(off...rank) return translate(off);

    /* Return a new domain that is the current domain translated by
       ``off(d)`` in each dimension ``d``. */
    proc translate(off) where isTuple(off) {
      if off.size != rank then
        compilerError("the domain and offset arguments of translate() must be of the same rank");
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = _value.dsiDim(i).translate(off(i));
      return new _domain(dist, rank, _value.idxType, stridable, ranges);
     }

    /* Return a new domain that is the current domain translated by
       ``off`` in each dimension. */
     proc translate(off) where rank != 1 && !isTuple(off) {
       var offTup: rank*off.type;
       for i in 0..rank-1 do
         offTup(i) = off;
       return translate(offTup);
     }

     /* Return true if the domain has no indices */
     proc isEmpty(): bool {
       return this.size == 0;
     }

    //
    // intended for internal use only:
    //
    proc chpl__unTranslate(off: integral ...rank) return chpl__unTranslate(off);
    proc chpl__unTranslate(off: rank*intIdxType) {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).chpl__unTranslate(off(i));
      return new _domain(dist, rank, _value.idxType, stridable, ranges);
    }

    pragma "no doc"
    proc setIndices(x) {
      _value.dsiSetIndices(x);
      if _isPrivatized(_instance) {
        _reprivatize(_value);
      }
    }

    pragma "no doc"
    proc getIndices()
      return _value.dsiGetIndices();

    pragma "no doc"
    proc writeThis(f) throws {
      _value.dsiSerialWrite(f);
    }

    pragma "no doc"
    proc readThis(f) throws {
      _value.dsiSerialRead(f);
    }

    pragma "no doc"
    proc localSlice(r... rank)
    where chpl__isTupleOfRanges(r) &&
          _value.isDefaultRectangular()
    {
      if (_value.locale != here) then
        halt("Attempting to take a local slice of a domain on locale ",
             _value.locale.id, " from locale ", here.id);
      return this((...r));
    }

    /*
       Return a local view of the sub-array (slice) defined by the provided
       range(s), halting if the slice contains elements that are not local.

       Indexing into this local view is cheaper, because the indices are known
       to be local.
    */
    proc localSlice(r... rank)
    where chpl__isTupleOfRanges(r) &&
          !_value.isDefaultRectangular()
    {
      return _value.dsiLocalSlice(chpl__anyStridable(r), r);
    }

    /*
       Return a local view of the sub-array (slice) defined by the provided
       domain, halting if the slice contains elements that are not local.

       Indexing into this local view is cheaper, because the indices are known
       to be local.
     */
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }

    // associative array interface
    /* Yield the domain indices in sorted order */
    iter sorted(comparator:?t = chpl_defaultComparator()) {
      for i in _value.dsiSorted(comparator) {
        yield i;
      }
    }

    pragma "no doc"
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    pragma "no doc"
    proc defaultSparseDist {
      // For now, this function just returns the same distribution
      // as the dense one. That works for:
      //  * sparse subdomains of defaultDist arrays (they use defaultDist)
      //  * sparse subdomains of Block distributed arrays (they use Block)
      // However, it is likely that DSI implementations will need to be
      // able to further customize this behavior. In particular, we
      // could add e.g. dsiDefaultSparseDist to the DSI interface
      // and have this function use _value.dsiDefaultSparseDist()
      // (or perhaps _value.dist.dsiDefaultSparseDist() ).
      return _getDistribution(_value.dist);
    }

    /* Cast a rectangular domain to another rectangular domain type.
       If the old type is stridable and the new type is not stridable,
       ensure that the stride was 1.
     */
    proc safeCast(type t:_domain)
      where chpl__isRectangularDomType(t) && isRectangularDom(this) {
      const tmpD: t;
      if tmpD.rank != this.rank then
        compilerError("rank mismatch in cast");
      if tmpD.idxType != this.idxType then
        compilerError("idxType mismatch in cast");
      if tmpD.stridable == this.stridable then
        return this;
      else if !tmpD.stridable && this.stridable {
        const inds = this.getIndices();
        var unstridableInds: rank*range(tmpD.idxType, stridable=false);

        for param dim in 0..inds.size-1 {
          if inds(dim).stride != 1 then
            halt("non-stridable domain assigned non-unit stride in dimension ", dim);
          unstridableInds(dim) = inds(dim).safeCast(range(tmpD.idxType,
                                                          stridable=false));
        }
        tmpD.setIndices(unstridableInds);
        return tmpD;
      } else /* if tmpD.stridable && !this.stridable */ {
        tmpD = this;
        return tmpD;
      }
    }

    /*
       Return an array of locales over which this domain has been distributed.
    */
    proc targetLocales() {
      return _value.dsiTargetLocales();
    }

    /* Return true if the local subdomain can be represented as a single
       domain. Otherwise return false. */
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
        compilerError("Domain's local domain is not a single domain");

      return _value.dsiLocalSubdomain(loc);
    }

    /*
       Yield the subdomains that are local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    iter localSubdomains(loc: locale = here) {
      if _value.dsiHasSingleLocalSubdomain() {
        yield localSubdomain(loc);
      } else {
        for d in _value.dsiLocalSubdomains(loc) do yield d;
      }
    }

    proc supportsAutoLocalAccess() param {
      return _value.dsiSupportsAutoLocalAccess();
    }

  }  // record _domain

  /* Cast a rectangular domain to a new rectangular domain type.  If the old
     type was stridable and the new type is not stridable then assume the
     stride was 1 without checking.

     For example:
     {1..10 by 2}:domain(stridable=false)

     results in the domain '{1..10}'
   */
  pragma "no doc"
  proc _cast(type t:_domain, d: _domain) where chpl__isRectangularDomType(t) && isRectangularDom(d) {
    const tmpD: t;
    if tmpD.rank != d.rank then
      compilerError("rank mismatch in cast");
    if tmpD.idxType != d.idxType then
      compilerError("idxType mismatch in cast");

    if tmpD.stridable == d.stridable then
      return d;
    else if !tmpD.stridable && d.stridable {
      var inds = d.getIndices();
      var unstridableInds: d.rank*range(tmpD.idxType, stridable=false);

      for param i in 0..tmpD.rank-1 {
        unstridableInds(i) = inds(i):range(tmpD.idxType, stridable=false);
      }
      tmpD.setIndices(unstridableInds);
      return tmpD;
    } else /* if tmpD.stridable && !d.stridable */ {
      tmpD = d;
      return tmpD;
    }
  }

  proc _cast(type t:string, x: _domain) {
    use IO;
    return stringify(x);
  }

  proc chpl_countDomHelp(dom, counts) {
    var ranges = dom.dims();
    for param i in 0..dom.rank-1 do
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

  pragma "fn returns aliasing array"
  proc #(arr: [], counts: integral) where isRectangularArr(arr) && arr.rank == 1 {
    return arr[arr.domain#counts];
  }

  pragma "fn returns aliasing array"
  proc #(arr: [], counts) where isRectangularArr(arr) && isTuple(counts) {
    if (counts.size != arr.rank) then
      compilerError("the domain and array arguments of # must have the same rank");
    return arr[arr.domain#counts];
  }

  proc +(d: domain, i: index(d)) {
    if isRectangularDom(d) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  proc +(i, d: domain) where isSubtype(i.type, index(d)) {
    if isRectangularDom(d) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  proc +(d: domain, i: index(d)) where isIrregularDom(d) {
    d.add(i);
    return d;
  }

  proc +(i, d: domain) where isSubtype(i.type,index(d)) && isIrregularDom(d) {
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
                                   (isSparseDom(d1)) {
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
    if d1._value.rank != d2._value.rank {
      return false;
    } else if d1._value == d2._value {
      return true;
    } else {
      for param i in 0..d1._value.rank-1 do
        if (d1.dim(i) != d2.dim(i)) then return false;
      return true;
    }
  }

  inline proc !=(d1: domain, d2: domain) where isRectangularDom(d1) &&
                                                        isRectangularDom(d2) {
    if d1._value.rank != d2._value.rank {
      return true;
    } else if d1._value == d2._value {
      return false;
    } else {
      for param i in 0..d1._value.rank-1 do
        if (d1.dim(i) != d2.dim(i)) then return true;
      return false;
    }
  }

  inline proc ==(d1: domain, d2: domain) where (isAssociativeDom(d1) &&
                                                         isAssociativeDom(d2)) {
    if d1._value == d2._value then return true;
    if d1.size != d2.size then return false;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return false;
    return true;
  }

  inline proc !=(d1: domain, d2: domain) where (isAssociativeDom(d1) &&
                                                         isAssociativeDom(d2)) {
    if d1._value == d2._value then return false;
    if d1.size != d2.size then return true;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return true;
    return false;
  }

  inline proc ==(d1: domain, d2: domain) where (isSparseDom(d1) &&
                                                         isSparseDom(d2)) {
    if d1._value == d2._value then return true;
    if d1.size != d2.size then return false;
    if d1._value.parentDom != d2._value.parentDom then return false;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return false;
    return true;
  }

  inline proc !=(d1: domain, d2: domain) where (isSparseDom(d1) &&
                                                         isSparseDom(d2)) {
    if d1._value == d2._value then return false;
    if d1.size != d2.size then return true;
    if d1._value.parentDom != d2._value.parentDom then return true;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return true;
    return false;
  }

  // any combinations not handled by the above

  inline proc ==(d1: domain, d2: domain) param {
    return false;
  }

  inline proc !=(d1: domain, d2: domain) param {
    return true;
  }

  pragma "no doc"
  proc shouldReturnRvalueByConstRef(type t) param {
    return true;
  }
  pragma "no doc"
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
  record _array {
    var _pid:int;  // only used when privatized
    pragma "owned"
    pragma "alias scope from this"
    var _instance; // generic, but an instance of a subclass of BaseArr
    var _unowned:bool;

    proc chpl__serialize() where _instance.chpl__rvfMe() {
      return _instance.chpl__serialize();
    }

    pragma "no doc"
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

    pragma "no doc"
    proc deinit() {
      _do_destroy_array(this);
    }

    /* The type of elements contained in the array */
    proc eltType type return _value.eltType;

    /* The type of indices used in the array's domain */
    proc idxType type return _value.idxType;

    pragma "return not owned"
    proc _dom return _getDomain(_value.dom);

    /* The number of dimensions in the array */
    proc rank param return this.domain.rank;

    /* return the array's indices as its domain */
    pragma "return not owned"
    proc indices
      return _dom;

    // bounds checking helpers
    pragma "insert line file info"
    pragma "always propagate line file info"
    pragma "no doc"
    proc checkAccess(indices, value) {
      if isRectangularArr(this) {
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
              if !value.dom.dsiDim(i).boundsCheck(indices(i)) {
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
    pragma "no doc"
    proc checkSlice(d: domain, value) {
      checkSlice((...d.dsiDims()), value=value);
    }

    pragma "insert line file info"
    pragma "always propagate line file info"
    pragma "no doc"
    proc checkSlice(ranges...rank, value) where chpl__isTupleOfRanges(ranges) {
      if isRectangularArr(this) {
        var ok = true;
        for param i in 0..rank-1 {
          ok &&= value.dom.dsiDim(i).boundsCheck(ranges(i));
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
              if !value.dom.dsiDim(i).boundsCheck(ranges(i)) {
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

    pragma "no doc" // ref version
    pragma "reference to const when const this"
    pragma "removable array access"
    pragma "alias scope from this"
    inline proc ref this(i: rank*_value.dom.idxType) ref {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }
    pragma "no doc" // value version, for POD types
    pragma "alias scope from this"
    inline proc const this(i: rank*_value.dom.idxType)
    where shouldReturnRvalueByValue(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }
    pragma "no doc" // const ref version, for not-POD types
    pragma "alias scope from this"
    inline proc const this(i: rank*_value.dom.idxType) const ref
    where shouldReturnRvalueByConstRef(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiAccess(i);
      else
        return value.dsiAccess(i(0));
    }



    pragma "no doc" // ref version
    pragma "reference to const when const this"
    pragma "removable array access"
    pragma "alias scope from this"
    inline proc ref this(i: _value.dom.idxType ...rank) ref
      return this(i);

    pragma "no doc" // value version, for POD types
    pragma "alias scope from this"
    inline proc const this(i: _value.dom.idxType ...rank)
    where shouldReturnRvalueByValue(_value.eltType)
      return this(i);

    pragma "no doc" // const ref version, for not-POD types
    pragma "alias scope from this"
    inline proc const this(i: _value.dom.idxType ...rank) const ref
    where shouldReturnRvalueByConstRef(_value.eltType)
      return this(i);


    pragma "no doc" // ref version
    pragma "reference to const when const this"
    pragma "alias scope from this"
    inline proc ref localAccess(i: rank*_value.dom.idxType) ref
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiLocalAccess(i);
      else
        return value.dsiLocalAccess(i(0));
    }
    pragma "no doc" // value version, for POD types
    pragma "alias scope from this"
    inline proc const localAccess(i: rank*_value.dom.idxType)
    where shouldReturnRvalueByValue(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiLocalAccess(i);
      else
        return value.dsiLocalAccess(i(0));
    }
    pragma "no doc" // const ref version, for not-POD types
    pragma "alias scope from this"
    inline proc const localAccess(i: rank*_value.dom.idxType) const ref
    where shouldReturnRvalueByConstRef(_value.eltType)
    {
      const value = _value;
      if boundsChecking then
        checkAccess(i, value=value);

      if isRectangularArr(this) || isSparseArr(this) then
        return value.dsiLocalAccess(i);
      else
        return value.dsiLocalAccess(i(0));
    }



    pragma "no doc" // ref version
    pragma "reference to const when const this"
    pragma "alias scope from this"
    inline proc ref localAccess(i: _value.dom.idxType ...rank) ref
      return localAccess(i);

    pragma "no doc" // value version, for POD types
    pragma "alias scope from this"
    inline proc const localAccess(i: _value.dom.idxType ...rank)
    where shouldReturnRvalueByValue(_value.eltType)
      return localAccess(i);

    pragma "no doc" // const ref version, for not-POD types
    pragma "alias scope from this"
    inline proc const localAccess(i: _value.dom.idxType ...rank) const ref
    where shouldReturnRvalueByConstRef(_value.eltType)
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
    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc this(d: domain) {
      if d.rank != rank then
        compilerError("slicing an array with a domain of a different rank");

      if boundsChecking then
        checkSlice(d, _value);

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
    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc this(ranges...rank) where chpl__isTupleOfRanges(ranges) {
      if boundsChecking then
        checkSlice((... ranges), value=_value);

      pragma "no auto destroy" var d = _dom((...ranges));
      d._value._free_when_no_arrs = true;

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
    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc this(args ...rank) where _validRankChangeArgs(args, _value.dom.idxType) {
      if boundsChecking then
        checkRankChange(args);

      const rcdom = this.domain[(...args)];

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
                                         idx=rcdom._value.idx);

      // this doesn't need to lock since we just created the domain d
      rcdom._value.add_arr(a, locking=false);
      return _newArray(a);
    }

    pragma "no doc"
    proc checkRankChange(args) {
      for param i in 0..args.size-1 do
        if !_value.dom.dsiDim(i).boundsCheck(args(i)) then
          halt("array slice out of bounds in dimension ", i, ": ", args(i));
    }

    // Special cases of local slices for DefaultRectangularArrs because
    // we can't take an alias of the ddata class within that class
    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc localSlice(r... rank)
    where isSubtype(_value.type, DefaultRectangularArr) &&
          chpl__isTupleOfRanges(r) {
      if boundsChecking then
        checkSlice((...r), value=_value);
      var dom = _dom((...r));
      return chpl__localSliceDefaultArithArrHelp(dom);
    }

    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc localSlice(d: domain)
    where isSubtype(_value.type, DefaultRectangularArr) {
      if boundsChecking then
        checkSlice((...d.getIndices()), value=_value);

      return chpl__localSliceDefaultArithArrHelp(d);
    }

    pragma "no copy return"
    proc chpl__localSliceDefaultArithArrHelp(d: domain) {
      if (_value.locale != here) then
        halt("Attempting to take a local slice of an array on locale ",
             _value.locale.id, " from locale ", here.id);
      return this(d);
    }
    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
    proc localSlice(r... rank)
    where chpl__isTupleOfRanges(r) &&
          !isSubtype(_value.type, DefaultRectangularArr) {
      if boundsChecking then
        checkSlice((...r), value=_value);
      return _value.dsiLocalSlice(r);
    }

    pragma "no doc"
    pragma "reference to const when const this"
    pragma "fn returns aliasing array"
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

    pragma "no doc"
    pragma "reference to const when const this"
    iter these(param tag: iterKind) ref
      where tag == iterKind.standalone &&
            __primitive("method call resolves", _value, "these", tag=tag) {
      for i in _value.these(tag) do
        yield i;
    }
    pragma "no doc"
    iter these(param tag: iterKind)
      where tag == iterKind.leader {
      for followThis in _value.these(tag) do
        yield followThis;
    }
    pragma "no doc"
    pragma "reference to const when const this"
    iter these(param tag: iterKind, followThis, param fast: bool = false) ref
      where tag == iterKind.follower {

      if __primitive("method call resolves", _value, "these",
                     tag=tag, followThis, fast=fast) {
        for i in _value.these(tag=tag, followThis, fast=fast) do
          yield i;
      } else {
        for i in _value.these(tag, followThis) do
          yield i;
      }
    }

    /* Deprecated - please use :proc:`size`. */
    proc numElements {
      compilerWarning("'array.numElements' is deprecated - " +
                      "please use 'array.size' instead");
      return size;
    }
    /* Return the number of elements in the array */
    proc size return _value.dom.dsiNumIndices;

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
      if (formalDom.dist._value.type != unmanaged DefaultDist) {
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
        if (runtimeChecks && formalDom.dist != this.domain.dist) then
          halt("Domain map mismatch passing array argument:\n",
               "  Formal domain map is: ", formalDom.dist, "\n",
               "  Actual domain map is: ", this.domain.dist);
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
      where isRectangularDom(this.domain) && isRectangularDom(newDomain)
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
      where isRectangularDom(this.domain)
    {
      for param i in 0..newDims.size-1 do
        if !isRange(newDims(i)) then
          compilerError("cannot reindex() a rectangular array to a tuple containing non-ranges");

      if this.rank != newDims.size then
        compilerError("rank mismatch: cannot reindex() from " + this.rank:string +
                      " dimension(s) to " + newDims.size:string);

      for param i in 0..rank-1 do
        if newDims(i).size != _value.dom.dsiDim(i).size then
          halt("extent in dimension ", i, " does not match actual");

      const thisDomClass = this._value.dom;
      const (dom, dompid) = (thisDomClass, thisDomClass.pid);

      pragma "no auto destroy"
      const updom = {(...newDims)};


      const redist = new unmanaged ArrayViewReindexDist(downDistPid = this.domain.dist._pid,
                                              downDistInst=this.domain.dist._instance,
                                              updom = updom._value,
                                              downdomPid = dompid,
                                              downdomInst = dom);
      const redistRec = new _distribution(redist);
      // redist._free_when_no_doms = true;

      pragma "no auto destroy" const newDom = new _domain(redistRec, rank, updom.idxType, updom.stridable, updom.dims());
      newDom._value._free_when_no_arrs = true;

      // TODO: With additional effort, we could collapse reindexings of
      // reindexed array views to a single array view, similar to what
      // we do for slices.
      const (arr, arrpid) = (this._value, this._pid);

      var x = new unmanaged ArrayViewReindexArr(eltType=this.eltType,
                                      _DomPid = newDom._pid,
                                      dom = newDom._instance,
                                      _ArrPid=arrpid,
                                      _ArrInstance=arr);
      // this doesn't need to lock since we just created the domain d
      newDom._value.add_arr(x, locking=false);
      return _newArray(x);
    }

    // reindex for all non-rectangular domain types.
    // See above for the rectangular version.
    pragma "no doc"
    pragma "fn returns aliasing array"
    proc reindex(d:domain) {
      compilerError("Reindexing non-rectangular arrays is not permitted.");
    }

    pragma "no doc"
    proc writeThis(f) throws {
      _value.dsiSerialWrite(f);
    }

    pragma "no doc"
    proc readThis(f) throws {
      _value.dsiSerialRead(f);
    }

    // sparse array interface
    /* Return the Implicitly Represented Value for sparse arrays */
    proc IRV where !isSparseArr(this) {
      compilerError("only sparse arrays have an IRV");
    }

    pragma "no doc"
    proc IRV ref where isSparseArr(this) {
      return _value.IRV;
    }

    /* Yield the array elements in sorted order. */
    iter sorted(comparator:?t = chpl_defaultComparator()) {
      if Reflection.canResolveMethod(_value, "dsiSorted", comparator) {
        for i in _value.dsiSorted(comparator) {
          yield i;
        }
      } else if Reflection.canResolveMethod(_value, "dsiSorted") {
        compilerError(_value.type:string + " does not support dsiSorted(comparator)");
      } else {
        use Sort;
        var copy = this;
        sort(copy, comparator=comparator);
        for ind in copy do
          yield ind;
      }
    }

    pragma "no doc"
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    /*
       Return an array of locales over which this array has been distributed.
    */
    proc targetLocales() {
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
        compilerError("Domain's local domain is not a single domain");

      return _value.dsiLocalSubdomain(loc);
    }

    /*
       Yield the subdomains that are local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    iter localSubdomains(loc: locale = here) {
      if _value.dsiHasSingleLocalSubdomain() {
        yield localSubdomain(loc);
      } else {
        for d in _value.dsiLocalSubdomains(loc) do yield d;
      }
    }

    proc chpl__isDense1DArray() param {
      return isRectangularArr(this) &&
             this.rank == 1 &&
             !this._value.stridable;
    }

    inline proc chpl__assertSingleArrayDomain(fnName: string) {
      if this.domain._value._arrs.size != 1 then
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
     */

    /* Return true if the array has no elements */
    proc isEmpty(): bool {
      return this.size == 0;
    }

    /* Return the first value in the array */
    // The return type used here is currently not pretty in the generated
    // documentation. Don't document it for now.
    pragma "no doc"
    proc head(): this._value.eltType {
      return this[this.domain.alignedLow];
    }

    /* Return the last value in the array */
    // The return type used here is currently not pretty in the generated
    // documentation. Don't document it for now.
    pragma "no doc"
    proc tail(): this._value.eltType {
      return this[this.domain.alignedHigh];
    }

    /* Return the last element in the array. The array must be a
       rectangular 1-D array.
     */
    proc back() {
      if !isRectangularArr(this) || this.rank != 1 then
        compilerError("back() is only supported on 1D rectangular arrays");

      if boundsChecking && isEmpty() then
        halt("back called on an empty array");

      return this(this.domain.high);
    }

    /* Return the first element in the array. The array must be a
       rectangular 1-D array.
     */
    proc front() {
      if !isRectangularArr(this) || this.rank != 1 then
        compilerError("front() is only supported on 1D rectangular arrays");

      if boundsChecking && isEmpty() then
        halt("front called on an empty array");

      return this(this.domain.low);
    }

    /* Reverse the order of the values in the array. */
    proc reverse() {
      if (!chpl__isDense1DArray()) then
        compilerError("reverse() is only supported on dense 1D arrays");
      const lo = this.domain.low,
            mid = this.domain.size / 2,
            hi = this.domain.high;
      for i in 0..#mid {
        this[lo + i] <=> this[hi - i];
      }
    }

    /* Return a tuple containing ``true`` and the index of the first
       instance of ``val`` in the array, or if ``val`` is not found, a
       tuple containing ``false`` and an unspecified value is returned.
     */
    proc find(val: this.eltType): (bool, index(this.domain)) {
      for i in this.domain {
        if this[i] == val then return (true, i);
      }
      var arbInd: index(this.domain);
      return (false, arbInd);
    }

    /* Return the number of times ``val`` occurs in the array. */
    proc count(val: this.eltType): int {
      return + reduce (this == val);
    }

   /* Return a tuple of integers describing the size of each dimension.
      For a sparse array, returns the shape of the parent domain.*/
    proc shape {
      return this.domain.shape;
    }

    pragma "no doc"
    proc _scan(op) where Reflection.canResolveMethod(_value, "doiScan", op, this.domain) {
      return _value.doiScan(op, this.domain);
    }

  }  // record _array

  // _instance is a subclass of BaseArr.  LYDIA NOTE: moved this from
  // being a method on _array so that it could be called on unwrapped
  // _instance fields
  inline proc _do_destroy_arr(_unowned: bool, _instance,
                              param deinitElts=true) {
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
  inline proc _do_destroy_array(array: _array, param deinitElts=true) {
    _do_destroy_arr(array._unowned, array._instance, deinitElts);
  }

  proc _deinitElementsIsParallel(type eltType) param {
    // TODO: Would anything be hurt if this always returned true?
    // one guess: arrays of arrays where all inner arrays share a domain?
    return false;
  }

  proc _deinitElements(array: _array) {
    param needsDestroy = __primitive("needs auto destroy", array.eltType);
    if needsDestroy {
      if _deinitElementsIsParallel(array.eltType) {
        forall elt in array {
          chpl__autoDestroy(elt);
        }
      } else {
        for elt in array {
          chpl__autoDestroy(elt);
        }
      }
    }
  }

  //
  // A helper function to check array equality (== on arrays promotes
  // to an array of booleans)
  //
  /* Return true if all this array is the same size and shape
     as argument ``that`` and all elements of this array are
     equal to the corresponding element in ``that``. Otherwise
     return false. */
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

    if this.size != that.size then
      return false;

    //
    // check that size/shape are the same to permit legal zippering
    //
    if isRectangularDom(this.domain) && isRectangularDom(that.domain) {
      for d in 0..#this.rank do
        if this.domain.dim(d).size != that.domain.dim(d).size then
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

  // The same as the built-in _cast, except accepts a param arg.
  pragma "no doc"
  proc _cast(type t:_array, param arg) {
    var result: t;
    // The would-be param version of proc =, inlined.
    chpl__transferArray(result, arg);
    return result;
  }

  // How to cast arrays to strings
  pragma "no doc"
  proc _cast(type t:string, x: []) {
    use IO;
    return stringify(x);
  }

  //
  // isXxxType, isXxxValue
  //

  /* Return true if ``t`` is a domain map type. Otherwise return false. */
  proc isDmapType(type t) param {
    proc isDmapHelp(type t: _distribution) param  return true;
    proc isDmapHelp(type t)                param  return false;
    return isDmapHelp(t);
  }

  pragma "no doc"
  proc isDmapValue(e: _distribution) param  return true;
  /* Return true if ``e`` is a domain map. Otherwise return false. */
  proc isDmapValue(e)                param  return false;

  /* Return true if ``t`` is a domain type. Otherwise return false. */
  proc isDomainType(type t) param {
    return isSubtype(t, _domain);
  }

  pragma "no doc"
  proc isDomainValue(e: domain) param  return true;
  /* Return true if ``e`` is a domain. Otherwise return false. */
  proc isDomainValue(e)         param  return false;

  /* Return true if ``t`` is an array type. Otherwise return false. */
  proc isArrayType(type t) param {
    return isSubtype(t, _array);
  }

  pragma "no doc"
  proc isArrayValue(e: []) param  return true;
  /* Return true if ``e`` is an array. Otherwise return false. */
  proc isArrayValue(e)     param  return false;

  proc -(a :domain, b :domain) where (a.type == b.type) && isAssociativeDom(a) {
    var newDom : a.type;
    serial !newDom._value.parSafe do
      forall e in a do
        if !b.contains(e) then newDom.add(e);
    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  proc -=(ref a :domain, b :domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in b do
      if a.contains(e) then
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
        if b.contains(k) then newDom += k;
    return newDom;
  }

  proc &=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    var removeSet: domain(a.idxType);
    for e in a do
      if !b.contains(e) then
        removeSet += e;
    for e in removeSet do
      a.remove(e);
  }

  proc ^(a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    var newDom : a.type;

    serial !newDom._value.parSafe {
      forall k in a do
        if !b.contains(k) then newDom.add(k);
      forall k in b do
        if !a.contains(k) then newDom.add(k);
    }

    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, it is
     added to the LHS.
  */
  proc ^=(ref a :domain, b: domain) where (a.type == b.type) && isAssociativeDom(a) {
    for e in b do
      if a.contains(e) then
        a.remove(e);
      else
        a.add(e);
  }

  //
  // BaseSparseDom operator overloads
  //
  proc +=(ref sd: domain, inds: [] index(sd)) where isSparseDom(sd) {
    if inds.size == 0 then return;

    sd._value.dsiBulkAdd(inds);
  }


  // TODO: Currently not optimized
  proc +=(ref sd: domain, d: domain)
  where isSparseDom(sd) && d.rank==sd.rank && sd.idxType==d.idxType {
    if d.size == 0 then return;

    const indCount = d.size;
    var arr: [{0..#indCount}] index(sd);

    for (i,j) in zip(d, 0..) do arr[j] = i;

    var rowSorted = false;

    // Once an interface supports it:
    // if sd.RMO && d.RMO then rowSorted = true;

    sd._value.dsiBulkAdd(arr, rowSorted, true, false);
  }

  // TODO: Implement bulkRemove
  proc -=(ref sd: domain, inds: [] index(sd)) where isSparseDom(sd) {
    for ind in inds do
      sd -= ind;
  }

  proc -=(ref sd: domain, d: domain)
  where isSparseDom(sd) && d.rank==sd.rank && sd.idxType==d.idxType {
    for ind in d do
      sd -= ind;
  }
  //
  // Helper functions
  //

  pragma "no doc"
  proc isCollapsedDimension(r: range(?e,?b,?s,?a)) param return false;
  pragma "no doc"
  proc isCollapsedDimension(r) param return true;


  // computes || reduction over stridable of ranges
  proc chpl__anyStridable(ranges) param {
    for param i in 0..ranges.size-1 do
      if ranges(i).stridable then
        return true;
    return false;
  }

  // computes || reduction over stridable of ranges, but permits some
  // elements not to be ranges (as in a rank-change slice)
  proc chpl__anyRankChangeStridable(args) param {
    for param i in 0..args.size-1 do
      if isRangeValue(args(i)) then
        if args(i).stridable then
          return true;
    return false;
  }

  // the following pair of routines counts the number of ranges in its
  // argument list and is used for rank-change slices
  proc chpl__countRanges(arg) param {
    return isRangeValue(arg):int;
  }

  proc chpl__countRanges(arg, args...) param {
    return chpl__countRanges(arg) + chpl__countRanges((...args));
  }

  // given a tuple args, returns true if the tuple contains only
  // integers and ranges; that is, it is a valid argument list for rank
  // change
  proc _validRankChangeArgs(args, type idxType) param {
    proc _validRankChangeArg(type idxType, r: range(?)) param return true;
    proc _validRankChangeArg(type idxType, i: idxType) param return true;
    proc _validRankChangeArg(type idxType, x) param return false;

    /*
    proc help(param dim: int) param {
      if !_validRankChangeArg(idxType, args(dim)) then
        return false;
      else if dim < args.size then
        return help(dim+1);
      else
        return true;
    }*/

    proc allValid() param {
      for param dim in 0.. args.size-1 {
        if !_validRankChangeArg(idxType, args(dim)) then
          return false;
      }
      return true;
    }
    proc oneRange() param {
      for param dim in 0.. args.size-1 {
        if isRange(args(dim)) then
          return true;
      }
      return false;
    }
    proc oneNonRange() param {
      for param dim in 0.. args.size-1 {
        if !isRange(args(dim)) then
          return true;
      }
      return false;
    }

    return allValid() && oneRange() && oneNonRange();
    //return help(0);
  }

  //
  // Assignment of domains and arrays
  //
  proc =(ref a: _distribution, b: _distribution) {
    if a._value == nil {
      __primitive("move", a, chpl__autoCopy(b.clone()));
    } else if a._value._doms.size == 0 {
      if a._value.type != b._value.type then
        compilerError("type mismatch in distribution assignment");
      if a._value == b._value {
        // do nothing
      } else
        a._value.dsiAssign(b._value);
      if _isPrivatized(a._instance) then
        _reprivatize(a._value);
    } else {
      halt("assignment to distributions with declared domains is not yet supported");
    }
  }

  proc =(ref a: domain, b: domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in domain assignment");

    if a.idxType != b.idxType then
      compilerError("index type mismatch in domain assignment");

    if isRectangularDom(a) && isRectangularDom(b) then
      if !a.stridable && b.stridable then
        compilerError("cannot assign from a stridable domain to an unstridable domain without an explicit cast");

    a._instance.dsiAssignDomain(b, lhsPrivate=false);

    if _isPrivatized(a._instance) {
      _reprivatize(a._instance);
    }
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
                 isRange(first) && isRange(rest(0));
      }
      proc peelArgs(first) param return isRange(first);

      return if !isTuple(a) then false else peelArgs((...a));
    }

    proc strideSafe(d, rt, param dim: int=0) param {
      return if dim == d.rank-1 then
               d.dim(dim).stridable || !rt(dim).stridable
             else
               (d.dim(dim).stridable || !rt(dim).stridable) && strideSafe(d, rt, dim+1);
    }
    return isRangeTuple(t) && d.rank == t.size && strideSafe(d, t);
  }

  proc =(ref a: domain, b: _tuple) {
    if chpl__isLegalRectTupDomAssign(a, b) {
      a = {(...b)};
    } else {
      a.clear();
      for ind in 0..#b.size {
        a.add(b(ind));
      }
    }
  }

  proc =(ref d: domain, r: range(?)) {
    d = {r};
  }

  proc =(ref a: domain, b) {  // b is iteratable
    if isRectangularDom(a) then
      compilerError("Illegal assignment to a rectangular domain");
    a.clear();
    for ind in b {
      a.add(ind);
    }
  }

  proc chpl__serializeAssignment(a: [], b) param {
    if a.rank != 1 && isRange(b) then
      return true;

    // Sparse arrays do not yet support parallel iteration.  We
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
  proc chpl__compatibleForBulkTransfer(a:[], b:[], param kind:_tElt) param {
    if !useBulkTransfer then return false;
    if a.eltType != b.eltType then return false;
    if kind==_tElt.move then return true;
    if kind==_tElt.initCopy && isConstCopyableType(a.eltType) then return true;
    if !chpl__supportedDataTypeForBulkTransfer(a.eltType) then return false;
    return true;
  }

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

  proc chpl__supportedDataTypeForBulkTransfer(x: string) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: bytes) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: sync) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: single) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: domain) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: []) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x: _distribution) param return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: locale) param return true;
  proc chpl__supportedDataTypeForBulkTransfer(x: chpl_anycomplex) param return true;
  // TODO -- why is the below line here?
  proc chpl__supportedDataTypeForBulkTransfer(x: borrowed object) param return false;
  proc chpl__supportedDataTypeForBulkTransfer(x) param return true;

  pragma "no doc"
  proc checkArrayShapesUponAssignment(a: [], b: []) {
    if isRectangularArr(a) && isRectangularArr(b) {
      const aDims = a._value.dom.dsiDims(),
            bDims = b._value.dom.dsiDims();
      compilerAssert(aDims.size == bDims.size);
      for param i in 0..aDims.size-1 {
        if aDims(i).size != bDims(i).size then
          halt("assigning between arrays of different shapes in dimension ",
               i, ": ", aDims(i).size, " vs. ", bDims(i).size);
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
      // default initializer is a forall expr. E.g. arrayInClassRecord.chpl.
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

    chpl__uncheckedArrayTransfer(a, b, kind=_tElt.assign);
  }

  // what kind of transfer to do for each element?
  pragma "no doc"
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
      forall ai in a.domain {
        ref aa = a[ai];
        pragma "no auto destroy"
        var copy: a.eltType = aa; // run copy initializer
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
        const ref rhsDist = elt.dist;
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
      forall ai in a.domain {
        ref aa = a[ai];
        fixRuntimeType(a.eltType, aa);
      }
    } else {
      forall aa in a {
        fixRuntimeType(a.eltType, aa);
      }
    }
  }

  inline proc chpl__uncheckedArrayTransfer(ref a: [], b:[], param kind) {

    var done = false;
    if !chpl__serializeAssignment(a, b) {
      if chpl__compatibleForBulkTransfer(a, b, kind) {
        done = chpl__bulkTransferArray(a, b);
      }
      else if chpl__compatibleForWidePtrBulkTransfer(a, b, kind) {
        done = chpl__bulkTransferPtrArray(a, b);
      }
      // If we did a bulk transfer, it just bit copied, so need to
      // run copy initializer still
      if done {
        if kind==_tElt.initCopy && !isPODType(a.eltType) {
          initCopyAfterTransfer(a);
        } else if kind==_tElt.move && (isSubtype(a.eltType, _array) ||
                                       isSubtype(a.eltType, _domain)) {
          fixEltRuntimeTypesAfterTransfer(a);
        }
      }
    }
    if !done {
      chpl__transferArray(a, b, kind);
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
  inline proc chpl__bulkTransferArray(ref a: [], AD : domain, const ref b: [], BD : domain) {
    return chpl__bulkTransferArray(a._value, AD, b._value, BD);
  }

  inline proc chpl__bulkTransferArray(destClass, destDom : domain, srcClass, srcDom : domain) {
    var success = false;

    inline proc bulkTransferDebug(msg:string) {
      if debugBulkTransfer then chpl_debug_writeln("proc =(a:[],b:[]): ", msg);
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
                                   destDom, srcClass, srcDom) {
      bulkTransferDebug("attempting doiBulkTransferFromKnown");
      success = destClass.doiBulkTransferFromKnown(destDom, srcClass, srcDom);
    } else if Reflection.canResolveMethod(srcClass, "doiBulkTransferToKnown",
                                          srcDom, destClass, destDom) {
      bulkTransferDebug("attempting doiBulkTransferToKnown");
      success = srcClass.doiBulkTransferToKnown(srcDom, destClass, destDom);
    } else if Reflection.canResolveMethod(destClass, "doiBulkTransferFromAny",
                                          destDom, srcClass, srcDom) {
      bulkTransferDebug("attempting doiBulkTransferFromAny");
      success = destClass.doiBulkTransferFromAny(destDom, srcClass, srcDom);
    } else if Reflection.canResolveMethod(srcClass, "doiBulkTransferToAny",
                                          srcDom, destClass, destDom) {
      bulkTransferDebug("attempting doiBulkTransferToAny");
      success = srcClass.doiBulkTransferToAny(srcDom, destClass, destDom);
    }

    if success then
      bulkTransferDebug("successfully completed bulk transfer");
    else
      bulkTransferDebug("bulk transfer did not happen");

    return success;
  }

  pragma "find user line"
  pragma "ignore transfer errors"
  inline proc chpl__transferArray(ref a: [], const ref b,
                           param kind=_tElt.assign) lifetime a <= b {
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
    } else if chpl__serializeAssignment(a, b) {
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
          [ (ai, bb) in zip(a.domain, b) ] {
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
          [ (ai, bb) in zip(a.domain, b) ] {
            ref aa = a[ai];
            pragma "no auto destroy"
            var copy: a.eltType = bb; // init copy
            // move it into the array
            __primitive("=", aa, copy);
          }

        } else {
          [ (aa,bb) in zip(a,b) ] {
            pragma "no auto destroy"
            var copy: a.eltType = bb; // init copy
            // move it into the array
            __primitive("=", aa, copy);
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

  inline proc =(ref a: [], b:domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in array assignment");
    if isAssociativeDom(b) && isRectangularArr(a) then
      compilerError("cannot assign to rectangular arrays from associative domains");
    chpl__transferArray(a, b);
  }

  inline proc =(a: [], b: range(?)) {
    if a.rank == 1 then
      chpl__transferArray(a, b);
    else
      compilerError("cannot assign from ranges to multidimensional arrays");
  }

  inline proc =(ref a: [], b) /* b is not an array nor a domain nor a tuple */ {
    chpl__transferArray(a, b);
  }

/* Does not work: compiler expects assignments to have 2 formals,
   whereas the below becomes a 1-argument function after resolution.
  inline proc =(ref a: [], param b) {
    chpl__transferArray(a, b);
  }
*/

  private proc helpInitArrFromTuple(ref j, param rank: int,
                                    ref a: [], b: _tuple,
                                    param kind:_tElt)
  lifetime a < b {

      type idxType = a.domain.idxType,
           strType = chpl__signedType(a.domain.intIdxType);

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
              var newArr = chpl__coerceMove(a.eltType, src);
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

  proc =(ref a: [], b: _tuple) where isRectangularArr(a) {
    initArrFromTuple(a, b, _tElt.assign);
  }

  proc _desync(type t:_syncvar) type {
    var x: t;
    return x.valType;
  }

  proc _desync(type t:_singlevar) type {
    var x: t;
    return x.valType;
  }


  proc _desync(type t) type {
    return t;
  }

  private proc desyncEltType(type t:_array) type {
    type eltType = chpl__eltTypeFromArrayRuntimeType(t);
    return _desync(eltType);
  }

  proc =(ref a: [], b: _desync(a.eltType)) {
    forall e in a do
      e = b;
  }

  //
  // op= overloads for array/scalar pairs
  //
  proc +=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e += b;
  }

  proc -=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e -= b;
  }

  proc *=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e *= b;
  }

  proc /=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e /= b;
  }

  proc %=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e %= b;
  }

  proc **=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e **= b;
  }

  proc &=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e &= b;
  }

  proc |=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e |= b;
  }

  proc ^=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e ^= b;
  }

  proc >>=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e >>= b;
  }

  proc <<=(a: [], b: _desync(a.eltType)) {
    forall e in a do
      e <<= b;
  }


  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_by(a:domain, b) { ... }
   *
   * because the parser renames the routine since 'by' is a keyword.
   */
  proc by(a: domain, b) {
    var r: a.rank*range(a._value.idxType,
                      BoundedRangeType.bounded,
                      true);
    var t = _makeIndexTuple(a.rank, b, expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) by t(i);
    return new _domain(a.dist, a.rank, a._value.idxType, true, r);
  }

  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_align(a:domain, b) { ... }
   *
   * because the parser renames the routine since 'align' is a keyword.
   */
  proc align(a: domain, b) {
    var r: a.rank*range(a._value.idxType,
                      BoundedRangeType.bounded,
                      a.stridable);
    var t = _makeIndexTuple(a.rank, b, expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) align t(i);
    return new _domain(a.dist, a.rank, a._value.idxType, a.stridable, r);
  }

  //
  // Swap operator for arrays
  //
  inline proc <=>(x: [], y: []) {
    forall (a,b) in zip(x, y) do
      a <=> b;
  }

  /* Return a copy of the array ``A`` containing the same values but
     in the shape of the domain ``D``. The number of indices in the
     domain must equal the number of elements in the array. The
     elements of ``A`` are copied into the new array using the
     default iteration orders over ``D`` and ``A``.  */
  proc reshape(A: [], D: domain) {
    if !isRectangularDom(D) then
      compilerError("reshape(A,D) is meaningful only when D is a rectangular domain; got D: ", D.type:string);
    if A.size != D.size then
      halt("reshape(A,D) is invoked when A has ", A.size,
           " elements, but D has ", D.size, " indices");
    var B: [D] A.eltType = for (i,a) in zip(D, A) do a;
    return B;
  }

  pragma "no doc"
  proc reshape(A: _iteratorRecord, D: domain) {
    if !isRectangularDom(D) then
      compilerError("reshape(A,D) is meaningful only when D is a rectangular domain; got D: ", D.type:string);
    var B = for (i,a) in zip(D, A) do a;
    return B;
  }

  pragma "no doc"
  iter linearize(Xs) {
    for x in Xs do yield x;
  }

  // This implementation of arrays and domains can create aliases
  // of domains and arrays. Additionally, array aliases are possible
  // in the language with the => operator.
  //
  // A call to the chpl__unalias function is added by the compiler when a user
  // variable is initialized from an expression that would normally not require
  // a copy.
  //
  // For example, if we have
  //   var A:[1..10] int;
  //   var B = A[1..3];
  // then B is initialized with a slice of A. But since B is a new
  // variable, it needs to be a new 3-element array with distinct storage.
  // Since the slice is implemented as a function call, without chpl__unalias,
  // B would just be initialized to the result of the function call -
  // meaning that B would not refer to distinct array elements.
  pragma "unalias fn"
  inline proc chpl__unalias(x: domain) {
    if _to_unmanaged(x._instance.type) != x._instance.type then
      compilerError("Domain on borrow created");

    if x._unowned {
      // We could add an autoDestroy here, but it wouldn't do anything for
      // an unowned domain.
      pragma "no auto destroy" var ret = x;
      return ret;
    } else {
      pragma "no copy" var ret = x;
      return ret;
    }
  }

  pragma "init copy fn"
  proc chpl__initCopy(const ref rhs: []) {
    pragma "no copy"
    var lhs = chpl__coerceCopy(rhs.type, rhs);
    return lhs;
  }

  pragma "auto copy fn" proc chpl__autoCopy(x: []) {
    pragma "no copy" var b = chpl__initCopy(x);
    return b;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:_domain) {
    param rhsIsLayout = rhs.dist._value.dsiIsLayout();

    var lhs:dstType;
    lhs; // no split init
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if lhs.dist._value.dsiIsLayout() && !rhsIsLayout then
      compilerWarning("initializing a non-distributed domain from a distributed domain. If you didn't mean to do that, add a dmapped clause to the type expression or remove the type expression altogether");

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:_domain) {
    param rhsIsLayout = rhs.dist._value.dsiIsLayout();

    // TODO: just return rhs
    // if the domain types are the same and their runtime types
    // are the same.

    var lhs:dstType;
    lhs; // no split init
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if lhs.dist._value.dsiIsLayout() && !rhsIsLayout then
      compilerWarning("initializing a non-distributed domain from a distributed domain. If you didn't mean to do that, add a dmapped clause to the type expression or remove the type expression altogether");

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:_tuple) {
    var lhs:dstType;
    lhs; // no split init
    if chpl__isLegalRectTupDomAssign(lhs, rhs) {
      lhs = {(...rhs)};
    } else {
      lhs = rhs;
    }

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:_tuple) {
    var lhs:dstType;
    lhs; // no split init
    if chpl__isLegalRectTupDomAssign(lhs, rhs) {
      lhs = {(...rhs)};
    } else {
      lhs = rhs;
    }

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:range(?)) {
    var lhs:dstType;
    lhs; // no split init
    lhs = {rhs};
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:range(?)) {
    var lhs:dstType;
    lhs; // no split init
    lhs = {rhs};
    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs: _iteratorRecord) {
    // assumes rhs is iterable
    var lhs:dstType;
    if isRectangularDom(lhs) then
      compilerError("Illegal assignment to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, rhs: _iteratorRecord) {
    // assumes rhs is iterable
    var lhs:dstType;
    if isRectangularDom(lhs) then
      compilerError("Illegal assignment to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs) {
    // assumes rhs is iterable (e.g. list)
    var lhs:dstType;
    if isRectangularDom(lhs) then
      compilerError("Illegal assignment to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs) {
    // assumes rhs is iterable (e.g. list)
    var lhs:dstType;
    if isRectangularDom(lhs) then
      compilerError("Illegal assignment to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }


  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:_array) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if !isCopyableType(eltType) then
      compilerError("Cannot copy-initialize array because element type '",
                    eltType:string, "' cannot be copy-initialized");

    if rhs._value == nil {
      // This happens e.g. for 'new' on a record with an array field whose
      // default initializer is a forall expr. E.g. arrayInClassRecord.chpl.
    } else if lhs._value == rhs._value {
      // do nothing (assert?)
    } else if lhs.size == 0 && rhs.size == 0 {
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
                        pragma "no auto destroy" in rhs:_array) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    // type mismatch important because RHS could be a slice, e.g.
    param typeMismatch = rhs._instance.type !=
                         chpl__instanceTypeFromArrayRuntimeType(dstType);

    param moveElts = !typeMismatch;

    // If the domains point to the same thing (and aren't just identical),
    // then we can simply return the RHS array.
    // TODO: if the domain types match, could steal data pointers
    if moveElts && dom._instance == rhs.domain._instance {
      return rhs;
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
    } else if lhs.size == 0 && rhs.size == 0 {
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
  proc chpl__coerceCopy(type dstType:_array, rhs:_domain) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if isAssociativeDom(rhs) && isRectangularArr(lhs) then
      compilerError("cannot assign to rectangular arrays from associative domains");
    if !isPODType(eltType) then
      compilerError("cannot assign to array from domain of non-POD indices");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array, in rhs:_domain) {
    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    var lhs = dom.buildArray(eltType, initElts=false);

    if lhs.rank != rhs.rank then
      compilerError("rank mismatch in array assignment");
    if isAssociativeDom(rhs) && isRectangularArr(lhs) then
      compilerError("cannot assign to rectangular arrays from associative domains");
    if !isPODType(eltType) then
      compilerError("cannot assign to array from domain of non-POD indices");

    chpl__transferArray(lhs, rhs, kind=_tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:range(?)) {
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
  proc chpl__coerceMove(type dstType:_array, in rhs:range(?)) {
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
  proc chpl__coerceCopy(type dstType:_array, rhs:_tuple) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if !isRectangularArr(lhs) then
      compilerError("Cannot assign from tuple to non-rectangular array");

    initArrFromTuple(lhs, rhs, _tElt.initCopy);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_array,
                        pragma "no auto destroy" in rhs:_tuple) {

    type eltType = chpl__eltTypeFromArrayRuntimeType(dstType);
    const ref dom = chpl__domainFromArrayRuntimeType(dstType);

    pragma "no copy" // avoid error about recursion for initCopy
    pragma "unsafe" // when eltType is non-nilable
    var lhs = dom.buildArray(eltType, initElts=false);

    if !isRectangularArr(lhs) then
      compilerError("Cannot assign from tuple to non-rectangular array");

    initArrFromTuple(lhs, rhs, _tElt.move);

    lhs._value.dsiElementInitializationComplete();

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_array, rhs:desyncEltType(dstType)) {
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
  proc chpl__coerceMove(type dstType:_array, in rhs:desyncEltType(dstType)) {
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
  proc chpl__coerceCopy(type dstType:_array, rhs: _iteratorRecord) {
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
  proc chpl__coerceMove(type dstType:_array, rhs: _iteratorRecord) {
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
  proc chpl__coerceCopy(type dstType:_array, rhs) {
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
  proc chpl__coerceMove(type dstType:_array, in rhs) {
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
    var toArray = chpl__initCopy(ir); // call iterator -> array copy fn
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


  // see comment on chpl__unalias for domains
  pragma "unalias fn"
  inline proc chpl__unalias(x: []) {
    param isview = (x._value.isSliceArrayView() ||
                    x._value.isRankChangeArrayView() ||
                    x._value.isReindexArrayView());

    if isview {
      // Intended to call chpl__initCopy
      pragma "no auto destroy" var ret = x;
      // Since chpl__unalias replaces a initCopy(auto/initCopy()) the
      // inner value needs to be auto-destroyed.
      // TODO: Should this be inserted by the compiler?
      chpl__autoDestroy(x);
      return ret;
    } else {
      // Just return a bit-copy/shallow-copy of 'x'
      pragma "no copy" var ret = x;
      return ret;
    }
  }

  // chpl__initCopy(ir: _iteratorRecord) is used to create an array
  // out of for-expressions, forall-expressions, promoted expressions.
  // The 'ir' iterator - or its standalone/leader/follower counterpart -
  // is invoked to generate the desired array elements.

  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord)
    where chpl_iteratorHasDomainShape(ir)
  {

    // ENGIN: here ir._shape_ could be a privatized domain. Make sure that the
    // initializer we call here do not create another set of privatized
    // instances
    var shape = new _domain(ir._shape_);

    // Important: ir._shape_ points to a domain class for a domain
    // that is owned by the forall-expression or the leader in the
    // promoted expression.
    shape._unowned = true;

    return chpl__initCopy_shapeHelp(shape, ir);
  }

  pragma "init copy fn"
  proc chpl__initCopy(ir: _iteratorRecord)
    where chpl_iteratorHasRangeShape(ir) && !chpl_iteratorFromForExpr(ir)
  {
    // Need this pragma in the range case to avoid leaking 'shape'.
    // If we use it in the domain case, we get one autoDestroy too many.
    pragma "insert auto destroy"
    var shape = {ir._shape_};

    return chpl__initCopy_shapeHelp(shape, ir);
  }

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
        for (ri, src) in zip(result.domain, ir) {
          ref r = result[ri];
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      } else {
        for (r, src) in zip(result, ir) {
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      }
    } else {
      if needsInitWorkaround(result.eltType) {
        forall (ri, src) in zip(result.domain, ir) {
          ref r = result[ri];
          pragma "no auto destroy"
          var copy = src; // init copy, might be elided
          __primitive("=", r, copy);
        }
      } else {
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
  proc chpl__initCopy(ir: _iteratorRecord) {
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
    var size = r.size : size_t;
    type elemType = iteratorToArrayElementType(ir.type);
    var data:_ddata(elemType) = nil;

    var callPostAlloc: bool;
    var subloc = c_sublocid_none;

    if size > 0 {
      data = _ddata_allocate_noinit(elemType, size, callPostAlloc);
    }

    try {
      for elt in ir {

        // Future: we should generally remove this copy.
        // Note though that in some cases it invokes this function
        // recursively - in that case it shouldn't be removed!
        pragma "no auto destroy"
        pragma "no copy"
        var eltCopy = try chpl__initCopy(elt);

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
    if shapeful && i < r.size then
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

      var A = D.buildArrayWith(elemType, data, size:int);

      return A;
    }
  }
}
