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

//
// Block Distribution
// 
//      Block       BlockDom     BlockArr
//
//   LocBlock    LocBlockDom  LocBlockArr
//

use DSIUtil;

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
// 1. Changes to Block.tasksPerLocale are not made in privatized
//    copies of this distribution.
//


config param debugBlockCyclicDist = false; // internal development flag (debugging)

proc _determineRankFromArg(startIdx) param {
  return if isTuple(startIdx) then startIdx.size else 1;
}

proc _determineIdxTypeFromArg(startIdx) type {
  return if isTuple(startIdx) then startIdx(1).type else startIdx.type;
}

proc _ensureTuple(arg) {
  return if isTuple(arg) then arg else (arg,);
}


////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Distribution Class
//
class BlockCyclic : BaseDist {
  param rank: int;
  type idxType = int;

  const lowIdx: rank*idxType;
  const blocksize: rank*int;
  const targetLocDom: domain(rank);
  const targetLocales: [targetLocDom] locale;
  const locDist: [targetLocDom] LocBlockCyclic(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  proc BlockCyclic(startIdx,  // ?nd*?idxType
                   blocksize,     // nd*int
                   targetLocales: [] locale = Locales, 
                   tasksPerLocale = 0,
                   param rank: int = _determineRankFromArg(startIdx),
                   type idxType = _determineIdxTypeFromArg(startIdx))
  {
    // argument sanity checks, with friendly error messages
    if isTuple(startIdx) != isTuple(blocksize) then compilerError("when invoking BlockCyclic constructor, startIdx and blocksize must be either both tuples or both integers");
    if isTuple(startIdx) && startIdx.size != blocksize.size then compilerError("when invoking BlockCyclic constructor and startIdx and blocksize are tuples, their sizes must match");
    if !isIntegralType(idxType) then compilerError("when invoking BlockCyclic constructor, startIdx must be an integer or a tuple of integers");
    if !isIntegralType(_determineIdxTypeFromArg(blocksize)) then compilerError("when invoking BlockCyclic constructor, blocksize must be an integer or a tuple of integers");

    this.lowIdx = _ensureTuple(startIdx);
    this.blocksize = _ensureTuple(blocksize);
    if rank == 1 {
      targetLocDom = {0..#targetLocales.numElements}; // 0-based for simplicity
      this.targetLocales = targetLocales;
    } else if targetLocales.rank == 1 then {

      // BLC: Common code, factor out

      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank do
        ranges(i) = 0..factors(i)-1;
      targetLocDom = {(...ranges)};
      for (loc1, loc2) in zip(this.targetLocales, targetLocales) do
        loc1 = loc2;
      if debugBlockCyclicDist {
        writeln(targetLocDom);
        writeln(this.targetLocales);
      }
    } else {
      if targetLocales.rank != rank then
    compilerError("locales array rank must be one or match distribution rank");

      var ranges: rank*range;
      for param i in 1..rank do {
    var thisRange = targetLocales.domain.dim(i);
    ranges(i) = 0..#thisRange.length; 
      }
      
      targetLocDom = {(...ranges)};
      if debugBlockCyclicDist then writeln(targetLocDom);

      this.targetLocales = reshape(targetLocales, targetLocDom);
      if debugBlockCyclicDist then writeln(this.targetLocales);
    }

    coforall locid in targetLocDom do
      on this.targetLocales(locid) do
        locDist(locid) = new LocBlockCyclic(rank, idxType, locid, this);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = 1;   // TODO: here.maxTaskPar;
    else
      this.tasksPerLocale = tasksPerLocale;

    if debugBlockCyclicDist then
      for loc in locDist do writeln(loc);
  }

  // copy constructor for privatization
  proc BlockCyclic(param rank: int, type idxType, other: BlockCyclic(rank, idxType)) {
    lowIdx = other.lowIdx;
    blocksize = other.blocksize;
    targetLocDom = other.targetLocDom;
    targetLocales = other.targetLocales;
    locDist = other.locDist;
    tasksPerLocale = other.tasksPerLocale;
  }

  proc dsiClone() {
    return new BlockCyclic(lowIdx, blocksize, targetLocales, tasksPerLocale);
  }
}

//
// create a new rectangular domain over this distribution
//
proc BlockCyclic.dsiNewRectangularDom(param rank: int, type idxType,
                           param stridable: bool) {
  if idxType != this.idxType then
    compilerError("BlockCyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("BlockCyclic domain rank does not match distribution's");

  var dom = new BlockCyclicDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  return dom;
}

//
// output distribution
//
proc BlockCyclic.writeThis(x:Writer) {
  x.writeln("BlockCyclic");
  x.writeln("-------");
  x.writeln("distributes: ", lowIdx, "...");
  x.writeln("in chunks of: ", blocksize);
  x.writeln("across locales: ", targetLocales);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] ", locDist(locid));
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
  for i in 1..rank {
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
// targetLocales.numElements and boundingbox.numIndices should be
// captured locally, or captured in the default dom/array implementation
// or inlined.  Not sure what that point was anymore, though.  Maybe
// someone else can help me remember it (since it was probably someone
// else's suggestion).
//
proc BlockCyclic.idxToLocaleInd(ind: idxType) where rank == 1 {
  const ind0 = ind - lowIdx(1);
  //  compilerError(typeToString((ind0/blocksize(1)%targetLocDom.dim(1).type));
  return (ind0 / blocksize(1)) % targetLocDom.dim(1).length;
}

proc BlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank == 1 {
  return idxToLocaleInd(ind(1));
}

proc BlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank != 1 {
  var locInd: rank*int;
  for param i in 1..rank {
    const ind0 = ind(i) - lowIdx(i);
    locInd(i) = ((ind0 / blocksize(i)) % targetLocDom.dim(i).length): int; 
  }
  return locInd;
}

pragma "auto copy fn"
proc chpl__autoCopy(x: BlockCyclic) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: BlockCyclic) {
  if !noRefCount {
    var cnt = x.destroyDist();
    if cnt == 0 then
      delete x;
  }
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
  // Constructor computes what chunk of index(1) is owned by the
  // current locale
  //
  proc LocBlockCyclic(param rank: int,
                 type idxType, 
                 locid,   // the locale index from the target domain
                 dist: BlockCyclic(rank, idxType)) { // reference to glob dist
    if rank == 1 {
      const lo = dist.lowIdx(1) + (locid * dist.blocksize(1));
      const str = dist.blocksize(1) * dist.targetLocDom.numIndices;
      myStarts(1) = lo.. by str;
    } else {
      for param i in 1..rank {
        const lo = dist.lowIdx(i) + (locid(i) * dist.blocksize(i));
        const str = dist.blocksize(i) * dist.targetLocDom.dim(i).length;
        myStarts(i) = lo.. by str;
      }
    }
  }
}


proc LocBlockCyclic.writeThis(x:Writer) {
  var localeid: int;
  on this {
    localeid = here.id;
  }
  x.write("locale ", localeid, " owns blocks: ", myStarts);
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Domain Class
//
class BlockCyclicDom: BaseRectangularDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: BlockCyclic(rank, idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  var locDoms: [dist.targetLocDom] LocBlockCyclicDom(rank, idxType, stridable);

  //
  // a domain describing the complete domain
  //
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  //  const startLoc: index(dist.targetLocDom);

  var pid: int = -1; // privatized object id
}

pragma "auto copy fn"
proc chpl__autoCopy(x: BlockCyclicDom) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: BlockCyclicDom) {
  if !noRefCount {
    var cnt = x.destroyDom();
    if cnt == 0 then
      delete x;
  }
}

proc BlockCyclicDom.dsiDims() return whole.dims();

proc BlockCyclicDom.dsiDim(d: int) return whole.dim(d);

iter BlockCyclicDom.these() {
  for i in whole do
    yield i;
}

iter BlockCyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  const precomputedNumTasks = dist.tasksPerLocale;
  const precomputedWholeLow = whole.low;
  if (precomputedNumTasks != 1) then
    halt("Can't use more than one task per locale with Block-Cyclic currently");
  coforall locDom in locDoms do on locDom {
      var tmpblock:rank*range(idxType, stridable=stridable);
    for i in locDom.myStarts {
      //      writeln("[", here.id, "] starting at ", i);
      for param j in 1..rank {
        // TODO: support a tuple-oriented iteration of vectors to avoid this?
        var lo: idxType;
        if rank == 1 then
          lo = i;
        else
          lo = i(j);
        tmpblock(j) = max(lo, whole.dim(j).low)..
                  min(lo + dist.blocksize(j)-1, whole.dim(j).high);
        //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
        tmpblock(j) = whole.dim(j)[tmpblock(j)];
        //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
        if rank == 1 then
          lo = whole.low;
        else
          lo = whole.low(j);
        //        writeln("lo = ", lo);
        tmpblock(j) = tmpblock(j).chpl__unTranslate(lo);
        //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
      }

      var retblock: rank*range(idxType);
      for param i in 1..rank {
        retblock(i) = (tmpblock(i).low / whole.dim(i).stride:idxType)..
                        #tmpblock(i).length;
          //        retblock(i) = (tmpblock(i) - whole.dim(i).low);
      }
      //      writeln(here.id, ": Domain leader yielding", retblock);
      yield retblock;
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
  //  writeln(here.id, ": Domain follower following ", follower);
  var t: rank*range(idxType, stridable=stridable);
  for param i in 1..rank {
    var stride = whole.dim(i).stride: idxType;
    var low = stride * followThis(i).low;
    var high = stride * followThis(i).high;
    t(i) = (low..high by stride:int) + whole.dim(i).low;
  }
  //  writeln(here.id, ": Changed it into: ", t);
  for i in {(...t)} {
    yield i;
  }
}

//
// output domain
//
proc BlockCyclicDom.dsiSerialWrite(x:Writer) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
proc BlockCyclicDom.dsiBuildArray(type eltType) {
  var arr = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

proc BlockCyclicDom.dsiNumIndices return whole.numIndices;
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
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
}

proc BlockCyclicDom.dsiGetIndices() {
  return whole.getIndices();
}

proc BlockCyclicDom.dsiMyDist() return dist;

proc BlockCyclicDom.setup() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocales(localeIdx) do
      if (locDoms(localeIdx) == nil) then
        locDoms(localeIdx) = new LocBlockCyclicDom(rank, idxType, stridable, this, 
                                                   dist.getStarts(whole, localeIdx));
      else {
        locDoms(localeIdx).myStarts = dist.getStarts(whole, localeIdx);
        locDoms(localeIdx).myFlatInds = {0..#locDoms(localeIdx).computeFlatInds()};
      }
  if debugBlockCyclicDist then
    enumerateBlocks();
}

proc BlockCyclicDom.~BlockCyclicDom() {
  // Free the locDoms array.
  coforall localeIdx in dist.targetLocDom do
    if locDoms(localeIdx) != nil then
      on locDoms(localeIdx) do
        delete locDoms(localeIdx);

  // Release our reference to the distribution.
  var cnt = dist.decRefCount();
  if cnt == 0 then
    delete dist;
}

proc BlockCyclicDom.enumerateBlocks() {
  for locidx in dist.targetLocDom {
    on dist.targetLocales(locidx) do locDoms(locidx).enumerateBlocks();
  }
}

proc BlockCyclicDom.dsiSupportsPrivatization() param return true;

proc BlockCyclicDom.dsiGetPrivatizeData() return 0;

proc BlockCyclicDom.dsiPrivatize(privatizeData) {
  var privateDist = new BlockCyclic(rank, idxType, dist);
  var c = new BlockCyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privateDist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

proc BlockCyclicDom.dsiGetReprivatizeData() return 0;

proc BlockCyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = other.whole;
}

proc BlockCyclicDom.dsiMember(i) {
  return whole.member(i);
}

proc BlockCyclicDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

proc BlockCyclicDom.dsiBuildRectangularDom(param rank: int, type idxType,
                                         param stridable: bool,
                                         ranges: rank*range(idxType,
                                                            BoundedRangeType.bounded,
                                                            stridable)) {
  if idxType != dist.idxType then
    compilerError("BlockCyclic domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("BlockCyclic domain rank does not match distribution's");

  var dom = new BlockCyclicDom(rank=rank, idxType=idxType,
                               dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;
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
  const globDom: BlockCyclicDom(rank, idxType, stridable);

  //
  // a local domain describing the indices owned by this locale
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myStarts: domain(rank, idxType, stridable=true);
  var myFlatInds: domain(1) = {0..#computeFlatInds()};
}

//
// Initialization helpers
//
proc LocBlockCyclicDom.computeFlatInds() {
  //  writeln("myStarts = ", myStarts);
  const numBlocks = * reduce [d in 1..rank] (myStarts.dim(d).length),
    indsPerBlk = * reduce [d in 1..rank] (globDom.dist.blocksize(d));
  //  writeln("Total number of inds = ", numBlocks * indsPerBlk);
  return numBlocks * indsPerBlk;
}

//
// output local domain piece
//
proc LocBlockCyclicDom.writeThis(x:Writer) {
  x.write(myStarts);
}

proc LocBlockCyclicDom.enumerateBlocks() {
  for i in myStarts {
    write(here.id, ": {");
    for param j in 1..rank {
      if (j != 1) {
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
proc LocBlockCyclicDom.numIndices {
  return myStarts.numIndices;
}

proc LocBlockCyclicDom.low {
  return myStarts.low;
}

proc LocBlockCyclicDom.high {
  return myStarts.high;
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Array Class
//
class BlockCyclicArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: BlockCyclicDom(rank, idxType, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  var locArr: [dom.dist.targetLocDom] LocBlockCyclicArr(eltType, rank, idxType, stridable);

  //
  // optimized reference to a local LocBlockCyclicArr instance (or nil)
  //
  var myLocArr: LocBlockCyclicArr(eltType, rank, idxType, stridable);

  var pid: int = -1; // privatized object id
}

pragma "auto copy fn"
proc chpl__autoCopy(x: BlockCyclicArr) {
  if !noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: BlockCyclicArr) {
  if !noRefCount {
    var cnt = x.destroyArr();
    if cnt == 0 then
      delete x;
  }
}

proc BlockCyclicArr.dsiGetBaseDom() return dom;

proc BlockCyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      locArr(localeIdx) = new LocBlockCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx), dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

proc BlockCyclicArr.~BlockCyclicArr() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      delete locArr(localeIdx);
    }
  }
}

proc BlockCyclicArr.dsiSupportsPrivatization() param return true;

proc BlockCyclicArr.dsiGetPrivatizeData() return 0;

proc BlockCyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  c.locArr = locArr;
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
    if myLocArr.indexDom.myStarts.member(i) then  // TODO: This could be beefed up; true for indices other than starts
      return myLocArr.this(i);
  }
  //  var loci = dom.dist.idxToLocaleInd(i);
  //  compilerError(typeToString(loci.type));
  //  var desc = locArr(loci);
  //  return locArr(loci)(i);
  return locArr(dom.dist.idxToLocaleInd(i))(i);
}

proc BlockCyclicArr.dsiAccess(i: rank*idxType) ref {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.myStarts.member(i) then
//       return myLocArr.this(i);
//   }
  if rank == 1 {
    return dsiAccess(i(1));
  } else {
    return locArr(dom.dist.idxToLocaleInd(i))(i);
  }
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
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    var low = followThis(i).low * stride;
    var high = followThis(i).high * stride;
    myFollowThis(i) = (low..high by stride) + dom.whole.dim(i).low;
    lowIdx(i) = myFollowThis(i).low;
  }
  const myFollowThisDom = {(...myFollowThis)};

  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  const myLocArr = locArr(dom.dist.idxToLocaleInd(lowIdx));

  //
  // we don't own all the elements we're following
  //
  proc accessHelper(i) ref {
//      if myLocArr.locale == here {
//  local {
//          if myLocArr.locDom.myStarts.member(i) then
//            return myLocArr.this(i);
//        }
//      }
    return dsiAccess(i);
  }
  for i in myFollowThisDom {
    yield accessHelper(i);
  }
}

//
// output array
//
proc BlockCyclicArr.dsiSerialWrite(f: Writer) {
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank).stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:idxType) {
          i(dim) += dom.dsiDim(dim).stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

proc BlockCyclicArr.dsiSlice(d: BlockCyclicDom) {
  var alias = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d);
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      alias.locArr[i] = new LocBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, allocDom=locArr[i].allocDom, indexDom=d.locDoms[i], myElems=>locArr[i].myElems);
    }
  }

  return alias;
}

proc BlockCyclicArr.dsiReindex(dom) {
  compilerError("reindexing not yet implemented for Block-Cyclic");
}

proc BlockCyclicArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc BlockCyclicArr.dsiHasSingleLocalSubdomain() param return false;

// essentially enumerateBlocks()
// basically add blocksize to the start indices
iter BlockCyclicArr.dsiLocalSubdomains() {
  for i in myLocArr.indexDom.myStarts {
    var temp : rank*range(idxType);
    const blockSizes = myLocArr.indexDom.globDom.dist.blocksize;
    const globDims = myLocArr.indexDom.globDom.whole.dims();
    for param j in 1..rank {
      var lo: idxType;
      if rank == 1 then lo = i;
      else lo = i(j);
      temp(j) = lo .. min(lo + blockSizes(j)-1, globDims(j).high);
    }
    yield {(...temp)};
  }
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
  const allocDom: LocBlockCyclicDom(rank, idxType, stridable);
  const indexDom: LocBlockCyclicDom(rank, idxType, stridable);


  // STATE:

  //
  // the block of local array data
  //
  var myElems: [allocDom.myFlatInds] eltType;

  // TODO: need to be able to access these, but is this the right place?
  const blocksize: [1..rank] int = [d in 1..rank] allocDom.globDom.dist.blocksize(d);
  const low = allocDom.globDom.dsiLow;
  const locsize: [1..rank] int = [d in 1..rank] allocDom.globDom.dist.targetLocDom.dim(d).length;
  const numblocks: [1..rank] int = [d in 1..rank] (allocDom.myStarts.dim(d).length);

}


proc LocBlockCyclicArr.mdInd2FlatInd(i: ?t, dim = 1) where t == idxType {
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
    var blkmults = * scan [d in 1..rank] blocksize(d);
    //    writeln("blkmults = ", blkmults);
    var numwholeblocks = 0;
    var blkOff = 0;
    for param d in rank..1 by -1 {
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
    //TODO: want negative scan: var blkmults = * scan [d in 1..rank] blocksize(d);
    var blkmults: [1..rank] int;
    blkmults(rank) = blocksize(rank);
    for d in rank-1..1 by -1 do
      blkmults(d) = blkmults(d+1) * blocksize(d);
    //    writeln("blkmults = ", blkmults);
    var numwholeblocks = 0;
    var blkOff = 0;
    for param d in 1..rank {
      const blksize = blocksize(d);
      const ind0 = (i(d) - low(d)): int;
      const blkNum = ind0 / (blksize * locsize(d));
      const blkDimOff = ind0 % blksize;
      if (d != 1) {
        numwholeblocks *= numblocks(rank-d+2);
        blkOff *= blkmults(rank-d+2);
      }
      numwholeblocks += blkNum;
      blkOff += blkDimOff;
      if (false && (i == (13,0) || i == (1,32))) {
          writeln(here.id, ":", "blksize = ", blksize);
          writeln(here.id, ":", "ind0 = ", ind0);
          writeln(here.id, ":", "blkNum = ", blkNum);
          writeln(here.id, ":", "blkDimOff = ", blkDimOff);
        }
    }

    if (false && (i == (13,0) || i == (1,32))) {
      writeln(here.id, ":", "numblocks = ", numblocks);
      writeln(here.id, ":", i, "->"); 
      writeln(here.id, ":","numwholeblocks = ", numwholeblocks);
      writeln(here.id, ":","blkOff = ", blkOff);
      writeln(here.id, ":","total = ", numwholeblocks * blkmults(1) + blkOff);
    }
    return (numwholeblocks * blkmults(1)) + blkOff;
  }
}

//
// the accessor for the local array -- assumes the index is local
//
proc LocBlockCyclicArr.this(i) ref {
  const flatInd = mdInd2FlatInd(i);
  //    writeln(i, "->", flatInd);
  return myElems(flatInd);
}

//
// output local array piece
//
proc LocBlockCyclicArr.writeThis(x: Writer) {
  // note on this fails; see writeThisUsingOn.chpl
  x.write(myElems);
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
