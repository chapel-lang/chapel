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

private use DSIUtil;
private use ChapelUtil;
private use CommDiagnostics;
private use ChapelLocks;
private use ChapelDebugPrint;
private use LayoutCS;

public use SparseBlockDist;
//
// These flags are used to output debug information and run extra
// checks when using Block.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugBlockDist = false;
config param debugBlockDistBulkTransfer = false;

// TODO: This is no longer used, deprecate with warning because it is used
// in miniMD's release Makefile and compopts.
config const disableAliasedBulkTransfer = true;

config param disableBlockDistBulkTransfer = false;

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


**Initializer Arguments**

The ``Block`` class initializer is defined as follows:

  .. code-block:: chapel

    proc Block.init(
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

**Convenience Initializer Functions**

It is common for a ``Block`` distribution to distribute its ``boundingBox``
across all locales. In this case, a convenience function can be used to
declare variables of block-distributed domain or array type.  These functions
take a domain or list of ranges as arguments and return a block-distributed
domain or array.

  .. code-block:: chapel

    use BlockDist;

    var BlockDom1 = newBlockDom({1..5, 1..5});
    var BlockArr1 = newBlockArr({1..5, 1..5}, real);
    var BlockDom2 = newBlockDom(1..5, 1..5);
    var BlockArr2 = newBlockArr(1..5, 1..5, real);

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
``sparseLayoutType`` initializer argument to Block.

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
  var locDist: [targetLocDom] unmanaged LocBlock(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
  type sparseLayoutType = unmanaged DefaultDist;
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
  const dist: unmanaged Block(rank, idxType, sparseLayoutType);
  var locDoms: [dist.targetLocDom] unmanaged LocBlockDom(rank, idxType, stridable);
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
  var dom: unmanaged BlockDom(rank, idxType, stridable, sparseLayoutType);
  var locArr: [dom.dist.targetLocDom] unmanaged LocBlockArr(eltType, rank, idxType, stridable);
  pragma "local field"
  var myLocArr: unmanaged LocBlockArr(eltType, rank, idxType, stridable)?;
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
  const locDom: unmanaged LocBlockDom(rank, idxType, stridable);
  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, stridable)?; // non-nil if doRADOpt=true
  pragma "local field" pragma "unsafe" // initialized separately
  var myElems: [locDom.myBlock] eltType;
  var locRADLock: chpl_LocalSpinlock;

  proc deinit() {
    if locRAD != nil then
      delete locRAD;
  }
}


////// Block and LocBlock methods ///////////////////////////////////////////

//
// Block initializer for clients of the Block distribution
//
proc Block.init(boundingBox: domain,
                targetLocales: [] locale = Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.idxType,
                type sparseLayoutType = unmanaged DefaultDist) {
  this.rank = rank;
  this.idxType = idxType;
  if rank != boundingBox.rank then
    compilerError("specified Block rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified Block index type != index type of specified bounding box");
  if rank != 2 && isCSType(sparseLayoutType) then 
    compilerError("CS layout is only supported for 2 dimensional domains");

  if boundingBox.size == 0 then
    halt("Block() requires a non-empty boundingBox");

  this.boundingBox = boundingBox : domain(rank, idxType, stridable = false);

  const ranges = setupTargetLocRanges(rank, targetLocales);
  this.targetLocDom = {(...ranges)};
  this.targetLocales = reshape(targetLocales, this.targetLocDom);

  // Instead of 'dummyLB', we could give 'locDistTemp' a nilable element type.
  const dummyLB = new unmanaged LocBlock(rank, idxType, dummy=true);
  var locDistTemp: [targetLocDom] unmanaged LocBlock(rank, idxType) = dummyLB;
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDistTemp(locid) = new unmanaged LocBlock(rank, idxType, locid,
                                                  boundingBox, targetLocDom);
  delete dummyLB;
  this.locDist = locDistTemp; //make this a serial loop instead?

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  this.sparseLayoutType = _to_unmanaged(sparseLayoutType);

  this.complete();

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

  coforall locid in targetLocDom do
    on targetLocales(locid) do
      locDist(locid) = new unmanaged LocBlock(rank, idxType, locid, boundingBox,
                                    targetLocDom);
}

//
// Block distributions are equivalent if they share the same bounding
// box and target locale set.
//
proc Block.dsiEqualDMaps(that: Block(?)) {
  return (this.rank == that.rank &&
          this.boundingBox == that.boundingBox &&
          this.targetLocales.equals(that.targetLocales));
}

//
// Block distributions are not equivalent to other domain maps.
//
proc Block.dsiEqualDMaps(that) param {
  return false;
}

proc Block.dsiClone() {
  return new unmanaged Block(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity,
                   rank,
                   idxType,
                   sparseLayoutType);
}

override proc Block.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

override proc Block.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

override proc Block.dsiNewRectangularDom(param rank: int, type idxType,
                                         param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Block domain rank does not match distribution's");

  const whole = createWholeDomainForInds(rank, idxType, stridable, inds);

  const dummyLBD = new unmanaged LocBlockDom(rank, idxType, stridable);
  var locDomsTemp: [this.targetLocDom]
                  unmanaged LocBlockDom(rank, idxType, stridable) = dummyLBD;
  coforall localeIdx in this.targetLocDom do
   on this.targetLocales(localeIdx) do
    locDomsTemp(localeIdx) = new unmanaged LocBlockDom(rank, idxType, stridable,
                                             this.getChunk(whole, localeIdx));
  delete dummyLBD;

  var dom = new unmanaged BlockDom(rank, idxType, stridable, sparseLayoutType,
                                   this: unmanaged, locDomsTemp, whole);

  if debugBlockDist {
    writeln("Creating new Block domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

override proc Block.dsiNewSparseDom(param rank: int, type idxType,
                                    dom: domain) {
  var ret =  new unmanaged SparseBlockDom(rank=rank, idxType=idxType,
                            sparseLayoutType=sparseLayoutType,
                            stridable=dom.stridable,
                            dist=_to_unmanaged(this), whole=dom._value.whole,
                            parentDom=dom);
  ret.setup();
  return ret;
}

//
// output distribution
//
proc Block.writeThis(x) throws {
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
    if chunk.size > 0 {
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
  for param i in 0..rank-1 do
    result(i) = max(0, min((targetLocDom.dim(i).size-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).size:idxType) /
                            boundingBox.dim(i).size):int));
  return if rank == 1 then result(0) else result;
}

// TODO: This will not trigger the bounded-coforall optimization
iter Block.activeTargetLocales(const space : domain = boundingBox) {
  const locSpace = {(...space.dims())}; // make a local domain in case 'space' is distributed
  const low = chpl__tuplify(targetLocsIdx(locSpace.first));
  const high = chpl__tuplify(targetLocsIdx(locSpace.last));
  var dims : rank*range(low(0).type);
  for param i in 0..rank-1 {
    dims(i) = low(i)..high(i);
  }

  // In case 'locSpace' is a strided domain we need to check that the locales
  // in 'dims' actually contain indices in 'locSpace'.
  //
  // Note that we cannot use a simple stride here because it is not guaranteed
  // that each locale contains the same number of indices. For example, the
  // domain {1..10} over four locales will split like:
  //   L0: -max(int)..3
  //   L1: 4..5
  //   L2: 6..8
  //   L3: 9..max(int)
  //
  // The subset {1..10 by 4} will involve locales 0, 1, and 3.
  for i in {(...dims)} {
    const chunk = chpl__computeBlock(i, targetLocDom, boundingBox);
    // TODO: Want 'contains' for a domain. Slicing is a workaround.
    if locSpace[(...chunk)].size > 0 then
      yield i;
  }
}

proc chpl__computeBlock(locid, targetLocBox, boundingBox) {
  param rank = targetLocBox.rank;
  type idxType = chpl__tuplify(boundingBox)(0).idxType;
  var inds: rank*range(idxType);
  for param i in 0..rank-1 {
    const lo = boundingBox.dim(i).low;
    const hi = boundingBox.dim(i).high;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox.dim(i).size;
    const (blo, bhi) = _computeBlock(numelems, numlocs, chpl__tuplify(locid)(i),
                                     max(idxType), min(idxType), lo);
    inds(i) = blo..bhi;
  }
  return inds;
}

proc LocBlock.init(param rank: int,
                   type idxType,
                   locid, // the locale index from the target domain
                   boundingBox,
                   targetLocDom: domain(rank)) {
  this.rank = rank;
  this.idxType = idxType;
  const inds = chpl__computeBlock(chpl__tuplify(locid), targetLocDom, boundingBox);
  myChunk = {(...inds)};
}

// Used to create a dummy instance.
proc LocBlock.init(param rank, type idxType, param dummy: bool) where dummy {
  this.rank = rank;
  this.idxType = idxType;
}


////// BlockDom and LocBlockDom methods /////////////////////////////////////

override proc BlockDom.dsiMyDist() return dist;

override proc BlockDom.dsiDisplayRepresentation() {
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
  return helper(0);
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
    for param i in 0..tmpBlock.rank-1 {
      const stride = tmpBlock.dim(i).stride;
      if stride < 0 && strType != idxType then
        halt("negative stride not supported with unsigned idxType");
        // (since locOffset is unsigned in that case)
      locOffset(i) = tmpBlock.dim(i).first / stride:idxType;
    }
    // Forward to defaultRectangular
    for followThis in tmpBlock.these(iterKind.leader, maxTasks,
                                     myIgnoreRunning, minSize, locOffset) do
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
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag then
    chpl__testParWriteln("Block domain follower invoked on ", followThis);

  var t: rank*range(idxType, stridable=stridable||anyStridable(followThis));
  type strType = chpl__signedType(idxType);
  for param i in 0..rank-1 {
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
  const dom = this;
  const creationLocale = here.id;
  const dummyLBD = new unmanaged LocBlockDom(rank, idxType, stridable);
  const dummyLBA = new unmanaged LocBlockArr(eltType, rank, idxType,
                                             stridable, dummyLBD);
  var locArrTemp: [dom.dist.targetLocDom]
        unmanaged LocBlockArr(eltType, rank, idxType, stridable) = dummyLBA;
  var myLocArrTemp: unmanaged LocBlockArr(eltType, rank, idxType, stridable)?;

  // formerly in BlockArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref myLocArrTemp) {
    on dom.dist.targetLocales(localeIdx) {
      const LBA = new unmanaged LocBlockArr(eltType, rank, idxType, stridable,
                                            dom.getLocDom(localeIdx));
      locArrTemp(localeIdx) = LBA;
      if here.id == creationLocale then
        myLocArrTemp = LBA;
    }
  }
  delete dummyLBA, dummyLBD;

  var arr = new unmanaged BlockArr(eltType=eltType, rank=rank, idxType=idxType,
       stridable=stridable, sparseLayoutType=sparseLayoutType,
       dom=_to_unmanaged(dom), locArr=locArrTemp, myLocArr=myLocArrTemp);

  // formerly in BlockArr.setup()
  if arr.doRADOpt && disableBlockLazyRAD then arr.setupRADOpt();

  return arr;
}

proc BlockDom.dsiNumIndices return whole.size;
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
  if x(0).idxType != idxType then
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
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocales(localeIdx) do
        locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    }
}

override proc BlockDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom do {
    on locDoms(localeIdx) do
      delete locDoms(localeIdx);
  }
}

proc BlockDom.dsiMember(i) {
  return whole.contains(i);
}

proc BlockDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocBlockDom.contains(i) return myBlock.contains(i);


////// BlockArr and LocBlockArr methods /////////////////////////////////////

override proc BlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD!.RAD);
  }
}

override proc BlockArr.dsiGetBaseDom() return dom;

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
        myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
        for l in dom.dist.targetLocDom {
          if l != localeIdx {
            myLocArr.locRAD!.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
          }
        }
      }
    }
  }
}

override proc BlockArr.dsiDestroyArr() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      delete locArr(localeIdx);
    }
  }
}

inline proc BlockArr.dsiLocalAccess(i: rank*idxType) ref {
  return _to_nonnil(myLocArr).this(i);
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
    if myLocArr != nil && _to_nonnil(myLocArr).locDom.contains(idx) then
      return _to_nonnil(myLocArr).this(idx);
  }
  return nonLocalAccess(idx);
}

inline proc BlockArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.dsiMember(i);
}

pragma "fn unordered safe"
proc BlockArr.nonLocalAccess(i: rank*idxType) ref {
  if doRADOpt {
    if this.myLocArr {
      const myLocArr = _to_nonnil(this.myLocArr);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableBlockLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.locRADLock.lock();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new unmanaged LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            myLocArr.locRAD = tempLocRAD;
          }
          myLocArr.locRADLock.unlock();
        }
        const locRAD = _to_nonnil(myLocArr.locRAD);
        if locRAD.RAD(rlocIdx).blk == SENTINEL {
          locRAD.lockRAD(rlocIdx);
          if locRAD.RAD(rlocIdx).blk == SENTINEL {
            locRAD.RAD(rlocIdx) =
              locArr(rlocIdx).myElems._value.dsiGetRAD();
          }
          locRAD.unlockRAD(rlocIdx);
        }
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr.locRAD;
      pragma "no copy" pragma "no auto destroy" var radata = _to_nonnil(myLocRAD).RAD;
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

override proc BlockArr.dsiStaticFastFollowCheck(type leadType) param {
  if isSubtype(leadType, BlockArr) {
    // Comparing domains for rank/idxType/stride/sparseLayout, which allows for
    // fast followers regardless of eltType.
    //
    // TODO: Remove once 'typeExpr.field' results in a type
    var x : leadType?;
    return _to_borrowed(x!.dom.type) == _to_borrowed(this.dom.type);
  } else {
    return _to_borrowed(leadType) == _to_borrowed(this.dom.type);
  }
}

proc BlockArr.dsiDynamicFastFollowCheck(lead: [])
  return this.dsiDynamicFastFollowCheck(lead.domain);

proc BlockArr.dsiDynamicFastFollowCheck(lead: domain) {
  // TODO: Should this return true for domains with the same shape?
  return lead.dist.dsiEqualDMaps(this.dom.dist) && lead._value.whole == this.dom.whole;
}

iter BlockArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
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

  for param i in 0..rank-1 {
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
      arrSection = _to_nonnil(myLocArr);

    local {
      const narrowArrSection = __primitive("_wide_get_addr", arrSection):arrSection.type?;
      ref myElems = _to_nonnil(narrowArrSection).myElems;
      for i in myFollowThisDom do yield myElems[i];
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
  chpl_serialReadWriteRectangular(f, this);
}

pragma "no copy return"
proc BlockArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 0..rank-1 {
    low(i) = ranges(i).alignedLow;
  }

  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 1;

  for param i in 0..args.size-1 {
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

  for param i in 0..args.size-1 {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}

override proc BlockArr.dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable))
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

override proc BlockArr.dsiPostReallocate() {
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


///// Privatization and serialization ///////////////////////////////////////

proc Block.init(other: Block, privateData,
                param rank = other.rank,
                type idxType = other.idxType,
                type sparseLayoutType = other.sparseLayoutType) {
  this.rank = rank;
  this.idxType = idxType;
  boundingBox = {(...privateData(0))};
  targetLocDom = {(...privateData(1))};
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = privateData(2);
  dataParIgnoreRunningTasks = privateData(3);
  dataParMinGranularity = privateData(4);
  this.sparseLayoutType = sparseLayoutType;
}

override proc Block.dsiSupportsPrivatization() param return true;

proc Block.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}

proc Block.dsiPrivatize(privatizeData) {
  return new unmanaged Block(_to_unmanaged(this), privatizeData);
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

proc BlockDom.chpl__serialize() {
  return pid;
}

// TODO: What happens when we try to deserialize on a locale that doesn't
// own a copy of the privatized class?  (can that happen?)  Could this
// be a way to lazily privatize by also making the originating locale part
// of the 'data'?
proc type BlockDom.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged BlockDom(rank=this.rank,
                              idxType=this.idxType,
                              stridable=this.stridable,
                              sparseLayoutType=this.sparseLayoutType),
           data);
}

override proc BlockDom.dsiSupportsPrivatization() param return true;

record BlockDomPrvData {
  var distpid;
  var dims;
  var locdoms;  //todo rvf its elements along with the rest of the record
}

proc BlockDom.dsiGetPrivatizeData() {
  return new BlockDomPrvData(dist.pid, whole.dims(), locDoms);
}

proc BlockDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData.distpid);

  var locDomsTemp: [privdist.targetLocDom]
                      unmanaged LocBlockDom(rank, idxType, stridable)
    = privatizeData.locdoms;

  // in initializer we have to pass sparseLayoutType as it has no default value
  const c = new unmanaged BlockDom(rank, idxType, stridable,
                                   privdist.sparseLayoutType, privdist,
                                   locDomsTemp, {(...privatizeData.dims)});
  return c;
}

proc BlockDom.dsiGetReprivatizeData() return whole.dims();

proc BlockDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = {(...reprivatizeData)};
}

proc BlockArr.chpl__serialize() {
  return pid;
}

proc type BlockArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged BlockArr(rank=this.rank,
                              idxType=this.idxType,
                              stridable=this.stridable,
                              eltType=this.eltType,
                              sparseLayoutType=this.sparseLayoutType),
           data);
}

override proc BlockArr.dsiSupportsPrivatization() param return true;

record BlockArrPrvData {
  var dompid;
  var locarr;  //todo rvf its elements along with the rest of the record
}

proc BlockArr.dsiGetPrivatizeData() {
  return new BlockArrPrvData(dom.pid, locArr);
}

proc BlockArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData.dompid);

  var locArrTemp: [privdom.dist.targetLocDom]
                     unmanaged LocBlockArr(eltType, rank, idxType, stridable)
    = privatizeData.locarr;

  var myLocArrTemp: unmanaged LocBlockArr(eltType, rank, idxType, stridable)?;
  for localeIdx in privdom.dist.targetLocDom do
    if locArrTemp(localeIdx).locale.id == here.id then
      myLocArrTemp = locArrTemp(localeIdx);

  const c = new unmanaged BlockArr(eltType=eltType, rank=rank, idxType=idxType,
                      stridable=stridable, sparseLayoutType=sparseLayoutType,
                      dom=privdom, locArr=locArrTemp, myLocArr = myLocArrTemp);
  return c;
}


////// more /////////////////////////////////////////////////////////////////

proc BlockArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc BlockDom.dsiTargetLocales() {
  return dist.targetLocales;
}

proc Block.dsiTargetLocales() {
  return targetLocales;
}

proc Block.chpl__locToLocIdx(loc: locale) {
  for locIdx in targetLocDom do
    if (targetLocales[locIdx] == loc) then
      return (true, locIdx);
  return (false, targetLocDom.first);
}

// Block subdomains are continuous

proc BlockArr.dsiHasSingleLocalSubdomain() param return true;
proc BlockDom.dsiHasSingleLocalSubdomain() param return true;

// returns the current locale's subdomain

proc BlockArr.dsiLocalSubdomain(loc: locale) {
  if (loc == here) {
    // quick solution if we have a local array
    if myLocArr != nil then
      return _to_nonnil(myLocArr).locDom.myBlock;
    // if not, we must not own anything
    var d: domain(rank, idxType, stridable);
    return d;
  } else {
    return dom.dsiLocalSubdomain(loc);
  }
}
proc BlockDom.dsiLocalSubdomain(loc: locale) {
  const (gotit, locid) = dist.chpl__locToLocIdx(loc);
  if (gotit) {
    var inds = chpl__computeBlock(locid, dist.targetLocDom, dist.boundingBox);
    return whole[(...inds)];
  } else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc BlockDom.numRemoteElems(viewDom, rlo, rid) {
  // NOTE: Not bothering to check to see if rid+1, length, or rlo-1 used
  //  below can fit into idxType
  var blo, bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank-1).size - 1) then
    bhi=viewDom.dim(rank-1).high;
  else {
      bhi = dist.boundingBox.dim(rank-1).low +
        intCeilXDivByY((dist.boundingBox.dim(rank-1).high - dist.boundingBox.dim(rank-1).low +1)*(rid+1):idxType,
                       dist.targetLocDom.dim(rank-1).size:idxType) - 1:idxType;
  }

  return (bhi - (rlo - 1):idxType);
}

private proc canDoAnyToBlock(Dest, destDom, Src, srcDom) param : bool {
  if Src.doiCanBulkTransferRankChange() == false &&
     Dest.rank != Src.rank then return false;

  use Reflection;

  // Does 'Src' support bulk transfers *to* a DefaultRectangular?
  if !canResolveMethod(Src, "doiBulkTransferToKnown", srcDom,
                       Dest.locArr[Dest.locArr.domain.first].myElems._value, destDom) {
    return false;
  }

  return !disableBlockDistBulkTransfer;
}

// Block = this
proc BlockArr.doiBulkTransferToKnown(srcDom, destClass:BlockArr, destDom) : bool
where this.sparseLayoutType == unmanaged DefaultDist &&
      destClass.sparseLayoutType == unmanaged DefaultDist &&
      !disableBlockDistBulkTransfer {
  _doSimpleBlockTransfer(destClass, destDom, this, srcDom);
  return true;
}

// this = Block
proc BlockArr.doiBulkTransferFromKnown(destDom, srcClass:BlockArr, srcDom) : bool
where this.sparseLayoutType == unmanaged DefaultDist &&
      srcClass.sparseLayoutType == unmanaged DefaultDist &&
      !disableBlockDistBulkTransfer {
  _doSimpleBlockTransfer(this, destDom, srcClass, srcDom);
  return true;
}

private proc _doSimpleBlockTransfer(Dest, destDom, Src, srcDom) {
  if debugBlockDistBulkTransfer then
    writeln("In Block=Block Bulk Transfer: Dest[", destDom, "] = Src[", srcDom, "]");

  // Cache to avoid GETs
  const DestPID = Dest.pid;
  const SrcPID = Src.pid;

  coforall i in Dest.dom.dist.activeTargetLocales(destDom) {
    on Dest.dom.dist.targetLocales[i] {
      // Relies on the fact that we privatize across all locales in the
      // program, not just the targetLocales of Dest/Src.
      const dst = if _privatization then chpl_getPrivatizedCopy(Dest.type, DestPID) else Dest;
      const src = if _privatization then chpl_getPrivatizedCopy(Src.type, SrcPID) else Src;

      // Compute the local portion of the destination domain, and find the
      // corresponding indices in the source's domain.
      const localDestBlock = dst.dom.locDoms[i].myBlock[destDom];
      assert(localDestBlock.size > 0);
      const corSrcBlock    = bulkCommTranslateDomain(localDestBlock, destDom, srcDom);
      if debugBlockDistBulkTransfer then
        writeln("  Dest[",localDestBlock,"] = Src[", corSrcBlock,"]");

      // The corresponding indices in the source's domain do not necessarily
      // all live on the same locale. This loop finds the chunks that live on a
      // single locale, translates those back to the destination's domain, and
      // performs the transfer.
      for srcLoc in src.dom.dist.activeTargetLocales(corSrcBlock) {
        const localSrcChunk  = corSrcBlock[src.dom.locDoms[srcLoc].myBlock];
        const localDestChunk = bulkCommTranslateDomain(localSrcChunk, corSrcBlock, localDestBlock);
        chpl__bulkTransferArray(dst.locArr[i].myElems._value, localDestChunk,
                                src.locArr[srcLoc].myElems._value, localSrcChunk);
      }
    }
  }
}

// Overload for any transfer *to* Block, if the RHS supports transfers to a
// DefaultRectangular
proc BlockArr.doiBulkTransferFromAny(destDom, Src, srcDom) : bool
where canDoAnyToBlock(this, destDom, Src, srcDom) {

  if debugBlockDistBulkTransfer then
    writeln("In BlockDist.doiBulkTransferFromAny");

  coforall j in dom.dist.activeTargetLocales(destDom) {
    on dom.dist.targetLocales(j) {
      const Dest = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;

      const inters   = Dest.dom.locDoms(j).myBlock[destDom];
      const srcChunk = bulkCommTranslateDomain(inters, destDom, srcDom);

      if debugBlockDistBulkTransfer then
        writeln("Dest.locArr[", j, "][", inters, "] = Src[", srcDom, "]");

      chpl__bulkTransferArray(Dest.locArr[j].myElems._value, inters, Src, srcChunk);
    }
  }

  return true;
}

// For assignments of the form: DefaultRectangular = Block
proc BlockArr.doiBulkTransferToKnown(srcDom, Dest:DefaultRectangularArr, destDom) : bool
where !disableBlockDistBulkTransfer {

  if debugBlockDistBulkTransfer then
    writeln("In BlockDist.doiBulkTransferToKnown(DefaultRectangular)");

  coforall j in dom.dist.activeTargetLocales(srcDom) {
    on dom.dist.targetLocales(j) {
      const Src = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;
      const inters = Src.dom.locDoms(j).myBlock[srcDom];

      const destChunk = bulkCommTranslateDomain(inters, srcDom, destDom);

      if debugBlockDistBulkTransfer then
        writeln("  A[",destChunk,"] = B[",inters,"]");

      const elemActual = Src.locArr[j].myElems._value;
      chpl__bulkTransferArray(Dest, destChunk, elemActual, inters);
    }
  }

  return true;
}

// For assignments of the form: Block = DefaultRectangular
proc BlockArr.doiBulkTransferFromKnown(destDom, Src:DefaultRectangularArr, srcDom) : bool
where !disableBlockDistBulkTransfer {
  if debugBlockDistBulkTransfer then
    writeln("In BlockArr.doiBulkTransferFromKnown(DefaultRectangular)");

  coforall j in dom.dist.activeTargetLocales(destDom) {
    on dom.dist.targetLocales(j) {
      // Grab privatized copy of 'this' to avoid extra GETs
      const Dest = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      assert(inters.size > 0);

      const srcChunk = bulkCommTranslateDomain(inters, destDom, srcDom);

      if debugBlockDistBulkTransfer then
        writeln("  A[",inters,"] = B[",srcChunk,"]");

      const elemActual = Dest.locArr[j].myElems._value;
      chpl__bulkTransferArray(elemActual, inters, Src, srcChunk);
    }
  }

  return true;
}

override proc BlockArr.doiCanBulkTransferRankChange() param return true;

config param debugBlockScan = false;

proc BlockArr.doiScan(op, dom) where (rank == 1) &&
                                     chpl__scanStateResTypesMatch(op) {

  // The result of this scan, which will be Block-distributed as well
  type resType = op.generate().type;
  var res: [dom] resType;

  // Store one element per locale in order to track our local total
  // for a cross-locale scan as well as flags to negotiate reading and
  // writing it.  This domain really wants an easier way to express
  // it...
  use ReplicatedDist;
  ref targetLocs = this.dsiTargetLocales();
  const elemPerLocDom = {1..1} dmapped Replicated(targetLocs);
  var elemPerLoc: [elemPerLocDom] resType;
  var inputReady$: [elemPerLocDom] sync bool;
  var outputReady$: [elemPerLocDom] sync bool;

  // Fire up tasks per participating locale
  coforall locid in dom.dist.targetLocDom {
    on targetLocs[locid] {
      const myop = op.clone(); // this will be deleted by doiScan()

      // set up some references to our LocBlockArr descriptor, our
      // local array, local domain, and local result elements
      ref myLocArrDesc = locArr[locid];
      ref myLocArr = myLocArrDesc.myElems;
      const ref myLocDom = myLocArr.domain;

      // Compute the local pre-scan on our local array
      var (numTasks, rngs, state, tot) = myLocArr._value.chpl__preScan(myop, res, myLocDom[dom]);
      if debugBlockScan then
        writeln(locid, ": ", (numTasks, rngs, state, tot));

      // save our local scan total away and signal that it's ready
      elemPerLoc[1] = tot;
      inputReady$[1] = true;

      // the "first" locale scans the per-locale contributions as they
      // become ready
      if (locid == dom.dist.targetLocDom.low) {
        const metaop = op.clone();

        var next: resType = metaop.identity;
        for locid in dom.dist.targetLocDom {
          const targetloc = targetLocs[locid];
          const locready = inputReady$.replicand(targetloc)[1];

          // store the scan value and mark that it's ready
          ref locVal = elemPerLoc.replicand(targetloc)[1];
          locVal <=> next;
          outputReady$.replicand(targetloc)[1] = true;

          // accumulate to prep for the next iteration
          metaop.accumulateOntoState(next, locVal);
        }
        delete metaop;
      }

      // block until someone tells us that our local value has been updated
      // and then read it
      const resready = outputReady$[1];
      const myadjust = elemPerLoc[1];
      if debugBlockScan then
        writeln(locid, ": myadjust = ", myadjust);

      // update our state vector with our locale's adjustment value
      for s in state do
        myop.accumulateOntoState(s, myadjust);
      if debugBlockScan then
        writeln(locid, ": state = ", state);

      // have our local array compute its post scan with the globally
      // accurate state vector
      myLocArr._value.chpl__postScan(op, res, numTasks, rngs, state);
      if debugBlockScan then
        writeln(locid, ": ", myLocArr);

      delete myop;
    }
  }

  delete op;
  return res;
}


////// Factory functions ////////////////////////////////////////////////////

proc newBlockDom(dom: domain) {
  return dom dmapped Block(dom);
}

proc newBlockArr(dom: domain, type eltType) {
  var D = newBlockDom(dom);
  var A: [D] eltType;
  return A;
}

proc newBlockDom(rng: range...) {
  return newBlockDom({(...rng)});
}

proc newBlockArr(rng: range..., type eltType) {
  return newBlockArr({(...rng)}, eltType);
}
