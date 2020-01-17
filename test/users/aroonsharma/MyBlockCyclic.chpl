/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

config const debugzipopt = false;
config const testzipopt = false; //not supported yet
config var totalcomm3 = false; //do count total communication volume
var total_communication_counts:[1..numLocales]int;


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


config param debugMyBlockCyclicDist = false; // internal development flag (debugging)

use BlockCycDist;
// proc _determineRankFromArg(startIdx) param {
//   return if isTuple(startIdx) then startIdx.size else 1;
// }
// 
// proc _determineIdxTypeFromArg(startIdx) type {
//   return if isTuple(startIdx) then startIdx(1).type else startIdx.type;
// }
// 
// proc _ensureTuple(arg) {
//   return if isTuple(arg) then arg else (arg,);
// }


////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Distribution Class
//
class MyBlockCyclic : BaseDist {
  param rank: int;
  type idxType = int;

  const lowIdx: rank*idxType;
  const blocksize: rank*int;
  const targetLocDom: domain(rank);
  const targetLocales: [targetLocDom] locale;
  const locDist: [targetLocDom] LocMyBlockCyclic(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  proc MyBlockCyclic(startIdx,  // ?nd*?idxType
                   blocksize,     // nd*int
                   targetLocales: [] locale = Locales, 
                   tasksPerLocale = 0,
                   param rank: int = _determineRankFromArg(startIdx),
                   type idxType = _determineIdxTypeFromArg(startIdx))
  {
    // argument sanity checks, with friendly error messages
    if isTuple(startIdx) != isTuple(blocksize) then compilerError("when invoking MyBlockCyclic constructor, startIdx and blocksize must be either both tuples or both integers");
    if isTuple(startIdx) && startIdx.size != blocksize.size then compilerError("when invoking MyBlockCyclic constructor and startIdx and blocksize are tuples, their sizes must match");
    if !_isIntegralType(idxType) then compilerError("when invoking MyBlockCyclic constructor, startIdx must be an integer or a tuple of integers");
    if !_isIntegralType(_determineIdxTypeFromArg(blocksize)) then compilerError("when invoking MyBlockCyclic constructor, blocksize must be an integer or a tuple of integers");

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
      if debugMyBlockCyclicDist {
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
      if debugMyBlockCyclicDist then writeln(targetLocDom);

      this.targetLocales = reshape(targetLocales, targetLocDom);
      if debugMyBlockCyclicDist then writeln(this.targetLocales);
    }

    coforall locid in targetLocDom do
      on this.targetLocales(locid) do
        locDist(locid) = new LocMyBlockCyclic(rank, idxType, locid, this);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = 1;   // TODO: here.numPUs() or here.maxTaskPar?
    else
      this.tasksPerLocale = tasksPerLocale;

    if debugMyBlockCyclicDist then
      for loc in locDist do writeln(loc);
  }

  // copy constructor for privatization
  proc MyBlockCyclic(param rank: int, type idxType, other: MyBlockCyclic(rank, idxType)) {
    lowIdx = other.lowIdx;
    blocksize = other.blocksize;
    targetLocDom = other.targetLocDom;
    targetLocales = other.targetLocales;
    locDist = other.locDist;
    tasksPerLocale = other.tasksPerLocale;
  }

  proc dsiClone() {
    return new MyBlockCyclic(lowIdx, blocksize, targetLocales, tasksPerLocale);
  }
}

//
// create a new rectangular domain over this distribution
//
proc MyBlockCyclic.dsiNewRectangularDom(param rank: int, type idxType,
                           param stridable: bool) {
  if idxType != this.idxType then
    compilerError("MyBlockCyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("MyBlockCyclic domain rank does not match distribution's");

  var dom = new MyBlockCyclicDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  return dom;
}

//
// output distribution
//
proc MyBlockCyclic.writeThis(x:Writer) {
  x.writeln("MyBlockCyclic");
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
proc MyBlockCyclic.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(idxToLocaleInd(ind));
}

proc MyBlockCyclic.dsiIndexToLocale(ind: rank*idxType) {
  return targetLocales(idxToLocaleInd(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc MyBlockCyclic.getStarts(inds, locid) {
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
proc MyBlockCyclic.idxToLocaleInd(ind: idxType) where rank == 1 {
  const ind0 = ind - lowIdx(1);
  //  compilerError((ind0/blocksize(1)%targetLocDom.dim(1).type):string);
  return (ind0 / blocksize(1)) % targetLocDom.dim(1).length;
}

proc MyBlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank == 1 {
  return idxToLocaleInd(ind(1));
}

proc MyBlockCyclic.idxToLocaleInd(ind: rank*idxType) where rank != 1 {
  var locInd: rank*int;
  for param i in 1..rank {
    const ind0 = ind(i) - lowIdx(i);
    locInd(i) = ((ind0 / blocksize(i)) % targetLocDom.dim(i).length): int; 
  }
  return locInd;
}

////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Local Distribution Class
//
class LocMyBlockCyclic {
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
  proc LocMyBlockCyclic(param rank: int,
                 type idxType, 
                 locid,   // the locale index from the target domain
                 dist: MyBlockCyclic(rank, idxType)) { // reference to glob dist
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


proc LocMyBlockCyclic.writeThis(x:Writer) {
  var localeid: int;
  on this {
    localeid = here.id;
  }
  x.write("locale ", localeid, " owns blocks: ", myStarts);
}

////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Domain Class
//
class MyBlockCyclicDom: BaseRectangularDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: MyBlockCyclic(rank, idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  var locDoms: [dist.targetLocDom] LocMyBlockCyclicDom(rank, idxType, stridable);

  //
  // a domain describing the complete domain
  //
  const whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  //  const startLoc: index(dist.targetLocDom);

  var pid: int = -1; // privatized object id
}

proc MyBlockCyclicDom.dsiDims() return whole.dims();

proc MyBlockCyclicDom.dsiDim(d: int) return whole.dim(d);

iter MyBlockCyclicDom.these() {
  for i in whole do
    yield i;
}

//qwerty
//since only 1d non strided is implemented in the rest of block cyclyc, only implement 1d for now
//change to yield blocks to yielding strides
iter MyBlockCyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  var blocksize=dist.blocksize(1);
  var cyclesize=locDoms.size;
  var blockcyclesize=blocksize*cyclesize; //same as locDom.myStarts.stride
  coforall locDom in locDoms do on locDom {
//    writeln(locDom.myStarts);
//     writeln(dist.blocksize(1));

//begin new code
    var tasks=here.numPUs();
    coforall core in 0..tasks-1 do
    for i in core..blocksize-1 by tasks {
      var ind=locDom.myStarts.low+i;
      var retstride: rank*range(idxType, stridable=true);
//       writeln("whole.low=", whole.low, " whole.dim=", whole.dim(1)); //.size
//       writeln("pattern size=", locDoms.size);
      //retstride(1) = locDom.myStarts.low+i .. locDom.myStarts.high+i by locDom.myStarts.stride;
      retstride(1) = (ind+blockcyclesize-whole.low%blockcyclesize)%blockcyclesize..whole.dim(1).size-1 by locDom.myStarts.stride;
//      writeln(retstride(1));
      yield(retstride);
    }
    
//end new code, begin old code
//    var tmpblock:range(idxType, stridable=stridable);
//     for i in locDom.myStarts {
//       //      writeln("[", here.id, "] starting at ", i);
//       var lo: idxType;
//       lo = i;
//       tmpblock = max(lo, whole.dim(1).low)..
//       min(lo + dist.blocksize(1)-1, whole.dim(1).high);
//       //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
//       tmpblock = whole.dim(1)[tmpblock];
//       //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
//       lo = whole.low;
//       //        writeln("lo = ", lo);
//       tmpblock = tmpblock.chpl__unTranslate(lo);
//       //        writeln("[", here.id, "] tmpblock(j) = ", tmpblock(j));
// 
//       var retblock: rank*range(idxType);
//       retblock(1) = (tmpblock.low / whole.dim(1).stride:idxType)..#tmpblock.length;
//       //        retblock(i) = (tmpblock(i) - whole.dim(i).low);
//       //      writeln(here.id, ": Domain leader yielding", retblock);
//       yield retblock;
//     }
//end old code

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
//DS this is probably broken after changing leader iterator (not used in benchmarks)
iter MyBlockCyclicDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
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
proc MyBlockCyclicDom.dsiSerialWrite(x:Writer) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
proc MyBlockCyclicDom.dsiBuildArray(type eltType) {
  var arr = new MyBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

proc MyBlockCyclicDom.dsiNumIndices return whole.numIndices;
proc MyBlockCyclicDom.dsiLow return whole.low;
proc MyBlockCyclicDom.dsiHigh return whole.high;
proc MyBlockCyclicDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make setIndices() for a rectangular
// domain take a domain rather than something else?
//
proc MyBlockCyclicDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
}

proc MyBlockCyclicDom.dsiSetIndices(x) {
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

proc MyBlockCyclicDom.dsiGetIndices() {
  return whole.getIndices();
}

proc MyBlockCyclicDom.dsiMyDist() return dist;

proc MyBlockCyclicDom.setup() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocales(localeIdx) do
      if (locDoms(localeIdx) == nil) then
        locDoms(localeIdx) = new LocMyBlockCyclicDom(rank, idxType, stridable, this, 
                                                   dist.getStarts(whole, localeIdx));
      else {
        locDoms(localeIdx).myStarts = dist.getStarts(whole, localeIdx);
        locDoms(localeIdx).myFlatInds = {0..#locDoms(localeIdx).computeFlatInds()};
      }
  if debugMyBlockCyclicDist then
    enumerateBlocks();
}

proc MyBlockCyclicDom.enumerateBlocks() {
  for locidx in dist.targetLocDom {
    on dist.targetLocales(locidx) do locDoms(locidx).enumerateBlocks();
  }
}

proc MyBlockCyclicDom.dsiSupportsPrivatization() param return true;

proc MyBlockCyclicDom.dsiGetPrivatizeData() return 0;

proc MyBlockCyclicDom.dsiPrivatize(privatizeData) {
  var privateDist = new MyBlockCyclic(rank, idxType, dist);
  var c = new MyBlockCyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privateDist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

proc MyBlockCyclicDom.dsiGetReprivatizeData() return 0;

proc MyBlockCyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = other.whole;
}

proc MyBlockCyclicDom.dsiMember(i) {
  return whole.contains(i);
}

proc MyBlockCyclicDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

proc MyBlockCyclicDom.dsiBuildRectangularDom(param rank: int, type idxType,
                                         param stridable: bool,
                                         ranges: rank*range(idxType,
                                                            BoundedRangeType.bounded,
                                                            stridable)) {
  if idxType != dist.idxType then
    compilerError("MyBlockCyclic domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("MyBlockCyclic domain rank does not match distribution's");

  var dom = new MyBlockCyclicDom(rank=rank, idxType=idxType,
                               dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;
}


////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Local Domain Class
//
class LocMyBlockCyclicDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // UP LINK: a reference to the parent global domain class
  //
  const globDom: MyBlockCyclicDom(rank, idxType, stridable);

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
proc LocMyBlockCyclicDom.computeFlatInds() {
  //  writeln("myStarts = ", myStarts);
  const numBlocks = * reduce [d in 1..rank] (myStarts.dim(d).length),
    indsPerBlk = * reduce [d in 1..rank] (globDom.dist.blocksize(d));
  //  writeln("Total number of inds = ", numBlocks * indsPerBlk);
  return numBlocks * indsPerBlk;
}

//
// output local domain piece
//
proc LocMyBlockCyclicDom.writeThis(x:Writer) {
  x.write(myStarts);
}

proc LocMyBlockCyclicDom.enumerateBlocks() {
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
proc LocMyBlockCyclicDom.numIndices {
  return myStarts.numIndices;
}

proc LocMyBlockCyclicDom.low {
  return myStarts.low;
}

proc LocMyBlockCyclicDom.high {
  return myStarts.high;
}

////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Array Class
//
class MyBlockCyclicArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: MyBlockCyclicDom(rank, idxType, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  var locArr: [dom.dist.targetLocDom] LocMyBlockCyclicArr(eltType, rank, idxType, stridable);

  //
  // optimized reference to a local LocMyBlockCyclicArr instance (or nil)
  //
  var myLocArr: LocMyBlockCyclicArr(eltType, rank, idxType, stridable);

  var pid: int = -1; // privatized object id
}

proc MyBlockCyclicArr.dsiGetBaseDom() return dom;

proc MyBlockCyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      locArr(localeIdx) = new LocMyBlockCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx), dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

proc MyBlockCyclicArr.dsiSupportsPrivatization() param return true;

proc MyBlockCyclicArr.dsiGetPrivatizeData() return 0;

proc MyBlockCyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new MyBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
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
proc MyBlockCyclicArr.dsiAccess(i: idxType) var where rank == 1 {
  if myLocArr then /* TODO: reenable */ /* local */ {
    if myLocArr.indexDom.myStarts.contains(i) then  // TODO: This could be beefed up; true for indices other than starts
      return myLocArr.this(i);
  }
  //  var loci = dom.dist.idxToLocaleInd(i);
  //  compilerError(loci.type:string);
  //  var desc = locArr(loci);
  //  return locArr(loci)(i);
  return locArr(dom.dist.idxToLocaleInd(i))(i);
}

proc MyBlockCyclicArr.dsiAccess(i: rank*idxType) var {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.myStarts.contains(i) then
//       return myLocArr.this(i);
//   }
  if rank == 1 {
    return dsiAccess(i(1));
  } else {
    return locArr(dom.dist.idxToLocaleInd(i))(i);
  }
}


iter MyBlockCyclicArr.these() var {
  for i in dom do
    yield dsiAccess(i);
}

iter MyBlockCyclicArr.these(param tag: iterKind) where tag == iterKind.leader {
  for yieldThis in dom.these(tag) do
    yield yieldThis;
}

//asdf
iter MyBlockCyclicArr.these(param tag: iterKind, followThis) var where tag == iterKind.follower {
  var myFollowThis: rank*range(idxType=idxType, stridable=true);
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    var low = followThis(i).low * stride;
    var high = followThis(i).high * stride;
    myFollowThis(i) = (low..high by stride*followThis(i).stride) + dom.whole.dim(i).low;
    lowIdx(i) = myFollowThis(i).low;
  }
  const myFollowThisDom = {(...myFollowThis)};
  
  if debugzipopt then writeln(followThis, ' ', myFollowThisDom, ' ', dom.whole.dim(1));
  //writeln(numLocales);

  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  const myLocArr = locArr(dom.dist.idxToLocaleInd(lowIdx));

  //if local todo
  if myLocArr.locale.id == here.id { //if mem is local
    if debugzipopt then writeln("mem is local ", here.id, " ", myFollowThisDom);
    local {
      for i in myFollowThisDom {
        yield myLocArr.this(i);
      }
    }
    return;
  }
  if debugzipopt then writeln("Nonlocal ", here.id, " ", myFollowThisDom);

    var blocksize=dist.blocksize(1);
    var cyclesize=locDoms.size;
    var blockcyclesize=blocksize*cyclesize;
  //if can do optimization (stepsize, blocksize*procs)
  //todo skipping this check
  //implemented purely for benchmarks, which will always have this condition met
  if true {
  
    var bufsize=myFollowThisDom.size;
    var buf: [1..bufsize] this.eltType;
    
    
    var dest = buf._value.theData;
    var src = myLocArr.myElems._value.theData;
    const rid=myLocArr.locale.id;


    var dstStride: [1..rank] int(32);
    var srcStride: [1..rank] int(32);
    var count: [1..rank+1] int(32);
    var stridelevels=rank:int(32);
    
    count(1)=1:int(32);
    count(2)=bufsize:int(32);
    dstStride(1)=1:int(32);
    srcStride(1)=dom.dist.blocksize(1):int(32);
    
    var dststr=dstStride._value.theData;
    var srcstr=srcStride._value.theData;
    var cnt=count._value.theData;
    
    //writeln(bufsize);
    //copy remote data to local buffer (todo don't do if not used, need to modify chapel compiler to check)
    __primitive("chpl_comm_get_strd",
      __primitive("array_get", dest, buf._value.getDataIndex(1)),
      __primitive("array_get",dststr,dstStride._value.getDataIndex(1)), 
      rid,
      __primitive("array_get", src, myLocArr.myElems._value.getDataIndex(myLocArr.mdInd2FlatInd(myFollowThisDom.low))),
      __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)),
      __primitive("array_get",cnt, count._value.getDataIndex(1)),
      stridelevels);
    var hereid=here.id;
    if totalcomm3 then on Locales[0] do total_communication_counts[hereid+1]+=bufsize;

    var changed=false;
    for i in buf {
      var old_val=i;
      yield i;
      changed |= i!=old_val;
    }
    
    if changed { //copy back incase they modified it
      __primitive("chpl_comm_put_strd",
        __primitive("array_get", src, myLocArr.myElems._value.getDataIndex(myLocArr.mdInd2FlatInd(myFollowThisDom.low))),
        __primitive("array_get",srcstr,srcStride._value.getDataIndex(1)), 
        rid,
        __primitive("array_get", dest, buf._value.getDataIndex(1)),
        __primitive("array_get",dststr,dstStride._value.getDataIndex(1)),
        __primitive("array_get",cnt, count._value.getDataIndex(1)),
        stridelevels);
      if totalcomm3 then on Locales[0] do total_communication_counts[hereid+1]+=bufsize;
    }
  }
  
  else {
    if debugzipopt then writeln('unable to do opt ', here.id);
    proc accessHelper(i) var {
      return dsiAccess(i);
    }
    for i in myFollowThisDom {
      yield accessHelper(i);
    }
  }
}

//
// output array
//
proc MyBlockCyclicArr.dsiSerialWrite(f: Writer) {
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

proc MyBlockCyclicArr.dsiSlice(d: MyBlockCyclicDom) {
  var alias = new MyBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      alias.locArr[i] = new LocMyBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, allocDom=locArr[i].allocDom, indexDom=d.locDoms[i], myElems=>locArr[i].myElems);
    }
  }

  return alias;
}

proc MyBlockCyclicArr.dsiReindex(dom) {
  compilerError("reindexing not yet implemented for Block-Cyclic");
}

////////////////////////////////////////////////////////////////////////////////
// MyBlockCyclic Local Array Class
//
class LocMyBlockCyclicArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: a reference to the local domain class for this array and locale
  //
  const allocDom: LocMyBlockCyclicDom(rank, idxType, stridable);
  const indexDom: LocMyBlockCyclicDom(rank, idxType, stridable);


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


proc LocMyBlockCyclicArr.mdInd2FlatInd(i: ?t, dim = 1) where t == idxType {
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

proc LocMyBlockCyclicArr.mdInd2FlatInd(i: ?t) where t == rank*idxType {
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
proc LocMyBlockCyclicArr.this(i) var {
  const flatInd = mdInd2FlatInd(i);
  //    writeln(i, "->", flatInd);
  return myElems(flatInd);
}

//
// output local array piece
//
proc LocMyBlockCyclicArr.writeThis(x: Writer) {
  // note on this fails; see writeThisUsingOn.chpl
  x.write(myElems);
}

// sungeun: This doesn't appear to be used yet, so I left it, but it
//  might be useful to others.  Consider putting it in DSIUtil.chpl.

//
// helper function for blocking index ranges
//
proc _computeMyBlockCyclic(waylo, numelems, lo, wayhi, numblocks, blocknum) {
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
