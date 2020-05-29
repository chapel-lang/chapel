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

//
// BlockCyclic Distribution
//
//      BlockCyclic    BlockCyclicDom     BlockCyclicArr
//
//   LocBlockCyclic    LocBlockCyclicDom  LocBlockCyclicArr
//

private use DSIUtil;

private use HaltWrappers;

//
// TODO List
//
// 1. Make multi-dimensional
// 2. Support strided domains of locales
// 3. Support strided Block domains
// 4. Support slices
//
// Limitations
//
// 1. Changes to Block.dataParTasksPerLocale are not made in privatized
//    copies of this distribution.
//


config param debugBlockCyclicDist = false; // internal development flag (debugging)

proc _determineRankFromArg(startIdx) param {
  return if isTuple(startIdx) then startIdx.size else 1;
}

proc _determineIdxTypeFromArg(startIdx) type {
  return if isTuple(startIdx) then startIdx(0).type else startIdx.type;
}

proc _ensureTuple(arg) {
  return if isTuple(arg) then arg else (arg,);
}


////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Distribution Class
//
/*

This Block-Cyclic distribution maps maps blocks of indices to locales in a
round-robin pattern according to a given block size and start index.

Formally, consider a Block-Cyclic distribution with:

  =============  ====================================================
  rank           ``d``
  start index    ``(s_1, ...., s_d)``
  block size     ``(b_1, ...., b_d)``
  over locales   ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
  =============  ====================================================

It maps an index ``(i_1, ...., i_d)``
to the locale ``targetLocales[j_1, ...., j_d]``
where, for each ``k`` in ``1..d``,
we have:

  ``j_k = (  (i_k - s_k) / b_k  ) (mod N_k)``


**Limitations**

This distribution is work in progress and so has significant limitations.

It has not been tuned for performance.

The only ``idxType`` currently supported is `int` or `int(64)`.


**Example**

The following code declares a domain ``D`` distributed over
a Block-Cyclic distribution with a start index of ``(1,1)``
and a block size of ``(2,3)``,
and declares an array ``A`` over that domain.
The `forall` loop sets each array element
to the ID of the locale to which it is mapped.

  .. code-block:: chapel

    use BlockCycDist;

    const Space = {1..8, 1..8};
    const D: domain(2)
      dmapped BlockCyclic(startIdx=Space.low,blocksize=(2,3))
      = Space;
    var A: [D] int;

    forall a in A do
      a = a.locale.id;

    writeln(A);

When run on 6 locales, the output is:

  ::

    0 0 0 1 1 1 0 0
    0 0 0 1 1 1 0 0
    2 2 2 3 3 3 2 2
    2 2 2 3 3 3 2 2
    4 4 4 5 5 5 4 4
    4 4 4 5 5 5 4 4
    0 0 0 1 1 1 0 0
    0 0 0 1 1 1 0 0


**Initializer Arguments**

The ``BlockCyclic`` class initializer is defined as follows:

  .. code-block:: chapel

    proc BlockCyclic.init(
      startIdx,
      blocksize,
      targetLocales: [] locale = Locales,
      dataParTasksPerLocale    = // value of dataParTasksPerLocale config const
      param rank: int          = // inferred from startIdx argument,
      type idxType             = // inferred from startIdx argument )

The argument ``startIdx`` is a tuple of integers defining an index
that will be distributed to the first locale in ``targetLocales``.
For a single dimensional distribution ``startIdx`` can be an integer
or a tuple with a single element.

The argument ``blocksize`` is a tuple of integers defining the block
size of indices that will be used in dealing out indices to the
locales. For a single dimensional distribution ``blocksize`` can be an
integer or a tuple with a single element.

The argument ``targetLocales`` is an array containing the target
locales to which this distribution maps indices and data.  The rank of
``targetLocales`` must match the rank of the distribution, or be one.
If the rank of ``targetLocales`` is one, a greedy heuristic is used to
reshape the array of target locales so that it matches the rank of the
distribution and each dimension contains an approximately equal number
of indices.

The argument ``dataParTasksPerLocale`` determines the maximum number
of tasks on each Locale for data parallelism.

The ``rank`` and ``idxType`` arguments are inferred from the
``startIdx`` argument unless explicitly set.
They must match the rank and index type of the domains
"dmapped" using that BlockCyclic instance.


**Data-Parallel Iteration**

A `forall` loop over a Cyclic-distributed domain or array
executes each iteration on the locale where that iteration's index
is mapped to.

*/
class BlockCyclic : BaseDist {
  param rank: int;
  type idxType = int;

  const lowIdx: rank*idxType;
  const blocksize: rank*int;
  const targetLocDom: domain(rank);

  // these need to be var, so we can bulk-assign
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] unmanaged LocBlockCyclic(rank, idxType);

  var dataParTasksPerLocale: int; // tasks per locale for forall iteration

  proc init(startIdx,  // ?nd*?idxType
            blocksize,     // nd*int
            targetLocales: [] locale = Locales,
            dataParTasksPerLocale    = getDataParTasksPerLocale(),
            param rank: int          = _determineRankFromArg(startIdx),
            type idxType             = _determineIdxTypeFromArg(startIdx))
  {
    // argument sanity checks, with friendly error messages
    if isTuple(startIdx) != isTuple(blocksize) then compilerError("when invoking BlockCyclic initializer, startIdx and blocksize must be either both tuples or both integers");
    if isTuple(startIdx) && startIdx.size != blocksize.size then compilerError("when invoking BlockCyclic initializer and startIdx and blocksize are tuples, their sizes must match");
    if !isIntegralType(idxType) then compilerError("when invoking BlockCyclic initializer, startIdx must be an integer or a tuple of integers");
    if !isIntegralType(_determineIdxTypeFromArg(blocksize)) then compilerError("when invoking BlockCyclic initializer, blocksize must be an integer or a tuple of integers");

    this.rank = rank;
    this.idxType = idxType;

    this.lowIdx = _ensureTuple(startIdx);
    this.blocksize = _ensureTuple(blocksize);

    const ranges = setupTargetLocRanges(rank, targetLocales);
    this.targetLocDom = {(...ranges)};
    this.targetLocales = reshape(targetLocales, this.targetLocDom);

    if debugBlockCyclicDist {
      writeln(this.targetLocDom);
      writeln(this.targetLocales);
    }

    const dummyLBC = new unmanaged LocBlockCyclic(rank, idxType, dummy=true);
    var locDistTemp: [targetLocDom] unmanaged LocBlockCyclic(rank, idxType) = dummyLBC;

    coforall locid in targetLocDom do
      on this.targetLocales(locid) do
        locDistTemp(locid) = new unmanaged LocBlockCyclic(rank, idxType, locid, this.lowIdx, this.blocksize, targetLocDom);

    delete dummyLBC;
    this.locDist = locDistTemp; //make this a serial loop instead?

    if dataParTasksPerLocale == 0 then
      this.dataParTasksPerLocale = here.maxTaskPar;
    else
      this.dataParTasksPerLocale = dataParTasksPerLocale;

    this.complete();

    if debugBlockCyclicDist then
      for loc in locDist do writeln(loc);
  }

  // copy initializer for privatization
  proc init(param rank: int, type idxType, other: unmanaged BlockCyclic(rank, idxType)) {
    this.rank = rank;
    this.idxType = idxType;
    lowIdx = other.lowIdx;
    blocksize = other.blocksize;
    targetLocDom = other.targetLocDom;
    targetLocales = other.targetLocales;
    locDist = other.locDist;
    dataParTasksPerLocale = other.dataParTasksPerLocale;
  }

  proc dsiClone() {
    return new unmanaged BlockCyclic(lowIdx, blocksize, targetLocales, dataParTasksPerLocale);
  }

  override proc dsiDestroyDist() {
    coforall ld in locDist do {
      on ld do
        delete ld;
    }
  }

  proc dsiEqualDMaps(that: BlockCyclic(?)) {
    //
    // TODO: In retrospect, I think that this equality check
    // is too simple.  Since a distribution distributes the
    // whole plane/space that its domains live in, the equality
    // check should be that the two lowIdx's are equal modulo
    // the number of locales in that dimension times the blocksize.
    // I think this is more akin to what cyclic does.  So the
    // current test is shallower, indicating whether they were
    // parameterized the same, but not whether they distribute
    // the same.
    //
    return (this.lowIdx == that.lowIdx &&
            this.blocksize == that.blocksize &&
            this.targetLocales.equals(that.targetLocales));
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }
}

proc BlockCyclic._locsize {
  var ret : rank*int;
  for param i in 0..rank-1 {
    ret(i) = targetLocDom.dim(i).size;
  }
  return ret;
}

//
// create a new rectangular domain over this distribution
//
override proc BlockCyclic.dsiNewRectangularDom(param rank: int, type idxType,
                                      param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("BlockCyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("BlockCyclic domain rank does not match distribution's");

  var dom = new unmanaged BlockCyclicDom(rank=rank, idxType=idxType,
                                         dist=_to_unmanaged(this),
                                         stridable=stridable);
  dom.dsiSetIndices(inds);
  return dom;
}

//
// output distribution
//
proc BlockCyclic.writeThis(x) throws {
  x <~> "BlockCyclic\n";
  x <~> "-------\n";
  x <~> "distributes: " <~> lowIdx <~> "..." <~> "\n";
  x <~> "in chunks of: " <~> blocksize <~> "\n";
  x <~> "across locales: " <~> targetLocales <~> "\n";
  x <~> "indexed via: " <~> targetLocDom <~> "\n";
  x <~> "resulting in: " <~> "\n";
  for locid in targetLocDom do
    x <~> "  [" <~> locid <~> "] " <~> locDist(locid) <~> "\n";
}

//
// convert an index into a locale value
//
proc BlockCyclic.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(idxToLocaleInd(ind));
}

proc BlockCyclic.dsiIndexToLocale(ind: rank*idxType) {
  return targetLocales(idxToLocaleInd(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc BlockCyclic.getStarts(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  var D: domain(rank, idxType, stridable=true);
  var R: rank*range(idxType, stridable=true);
  for i in 0..rank-1 {
    var lo, hi: idxType;
    const domlo = inds.dim(i).low,
          domhi = inds.dim(i).high;
    const mylo = locDist(locid).myStarts(i).low;
    const mystr = locDist(locid).myStarts(i).stride;
    if (domlo != lowIdx(i)) {
      if (domlo <= domhi) {
        if (domlo > lowIdx(i)) {
      const off = (domlo - mylo)%mystr;
      if (off == 0) {
        lo = domlo;
        hi = domhi;
      } else {
        lo = domlo-off;
        hi = domhi;
//      halt("BLC: need to handle unaligned low");
      }
    } else {
      halt("BLC: need to handle domain low less than lowIdx");
        }
      } else {
        lo = domlo;
        hi = domhi;
      }
    } else {
      lo = mylo;
      hi = domhi;
    }
    R(i) = lo..hi by mystr;
  }
  D.setIndices(R);
  return D;
}

//
// determine which locale owns a particular index
//
// TODO: I jotted down a note during the code review asking whether
// targetLocales.size and boundingbox.size should be
// captured locally, or captured in the default dom/array implementation
// or inlined.  Not sure what that point was anymore, though.  Maybe
// someone else can help me remember it (since it was probably someone
// else's suggestion).
//
proc BlockCyclic.idxToLocaleInd(ind: idxType) where rank == 1 {
  const ind0 = ind - lowIdx(0);
  //  compilerError((ind0/blocksize(0)%targetLocDom.dim(0).type:string);
  return (ind0 / blocksize(0)) % targetLocDom.dim(0).size;
}

proc BlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank == 1 {
  return idxToLocaleInd(ind(0));
}

proc BlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank != 1 {
  var locInd: rank*int;
  for param i in 0..rank-1 {
    const ind0 = ind(i) - lowIdx(i);
    locInd(i) = ((ind0 / blocksize(i)) % targetLocDom.dim(i).size): int;
  }
  return locInd;
}

proc BlockCyclic.init(other: BlockCyclic, privatizeData,
                      param rank = other.rank, type idxType = other.idxType) {
  this.rank = rank;
  this.idxType = idxType;
  lowIdx = privatizeData[0];
  blocksize = privatizeData[1];
  targetLocDom = {(...privatizeData[2])};
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = privatizeData[3];
}

override proc BlockCyclic.dsiSupportsPrivatization() param return true;

proc BlockCyclic.dsiGetPrivatizeData() {
  return (lowIdx, blocksize, targetLocDom.dims(), dataParTasksPerLocale);
}

proc BlockCyclic.dsiPrivatize(privatizeData) {
  return new unmanaged BlockCyclic(_to_unmanaged(this), privatizeData);
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Distribution Class
//
class LocBlockCyclic {
  param rank: int;
  type idxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  const myStarts: rank*range(idxType, BoundedRangeType.boundedLow, stridable=true);

  //
  // Constructor computes what chunk of index(0) is owned by the
  // current locale
  //
  proc init(param rank: int,
            type idxType,
            locid,   // the locale index from the target domain
            lowIdx, blocksize, targetLocDom) {  // from global dist
    this.rank = rank;
    this.idxType = idxType;

    this.complete();

    if rank == 1 {
      const lo = lowIdx(0) + (locid * blocksize(0));
      const str = blocksize(0) * targetLocDom.size;
      myStarts(0) = lo.. by str;
    } else {
      for param i in 0..rank-1 {
        const lo = lowIdx(i) + (locid(i) * blocksize(i));
        const str = blocksize(i) * targetLocDom.dim(i).size;
        myStarts(i) = lo.. by str;
      }
    }
  }

  // Used to create a dummy instance.
  proc init(param rank, type idxType, param dummy: bool) where dummy {
    this.rank = rank;
    this.idxType = idxType;
 }
}


proc LocBlockCyclic.writeThis(x) throws {
  var localeid: int;
  on this {
    localeid = here.id;
  }
  x <~> "locale " <~> localeid <~> " owns blocks: " <~> myStarts;
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Domain Class
//
class BlockCyclicDom: BaseRectangularDom {
  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: unmanaged BlockCyclic(rank, idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  var locDomsNil: [dist.targetLocDom] unmanaged LocBlockCyclicDom(rank, idxType, stridable)?;

  inline proc locDoms(idx) return locDomsNil(idx)!;


  //
  // a domain describing the complete domain
  //
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  //  const startLoc: index(dist.targetLocDom);
}

proc BlockCyclicDom.dsiDims() return whole.dims();

proc BlockCyclicDom.dsiDim(d: int) return whole.dim(d);

iter BlockCyclicDom.these() {
  for i in whole do
    yield i;
}

iter BlockCyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks      = dist.dataParTasksPerLocale;
  const ignoreRunning = getDataParIgnoreRunningTasks();
  const minSize       = getDataParMinGranularity();
  coforall locDomQ in locDomsNil do on locDomQ {
    const locDom = locDomQ!;
    // TODO: There's a compiler bug when using a simple ``forall`` over ``myStarts``
    // that impacts reductions over BlockCyclic arrays. It appears that the
    // result of the reduction is always 0 (for an array of ints). Looking at
    // the generated code, it seems like the reduction object is not being
    // passed through nested loops correctly (or at all).
    for follow in locDom.myStarts.these(iterKind.leader, maxTasks, ignoreRunning, minSize) {
      for i in locDom.myStarts.these(iterKind.follower, follow, maxTasks, ignoreRunning, minSize) {
        var retblock: rank*range(idxType);
        for param j in 0..rank-1 {
          const lo     = if rank == 1 then i else i(j);
          const dim    = whole.dim(j);
          const dimLow = dim.low;

          var temp : range(idxType, stridable=stridable);
          temp = max(lo, dimLow)..
                     min(lo + dist.blocksize(j)-1, dim.high);
          temp     = dim[temp];
          temp     = temp.chpl__unTranslate(dimLow);

          retblock(j) = (temp.low / dim.stride:idxType)..
                        #temp.size;
        }
        yield retblock;
      }
    }
  }
}

//
// TODO: Abstract the addition of low into a function?
// Note relationship between this operation and the
// order/position functions -- any chance for creating similar
// support? (esp. given how frequent this seems likely to be?)
//
// TODO: Is there some clever way to invoke the leader/follower
// iterator on the local blocks in here such that the per-core
// parallelism is expressed at that level?  Seems like a nice
// natural composition and might help with my fears about how
// stencil communication will be done on a per-locale basis.
//
iter BlockCyclicDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  var t: rank*range(idxType, stridable=stridable);

  for param i in 0..rank-1 {
    const curFollow = followThis(i);
    const dim       = whole.dim(i);
    const stride    = dim.stride: idxType;
    const low       = stride * curFollow.low;
    const high      = stride * curFollow.high;
    t(i) = ((low..high by stride:int) + dim.low).safeCast(t(i).type);
  }

  for i in {(...t)} {
    yield i;
  }
}

//
// output domain
//
proc BlockCyclicDom.dsiSerialWrite(x) {
  x <~> whole;
}

//
// how to allocate a new array over this domain
//
proc BlockCyclicDom.dsiBuildArray(type eltType, param initElts:bool) {
  const dom = this;
  var locArrTemp: [dom.dist.targetLocDom] unmanaged LocBlockCyclicArr(eltType, rank, idxType, stridable)?;
  var myLocArrTemp: unmanaged LocBlockCyclicArr(eltType, rank, idxType, stridable)?;
  const creationLocale = here;

  // formerly BlockCyclicArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref myLocArrTemp) {
    on dom.dist.targetLocales(localeIdx) {
      const LBCA = new unmanaged LocBlockCyclicArr(eltType, rank,
                                                   idxType, stridable,
                                                   dom.locDoms(localeIdx),
                                                   dom.locDoms(localeIdx),
                                                   initElts=initElts);
      locArrTemp(localeIdx) = LBCA;
      if here == creationLocale then
        myLocArrTemp = LBCA;
    }
  }
  var locArrNN = locArrTemp!; //#15080

  var arr = new unmanaged BlockCyclicArr(eltType=eltType, rank=rank,
                                         idxType=idxType, stridable=stridable,
                                         locArr = locArrNN,
                                         myLocArr = myLocArrTemp,
                                         dom=_to_unmanaged(this));

  return arr;
}

proc BlockCyclicDom.dsiNumIndices return whole.size;
proc BlockCyclicDom.dsiLow return whole.low;
proc BlockCyclicDom.dsiHigh return whole.high;
proc BlockCyclicDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make setIndices() for a rectangular
// domain take a domain rather than something else?
//
proc BlockCyclicDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
}

proc BlockCyclicDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(0).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
}

proc BlockCyclicDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

proc BlockCyclicDom.dsiGetIndices() {
  return whole.getIndices();
}

override proc BlockCyclicDom.dsiMyDist() return dist;

proc BlockCyclicDom.setup() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocales(localeIdx) do
      if (locDomsNil(localeIdx) == nil) then
        locDomsNil(localeIdx) = new unmanaged LocBlockCyclicDom(rank, idxType, stridable, _to_unmanaged(this),
                                                   dist.getStarts(whole, localeIdx));
      else {
        locDoms(localeIdx).myStarts = dist.getStarts(whole, localeIdx);
        locDoms(localeIdx).myFlatInds = {0..#locDoms(localeIdx).computeFlatInds()};
      }
  if debugBlockCyclicDist then
    enumerateBlocks();
}

override proc BlockCyclicDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocales(localeIdx) do
      delete locDoms(localeIdx);
}

proc BlockCyclicDom.enumerateBlocks() {
  for locidx in dist.targetLocDom {
    on dist.targetLocales(locidx) do locDoms(locidx).enumerateBlocks();
  }
}

proc BlockCyclicDom.chpl__serialize() {
  return pid;
}

// TODO: What happens when we try to deserialize on a locale that doesn't
// own a copy of the privatized class?  (can that happen?)  Could this
// be a way to lazily privatize by also making the originating locale part
// of the 'data'?
proc type BlockCyclicDom.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged BlockCyclicDom(rank=this.rank,
                                    idxType=this.idxType,
                                    stridable=this.stridable),
           data);
}

override proc BlockCyclicDom.dsiSupportsPrivatization() param return true;

proc BlockCyclicDom.dsiGetPrivatizeData() return dist.pid;

proc BlockCyclicDom.dsiPrivatize(privatizeData) {
  var privateDist = chpl_getPrivatizedCopy(dist.type, privatizeData);
  var c = new unmanaged BlockCyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privateDist);
  c.locDomsNil = locDomsNil;
  c.whole = whole;
  return c;
}

proc BlockCyclicDom.dsiGetReprivatizeData() return 0;

proc BlockCyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDomsNil = other.locDomsNil;
  whole = other.whole;
}

proc BlockCyclicDom.dsiMember(i) {
  return whole.contains(i);
}

proc BlockCyclicDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}


////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Domain Class
//
class LocBlockCyclicDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // UP LINK: a reference to the parent global domain class
  //
  const globDom: unmanaged BlockCyclicDom(rank, idxType, stridable);

  //
  // a local domain describing the indices owned by this locale
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myStarts: domain(rank, idxType, stridable=true);
  var myFlatInds: domain(1);
}

proc LocBlockCyclicDom.postinit() {
  myFlatInds = {0..#computeFlatInds()};
}

//
// Initialization helpers
//
proc LocBlockCyclicDom.computeFlatInds() {
  //  writeln("myStarts = ", myStarts);
  const numBlocks = * reduce [d in 0..rank-1] (myStarts.dim(d).size),
    indsPerBlk = * reduce [d in 0..rank-1] (globDom.dist.blocksize(d));
  //  writeln("Total number of inds = ", numBlocks * indsPerBlk);
  return numBlocks * indsPerBlk;
}

//
// output local domain piece
//
proc LocBlockCyclicDom.writeThis(x) throws {
  x <~> myStarts;
}

proc LocBlockCyclicDom.enumerateBlocks() {
  for i in myStarts {
    write(here.id, ": {");
    for param j in 0..rank-1 {
      if (j != 0) {
        write(", ");
      }
      // TODO: support a tuple-oriented iteration of vectors to avoid this?
      var lo: idxType;
      if rank == 1 then
        lo = i;
      else
        lo = i(j);
      write(lo, "..", min(lo + globDom.dist.blocksize(j)-1,
                          globDom.whole.dim(j).high));
    }
    writeln("}");
  }
}


//
// queries for this locale's number of indices, low, and high bounds
//
// TODO: I believe these are only used by the random number generator
// in stream -- will they always be required once that is rewritten?
//
proc LocBlockCyclicDom.size {
  return myStarts.size;
}

proc LocBlockCyclicDom.low {
  return myStarts.low;
}

proc LocBlockCyclicDom.high {
  return myStarts.high;
}

proc LocBlockCyclicDom._lens {
  var ret : rank*int;
  for param i in 0..rank-1 {
    ret(i) = myStarts.dim(i).size;
  }
  return ret;
}

proc LocBlockCyclicDom._sizes {
    //
    // For dimension ``i`` in 1 <= d <= rank, sizes(d+1) represents the
    // distance in the flat array between elements in dimension ``i``. Similar
    // to ``blk`` in DefaultRectangular.
    //
    var sizes : (rank+1)*int;
    sizes(rank) = 1;
    for i in 0..rank-1 by -1 do
      sizes(i) = sizes(i+1) * (globDom.dist.blocksize(i) * myStarts.dim(i).size);
    return sizes;
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Array Class
//
class BlockCyclicArr: BaseRectangularArr {

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: unmanaged BlockCyclicDom(rank, idxType, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  var locArr: [dom.dist.targetLocDom] unmanaged LocBlockCyclicArr(eltType, rank, idxType, stridable);

  //
  // optimized reference to a local LocBlockCyclicArr instance (or nil)
  //
  var myLocArr: unmanaged LocBlockCyclicArr(eltType, rank, idxType, stridable)?;
}

override proc BlockCyclicArr.dsiGetBaseDom() return dom;

override proc BlockCyclicArr.dsiElementInitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      var arr = locArr(localeIdx);
      arr.myElems.dsiElementInitializationComplete();
    }
  }
}

override proc BlockCyclicArr.dsiDestroyArr(param deinitElts:bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      var arr = locArr(localeIdx);
      if deinitElts {
        for subdom in do_dsiLocalSubdomains(arr!.indexDom) {
          forall j in subdom {
            chpl__autoDestroy(arr(j));
          }
        }
      }
      delete arr;
    }
  }
}

proc BlockCyclicArr.chpl__serialize() {
  return pid;
}

proc type BlockCyclicArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged BlockCyclicArr(rank=this.rank,
                                    idxType=this.idxType,
                                    stridable=this.stridable,
                                    eltType=this.eltType),
           data);
}

override proc BlockCyclicArr.dsiSupportsPrivatization() param return true;

proc BlockCyclicArr.dsiGetPrivatizeData() return 0;

proc BlockCyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new unmanaged BlockCyclicArr(eltType=eltType, rank=rank,
    idxType=idxType, stridable=stridable, dom=privdom, locArr=locArr);

  for localeIdx in dom.dist.targetLocDom do
    if c.locArr(localeIdx).locale == here then
      c.myLocArr = c.locArr(localeIdx);

  return c;
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in idxToLocaleind?
//
proc BlockCyclicArr.dsiAccess(i: idxType) ref where rank == 1 {
  if myLocArr then /* TODO: reenable */ /* local */ {
    if myLocArr!.indexDom.myStarts.contains(i) then  // TODO: This could be beefed up; true for indices other than starts
      return myLocArr!.this(i);
  }
  //  var loci = dom.dist.idxToLocaleInd(i);
  //  compilerError(loci.type:string);
  //  var desc = locArr(loci);
  //  return locArr(loci)(i);
  return locArr(dom.dist.idxToLocaleInd(i))(i);
}

proc BlockCyclicArr.dsiAccess(i: rank*idxType) ref {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.myStarts.contains(i) then
//       return myLocArr.this(i);
//   }
  if rank == 1 {
    return dsiAccess(i(0));
  } else {
    return locArr(dom.dist.idxToLocaleInd(i))(i);
  }
}

proc BlockCyclicArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.dsiMember(i);
}

iter BlockCyclicArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

iter BlockCyclicArr.these(param tag: iterKind) where tag == iterKind.leader {
  for yieldThis in dom.these(tag) do
    yield yieldThis;
}

iter BlockCyclicArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  var myFollowThis: rank*range(idxType=idxType, stridable=stridable);

  for param i in 0..rank-1 {
    const curFollow = followThis(i);
    const dim       = dom.whole.dim(i);
    const stride    = dim.stride;
    const low       = curFollow.low * stride;
    const high      = curFollow.high * stride;
    myFollowThis(i) = ((low..high by stride) + dim.low).safeCast(curFollow.type);
  }

  const myFollowThisDom = {(...myFollowThis)};

  // TODO: Can we use the fast-follower optimization here at all? See BlockDist
  // for a working examples.
  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  // const myLocArr = locArr(dom.dist.idxToLocaleInd(lowIdx));

  //
  // we don't own all the elements we're following
  //
  for i in myFollowThisDom {
    yield dsiAccess(i);
  }
}

proc BlockCyclicArr.dsiSerialRead(f) {
  chpl_serialReadWriteRectangular(f, this);
}

//
// output array
//
proc BlockCyclicArr.dsiSerialWrite(f) {
  chpl_serialReadWriteRectangular(f, this);
}

proc BlockCyclicArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}
proc BlockCyclicDom.dsiTargetLocales() {
  return dist.targetLocales;
}
proc BlockCyclic.dsiTargetLocales() {
  return targetLocales;
}


proc BlockCyclicArr.dsiHasSingleLocalSubdomain() param return false;
proc BlockCyclicDom.dsiHasSingleLocalSubdomain() param return false;

// essentially enumerateBlocks()
// basically add blocksize to the start indices
private
iter do_dsiLocalSubdomains(indexDom) {
  param rank = indexDom.rank;
  type idxType = indexDom.idxType;
  for i in indexDom.myStarts {
    var temp : rank*range(idxType);
    const blockSizes = indexDom.globDom.dist.blocksize;
    const globDims = indexDom.globDom.whole.dims();
    for param j in 0..rank-1 {
      var lo: idxType;
      if rank == 1 then lo = i;
      else lo = i(j);
      temp(j) = lo .. min(lo + blockSizes(j)-1, globDims(j).high);
    }
    yield {(...temp)};
  }
}
iter BlockCyclicArr.dsiLocalSubdomains(loc: locale) {
  if loc != here then
    unimplementedFeatureHalt("BlockCyclic", "remote subdomain queries");

  for i in do_dsiLocalSubdomains(myLocArr!.indexDom) do
    yield i;
}
iter BlockCyclicDom.dsiLocalSubdomains(loc: locale) {
  if loc != here then
    unimplementedFeatureHalt("BlockCyclic", "remote subdomain queries");

  // TODO -- could be replaced by a privatized myLocDom in BlockCyclicDom
  // as it is with BlockCyclicArr
  var myLocDom:unmanaged LocBlockCyclicDom(rank, idxType, stridable)? = nil;
  for (loc, locDom) in zip(dist.targetLocales, locDomsNil) {
    if loc == here then
      myLocDom = locDom;
  }
  for i in do_dsiLocalSubdomains(myLocDom!) do
    yield i;
}


////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Array Class
//
class LocBlockCyclicArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: a reference to the local domain class for this array and locale
  //
  const allocDom: unmanaged LocBlockCyclicDom(rank, idxType, stridable);
  // MPF TODO: Why do we need indexDom at all? is it always == allocDom?
  const indexDom: unmanaged LocBlockCyclicDom(rank, idxType, stridable);


  // STATE:

  //
  // the block of local array data
  //
  pragma "local field" pragma "unsafe" pragma "no auto destroy"
  // may be initialized separately
  // always destroyed explicitly (to control deiniting elts)
  var myElems: [allocDom.myFlatInds] eltType;

  // TODO: need to be able to access these, but is this the right place?
  const blocksize : rank*int = allocDom.globDom.dist.blocksize;
  const low                  = allocDom.globDom.dsiLow;
  const locsize   : rank*int = allocDom.globDom.dist._locsize;
  const numblocks : rank*int = allocDom._lens;

  const sizes : (rank+1)*int = allocDom._sizes;

  proc init(type eltType,
            param rank: int,
            type idxType,
            param stridable: bool,
            allocDom: unmanaged LocBlockCyclicDom(rank, idxType, stridable),
            indexDom: unmanaged LocBlockCyclicDom(rank, idxType, stridable),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.stridable = stridable;
    this.allocDom = allocDom;
    this.indexDom = indexDom;
    this.myElems = this.allocDom.myFlatInds.buildArray(eltType, initElts=false);
    this.complete();

    // Initialize only those elements referring to valid indices
    // since these are the only ones that will be deinitialized.
    if initElts {
      for subdom in do_dsiLocalSubdomains(indexDom) {
        forall j in subdom {
          pragma "no auto destroy" pragma "unsafe"
          var def: eltType;
          __primitive("=", this.this(j), def);
        }
      }
    }
  }
  proc deinit() {
    // Elements in myElems are deinited in dsiDestroyArr if necessary.
    // Here we need to clean up the rest of the array.
    _do_destroy_array(myElems, deinitElts=false);
  }
}


proc LocBlockCyclicArr.mdInd2FlatInd(i: ?t, dim = 0) where t == idxType {
  //  writeln("blksize");
  const blksize = blocksize(dim);
  //  writeln("ind0");
  const ind0 = (i - low): int;
  //  writeln("blkNum");
  const blkNum = ind0 / (blksize * locsize(dim));
  //  writeln("blkOff");
  const blkOff = ind0 % blksize;
  //  writeln("returning");
  return  blkNum * blksize + blkOff;
}

proc LocBlockCyclicArr.mdInd2FlatInd(i: ?t) where t == rank*idxType {
  if (false) {  // CMO
    var blkmults = * scan [d in 0..rank-1] blocksize(d);
    //    writeln("blkmults = ", blkmults);
    var numwholeblocks = 0;
    var blkOff = 0;
    for param d in 0..rank-1 by -1 {
      const blksize = blocksize(d);
      const ind0 = (i(d) - low(d)): int;
      const blkNum = ind0 / (blksize * locsize(d));
      const blkDimOff = ind0 % blksize;
      if (d != rank) {
        numwholeblocks *= numblocks(rank-d);
        blkOff *= blkmults(rank-d);
      }
      numwholeblocks += blkNum;
      blkOff += blkDimOff;
    }
    return (numwholeblocks * blocksize(rank)) + blkOff;
  } else { // RMO

    var idx = 0;

    for param d in 0..rank-1 {
      const bs              = blocksize(d);  // cache for performance
      const base            = i(d) - low(d); // zero-based index
      const localBlockNum   = base / (bs * locsize(d));
      const localBlockStart = localBlockNum * bs;
      const remainder       = base % bs;

      const locIdx = localBlockStart + remainder;

      idx += locIdx * sizes(d+1);
    }

    return idx;
  }
}

//
// the accessor for the local array -- assumes the index is local
//
proc LocBlockCyclicArr.this(i) ref {
  const flatInd = mdInd2FlatInd(i);
  //writeln(i, " --> ", flatInd);
  return myElems(flatInd);
}

//
// output local array piece
//
proc LocBlockCyclicArr.writeThis(x) throws {
  // note on this fails; see writeThisUsingOn.chpl
  x <~> myElems;
}

// sungeun: This doesn't appear to be used yet, so I left it, but it
//  might be useful to others.  Consider putting it in DSIUtil.chpl.

//
// helper function for blocking index ranges
//
proc _computeBlockCyclic(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  proc procToData(x, lo)
    return lo + (x:lo.type) + (x:real != x:int:real):lo.type;

  const blo =
    if blocknum == 0 then waylo
      else procToData((numelems:real * blocknum) / numblocks, lo);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
      else procToData((numelems:real * (blocknum+1)) / numblocks, lo) - 1;

  return (blo, bhi);
}
