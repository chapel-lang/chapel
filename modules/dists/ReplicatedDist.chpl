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

/*****************************************************************************
*** THE REPLICATED DISTRIBUTION ***

This ReplicatedDist distribution causes a domain and its arrays
to be replicated across the desired locales (all the locales by default).
An array receives a distinct set of elements - a "replicand" -
allocated on each locale.

In other words, mapping a domain with ReplicatedDist gives it
an implicit additional dimension - over the locales,
making it behave as if there is one copy of its indices per locale.

Replication over locales is observable:
- when iterating over a domain or array
- when printing with write() et al.
- when zippering and the replicated domain/array is
  the first among the zippered items
- when assigning into the replicated array
- when inquiring about the domain's numIndices
  or the array's numElements
- when accessing array element(s) from a locale that was not included
  in the array passed explicitly to the ReplicatedDist constructor,
  an out-of-bounds error will result

Only the replicand *on the current locale* is accessed
(i.e. replication is not observable):
- when examining certain domain properties:
  dim(d), dims(), low, high, stride;
  but not numIndices
- when indexing into an array
- when slicing an array  TODO: right?
- when zippering and the first zippered item is not replicated
- when assigning to a non-replicated array,
  i.e. the replicated array is on the right-hand side of the assignment
- when there is only a single locale (trivially: only one replicand)

E.g. when iterating, the number of iterations will be (the number of
locales involved) times (the number of iterations over this domain if
it were distributed with the default distribution).

Note that the above behavior may change in the future.

Features/limitations:
* Consistency/coherence among replicands' array elements is NOT maintained.
* Only rectangular domains are presently supported.
* Serial iteration over a replicated domain (or array) visits the indices
  (or array elements) of all replicands *from the current locale*.
* When replicating over user-provided array of locales, that array
  must be "consistent" (see below).

"Consistent" array requirement:
* The array of locales passed to the ReplicatedDist constructor, if any,
  must be "consistent".
* A is "consistent" if for each ix in A.domain, A[ix].id == ix.
* Tip: if the domain of the desired array of locales cannot be described
  as a rectangular domain (which could be strided, multi-dimensional,
  and/or sparse), make that array's domain associative over int.

Examples:

const Dbase = {1..5};
const Drepl: domain(1) dmapped ReplicatedDist() = Dbase;
var Abase: [Dbase] int;
var Arepl: [Drepl] int;

// only the current locale's replicand is accessed
Arepl[3] = 4;

// these iterate over Dbase, so
// only the current locale's replicand is accessed
forall (b,r) in zip(Abase,Arepl) b = r;
Abase = Arepl;

// these iterate over Drepl;
// each replicand will be zippered against (and copied from) the entire Abase
forall (r,b) in zip(Arepl,Abase) r = b;
Arepl = Abase;

// sequential zippering will detect difference in sizes
// (if multiple locales)
for (b,r) in zip(Abase,Arepl) ... // error
for (r,b) in zip(Arepl,Abase) ... // error

Potential extensions:
- support other kinds of domains
- allow run-time change in locales
*/


/*****************************************************************************/
// THE REPLICATED DISTRIBUTION IMPLEMENTATION
//
// Classes defined:
//  ReplicatedDist -- Global distribution descriptor
//  ReplicatedDom -- Global domain descriptor
//  LocReplicatedDom -- Local domain descriptor
//  ReplicatedArray -- Global array descriptor
//  LocReplicatedArray -- Local array descriptor

// include locale information when printing out domains and arrays
config param printReplicatedLocales = true;

// trace certain DSI methods as they are being invoked
config param traceReplicatedDist = false;


/////////////////////////////////////////////////////////////////////////////
// distribution

//
// (global) distribution class
//
class ReplicatedDist : BaseDist {
  // the desired locales (an array of locales)
  const targetLocales;
  // "IDs" are indices into targetLocales
  proc targetIds return targetLocales.domain;

  // privatized object id
  var pid: int = -1;
}


// constructor: replicate over the given locales
// (by default, over all locales)
proc ReplicatedDist.ReplicatedDist(targetLocales: [] locale = Locales,
                 purposeMessage: string = "used to create a ReplicatedDist")
{
  if targetLocales.rank != 1 then
    compilerError("ReplicatedDist only accepts a 1D targetLocales array");
  if traceReplicatedDist then
    writeln("ReplicatedDist constructor over ", targetLocales);
  _localesCheckHelper(purposeMessage);
}

// helper to check consistency of the locales array
// TODO: going over all the locales - is there a scalability issue?
proc ReplicatedDist._localesCheckHelper(purposeMessage: string): void {
  // ideally would like to make this a "eureka"
  forall (ix, loc) in zip(targetIds, targetLocales) do
    if loc.id != ix {
      halt("The array of locales ", purposeMessage, " must be \"consistent\".",
           " See ReplicatedDist documentation for details.");
    }
}

proc ReplicatedDist.dsiEqualDMaps(that: ReplicatedDist(?)) {
  return this.targetLocales.equals(that.targetLocales);
}

proc ReplicatedDist.dsiEqualDMaps(that) param {
  return false;
}

// privatization

proc ReplicatedDist.dsiSupportsPrivatization() param return true;

proc ReplicatedDist.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("ReplicatedDist.dsiGetPrivatizeData");

  // TODO: return the targetLocales array by value,
  // to reduce communication needed in dsiPrivatize()
  // perhaps by wrapping it in a class (or tuple?).
  return targetLocales;
}

proc ReplicatedDist.dsiPrivatize(privatizeData: this.targetLocales.type)
  : this.type
{
  if traceReplicatedDist then writeln("ReplicatedDist.dsiPrivatize on ", here);

  const pdTargetLocales = privatizeData;
  // make private copy of targetLocales and its domain
  const privTargetIds: domain(pdTargetLocales.domain.rank,
                              pdTargetLocales.domain.idxType,
                              pdTargetLocales.domain.stridable
                              ) = pdTargetLocales.domain;
  const privTargetLocales: [privTargetIds] locale = pdTargetLocales;
  return new ReplicatedDist(privTargetLocales, "used during privatization");
}


/////////////////////////////////////////////////////////////////////////////
// domains

//
// global domain class
//
class ReplicatedDom : BaseRectangularDom {
  // to support rectangular domains
  param rank: int;
  type idxType;
  param stridable: bool;
  // we need to be able to provide the domain map for our domain - to build its
  // runtime type (because the domain map is part of the type - for any domain)
  // (looks like it must be called exactly 'dist')
  const dist; // must be a ReplicatedDist

  // this is our index set; we store it here so we can get to it easily
  var domRep: domain(rank, idxType, stridable);

  // local domain objects
  // NOTE: 'dist' must be initialized prior to 'localDoms'
  // => currently have to use the default constructor
  // NOTE: if they ever change after the constructor - Reprivatize them
  var localDoms: [dist.targetIds] LocReplicatedDom(rank, idxType, stridable);

  proc numReplicands return localDoms.numElements;

  // privatized object id
  var pid: int = -1;
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


// No explicit ReplicatedDom constructor - use the default one.
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
proc ReplicatedDom.dsiMyDist() return dist;


// privatization

proc ReplicatedDom.dsiSupportsPrivatization() param return true;

proc ReplicatedDom.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiGetPrivatizeData");

  // TODO: perhaps return 'domRep' and 'localDoms' by value,
  // to reduce communication needed in dsiPrivatize().
  return (dist.pid, domRep, localDoms);
}

proc ReplicatedDom.dsiPrivatize(privatizeData): this.type {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiPrivatize on ", here);

  var privdist = chpl_getPrivatizedCopy(this.dist.type, privatizeData(1));
  return new ReplicatedDom(rank=rank, idxType=idxType, stridable=stridable,
                           dist = privdist,
                           domRep = privatizeData(2),
                           localDoms = privatizeData(3));
}

proc ReplicatedDom.dsiGetReprivatizeData() {
  return (domRep,);
}

proc ReplicatedDom.dsiReprivatize(other, reprivatizeData): void {
  assert(this.rank == other.rank &&
         this.idxType == other.idxType &&
         this.stridable == other.stridable);

  this.domRep = reprivatizeData(1);
}


proc ReplicatedDist.dsiClone(): this.type {
  if traceReplicatedDist then writeln("ReplicatedDist.dsiClone");
  return new ReplicatedDist(targetLocales);
}

// create a new domain mapped with this distribution
proc ReplicatedDist.dsiNewRectangularDom(param rank: int,
                                         type idxType,
                                         param stridable: bool)
  : ReplicatedDom(rank, idxType, stridable, this.type)
{
  if traceReplicatedDist then writeln("ReplicatedDist.dsiNewRectangularDom ",
                                      (rank, typeToString(idxType), stridable));

  // Have to call the default constructor because we need to initialize 'dist'
  // prior to initializing 'localDoms' (which needs a non-nil value for 'dist'.
  var result = new ReplicatedDom(rank=rank, idxType=idxType,
                                 stridable=stridable, dist=this);

  // create local domain objects
  coforall (loc, locDom) in zip(targetLocales, result.localDoms) do
    on loc do
      locDom = new LocReplicatedDom(rank, idxType, stridable);

  return result;
}

// create a new domain mapped with this distribution representing 'ranges'
proc ReplicatedDom.dsiBuildRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool,
                                          ranges: rank * range(idxType,
                                                BoundedRangeType.bounded,
                                                               stridable))
  : ReplicatedDom(rank, idxType, stridable, this.dist.type)
{
  // could be made more efficient to avoid visiting each locale twice
  // but perhaps not a big deal, for now anyways
  var result = dist.dsiNewRectangularDom(rank, idxType, stridable);
  result.dsiSetIndices(ranges);
  return result;
}

// Given an index, this should return the locale that owns that index.
// (This is the implementation of dmap.idxToLocale().)
// For ReplicatedDist, we point it to the current locale.
proc ReplicatedDist.dsiIndexToLocale(indexx): locale {
  return here;
}

/*
dsiSetIndices accepts ranges because it is invoked so from ChapelArray or so.
Most dsiSetIndices() on a tuple of ranges can be the same as this one.
Or that call dsiSetIndices(ranges) could be converted following this example.
*/
proc ReplicatedDom.dsiSetIndices(rangesArg: rank * range(idxType,
                                          BoundedRangeType.bounded,
                                                         stridable)): void {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on ranges");
  dsiSetIndices({(...rangesArg)});
}

proc ReplicatedDom.dsiSetIndices(domArg: domain(rank, idxType, stridable)): void {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on domain ", domArg);
  domRep = domArg;
  coforall locDom in localDoms do
    on locDom do
      locDom.domLocalRep = domArg;
}

proc ReplicatedDom.dsiGetIndices(): rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                 stridable) {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiGetIndices");
  return redirectee().getIndices();
}

// Iterators over the domain's indices (serial, leader, follower).
// Our semantics: yield each of the domain's indices once per each locale.

// Serial iterator: the compiler forces it to be completely serial
iter ReplicatedDom.these() {
  // compiler does not allow 'on' here (see r16137 and nestedForall*)
  // so instead of ...
  //---
  //for locDom in localDoms do
  //  on locDom do
  //    for i in locDom.domLocalRep do
  //      yield i;
  //---
  // ... so we simply do the same a few times
  var dom = redirectee();
  for count in 1..#numReplicands do
    for i in dom do
      yield i;
}

iter ReplicatedDom.these(param tag: iterKind) where tag == iterKind.leader {
  coforall locDom in localDoms do
    on locDom do
      // there, for simplicity, redirect to DefaultRectangular's leader
      for follow in locDom.domLocalRep._value.these(tag) do
        yield follow;
}

iter ReplicatedDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  // redirect to DefaultRectangular
  for i in redirectee()._value.these(tag, followThis) do
    yield i;
}

/* Write the domain out to the given Writer serially. */
proc ReplicatedDom.dsiSerialWrite(f: Writer): void {
  // redirect to DefaultRectangular
  redirectee()._value.dsiSerialWrite(f);
  if printReplicatedLocales {
    f.write(" replicated over ");
    dist.targetLocales._value.dsiSerialWrite(f);
  }
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

// here replication is visible
proc ReplicatedDom.dsiNumIndices
  return redirectee().numIndices * numReplicands;

proc ReplicatedDom.dsiMember(indexx)
  return redirectee().member(indexx);

proc ReplicatedDom.dsiIndexOrder(indexx)
  return redirectee().dsiIndexOrder(indexx);


/////////////////////////////////////////////////////////////////////////////
// arrays

//
// global array class
//
class ReplicatedArr : BaseArr {
  // These two are hard-coded in the compiler - it computes the array's
  // type string as '[dom.type] eltType.type'
  type eltType;
  const dom; // must be a ReplicatedDom

  // the replicated arrays
  // NOTE: 'dom' must be initialized prior to initializing 'localArrs'
  var localArrs: [dom.dist.targetIds]
              LocReplicatedArr(eltType, dom.rank, dom.idxType, dom.stridable);

  // privatized object id
  var pid: int = -1;
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

  var myDom: LocReplicatedDom(rank, idxType, stridable);
  var arrLocalRep: [myDom.domLocalRep] eltType;
}


// ReplicatedArr constructor.
// We create our own to make field initializations convenient:
// 'eltType' and 'dom' as passed explicitly;
// the fields in the parent class, BaseArr, are initialized to their defaults.
//
proc ReplicatedArr.ReplicatedArr(type eltType, dom: ReplicatedDom) {
  // initializes the fields 'eltType', 'dom' by name
}

// The same across all domain maps
proc ReplicatedArr.dsiGetBaseDom() return dom;


// privatization

proc ReplicatedArr.dsiSupportsPrivatization() param return true;

proc ReplicatedArr.dsiGetPrivatizeData() {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiGetPrivatizeData");

  // TODO: perhaps return 'localArrs' by value,
  // to reduce communication needed in dsiPrivatize().
  return (dom.pid, localArrs);
}

proc ReplicatedArr.dsiPrivatize(privatizeData) {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiPrivatize on ", here);

  var privdom = chpl_getPrivatizedCopy(this.dom.type, privatizeData(1));
  var result = new ReplicatedArr(eltType, privdom);
  result.localArrs = privatizeData(2);
  return result;
}


// create a new array over this domain
proc ReplicatedDom.dsiBuildArray(type eltType)
  : ReplicatedArr(eltType, this.type)
{
  if traceReplicatedDist then writeln("ReplicatedDom.dsiBuildArray");
  var result = new ReplicatedArr(eltType, this);
  coforall (loc, locDom, locArr)
   in zip(dist.targetLocales, localDoms, result.localArrs) do
    on loc do
      locArr = new LocReplicatedArr(eltType, rank, idxType, stridable,
                                    locDom);
  return result;
}

// Return the array element corresponding to the index - on the current locale
proc ReplicatedArr.dsiAccess(indexx) ref: eltType {
  return localArrs[here.id].arrLocalRep[indexx];
}

// Write the array out to the given Writer serially.
proc ReplicatedArr.dsiSerialWrite(f: Writer): void {
  var neednl = false;
  for locArr in localArrs {
//  on locArr {  // may cause deadlock
      if neednl then f.write("\n"); neednl = true;
      if printReplicatedLocales then
        f.write(locArr.locale, ":\n");
      locArr.arrLocalRep._value.dsiSerialWrite(f);
//  }
  }
}

// iterators

// completely serial
iter ReplicatedArr.these() ref: eltType {
  for locArr in localArrs do
//  on locArr do // compiler does not allow; see r16137 and nestedForall*
      for a in locArr.arrLocalRep do
        yield a;
}

iter ReplicatedArr.these(param tag: iterKind) where tag == iterKind.leader {
  // redirect to ReplicatedDom's leader
  for follow in dom.these(tag) do
    yield follow;
}

iter ReplicatedArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  // redirect to DefaultRectangular
  for a in localArrs[here.id].arrLocalRep._value.these(tag, followThis) do
    yield a;
}


/////////////////////////////////////////////////////////////////////////////
// slicing, reindexing, etc.

// This supports reassignment of the array's domain.
/*
This gets invoked upon reassignment of the array's domain,
prior to calling this.dom.dsiSetIndices().
So this needs to adjust anything in the array that won't be taken care of
in this.dom.dsiSetIndices(). In our case, that's nothing.
*/
proc ReplicatedArr.dsiReallocate(d: domain): void {
  if traceReplicatedDist then
    writeln("ReplicatedArr.dsiReallocate ", dom.domRep, " -> ", d, " (no-op)");
}

// array slicing
proc ReplicatedArr.dsiSlice(sliceDef: ReplicatedDom) {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiSlice on ", sliceDef);
  const slicee = this;
  const result = new ReplicatedArr(slicee.eltType, sliceDef);

  // ensure sliceDef and slicee are over the same set of locales/targetIds
  assert(sliceDef.localDoms.domain == slicee.localArrs.domain);

  coforall (loc, sliceDefLocDom, sliceeLocArr, resultLocArr)
   in zip(sliceDef.dist.targetLocales, sliceDef.localDoms,
       slicee.localArrs, result.localArrs) do
    on loc do
      resultLocArr = new LocReplicatedArr(eltType,
        sliceDef.rank, sliceDef.idxType, sliceDef.stridable,
        myDom = sliceDefLocDom,
        arrLocalRep => sliceeLocArr.arrLocalRep[sliceDefLocDom.domLocalRep]);

  return result;
}

// array reindexing
// very similar to array slicing
proc ReplicatedArr.dsiReindex(sliceDef: ReplicatedDom) {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiReindex on ", sliceDef);
  var result = new ReplicatedArr(eltType, sliceDef);
  var slicee = this;

  // ensure 'dom' and 'slicee' are over the same set of locales/targetIds
  assert(sliceDef.localDoms.domain == slicee.localArrs.domain);

  coforall (loc, sliceDefLocDom, sliceeLocArr, resultLocArr)
   in zip(sliceDef.dist.targetLocales, sliceDef.localDoms,
       slicee.localArrs, result.localArrs) do
    on loc do
     {
      var locAlias: [sliceDefLocDom.domLocalRep] => sliceeLocArr.arrLocalRep;
      resultLocArr = new LocReplicatedArr(eltType,
        sliceDef.rank, sliceDef.idxType, sliceDef.stridable,
        myDom = sliceDefLocDom,
        arrLocalRep => locAlias);
     }

  return result;
}

// rank-change slicing
// very similar to slicing
proc ReplicatedArr.dsiRankChange(sliceDef: ReplicatedDom,
                                 param newRank: int,
                                 param newStridable: bool,
                                 args) {
  if traceReplicatedDist then writeln("ReplicatedArr.dsiRankChange");
  var result = new ReplicatedArr(eltType, sliceDef);
  var slicee = this;

  // ensure 'dom' and 'slicee' are over the same set of locales/targetIds
  assert(sliceDef.localDoms.domain == slicee.localArrs.domain);

  coforall (loc, sliceDefLocDom, sliceeLocArr, resultLocArr)
   in zip(sliceDef.dist.targetLocales, sliceDef.localDoms,
       slicee.localArrs, result.localArrs) do
    on loc do
      resultLocArr = new LocReplicatedArr(eltType,
        sliceDef.rank, sliceDef.idxType, sliceDef.stridable,
        myDom = sliceDefLocDom,
        arrLocalRep => sliceeLocArr.arrLocalRep[(...args)]);

  return result;
}

proc ReplicatedArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc ReplicatedArr.dsiHasSingleLocalSubdomain() param  return true;

proc ReplicatedArr.dsiLocalSubdomain() {
  return localArrs[here.id].myDom.domLocalRep;
}

// todo? these two seem to work (written by analogy with DefaultRectangular)
proc ReplicatedDist.dsiCreateReindexDist(newSpace, oldSpace) return this;
proc ReplicatedDist.dsiCreateRankChangeDist(param newRank, args) return this;
