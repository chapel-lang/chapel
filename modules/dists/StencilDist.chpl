/*
 * Copyright 2004-2017 Cray Inc.
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
// The Stencil distribution is defined with six classes:
//
//   Stencil       : distribution class
//   StencilDom    : domain class
//   StencilArr    : array class
//   LocStencil    : local distribution class (per-locale instances)
//   LocStencilDom : local domain class (per-locale instances)
//   LocStencilArr : local array class (per-locale instances)
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

use BlockDist;
use DSIUtil;
use ChapelUtil;
use CommDiagnostics;


//
// These flags are used to output debug information and run extra
// checks when using Stencil.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugStencilDist = false;
config param debugStencilDistBulkTransfer = false;

// Re-uses these flags from BlockDist:
//   - disableAliasedBulkTransfer
//   - sanityCheckDistribution
//   - testFastFollowerOptimization

//
// This flag is used to disable lazy initialization of the RAD cache.
//
config param disableStencilLazyRAD = defaultDisableLazyRADOpt;

//
// Stencil Distribution Class
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
//                        a Stencil-distributed domain or array
//
// dataParIgnoreRunningTasks: a boolean what dictates whether the number of
//                            task use on each locale should be limited
//                            by the available parallelism
//
// dataParMinGranularity: the minimum required number of elements per
//                        task created
//

/*
  The Stencil distribution is a variant of the :mod:`Block <BlockDist>`
  distribution that attempts to improve performance for stencil computations by
  reducing the amount of communication necessary during array accesses. From
  the user's perspective, it behaves very similarly to the Block distribution
  where reads, writes, and iteration are concerned.

  This distribution reduces communication by creating read-only caches for
  elements adjacent to the block of elements owned by each locale. This
  documentation may refer to these cached regions as 'ghost cells' or 'fluff'.
  This approach can avoid many fine-grained GETs and PUTs when performing a
  stencil computation near the boundary of the current locale's chunk of array
  elements. The user must manually refresh these caches after writes by calling
  the ``updateFluff`` method. Otherwise, reading and writing array elements
  behaves the same as a Block-distributed array.
  
  The indices are partitioned in the same way as the :mod:`Block <BlockDist>`
  distribution.

  The ``Stencil`` class constructor is defined as follows:

    .. code-block:: chapel

      proc Stencil(
        boundingBox: domain,
        targetLocales: [] locale  = Locales,
        dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
        dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
        dataParMinGranularity     = // value of  dataParMinGranularity      config const,
        param rank                = boundingBox.rank,
        type  idxType             = boundingBox.idxType,
        fluff: rank*idxType       = makeZero(rank),
        periodic: bool            = false)

  The ``fluff`` argument indicates the requested number of cached elements in
  each dimension. If an element of ``fluff`` is greater than zero, the user can
  use indices outside of ``boundingBox`` to index into the array. If the domain
  is not strided, you can consider indices for dimension ``i`` to be:

  .. code-block:: chapel

     boundingBox.dim(i).expand(fluff(i))

  If the domain is strided:

  .. code-block:: chapel

     const bb = boundingBox.dim(i);
     bb.expand(fluff(i) * abs(bb.stride));

  The same logic is used when determining the cached index set on each locale,
  except you can imagine ``boundingBox`` to be replaced with the returned
  value from :proc:`~ChapelArray.localSubdomain`.

  The ``periodic`` argument indicates whether or not the stencil distribution
  should treat the array as a discrete chunk in a larger space. When enabled,
  the ghost cells outside of ``boundingBox`` will contain values as if the
  array was replicated on all sides of the space. When disabled, the outermost
  ghost cells will be initialized with the default value of the element's type.
  The ``periodic`` functionality is disabled by default.

  .. note::
  
     Note that this domain does not currently handle indices outside of
     the expanded bounding box, so a user must manually wrap periodic indices
     themselves.
  
  Iterating directly over a Stencil-distributed domain or array will only yield
  indices and elements within the ``boundingBox``.

  **Updating the Cached Elements**

  Once you have completed a series of writes to the array, you will need to
  call the ``updateFluff`` function to update the cached elements for each
  locale. Here is a simple example:

  .. code-block:: chapel

    use StencilDist;

    const Dom = {1..10, 1..10};
    const Space = Dom dmapped Stencil(Dom, fluff=(1,1));
    var A : [Space] int;

    [(i,j) in Space] A[i,j] = i*10 + j;

    // At this point, the ghost cell caches are out of date

    A.updateFluff();

    // ghost caches are now up-to-date

  After updating, any read from the array should be up-to-date. The
  ``updateFluff`` function does not currently accept any arguments.

  **Reading and Writing to Array Elements**

  The Stencil distribution uses ghost cells as cached read-only values from
  other locales. When reading from a Stencil-distributed array, the
  distribution will attempt to read from the local ghost cache first. If the
  index is not within the cached index set of the current locale, then we
  default to a remote read from the locale on which the element is located.

  Any write to array data will be applied to the actual element, the same as if
  you were using a Block-distributed array.

  **Modifying Exterior Ghost Cells**

  Updating the outermost ghost cells can be useful when working with a periodic
  stencil-distributed array. If your array contains position information, you may
  want to modify the ghost cells to 'wrap' around the physical space correctly.

  You can currently do this with the ``boundaries()`` iterator on a
  stencil-distributed array. This iterator yields a tuple where the first component
  is the ghost cell element to be modified, and the second component is a tuple
  indicating the side on which this ghost cell lives. This direction tuple will
  contain values in the range ``-1..1``.

  The release benchmark 'miniMD' contains an example of how one might use this
  iterator.

  .. warning::

    There is a known issue with this iterator where the program will fail to
    compile if the array element is not an array or a class.

*/
class Stencil : BaseDist {
  param rank: int;
  type idxType = int;
  param ignoreFluff: bool;
  var boundingBox: domain(rank, idxType);
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] LocStencil(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
  var fluff: rank*idxType;
  var periodic: bool = false;
}

//
// Local Stencil Distribution Class
//
// rank : generic rank that matches Stencil.rank
// idxType: generic index type that matches Stencil.idxType
// myChunk: a non-distributed domain that defines this locale's indices
//
class LocStencil {
  param rank: int;
  type idxType;
  const myChunk: domain(rank, idxType);
}

//
// Stencil Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class StencilDom: BaseRectangularDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  param ignoreFluff : bool;
  const dist: Stencil(rank, idxType, ignoreFluff);
  var locDoms: [dist.targetLocDom] LocStencilDom(rank, idxType, stridable);
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  var fluff: rank*idxType;
  var periodic: bool = false;
  var wholeFluff : domain(rank=rank, idxType=idxType, stridable=stridable);
}

//
// Local Stencil Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// myBlock: a non-distributed domain that defines the local indices
//
class LocStencilDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var myBlock, myFluff: domain(rank, idxType, stridable);
  var NeighDom: domain(rank);
  var Dest, Src: [NeighDom] domain(rank, idxType, stridable);
  var Neighs: [NeighDom] rank*int;
}

//
// Stencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class StencilArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  param ignoreFluff: bool;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: StencilDom(rank, idxType, stridable, ignoreFluff);
  var locArr: [dom.dist.targetLocDom] LocStencilArr(eltType, rank, idxType, stridable);
  var myLocArr: LocStencilArr(eltType, rank, idxType, stridable);
  const SENTINEL = max(rank*idxType);
}

//
// Local Stencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocStencilArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  const locDom: LocStencilDom(rank, idxType, stridable);
  var locRAD: LocRADCache(eltType, rank, idxType, stridable); // non-nil if doRADOpt=true
  var myElems: [locDom.myFluff] eltType;
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
}

private proc makeZero(param rank : int) {
  var ret : rank*int;
  return ret;
}
//
// Stencil constructor for clients of the Stencil distribution
//
proc Stencil.Stencil(boundingBox: domain,
                targetLocales: [] locale = Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.idxType,
                fluff: rank*idxType = makeZero(rank), 
                periodic: bool = false,
                param ignoreFluff = false) {
  if rank != boundingBox.rank then
    compilerError("specified Stencil rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified Stencil index type != index type of specified bounding box");

  this.boundingBox = boundingBox : domain(rank, idxType, stridable=false);
  this.fluff = fluff;

  // can't have periodic if there's no fluff
  this.periodic = periodic && !zeroTuple(fluff);

  setupTargetLocalesArray(targetLocDom, this.targetLocales, targetLocales);

  const boundingBoxDims = this.boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDist(locid) =  new LocStencil(rank, idxType, locid, boundingBoxDims,
                                     targetLocDomDims);

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  if debugStencilDist {
    writeln("Creating new Stencil distribution:");
    dsiDisplayRepresentation();
  }
}

proc Stencil.dsiAssign(other: this.type) {
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
      locDist(locid) = new LocStencil(rank, idxType, locid, boundingBoxDims,
                                    targetLocDomDims);
}

//
// Stencil distributions are equivalent if they share the same bounding
// box and target locale set.
//
proc Stencil.dsiEqualDMaps(that: Stencil(?)) {
  return (this.boundingBox == that.boundingBox &&
          this.targetLocales.equals(that.targetLocales) &&
          this.fluff == that.fluff &&
          this.periodic == that.periodic);
}


proc Stencil.dsiEqualDMaps(that) param {
  return false;
}



proc Stencil.dsiClone() {
  return new Stencil(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
}

proc Stencil.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

proc Stencil.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

proc Stencil.dsiNewRectangularDom(param rank: int, type idxType,
                                  param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Stencil domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Stencil domain rank does not match distribution's");

  var dom = new StencilDom(rank=rank, idxType=idxType, dist=this, stridable=stridable, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
  dom.dsiSetIndices(inds);
  if debugStencilDist {
    writeln("Creating new Stencil domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

//
// output distribution
//
proc Stencil.writeThis(x) {
  x.writeln("Stencil");
  x.writeln("-------");
  x.writeln("distributes: ", boundingBox);
  x.writeln("across locales: ", targetLocales);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] locale ", locDist(locid).locale.id, " owns chunk: ", locDist(locid).myChunk);
}

proc Stencil.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

proc Stencil.dsiIndexToLocale(ind: rank*idxType) where rank > 1 {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc Stencil.getChunk(inds, locid) {
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
proc Stencil.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx((ind,));
}

proc Stencil.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 1..rank do
    result(i) = max(0, min((targetLocDom.dim(i).length-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).length:idxType) /
                            boundingBox.dim(i).length):int));
  return if rank == 1 then result(1) else result;
}

proc Stencil.dsiCreateReindexDist(newSpace, oldSpace) {
  proc anyStridable(space, param i=1) param
    return if i == space.size then space(i).stridable
           else space(i).stridable || anyStridable(space, i+1);

  // Should this error be in ChapelArray or not an error at all?
  if newSpace(1).idxType != oldSpace(1).idxType then
    compilerError("index type of reindex domain must match that of original domain");
  if anyStridable(newSpace) || anyStridable(oldSpace) then
    compilerWarning("reindexing stridable Stencil arrays is not yet fully supported");

  /* To shift the bounding box, we must perform the following operation for
   *  each dimension:
   *
   *   bbox(r).low - (oldSpace(r).low - newSpace(r).low)
   *   bbox(r).high - (oldSpace(r).low - newSpace(r).low)
   *
   * The following is guaranteed on entry:
   *
   *   oldSpace(r).low-newSpace(r).low = oldSpace(r).high-newSpace(r).high
   *
   * We need to be able to do this without going out of range of the index
   *  type.  The approach we take is to check if there is a way to perform
   *  the calculation without having any of the intermediate results go out
   *  of range.
   *
   *    newBbLow = bbLow - (oldLow - newLow)
   *    newBbLow = bbLow - oldLow + newLow
   *
   * Can be performed as:
   *
   *    t = oldLow-newLow;
   *    newBbLow = bbLow-t;
   * or
   *    t = bbLow-oldLow;
   *    newBbLow = t+newLow;
   * or
   *    t = bbLow+newLow;
   *    newBbLow = t-oldLow;
   *
   */
  proc adjustBound(bbound, oldBound, newBound) {
    var t: bbound.type;
    if safeSub(oldBound, newBound) {
      t = oldBound-newBound;
      if safeSub(bbound, t) {
        return (bbound-t, true);
      }
    }
    if safeSub(bbound, oldBound) {
      t = bbound-oldBound;
      if safeAdd(t, newBound) {
        return (t+newBound, true);
      }
    }
    if safeAdd(bbound, newBound) {
      t = bbound+newBound;
      if safeSub(t, oldBound) {
        return(t-oldBound, true);
      }
    }
    return (bbound, false);
  }

  var myNewBbox = boundingBox.dims();
  for param r in 1..rank {
    var oldLow = oldSpace(r).low;
    var newLow = newSpace(r).low;
    var oldHigh = oldSpace(r).high;
    var newHigh = newSpace(r).high;
    var valid: bool;
    if oldLow != newLow {
      (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldLow,newLow);
      if !valid then // try with high
        (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldHigh,newHigh);
      if !valid then
        halt("invalid reindex for Stencil: distribution bounding box (low) out of range in dimension ", r);

      (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldHigh,newHigh);
      if !valid then
        (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldLow,newLow);
      if !valid then // try with low
        halt("invalid reindex for Stencil: distribution bounding box (high) out of range in dimension ", r);
    }
  }
  var d = {(...myNewBbox)};
  var newDist = new Stencil(d, targetLocales, 
                          dataParTasksPerLocale, dataParIgnoreRunningTasks,
                          dataParMinGranularity, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
  return newDist;
}


proc LocStencil.LocStencil(param rank: int,
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

proc StencilDom.dsiMyDist() return dist;

proc StencilDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
}

proc StencilDom.dsiDims() return whole.dims();

proc StencilDom.dsiDim(d: int) return whole.dim(d);

// stopgap to avoid accessing locDoms field (and returning an array)
proc StencilDom.getLocDom(localeIdx) return locDoms(localeIdx);


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


iter StencilDom.these() {
  for i in whole do
    yield i;
}

iter StencilDom.these(param tag: iterKind) where tag == iterKind.leader {
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
    const tmpStencil = locDom.myBlock.chpl__unTranslate(wholeLow);
    var locOffset: rank*idxType;
    for param i in 1..tmpStencil.rank do
      locOffset(i) = tmpStencil.dim(i).first/tmpStencil.dim(i).stride:strType;
    // Forward to defaultRectangular
    for followThis in tmpStencil._value.these(iterKind.leader, maxTasks,
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
iter StencilDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag then
    chpl__testPar("Stencil domain follower invoked on ", followThis);

  var t: rank*range(idxType, stridable=stridable||anyStridable(followThis));
  type strType = chpl__signedType(idxType);
  for param i in 1..rank {
    var stride = whole.dim(i).stride: strType;
    // not checking here whether the new low and high fit into idxType
    var low = (stride * followThis(i).low:strType):idxType;
    var high = (stride * followThis(i).high:strType):idxType;
    t(i) = ((low..high by stride:strType) + whole.dim(i).low by followThis(i).stride:strType).safeCast(t(i).type);
  }
  for i in {(...t)} {
    yield i;
  }
}

//
// output domain
//
proc StencilDom.dsiSerialWrite(x) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
proc StencilDom.dsiBuildArray(type eltType) {
  var arr = new StencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this, ignoreFluff=this.ignoreFluff);
  arr.setup();
  return arr;
}

proc StencilDom.dsiNumIndices return whole.numIndices;
proc StencilDom.dsiLow return whole.low;
proc StencilDom.dsiHigh return whole.high;
proc StencilDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for a rectangular
// domain take a domain rather than something else?
//
proc StencilDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 1..rank {
      absFluff(i) = abs(fluff(i) * x.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
  setup();
  if debugStencilDist {
    writeln("Setting indices of Stencil domain:");
    dsiDisplayRepresentation();
  }
}

proc StencilDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 1..rank {
      absFluff(i) = abs(fluff(i) * whole.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
  setup();
  if debugStencilDist {
    writeln("Setting indices of Stencil domain:");
    dsiDisplayRepresentation();
  }
}

proc StencilDom.dsiGetIndices() {
  return whole.getIndices();
}

// dsiLocalSlice
proc StencilDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

proc StencilDom.setup() {
  coforall localeIdx in dist.targetLocDom do {
    on dist.targetLocales(localeIdx) {
      ref myLocDom = locDoms(localeIdx);

      // Create a domain that points to the nearest neighboring locales
      var nearest : rank*range;
      for param i in 1..rank do nearest(i) = -1..1;
      const ND : domain(rank) = nearest;

      var abstr : rank*whole.dim(1).stride.type;
      for param i in 1..rank {
        abstr(i) = abs(whole.dim(i).stride);
      }

      if myLocDom == nil {
        myLocDom = new LocStencilDom(rank, idxType, stridable,
                                             dist.getChunk(whole, localeIdx), NeighDom=ND);
      } else {
        myLocDom.myBlock = dist.getChunk(whole, localeIdx);
      }

      if !zeroTuple(fluff) && myLocDom.myBlock.numIndices!=0 then {
        myLocDom.myFluff = myLocDom.myBlock.expand(fluff*abstr);
      }
      else {
        myLocDom.myFluff = myLocDom.myBlock;
      }

      if !zeroTuple(fluff) && myLocDom.myBlock.size > 0 {
        //
        // Recompute Src and Dest domains, later used to update fluff regions
        //
        for (S, D, off) in zip(myLocDom.Src, myLocDom.Dest, ND) {
          const to = if isTuple(off) then off else (off,);
          var dr : rank*whole.dim(1).type;
          for i in 1..rank {
            const fa = fluff(i) * abstr(i);
            const cur = myLocDom.myBlock.dim(i);
            if to(i) < 0 then
              dr(i) = cur.first - fa .. cur.first-abstr(i);
            else if to(i) > 0 then
              dr(i) = cur.last+abstr(i)..cur.last+fa;
            else
              dr(i) = cur.first..cur.last;
            if stridable then
              dr(i) = dr(i) by cur.stride;
          }
          D = {(...dr)};
          assert(wholeFluff.member(D.first) && wholeFluff.member(D.last), "StencilDist: Error computing destination slice.");
          if periodic then S = D;
        }

        for (N, L) in zip(myLocDom.Neighs, ND) {
          if zeroTuple(L) then continue;
          if rank == 1 then
            N(1) = localeIdx + L;
          else
            N = localeIdx + L;
        }

        if periodic {
          for (S, D, N, L) in zip(myLocDom.Src, myLocDom.Dest, myLocDom.Neighs, ND) {
            if zeroTuple(L) then continue; // Skip center

            var offset : rank*idxType;
            for i in 1..rank {
              if S.dim(i).low > whole.dim(i).high then
                offset(i) = -whole.dim(i).size * abstr(i);
              else if S.dim(i).high < whole.dim(i).low then
                offset(i) = whole.dim(i).size * abstr(i);
            }

            S = S.translate(offset);
            if rank == 1 then
              N(1) = dist.targetLocsIdx(S.first);
            else
              N = dist.targetLocsIdx(S.first);
            assert(whole.member(S.first) && whole.member(S.last), "StencilDist: Failure to compute Src slice.");
            assert(dist.targetLocDom.member(N), "StencilDist: Error computing neighbor index.");
          }
        }
      }
    }
  }
}

proc StencilDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom {
    on locDoms(localeIdx) do
      delete locDoms(localeIdx);
  }
}

proc StencilDom.dsiMember(i) {
  return wholeFluff.member(i);
}

proc StencilDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocStencilDom.member(i) return myBlock.member(i);

proc StencilArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
  }
}

proc StencilArr.dsiGetBaseDom() return dom;

//
// NOTE: Each locale's myElems array must be initialized prior to setting up
// the RAD cache.
//
proc StencilArr.setupRADOpt() {
  for localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const myLocArr = locArr(localeIdx);
      if myLocArr.locRAD != nil {
        delete myLocArr.locRAD;
        myLocArr.locRAD = nil;
      }
      if disableStencilLazyRAD {
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

proc StencilArr.setup() {
  var thisid = this.locale.id;
  var nearest : rank*range;
  for param i in 1..nearest.size do nearest(i) = -1..1;
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const locDom = dom.getLocDom(localeIdx);
      locArr(localeIdx) = new LocStencilArr(eltType, rank, idxType, stridable, locDom);
      if thisid == here.id then
        myLocArr = locArr(localeIdx);
    }
  }

  if doRADOpt && disableStencilLazyRAD then setupRADOpt();
}

proc StencilArr.dsiDestroyArr(isslice : bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      if !ignoreFluff then
        delete locArr(localeIdx);
    }
  }
}

// Re-use _remoteAccessData.getDataIndex from BlockDist


inline proc StencilArr.dsiLocalAccess(i: rank*idxType) ref {
  return myLocArr.this(i);
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in targetLocsIdx?
//
// BHARSH TODO: Like BlockDist, 'idx' should probably have a 'const in' or 'in'
// intent
//
inline
proc StencilArr.do_dsiAccess(param setter, idx: rank*idxType) ref {
  var i = idx;
  local {
    if myLocArr != nil {
      if setter || this.ignoreFluff {
        // A write: return from actual data and not fluff
        if myLocArr.locDom.member(i) then return myLocArr.this(i);
      } else {
        // A read: return from fluff if possible
        // If there is no fluff, then myFluff == myBlock
        if myLocArr.locDom.myFluff.member(i) then return myLocArr.this(i);
      }
    }
  }

  return nonLocalAccess(i);
}

proc StencilArr.nonLocalAccess(i: rank*idxType) ref {

  if doRADOpt {
    if myLocArr {
      if boundsChecking {
        if !dom.wholeFluff.member(i) {
          halt("array index out of bounds: ", i);
        }
      }
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableStencilLazyRAD {
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
      if radata(rlocIdx).shiftedDataChunk(0) != nil {
        var dataIdx = radata(rlocIdx).getDataIndex(i);
        return radata(rlocIdx).getDataElem(dataIdx);
      }
    }
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

// ref version
inline proc StencilArr.dsiAccess(i: rank*idxType) ref {
  return do_dsiAccess(true, i);
}
// value version for POD types
inline proc StencilArr.dsiAccess(i: rank*idxType)
where shouldReturnRvalueByValue(eltType) {
  return do_dsiAccess(false, i);
}
// const ref version for types with copy-ctor
inline proc StencilArr.dsiAccess(i: rank*idxType) const ref
where shouldReturnRvalueByConstRef(eltType) {
  return do_dsiAccess(false, i);
}


// ref version
inline proc StencilArr.dsiAccess(i: idxType...rank) ref
  return dsiAccess(i);
// value version for POD types
inline proc StencilArr.dsiAccess(i: idxType...rank)
where shouldReturnRvalueByValue(eltType)
  return dsiAccess(i);
// const ref version for types with copy-ctor
inline proc StencilArr.dsiAccess(i: idxType...rank) const ref
where shouldReturnRvalueByConstRef(eltType)
  return dsiAccess(i);


iter StencilArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
iter StencilArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}

proc StencilArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

proc StencilArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain._value == this.dom;

proc StencilArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead._value == this.dom;

iter StencilArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag {
    if fast then
      chpl__testPar("Stencil array fast follower invoked on ", followThis);
    else
      chpl__testPar("Stencil array non-fast follower invoked on ", followThis);
  }

  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Stencil array");

  var myFollowThis: rank*range(idxType=idxType, stridable=stridable || anyStridable(followThis));
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    // NOTE: Not bothering to check to see if these can fit into idxType
    var low = followThis(i).low * abs(stride):idxType;
    var high = followThis(i).high * abs(stride):idxType;
    myFollowThis(i) = ((low..high by stride) + dom.whole.dim(i).low by followThis(i).stride).safeCast(myFollowThis(i).type);
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

//
// output array
//
proc StencilArr.dsiSerialWrite(f) {
  type strType = chpl__signedType(idxType);
  var binary = f.binary();
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:strType) {
      if ! binary then f.write(" ");
      i(rank) += dom.dsiDim(rank).stride:strType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:strType) {
          i(dim) += dom.dsiDim(dim).stride:strType;
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

pragma "no copy return"
proc StencilArr.dsiLocalSlice(ranges) {
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

private inline proc zeroTuple(t) {
  if isTuple(t) {
    for param i in 1..t.size do
      if t(i) != 0 then return false;
  } else if isIntegral(t) {
    return t == 0;
  }
  else compilerError("Incorrect usage of 'zeroTuple' utility function.");
  return true;
}

iter _array.boundaries() {
  for d in _value.dsiBoundaries() do yield d;
}

iter _array.boundaries(param tag : iterKind) where tag == iterKind.standalone {
  forall d in _value.dsiBoundaries() do yield d;
}

iter StencilArr.dsiBoundaries() {
  for i in dom.dist.targetLocDom {
    var LSA = locArr[i];
    ref myLocDom = LSA.locDom;
    for (D, N, L) in zip(myLocDom.Dest, myLocDom.Neighs, myLocDom.NeighDom) {
      const target = i + L;
      const low = dom.dist.targetLocDom.low;
      const high = dom.dist.targetLocDom.high;

      if (!dom.dist.targetLocDom.member(target)) {
        var translated : target.type;
        for param r in 1..LSA.rank {
          if target(r) < low(r) {
            translated(r) = target(r);
          } else if target(r) > high(r) {
            translated(r) = target(r) - high(r);
          } else {
            translated(r) = 0;
          }
        }

        for el in LSA.myElems[D] do yield (el, translated);
      }
    }
  }
}

//
// Yields any 'fluff' boundary chunks in the StencilArr along with a global coordinate of
// where the chunk lives relative to the core.
//
iter StencilArr.dsiBoundaries(param tag : iterKind) where tag == iterKind.standalone {
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      var LSA = locArr[i];
      ref myLocDom = LSA.locDom;
      forall (D, N, L) in zip(myLocDom.Dest, myLocDom.Neighs, myLocDom.NeighDom) {
        const target = i + L;
        const low = dom.dist.targetLocDom.low;
        const high = dom.dist.targetLocDom.high;
        //
        // if `LSA` has a cache section of the outermost fluff/boundary,
        // then we should yield that so it is updated correctly. To do
        // so, we need to translate the local offset to the global offset.
        // A 2D example:
        //
        //         |-----|-----|
        //  X=>|---|     |     |
        //     |---|-----|-----|
        //         |  A  |     |
        //         |-----|-----|
        //
        // Locale-chunk `A` knows about its cache section `X` by the
        // coordinates (-1, 1). However, `X` is globally on the (-1,0)
        // face of the grid. The logic below performs the translation
        // from the local coordinate to the global coordinate.
        //

        // If the target locale is outside the grid, it's a boundary chunk
        if (!dom.dist.targetLocDom.member(target)) {
          var translated : target.type;
          for param r in 1..LSA.rank {
            if target(r) < low(r) {
              translated(r) = target(r);
            } else if target(r) > high(r) {
              translated(r) = target(r) - high(r);
            } else {
              translated(r) = 0;
            }
          }

          // TODO: should we 'forall' over this instead?
          for el in LSA.myElems[D] do yield (el, translated);
        }
      }
    }
  }
}

//
// Returns a view of this Stencil-distributed array without any fluff.
// Useful for ensuring that reads/writes always operate on non-cached data
//
// Copies the range slice function in ChapelArray.
//
// TODO: Return an ArrayView instead of something that looks like a normal
// array
//
pragma "no copy return"
proc _array.noFluffView() {
  var a = _value.dsiNoFluffView();
  a._arrAlias = _value;
  return _newArray(a);
}

proc StencilArr.dsiNoFluffView() {
  var tempDist = new Stencil(dom.dist.boundingBox, dom.dist.targetLocales,
                             dom.dist.dataParTasksPerLocale, dom.dist.dataParIgnoreRunningTasks,
                             dom.dist.dataParMinGranularity, ignoreFluff=true);
  pragma "no auto destroy" var newDist = _newDistribution(tempDist);
  pragma "no auto destroy" var tempDom = _newDomain(newDist.newRectangularDom(rank, idxType, dom.stridable, dom.whole.dims()));
  newDist._value.add_dom(tempDom._value);

  var newDom = tempDom._value;

  var alias = new StencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=newDom.stridable, dom=newDom, ignoreFluff=true);
  alias.locArr = this.locArr;
  alias.myLocArr = this.myLocArr;

  newDom.add_arr(alias, locking=false);
  return alias;
}

// wrapper
proc _array.updateFluff() {
  _value.dsiUpdateFluff();
}

// copy over into cache
proc StencilArr.dsiUpdateFluff() {
  if zeroTuple(dom.fluff) then return;
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      ref myLocDom = locArr[i].locDom;
      forall (S, D, N, L) in zip(myLocDom.Src, myLocDom.Dest,
          myLocDom.Neighs, myLocDom.NeighDom) {
        if !zeroTuple(L) {
          if !dom.dist.targetLocDom.member(i+L) && dom.periodic then
            locArr[i].myElems[D] = locArr[N].myElems[S];
          else if dom.dist.targetLocDom.member(N) then
            locArr[i].myElems[D] = locArr[N].myElems[D];
        }
      }
    }
  }
}

proc StencilArr.dsiReallocate(d: domain) {
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

proc StencilArr.dsiPostReallocate() {
  // Call this *after* the domain has been reallocated
  if doRADOpt then setupRADOpt();
}

proc StencilArr.setRADOpt(val=true) {
  doRADOpt = val;
  if doRADOpt then setupRADOpt();
}

//
// the accessor for the local array -- assumes the index is local
//
proc LocStencilArr.this(i) ref {
  return myElems(i);
}

//
// Privatization
//
proc Stencil.Stencil(other: Stencil, privateData,
                param rank = other.rank,
                type idxType = other.idxType,
                param ignoreFluff = other.ignoreFluff) {
  boundingBox = {(...privateData(1))};
  targetLocDom = {(...privateData(2))};
  dataParTasksPerLocale = privateData(3);
  dataParIgnoreRunningTasks = privateData(4);
  dataParMinGranularity = privateData(5);
  fluff = privateData(6);
  periodic = privateData(7);

  for i in targetLocDom {
    targetLocales(i) = other.targetLocales(i);
    locDist(i) = other.locDist(i);
  }
}

proc Stencil.dsiSupportsPrivatization() param return true;

proc Stencil.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity,
          fluff, periodic);
}

proc Stencil.dsiPrivatize(privatizeData) {
  return new Stencil(this, privatizeData);
}

proc Stencil.dsiGetReprivatizeData() return boundingBox.dims();

proc Stencil.dsiReprivatize(other, reprivatizeData) {
  boundingBox = {(...reprivatizeData)};
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

proc StencilDom.dsiSupportsPrivatization() param return true;

proc StencilDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc StencilDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(1));
  var c = new StencilDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = {(...privatizeData(2))};
  if c.whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 1..rank {
      absFluff(i) = abs(fluff(i) * c.whole.dim(i).stride);
    }
    c.wholeFluff = c.whole.expand(absFluff);
  }
  return c;
}

proc StencilDom.dsiGetReprivatizeData() return whole.dims();

proc StencilDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = {(...reprivatizeData)};
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 1..rank {
      absFluff(i) = abs(fluff(i) * whole.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
}

proc StencilArr.dsiSupportsPrivatization() param return true;

proc StencilArr.dsiGetPrivatizeData() return dom.pid;

proc StencilArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);
  var c = new StencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom, ignoreFluff=this.ignoreFluff);
  for localeIdx in c.dom.dist.targetLocDom {
    c.locArr(localeIdx) = locArr(localeIdx);
    if c.locArr(localeIdx).locale.id == here.id then
      c.myLocArr = c.locArr(localeIdx);
  }
  return c;
}

proc StencilArr.dsiSupportsBulkTransfer() param return true;
proc StencilArr.dsiSupportsBulkTransferInterface() param return true;

proc StencilArr.doiCanBulkTransfer(viewDom) {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiCanBulkTransfer");

  if dom.stridable then
    for param i in 1..rank do
      if viewDom.dim(i).stride != 1 then return false;

  // See above note regarding aliased arrays
  if disableAliasedBulkTransfer then
    if _arrAlias != nil then return false;

  return true;
}

proc StencilArr.doiCanBulkTransferStride(viewDom) param {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiCanBulkTransferStride");

  return useBulkTransferDist;
}

proc StencilArr.oneDData {
  if defRectSimpleDData {
    return true;
  } else {
    // TODO: do this when we create the array?  if not, then consider:
    // TODO: use locRad oneDData, if available
    // TODO: with more coding complexity we could get a much quicker
    //       answer in the 'false' case, but how to avoid penalizing
    //       the 'true' case at scale?
    var allBlocksOneDData: bool;
    on this {
      var myAllBlocksOneDData: atomic bool;
      myAllBlocksOneDData.write(true);
      forall la in locArr {
        if !la.myElems._value.oneDData then
          myAllBlocksOneDData.write(false);
      }
      allBlocksOneDData = myAllBlocksOneDData.read();
    }
    return allBlocksOneDData;
  }
}

proc StencilArr.doiUseBulkTransfer(B) {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiUseBulkTransfer()");

  //
  // Absent multi-ddata, for the array as a whole, say bulk transfer
  // is possible.  We'll make a final determination for each block in
  // doiBulkTransfer(), based on the characteristics of the blocks
  // themselves.
  //
  // If multi-ddata is possible then we can only do bulk transfer when
  // either the domains are identical (so we can defer the decision as
  // above) or all the blocks of both arrays have but a single ddata
  // chunk.
  //
  if this.rank != B.rank then return false;
  return defRectSimpleDData
         || dom == B._value.dom
         || (oneDData && chpl__getActualArray(B).oneDData);
}

proc StencilArr.doiUseBulkTransferStride(B) {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiUseBulkTransferStride()");

  //
  // Absent multi-ddata, for the array as a whole, say bulk transfer
  // is possible even though as things are currently coded we'll always
  // do regular bulk transfer and never even ask about strided.
  //
  // If multi-ddata is possible then we can only do strided bulk
  // transfer when all the blocks have but a single ddata chunk.
  //
  if this.rank != B.rank then return false;
  return defRectSimpleDData
         || (oneDData && chpl__getActualArray(B).oneDData);
}

proc StencilArr.doiBulkTransfer(B, viewDom) {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransfer");
  const actual = chpl__getActualArray(B);
  const actDom = chpl__getViewDom(B);

  if debugStencilDistBulkTransfer then resetCommDiagnostics();
  var sameDomain: bool;
  // We need to do the following on the locale where 'this' was allocated,
  //  but hopefully, most of the time we are initiating the transfer
  //  from the same locale (local on clauses are optimized out).
  on this do sameDomain = viewDom==actDom;
  // Use zippered iteration to piggyback data movement with the remote
  //  fork.  This avoids remote gets for each access to locArr[i] and
  //  actual.locArr[i]
  coforall (i, myLocArr, BmyLocArr) in zip(this.dom.dist.targetLocDom,
                                        locArr,
                                        actual.locArr) do
    on this.dom.dist.targetLocales(i) {

    if this.rank == B.rank {
      // Take advantage of DefaultRectangular bulk transfer
      if debugStencilDistBulkTransfer then startCommDiagnosticsHere();
      const lview = myLocArr.locDom.myBlock[viewDom];
      const rview = BmyLocArr.locDom.myBlock[actDom];
      myLocArr.myElems[lview] = BmyLocArr.myElems[rview];
      if debugStencilDistBulkTransfer then stopCommDiagnosticsHere();
    } else {
      halt("bulk-transfer called with Stencil of differing rank!");
    }
  }
  if debugStencilDistBulkTransfer then writeln("Comms:",getCommDiagnostics());
}

proc StencilArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc StencilDom.dsiTargetLocales() {
  return dist.targetLocales;
}

proc Stencil.dsiTargetLocales() {
  return targetLocales;
}

// Stencil subdomains are continuous

proc StencilArr.dsiHasSingleLocalSubdomain() param return true;
proc StencilDom.dsiHasSingleLocalSubdomain() param return true;

// returns the current locale's subdomain

proc StencilArr.dsiLocalSubdomain() {
  return myLocArr.locDom.myBlock;
}
proc StencilDom.dsiLocalSubdomain() {
  // TODO -- could be replaced by a privatized myLocDom in StencilDom
  // as it is with StencilArr
  var myLocDom:LocStencilDom(rank, idxType, stridable) = nil;
  for (loc, locDom) in zip(dist.targetLocales, locDoms) {
    if loc == here then
      myLocDom = locDom;
  }
  return myLocDom.myBlock;
}

proc StencilDom.numRemoteElems(rlo,rid){
  // NOTE: Not bothering to check to see if rid+1, length, or rlo-1 used
  //  below can fit into idxType
  var blo,bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank).length - 1) then
    bhi=whole.dim(rank).high;
  else
      bhi=dist.boundingBox.dim(rank).low +
        intCeilXDivByY((dist.boundingBox.dim(rank).high - dist.boundingBox.dim(rank).low +1)*(rid+1):idxType,
                       dist.targetLocDom.dim(rank).length:idxType) - 1:idxType;

  return(bhi - (rlo - 1):idxType);
}

//For assignments of the form: "Stencil = any"
//where "any" means any array that implements the bulk transfer interface
proc StencilArr.doiBulkTransferFrom(Barg, viewDom)
{
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferFrom()");

  if this.rank == Barg.rank {
    const Dest = this;
    const Src = chpl__getActualArray(Barg);
    const srcView = chpl__getViewDom(Barg);
    type el = Dest.idxType;
    coforall i in Dest.dom.dist.targetLocDom do // for all locales
      on Dest.dom.dist.targetLocales(i)
      {
        var regionDest = Dest.dom.locDoms(i).myBlock[viewDom];
        var regionSrc = Src.dom.locDoms(i).myBlock[srcView];
        if regionDest.numIndices>0
        {
          const ini=bulkCommConvertCoordinate(regionDest.first, viewDom, srcView);
          const end=bulkCommConvertCoordinate(regionDest.last, viewDom, srcView);
          const sb=chpl__tuplify(regionSrc.stride);

          var r1,r2: rank * range(idxType = el,stridable = true);
          r2=regionDest.dims();
           //In the case that the number of elements in dimension t for r1 and r2
           //were different, we need to calculate the correct stride in r1
          for param t in 1..rank{
              r1[t] = (ini[t]:el..end[t]:el by sb[t]:el);
              if r1[t].length != r2[t].length then
                r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
          }
        
          if debugStencilDistBulkTransfer then
              writeln("B{",(...r1),"}.ToDR",regionDest);

          Barg._value.doiBulkTransferToDR(Dest.locArr[i].myElems[regionDest], Barg.domain[(...r1)]);
        }
      }
  }
}
 
//For assignments of the form: DR = Stencil 
//(default rectangular array = block distributed array)
proc StencilArr.doiBulkTransferToDR(Barg, viewDom)
{
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferToDR()");

  if this.rank == Barg.rank {
    const Src = this;
    const Dest = chpl__getActualArray(Barg);
    const destView = chpl__getViewDom(Barg);
    type el = Src.idxType;
    coforall j in Src.dom.dist.targetLocDom do
      on Src.dom.dist.targetLocales(j)
      {
        const inters=Src.dom.locDoms(j).myBlock[viewDom];
        if(inters.numIndices>0)
        {
          const ini=bulkCommConvertCoordinate(inters.first, viewDom, destView);
          const end=bulkCommConvertCoordinate(inters.last, viewDom, destView);
          const sa = chpl__tuplify(destView.stride);

          var r1,r2: rank * range(idxType = el,stridable = true);
          for param t in 1..rank
          {
            r2[t] = (chpl__tuplify(inters.first)[t]
                     ..chpl__tuplify(inters.last)[t]
                     by chpl__tuplify(inters.stride)[t]);
            r1[t] = (ini[t]:el..end[t]:el by sa[t]:el);
          }

          if debugStencilDistBulkTransfer then
            writeln("A[",r1,"] = B[",r2,"]");
        
          Barg[(...r1)] = Src.locArr[j].myElems[(...r2)];
        }
      }
  }
}

//For assignments of the form: Stencil = DR 
//(block distributed array = default rectangular)
proc StencilArr.doiBulkTransferFromDR(Barg, viewDom)
{
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferFromDR");

  if this.rank == Barg.rank {
    const Dest = this;
    const srcView = chpl__getViewDom(Barg);
    type el = Dest.idxType;
    coforall j in Dest.dom.dist.targetLocDom do
      on Dest.dom.dist.targetLocales(j)
      {
        const inters=Dest.dom.locDoms(j).myBlock[viewDom];
        if(inters.numIndices>0)
        {
          const ini=bulkCommConvertCoordinate(inters.first, viewDom, srcView);
          const end=bulkCommConvertCoordinate(inters.last, viewDom, srcView);
          const sb = chpl__tuplify(srcView.stride);

          var r1,r2: rank * range(idxType = el,stridable = true);
          for param t in 1..rank
          {
            r2[t] = (chpl__tuplify(inters.first)[t]
                     ..chpl__tuplify(inters.last)[t]
                     by chpl__tuplify(inters.stride)[t]);
            r1[t] = (ini[t]:el..end[t]:el by sb[t]:el);
          }

          if debugStencilDistBulkTransfer then
            writeln("A[",r2,"] = B[",r1,"]");

          Dest.locArr[j].myElems[(...r2)] = Barg[(...r1)];
        }
      }
  }
}
