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


// THE REPLICATED DISTRIBUTION IMPLEMENTATION
//
// Classes defined:
//  Replicated     -- Global distribution descriptor
//  ReplicatedDom      -- Global domain descriptor
//  LocReplicatedDom   -- Local domain descriptor
//  ReplicatedArr      -- Global array descriptor
//  LocReplicatedArr   -- Local array descriptor
//
// Potential extensions:
// - support other kinds of domains
// - allow run-time change in locales

// trace certain DSI methods as they are being invoked
config param traceReplicatedDist = false;


/////////////////////////////////////////////////////////////////////////////
// distribution

//
// (global) distribution class
//
// chpldoc TODO
//   nicer example - pull from primers/distributions.chpl

/*
This Replicated distribution causes a domain and its arrays
to be replicated across the desired locales (all the locales by default).
An array receives a distinct set of elements - a "replicand" -
allocated on each locale.

In other words, a Replicated-distributed domain has
an implicit additional dimension - over the locales,
making it behave as if there is one copy of its indices per locale.

Consistency among the replicands is not preserved automatically.
That is, changes to one replicand of an array are never propagated to
the other replicands by the distribution implementation.
If desired, consistency needs to be maintained by the user.

When accessing a replicated domain or array from a locale *not* in the
set of target locales, an error is reported if bounds-checking is on;
undefined behavior occurs if it is off.

Otherwise, only the replicand on the current locale is accessed when
referring to the domain or array.

**Example**

  .. code-block:: chapel

    const Dbase = {1..5};  // A default-distributed domain
    const Drepl = Dbase dmapped Replicated();
    var Abase: [Dbase] int;
    var Arepl: [Drepl] int;

    // Only the current locale's replicand is accessed
    Arepl[3] = 4;

    // Only the current locale's replicand is accessed
    forall (b,r) in zip(Abase,Arepl) do b = r;
    Abase = Arepl;

    // Access other locales' replicand with the replicand(loc) method
    Arepl.replicand(Locales[1]) = Arepl;

See the :ref:`primers-replicated` primer for more examples of the Replicated
distribution.

**Initializer Arguments**

The ``Replicated`` class initializer is defined as follows:

  .. code-block:: chapel

    proc Replicated.init(
      targetLocales: [] locale = Locales,
      purposeMessage: string = "used to create a Replicated")

The optional ``purposeMessage`` may be useful for debugging
when the initializer encounters an error.


**Limitations**

* Only rectangular domains are presently supported.

*/
class Replicated : BaseDist {
  var targetLocDom : domain(here.id.type);

  // the desired locales (an array of locales)
  const targetLocales : [targetLocDom] locale;
}


// initializer: replicate over the given locales
// (by default, over all locales)
proc Replicated.init(targetLocales: [] locale = Locales,
                     purposeMessage: string = "used to create a Replicated")
{
  this.complete();

  for loc in targetLocales {
    this.targetLocDom.add(loc.id);
    this.targetLocales[loc.id] = loc;
  }

  if traceReplicatedDist then
    writeln("Replicated initializer over ", targetLocales);
}

proc Replicated.dsiEqualDMaps(that: Replicated(?)) {
  return this.targetLocales.equals(that.targetLocales);
}

proc Replicated.dsiEqualDMaps(that) param {
  return false;
}

override proc Replicated.dsiDestroyDist() {
  // no action necessary here
}

// privatization

override proc Replicated.dsiSupportsPrivatization() param return true;

proc Replicated.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("Replicated.dsiGetPrivatizeData");

  // TODO: Returning 'targetLocales' here results in a memory leak. Why?
  // Other distributions seem to do this 'return 0' as well...
  return 0;
}

proc Replicated.dsiPrivatize(privatizeData)
{
  if traceReplicatedDist then writeln("Replicated.dsiPrivatize on ", here);

  const otherTargetLocales = this.targetLocales;

  // make private copy of targetLocales and its domain
  const privDom = otherTargetLocales.domain;
  const privTargetLocales: [privDom] locale = otherTargetLocales;
 
  const nonNilWrapper: [0..#privTargetLocales.size] locale =
    for loc in otherTargetLocales do loc; 

  return new unmanaged Replicated(nonNilWrapper, "used during privatization");
}


/////////////////////////////////////////////////////////////////////////////
// domains

//
// global domain class
//
class ReplicatedDom : BaseRectangularDom {
  // we need to be able to provide the domain map for our domain - to build its
  // runtime type (because the domain map is part of the type - for any domain)
  // (looks like it must be called exactly 'dist')
  const dist : unmanaged Replicated; // must be a Replicated

  // this is our index set; we store it here so we can get to it easily
  var domRep: domain(rank, idxType, stridable);

  // local domain objects
  // NOTE: if they ever change after the initializer - Reprivatize them
  var localDoms: [dist.targetLocDom] unmanaged LocReplicatedDom(rank, idxType, stridable)?;

  proc numReplicands return localDoms.size;

  //
  // helper function to get the local domain safely
  //
  pragma "no doc"
  proc chpl_myLocDom() {
    if boundsChecking then
      if (!dist.targetLocDom.contains(here.id)) then
        halt("locale ", here.id, " has no local replicand");
    // Force unwrap this reference for ease of use.
    return localDoms[here.id]!;
  }

}

//
// local domain class
//
class LocReplicatedDom {
  // copy from the global domain
  param rank: int;
  type idxType;
  param stridable: bool;

  // our index set, copied from the global domain
  var domLocalRep: domain(rank, idxType, stridable);
}


// No explicit ReplicatedDom initializer - use the default one.
// proc ReplicatedDom.ReplicatedDom(...){...}

// Since we piggy-back on (default-mapped) Chapel domains, we can redirect
// a few operations to those. This function returns a Chapel domain
// that's fastest to access from the current locale.
// With privatization this is in the privatized copy of the ReplicatedDom.
//
// Not a parentheses-less method because of a bug as of r18460
// (see generic-parenthesesless-3.chpl).
proc ReplicatedDom.redirectee(): domain(rank, idxType, stridable)
  return domRep;

// The same across all domain maps
override proc ReplicatedDom.dsiMyDist() return dist;


// privatization

override proc ReplicatedDom.dsiSupportsPrivatization() param return true;

record ReplicatedDomPrvData {
  var distpid;
  var domRep;
  var localDoms;
}

proc ReplicatedDom.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiGetPrivatizeData");

  return new ReplicatedDomPrvData(dist.pid, domRep, localDoms);
}

proc ReplicatedDom.dsiPrivatize(privatizeData) {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiPrivatize on ", here);

  var privdist = chpl_getPrivatizedCopy(this.dist.type, privatizeData.distpid);
  return new unmanaged ReplicatedDom(rank=rank, idxType=idxType, stridable=stridable,
                           dist = privdist,
                           domRep = privatizeData.domRep,
                           localDoms = privatizeData.localDoms);
}

proc ReplicatedDom.dsiGetReprivatizeData() {
  return domRep;
}

proc ReplicatedDom.dsiReprivatize(other, reprivatizeData): void {
  assert(this.rank == other.rank &&
         this.idxType == other.idxType &&
         this.stridable == other.stridable);

  this.domRep = reprivatizeData;
}


proc Replicated.dsiClone(): _to_unmanaged(this.type) {
  if traceReplicatedDist then writeln("Replicated.dsiClone");
  var nonNilWrapper: [0..#targetLocales.size] locale =
    for loc in targetLocales do loc;
  return new unmanaged Replicated(nonNilWrapper);
}

// create a new domain mapped with this distribution
override proc Replicated.dsiNewRectangularDom(param rank: int,
                                         type idxType,
                                         param stridable: bool,
                                         inds)
{
  if traceReplicatedDist then writeln("Replicated.dsiNewRectangularDom ",
                                      (rank, idxType:string, stridable, inds));

  // Have to call the default initializer because we need to initialize 'dist'
  // prior to initializing 'localDoms' (which needs a non-nil value for 'dist'.
  var result = new unmanaged ReplicatedDom(rank=rank, idxType=idxType,
                                 stridable=stridable, dist=_to_unmanaged(this));

  // create local domain objects
  coforall (loc, locDom) in zip(targetLocales, result.localDoms) do
    on loc do
      locDom = new unmanaged LocReplicatedDom(rank, idxType, stridable);
  result.dsiSetIndices(inds);

  return result;
}

// Given an index, this should return the locale that owns that index.
// (This is the implementation of dmap.idxToLocale().)
// For Replicated, we point it to the current locale.
proc Replicated.dsiIndexToLocale(indexx): locale {
  return here;
}

// Call 'setIndices' in order to leverage DefaultRectangular's handling of
// assignments from unstrided domains to strided domains.
proc ReplicatedDom.dsiSetIndices(x) where isTuple(x) && isRange(x(0)) {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on ", x.type:string, ": ", x);
  dsiSetIndices({(...x)});
}

proc ReplicatedDom.dsiSetIndices(domArg: domain): void {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on domain ", domArg);
  domRep = domArg;
  coforall locDom in localDoms do
    on locDom do
      locDom!.domLocalRep = domArg;
}

proc ReplicatedDom.dsiGetIndices(): rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                 stridable) {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiGetIndices");
  return redirectee().getIndices();
}

// Iterators over the domain's indices (serial, leader, follower).
// Our semantics: yield each of the local domain's indices.

// Serial iterator
iter ReplicatedDom.these() {
  var dom = redirectee();
  for i in dom do
    yield i;
}

iter ReplicatedDom.these(param tag: iterKind) where tag == iterKind.leader {
  // redirect to DefaultRectangular's leader
  for follow in chpl_myLocDom().domLocalRep.these(tag) do
    yield follow;
}

iter ReplicatedDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  // redirect to DefaultRectangular
  for i in redirectee().these(tag, followThis) do
    yield i;
}

/* Write the domain out to the given Writer serially. */
proc ReplicatedDom.dsiSerialWrite(f): void {
  // redirect to DefaultRectangular
  redirectee()._value.dsiSerialWrite(f);
}

proc ReplicatedDom.dsiDims(): rank * range(idxType,
                                           BoundedRangeType.bounded,
                                           stridable)
  return redirectee().dims();

proc ReplicatedDom.dsiDim(dim: int): range(idxType,
                                           BoundedRangeType.bounded,
                                           stridable)
  return redirectee().dim(dim);

proc ReplicatedDom.dsiLow
  return redirectee().low;

proc ReplicatedDom.dsiHigh
  return redirectee().high;

proc ReplicatedDom.dsiStride
  return redirectee().stride;

proc ReplicatedDom.dsiAlignedLow
  return redirectee().alignedLow;

proc ReplicatedDom.dsiAlignedHigh
  return redirectee().alignedHigh;

proc ReplicatedDom.dsiAlignment
  return redirectee().alignment;

// here replication is visible
proc ReplicatedDom.dsiNumIndices
  return redirectee().size;

proc ReplicatedDom.dsiMember(indexx)
  return redirectee().contains(indexx);

proc ReplicatedDom.dsiIndexOrder(indexx)
  return redirectee().dsiIndexOrder(indexx);

override proc ReplicatedDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom {
    on dist.targetLocales(localeIdx) do
      delete localDoms(localeIdx);
  }
}

proc ReplicatedDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  // Don't do anything for the arrays (no dsiReallocate/dsiPostReallocate)
  this.dsiSetIndices(rhs.getIndices());
}

/////////////////////////////////////////////////////////////////////////////
// arrays

//
// global array class
//
class ReplicatedArr : AbsBaseArr {
  // These two are hard-coded in the compiler - it computes the array's
  // type string as '[dom.type] eltType.type'
  const dom; // must be a ReplicatedDom

  // the replicated arrays
  // NOTE: 'dom' must be initialized prior to initializing 'localArrs'
  var localArrs: [dom.dist.targetLocDom]
              unmanaged LocReplicatedArr(eltType, dom.rank, dom.idxType, dom.stridable)?;

  //
  // helper function to get the local array safely
  //
  pragma "no doc"
  proc chpl_myLocArr() {
    if boundsChecking then
      if (!dom.dist.targetLocDom.contains(here.id)) then
        halt("locale ", here.id, " has no local replicand");
    // Force unwrap this reference for ease of use.
    return localArrs[here.id]!;
  }

  //
  // Access another locale's local array representation
  //
  proc replicand(loc: locale) ref {
    return localArrs[loc.id]!.arrLocalRep;
  }
}

pragma "no doc"
// TODO: Placeholder until we can do forwarding on _value fields
proc _array.replicand(loc: locale) ref {
  return _value.replicand(loc);
}

//
// local array class
//
class LocReplicatedArr {
  // these generic fields let us give types to the other fields easily
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  var myDom: unmanaged LocReplicatedDom(rank, idxType, stridable);
  pragma "local field" pragma "unsafe" pragma "no auto destroy"
  // may be re-initialized separately
  // always destroyed explicitly (to control deiniting elts)
  var arrLocalRep: [myDom.domLocalRep] eltType;

  proc init(type eltType,
            param rank: int,
            type idxType,
            param stridable: bool,
            myDom: unmanaged LocReplicatedDom(rank, idxType, stridable),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.stridable = stridable;
    this.myDom = myDom;
    // always initialize the elements because in an initialization
    // context, we won't know which replicand is initialized,
    // because the RHS could be an arbitrary forall or for expression.
    this.arrLocalRep = this.myDom.domLocalRep.buildArray(eltType,
                                                         initElts=true);

    if initElts == false && !isPODType(eltType) {

      compilerError("ReplicatedDist array initialization is not currently supported for element type " + eltType:string + " - please default-initialize the array");
    }
  }

  proc deinit() {
    _do_destroy_array(arrLocalRep, deinitElts=true);
  }

  // guard against dynamic dispatch resolution trying to resolve
  // write()ing out an array of sync vars and hitting the sync var
  // type's compilerError()
  override proc writeThis(f) throws {
    halt("LocReplicatedArr.writeThis() is not implemented / should not be needed");
  }
}


// ReplicatedArr initializer.
// We create our own to make field initializations convenient:
// 'eltType' and 'dom' as passed explicitly;
// the fields in the parent class, BaseArr, are initialized to their defaults.
//
proc ReplicatedArr.init(type eltType, dom) {
  super.init(eltType = eltType);
  this.dom = dom;
}

proc ReplicatedArr.stridable param {
  return dom.stridable;
}

proc ReplicatedArr.idxType type {
  return dom.idxType;
}

proc ReplicatedArr.rank param {
  return dom.rank;
}

// The same across all domain maps
override proc ReplicatedArr.dsiGetBaseDom() return dom;


// privatization

override proc ReplicatedArr.dsiSupportsPrivatization() param return true;

record ReplicatedArrPrvData {
  var dompid;
  var localArrs;
}

proc ReplicatedArr.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiGetPrivatizeData");

  return new ReplicatedArrPrvData(dom.pid, localArrs);
}

proc ReplicatedArr.dsiPrivatize(privatizeData) {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiPrivatize on ", here);

  var privdom = chpl_getPrivatizedCopy(this.dom.type, privatizeData.dompid);
  var result = new unmanaged ReplicatedArr(eltType, privdom);
  result.localArrs = privatizeData.localArrs;
  return result;
}


// create a new array over this domain
proc ReplicatedDom.dsiBuildArray(type eltType, param initElts:bool)
  : unmanaged ReplicatedArr(eltType, _to_unmanaged(this.type))
{
  if traceReplicatedDist then writeln("ReplicatedDom.dsiBuildArray");
  var result = new unmanaged ReplicatedArr(eltType, _to_unmanaged(this));
  coforall (loc, locDom, locArr)
   in zip(dist.targetLocales, localDoms, result.localArrs) do
    on loc do
      locArr = new unmanaged LocReplicatedArr(eltType, rank, idxType, stridable,
                                              locDom!, initElts=initElts);
  return result;
}

// Return the array element corresponding to the index - on the current locale
proc ReplicatedArr.dsiAccess(indexx) ref {
  return chpl_myLocArr().arrLocalRep[indexx];
}

proc ReplicatedArr.dsiBoundsCheck(indexx) {
  return dom.dist.targetLocDom.contains(here.id);
}

// Write the array out to the given Writer serially.
proc ReplicatedArr.dsiSerialWrite(f): void {
  localArrs[f.readWriteThisFromLocale().id]!.arrLocalRep._value.dsiSerialWrite(f);
}

proc ReplicatedArr.dsiSerialRead(f, loc): void {
  localArrs[f.readWriteThisFromLocale().id]!.arrLocalRep._value.dsiSerialRead(f);
}

proc isReplicatedArr(arr) param {
  return isSubtype(_to_borrowed(chpl__getActualArray(arr)).type, ReplicatedArr);
}

proc chpl_serialReadWriteRectangular(f, arr, dom) where isReplicatedArr(arr) {
  const origloc = f.readWriteThisFromLocale();
  on origloc do
    chpl_serialReadWriteRectangularHelper(f, arr, dom);
}

override proc ReplicatedArr.dsiElementInitializationComplete() {
}

override proc ReplicatedArr.dsiElementDeinitializationComplete() {
}

override proc ReplicatedArr.dsiDestroyArr(deinitElts:bool) {
  coforall (loc, locArr) in zip(dom.dist.targetLocales, localArrs) {
    on loc {
      delete locArr;
    }
  }
}

// iterators

// completely serial
iter ReplicatedArr.these() ref: eltType {
  for a in chpl_myLocArr().arrLocalRep do
    yield a;
}

iter ReplicatedArr.these(param tag: iterKind) where tag == iterKind.leader {
  // redirect to ReplicatedDom's leader
  for follow in dom.these(tag) do
    yield follow;
}

iter ReplicatedArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  // redirect to DefaultRectangular
  for a in chpl_myLocArr().arrLocalRep.these(tag, followThis) do
    yield a;
}


/////////////////////////////////////////////////////////////////////////////
// reallocation

// This supports reassignment of the array's domain.
/*
This gets invoked upon reassignment of the array's domain,
prior to calling this.dom.dsiSetIndices().
So this needs to adjust anything in the array that won't be taken care of
in this.dom.dsiSetIndices(). In our case, that's nothing.
*/
/* no longer called
proc ReplicatedArr.dsiReallocate(d: domain): void {
  if traceReplicatedDist then
    writeln("ReplicatedArr.dsiReallocate ", dom.domRep, " -> ", d, " (no-op)");
}
*/

// Note: returns an associative array
proc Replicated.dsiTargetLocales() {
  return targetLocales;
}
proc ReplicatedDom.dsiTargetLocales() {
  return dist.targetLocales;
}
proc ReplicatedArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc ReplicatedDom.dsiHasSingleLocalSubdomain() param  return true;
proc ReplicatedArr.dsiHasSingleLocalSubdomain() param  return true;

proc ReplicatedDom.dsiLocalSubdomain(loc: locale) {
  if localDoms.domain.contains(loc.id) then
    return domRep;
  else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc ReplicatedArr.dsiLocalSubdomain(loc: locale) {
  if localArrs.domain.contains(loc.id) then
    return dom.domRep;
  else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc ReplicatedArr.dsiLocalSlice(ranges) {
  return chpl_myLocArr().arrLocalRep((...ranges));
}
