/*
 * Copyright 2004-2018 Cray Inc.
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
// The Block distribution is defined with six classes:
//
//   Block       : distribution class
//   BlockDom    : domain class
//   BlockArr    : array class
//   LocBlock    : local distribution class (per-locale instances)
//   LocBlockDom : local domain class (per-locale instances)
//   LocBlockArr : local array class (per-locale instances)
//
// When a distribution, domain, or array class instance is created, a
// corresponding local class instance is created on each locale that is
// mapped to by the distribution.
//

//
// TO DO List
//
// 1. refactor pid fields from distribution, domain, and array classes
//

use DSIUtil;
use ChapelUtil;
use CommDiagnostics;
use SparseBlockDist;
use LayoutCS;
//
// These flags are used to output debug information and run extra
// checks when using Block.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugBlockDist = false;
config param debugBlockDistBulkTransfer = false;

// This flag is used to enable bulk transfer when aliased arrays are
// involved.  Currently, aliased arrays are not eligible for the
// optimization due to a bug in bulk transfer for rank changed arrays
// in which the last (right-most) dimension is collapsed.  Disabling
// the optimization for all aliased arrays is very conservative, so
// we provide this flag to allow the user to override the decision,
// with the caveat that it will likely not work for the above case.
config const disableAliasedBulkTransfer = true;

config param sanityCheckDistribution = false;

//
// If the testFastFollowerOptimization flag is set to true, the
// follower will write output to indicate whether the fast follower is
// used or not.  This is used in regression testing to ensure that the
// 'fast follower' optimization is working.
//
config param testFastFollowerOptimization = false;

//
// This flag is used to disable lazy initialization of the RAD cache.
//
config param disableBlockLazyRAD = defaultDisableLazyRADOpt;

//
// Block Distribution Class
//
//   The fields dataParTasksPerLocale, dataParIgnoreRunningTasks, and
//   dataParMinGranularity can be changed, but changes are
//   not reflected in privatized copies of this distribution.  Perhaps
//   this is a feature, not a bug?!
//
// rank : generic rank that must match the rank of domains and arrays
//        declared over this distribution
//
// idxType: generic index type that must match the index type of
//          domains and arrays declared over this distribution
//
// boundingBox: a non-distributed domain defining a bounding box used
//              to partition the space of all indices across the array
//              of target locales; the indices inside the bounding box
//              are partitioned "evenly" across the locales and
//              indices outside the bounding box are mapped to the
//              same locale as the nearest index inside the bounding
//              box
//
// targetLocDom: a non-distributed domain over which the array of
//               target locales and the array of local distribution
//               classes are defined
//
// targetLocales: a non-distributed array containing the target
//                locales to which this distribution partitions indices
//                and data
//
// locDist: a non-distributed array of local distribution classes
//
// dataParTasksPerLocale: an integer that specifies the number of tasks to
//                        use on each locale when iterating in parallel over
//                        a Block-distributed domain or array
//
// dataParIgnoreRunningTasks: a boolean what dictates whether the number of
//                            task use on each locale should be limited
//                            by the available parallelism
//
// dataParMinGranularity: the minimum required number of elements per
//                        task created
//

// chpldoc TODO:
//   a good reference to
//     dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity
//   remove the above comments to avoid duplication/maintenance?
//   talk about RAD - here or in DefaultRectangular?
//   supports RAD opt, Bulk Transfer optimization, localSubdomain
//   disableBlockLazyRAD
//   disableAliasedBulkTransfer
//
/*
This Block distribution partitions indices into blocks
according to a ``boundingBox`` domain
and maps each entire block onto a locale from a ``targetLocales`` array.

The indices inside the bounding box are partitioned "evenly" across
the target locales. An index outside the bounding box is mapped to the
same locale as the nearest index inside the bounding box.

Formally, an index ``idx`` is mapped to ``targetLocales[locIdx]``,
where ``locIdx`` is computed as follows.

In the 1-dimensional case, for a Block distribution with:


  =================   ====================
  ``boundingBox``     ``{low..high}``
  ``targetLocales``   ``[0..N-1] locale``
  =================   ====================

we have:

  ===================    ==========================================
  if ``idx`` is ...      ``locIdx`` is ...
  ===================    ==========================================
  ``low<=idx<=high``     ``floor(  (idx-low)*N / (high-low+1)  )``
  ``idx < low``          ``0``
  ``idx > high``         ``N-1``
  ===================    ==========================================

In the multidimensional case, ``idx`` and ``locIdx`` are tuples
of indices; ``boundingBox`` and ``targetLocales`` are multi-dimensional;
the above computation is applied in each dimension.


**Example**

The following code declares a domain ``D`` distributed over
a Block distribution with a bounding box equal to the domain ``Space``,
and declares an array ``A`` over that domain.
The `forall` loop sets each array element
to the ID of the locale to which it is mapped.

  .. code-block:: chapel

    use BlockDist;

    const Space = {1..8, 1..8};
    const D: domain(2) dmapped Block(boundingBox=Space) = Space;
    var A: [D] int;

    forall a in A do
      a = a.locale.id;

    writeln(A);

When run on 6 locales, the output is:

  ::

    0 0 0 0 1 1 1 1
    0 0 0 0 1 1 1 1
    0 0 0 0 1 1 1 1
    2 2 2 2 3 3 3 3
    2 2 2 2 3 3 3 3
    2 2 2 2 3 3 3 3
    4 4 4 4 5 5 5 5
    4 4 4 4 5 5 5 5


**Constructor Arguments**

The ``Block`` class constructor is defined as follows:

  .. code-block:: chapel

    proc Block(
      boundingBox: domain,
      targetLocales: [] locale  = Locales, 
      dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
      dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
      dataParMinGranularity     = // value of  dataParMinGranularity      config const,
      param rank                = boundingBox.rank,
      type  idxType             = boundingBox.idxType,
      type  sparseLayoutType    = DefaultDist)

The arguments ``boundingBox`` (a domain) and ``targetLocales`` (an array)
define the mapping of any index of ``idxType`` type to a locale
as described above.

The rank of ``targetLocales`` must match the rank of the distribution,
or be ``1``.  If the rank of ``targetLocales`` is ``1``, a greedy
heuristic is used to reshape the array of target locales so that it
matches the rank of the distribution and each dimension contains an
approximately equal number of indices.

The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
and ``dataParMinGranularity`` set the knobs that are used to
control intra-locale data parallelism for Block-distributed domains
and arrays in the same way that the like-named config constants
control data parallelism for ranges and default-distributed domains
and arrays.

The ``rank`` and ``idxType`` arguments are inferred from the ``boundingBox``
argument unless explicitly set.  They must match the rank and index type of the
domains "dmapped" using that Block instance. If the ``boundingBox`` argument is
a stridable domain, the stride information will be ignored and the
``boundingBox`` will only use the lo..hi bounds.

When a ``sparse subdomain`` is created for a ``Block`` distributed domain, the
``sparseLayoutType`` will be the layout of these sparse domains. The default is
currently coordinate, but :class:`LayoutCS.CS` is an interesting alternative.

**Data-Parallel Iteration**

A `forall` loop over a Block-distributed domain or array
executes each iteration on the locale where that iteration's index
is mapped to.

Parallelism within each locale is guided by the values of
``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``, and
``dataParMinGranularity`` of the respective Block instance.
Updates to these values, if any, take effect only on the locale
where the updates are made.

**Sparse Subdomains**

When a ``sparse subdomain`` is declared as a subdomain to a Block-distributed
domain, the resulting sparse domain will also be Block-distributed. The
sparse layout used in this sparse subdomain can be controlled with the
``sparseLayoutType`` constructor argument to Block.

This example demonstrates a Block-distributed sparse domain and array:

  .. code-block:: chapel

   use BlockDist;

    const Space = {1..8, 1..8};

    // Declare a dense, Block-distributed domain.
    const DenseDom: domain(2) dmapped Block(boundingBox=Space) = Space;

    // Declare a sparse subdomain.
    // Since DenseDom is Block-distributed, SparseDom will be as well.
    var SparseDom: sparse subdomain(DenseDom);

    // Add some elements to the sparse subdomain.
    // SparseDom.bulkAdd is another way to do this that allows more control.
    SparseDom += [ (1,2), (3,6), (5,4), (7,8) ];

    // Declare a sparse array.
    // This array is also Block-distributed.
    var A: [SparseDom] int;

    A = 1;

    writeln( "A[(1, 1)] = ", A[1,1]);
    for (ij,x) in zip(SparseDom, A) {
      writeln( "A[", ij, "] = ", x, " on locale ", x.locale);
    }

   // Results in this output when run on 4 locales:
   // A[(1, 1)] = 0
   // A[(1, 2)] = 1 on locale LOCALE0
   // A[(3, 6)] = 1 on locale LOCALE1
   // A[(5, 4)] = 1 on locale LOCALE2
   // A[(7, 8)] = 1 on locale LOCALE3


*/
class Block : BaseDist {
  param rank: int;
  type idxType = int;
  var boundingBox: domain(rank, idxType);
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] LocBlock(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
  type sparseLayoutType = DefaultDist;
}

//
// Local Block Distribution Class
//
// rank : generic rank that matches Block.rank
// idxType: generic index type that matches Block.idxType
// myChunk: a non-distributed domain that defines this locale's indices
//
class LocBlock {
  param rank: int;
  type idxType;
  const myChunk: domain(rank, idxType);
}

//
// Block Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class BlockDom: BaseRectangularDom {
  type sparseLayoutType;
  const dist: Block(rank, idxType, sparseLayoutType);
  var locDoms: [dist.targetLocDom] LocBlockDom(rank, idxType, stridable);
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
}

//
// Local Block Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// myBlock: a non-distributed domain that defines the local indices
//
class LocBlockDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var myBlock: domain(rank, idxType, stridable);
}

//
// Block Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class BlockArr: BaseRectangularArr {
  type sparseLayoutType;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: BlockDom(rank, idxType, stridable, sparseLayoutType);
  var locArr: [dom.dist.targetLocDom] LocBlockArr(eltType, rank, idxType, stridable);
  pragma "local field"
  var myLocArr: LocBlockArr(eltType, rank, idxType, stridable);
  const SENTINEL = max(rank*idxType);
}

//
// Local Block Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocBlockArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  const locDom: LocBlockDom(rank, idxType, stridable);
  var locRAD: LocRADCache(eltType, rank, idxType, stridable); // non-nil if doRADOpt=true
  pragma "local field"
  var myElems: [locDom.myBlock] eltType;
  var locRADLock: atomicbool; // This will only be accessed locally
                              // force the use of processor atomics

  // These functions will always be called on this.locale, and so we do
  // not have an on statement around the while loop below (to avoid
  // the repeated on's from calling testAndSet()).
  inline proc lockLocRAD() {
    while locRADLock.testAndSet() do chpl_task_yield();
  }

  inline proc unlockLocRAD() {
    locRADLock.clear();
  }

  proc deinit() {
    if locRAD != nil then
      delete locRAD;
  }
}

//
// Block constructor for clients of the Block distribution
//
proc Block.Block(boundingBox: domain,
                targetLocales: [] locale = Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.idxType,
                type sparseLayoutType = DefaultDist) {
  if rank != boundingBox.rank then
    compilerError("specified Block rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified Block index type != index type of specified bounding box");
  if rank != 2 && isCSType(sparseLayoutType) then 
    compilerError("CS layout is only supported for 2 dimensional domains");

  if boundingBox.size == 0 then
    halt("Block() requires a non-empty boundingBox");

  this.boundingBox = boundingBox : domain(rank, idxType, stridable = false);

  setupTargetLocalesArray(targetLocDom, this.targetLocales, targetLocales);

  const boundingBoxDims = this.boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDist(locid) =  new LocBlock(rank, idxType, locid, boundingBoxDims,
                                     targetLocDomDims);

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  if debugBlockDist {
    writeln("Creating new Block distribution:");
    dsiDisplayRepresentation();
  }
}

proc Block.dsiAssign(other: this.type) {
  coforall locid in targetLocDom do
    on targetLocales(locid) do
      delete locDist(locid);
  boundingBox = other.boundingBox;
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
  const boundingBoxDims = boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();

  coforall locid in targetLocDom do
    on targetLocales(locid) do
      locDist(locid) = new LocBlock(rank, idxType, locid, boundingBoxDims,
                                    targetLocDomDims);
}

//
// Block distributions are equivalent if they share the same bounding
// box and target locale set.
//
proc Block.dsiEqualDMaps(that: Block(?)) {
  return (this.boundingBox == that.boundingBox &&
          this.targetLocales.equals(that.targetLocales));
}

//
// Block distributions are not equivalent to other domain maps.
//
proc Block.dsiEqualDMaps(that) param {
  return false;
}

proc Block.dsiClone() {
  return new Block(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity,
                   rank,
                   idxType,
                   sparseLayoutType);
}

proc Block.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

proc Block.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

proc Block.dsiNewRectangularDom(param rank: int, type idxType,
                                param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Block domain rank does not match distribution's");

  var dom = new BlockDom(rank=rank, idxType=idxType, dist=this,
      stridable=stridable, sparseLayoutType=sparseLayoutType);
  dom.dsiSetIndices(inds);
  if debugBlockDist {
    writeln("Creating new Block domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

proc Block.dsiNewSparseDom(param rank: int, type idxType, dom: domain) {
  return new SparseBlockDom(rank=rank, idxType=idxType,
                            sparseLayoutType=sparseLayoutType,
                            stridable=dom.stridable,
                            dist=this, whole=dom._value.whole,
                            parentDom=dom);
}

//
// output distribution
//
proc Block.writeThis(x) {
  x <~> "Block" <~> "\n";
  x <~> "-------" <~> "\n";
  x <~> "distributes: " <~> boundingBox <~> "\n";
  x <~> "across locales: " <~> targetLocales <~> "\n";
  x <~> "indexed via: " <~> targetLocDom <~> "\n";
  x <~> "resulting in: " <~> "\n";
  for locid in targetLocDom do
    x <~> "  [" <~> locid <~> "] locale " <~> locDist(locid).locale.id <~>
      " owns chunk: " <~> locDist(locid).myChunk <~> "\n";
}

proc Block.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

proc Block.dsiIndexToLocale(ind: rank*idxType) {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc Block.getChunk(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  if sanityCheckDistribution then
    if chunk.numIndices > 0 {
      if targetLocsIdx(chunk.low) != locid then
        writeln("[", here.id, "] ", chunk.low, " is in my chunk but maps to ",
                targetLocsIdx(chunk.low));
      if targetLocsIdx(chunk.high) != locid then
        writeln("[", here.id, "] ", chunk.high, " is in my chunk but maps to ",
                targetLocsIdx(chunk.high));
    }
  return chunk;
}

//
// get the index into the targetLocales array for a given distributed index
//
proc Block.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx((ind,));
}

proc Block.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 1..rank do
    result(i) = max(0, min((targetLocDom.dim(i).length-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).length:idxType) /
                            boundingBox.dim(i).length):int));
  return if rank == 1 then result(1) else result;
}

proc LocBlock.LocBlock(param rank: int,
                      type idxType, 
                      locid, // the locale index from the target domain
                      boundingBox: rank*range(idxType),
                      targetLocBox: rank*range) {
  if rank == 1 {
    const lo = boundingBox(1).low;
    const hi = boundingBox(1).high;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox(1).length;
    const (blo, bhi) = _computeBlock(numelems, numlocs, locid,
                                     max(idxType), min(idxType), lo);
    myChunk = {blo..bhi};
  } else {
    var inds: rank*range(idxType);
    for param i in 1..rank {
      const lo = boundingBox(i).low;
      const hi = boundingBox(i).high;
      const numelems = hi - lo + 1;
      const numlocs = targetLocBox(i).length;
      const (blo, bhi) = _computeBlock(numelems, numlocs, locid(i),
                                       max(idxType), min(idxType), lo);
      inds(i) = blo..bhi;
    }
    myChunk = {(...inds)};
  }
}


proc BlockDom.dsiMyDist() return dist;

proc BlockDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
}

proc BlockDom.dsiDims() return whole.dims();

proc BlockDom.dsiDim(d: int) return whole.dim(d);

// stopgap to avoid accessing locDoms field (and returning an array)
proc BlockDom.getLocDom(localeIdx) return locDoms(localeIdx);


//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int(32)), int(32), (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int(32), range(int(32)), range(int(32)))
//
proc _matchArgsShape(type rangeType, type scalarType, args) type {
  proc helper(param i: int) type {
    if i == args.size {
      if isCollapsedDimension(args(i)) then
        return (scalarType,);
      else
        return (rangeType,);
    } else {
      if isCollapsedDimension(args(i)) then
        return (scalarType, (... helper(i+1)));
      else
        return (rangeType, (... helper(i+1)));
    }
  }
  return helper(1);
}


iter BlockDom.these() {
  for i in whole do
    yield i;
}

iter BlockDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const wholeLow = whole.low;

  // If this is the only task running on this locale, we don't want to
  // count it when we try to determine how many tasks to use.  Here we
  // check if we are the only one running, and if so, use
  // ignoreRunning=true for this locale only.  Obviously there's a bit
  // of a race condition if some other task starts after we check, but
  // in that case there is no correct answer anyways.
  //
  // Note that this code assumes that any locale will only be in the
  // targetLocales array once.  If this is not the case, then the
  // tasks on this locale will *all* ignoreRunning, which may have
  // performance implications.
  const hereId = here.id;
  const hereIgnoreRunning = if here.runningTasks() == 1 then true
                            else ignoreRunning;
  coforall locDom in locDoms do on locDom {
    const myIgnoreRunning = if here.id == hereId then hereIgnoreRunning
      else ignoreRunning;
    // Use the internal function for untranslate to avoid having to do
    // extra work to negate the offset
    type strType = chpl__signedType(idxType);
    const tmpBlock = locDom.myBlock.chpl__unTranslate(wholeLow);
    var locOffset: rank*idxType;
    for param i in 1..tmpBlock.rank {
      const stride = tmpBlock.dim(i).stride;
      if stride < 0 && strType != idxType then
        halt("negative stride not supported with unsigned idxType");
        // (since locOffset is unsigned in that case)
      locOffset(i) = tmpBlock.dim(i).first / stride:idxType;
    }
    // Forward to defaultRectangular
    for followThis in tmpBlock._value.these(iterKind.leader, maxTasks,
                                            myIgnoreRunning, minSize,
                                            locOffset) do
      yield followThis;
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
// TODO: Can we just re-use the DefaultRectangularDom follower here?
//
iter BlockDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag then
    chpl__testParWriteln("Block domain follower invoked on ", followThis);

  var t: rank*range(idxType, stridable=stridable||anyStridable(followThis));
  type strType = chpl__signedType(idxType);
  for param i in 1..rank {
    var stride = whole.dim(i).stride: strType;
    // not checking here whether the new low and high fit into idxType
    var low = (stride * followThis(i).low:strType):idxType;
    var high = (stride * followThis(i).high:strType):idxType;
    t(i) = ((low..high by stride:strType) + whole.dim(i).alignedLow by followThis(i).stride:strType).safeCast(t(i).type);
  }
  for i in {(...t)} {
    yield i;
  }
}

//
// output domain
//
proc BlockDom.dsiSerialWrite(x) {
  x <~> whole;
}

//
// how to allocate a new array over this domain
//
proc BlockDom.dsiBuildArray(type eltType) {
  var arr = new BlockArr(eltType=eltType, rank=rank, idxType=idxType,
      stridable=stridable, sparseLayoutType=sparseLayoutType, dom=this);
  arr.setup();
  return arr;
}

proc BlockDom.dsiNumIndices return whole.numIndices;
proc BlockDom.dsiLow return whole.low;
proc BlockDom.dsiHigh return whole.high;
proc BlockDom.dsiStride return whole.stride;
proc BlockDom.dsiAlignedLow return whole.alignedLow;
proc BlockDom.dsiAlignedHigh return whole.alignedHigh;
proc BlockDom.dsiAlignment return whole.alignment;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for a rectangular
// domain take a domain rather than something else?
//
proc BlockDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
  if debugBlockDist {
    writeln("Setting indices of Block domain:");
    dsiDisplayRepresentation();
  }
}

proc BlockDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
  if debugBlockDist {
    writeln("Setting indices of Block domain:");
    dsiDisplayRepresentation();
  }
}

proc BlockDom.dsiGetIndices() {
  return whole.getIndices();
}

proc BlockDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

// dsiLocalSlice
proc BlockDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

proc BlockDom.setup() {
  if locDoms(dist.targetLocDom.low) == nil {
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocales(localeIdx) do
        locDoms(localeIdx) = new LocBlockDom(rank, idxType, stridable,
                                             dist.getChunk(whole, localeIdx));
    }
  } else {
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocales(localeIdx) do
        locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    }
  }
}

proc BlockDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom do {
    on locDoms(localeIdx) do
      delete locDoms(localeIdx);
  }
}

proc BlockDom.dsiMember(i) {
  return whole.member(i);
}

proc BlockDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocBlockDom.member(i) return myBlock.member(i);

proc BlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
  }
}

proc BlockArr.dsiGetBaseDom() return dom;

//
// NOTE: Each locale's myElems array must be initialized prior to
// setting up the RAD cache.
//
proc BlockArr.setupRADOpt() {
  for localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const myLocArr = locArr(localeIdx);
      if myLocArr.locRAD != nil {
        delete myLocArr.locRAD;
        myLocArr.locRAD = nil;
      }
      if disableBlockLazyRAD {
        myLocArr.locRAD = new LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
        for l in dom.dist.targetLocDom {
          if l != localeIdx {
            myLocArr.locRAD.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
          }
        }
      }
    }
  }
}

proc BlockArr.setup() {
  var thisid = this.locale.id;
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const locDom = dom.getLocDom(localeIdx);
      locArr(localeIdx) = new LocBlockArr(eltType, rank, idxType, stridable, locDom);
      if thisid == here.id then
        myLocArr = locArr(localeIdx);
    }
  }

  if doRADOpt && disableBlockLazyRAD then setupRADOpt();
}

proc BlockArr.dsiDestroyArr() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      delete locArr(localeIdx);
    }
  }
}

inline proc BlockArr.dsiLocalAccess(i: rank*idxType) ref {
  return myLocArr.this(i);
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in targetLocsIdx?
//
// By splitting the non-local case into its own function, we can inline the
// fast/local path and get better performance.
//
inline proc BlockArr.dsiAccess(const in idx: rank*idxType) ref {
  local {
    if myLocArr != nil && myLocArr.locDom.member(idx) then
      return myLocArr.this(idx);
  }
  return nonLocalAccess(idx);
}

proc BlockArr.nonLocalAccess(i: rank*idxType) ref {
  if doRADOpt {
    if myLocArr {
      if boundsChecking then
        if !dom.dsiMember(i) then
          halt("array index out of bounds: ", i);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableBlockLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.lockLocRAD();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            myLocArr.locRAD = tempLocRAD;
          }
          myLocArr.unlockLocRAD();
        }
        if myLocArr.locRAD.RAD(rlocIdx).blk == SENTINEL {
          myLocArr.locRAD.lockRAD(rlocIdx);
          if myLocArr.locRAD.RAD(rlocIdx).blk == SENTINEL {
            myLocArr.locRAD.RAD(rlocIdx) =
              locArr(rlocIdx).myElems._value.dsiGetRAD();
          }
          myLocArr.locRAD.unlockRAD(rlocIdx);
        }
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr.locRAD;
      pragma "no copy" pragma "no auto destroy" var radata = myLocRAD.RAD;
      if radata(rlocIdx).shiftedData != nil {
        var dataIdx = radata(rlocIdx).getDataIndex(i);
        return radata(rlocIdx).getDataElem(dataIdx);
      }
    }
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

proc BlockArr.dsiAccess(i: idxType...rank) ref
  return dsiAccess(i);

iter BlockArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
iter BlockArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}

proc BlockArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

proc BlockArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain._value == this.dom;

proc BlockArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead._value == this.dom;

iter BlockArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag {
    if fast then
      chpl__testParWriteln("Block array fast follower invoked on ", followThis);
    else
      chpl__testParWriteln("Block array non-fast follower invoked on ", followThis);
  }

  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Block array");

  var myFollowThis: rank*range(idxType=idxType, stridable=stridable || anyStridable(followThis));
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    // NOTE: Not bothering to check to see if these can fit into idxType
    var low = followThis(i).low * abs(stride):idxType;
    var high = followThis(i).high * abs(stride):idxType;
    myFollowThis(i) = ((low..high by stride) + dom.whole.dim(i).alignedLow by followThis(i).stride).safeCast(myFollowThis(i).type);
    lowIdx(i) = myFollowThis(i).low;
  }

  const myFollowThisDom = {(...myFollowThis)};
  if fast {
    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArr/locDoms arrays should be associative over locale values.
    //
    var arrSection = locArr(dom.dist.targetLocsIdx(lowIdx));

    //
    // if arrSection is not local and we're using the fast follower,
    // it means that myFollowThisDom is empty; make arrSection local so
    // that we can use the local block below
    //
    if arrSection.locale.id != here.id then
      arrSection = myLocArr;

    //
    // Slicing arrSection.myElems will require reference counts to be updated.
    // If myElems is an array of arrays, the inner array's domain or dist may
    // live on a different locale and require communication for reference
    // counting. Simply put: don't slice inside a local block.
    //
    ref chunk = arrSection.myElems(myFollowThisDom);
    local {
      for i in chunk do yield i;
    }
  } else {
    //
    // we don't necessarily own all the elements we're following
    //
    for i in myFollowThisDom {
      yield dsiAccess(i);
    }
  }
}

proc BlockArr.dsiSerialRead(f) {
  chpl_serialReadWriteRectangular(f, this);
}

//
// output array
//
proc BlockArr.dsiSerialWrite(f) {
  type strType = chpl__signedType(idxType);
  var binary = f.binary();
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f <~> dsiAccess(i);
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:strType) {
      if ! binary then f <~> " ";
      i(rank) += dom.dsiDim(rank).stride:strType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:strType) {
          i(dim) += dom.dsiDim(dim).stride:strType;
          for dim2 in dim+1..rank {
            f <~> "\n";
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

pragma "no copy return"
proc BlockArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }
  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idx(j);
      j += 1;
    }
  }
  return tup;
}

proc _extendTuple(type t, idx, args) {
  var tup: args.size*t;
  var idxTup = (idx,);
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}

proc BlockArr.dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable))
{
  //
  // For the default rectangular array, this function changes the data
  // vector in the array class so that it is setup once the default
  // rectangular domain is changed.  For this distributed array class,
  // we don't need to do anything, because changing the domain will
  // change the domain in the local array class which will change the
  // data in the local array class.  This will only work if the domain
  // we are reallocating to has the same distribution, but domain
  // assignment is defined so that only the indices are transferred.
  // The distribution remains unchanged.
  //
}

proc BlockArr.dsiPostReallocate() {
  // Call this *after* the domain has been reallocated
  if doRADOpt then setupRADOpt();
}

proc BlockArr.setRADOpt(val=true) {
  doRADOpt = val;
  if doRADOpt then setupRADOpt();
}

//
// the accessor for the local array -- assumes the index is local
//
// TODO: Should this be inlined?
//
inline proc LocBlockArr.this(i) ref {
  return myElems(i);
}

//
// Privatization
//
proc Block.Block(other: Block, privateData,
                param rank = other.rank,
                type idxType = other.idxType,
                type sparseLayoutType = other.sparseLayoutType) {
  boundingBox = {(...privateData(1))};
  targetLocDom = {(...privateData(2))};
  dataParTasksPerLocale = privateData(3);
  dataParIgnoreRunningTasks = privateData(4);
  dataParMinGranularity = privateData(5);

  for i in targetLocDom {
    targetLocales(i) = other.targetLocales(i);
    locDist(i) = other.locDist(i);
  }
}

proc Block.dsiSupportsPrivatization() param return true;

proc Block.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}

proc Block.dsiPrivatize(privatizeData) {
  return new Block(this, privatizeData);
}

proc Block.dsiGetReprivatizeData() return boundingBox.dims();

proc Block.dsiReprivatize(other, reprivatizeData) {
  boundingBox = {(...reprivatizeData)};
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

proc BlockDom.dsiSupportsPrivatization() param return true;

proc BlockDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc BlockDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(1));
  // in constructor we have to pass sparseLayoutType as it has no default value
  var c = new BlockDom(rank=rank, idxType=idxType, stridable=stridable,
      sparseLayoutType=privdist.sparseLayoutType, dist=privdist);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = {(...privatizeData(2))};
  return c;
}

proc BlockDom.dsiGetReprivatizeData() return whole.dims();

proc BlockDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = {(...reprivatizeData)};
}

proc BlockArr.dsiSupportsPrivatization() param return true;

proc BlockArr.dsiGetPrivatizeData() return dom.pid;

proc BlockArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);
  var c = new BlockArr(eltType=eltType, rank=rank, idxType=idxType,
      stridable=stridable, sparseLayoutType=sparseLayoutType, dom=privdom);
  for localeIdx in c.dom.dist.targetLocDom {
    c.locArr(localeIdx) = locArr(localeIdx);
    if c.locArr(localeIdx).locale.id == here.id then
      c.myLocArr = c.locArr(localeIdx);
  }
  return c;
}

private proc _canDoSimpleBlockTransfer(A, aView, B, bView) {
  if debugBlockDistBulkTransfer then
    writeln("In BlockDist._canDoSimpleBlockTransfer");

  if A.rank != B.rank then
    compilerError("_canDoSimpleBlockTransfer expecting equal ranks: ", A.rank:string, " vs ", B.rank:string);

  if A.sparseLayoutType != DefaultDist ||
     B.sparseLayoutType != DefaultDist {
    return false;
  } else if aView.stridable || bView.stridable {
    for param i in 1..A.rank {
      if aView.dim(i).stride != 1 ||
         bView.dim(i).stride != 1 then return false;
    }
  }

  return useBulkTransferDist;
}

// Block = this
proc BlockArr.doiBulkTransferToKnown(srcDom, destClass:BlockArr, destDom) : bool
where useBulkTransferDist {
  if _canDoSimpleBlockTransfer(destClass, destDom, this, srcDom) {
    _doSimpleBlockTransfer(destClass, destDom, this, srcDom);
    return true;
  } else {
    return false;
  }
}

// this = Block
proc BlockArr.doiBulkTransferFromKnown(destDom, srcClass:BlockArr, srcDom) : bool
where useBulkTransferDist {
  if _canDoSimpleBlockTransfer(this, destDom, srcClass, srcDom) {
    _doSimpleBlockTransfer(this, destDom, srcClass, srcDom);
    return true;
  } else {
    return false;
  }
}

private proc _doSimpleBlockTransfer(Dest, destDom, Src, srcDom) {
  if debugBlockDistBulkTransfer then
    writeln("In BlockDist._doSimpleBlockTransfer");

  if debugBlockDistBulkTransfer then resetCommDiagnostics();

  param rank = Dest.rank;

  const equalDoms = (Dest.dom.whole == Src.dom.whole) &&
                    (srcDom == destDom) &&
                    (Dest.dom.dist.dsiEqualDMaps(Src.dom.dist));

  // Use zippered iteration to piggyback data movement with the remote
  // fork.  This avoids remote gets for each access to Dest.locArr[i] and
  // Src.locArr[i]
  coforall (i, destLocArr, srcLocArr, destLocDom, srcLocDom) in zip(Dest.dom.dist.targetLocDom,
                                                                  Dest.locArr,
                                                                  Src.locArr,
                                                                  Dest.dom.locDoms,
                                                                  Src.dom.locDoms) {
    on Dest.dom.dist.targetLocales(i) {
      if debugBlockDistBulkTransfer then startCommDiagnosticsHere();

      const viewBlock = destLocDom.myBlock[destDom];

      if equalDoms {
        const theirView = srcLocDom.myBlock[srcDom];
        destLocArr.myElems[viewBlock] = srcLocArr.myElems[theirView];
      } else if Dest.rank == 1 {
        var start = viewBlock.low;

        for (rid, rlo, size) in ConsecutiveChunks(destDom, Src.dom, srcDom, viewBlock.size, start) {
          destLocArr.myElems[start..#size] = Src.locArr[rid].myElems[rlo..#size];
          start += size;
        }
      } else {
        const orig = viewBlock.low(rank);

        for coord in dropDims(viewBlock, viewBlock.rank) {
          var lo = if rank == 2 then (coord, orig) else ((...coord), orig);

          for (rid, rlo, size) in ConsecutiveChunksD(destDom, Src.dom, srcDom, viewBlock.dim(rank).length, lo) {
            var LSlice, RSlice : rank*range(idxType = Dest.dom.idxType);

            for param i in 1..rank-1 {
              LSlice(i) = if rank == 2 then coord..coord else coord(i)..coord(i);
              RSlice(i) = rlo(i)..rlo(i);
            }
            LSlice(rank) = lo(rank)..#size;
            RSlice(rank) = rlo(rank)..#size;

            destLocArr.myElems[(...LSlice)] = Src.locArr[rid].myElems[(...RSlice)];

            lo(rank) += size;
          }
        }
      }

      if debugBlockDistBulkTransfer then stopCommDiagnosticsHere();
    }
  }
  if debugBlockDistBulkTransfer then writeln("Comms:",getCommDiagnostics());
}

proc BlockArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc BlockDom.dsiTargetLocales() {
  return dist.targetLocales;
}

proc Block.dsiTargetLocales() {
  return targetLocales;
}

// Block subdomains are continuous

proc BlockArr.dsiHasSingleLocalSubdomain() param return true;
proc BlockDom.dsiHasSingleLocalSubdomain() param return true;

// returns the current locale's subdomain

proc BlockArr.dsiLocalSubdomain() {
  return myLocArr.locDom.myBlock;
}
proc BlockDom.dsiLocalSubdomain() {
  // TODO -- could be replaced by a privatized myLocDom in BlockDom
  // as it is with BlockArr
  var myLocDom:LocBlockDom(rank, idxType, stridable) = nil;
  for (loc, locDom) in zip(dist.targetLocales, locDoms) {
    if loc == here then
      myLocDom = locDom;
  }
  return myLocDom.myBlock;
}

iter ConsecutiveChunks(LView, RDomClass, RView, len, in start) {
  var elemsToGet = len;
  const offset   = RView.low - LView.low;
  var rlo        = start + offset;
  var rid        = RDomClass.dist.targetLocsIdx(rlo);
  while elemsToGet > 0 {
    const size = min(RDomClass.numRemoteElems(RView, rlo, rid), elemsToGet);
    yield (rid, rlo, size);
    rid += 1;
    rlo += size;
    elemsToGet -= size;
  }
}

iter ConsecutiveChunksD(LView, RDomClass, RView, len, in start) {
  param rank     = LView.rank;
  var elemsToGet = len;
  const offset   = RView.low - LView.low;
  var rlo        = start + offset;
  var rid        = RDomClass.dist.targetLocsIdx(rlo);
  while elemsToGet > 0 {
    const size = min(RDomClass.numRemoteElems(RView, rlo(rank):int, rid(rank):int), elemsToGet);
    yield (rid, rlo, size);
    rid(rank) +=1;
    rlo(rank) += size;
    elemsToGet -= size;
  }
}

proc BlockDom.numRemoteElems(viewDom, rlo, rid) {
  // NOTE: Not bothering to check to see if rid+1, length, or rlo-1 used
  //  below can fit into idxType
  var blo, bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank).length - 1) then
    bhi=viewDom.dim(rank).high;
  else {
      bhi = dist.boundingBox.dim(rank).low +
        intCeilXDivByY((dist.boundingBox.dim(rank).high - dist.boundingBox.dim(rank).low +1)*(rid+1):idxType,
                       dist.targetLocDom.dim(rank).length:idxType) - 1:idxType;
  }

  return (bhi - (rlo - 1):idxType);
}

//Brad's utility function. It drops from Domain D the dimensions
//indicated by the subsequent parameters dims.
proc dropDims(D: domain, dims...) {
  var r = D.dims();
  var r2: (D.rank-dims.size)*r(1).type;
  var j = 1;
  for i in 1..D.rank do
    for k in 1..dims.size do
      if dims(k) != i {
        r2(j) = r(i);
        j+=1;
      }
  var DResult = {(...r2)};
  return DResult;
}

private proc canDoAnyToBlock(Dest, destDom, Src, srcDom) param : bool {
  if Dest.rank != Src.rank then return false;
  use Reflection;

  // Does 'Src' support bulk transfers *to* a DefaultRectangular?
  if !canResolveMethod(Src, "doiBulkTransferToKnown", srcDom,
                       Dest.locArr[Dest.locArr.domain.first].myElems._value, destDom) {
    return false;
  }

  return useBulkTransferDist;
}

// Overload for any transfer *to* Block, if the RHS supports transfers to a
// DefaultRectangular
//
// TODO: avoid spawning so many coforall-ons
//   - clean up some of this range creation logic
proc BlockArr.doiBulkTransferFromAny(destDom, Src, srcDom) : bool
where canDoAnyToBlock(this, destDom, Src, srcDom) {

  if debugBlockDistBulkTransfer then
    writeln("In BlockDist.doiBulkTransferFromAny");

  const Dest = this;
  type el    = Dest.idxType;

  coforall i in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocales(i) {
      const regionDest = Dest.dom.locDoms(i).myBlock[destDom];
      const regionSrc  = Src.dom.locDoms(i).myBlock[srcDom];
      if regionDest.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(regionDest.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(regionDest.last, destDom, srcDom);
        const sb  = chpl__tuplify(regionSrc.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = regionDest.dims();
         //In the case that the number of elements in dimension t for r1 and r2
         //were different, we need to calculate the correct stride in r1
        for param t in 1..rank {
            r1[t] = (ini[t]:el..end[t]:el by sb[t]);
            if r1[t].length != r2[t].length then
              r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
        }

        if debugBlockDistBulkTransfer then
          writeln("A.locArr[i][", regionDest, "] = B[", (...r1), "]");

        // TODO: handle possibility that this function returns false
        chpl__bulkTransferArray(Dest.locArr[i].myElems._value, regionDest, Src, {(...r1)});
      }
    }
  }

  return true;
}

// For assignments of the form: DefaultRectangular = Block
proc BlockArr.doiBulkTransferToKnown(srcDom, Dest:DefaultRectangularArr, destDom) : bool
where useBulkTransferDist {

  if debugBlockDistBulkTransfer then
    writeln("In BlockDist.doiBulkTransferToKnown(DefaultRectangular)");

  const Src = this;
  type el   = Src.idxType;

  coforall j in Src.dom.dist.targetLocDom {
    on Src.dom.dist.targetLocales(j) {
      const inters = Src.dom.locDoms(j).myBlock[srcDom];
      if inters.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, srcDom, destDom);
        const end = bulkCommConvertCoordinate(inters.last, srcDom, destDom);
        const sa  = chpl__tuplify(destDom.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        for param t in 1..rank
        {
          r2[t] = (chpl__tuplify(inters.first)[t]
                   ..chpl__tuplify(inters.last)[t]
                   by chpl__tuplify(inters.stride)[t]);
          r1[t] = (ini[t]:el..end[t]:el by sa[t]);
        }

        if debugBlockDistBulkTransfer then
          writeln("A[",r1,"] = B[",r2,"]");

        const elemActual = Src.locArr[j].myElems._value;
        chpl__bulkTransferArray(Dest, {(...r1)}, elemActual, {(...r2)});
      }
    }
  }

  return true;
}

// For assignments of the form: Block = DefaultRectangular
proc BlockArr.doiBulkTransferFromKnown(destDom, Src:DefaultRectangularArr, srcDom) : bool
where useBulkTransferDist {
  if debugBlockDistBulkTransfer then
    writeln("In BlockArr.doiBulkTransferFromKnown(DefaultRectangular)");

  const Dest = this;
  type el    = Dest.idxType;

  coforall j in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocales(j) {
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      if inters.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(inters.last, destDom, srcDom);
        const sb  = chpl__tuplify(srcDom.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        for param t in 1..rank {
          r2[t] = (chpl__tuplify(inters.first)[t]
                   ..chpl__tuplify(inters.last)[t]
                   by chpl__tuplify(inters.stride)[t]);
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
        }

        if debugBlockDistBulkTransfer then
          writeln("A[",r2,"] = B[",r1,"]");

        const elemActual = Dest.locArr[j].myElems._value;
        chpl__bulkTransferArray(elemActual, {(...r2)}, Src, {(...r1)});
      }
    }
  }

  return true;
}
