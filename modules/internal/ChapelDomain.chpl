/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
  use ArrayViewRankChange;

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
  proc chpl__buildDomainRuntimeType(dist: _distribution, param rank: int,
                                    type idxType = int,
                                    param stridable: bool = false) type {
    return new _domain(dist, rank, idxType, stridable);
  }

  pragma "runtime type init fn"
  proc chpl__buildDomainRuntimeType(dist: _distribution, type idxType,
                                    param parSafe: bool = true) type {
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
  proc chpl__buildSparseDomainRuntimeType(dist: _distribution,
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

  proc chpl__convertRuntimeTypeToValue(dist: _distribution,
                                       param rank: int,
                                       type idxType = int,
                                       param stridable: bool,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, rank, idxType, stridable, definedConst);
  }

  proc chpl__convertRuntimeTypeToValue(dist: _distribution, type idxType,
                                       param parSafe: bool,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, idxType, parSafe);
  }

  proc chpl__convertRuntimeTypeToValue(dist: _distribution,
                                       parentDom: domain,
                                       param isNoInit: bool,
                                       definedConst: bool) {
    return new _domain(dist, parentDom);
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

  proc chpl__buildDomainExpr(ranges..., definedConst)
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
    if definedConst then
      chpl__setDomainConst(d);
    return d;
  }

  pragma "no doc"
  private proc chpl__setDomainConst(dom: domain) {
    dom._value.definedConst = true;
  }

  // definedConst is added only for interface consistency
  proc chpl__buildDomainExpr(keys..., definedConst) {
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

  deprecated "isRectangularDom is deprecated - please use isRectangular method on domain"
  proc isRectangularDom(d: domain) param return d.isRectangular();

  deprecated "isIrregularDom is deprecated - please use isIrregular method on domain"
  proc isIrregularDom(d: domain) param return d.isIrregular();

  deprecated "isAssociativeDom is deprecated - please use isAssociative method on domain"
  proc isAssociativeDom(d: domain) param return d.isAssociative();

  proc chpl_isAssociativeDomClass(dc: BaseAssociativeDom) param return true;
  proc chpl_isAssociativeDomClass(dc) param return false;

  deprecated "isSparseDom is deprecated - please use isSparse method on domain"
  proc isSparseDom(d: domain) param return d.isSparse();

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
    return dom[(...ranges)];
  }

  operator #(dom: domain, counts: integral) {
    errorIfNotRectangular(dom, "#", " and arrays");
    if dom.rank != 1 then compilerError(
      "cannot apply '#' with an integer to multi-dimensional domains and arrays");

    return chpl_countDomHelp(dom, (counts,));
  }

  operator #(dom: domain, counts: _tuple) {
    errorIfNotRectangular(dom, "#", " and arrays");
    if (counts.size != dom.rank) then compilerError(
      "rank mismatch in '#'");
    return chpl_countDomHelp(dom, counts);
  }

  pragma "last resort"
  operator #(dom: domain, counts) {
    compilerError("cannot apply '#' to '", dom.type:string,
                  "' using count(s) of type ", counts.type:string);
  }

  operator +(d: domain, i: index(d)) {
    if d.isRectangular() then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  operator +(i, d: domain) where isSubtype(i.type, index(d)) && !d.isIrregular() {
    if d.isRectangular() then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  operator +(in d: domain, i: index(d)) where d.isIrregular() {
    d.add(i);
    return d;
  }

  operator +(i, in d: domain) where isSubtype(i.type,index(d)) && d.isIrregular() {
    d.add(i);
    return d;
  }

  operator +(in d1: domain, d2: domain) where
                                    d1.type == d2.type &&
                                    d1.isIrregular() &&
                                    d2.isIrregular() {
    // This should eventually become a forall loop
    for e in d2 do d1.add(e);
    return d1;
  }

  operator +(d1: domain, d2: domain) {
    if (d1.isRectangular() || d2.isRectangular()) then
      compilerError("Cannot add indices to a rectangular domain");
    else
      compilerError("Cannot add indices to this domain type");
  }

  inline operator +=(ref D: domain, idx) { D.add(idx); }
  inline operator +=(ref D: domain, param idx) { D.add(idx); }

  operator -(d: domain, i: index(d)) {
    if d.isRectangular() then
      compilerError("Cannot remove indices from a rectangular domain");
    else
      compilerError("Cannot remove indices from this domain type");
  }

  operator -(in d: domain, i: index(d)) where d.isIrregular() {
    d.remove(i);
    return d;
  }

  operator -(in d1: domain, d2: domain) where
                                    d1.type == d2.type &&
                                    d1.isSparse() {
    // This should eventually become a forall loop
    for e in d2 do d1.remove(e);
    return d1;
  }

  operator -(d1: domain, d2: domain) {
    if (d1.isRectangular() || d2.isRectangular()) then
      compilerError("Cannot remove indices from a rectangular domain");
    else
      compilerError("Cannot remove indices from this domain type");
  }

  inline operator -=(ref D: domain, idx) { D.remove(idx); }
  inline operator -=(ref D: domain, param idx) { D.remove(idx); }

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

  inline operator !=(d1: domain, d2: domain) where d1.isRectangular() &&
                                                   d2.isRectangular() {
    return !(d1 == d2);
  }

  inline operator ==(d1: domain, d2: domain) where d1.isAssociative() &&
                                                   d2.isAssociative() {
    if d1._value == d2._value then return true;
    if d1.sizeAs(uint) != d2.sizeAs(uint) then return false;
    // Should eventually be a forall+reduction
    for idx in d1 do
      if !d2.contains(idx) then return false;
    return true;
  }

  inline operator !=(d1: domain, d2: domain) where d1.isAssociative() &&
                                                   d2.isAssociative() {
    return !(d1 == d2);
  }

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

  inline operator ==(d1: domain, d2: domain) {
    cmpError(d1, d2);
  }

  inline operator !=(d1: domain, d2: domain) {
    cmpError(d1, d2);
  }

  // check this before comparing two domains with == or !=
  proc chpl_sameDomainKind(d1: domain, d2: domain) param
    return (d1.isRectangular() && d2.isRectangular()) ||
           (d1.isAssociative() && d2.isAssociative()) ||
           (d1.isSparse()      && d2.isSparse()     );

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
  operator -=(ref a :domain, b :domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      if a.contains(e) then
        a.remove(e);
  }

  operator |(a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    return a + b;
  }

  deprecated "'|' on rectangular domains is deprecated"
  operator |(a :domain, b: domain) where a.isRectangular() && b.isRectangular()
    return forall (aa, bb) in zip(a, b) do aa|bb;

  operator |=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      a.add(e);
  }

  operator |=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '|=' on a rectangular domain");
  }

  operator +=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    a |= b;
  }

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, no error
     occurs.
  */
  operator &(a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    var newDom : a.type;

    serial !newDom._value.parSafe do
      forall k in a with (ref newDom) do // no race - in 'serial'
        if b.contains(k) then newDom += k;
    return newDom;
  }

  deprecated "'&' on rectangular domains is deprecated"
  operator &(a :domain, b: domain) where a.isRectangular() && b.isRectangular()
    return forall (aa, bb) in zip(a, b) do aa&bb;

  operator &=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    var removeSet: domain(a.idxType);
    for e in a do
      if !b.contains(e) then
        removeSet += e;
    for e in removeSet do
      a.remove(e);
  }

  operator &=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '&=' on a rectangular domain");
  }

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

  deprecated "'^' on rectangular domains is deprecated"
  operator ^(a :domain, b: domain) where a.isRectangular() && b.isRectangular()
    return forall (aa, bb) in zip(a, b) do aa^bb;

  /*
     We remove elements in the RHS domain from those in the LHS domain only if
     they exist. If an element in the RHS is not present in the LHS, it is
     added to the LHS.
  */
  operator ^=(ref a :domain, b: domain) where (a.type == b.type) &&
    a.isAssociative() {
    for e in b do
      if a.contains(e) then
        a.remove(e);
      else
        a.add(e);
  }

  operator ^=(a :domain, b: domain) where a.isRectangular() {
    compilerError("cannot invoke '^=' on a rectangular domain");
  }

  //
  // BaseSparseDom operator overloads
  //
  operator +=(ref sd: domain, inds: [] index(sd)) where sd.isSparse() {
    if inds.sizeAs(int) == 0 then return;

    sd._value.dsiBulkAdd(inds);
  }


  // TODO: Currently not optimized
  operator +=(ref sd: domain, d: domain)
  where sd.isSparse() && d.rank==sd.rank && sd.idxType==d.idxType {
    if d.sizeAs(int) == 0 then return;

    const indCount = d.sizeAs(int);
    var arr: [{0..#indCount}] index(sd);

    for (i,j) in zip(d, 0..) do arr[j] = i;

    var rowSorted = false;

    // Once an interface supports it:
    // if sd.RMO && d.RMO then rowSorted = true;

    sd._value.dsiBulkAdd(arr, rowSorted, true, false);
  }

  // TODO: Implement bulkRemove
  operator -=(ref sd: domain, inds: [] index(sd)) where sd.isSparse() {
    for ind in inds do
      sd -= ind;
  }

  operator -=(ref sd: domain, d: domain)
  where sd.isSparse() && d.rank==sd.rank && sd.idxType==d.idxType {
    for ind in d do
      sd -= ind;
  }

  //
  // Assignment of domains
  //
  operator =(ref a: domain, b: domain) {
    if a.rank != b.rank then
      compilerError("rank mismatch in domain assignment");

    if a.idxType != b.idxType then
      compilerError("index type mismatch in domain assignment");

    if a.isRectangular() && b.isRectangular() then
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

  operator =(ref a: domain, b: _tuple) {
    if chpl__isLegalRectTupDomAssign(a, b) {
      a = {(...b)};
    } else {
      a.clear();
      for ind in 0..#b.size {
        a.add(b(ind));
      }
    }
  }

  operator =(ref d: domain, r: range(?)) {
    d = {r};
  }

  operator =(ref a: domain, b) {  // b is iteratable
    if a.isRectangular() then
      compilerError("Illegal assignment to a rectangular domain");
    a.clear();
    for ind in b {
      a.add(ind);
    }
  }

  // This is the definition of the 'by' operator for domains.
  operator by(a: domain, b) {
    errorIfNotRectangular(a, "by");
    var r: a.rank*range(a._value.idxType,
                      BoundedRangeType.bounded,
                      true);
    var t = _makeIndexTuple(a.rank, b, "step", expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) by t(i);
    return new _domain(a.dist, a.rank, a._value.idxType, true, r);
  }

  // This is the definition of the 'align' operator for domains.
  // It produces a new domain with the specified alignment.
  // See also: 'align' operator on ranges.
  operator align(a: domain, b) {
    errorIfNotRectangular(a, "align");
    var r: a.rank*range(a._value.idxType,
                      BoundedRangeType.bounded,
                      a.stridable);
    var t = _makeIndexTuple(a.rank, b, "alignment", expand=true);
    for param i in 0..a.rank-1 do
      r(i) = a.dim(i) align t(i);
    return new _domain(a.dist, a.rank, a._value.idxType, a.stridable, r);
  }

  // This function exists to avoid communication from computing _value when
  // the result is param.
  proc domainDistIsLayout(d: domain) param {
    return d.dist._value.dsiIsLayout();
  }

  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceCopy(type dstType:_domain, rhs:_domain, definedConst: bool) {
    param rhsIsLayout = domainDistIsLayout(rhs);

    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if domainDistIsLayout(lhs) && !rhsIsLayout then
      compilerWarning("initializing a non-distributed domain from a distributed domain. If you didn't mean to do that, add a dmapped clause to the type expression or remove the type expression altogether");

    return lhs;
  }
  pragma "find user line"
  pragma "coerce fn"
  proc chpl__coerceMove(type dstType:_domain, in rhs:_domain,
                        definedConst: bool) {
    param rhsIsLayout = domainDistIsLayout(rhs);

    // TODO: just return rhs
    // if the domain types are the same and their runtime types
    // are the same.

    pragma "no copy"
    var lhs = chpl__coerceHelp(dstType, definedConst);
    lhs = rhs;

    // Error for assignment between local and distributed domains.
    if domainDistIsLayout(lhs) && !rhsIsLayout then
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
      compilerError("Illegal assignment to a rectangular domain");
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
      compilerError("Illegal assignment to a rectangular domain");
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
      compilerError("Illegal assignment to a rectangular domain");
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
      compilerError("Illegal assignment to a rectangular domain");
    lhs.clear();
    for ind in rhs {
      lhs.add(ind);
    }
    return lhs;
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
              param stridable: bool = false,
              definedConst: bool = false) {
      this.init(d.newRectangularDom(rank, idxType, stridable, definedConst));
    }

    proc init(d: _distribution,
              param rank : int,
              type idxType = int,
              param stridable: bool = false,
              ranges: rank*range(idxType, BoundedRangeType.bounded,stridable),
              definedConst: bool = false) {
      this.init(d.newRectangularDom(rank, idxType, stridable, ranges,
                definedConst));
    }

    proc init(d: _distribution,
              type idxType,
              param parSafe: bool = true,
              definedConst: bool = false) {
      this.init(d.newAssociativeDom(idxType, parSafe));
    }

    proc init(d: _distribution,
              dom: domain,
              definedConst: bool = false) {
      this.init(d.newSparseDom(dom.rank, dom._value.idxType, dom));
    }

    // Note: init= does not handle the case where the type of 'this' does not
    // handle the type of 'other'. That case is currently managed by the
    // compiler and various helper functions involving runtime types.
    proc init=(const ref other : domain) where other.isRectangular() {
      this.init(other.dist, other.rank, other.idxType, other.stridable, other.dims());
    }

    proc init=(const ref other : domain) {
      if other.isAssociative() {
        this.init(other.dist, other.idxType, other.parSafe);
      } else if other.isSparse() {
        this.init(other.dist, other.parentDom);
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
      if this.isRectangular() || this.isSparse() then
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
    proc stridable param where this.isRectangular() {
      return _value.stridable;
    }

    pragma "no doc"
    proc stridable param where this.isSparse() {
      return _value.parentDom.stridable;
    }

    pragma "no doc"
    proc stridable param where this.isAssociative() {
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
            __primitive("resolves", _value.these(tag=tag)) {
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

   /* Return a tuple of ``int`` values representing the size of each
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

    pragma "no doc"
    /* Associative domains assumed to be 1-D. */
    proc shape where this.isAssociative() {
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
      } else if this.isSparse() && !isDefaultInitializable(eltType) {
        if isNonNilableClass(eltType) {
          compilerError("sparse arrays of non-nilable classes are not currently supported");
        } else {
          compilerError("sparse arrays of non-default-initializable types are not currently supported");
        }
      }

      if chpl_warnUnstable then
        if this.isRectangular() && this.stridable then
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

    // assumes that data is already initialized
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
    record unsafeAssignManager {
      pragma "no doc"
      var _lhsInstance;

      pragma "no doc"
      var _lhsPid: int;

      pragma "no doc"
      var _rhsInstance;

      pragma "no doc"
      var _rhsPid: int;

      pragma "no doc"
      var _oldLhsDomainCopy: domain;

      pragma "no doc"
      param _checks: bool;

      pragma "no doc"
      var _isActiveManager: bool;

      pragma "no doc"
      iter _arraysOverLhsDom() {
        for baseArr in _lhsInstance._arrs do
          if !chpl__isArrayView(baseArr) then
            yield baseArr;
      }

      pragma "no doc"
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
      inline proc checks param return _checks;

      // Called by implementation code.
      pragma "no doc"
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
      pragma "no doc"
      proc isClassReferenceNil(const ref x) {
        return this.type.isClassReferenceNil(x);
      }

      pragma "no doc"
      proc _checkThatArrayShapeIsSupported(arr) param {

        // TODO: Test associative, ND rectangular with rank 2+.
        if !arr.isDefaultRectangular() {
          compilerError('The array shape ' + arr.type:string + ' is ' +
                        'not supported');
        }
      }

      pragma "no doc"
      proc _checkThatIndexMatchesArrayShape(arr, idx) param {
        if arr.rank > 1 || idx.type != arr.idxType {
          if idx.type != (arr.rank*arr.idxType) {
            compilerError('invalid index type ' + idx.type:string +
                          ' for array with rank ' + arr.rank:string);
          }
        }
      }

      pragma "no doc"
      proc _isBaseArrClassElementNil(baseArr: BaseArr, idx) {
        var idxTup = if isTuple(idx) then idx else (idx,);
        return baseArr.chpl_unsafeAssignIsClassElementNil(this, idxTup);
      }

      /*
        Return ``true`` if the value at a given index in an array has
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

      pragma "no doc"
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

      pragma "no doc"
      proc _ensureNoLongerManagingThis() {
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

      pragma "no doc"
      proc deinit() {
        _ensureNoLongerManagingThis();
      }

      pragma "no doc"
      proc _isArrayOwnedByLhsDomain(arr) {
        return arr.dsiGetBaseDom() == _lhsInstance;
      }

      pragma "no doc"
      proc _moveInitializeElement(arr, idx, in value) {
        import Memory.Initialization.moveInitialize;
        ref elem = arr[idx];
        moveInitialize(elem, value);
      }

      pragma "no doc"
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
        Initialize a newly added array element at an index with a new value.

        If `checks` is ``true`` and the array element at `idx` has already
        been initialized, this method will halt. If `checks` is ``false``,
        then calling this method on an already initialized element will
        result in undefined behavior.

        It is an error if `idx` is not a valid index in `arr`.
      */
      proc initialize(arr: [?d], idx, in value: arr.eltType) {

        // Check to make sure value and array element types match.
        if arr.eltType != value.type then
          compilerError('Initialization value type \'' + value:string +
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
          halt('Array index out of bounds: ' + idx:string);

        // Get a reference to the array slot.
        ref elem = arr[idx];

        if _checks {
          if isElementInitialized(arr, idx) {
            halt('Element at array index \'' + idx:string + '\' ' +
                 'is already initialized');
          }
        }

        _moveInitializeElement(arr, idx, value);
      }

      pragma "no doc"
      proc enterThis() ref {

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

      pragma "no doc"
      proc leaveThis(in err: owned Error?) throws {
        _ensureNoLongerManagingThis();
        if err then throw err;
      }

      /*
        Iterate over any new indices that will be added to this domain as a
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

    /* Remove all indices from this domain, leaving it empty */
    proc ref clear() where this.isRectangular() {
      // For rectangular domains, create an empty domain and assign it to this
      // one to make sure that we leverage all of the array's normal resizing
      // machinery.

      var emptyDom: this.type;
      this = emptyDom;
    }

    // For other domain types, the implementation probably knows the most
    // efficient way to clear its index set, so make a dsiClear() call.
    pragma "no doc"
    proc ref clear() {
      _value.dsiClear();
    }

    /* Add index ``idx`` to this domain. This method is also available
       as the ``+=`` operator.

       The domain must be irregular.
     */
    proc ref add(in idx) {
      return _value.dsiAdd(idx);
    }

    pragma "last resort" pragma "no doc"
    deprecated "the formal 'i' is deprecated, please use 'idx' instead"
    proc ref add(in i) {
      return add(i);
    }

    pragma "no doc"
    proc ref bulkAdd(inds: [] _value.idxType, dataSorted=false,
        isUnique=false, preserveInds=true, addOn=nilLocale)
        where this.isSparse() && _value.rank==1 {
      if chpl_warnUnstable then compilerWarning(
        "bulkAdd() is subject to change in the future.");

      if inds.isEmpty() then return 0;

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

          The interface and implementation are not stable and may change in the
          future.

     :arg size: Size of the buffer in number of indices.
     :type size: int
    */
    inline proc makeIndexBuffer(size: int) {
      if chpl_warnUnstable then compilerWarning(
        "makeIndexBuffer() is subject to change in the future.");

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

       .. note::

          The interface and implementation are not stable and may change in the
          future.

       :arg inds: Indices to be added. ``inds`` must be an array of
                  ``rank*idxType``, except for 1-D domains, where it must be
                  an array of ``idxType``.

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
    proc ref bulkAdd(inds: [] _value.rank*_value.idxType,
        dataSorted=false, isUnique=false, preserveInds=true, addOn=nilLocale)
        where this.isSparse() && _value.rank>1 {
      if chpl_warnUnstable then compilerWarning(
        "bulkAdd() is subject to change in the future.");

      if inds.isEmpty() then return 0;

      return _value.dsiBulkAdd(inds, dataSorted, isUnique, preserveInds, addOn);
    }

    pragma "no doc" pragma "last resort"
    proc bulkAdd(args...) {
      compilerError("incompatible argument(s) or this domain type does not support 'bulkAdd'");
    }

    /* Remove index ``idx`` from this domain */
    proc ref remove(idx) {
      return _value.dsiRemove(idx);
    }

    pragma "last resort" pragma "no doc"
    deprecated "the formal 'i' is deprecated, please use 'idx' instead"
    proc ref remove(i) {
      return remove(i);
    }

    /* Request space for a particular number of values in an
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

    pragma "last resort" pragma "no doc"
    deprecated "the formal 'i' is deprecated, please use 'capacity' instead"
    proc ref requestCapacity(i) {
      requestCapacity(i);
    }

    /*
      Return the number of indices in this domain as an ``int``.
    */
    proc size: int {
      return this.sizeAs(int);
    }

    /* Return the number of indices in this domain as the specified type */
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

    /* Returns the domain's 'pure' low bound.  For example, given the
       domain ``{1..10 by -2}``, ``.lowBound`` would return 1, whereas
       ``.low`` would return 2 since it's the lowest index represented
       by the domain.  This routine is only supported on rectangular
       domains. */
    proc lowBound {
      return _value.dsiLow;
    }

    /* Return the lowest index represented by a rectangular domain. */
    proc low {
      if !alignedBoundsByDefault && stridable {
        compilerWarning("The '.low' query on ranges is in the process of changing from returning the pure low bound to the aligned low bound (e.g., from '1' to '2' for '1..10 by -2').  Update to the '.lowBound' query if you want to retain the old behavior, or recompile with '-salignedBoundsByDefault=true' to opt into the new behavior.");
      }
      return if alignedBoundsByDefault then _value.dsiAlignedLow else _value.dsiLow;
    }
    pragma "no doc"
    proc low where this.isAssociative() {
      compilerError("associative domains do not support '.low'");
    }

    /* Return the domain's 'pure' high bound.  For example, given the
       domain ``{1..10 by 2}``, ``.highBound`` would return 10,
       whereas ``.high`` would return 9 since it's the highest index
       represented by the domain.  This routine is only supported on
       rectangular domains. */
    proc highBound {
      return _value.dsiHigh;
    }
    /* Return the highest index represented by a rectangular domain. */
    proc high {
    if !alignedBoundsByDefault && stridable {
      compilerWarning("The '.high' query on ranges is in the process of changing from returning the pure high bound to the aligned high bound (e.g., from '10' to '9' for '1..10 by 2').  Update to the '.highBound' query if you want to retain the old behavior, or recompile with '-salignedBoundsByDefault=true' to opt into the new behavior now and avoid this warning.");
    }
      return if alignedBoundsByDefault then _value.dsiAlignedHigh else _value.dsiHigh;
    }
    pragma "no doc"
    proc high where this.isAssociative() {
      compilerError("associative domains do not support '.high'");
    }
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
    proc contains(idx: rank*_value.idxType) {
      if this.isRectangular() || this.isSparse() then
        return _value.dsiMember(_makeIndexTuple(rank, idx, "index"));
      else
        return _value.dsiMember(idx(0));
    }

    /* Return true if this domain contains ``idx``. Otherwise return false.
       For sparse domains, only indices with a value are considered
       to be contained in the domain.
     */
    inline proc contains(idx: _value.idxType ...rank) {
      return contains(idx);
    }

    pragma "last resort" pragma "no doc"
    deprecated "the formal 'i' is deprecated, please use 'idx' instead"
    inline proc contains(i: _value.idxType) {
      return contains(idx=i);
    }

    pragma "last resort" pragma "no doc"
    deprecated "the formal 'i' is deprecated, please use 'idx' instead"
    inline proc contains(i: rank*_value.idxType) {
      return contains(idx=i);
    }

    /* Return true if this domain is a subset of ``super``. Otherwise
       returns false. */
    deprecated "'domain1.isSubset(domain2)' is deprecated, instead please use 'domain2.contains(domain1)'"
    proc isSubset(super : domain) {
      return super.contains(this);
    }

    /* Return true if this domain is a superset of ``sub``. Otherwise
       returns false. */
    deprecated "'domain1.isSuper(domain2)' is deprecated, instead please use 'domain1.contains(domain2)'"
    proc isSuper(sub : domain) {
      return this.contains(sub);
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
    pragma "no doc"
    proc indexOrder(i) return _value.dsiIndexOrder(_makeIndexTuple(rank, i, "index"));

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
    proc orderToIndex(order: int) where (this.isRectangular() && isNumericType(this.idxType)){

      if boundsChecking then
        checkOrderBounds(order);

      var rankOrder = order;
      var idx: (rank*_value.idxType);
      var div = this.sizeAs(int);

      for param i in 0..<rank {
          var currDim = this.dim(i);
          div /= currDim.sizeAs(int);
          const lo = currDim.alignedLow;
          const hi = currDim.alignedHigh;
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
    proc orderToIndex(order) {
      if this.isRectangular() && isNumericType(this.idxType) then
        compilerError("illegal value passed to orderToIndex():",
          " the argument 'order' must be an integer, excluding uint(64)");
      else
        compilerError("this domain type does not support 'orderToIndex'");
    }

    pragma "no doc"
    proc checkOrderBounds(order: int){
      if order >= this.sizeAs(uint) || order < 0 {
        if this.isEmpty() then
          halt("orderToIndex() invoked on an empty domain");
        else
          halt("illegal order in orderToIndex(): ", order,
               ". For this domain, order must lie in 0..", this.sizeAs(uint)-1);
      }
    }

    pragma "no doc"
    proc position(i) {
      var ind = _makeIndexTuple(rank, i, "index"), pos: rank*intIdxType;
      for d in 0..rank-1 do
        pos(d) = _value.dsiDim(d).indexOrder(ind(d));
      return pos;
    }

    pragma "no doc"
    proc expand(off: rank*intIdxType) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("expand not supported on associative domains");
      else if this.isSparse() then
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
        if (ranges(i).lowBound > ranges(i).highBound) {
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
    proc exterior(off: rank*intIdxType) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("exterior not supported on associative domains");
      else if this.isSparse() then
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
    proc interior(off: rank*intIdxType) where !this.isRectangular() {
      if this.isAssociative() then
        compilerError("interior not supported on associative domains");
      else if this.isSparse() then
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
    proc translate(off) where !this.isRectangular() {
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
       return this.sizeAs(uint) == 0;
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
    proc ref setIndices(x) {
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
    proc ref readThis(f) throws {
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
      where chpl__isRectangularDomType(t) && this.isRectangular() {
      var tmpD: t;
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
    proc targetLocales() const ref {
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
        foreach d in _value.dsiLocalSubdomains(loc) do yield d;
      }
    }

    pragma "no doc"
    proc supportsAutoLocalAccess() param {
      return _value.dsiSupportsAutoLocalAccess();
    }

    pragma "no doc"
    proc iteratorYieldsLocalElements() param {
      return _value.dsiIteratorYieldsLocalElements();
    }

    /* Cast a rectangular domain to a new rectangular domain type.  If the old
       type was stridable and the new type is not stridable then assume the
       stride was 1 without checking.

       For example:
       {1..10 by 2}:domain(stridable=false)

       results in the domain '{1..10}'
     */
    pragma "no doc"
    operator :(d: _domain, type t:_domain) where chpl__isRectangularDomType(t) && d.isRectangular() {
      var tmpD: t;
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

    pragma "no doc"
    operator :(val: _domain, type t: string) {
      use Reflection;
      if canResolveMethod(val._value, "doiToString") {
        return val._value.doiToString();
      } else {
        use IO;
        return stringify(val);
      }
    }

    /* Return true if this domain is a rectangular.
       Otherwise return false.  */
    proc isRectangular() param {
      return this._value.isRectangular();
    }

    /* Return true if ``d`` is an irregular domain; e.g. is not rectangular.
       Otherwise return false. */
    proc isIrregular() param {
      return this.isSparse() || this.isAssociative();
    }

    /* Return true if ``d`` is an associative domain. Otherwise return false. */
    proc isAssociative() param {
      return this._value.isAssociative();
    }

    /* Return true if ``d`` is a sparse domain. Otherwise return false. */
    proc isSparse() param {
      return this._value.isSparse();
    }

  }  // record _domain

}
