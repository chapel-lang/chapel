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

// ChapelDomain.chpl
//
module ChapelDomain {

  public use ChapelBase;
  use ArrayViewRankChange, ChapelTuple;

  private use ChapelRange only chpl_isValidRangeIdxType;
  /*
     Fractional value that specifies how full this domain can be
     before requesting additional memory. The default value of
     0.5 means that the map will not resize until the map is more
     than 50% full. The acceptable values for this argument are
     between 0 and 1, exclusive, meaning (0,1). A lower
     `defaultHashTableResizeThreshold` value can potentially
     improve indexing performance, since the table will likely
     have fewer collisions, while a higher value can help save
     memory.
     Note that this value also impacts all of Chapel's hash-based
     data structures, such as `set` and `map`.
  */
  config const defaultHashTableResizeThreshold = 0.5;

  /* Compile with ``-snoNegativeStrideWarnings``
     to suppress the warning about arrays and slices with negative strides. */
  @chpldoc.nodoc
  config param noNegativeStrideWarnings = false;

  @chpldoc.nodoc
  config param noSortedWarnings = false;

  pragma "no copy return"
  pragma "return not owned"
  proc _getDomain(value) {
    if _to_unmanaged(value.type) != value.type then
      compilerError("Domain on borrow created");

    if _isPrivatized(value) then
      return new _domain(value.pid, value, _unowned=true);
    else
      return new _domain(nullPid, value, _unowned=true);
  }

  @chpldoc.nodoc
  proc tupleOfRangesSlice(base, slice) where chpl__isTupleOfRanges(base) &&
                                             chpl__isTupleOfRanges(slice) {

    if base.size != slice.size then
      compilerError("tuple size mismatch in tupleOfRangesSlice");

    param rank = base.size;

    proc resultStrides(param dim = 0) param do return
      if dim == rank-1 then ( base(dim)[slice(dim)] ).strides
      else chpl_strideUnion( ( base(dim)[slice(dim)] ).strides,
                                    resultStrides(dim+1) );

    var r: rank*range(base[0].idxType, boundKind.both, resultStrides());
    for param i in 0..rank-1 {
      r(i) = base(i)[slice(i)];
    }
    return r;
  }

  // Run-time type support
  //
  // NOTE: the bodies of functions marked with runtime type init fn (such as
  // chpl__buildDomainRuntimeType) are replaced by the compiler to just create
  // a record storing the arguments.  The return type of
  // chpl__build...RuntimeType is what tells the compiler which runtime type it
  // is creating. These functions are written to return a value even though
  // they are marked as type functions.

  //
  // Support for domain types
  //
  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(dist, param rank: int,
                                    type idxType = int,
                                    param strides: strideKind = strideKind.one
                                    ) type {
    return new _domain(dist, rank, idxType, strides);
  }

  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(dist, type idxType,
                                    param parSafe: bool = false) type {
    if isDomainType(idxType) then
      compilerError("Values of 'domain' type do not support hash functions yet, so cannot be used as an associative domain's index type");
    return new _domain(dist, idxType, parSafe);
  }

  private proc isUltimatelyRectangularParent(parentDom: domain) param {
    if parentDom.isRectangular() then
      return true;
    else if parentDom.isSparse() then
      return isUltimatelyRectangularParent(parentDom._value.parentDom);
    else
      return false;
  }

  pragma "runtime type init fn"
  proc chpl__buildSparseDomainRuntimeType(dist,
                                          parentDom: domain) type {
    if ! isUltimatelyRectangularParent(parentDom) then
      compilerError("sparse subdomains are currently supported only for " +
                    "rectangular domains");

    return new _domain(dist, parentDom);
  }

  proc chpl__buildSparseDomainRuntimeTypeForParentDomain(parentDom) type {
    if ! isDomain(parentDom) then
      compilerError("building a sparse subdomain of a non-domain value",
                    " of type ", parentDom.type: string);

    return chpl__buildSparseDomainRuntimeType(parentDom.defaultSparseDist,
                                              parentDom);
  }

  proc chpl__buildSparseDomainRuntimeTypeForParentDomain(type parentDom) {
    compilerError("building a sparse subdomain of a type;",
                  " please supply a domain value instead");
  }

  proc chpl__convertRuntimeTypeToValue(dist,
                                       param rank: int,
                                       type idxType = int,
                                       param strides: strideKind,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, rank, idxType, strides, definedConst);
  }

  proc chpl__convertRuntimeTypeToValue(dist, type idxType,
                                       param parSafe: bool,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, idxType, parSafe);
  }

  proc chpl__convertRuntimeTypeToValue(dist,
                                       parentDom: domain,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, parentDom);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseRectangularDom) {
    return chpl__buildDomainRuntimeType(dom.distribution, dom._value.rank,
                                      dom._value.idxType, dom._value.strides);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseAssociativeDom) {
    return chpl__buildDomainRuntimeType(dom.distribution, dom._value.idxType, dom._value.parSafe);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type
   where isSubtype(dom._value.type, BaseSparseDom) {
    return chpl__buildSparseDomainRuntimeType(dom.distribution, dom._value.parentDom);
  }

  proc chpl__convertValueToRuntimeType(dom: domain) type {
    compilerError("the global domain class of each domain map implementation must be a subclass of BaseRectangularDom, BaseAssociativeDom, or BaseSparseDom", 0);
    return 0; // dummy
  }

  //
  // Support for subdomain types
  //
  // Note the domain of a subdomain is not yet part of the runtime type
  //
  proc chpl__buildSubDomainType(dom: domain) type do
    return chpl__convertValueToRuntimeType(dom);

  //
  // Support for domain expressions, e.g., {1..3, 1..3}
  //

  proc chpl__isTupleOfRanges(tup) param {
    if !isTuple(tup) then return false;
    for param i in 0..tup.size-1 {
      if !isRangeType(tup(i).type) then
        return false;
    }
    return true;
  }

  proc chpl__buildDomainExpr(ranges..., definedConst)
  where chpl__isTupleOfRanges(ranges) {
    param rank = ranges.size;
    for param i in 1..rank-1 do
      if ranges(0).idxType != ranges(i).idxType then
        compilerError("idxType varies among domain's dimensions");
    for param i in 0..rank-1 do
      if ranges(i).bounds != boundKind.both then
        compilerError("one of domain's dimensions is not a bounded range");
    var d: domain(rank, ranges(0).idxType, chpl_strideUnion(ranges));
    d.setIndices(ranges);
    if definedConst then
      chpl__setDomainConst(d);
    return d;
  }

  private proc chpl__setDomainConst(dom: domain) {
    dom._value.definedConst = true;
  }

  // definedConst is added only for interface consistency
  proc chpl__buildDomainExpr(const keys..., definedConst) {
    param count = keys.size;
    // keyType of string literals is assumed to be type string
    type keyType = keys(0).type;
    for param i in 1..count-1 do
      if keyType != keys(i).type {
        compilerError("Associative domain element " + i:string +
                      " expected to be of type " + keyType:string +
                      " but is of type " + keys(i).type:string);
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

  private proc chpl_checkForAnonAssocDom(dims) param : string {
    for param i in 0..<dims.size do
      if (!isRange(dims(i))) then
        return dims(i).type:string;
    return "";
  }

  // pragma is a workaround for ref-pair vs generic/specific overload resolution
  pragma "compiler generated"
  pragma "last resort"
  proc chpl__ensureDomainExpr(x...) {
    // we are creating array with a range literal(s). So, the array's domain
    // cannot be changed anymore.
    param dimType = chpl_checkForAnonAssocDom(x);
    if dimType != "" then
      compilerError("Arrays with anonymous domains must either be defined using a list of ranges or use curly brackets.  If you were trying to create an array over a set of '" + dimType + "' indices, please use curly brackets.");

    return chpl__buildDomainExpr((...x), definedConst=true);
  }

  pragma "compiler generated"
  pragma "last resort"
  proc chpl__ensureDomainExpr(type t) {
    compilerError("Domain expression was a type ('", t:string, "') rather than a domain value or range list as expected");
  }

  proc chpl__isRectangularDomType(type domainType) param {
    var dom: domainType;
    return isDomainType(domainType) && dom.isRectangular();
  }

  proc chpl__isSparseDomType(type domainType) param {
    var dom: domainType;
    return dom.isSparse();
  }

  pragma "no copy return"
  pragma "return not owned"
  proc chpl__parentDomainFromDomainRuntimeType(type domainType) {
    pragma "no copy"
    pragma "no auto destroy"
    var parentDom = __primitive("get runtime type field",
                                domainType, "parentDom");

    return _getDomain(parentDom._value);
  }

  pragma "no copy return"
  pragma "return not owned"
  proc chpl__domainFromArrayRuntimeType(type rtt) {
    pragma "no copy"
    pragma "no auto destroy"
    var dom = __primitive("get runtime type field", rtt, "dom");

    return _getDomain(dom._value);
  }

  proc chpl_isAssociativeDomClass(dc: BaseAssociativeDom) param do return true;
  pragma "last resort"
  proc chpl_isAssociativeDomClass(dc) param do return false;

  private proc errorIfNotRectangular(dom: domain, param op, param arrays="") {
    if dom.isAssociative() || dom.isSparse() then
      compilerError("cannot apply '", op,
                    "' to associative and sparse domains", arrays);
    if ! dom.isRectangular() then
      compilerError("cannot apply '", op, "' to '", dom.type:string, "'");
  }

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

  proc chpl_countDomHelp(dom, counts) {
    var ranges = dom.dims();
    for param i in 0..dom.rank-1 do
      ranges(i) = ranges(i) # counts(i);

    return new _domain(dom.distribution, dom.rank, dom.idxType, dom.strides, ranges);
  }

  @chpldoc.nodoc
  operator #(dom: domain, counts: integral) {
    errorIfNotRectangular(dom, "#", " and arrays");
    if dom.rank != 1 then compilerError(
      "cannot apply '#' with an integer to multi-dimensional domains and arrays");

    return chpl_countDomHelp(dom, (counts,));
  }

  @chpldoc.nodoc
  operator #(dom: domain, counts: _tuple) {
    errorIfNotRectangular(dom, "#", " and arrays");
    if (counts.size != dom.rank) then compilerError(
      "rank mismatch in '#'");
    return chpl_countDomHelp(dom, counts);
  }

  pragma "last resort"
  @chpldoc.nodoc
  operator #(dom: domain, counts) {
    compilerError("cannot apply '#' to '", dom.type:string,
                  "' using count(s) of type ", counts.type:string);
  }

  //
  // Disallow additions and subtractions to rectangular domains of these types
  // with a specific message, to avoid surprises.
  // Note: add/sub of a rectangular domain and another type will either
  // produce a generic error message or compile as a promoted expression.
  //
  private proc noRDadds(type t) param do return
    isPrimitive(t) || isRange(t) || isTuple(t) || isEnum(t);

  @chpldoc.nodoc
  operator +(d: domain, i: ?t) where d.isRectangular() && noRDadds(t) do
    compilerError("addition of a rectangular domain and ", t:string,
                  " is currently not supported");

  @chpldoc.nodoc
  operator +(i: ?t, d: domain) where d.isRectangular() && noRDadds(t) do
    compilerError("addition of ", t:string,
                  " and a rectangular domain is currently not supported");

  @chpldoc.nodoc
  operator -(d: domain, i: ?t) where d.isRectangular() && noRDadds(t) do
    compilerError("subtraction of a rectangular domain and ", t:string,
                  " is currently not supported");

  // addition and subtraction on irregular domains has set semantics

  @chpldoc.nodoc
  @unstable("'+' on domains is unstable and may change in the future")
  operator +(in d: domain, i: index(d)) where d.isIrregular() {
    d.add(i);
    return d;
  }

  @chpldoc.nodoc
  @unstable("'+' on domains is unstable and may change in the future")
  operator +(i, in d: domain) where isSubtype(i.type,index(d)) && d.isIrregular() {
    d.add(i);
    return d;
  }

  @chpldoc.nodoc
  @unstable("'+' on domains is unstable and may change in the future")
  operator +(in d1: domain, d2: domain) where
                                    d1.type == d2.type &&
                                    d1.isIrregular() &&
                                    d2.isIrregular() {
    // This should eventually become a forall loop
    for e in d2 do d1.add(e);
    return d1;
  }

  @chpldoc.nodoc
  operator +(d1: domain, d2: domain) {
    if (d1.isRectangular() || d2.isRectangular()) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  @chpldoc.nodoc
  @unstable("'+=' on domains is unstable and may change in the future")
  inline operator +=(ref D: domain, idx) { D.add(idx); }
  @chpldoc.nodoc
  @unstable("'+=' on domains is unstable and may change in the future")
  inline operator +=(ref D: domain, param idx) { D.add(idx); }

  @chpldoc.nodoc
  @unstable("'-' on domains is unstable and may change in the future")
  operator -(in d: domain, i: index(d)) where d.isIrregular() {
    d.remove(i);
    return d;
  }

  @chpldoc.nodoc
  operator -(in d1: domain, d2: domain) where
                                    d1.type == d2.type &&
                                    d1.isSparse() {
    // This should eventually become a forall loop
    for e in d2 do d1.remove(e);
    return d1;
  }

  @chpldoc.nodoc
  operator -(d1: domain, d2: domain) {
    if (d1.isRectangular() || d2.isRectangular()) then
      compilerError("Cannot remove indices from a rectangular domain");
    else
      compilerError("Cannot remove indices from this domain type");
  }

  @chpldoc.nodoc
  @unstable("'-=' on domains is unstable and may change in the future")
  inline operator -=(ref D: domain, idx) { D.remove(idx); }
  @chpldoc.nodoc
  @unstable("'-=' on domains is unstable and may change in the future")
  inline operator -=(ref D: domain, param idx) { D.remove(idx); }

  @chpldoc.nodoc
  inline operator ==(d1: domain, d2: domain) where d1.isRectangular() &&
                                                   d2.isRectangular() {
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

  @chpldoc.nodoc
  inline operator !=(d1: domain, d2: domain) where d1.isRectangular() &&
                                                   d2.isRectangular() {
    return !(d1 == d2);
  }

  @chpldoc.nodoc
  inline operator ==(d1: domain, d2: domain) where d1.isAssociative() &&
                                                   d2.isAssociative() {
    if d1._value == d2._value then return true;
    if d1.sizeAs(uint) != d2.sizeAs(uint) then return false;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return false;
    return true;
  }

  @chpldoc.nodoc
  inline operator !=(d1: domain, d2: domain) where d1.isAssociative() &&
                                                   d2.isAssociative() {
    return !(d1 == d2);
  }

  @chpldoc.nodoc
  inline operator ==(d1: domain, d2: domain) where d1.isSparse() &&
                                                   d2.isSparse() {
    if d1._value == d2._value then return true;
    if d1.sizeAs(uint) != d2.sizeAs(uint) then return false;
    if d1._value.parentDom != d2._value.parentDom then return false;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return false;
    return true;
  }

  @chpldoc.nodoc
  inline operator !=(d1: domain, d2: domain) where d1.isSparse() &&
                                                   d2.isSparse() {
    return !(d1 == d2);
  }

  // any combinations not handled by the above

  private proc cmpError(d1, d2) {
    if d1.isRectangular() then
      compilerError("comparing a rectangular domain against",
        " an associative or sparse domain is not currently supported");
    if d1.isAssociative() then
      compilerError("comparing an associative domain against",
        " a rectangular or sparse domain is not currently supported");
    if d1.isSparse() then
      compilerError("comparing a sparse domain against",
        " a rectangular or associative domain is not currently supported");
    // we should not get here, however if we do...
    compilerError("comparing '", d1.type:string, "' against '",
                  d2.type:string, "' is not currently supported");
  }

  @chpldoc.nodoc
  inline operator ==(d1: domain, d2: domain) {
    cmpError(d1, d2);
  }

  @chpldoc.nodoc
  inline operator !=(d1: domain, d2: domain) {
    cmpError(d1, d2);
  }

  // check this before comparing two domains with == or !=
  proc chpl_sameDomainKind(d1: domain, d2: domain) param do
    return (d1.isRectangular() && d2.isRectangular()) ||
           (d1.isAssociative() && d2.isAssociative()) ||
           (d1.isSparse()      && d2.isSparse()     );

  // This is perhaps an approximation, for use in error messages.
  private proc canBeIteratedOver(const ref arg) param {
    use Reflection;
    return isSubtype(arg.type, _iteratorRecord) ||
           canResolveMethod(arg, "these");
  }

  private proc domainDescription(const ref d) param do return
    if d.isRectangular() then "a rectangular " + d.rank:string + "-dim domain"
    else if d.isSparse() then "a sparse "      + d.rank:string + "-dim domain"
    else "an associative domain";

  @chpldoc.nodoc
  @unstable("'-' on domains is unstable and may change in the future")
  operator -(a :domain, b :domain) where (a.type == b.type) &&
    a.isAssociative() {
    var newDom : a.type;
    serial !newDom._value.parSafe do
      forall e in a with(ref newDom) do
        if !b.contains(e) then newDom.add(e);
    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  @chpldoc.nodoc
  @unstable("'-=' on domains is unstable and may change in the future")
  operator -=(ref a :domain, b :domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      if a.contains(e) then
        a.remove(e);
  }

  @chpldoc.nodoc
  @unstable("'|' on domains is unstable and may change in the future")
  operator |(a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    return a + b;
  }

  @chpldoc.nodoc
  @unstable("'|=' on domains is unstable and may change in the future")
  operator |=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      a.add(e);
  }

  @chpldoc.nodoc
  operator |=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '|=' on a rectangular domain");
  }

  @chpldoc.nodoc
  @unstable("'+=' on domains is unstable and may change in the future")
  operator +=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    a |= b;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  @chpldoc.nodoc
  @unstable("'&' on domains is unstable and may change in the future")
  operator &(a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    var newDom : a.type;

    serial !newDom._value.parSafe do
      forall k in a with (ref newDom) do // no race - in 'serial'
        if b.contains(k) then newDom += k;
    return newDom;
  }

  @chpldoc.nodoc
  @unstable("'&=' on domains is unstable and may change in the future")
  operator &=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    var removeSet: domain(a.idxType);
    for e in a do
      if !b.contains(e) then
        removeSet += e;
    for e in removeSet do
      a.remove(e);
  }

  @chpldoc.nodoc
  operator &=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '&=' on a rectangular domain");
  }

  @chpldoc.nodoc
  @unstable("'^' on domains is unstable and may change in the future")
  operator ^(a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    var newDom : a.type;

    serial !newDom._value.parSafe {
      forall k in a with(ref newDom) do
        if !b.contains(k) then newDom.add(k);
      forall k in b with(ref newDom) do
        if !a.contains(k) then newDom.add(k);
    }

    return newDom;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, it is
     added to the LHS.
  */
  @chpldoc.nodoc
  @unstable("'^=' on domains is unstable and may change in the future")
  operator ^=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      if a.contains(e) then
        a.remove(e);
      else
        a.add(e);
  }

  @chpldoc.nodoc
  operator ^=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '^=' on a rectangular domain");
  }

  //
  // BaseSparseDom operator overloads
  //
  @chpldoc.nodoc
  operator +=(ref sd: domain, inds: [] index(sd)) where sd.isSparse() {
    if inds.sizeAs(int) == 0 then return;

    sd._value.dsiBulkAdd(inds);
  }


  // TODO: Currently not optimized
  @chpldoc.nodoc
  operator +=(ref sd: domain, d: domain)
  where sd.isSparse() && d.rank==sd.rank && sd.idxType==d.idxType {
    if d.sizeAs(int) == 0 then return;

    const indCount = d.sizeAs(int);
    var arr: [{0..#indCount}] index(sd);

    for (i,j) in zip(d, 0..) do arr[j] = i;

    var rowSorted = false;

    // Once an interface supports it:
    // if sd.RMO && d.RMO then rowSorted = true;

    sd._value.dsiBulkAddNoPreserveInds(arr, rowSorted, true);
  }

  // TODO: Implement bulkRemove
  @chpldoc.nodoc
  operator -=(ref sd: domain, inds: [] index(sd)) where sd.isSparse() {
    for ind in inds do
      sd -= ind;
  }

  @chpldoc.nodoc
  operator -=(ref sd: domain, d: domain)
  where sd.isSparse() && d.rank==sd.rank && sd.idxType==d.idxType {
    for ind in d do
      sd -= ind;
  }

  //
  // Assignment of domains
  //
  @chpldoc.nodoc
  operator =(ref a: domain, b: domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in domain assignment");

    if a.idxType != b.idxType then
      compilerError("index type mismatch in domain assignment");

    if a.isRectangular() && b.isRectangular() then
      if ! chpl_assignStrideIsSafe(a.strides, b.strides) then
        compilerError("assigning to a domain with strideKind.",a.strides:string,
                      " from a domain with strideKind.", b.strides:string,
                      " without an explicit cast");

    a._instance.dsiAssignDomain(b, lhsPrivate=false);

    if _isPrivatized(a._instance) {
      _reprivatize(a._instance);
    }
  }

  proc chpl__checkTupIrregDomAssign(const ref d, const ref idx, param msg)  {
    if isCoercible(idx.type, d.fullIdxType) ||
          // sparse 1-d domains also allow adding 1-tuples
          d.isSparse() && d.rank == 1 && isCoercible(idx.type, 1*d.idxType)
      then return;

    compilerError("cannot assign a tuple ", msg, idx.type:string,
                  " into ", domainDescription(d),
                  " with idxType ", d.idxType:string);
  }

  //
  // Return true if t is a tuple of ranges that is legal to assign to
  // rectangular domain d
  // The check that d.idxType accepts t(i) is done in op=(domain,domain).
  //
  proc chpl__isLegalRectTupDomAssign(d, t) param {
    if ! d.isRectangular() then return false;
    if ! (d.rank == t.size) then return false;

    // does the tuple 't' contain only ranges?
    for param dim in 0..t.size-1 do
      if ! isRange(t(dim)) then return false;

    // are those ranges' 'strides' compatible with 'd'?
    for param dim in 0..t.size-1 do
      if ! chpl_assignStrideIsSafe(d.dim(dim), t(dim)) then return false;

    // all checks passed
    return true;
  }

  @chpldoc.nodoc
  operator =(ref a: domain, b: _tuple) {
    if chpl__isLegalRectTupDomAssign(a, b) {
      a = {(...b)};
    } else if a.isRectangular() {
      compilerError("cannot assign a ", b.type:string,
                    " to a rectangular domain");
    } else {
      a.clear();
      if isHomogeneousTuple(b) then
        // let the backend compiler unroll this loop to optimize
        for ind in 0..#b.size {
          chpl__checkTupIrregDomAssign(a, b(ind), "of ");
          a.add(b(ind));
        }
      else
        // unroll in the source code to allow heterogeneous tuple elements
        for ind in b {
          chpl__checkTupIrregDomAssign(a, ind, "containing ");
          a.add(ind);
        }
    }
  }

  @chpldoc.nodoc
  operator =(ref d: domain, r: range(?)) {
    d = {r};
  }

  @chpldoc.nodoc
  operator =(ref a: domain, b) {  // b is iteratable
    if a.isRectangular() then
      compilerError("assigning ", b.type:string, " to a rectangular domain");
    if ! canBeIteratedOver(b) then
      compilerError("assigning ", b.type:string, " to an irregular domain");
    a.clear();
    for ind in b {
      a.add(ind);
    }
  }

  private proc allUint(b) param {
    if isInt(b) then return false;
    if isUint(b) then return true;
    if isTuple(b) then return allUintTup(0);
    return false; // an error will be issued by _makeIndexTuple()
    proc allUintTup(param dim) param do
      if dim == b.size then return true;
      else if ! isUint(b(dim)) then return false;
      else return allUintTup(dim+1);
  }

  // This is the definition of the 'by' operator for domains.
  @chpldoc.nodoc
  operator by(a: domain, b) {
    errorIfNotRectangular(a, "by");
    param newStrides = if ! allUint(b) then strideKind.any
                       else chpl_strideProduct(a.strides, strideKind.positive);
    var r: a.rank*range(a._value.idxType, boundKind.both, newStrides);
    var t = _makeIndexTuple(a.rank, b, "step", expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) by t(i);
    return new _domain(a.distribution, a.rank, a._value.idxType, newStrides, r);
  }

  @chpldoc.nodoc
  operator by(a: domain, param b: integral) {
    errorIfNotRectangular(a, "by");
    param newStrides = chpl_strideProduct(a.dim(0), b);
    var r: a.rank*range(a._value.idxType, boundKind.both, newStrides);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) by b;
    return new _domain(a.distribution, a.rank, a._value.idxType, newStrides, r);
  }

  // This is the definition of the 'align' operator for domains.
  // It produces a new domain with the specified alignment.
  // See also: 'align' operator on ranges.
  @chpldoc.nodoc
  operator align(a: domain, b) {
    errorIfNotRectangular(a, "align");
    var r: a.rank*range(a._value.idxType, boundKind.both, a.strides);
    var t = _makeIndexTuple(a.rank, b, "alignment", expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) align t(i);
    return new _domain(a.distribution, a.rank, a._value.idxType, a.strides, r);
  }

  // This function exists to avoid communication from computing _value when
  // the result is param.
  proc chpl_domainDistIsLayout(d: domain) param {
    return d.distribution._value.dsiIsLayout();
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:_domain, definedConst: bool) {
    param rhsIsLayout = chpl_domainDistIsLayout(rhs);

    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if chpl_domainDistIsLayout(lhs) && !rhsIsLayout then
      compilerWarning("initializing a non-distributed domain from a distributed domain. If you didn't mean to do that, add a dmapped clause to the type expression or remove the type expression altogether");

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:_domain,
                        definedConst: bool) {
    param rhsIsLayout = chpl_domainDistIsLayout(rhs);

    // TODO: just return rhs
    // if the domain types are the same and their runtime types
    // are the same.

    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if chpl_domainDistIsLayout(lhs) && !rhsIsLayout then
      compilerWarning("initializing a non-distributed domain from a distributed domain. If you didn't mean to do that, add a dmapped clause to the type expression or remove the type expression altogether");

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:_tuple, definedConst: bool) {
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if chpl__isLegalRectTupDomAssign(lhs, rhs) {
      lhs = {(...rhs)};
    } else {
      lhs = rhs;
    }

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:_tuple, definedConst: bool) {
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if chpl__isLegalRectTupDomAssign(lhs, rhs) {
      lhs = {(...rhs)};
    } else {
      lhs = rhs;
    }

    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:range(?), definedConst: bool) {
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = {rhs};
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:range(?), definedConst: bool) {
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = {rhs};
    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs: _iteratorRecord, definedConst: bool) {
    // assumes rhs is iterable
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if lhs.isRectangular() then
      compilerError("assigning ", rhs.type:string, " to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, rhs: _iteratorRecord, definedConst: bool) {
    // assumes rhs is iterable
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if lhs.isRectangular() then
      compilerError("assigning ", rhs.type:string, " to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs, definedConst: bool) {
    // assumes rhs is iterable (e.g. list)
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if lhs.isRectangular() then
      compilerError("assigning ", rhs.type:string, " to a rectangular domain");
    if ! canBeIteratedOver(rhs) then
      compilerError("assigning ", rhs.type:string, " to an irregular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs, definedConst: bool) {
    // assumes rhs is iterable (e.g. list)
    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    if lhs.isRectangular() then
      compilerError("assigning ", rhs.type:string, " to a rectangular domain");
    if ! canBeIteratedOver(rhs) then
      compilerError("assigning ", rhs.type:string, " to an irregular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
  }




  //
  // Domain wrapper record.
  //
  /* The domain type. */
  pragma "domain"
  pragma "has runtime type"
  pragma "ignore noinit"
  record _domain : writeSerializable, readDeserializable {
    var _pid:int; // only used when privatized
    pragma "owned"
    var _instance; // generic, but an instance of a subclass of BaseDom
    var _unowned:bool; // 'true' for the result of 'getDomain'
                       // in which case, the record destructor should
                       // not attempt to delete the _instance.

    proc chpl__promotionType() type {
      return index(rank, _value.idxType);
    }

    @chpldoc.nodoc
    proc init(_pid: int, _instance, _unowned: bool) {
      this._pid = _pid;
      this._instance = _instance;
      this._unowned = _unowned;
    }

    @chpldoc.nodoc
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

    @chpldoc.nodoc
    proc init(d,
              param rank : int,
              type idxType = int,
              param strides = strideKind.one,
              definedConst: bool = false) {
      this.init(d.newRectangularDom(rank, idxType, strides, definedConst));
    }

    @chpldoc.nodoc
    proc init(d,
              param rank : int,
              type idxType = int,
              param strides = strideKind.one,
              ranges: rank*range(idxType, boundKind.both,strides),
              definedConst: bool = false) {
      this.init(d.newRectangularDom(rank, idxType, strides, ranges,
                definedConst));
    }

    @chpldoc.nodoc
    proc init(d,
              type idxType,
              param parSafe: bool = false,
              definedConst: bool = false) {
      this.init(d.newAssociativeDom(idxType, parSafe));
    }

    @chpldoc.nodoc
    proc init(d,
              dom: domain,
              definedConst: bool = false) {
      this.init(d.newSparseDom(dom.rank, dom._value.idxType, dom));
    }

    // Note: init= does not handle the case where the type of 'this' does not
    // handle the type of 'other'. That case is currently managed by the
    // compiler and various helper functions involving runtime types.
    @chpldoc.nodoc
    proc init=(const ref other : domain) where other.isRectangular() {
      this.init(other.distribution, other.rank, other.idxType, other.strides,
                other.dims());
    }

    @chpldoc.nodoc
    proc init=(const ref other : domain) {
      if other.isAssociative() {
        this.init(other.distribution, other.idxType, other.parSafe);
      } else if other.isSparse() {
        this.init(other.distribution, other.parentDom);
      } else {
        compilerError("cannot initialize '", this.type:string, "' from '", other.type:string, "'");
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

    proc chpl__serialize()
      where this._value.isDefaultRectangular() {
      return this._value.chpl__serialize();
    }

    // TODO: we *SHOULD* be allowed to query the type of '_instance' directly
    // This function may not use any run-time type information passed to it
    // in the form of the 'this' argument. Static/param information is OK.
    proc type chpl__deserialize(data) {
      type valueType = __primitive("static field type", this, "_instance");
      return new _domain(_to_borrowed(valueType).chpl__deserialize(data));
    }

    @chpldoc.nodoc
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
    @chpldoc.nodoc
    proc deinit () {
      _do_destroy();
    }

    /////////// basic properties ///////////

    /* Returns the domain map that implements this domain. */
    pragma "return not owned"
    proc distribution {
      use Reflection;
      if canResolveMethod(_value, "dsiGetDist") {
        return _value.dsiGetDist();
      } else {
        // TODO: Remove this branch and conditional once _distribution is
        // retired
        return _getDistribution(_value.dist);
      }
    }

    /* Prevent users from accessing internal datatypes unintentionally. It
       used to be a public method deprecated in favor of domain.distribution. */
    @chpldoc.nodoc
    proc dist do compilerError("'domain.dist' is no longer supported,",
                               " use 'domain.distribution' instead");

    /* Returns the number of dimensions in this domain. */
    proc rank param {
      if this.isRectangular() || this.isSparse() then
        return _value.rank;
      else
        return 1;
    }

    /* Returns the type used to represent the indices of this domain.
       For a multidimensional domain, this represents the
       per-dimension index type. */
    proc idxType type {
      return _value.idxType;
    }

    /* Returns the full type used to represent the indices of this
       domain.  For a 1D or associative domain, this is the same
       as :proc:`idxType` above.  For a multidimensional domain, it
       is :proc:`rank` * :proc:`idxType`. */
    proc fullIdxType type {
      if this.isAssociative() || this.rank == 1 {
        return this.idxType;
      } else {
        return this.rank * this.idxType;
      }
    }

    proc chpl_integralIdxType type {
      return chpl__idxTypeToIntIdxType(_value.idxType);
    }

    /* Returns the 'strides' parameter of the domain. */
    proc strides param where this.isRectangular() do return _value.strides;

    @chpldoc.nodoc
    proc strides param where this.isSparse() do return _value.parentDom.strides;

    @chpldoc.nodoc
    proc strides param where this.isAssociative() {
      compilerError("associative domains do not support .strides");
    }

    @chpldoc.nodoc proc hasUnitStride() param do return strides.isOne();
    @chpldoc.nodoc proc hasPosNegUnitStride() param do return strides.isPosNegOne();

    /* Returns the stride of the indices in this domain. */
    proc stride do return _value.dsiStride;

    @chpldoc.nodoc proc stride param where rank==1 &&
      (isRectangular() || isSparse()) && strides.isPosNegOne() do
      return if strides.isOne() then 1 else -1;

    /* Returns the alignment of the indices in this domain. */
    proc alignment do return _value.dsiAlignment;

    @chpldoc.nodoc proc alignment param where rank==1 &&
      (isRectangular() || isSparse()) && strides.isPosNegOne() do return 0;

    /* Returns an array of locales over which this domain
       has been distributed.  */
    proc targetLocales() const ref {
      return _value.dsiTargetLocales();
    }

    /////////// these() and this() ///////////

    /* Yield the domain indices */
    @chpldoc.nodoc
    iter these() {
      for i in _value.these() {
        yield i;
      }
    }
    @chpldoc.nodoc
    iter these(param tag: iterKind)
      where tag == iterKind.standalone &&
            __primitive("resolves", _value.these(tag=tag)) {
      for i in _value.these(tag) do
        yield i;
    }
    @chpldoc.nodoc
    iter these(param tag: iterKind)
      where tag == iterKind.leader {
      // If I use forall here, it says
      //   error: user invocation of a parallel iterator should not supply tag
      //   arguments -- they are added implicitly by the compiler
      for followThis in _value.these(tag) do
        yield followThis;
    }
    @chpldoc.nodoc
    iter these(param tag: iterKind, followThis, param fast: bool = false)
      where tag == iterKind.follower {

      if __primitive("resolves", _value.these(tag=tag, followThis, fast=fast)) {
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
    @chpldoc.nodoc
    proc this(d: domain) {
      if d.rank == rank then
        return this((...d.getIndices()));
      else
        compilerError("slicing a domain with a domain of a different rank");
    }

    // domain slicing by tuple of ranges
    @chpldoc.nodoc
    proc this(ranges...rank)
    where chpl__isTupleOfRanges(ranges) {
      const r = tupleOfRangesSlice(dims(), ranges);
      return new _domain(distribution, rank, _value.idxType, r(0).strides, r);
    }

    // domain rank change
    @chpldoc.nodoc
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
      param upstrides = chpl_strideUnion(this.strides,
                                         chpl_strideUnionRC(args));
      var upranges: uprank*range(idxType=_value.idxType, strides=upstrides);
      var updim = 0;

      for param i in 0..rank-1 {
        if (isRange(args(i))) {
          collapsedDim(i) = false;
          idx(i) = dim(i).low;
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

      const rcdist = new unmanaged ArrayViewRankChangeDist(downDistPid=distribution._pid,
                                                 downDistInst=distribution._instance,
                                                 collapsedDim=collapsedDim,
                                                 idx = idx);
      // TODO: Should this be set?
      //rcdist._free_when_no_doms = true;

      const rcdistRec = new _distribution(rcdist);

      return new _domain(rcdistRec, uprank,
                                    upranges(0).idxType,
                                    upranges(0).strides,
                                    upranges);
    }

    // error case for all-int access
    @chpldoc.nodoc
    proc this(i: integral ... rank) {
      compilerError("domain slice requires a range in at least one dimension");
    }

    // anything that is not covered by the above
    @chpldoc.nodoc
    proc this(args ...?numArgs) {
      if numArgs == rank {
        compilerError("invalid argument types for domain slicing");
      } else
        compilerError("a domain slice requires either a single domain argument or exactly one argument per domain dimension");
    }

    /////////// size and dimensions ///////////

    /* Returns true if the domain has no indices. */
    proc isEmpty(): bool {
      return this.sizeAs(uint) == 0;
    }

    /* Returns the number of indices in this domain as an ``int``. */
    proc size: int {
      return this.sizeAs(int);
    }

    /* Returns the number of indices in this domain as the specified type. */
    proc sizeAs(type t: integral): t {
      use HaltWrappers;
      const size = _value.dsiNumIndices;
      if boundsChecking && t != uint && size > max(t) {
        var error = ".size query exceeds max(" + t:string + ")";
        if this.isRectangular() {
          error += " for: '" + this:string + "'";
        }
        HaltWrappers.boundsCheckHalt(error);
      }
      return size: t;
    }

    /*
       Returns a tuple of ranges describing the bounds of a rectangular domain.
       For a sparse domain, returns the bounds of the parent domain.
     */
    proc dims() do return _value.dsiDims();

    /*
       Returns a range representing the boundary of this
       domain in a particular dimension.
     */
    proc dim(d : int) {
      use HaltWrappers;
      import IO.FormattedIO.string;
      if boundsChecking then
        if (d < 0 || d >= rank) then
          HaltWrappers.boundsCheckHalt(try! "dim(%i) is out-of-bounds; must be 0..%i".format(d, rank-1));
      return _value.dsiDim(d);
    }

    @chpldoc.nodoc
    proc dim(param d : int) {
      if (d < 0 || d > rank-1) then
        compilerError("dim() must take a value from 0..", rank-1);
      return _value.dsiDim(d);
    }

    @chpldoc.nodoc
    iter dimIter(param d, ind) {
      for i in _value.dimIter(d, ind) do yield i;
    }

   /* Returns a tuple of ``int`` values representing the size of each
      dimension.

      For a sparse domain, this returns the shape of the parent domain.
    */
    proc shape: rank*int where this.isRectangular() || this.isSparse() {
      return chpl_shapeAs(int);
    }

    proc chpl_shapeAs(type t: integral) {
      var s: rank*t;
      for (i, r) in zip(0..#s.size, dims()) do
        s(i) = r.sizeAs(t);
      return s;
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
      compilerError(".shape not supported on this domain");
    }

    /* This error overload is here because without it, the domain's
       indices tend to be promoted across the `.indices` calls of
       their idxType which can be very confusing. */
    @chpldoc.nodoc
    proc indices {
      compilerError("domains do not support '.indices'");
    }

    // returns a default rectangular domain
    @chpldoc.nodoc proc boundingBox() where this.isRectangular() {
      var dst: rank*range(this.idxType, boundKind.both, strideKind.one);
      const src = this.dims();
      for param dim in 0..rank-1 do dst[dim] = src[dim].boundingBox();
      return {(...dst)};
    }

    /////////// low, high, first, last ///////////

    /* Returns the lowest index represented by a rectangular domain. */
    proc low {
      return _value.dsiAlignedLow;
    }

    @chpldoc.nodoc
    proc low where this.isAssociative() {
      compilerError("associative domains do not support '.low'");
    }

    /* Returns the highest index represented by a rectangular domain. */
    proc high {
      return _value.dsiAlignedHigh;
    }

    @chpldoc.nodoc
    proc high where this.isAssociative() {
      compilerError("associative domains do not support '.high'");
    }

    /* Returns the domain's 'pure' low bound.  For example, given the
       domain ``{1..10 by -2}``, ``.lowBound`` would return 1, whereas
       ``.low`` would return 2 since it's the lowest index represented
       by the domain.  This routine is only supported on rectangular
       domains. */
    proc lowBound {
      return _value.dsiLow;
    }

    /* Returns the domain's 'pure' high bound.  For example, given the
       domain ``{1..10 by 2}``, ``.highBound`` would return 10,
       whereas ``.high`` would return 9 since it's the highest index
       represented by the domain.  This routine is only supported on
       rectangular domains. */
    proc highBound {
      return _value.dsiHigh;
    }

    /* Returns the first index in this domain. */
    proc first do return _value.dsiFirst;

    /* Returns the last index in this domain. */
    proc last do return _value.dsiLast;

    /////////// other ///////////

    proc chpl_checkEltType(type eltType) /*private*/ {
      if eltType == void {
        compilerError("array element type cannot be 'void'");
      } else if eltType == nothing {
        compilerError("array element type cannot be 'nothing'");
      } else if isGenericType(eltType) {
        compilerWarning("creating an array with element type " +
                        eltType:string);
        if isClassType(eltType) && !isGenericType(eltType:borrowed) {
          compilerWarning("which is a class type with generic management");
        }
        compilerError("array element type cannot currently be generic");
        // In the future we might support it if the array is not default-inited
      } else if this.isSparse() && !isDefaultInitializable(eltType) {
        if isNonNilableClass(eltType) {
          compilerError("sparse arrays of non-nilable classes are not currently supported");
        } else {
          compilerError("sparse arrays of non-default-initializable types are not currently supported");
        }
      }
    }

    proc chpl_checkNegativeStride() /*private*/ {
      if noNegativeStrideWarnings then return;
      // todo: add compile-time checks for neg. strides once ranges allow that

      if this.isRectangular() {
/*
// The following causes duplicate compile-time warnings,
// so leaving just the runtime-time ones:
       if this.strides.isNegative() {
        compilerWarning("arrays and array slices with negatively-strided dimensions are currently unsupported and may lead to unexpected behavior; compile with -snoNegativeStrideWarnings to suppress this warning");
       } else
*/
        if ! this.strides.isPositive() {
          for s in chpl__tuplify(this.stride) {
            if s < 0 {
              warning("arrays and array slices with negatively-strided dimensions are currently unsupported and may lead to unexpected behavior; compile with -snoNegativeStrideWarnings to suppress this warning; the dimension(s) are: ", this.dsiDims());
              break;
            }
          }
        }
      }
    }

    pragma "no copy return"
    @chpldoc.nodoc
    proc buildArray(type eltType, param initElts:bool) {
      chpl_checkEltType(eltType);
      chpl_checkNegativeStride();

      var x = _value.dsiBuildArray(eltType, initElts);
      pragma "dont disable remote value forwarding"
      proc help(thisDom) {
        // Workaround: added 'this.' qualification as Dyno can't resolve method
        // call in nested method atm. Anna 2025-02-27
        // Additional workaround: Switched to using explictly passed-in value
        // for 'this'. Anna 2025-03-07.
        thisDom._value.add_arr(x);
      }
      help(this);

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);
    }

    /*
      Invoking this method will attempt to create and return an array
      declared over the domain instance. If there is not enough memory
      to satisfy the allocation, an error will be thrown, allowing
      the program to continue if handled, as opposed to halting and
      thus stopping program execution.

      This method will be most reliable in configurations that use a
      fixed heap (e.g., when using ``CHPL_GASNET_SEGMENT=large``), but
      can be called in all configurations. In the case of a dynamic
      heap, it is possible that overcommit will cause the array
      allocation to succeed, even if there is not enough physical
      memory to satisfy the allocation, which will then fail with a bus
      error when attempting to access the array.

      This method can be called on all domains that implement a
      'doiTryCreateArray' method.

      Throws an `ArrayOomError` when out of memory allocating elements.
    */
    pragma "no copy return"
    @unstable("tryCreateArray() is subject to change in the future.")
    proc tryCreateArray(type eltType) throws {
      if !(__primitive("resolves", _value.doiTryCreateArray(eltType))) then
        compilerError("cannot call 'tryCreateArray' on domains that do not" +
                      " support a 'doiTryCreateArray' method.");

      chpl_checkEltType(eltType);
      chpl_checkNegativeStride();

      var x = _value.doiTryCreateArray(eltType);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);
    }

    pragma "no copy return"
    @unstable("tryCreateArray() is subject to change in the future.")
    proc tryCreateArray(type eltType, initExpr: ?t) throws
      where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType) {
      if !(__primitive("resolves", _value.doiTryCreateArray(eltType))) then
        compilerError("cannot call 'tryCreateArray' on domains that do not" +
                      " support a 'doiTryCreateArray' method.");

      chpl_checkEltType(eltType);
      chpl_checkNegativeStride();

      var x = _value.doiTryCreateArray(eltType);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);
      var res = _newArray(x);
      res = initExpr;

      return res;
    }

    pragma "no copy return"
    @unstable("tryCreateArray() is subject to change in the future.")
    proc tryCreateArray(type eltType, initExpr: [?dom] ?arrayEltType) throws
      where this.rank == dom.rank && isCoercible(arrayEltType, eltType) {
      if !(__primitive("resolves", _value.doiTryCreateArray(eltType))) then
        compilerError("cannot call 'tryCreateArray' on domains that do not" +
                      " support a 'doiTryCreateArray' method.");
      if boundsChecking then
        for (d, ad, i) in zip(this.dims(), dom.dims(), 0..) do
          if d.size != ad.size then halt("Domain size mismatch in 'tryCreateArray' dimension " + i:string);

      chpl_checkEltType(eltType);
      chpl_checkNegativeStride();

      var x = _value.doiTryCreateArray(eltType);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);
      var res = _newArray(x);
      res = initExpr;

      return res;
    }

    // assumes that data is already initialized
    // this function is used in Fortran interop
    pragma "no copy return"
    @chpldoc.nodoc
    proc buildArrayWith(type eltType, data:_ddata(eltType), allocSize:int) {
      chpl_checkEltType(eltType);
      chpl_checkNegativeStride();

      var x = _value.dsiBuildArrayWith(eltType, data, allocSize);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);
    }

    // assumes that the caller has checked:
    //  * that 'from' and the receiver domain match & have compatible types
    //  * that the distributions match as well
    //  * that the 'from array is not unowned
    //  * that the domain/array implementation supports doiBuildArrayMoving
    pragma "no copy return"
    @chpldoc.nodoc
    proc buildArrayMoving(from) {
      var x = _value.doiBuildArrayMoving(from);
      pragma "dont disable remote value forwarding"
      proc help() {
        _value.add_arr(x);
      }
      help();

      chpl_incRefCountsForDomainsInArrayEltTypes(x, x.eltType);

      return _newArray(x);

      // note: 'from' will be deinited here, normally leading to
      // deleting the array instance. The array implementation needs
      // to have set anything stolen to 'nil' in doiBuildArrayMoving
      // and then to take no action on it when deleting.
    }

    /*
      An instance of this type is a context manager that can be used in
      manage statements to resize arrays of non-default-initializable
      element types after resizing their underlying domain.

      Using an instance of this type in a manage statement will cause a
      domain assignment to occur before executing the statement body. The
      left-hand-side of the assignment is the receiver domain that had
      ``unsafeAssign()`` called on it, while the right-hand-side is the
      `dom` formal of the same call.

      If the assignment adds new indices to the assigned domain, then
      corresponding elements are added to arrays declared over it.
      If an array's element type is non-default-initializable, then any
      newly added elements remain uninitialized.

      The ``initialize()`` method can be used within the manage statement
      body to initialize new elements of non-default-initializable arrays
      declared over the assigned domain.

      The new elements of default-initializable arrays over the assigned
      domain will be default-initialized. They can be set to desired
      values as usual, for example using an assignment operator.
    */
    record unsafeAssignManager : contextManager {
      @chpldoc.nodoc
      var _lhsInstance;

      @chpldoc.nodoc
      var _lhsPid: int;

      @chpldoc.nodoc
      var _rhsInstance;

      @chpldoc.nodoc
      var _rhsPid: int;

      @chpldoc.nodoc
      var _oldLhsDomainCopy: domain(?);

      @chpldoc.nodoc
      param _checks: bool;

      @chpldoc.nodoc
      var _isActiveManager: bool;

      @chpldoc.nodoc
      iter _arraysOverLhsDom() {
        for baseArr in _lhsInstance._arrs do
          if !chpl__isArrayView(baseArr) then
            yield baseArr;
      }

      @chpldoc.nodoc
      proc postinit() {
        for baseArr in _arraysOverLhsDom() {
          if _checks {
            if !baseArr.chpl_isElementTypeDefaultInitializable() &&
               !baseArr.chpl_isElementTypeNonNilableClass() {
              param msg = 'Checks in \'unsafeAssign\' for arrays of non-' +
                          'default-initializable elements are not ' +
                          'supported yet';
              halt(msg);
            }
          }
        }
      }

      /*
        Returns ``true`` if this manager has runtime safety checks enabled.
      */
      inline proc checks param do return _checks;

      // Called by implementation code.
      @chpldoc.nodoc
      proc type isClassReferenceNil(const ref x) {
        if isClassType(x.type) {
          return if isBorrowedOrUnmanagedClassType(x.type)
            then x == nil
            else x.chpl_p == nil;
        } else {
          return false;
        }
      }

      // TODO: Make 'nonNilClass == nil' avoid runtime nil checks.
      @chpldoc.nodoc
      proc isClassReferenceNil(const ref x) {
        return this.type.isClassReferenceNil(x);
      }

      @chpldoc.nodoc
      proc _checkThatArrayShapeIsSupported(arr) param {

        // TODO: Test associative, ND rectangular with rank 2+.
        if !arr.isDefaultRectangular() {
          compilerError('The array shape ' + arr.type:string + ' is ' +
                        'not supported');
        }
      }

      @chpldoc.nodoc
      proc _checkThatIndexMatchesArrayShape(arr, idx) param {
        if arr.rank > 1 || idx.type != arr.idxType {
          if idx.type != (arr.rank*arr.idxType) {
            compilerError('invalid index type ' + idx.type:string +
                          ' for array with rank ' + arr.rank:string);
          }
        }
      }

      @chpldoc.nodoc
      proc _isBaseArrClassElementNil(baseArr: BaseArr, idx) {
        return baseArr.chpl_unsafeAssignIsClassElementNil(this,
                                             chpl__tuplify(idx));
      }

      /*
        Returns ``true`` if the value at a given index in an array has
        been initialized.
      */
      proc isElementInitialized(arr: [?d], idx) {

        // E.g., default rectangular...
        _checkThatArrayShapeIsSupported(arr);

        // Only certain forms of index are permissible.
        _checkThatIndexMatchesArrayShape(arr, idx);

        if _checks {
          type T = arr.eltType;
          if isNonNilableClassType(T) {
            var baseArr = chpl__getActualArray(arr);
            return !_isBaseArrClassElementNil(baseArr, idx);
          } else if !isDefaultInitializable(T) {
            halt('Checking if a non-default-initializable element is ' +
                 'initialized is not supported yet');
            return false;
          } else {
            return true;
          }

        // No checks, so just issue a compiler error.
        } else {
          param msg = 'Cannot check initialization state of non-' +
                      'default-initializable array elements from a ' +
                      'manager initialized with \'checks=false\'';
          compilerError(msg);
          return false;
        }
      }

      @chpldoc.nodoc
      proc _checkIfAllElementsAreInitialized(baseArr) {

        // Arrays of non-nilable classes.
        if baseArr.chpl_isElementTypeNonNilableClass() {
          for idx in newIndices() {
            if _isBaseArrClassElementNil(baseArr, idx) {
              baseArr.chpl_unsafeAssignHaltUninitializedElement(idx);
            }
          }

        // TODO: For non-default-initializable elements, we'd iterate
        // through a bitvector mapped to newly added slots.
        } else if !baseArr.chpl_isElementTypeDefaultInitializable() {

          // Should have already halted in postinit.
          halt('internal error: checks for arrays of non-default-' +
               'initializable elements are not supported');

        // Should never reach here.
        } else {
          halt('Internal error!');
        }
      }

      @chpldoc.nodoc
      proc ref _ensureNoLongerManagingThis() {
        if !_isActiveManager then return; else _isActiveManager = false;

        // Possible runtime checks, reset the resize policy of owned arrays.
        for baseArr in _arraysOverLhsDom() {
          if !baseArr.chpl_isElementTypeDefaultInitializable() {
            if _checks then _checkIfAllElementsAreInitialized(baseArr);
            const policy = chpl_ddataResizePolicy.normalInit;
            baseArr.chpl_setResizePolicy(policy);
          }
        }
      }

      @chpldoc.nodoc
      proc ref deinit() {
        _ensureNoLongerManagingThis();
      }

      @chpldoc.nodoc
      proc _isArrayOwnedByLhsDomain(arr) {
        return arr.dsiGetBaseDom() == _lhsInstance;
      }

      @chpldoc.nodoc
      proc _moveInitializeElement(ref arr, idx, in value) {
        import MemMove.moveInitialize;
        ref elem = arr[idx];
        moveInitialize(elem, value);
      }

      @chpldoc.nodoc
      proc _checkNoChecksWhenNonDefaultInitializableEltType(arr) {
        if _checks {
          if !isDefaultInitializable(arr.eltType) &&
             !isNonNilableClass(arr.eltType) {
            param msg = 'Checks in \'unsafeAssign\' for arrays of non-' +
                        'default-initializable elements are not ' +
                        'supported yet';
            compilerError(msg);
          }
        }
      }

      /*
        Initializes a newly added array element at an index with a new value.

        If `checks` is ``true`` and the array element at `idx` has already
        been initialized, this method will halt. If `checks` is ``false``,
        then calling this method on an already initialized element will
        result in undefined behavior.

        It is an error if `idx` is not a valid index in `arr`.
      */
      proc initialize(ref arr: [?d], idx, in value: arr.eltType) {
        import IO.FormattedIO.string;

        // Check to make sure value and array element types match.
        if arr.eltType != value.type then
          compilerError('Initialization value type \'' + value.type:string +
                        '\' does not match array element type \'' +
                        arr.eltType:string + '\'');

        // E.g., default rectangular...
        _checkThatArrayShapeIsSupported(arr);

        // Only certain forms of index are permissible.
        _checkThatIndexMatchesArrayShape(arr, idx);

        // Produce a compiler error.
        _checkNoChecksWhenNonDefaultInitializableEltType(arr);

        // TODO: Remove this restriction in the future?
        if isDefaultInitializable(arr.eltType) then
          compilerError('Cannot call \'initialize\' on array with ' +
                        'default-initializable element type ' +
                        '\'' + arr.eltType:string + '\'');

        if !_isArrayOwnedByLhsDomain(arr) then
          halt('Can only initialize elements of arrays declared over ' +
               'the domain being resized');

        if !arr.domain.contains(idx) then
          halt(try! 'Array index out of bounds: %?'.format(idx));

        if _checks {
          if isElementInitialized(arr, idx) {
            halt(try! "Element at array index '%?' is already initialized".format(idx));
          }
        }

        _moveInitializeElement(arr, idx, value);
      }

      @chpldoc.nodoc
      proc ref enterContext() ref {

        // TODO: Is it possible to nest unsafe assignments? Future work...
        if _isActiveManager {
          halt('Cannot nest a manager for unsafe domain assignment');
        } else {
          _isActiveManager = true;
        }

        for baseArr in _arraysOverLhsDom() {

          // Don't care about arrays with default-initializable elements.
          if baseArr.chpl_isElementTypeDefaultInitializable() then
            continue;

          // Array elements are non-nilable classes. In this case, tell
          // the array to clear new slots when resizing by setting the
          // resize policy. We can use 'nil' slots as sentinel values.
          if baseArr.chpl_isElementTypeNonNilableClass() {
            const policy = if _checks
              then chpl_ddataResizePolicy.skipInitButClearMem
              else chpl_ddataResizePolicy.skipInit;
            baseArr.chpl_setResizePolicy(policy);

          // The array element type is non-default-initializable (but it is
          // not a non-nilable class).
          } else {

            // TODO: Future work could handle this case by using a bitvector
            // to mark slot initialization status. Initialization calls
            // would have to go through the 'initialize' method in order
            // for this approach to work.
            if _checks {
              halt('Runtime checks are currently only supported for ' +
                   'arrays of non-nilable classes');
            } else {
              const policy = chpl_ddataResizePolicy.skipInitButClearMem;
              baseArr.chpl_setResizePolicy(policy);
            }
          }
        }

        // Create temporary shells for the LHS and RHS domains so that we
        // can perform domain assignment.
        var lhsTmpDom = new _domain(_lhsPid, _lhsInstance, _unowned=true);
        var rhsTmpDom = new _domain(_rhsPid, _rhsInstance, _unowned=true);

        // Perform the actual assignment.
        lhsTmpDom = rhsTmpDom;

        return this;
      }

      @chpldoc.nodoc
      proc ref exitContext(in err: owned Error?) throws {
        _ensureNoLongerManagingThis();
        if err then throw err;
      }

      /*
        Iterates over any new indices that will be added to this domain as a
        result of unsafe assignment.
      */
      iter newIndices() {
        var rhsTmpDom = new _domain(_rhsPid, _rhsInstance, _unowned=true);
        for idx in rhsTmpDom do
          if !_oldLhsDomainCopy.contains(idx) then
            yield idx;
      }
    }

    /*
      Returns an instance of :type:`unsafeAssignManager`.

      The returned context manager can be used in a manage statement to
      assign the indices of `dom` into the receiver domain. Within the body
      of the manage statement, the manager can initialize elements of
      non-default-initializable arrays declared over the receiver domain.

      If `checks` is ``true``, this method will guarantee that:

        - Newly added elements of any non-default-initializable arrays
          declared over the receiver domain have been initialized by the
          end of the manage statement
        - Newly added elements are only initialized once

      These guarantees hold only for initialization done through calls to
      the ``initialize()`` method on the context manager. Performing
      any other operation on a newly added array element causes undefined
      behavior until after ``initialize()`` has been called.

      For example:

      .. code-block:: chapel

        var D = {0..0};
        var A: [D] shared C = [new shared C(0)];
        manage D.unsafeAssign({0..1}, checks=true) as mgr {
          // 'D' has a new index '1', so 'A' has a new element at '1',
          // which we need to initialize:
          mgr.initialize(A, 1, new shared C(1));
        }

      .. note::

        Checks are not currently supported for arrays of
        non-default-initializable element types other than arrays of
        non-nilable classes.

      :arg dom: The domain to assign to the receiver
      :arg checks: If this manager should provide runtime safety checks

      :returns: A :type:`unsafeAssignManager` for use in manage statements
    */
    proc ref unsafeAssign(const ref dom: domain, param checks: bool=false) {
      return new unsafeAssignManager(_lhsInstance=_value,
                                     _lhsPid=_pid,
                                     _oldLhsDomainCopy=this,
                                     _rhsInstance=dom._value,
                                     _rhsPid=dom._pid,
                                     _checks=checks,
                                     _isActiveManager=false);
    }

    /* Removes all indices from this domain, leaving it empty. */
    @chpldoc.nodoc
    proc ref clear() where this.isRectangular() {
      // For rectangular domains, create an empty domain and assign it to this
      // one to make sure that we leverage all of the array's normal resizing
      // machinery.

      var emptyDom: this.type;
      this = emptyDom;
    }

    // For other domain types, the implementation probably knows the most
    // efficient way to clear its index set, so make a dsiClear() call.
    /* Removes all indices from this domain, leaving it empty. */
    proc ref clear() {
      _value.dsiClear();
    }

    /* Removes index ``idx`` from this domain. */
    proc ref remove(idx) {
      return _value.dsiRemove(idx);
    }

    // todo: when is it better to have a ref or const ref intent for 'idx'?
    /* Adds index ``idx`` to this domain. This method is also available
       as the ``+=`` operator.
       Returns the number of indices that were added.

       The domain must be irregular.
     */
    proc ref add(in idx) {
      // ensure that the rest of add() deals only with irregular domains
      if isRectangular() {
        compilerError("Cannot add indices to a rectangular domain");

      // 'idx' is an index
      } else if isCoercible(idx.type, fullIdxType) ||
          // sparse 1-d domains also allow adding 1-tuples
          isSparse() && rank == 1 && isCoercible(idx.type, 1*idxType) {
        return _value.dsiAdd(idx);

      // allow promotion
      } else if isCoercible(__primitive("scalar promotion type", idx), fullIdxType) {
        if isSparse() || (isAssociative() && ! this.parSafe) then
          compilerWarning("this promoted addition of indices to ",
            if isSparse() then "a sparse" else "an associative",
            " domain may be unsafe due to race conditions;",
            " consider replacing promotion with an explicit for loop",
            if isSparse() then "" else
              " or declaring the domain type with 'parSafe=true'");
        // we could force serial execution in non-parSafe cases, see #24565
        return + reduce [oneIdx in idx] _value.dsiAdd(oneIdx);

      // for now, disallow calling add() in any other way
      } else {
        compilerError("cannot add a ", idx.type:string, " to ",
                      domainDescription(this), " with idxType ", idxType:string);
      }
    }

    @chpldoc.nodoc
    @unstable("bulkAdd() is subject to change in the future.")
    proc ref bulkAdd(inds: [] _value.idxType, dataSorted=false,
        isUnique=false, addOn=nilLocale)
        where this.isSparse() && _value.rank==1 {
      if inds.isEmpty() then return 0;

      return _value.dsiBulkAdd(inds, dataSorted, isUnique, addOn);
    }

    @chpldoc.nodoc
    @unstable("bulkAddNoPreserveInds() is subject to change in the future.")
    proc ref bulkAddNoPreserveInds(ref inds: [] _value.idxType, dataSorted=false,
        isUnique=false, addOn=nilLocale)
        where this.isSparse() && _value.rank==1 {
      if inds.isEmpty() then return 0;

      return _value.dsiBulkAddNoPreserveInds(inds, dataSorted, isUnique, addOn);
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
          var idxBuf = spsDom.createIndexBuffer(size=N);
          for i in someIndexIterator() do
            idxBuf.add(i);
          idxBuf.commit();

     The above snippet will create a buffer of size N indices, and will
     automatically commit indices to the sparse domain as the buffer fills up.
     Indices are also committed when the buffer goes out of scope.

     :arg size: Size of the buffer in number of indices.
     :type size: int
    */
    @unstable("createIndexBuffer() is subject to change in the future.")
    inline proc createIndexBuffer(size: int) {
      return _value.dsiCreateIndexBuffer(size);
    }

    /*
       Adds indices in ``inds`` to this domain in bulk.

       For sparse domains, an operation equivalent to this method is available
       with the ``+=`` operator, where the right-hand-side is an array. However,
       in that case, default values will be used for the flags ``dataSorted`` and
       ``isUnique``. This method is available because in
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

       .. note::

         This method may make a copy of ``inds`` if the data is not sorted to
         preserve the indices used. If the data is already sorted, it is
         possible to avoid this extra copy by using :proc:`bulkAddNoPreserveInds`,
         which does not copy the indices and may modify ``inds`` in place.

       :arg inds: Indices to be added. ``inds`` must be an array of
                  ``rank*idxType``, except for 1-D domains, where it must be
                  an array of ``idxType``.

       :arg dataSorted: ``true`` if data in ``inds`` is sorted.
       :type dataSorted: bool

       :arg isUnique: ``true`` if data in ``inds`` has no duplicates.
       :type isUnique: bool

       :arg addOn: The locale where the indices should be added. Default value
                   is ``nil`` which indicates that locale is unknown or there
                   are more than one.
       :type addOn: locale

       :returns: Number of indices added to the domain
       :rtype: int
    */
    @unstable("bulkAdd() is subject to change in the future.")
    proc ref bulkAdd(inds: [] _value.rank*_value.idxType,
        dataSorted=false, isUnique=false, addOn=nilLocale)
        where this.isSparse() && _value.rank>1 {
      if inds.isEmpty() then return 0;

      return _value.dsiBulkAdd(inds, dataSorted, isUnique, addOn);
    }

    /*
       Adds indices in ``inds`` to this domain in bulk.

       This is nearly identical to :proc:`bulkAdd`. :proc:`bulkAdd` may
       make a copy of ``inds`` if the data is unsorted, whereas this method will
       modify ``inds`` in place.

       .. note::

         Right now, this method is only available for sparse domains.
         In the future, we expect that this method will be available for all
         irregular domains.

       .. note::

         ``nilLocale`` is a sentinel value to denote that the locale where this
         addition should occur is unknown. We expect this to change in the
         future.

       :arg inds: Indices to be added. ``inds`` must be an array of
                  ``rank*idxType``, except for 1-D domains, where it must be
                  an array of ``idxType``.

       :arg dataSorted: ``true`` if data in ``inds`` is sorted.
       :type dataSorted: bool

       :arg isUnique: ``true`` if data in ``inds`` has no duplicates.
       :type isUnique: bool

       :arg addOn: The locale where the indices should be added. Default value
                   is ``nil`` which indicates that locale is unknown or there
                   are more than one.
       :type addOn: locale

       :returns: Number of indices added to the domain
       :rtype: int
    */
    @unstable("bulkAddNoPreserveInds() is subject to change in the future.")
    proc ref bulkAddNoPreserveInds(ref inds: [] _value.rank*_value.idxType,
        dataSorted=false, isUnique=false, addOn=nilLocale)
        where this.isSparse() && _value.rank>1 {
      if inds.isEmpty() then return 0;

      return _value.dsiBulkAddNoPreserveInds(inds, dataSorted, isUnique, addOn);
    }

    pragma "last resort" @chpldoc.nodoc
    proc bulkAdd(args...) {
      compilerError("incompatible argument(s) or this domain type does not support 'bulkAdd'");
    }

    pragma "last resort" @chpldoc.nodoc
    proc bulkAddNoPreserveInds(args...) {
      compilerError("incompatible argument(s) or this domain type does not support 'bulkAddNoPreserveInds'");
    }

    /* Requests space for a particular number of values in an
       domain.

       Currently only applies to associative domains.
     */
    proc ref requestCapacity(capacity) {

      if capacity < 0 then
        halt("domain.requestCapacity can only be invoked when capacity >= 0");

      if !this.isAssociative() then
        compilerError("domain.requestCapacity only applies to associative domains");

      _value.dsiRequestCapacity(capacity);
    }

    @chpldoc.nodoc
    proc contains(const idx: rank*_value.idxType) {
      if this.isRectangular() || this.isSparse() then
        return _value.dsiMember(_makeIndexTuple(rank, idx, "index"));
      else
        return _value.dsiMember(idx(0));
    }

    /* Returns true if this domain contains ``idx``. Otherwise returns false.
       For sparse domains, only indices with a value are considered
       to be contained in the domain.
     */
    inline proc contains(const idx: _value.idxType ...rank) {
      return contains(idx);
    }

    /* Returns true if this domain contains all the indices in the domain
       ``other``. */
    proc contains(other: domain) {
      if this.rank != other.rank then
        compilerError("rank mismatch in 'domain.contains()': ",
                      this.rank:string, " vs. ", other.rank:string);

      if this.isRectangular() && other.isRectangular() {
        const thisDims  = this.dims();
        const otherDims = other.dims();
        for param i in 0..<this.rank do
          if ! thisDims(i).contains(otherDims(i)) then
            return false;
        return true;

      } else {
        if ! isCoercible(other.idxType, this.idxType) then
          compilerError("incompatible idxType in 'domain.contains()':",
                        " cannot convert from '", other.idxType:string,
                        "' to '", this.idxType:string, "'");

        const otherSize = other.sizeAs(uint);
        if otherSize == 0 then return true;

        const thisSize  = this.sizeAs(uint);
        if thisSize < otherSize then return false;

        return && reduce forall i in other do this.contains(i);
      }
    }

    // 1/5/10: do we want to support order() and position()?
    @chpldoc.nodoc
    proc indexOrder(i) do return _value.dsiIndexOrder(_makeIndexTuple(rank, i, "index"));

    /*
      Returns the `ith` index in the domain counting from 0.
      For example, ``{2..10 by 2}.orderToIndex(2)`` would return ``6``.

      The order of a multidimensional domain follows its serial iterator.
      For example, ``{1..3, 1..2}.orderToIndex(3)`` would return ``(2, 2)``.

      .. note::

        Right now, this method supports only dense rectangular domains with
        numeric indices

      :arg order: Order for which the corresponding index in the domain
                  has to be found.

      :returns: Domain index for a given order in the domain.
    */
    @unstable("domain.orderToIndex() is unstable and its behavior may change in the future")
    proc orderToIndex(order: int) where (this.isRectangular() && isNumericType(this.idxType)){

      if boundsChecking then
        checkOrderBounds(order);

      var rankOrder = order;
      var idx: (rank*_value.idxType);
      var div = this.sizeAs(int);

      for param i in 0..<rank {
          var currDim = this.dim(i);
          div /= currDim.sizeAs(int);
          const lo = currDim.low;
          const hi = currDim.high;
          const stride = currDim.stride;
          const zeroInd = rankOrder/div;
          var currInd = zeroInd*stride;
          if stride < 0 then
            currInd+=hi;
          else
            currInd+=lo;
          idx[i] = currInd: idxType;
          rankOrder = rankOrder%div;
      }
      if(this.rank==1) then
        return idx[0];
      else
        return idx;
    }

    pragma "last resort"
    @chpldoc.nodoc
    proc orderToIndex(order) {
      if this.isRectangular() && isNumericType(this.idxType) then
        compilerError("illegal value passed to orderToIndex():",
          " the argument 'order' must be an integer, excluding uint(64)");
      else
        compilerError("this domain type does not support 'orderToIndex'");
    }

    @chpldoc.nodoc
    proc checkOrderBounds(order: int){
      if order >= this.sizeAs(uint) || order < 0 {
        if this.isEmpty() then
          halt("orderToIndex() invoked on an empty domain");
        else
          halt("illegal order in orderToIndex(): ", order,
               ". For this domain, order must lie in 0..", this.sizeAs(uint)-1);
      }
    }

    @chpldoc.nodoc
    proc position(i) {
      var ind = _makeIndexTuple(rank, i, "index"), pos: rank*chpl_integralIdxType;
      for d in 0..rank-1 do
        pos(d) = _value.dsiDim(d).indexOrder(ind(d));
      return pos;
    }

    @chpldoc.nodoc
    proc expand(off: rank*integral) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("expand not supported on associative domains");
      else if this.isSparse() then
        compilerError("expand not supported on sparse domains");
      else
        compilerError("expand not supported on this domain type");
    }

    @chpldoc.nodoc
    @unstable("domain.expand() is unstable and its behavior may change in the future")
    proc expand(off: integral ...rank) do return expand(off);

    /* Returns a new domain that is the current domain expanded by
       ``off(d)`` in dimension ``d`` if ``off(d)`` is positive or
       contracted by ``off(d)`` in dimension ``d`` if ``off(d)``
       is negative.

       See :proc:`ChapelRange.range.expand` for further information about what
       it means to expand a range.

     */
    @unstable("domain.expand() is unstable and its behavior may change in the future")
    proc expand(off: rank*integral) {
      var ranges = dims();
      for i in 0..rank-1 do {
        ranges(i) = ranges(i).expand(off(i));
        if (ranges(i).lowBound > ranges(i).highBound) {
          halt("***Error: Degenerate dimension created in dimension ", i, "***");
        }
      }

      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    /* Returns a new domain that is the current domain expanded by
       ``off`` in all dimensions if ``off`` is positive or contracted
       by ``off`` in all dimensions if ``off`` is negative.

       See :proc:`ChapelRange.range.expand` for further information about what
       it means to expand a range.
     */
    @unstable("domain.expand() is unstable and its behavior may change in the future")
    proc expand(off: integral) where rank > 1 {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).expand(off);
      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    @chpldoc.nodoc
    proc exterior(off: rank*integral) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("exterior not supported on associative domains");
      else if this.isSparse() then
        compilerError("exterior not supported on sparse domains");
      else
        compilerError("exterior not supported on this domain type");
    }

    @chpldoc.nodoc
    @unstable("domain.exterior() is unstable and its behavior may change in the future")
    proc exterior(off: integral ...rank) do return exterior(off);

    /* Returns a new domain that is the exterior portion of the
       current domain with ``off(d)`` indices for each dimension ``d``.
       If ``off(d)`` is negative, compute the exterior from the low
       bound of the dimension; if positive, compute the exterior
       from the high bound.

       See :proc:`ChapelRange.range.exterior` for further information about what
       it means to compute the exterior of a range.

     */
    @unstable("domain.exterior() is unstable and its behavior may change in the future")
    proc exterior(off: rank*integral) {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).exterior(off(i));
      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    /* Returns a new domain that is the exterior portion of the
       current domain with ``off`` indices for each dimension.
       If ``off`` is negative, compute the exterior from the low
       bound of the dimension; if positive, compute the exterior
       from the high bound.

       See :proc:`ChapelRange.range.exterior` for further information about what
       it means to compute the exterior of a range.

     */
    @unstable("domain.exterior() is unstable and its behavior may change in the future")
    proc exterior(off:integral) where rank != 1 {
      var offTup: rank*off.type;
      for i in 0..rank-1 do
        offTup(i) = off;
      return exterior(offTup);
    }

    @chpldoc.nodoc
    proc interior(off: rank*integral) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("interior not supported on associative domains");
      else if this.isSparse() then
        compilerError("interior not supported on sparse domains");
      else
        compilerError("interior not supported on this domain type");
    }

    @chpldoc.nodoc
    @unstable("domain.interior() is unstable and its behavior may change in the future")
    proc interior(off: integral ...rank) do return interior(off);

    /* Returns a new domain that is the interior portion of the
       current domain with ``off(d)`` indices for each dimension
       ``d``. If ``off(d)`` is negative, compute the interior from
       the low bound of the dimension; if positive, compute the
       interior from the high bound.

       See :proc:`ChapelRange.range.interior` for further information about what
       it means to compute the exterior of a range.

     */
    @unstable("domain.interior() is unstable and its behavior may change in the future")
    proc interior(off: rank*integral) {
      var ranges = dims();
      for i in 0..rank-1 do {
        if ((off(i) > 0) && (dim(i)._high+1-off(i) < dim(i)._low) ||
            (off(i) < 0) && (dim(i)._low-1-off(i) > dim(i)._high)) {
          halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
        }
        ranges(i) = _value.dsiDim(i).interior(off(i));
      }
      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    /* Returns a new domain that is the interior portion of the
       current domain with ``off`` indices for each dimension.
       If ``off`` is negative, compute the interior from the low
       bound of the dimension; if positive, compute the interior
       from the high bound.

       See :proc:`ChapelRange.range.interior` for further information about what
       it means to compute the exterior of a range.

     */
    @unstable("domain.interior() is unstable and its behavior may change in the future")
    proc interior(off: integral) where rank != 1 {
      var offTup: rank*off.type;
      for i in 0..rank-1 do
        offTup(i) = off;
      return interior(offTup);
    }

    //
    // NOTE: We eventually want to support translate on other domain types
    //
    @chpldoc.nodoc
    proc translate(off: rank*integral) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("translate not supported on associative domains");
      else if this.isSparse() then
        compilerError("translate not supported on sparse domains");
      else
        compilerError("translate not supported on this domain type");
    }

    //
    // Notice that the type of the offset does not have to match the
    // index type.  This is handled in the range.translate().
    //
    @chpldoc.nodoc
    @unstable("domain.translate() is unstable and its behavior may change in the future")
    proc translate(off: integral ...rank) do return translate(off);

    /* Returns a new domain that is the current domain translated by
       ``off(d)`` in each dimension ``d``.

       See :proc:`ChapelRange.range.translate` for further information about
       what it means to translate a range.

     */
    @unstable("domain.translate() is unstable and its behavior may change in the future")
    proc translate(off: rank*integral) {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = _value.dsiDim(i).translate(off(i));
      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    /* Returns a new domain that is the current domain translated by
       ``off`` in each dimension.

       See :proc:`ChapelRange.range.translate()` for further information about
       what it means to translate a range.

     */
    @unstable("domain.translate() is unstable and its behavior may change in the future")
    proc translate(off: integral) where rank != 1 {
      var offTup: rank*off.type;
      for i in 0..rank-1 do
        offTup(i) = off;
      return translate(offTup);
    }

    //
    // intended for internal use only:
    //
    proc chpl__unTranslate(off: integral ...rank) do return chpl__unTranslate(off);
    proc chpl__unTranslate(off: rank*chpl_integralIdxType) {
      var ranges = dims();
      for i in 0..rank-1 do
        ranges(i) = dim(i).chpl__unTranslate(off(i));
      return new _domain(distribution, rank, _value.idxType, strides, ranges);
    }

    @chpldoc.nodoc
    proc ref setIndices(x) {
      _value.dsiSetIndices(x);
      if _isPrivatized(_instance) {
        _reprivatize(_value);
      }
    }

    @chpldoc.nodoc
    proc getIndices() do
      return _value.dsiGetIndices();

    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      _value.dsiSerialWrite(writer);
    }

    @chpldoc.nodoc
    proc ref deserialize(reader, ref deserializer) throws {
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

    @chpldoc.nodoc
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
       Returns a local view of the sub-domain (slice) defined by the provided
       range(s), halting if the slice contains elements that are not local.
    */
    pragma "no where doc"
    proc localSlice(r... rank)
    where chpl__isTupleOfRanges(r) &&
          !_value.isDefaultRectangular()
    {
      return _value.dsiLocalSlice(chpl_strideUnion(r), r);
    }

    /*
       Returns a local view of the sub-domain (slice) defined by the provided
       domain, halting if the slice contains elements that are not local.
     */
    proc localSlice(d: domain) {
      return localSlice((...d.getIndices()));
    }

    // associative array interface
    /*
      Yields the domain indices in sorted order. This method is only supported
      on associative domains.

      .. warning::

         It is recommended to use :proc:`Sort.sorted` instead of this method.

    */
    iter sorted(comparator:?t = chpl_defaultComparator()) {
      if !this.isAssociative() then
        compilerError("'.sorted()' is only supported on associative domains");
      if !noSortedWarnings then
        compilerWarning(
          "It is recommended to use 'Sort.sorted' instead of this method. ",
          "Compile with '-snoSortedWarnings' to suppress this warning.");
      for x in this._value.dsiSorted(comparator) do yield x;
    }

    @chpldoc.nodoc
    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    @chpldoc.nodoc
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
      return this.distribution;
    }

    /* Returns true if the local subdomain can be represented as a single
       domain. Otherwise returns false. */
    @unstable("'hasSingleLocalSubdomain' on domains is unstable and may change in the future")
    proc hasSingleLocalSubdomain() param {
      return _value.dsiHasSingleLocalSubdomain();
    }

    /*
       Returns the subdomain that is local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    proc localSubdomain(loc: locale = here) {
      if !_value.dsiHasSingleLocalSubdomain() then
        compilerError("the domain may have multiple local subdomains");

      return _value.dsiLocalSubdomain(loc);
    }

    /*
       Yields the subdomains that are local to `loc`.

       :arg loc: indicates the locale for which the query should take
                 place (defaults to `here`)
       :type loc: locale
    */
    @unstable("'localSubdomains' on domains is unstable and may change in the future")
    iter localSubdomains(loc: locale = here) {
      if _value.dsiHasSingleLocalSubdomain() {
        yield localSubdomain(loc);
      } else {
        foreach d in _value.dsiLocalSubdomains(loc) do yield d;
      }
    }

    @chpldoc.nodoc
    proc supportsAutoLocalAccess() param {
      return _value.dsiSupportsAutoLocalAccess();
    }

    @chpldoc.nodoc
    proc supportsOffsetAutoLocalAccess() param {
      return _value.dsiSupportsOffsetAutoLocalAccess();
    }

    @chpldoc.nodoc
    proc autoLocalAccessOffsetCheck(offsets) {
      return _value.dsiAutoLocalAccessOffsetCheck(offsets);
    }

    @chpldoc.nodoc
    proc supportsArrayViewElision() param {
      return _value.dsiSupportsArrayViewElision();
    }

    @chpldoc.nodoc
    proc supportsShortArrayTransfer() param {
      return _value.dsiSupportsShortArrayTransfer();
    }

    @chpldoc.nodoc
    proc iteratorYieldsLocalElements() param {
      return _value.dsiIteratorYieldsLocalElements();
    }

    /* Casts a rectangular domain to a new rectangular domain type.
       Throws an IllegalArgumentError when the original bounds and/or stride(s)
       do not fit in the new idxType or when the original stride(s)
       are not legal for the new `strides` parameter.
     */
    pragma "no where doc"
    proc tryCast(type t: domain)
      where chpl__isRectangularDomType(t) && this.isRectangular()
        &&  this.chpl_domainTryCastIsSafe(t)
    do
      return try! this.chpl_domainTryCastHelper(t);

    // This overload catches unsupported cases.
    @chpldoc.nodoc
    proc tryCast(type t: domain) throws
    do
      if ! chpl__isRectangularDomType(t) || ! this.isRectangular() then
        compilerError("tryCast() from ", this.type:string, " to ",
                      t:string, " is not available");
      else
        return this.chpl_domainTryCastHelper(t);

    // identical to chpl_domainCastHelper except uses tryCast instead of ':'
    inline proc chpl_domainTryCastHelper(type t:_domain) throws {
      var tmpD: t;
      const ref d = this;
      if tmpD.rank != d.rank then
        compilerError("rank mismatch in tryCast()");
      else {
        var inds = d.getIndices();
        var newInds: tmpD.getIndices().type;
        for param i in 0..tmpD.rank-1 {
          newInds(i) = inds(i).tryCast( newInds(i).type );
        }
        tmpD.setIndices(newInds);
        return tmpD;
      }
    }

    proc chpl_domainTryCastIsSafe(type t: domain) param {
      var dst: t;
      // this is implemented only for rectangular domains
      compilerAssert(this.isRectangular() && dst.isRectangular());
      return chpl_tryCastIsSafe(this.dim(0), dst.dim(0).type);
    }

    /* Casts a rectangular domain to a new rectangular domain type.
       The overload below throws when the original bounds and/or stride
       do not fit in the new type or 'strides'.
       TODO: should we allow 't' to be generic?
     */
    @chpldoc.nodoc
    operator :(d: _domain, type t:_domain)
      where chpl__isRectangularDomType(t) && d.isRectangular()
        &&  d.chpl_domainCastIsSafe(t)
    do
      return try! d.chpl_domainCastHelper(t);

    // This overload catches unsupported cases.
    @chpldoc.nodoc
    operator :(d: _domain, type t: domain) throws
    do
      if ! chpl__isRectangularDomType(t) || ! d.isRectangular() then
        compilerError("cast from ", d.type:string, " to ",
                      t:string, " is not available");
      else
        return d.chpl_domainCastHelper(t);

    inline proc chpl_domainCastHelper(type t:_domain) throws {
      var tmpD: t;
      const ref d = this;
      if tmpD.rank != d.rank then
        compilerError("rank mismatch in cast");
      else {
        var inds = d.getIndices();
        var newInds: tmpD.getIndices().type;
        for param i in 0..tmpD.rank-1 {
          newInds(i) = inds(i): newInds(i).type;
        }
        tmpD.setIndices(newInds);
        return tmpD;
      }
    }

    proc chpl_domainCastIsSafe(type t: domain) param {
      var dst: t;
      // this is implemented only for rectangular domains
      compilerAssert(this.isRectangular() && dst.isRectangular());
      return chpl_castIsSafe(this.dim(0), dst.dim(0).type);
    }

    @chpldoc.nodoc
    operator :(val: _domain, type t: string) {
      use Reflection;
      if canResolveMethod(val._value, "doiToString") {
        return val._value.doiToString();
      } else {
        import IO.FormattedIO.string;
        return try! "%?".format(val);
      }
    }

    /* Returns true if this domain is a rectangular.
       Otherwise returns false.  */
    proc isRectangular() param {
      return this._value.isRectangular();
    }

    /* Returns true if ``d`` is an irregular domain; e.g. is not rectangular.
       Otherwise returns false. */
    proc isIrregular() param {
      return this.isSparse() || this.isAssociative();
    }

    /* Returns true if ``d`` is an associative domain.
       Otherwise returns false. */
    proc isAssociative() param {
      return this._value.isAssociative();
    }

    /* Returns true if ``d`` is a sparse domain. Otherwise returns false. */
    proc isSparse() param {
      return this._value.isSparse();
    }

  }  // record _domain

  //
  // Support for creating domains using tuples of bounds
  //

  /* Creates a rectangular domain with bounds defined by the scalar values `low`
      and `high`. If `inclusive` is true, the domain includes the `high` value.
      Otherwise, the domain excludes the `high` value.
   */
  @unstable("makeRectangularDomain() is subject to change in the future.")
  proc makeRectangularDomain(low: ?t1, high: ?t2, param inclusive: bool = true)
    where chpl_isValidRangeIdxType(t1) && chpl_isValidRangeIdxType(t2)
  {
    return if inclusive then {low..high} else {low..<high};
  }

  /* Creates a multidimensional rectangular domain with bounds defined by the
     pairwise elements of `low` and `high`. If `inclusive` is true, the domain
     includes the `high` values. Otherwise, the domain excludes the `high`
     values. For example, `makeRectangularDomain((1, 2), (10,11))` is
     equivalent to `{1..10, 2..11}`.
   */
  @unstable("makeRectangularDomain() is subject to change in the future.")
  proc makeRectangularDomain(low: ?t1, high: ?t2, param inclusive: bool = true)
    where isTuple(low) && isTuple(high) &&
          isHomogeneousTuple(low) && isHomogeneousTuple(high) &&
          low.size == high.size &&
          (isCoercible(low(0).type, high(0).type) ||
           isCoercible(high(0).type, low(0).type))
  {
    param size = low.size;
    type eltType;
    if (low(0).type == high(0).type) {
      eltType = low(0).type;
    } else {
      eltType = (low(0) + high(0)).type;
    }
    var ranges:  size*range(eltType);
    for param i in 0..<size {
        if inclusive then
          ranges[i] = low[i]..high[i];
        else
          ranges[i] = low[i]..<high[i];
    }
    const d: domain(size, eltType) = ranges;
    return d;
  }

  /* Creates a rectangular domain with bounds defined by one tuple and one
     scalar value. The scalar argument is used in each dimension of the domain,
     while the 'n'-th tuple element is used to define the 'n'-th dimension of
     the domain. If `inclusive` is true, the domain includes the `high` value.
     Otherwise, the domain excludes the `high` value. For example,
     `makeRectangularDomain((1, 2), 10)` is equivalent to `{1..10, 2..10}`
     and `makeRectangularDomain(1, (10, 11), inclusive=false)` is equivalent
     to `{1..<10, 1..<11}`.
    */
  @unstable("makeRectangularDomain() is subject to change in the future.")
  proc makeRectangularDomain(low: ?t1, high: ?t2, param inclusive: bool = true)
    where isTuple(low) != isTuple(high) {
      param size = if isTuple(low) then low.size else high.size;
      type eltType = if isTuple(low) then
                       (if low(0).type == high.type then low(0).type else (low(0) + high).type)
                     else
                       (if high(0).type == low.type then high(0).type else (low + high(0)).type);
      var ranges: size*range(eltType);
      if isTuple(low) {
        if !isHomogeneousTuple(low) then compilerError("Domains defined using tuple bounds must use homogeneous tuples, but got '" + low.type:string + "'");
        for param i in 0..<size {
          if inclusive then
            ranges[i] = low[i]..high;
          else
            ranges[i] = low[i]..<high;
        }
      } else {
        if !isHomogeneousTuple(high) then compilerError("Domains defined using tuple bounds must use homogeneous tuples, but got '" + high.type:string + "'");
        for param i in 0..<size {
          if inclusive then
            ranges[i] = low..high[i];
          else
            ranges[i] = low..<high[i];
        }
      }
      const d: domain(size, eltType) = ranges;
      return d;
  }

  pragma "last resort"
  @chpldoc.nodoc
  proc makeRectangularDomain(low: ?t1, high: ?t2, param inclusive: bool = true)
    where isTuple(low) && isTuple(high)
  {
    if !isHomogeneousTuple(low) || !isHomogeneousTuple(high) then
      compilerError("Domains defined using tuple bounds must use homogeneous tuples, but got '" +
                    low.type:string + "' and '" + high.type:string + "'");
    else if low.size != high.size then
      compilerError("Domains defined using tuple bounds must use tuples of the same length, " +
                    "but got '" + low.type:string + "' and '" + high.type:string + "'");
    else if !(isCoercible(low(0).type, high(0).type) ||
              isCoercible(high(0).type, low(0).type)) then
      compilerError("Domains defined using tuple bounds must use tuples of coercible types. " +
                    "Cannot coerce between '" + low(0).type:string + "' and '" +
                    high(0).type:string + "'");
  }
}
