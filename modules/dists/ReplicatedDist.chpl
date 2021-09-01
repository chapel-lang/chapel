/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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


use ChapelDebugPrint;

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
  const targetLocDom : domain(1);
  const targetLocales : [targetLocDom] locale;

  const localeIdToIndexDom: domain(int);
  const localeIdToIndexArr: [localeIdToIndexDom] int;
}


// initializer: replicate over the given locales
// (by default, over all locales)
proc Replicated.init(const targetLocales: [] locale = Locales,
                     purposeMessage: string = "used to create a Replicated")
{
  // bulk copy targetLocales in the event that it is remote
  const locTargetLocDom = targetLocales.domain;
  const locTargetLocalesArr:[locTargetLocDom] locale = targetLocales;

  // remove duplicates in targetLocales in a way that arranges for
  // the last entry with a given id to "win".
  var locTargetLocalesDomAssoc : domain(here.id.type);
  var localeIdToIndexDom: domain(int);
  for loc in locTargetLocalesArr {
    locTargetLocalesDomAssoc += loc.id;
    localeIdToIndexDom += loc.id;
  }
  var locTargetLocalesAssoc: [locTargetLocalesDomAssoc] locale;
  var localeIdToIndexArr: [localeIdToIndexDom] int;
  for loc in locTargetLocalesArr {
    locTargetLocalesAssoc[loc.id] = loc;
  }

  this.targetLocDom = {0..#locTargetLocDom.size};
  // Now create a local DR array for the target locales.
  var locTargetLocales: [this.targetLocDom] locale;
  var idx = 0;
  for loc in locTargetLocalesArr {
    // consider only the unique locales
    if locTargetLocalesAssoc[loc.id] == loc {
      locTargetLocales[idx] = loc;
      localeIdToIndexArr[loc.id] = idx;
      idx += 1;
    }
  }

  this.targetLocales = locTargetLocales;
  this.complete();

  if traceReplicatedDist then
    chpl_debug_writeln("Created Replicated over ", targetLocales.size);
}

proc Replicated.indexForLocale(whichloc: locale) {
  if !localeIdToIndexDom.contains(whichloc.id) then
    return -1;

  return localeIdToIndexArr[whichloc.id];
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
  if traceReplicatedDist then chpl_debug_writeln("Replicated.dsiGetPrivatizeData");

  // TODO: Returning 'targetLocales' here results in a memory leak. Why?
  // Other distributions seem to do this 'return 0' as well...
  return 0;
}

proc Replicated.dsiPrivatize(privatizeData)
{
  if traceReplicatedDist then
    chpl_debug_writeln("\nReplicated.dsiPrivatize on ", here.id,
                       " from ", this.locale.id);

  return new unmanaged Replicated(this.targetLocales,
                                  "used during privatization");
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

  pragma "local field"
  var myLocDom: unmanaged LocReplicatedDom(rank, idxType, stridable)?;

  proc numReplicands return localDoms.sizeAs(int);

  //
  // helper function to get the local domain safely
  //
  pragma "no doc"
  proc chpl_myLocDom() {
    if boundsChecking then
      if myLocDom == nil then
        halt("locale ", here.id, " has no local replicand");
    // Force unwrap this reference for ease of use.
    return myLocDom!;
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
  const distpid;
  const domRep;
  const localDoms;
}

proc ReplicatedDom.dsiGetPrivatizeData() {
  if traceReplicatedDist then chpl_debug_writeln("ReplicatedDom.dsiGetPrivatizeData");

  return new ReplicatedDomPrvData(dist.pid, domRep, localDoms);
}

proc ReplicatedDom.dsiPrivatize(privatizeData) {
  if traceReplicatedDist then chpl_debug_writeln("ReplicatedDom.dsiPrivatize on ", here);

  var privdist = chpl_getPrivatizedCopy(this.dist.type, privatizeData.distpid);
  var ret = new unmanaged ReplicatedDom(
                           rank=rank, idxType=idxType, stridable=stridable,
                           dist = privdist,
                           domRep = privatizeData.domRep,
                           localDoms = privatizeData.localDoms);

  for (localDom, loc) in zip(ret.localDoms, ret.dist.targetLocales) {
    if loc.id == here.id {
      ret.myLocDom = localDom;
    }
  }

  return ret;
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
  if traceReplicatedDist then chpl_debug_writeln("Replicated.dsiClone");
  var nonNilWrapper: [0..#targetLocales.sizeAs(int)] locale =
    for loc in targetLocales do loc;
  return new unmanaged Replicated(targetLocales);
}

// create a new domain mapped with this distribution
override proc Replicated.dsiNewRectangularDom(param rank: int,
                                         type idxType,
                                         param stridable: bool,
                                         inds)
{
  if traceReplicatedDist then chpl_debug_writeln("Replicated.dsiNewRectangularDom ",
                                      (rank, idxType:string, stridable, inds));

  // Have to call the default initializer because we need to initialize 'dist'
  // prior to initializing 'localDoms' (which needs a non-nil value for 'dist'.
  var result = new unmanaged ReplicatedDom(rank=rank, idxType=idxType,
                                 stridable=stridable, dist=_to_unmanaged(this));

  const creationLocale = here.id;
  // create local domain objects
  coforall (loc, locDom) in zip(targetLocales, result.localDoms) {
    on loc {
      const LD = new unmanaged LocReplicatedDom(rank, idxType, stridable);
      locDom = LD;
      if here.id == creationLocale then
        result.myLocDom = LD;
    }
  }

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
    chpl_debug_writeln("ReplicatedDom.dsiSetIndices on ", x.type:string, ": ", x);
  dsiSetIndices({(...x)});
}

proc ReplicatedDom.dsiSetIndices(domArg: domain): void {
  if traceReplicatedDist then
    chpl_debug_writeln("ReplicatedDom.dsiSetIndices on domain ", domArg);
  domRep = domArg;
  coforall locDom in localDoms do
    on locDom do
      locDom!.domLocalRep = domArg;
}

proc ReplicatedDom.dsiGetIndices(): rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                 stridable) {
  if traceReplicatedDist then chpl_debug_writeln("ReplicatedDom.dsiGetIndices");
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

proc ReplicatedDom.doiToString() {
  return redirectee()._value.doiToString();
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
  return redirectee().sizeAs(uint);

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

  pragma "local field"
  var myLocArr: unmanaged LocReplicatedArr(eltType, dom.rank, dom.idxType, dom.stridable)?;

  //
  // helper function to get the local array safely
  //
  pragma "no doc"
  proc chpl_myLocArr() {
    if boundsChecking then
      if myLocArr == nil then
        halt("locale ", here.id, " has no local replicand");
    // Force unwrap this reference for ease of use.
    return myLocArr!;
  }

  //
  // Access another locale's local array representation
  //
  proc replicand(loc: locale) ref {
    if loc.id == here.id then
      return chpl_myLocArr();

    const idx = dom.dist.indexForLocale(loc);
    if boundsChecking then
      if !dom.dist.targetLocDom.contains(idx) then
        halt("locale ", loc.id, " has no local replicand");

    return localArrs[idx]!.arrLocalRep;
  }

  // scan the local locale's replicand
  proc doiScan(op, dom) where ((rank == 1) &&
                               chpl__scanStateResTypesMatch(op)) {
    return chpl_myLocArr().arrLocalRep._instance.doiScan(op, dom);
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

    //
    // Trust that the caller will select the right `initElts` value for this
    // particular replicand.
    //
    this.arrLocalRep = this.myDom.domLocalRep
          .buildArray(eltType, initElts);
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
  if traceReplicatedDist then chpl_debug_writeln("ReplicatedArr.dsiGetPrivatizeData");

  return new ReplicatedArrPrvData(dom.pid, localArrs);
}

proc ReplicatedArr.dsiPrivatize(privatizeData) {
  if traceReplicatedDist then chpl_debug_writeln("ReplicatedArr.dsiPrivatize on ", here);

  var privdom = chpl_getPrivatizedCopy(this.dom.type, privatizeData.dompid);
  var ret = new unmanaged ReplicatedArr(eltType, privdom);

  ret.localArrs = privatizeData.localArrs;

  for (localArr, loc) in zip(ret.localArrs, ret.dom.dist.targetLocales) {
    if loc.id == here.id {
      ret.myLocArr = localArr;
    }
  }

  return ret;
}


// create a new array over this domain
proc ReplicatedDom.dsiBuildArray(type eltType, param initElts:bool)
      : unmanaged ReplicatedArr(eltType, _to_unmanaged(this.type)) {

  // In order to support this, we would have to make copy-initialization
  // for replicated arrays initialize all replicands.
  if !isDefaultInitializable(eltType) then
    compilerError('cannot initialize replicated array because element ',
                  'type ', eltType:string, ' cannot be copied');

  if traceReplicatedDist then chpl_debug_writeln("ReplicatedDom.dsiBuildArray");

  const creationLocale = here.id;

  var result = new unmanaged ReplicatedArr(eltType, _to_unmanaged(this));

  // The locale where the 'dsiBuildArray' call originally takes place. We
  // need to save this so that we can decide which replicand to build with
  // `initElts=false` in the loop below.
  const globalArrayLocale = here;

  coforall (loc, locDom, locArr)
      in zip(dist.targetLocales, localDoms, result.localArrs) do on loc {

    const LA;

    //
    // When a replicated array is initialized with `initElts=false`, only
    // the replicand on the locale where the global array descriptor is
    // being built is also initialized with `initElts=false`. All the
    // other replicands are initialized with `initElts=true`, because they
    // will be default-initialized.
    //
    if here == globalArrayLocale && !initElts {
      LA = new unmanaged LocReplicatedArr(eltType, rank, idxType,
                                          stridable,
                                          locDom!,
                                          initElts=false);
    } else {
      LA = new unmanaged LocReplicatedArr(eltType, rank, idxType,
                                          stridable,
                                          locDom!,
                                          initElts=true);
    }

    locArr = LA;

    if here.id == creationLocale then
      result.myLocArr = LA;
  }

  return result;
}

// Return the array element corresponding to the index - on the current locale
proc ReplicatedArr.dsiAccess(indexx) ref {
  return chpl_myLocArr().arrLocalRep[indexx];
}

proc ReplicatedArr.dsiBoundsCheck(indexx) {
  for loc in dom.dist.targetLocales {
    if loc.id == here.id then
      return true;
  }
  return false;
}

// Write the array out to the given Writer serially.
proc ReplicatedArr.dsiSerialWrite(f): void {
  replicand(f.readWriteThisFromLocale())._value.dsiSerialWrite(f);
}

proc ReplicatedArr.dsiSerialRead(f, loc): void {
  replicand(f.readWriteThisFromLocale())._value.dsiSerialRead(f);
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

  // Replicated arrays are weird. If a replicated is created via a coerceCopy,
  // then it was initialized with `initElts=false`. For replicated, this
  // means that only the replicand on the same locale as the newly created
  // array will be initialized with `initElts=false`. For correct behavior,
  // we only call "complete" for the replicand on the locale we're currently
  // on (which should be the same locale as the newly created array).
  chpl_myLocArr().arrLocalRep.dsiElementInitializationComplete();
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
    chpl_debug_writeln("ReplicatedArr.dsiReallocate ", dom.domRep, " -> ", d, " (no-op)");
}
*/

// Note: returns an associative array
proc Replicated.dsiTargetLocales() const ref {
  return targetLocales;
}
proc ReplicatedDom.dsiTargetLocales() const ref {
  return dist.targetLocales;
}
proc ReplicatedArr.dsiTargetLocales() const ref {
  return dom.dist.targetLocales;
}

proc ReplicatedDom.dsiHasSingleLocalSubdomain() param  return true;
proc ReplicatedArr.dsiHasSingleLocalSubdomain() param  return true;

proc ReplicatedDom.dsiLocalSubdomain(loc: locale) {
  var idx = dist.indexForLocale(loc);
  if idx >= 0 then
    return domRep;
  else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc ReplicatedArr.dsiLocalSubdomain(loc: locale) {
  var idx = dom.dist.indexForLocale(loc);
  if idx >= 0 then
    return dom.domRep;
  else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc ReplicatedArr.dsiLocalSlice(ranges) {
  return chpl_myLocArr().arrLocalRep((...ranges));
}
