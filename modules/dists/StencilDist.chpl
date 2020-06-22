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

private use BlockDist;
private use DSIUtil;
private use ChapelUtil;
private use CommDiagnostics;
private use ChapelLocks;
private use ChapelDebugPrint;


//
// These flags are used to output debug information and run extra
// checks when using Stencil.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugStencilDist = false;
config param debugStencilDistBulkTransfer = false;

// TODO: part of initializer?
config param stencilDistAllowPackedUpdateFluff = true;

config param disableStencilDistBulkTransfer = false;

// Instructs the _packedUpdate method to only perform the optimized buffer
// packing if the number of GETs/PUTs would be greater than or equal to the
// value in this config const.
//
// Currently disabled until we can find a way to determine a good value.
//
// TODO: find better heuristic through experimentation
config const stencilDistPackedUpdateMinChunks = 1;

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

  The ``Stencil`` class initializer is defined as follows:

    .. code-block:: chapel

      proc Stencil.init(
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
  var fluff: rank*idxType;
  var periodic: bool = false;
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] unmanaged LocStencil(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
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
  param ignoreFluff : bool;
  const dist: unmanaged Stencil(rank, idxType, ignoreFluff);
  var locDoms: [dist.targetLocDom] unmanaged LocStencilDom(rank, idxType, stridable);
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
// NeighDom will be a rectangular domain where each dimension is the range
// -1..1
//
class LocStencilDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var myBlock, myFluff: domain(rank, idxType, stridable);
  var NeighDom: domain(rank);
  var bufDom : domain(1);
  var recvDest, recvSrc,
      sendDest, sendSrc: [NeighDom] domain(rank, idxType, stridable);
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
class StencilArr: BaseRectangularArr {
  param ignoreFluff: bool;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: unmanaged StencilDom(rank, idxType, stridable, ignoreFluff);
  var locArr: [dom.dist.targetLocDom] unmanaged LocStencilArr(eltType, rank, idxType, stridable);
  pragma "local field"
  var myLocArr: unmanaged LocStencilArr(eltType, rank, idxType, stridable)?;
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
  const locDom: unmanaged LocStencilDom(rank, idxType, stridable);
  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, stridable)?; // non-nil if doRADOpt=true
  pragma "local field" pragma "unsafe" pragma "no auto destroy"
  // may be initialized separately
  // always destroyed explicitly (to control deiniting elts)
  var myElems: [locDom.myFluff] eltType;
  var locRADLock: chpl_LocalSpinlock;

  // TODO: use void type when packed update is disabled
  var recvBufs, sendBufs : [locDom.NeighDom] [locDom.bufDom] eltType;
  var sendRecvFlag : [locDom.NeighDom] atomic bool;

  proc init(type eltType,
            param rank: int,
            type idxType,
            param stridable: bool,
            const locDom: unmanaged LocStencilDom(rank, idxType, stridable),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.stridable = stridable;
    this.locDom = locDom;
    this.myElems = this.locDom.myFluff.buildArray(eltType, initElts=initElts);

    // Even if the array elements don't need to be initialized now,
    // do initialize the fluff.
    if initElts == false {
      if this.locDom.myBlock != this.locDom.myFluff {
        for fluffDom in this.locDom.recvDest {
          forall i in fluffDom {
            pragma "no auto destroy" pragma "unsafe"
            var def: eltType;
            __primitive("=", myElems[i], def);
          }
        }
      }
    }
  }

  proc deinit() {
    // Even if the array elements don't need to be de-initialized now,
    // do de-initialize the fluff.
    param needsDestroy = __primitive("needs auto destroy", eltType);
    if needsDestroy {
      if this.locDom.myBlock != this.locDom.myFluff {
        for fluffDom in this.locDom.recvDest {
          forall i in fluffDom {
            chpl__autoDestroy(myElems[i]);
          }
        }
      }
    }

    // Elements in myElems are deinited in dsiDestroyArr if necessary.
    // Here we need to clean up the rest of the array.
    _do_destroy_array(myElems, deinitElts=false);

    if locRAD != nil then
      delete locRAD;
  }

  // guard against dynamic dispatch resolution trying to resolve
  // write()ing out an array of sync vars and hitting the sync var
  // type's compilerError()
  override proc writeThis(f) throws {
    halt("LocStencilArr.writeThis() is not implemented / should not be needed");
  }
}

private proc makeZero(param rank : int, type idxType) {
  var ret : rank*idxType;
  return ret;
}
//
// Stencil initializer for clients of the Stencil distribution
//
proc Stencil.init(boundingBox: domain,
                  targetLocales: [] locale = Locales,
                  dataParTasksPerLocale=getDataParTasksPerLocale(),
                  dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                  dataParMinGranularity=getDataParMinGranularity(),
                  param rank = boundingBox.rank,
                  type idxType = boundingBox.idxType,
                  fluff: rank*idxType = makeZero(rank, idxType),
                  periodic: bool = false,
                  param ignoreFluff = false) {
  if rank != boundingBox.rank then
    compilerError("specified Stencil rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified Stencil index type != index type of specified bounding box");

  this.rank = rank;
  this.idxType = idxType;
  this.ignoreFluff = ignoreFluff;

  this.boundingBox = boundingBox : domain(rank, idxType, stridable=false);
  this.fluff = fluff;

  // can't have periodic if there's no fluff
  this.periodic = periodic && !isZeroTuple(fluff);

  const ranges = setupTargetLocRanges(rank, targetLocales);
  this.targetLocDom = {(...ranges)};
  this.targetLocales = reshape(targetLocales, this.targetLocDom);

  // Instead of 'dummyLS', we could give 'locDistTemp' a nilable element type.
  const dummyLS = new unmanaged LocStencil(rank, idxType, dummy=true);
  var locDistTemp: [targetLocDom] unmanaged LocStencil(rank, idxType) = dummyLS;

  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDistTemp(locid) = new unmanaged LocStencil(rank, idxType, locid,
                                                    boundingBox, targetLocDom);
  delete dummyLS;
  this.locDist = locDistTemp; //make this a serial loop instead?

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  this.complete();

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
      locDist(locid) = new unmanaged LocStencil(rank, idxType, locid, boundingBoxDims,
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
  return new unmanaged Stencil(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity, fluff=fluff, periodic=periodic,
                   ignoreFluff=this.ignoreFluff);
}

override proc Stencil.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

override proc Stencil.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

override proc Stencil.dsiNewRectangularDom(param rank: int, type idxType,
                                           param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Stencil domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Stencil domain rank does not match distribution's");

  const dummyLSD = new unmanaged LocStencilDom(rank, idxType, stridable);
  var locDomsTemp: [this.targetLocDom]
                  unmanaged LocStencilDom(rank, idxType, stridable) = dummyLSD;
  coforall localeIdx in this.targetLocDom do
    on this.targetLocales(localeIdx) do
      locDomsTemp(localeIdx) =
        new unmanaged LocStencilDom(rank, idxType, stridable,
                                    NeighDom=nearestDom(rank));
  delete dummyLSD;

  var dom = new unmanaged StencilDom(rank, idxType, stridable, ignoreFluff,
             _to_unmanaged(this), locDomsTemp, fluff=fluff, periodic=periodic);
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
proc Stencil.writeThis(x) throws {
  x <~> "Stencil\n";
  x <~> "-------\n";
  x <~> "distributes: " <~> boundingBox <~> "\n";
  x <~> "across locales: " <~> targetLocales <~> "\n";
  x <~> "indexed via: " <~> targetLocDom <~> "\n";
  x <~> "resulting in: " <~> "\n";
  for locid in targetLocDom do
    x <~> "  [" <~> locid <~> "] locale " <~> locDist(locid).locale.id <~>
      " owns chunk: " <~> locDist(locid).myChunk <~> "\n";
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
proc Stencil.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx((ind,));
}

proc Stencil.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 0..rank-1 do
    result(i) = max(0, min((targetLocDom.dim(i).size-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).size:idxType) /
                            boundingBox.dim(i).size):int));
  return if rank == 1 then result(0) else result;
}

// TODO: This will not trigger the bounded-coforall optimization
iter Stencil.activeTargetLocales(const space : domain = boundingBox) {
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

proc LocStencil.init(param rank: int,
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
proc LocStencil.init(param rank, type idxType, param dummy: bool) where dummy {
  this.rank = rank;
  this.idxType = idxType;
}

override proc StencilDom.dsiMyDist() return dist;

override proc StencilDom.dsiDisplayRepresentation() {
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
    if i == args.size-1 {
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
    for param i in 0..tmpStencil.rank-1 do
      locOffset(i) = tmpStencil.dim(i).first/tmpStencil.dim(i).stride:strType;
    // Forward to defaultRectangular
    for followThis in tmpStencil.these(iterKind.leader, maxTasks,
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
iter StencilDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag then
    chpl__testPar("Stencil domain follower invoked on ", followThis);

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
proc StencilDom.dsiSerialWrite(x) {
  x <~> whole;
}

//
// how to allocate a new array over this domain
//
proc StencilDom.dsiBuildArray(type eltType, param initElts:bool) {
  const dom = this;
  const creationLocale = here.id;
  const dummyLSD = new unmanaged LocStencilDom(rank, idxType, stridable);
  const dummyLSA = new unmanaged LocStencilArr(eltType, rank, idxType,
                                               stridable, dummyLSD, false);
  var locArrTemp: [dom.dist.targetLocDom]
        unmanaged LocStencilArr(eltType, rank, idxType, stridable) = dummyLSA;
  var myLocArrTemp: unmanaged LocStencilArr(eltType, rank, idxType, stridable)?;

  // formerly in StencilArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref myLocArrTemp) {
    on dom.dist.targetLocales(localeIdx) {
      const LSA = new unmanaged LocStencilArr(eltType, rank, idxType, stridable,
                                              dom.getLocDom(localeIdx),
                                              initElts=initElts);
      locArrTemp(localeIdx) = LSA;
      if here.id == creationLocale then
        myLocArrTemp = LSA;
    }
  }
  delete dummyLSA, dummyLSD;

  var arr = new unmanaged StencilArr(eltType=eltType, rank=rank,
        idxType=idxType, stridable=stridable, ignoreFluff=this.ignoreFluff,
        dom=_to_unmanaged(this), locArr=locArrTemp, myLocArr=myLocArrTemp);

  // formerly in StencilArr.setup()
  if arr.doRADOpt && disableStencilLazyRAD then arr.setupRADOpt();

  return arr;
}

proc StencilDom.dsiNumIndices return whole.size;
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
    for param i in 0..rank-1 {
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
  if x(0).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
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

proc StencilDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

proc StencilDom.dsiGetIndices() {
  return whole.getIndices();
}

// dsiLocalSlice
proc StencilDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

// Create a domain that points to the nearest neighboring locales
private proc nearestDom(param rank) {
  var nearest : rank*range;
  for param i in 0..rank-1 do nearest(i) = -1..1;
  const ND : domain(rank) = nearest;
  return ND;
}

proc StencilDom.setup() {
  coforall localeIdx in dist.targetLocDom {
    on dist.targetLocales(localeIdx) {
      const myLocDom = locDoms(localeIdx);
      const ND = nearestDom(rank);

      var abstr : rank*whole.dim(0).stride.type;
      for param i in 0..rank-1 {
        abstr(i) = abs(whole.dim(i).stride);
      }

      myLocDom.myBlock = dist.getChunk(whole, localeIdx);

      if !isZeroTuple(fluff) && myLocDom.myBlock.size != 0 then {
        myLocDom.myFluff = myLocDom.myBlock.expand(fluff*abstr);
      } else {
        myLocDom.myFluff = myLocDom.myBlock;
      }

      if !isZeroTuple(fluff) && myLocDom.myBlock.size > 0 {
        //
        // Recompute Src and Dest domains, later used to update fluff regions
        //
        const blockDims = myLocDom.myBlock.dims();
        var bufLen = 1;
        forall (recvS, recvD, sendS, sendD, N, L) in zip(myLocDom.recvSrc, myLocDom.recvDest,
                                                         myLocDom.sendSrc, myLocDom.sendDest,
                                                         myLocDom.Neighs, ND)
          with ( max reduce bufLen) {

          // if L is a tuple of zeroes then we are in the center, which needs
          // no updating.
          const to = chpl__tuplify(L);

          // If halo(i) is zero then there cannot be any fluff along that
          // dimension, meaning that neigh(i) cannot be '1' or '-1'. If neigh(i)
          // is not zero, then we should skip the computation of send/recv
          // slices, which will prevent updateFluff from attempting to use
          // those slices later.
          var skipNeigh = || reduce for (f,t) in zip(fluff, to) do (f == 0 && t != 0);

          if !isZeroTuple(to) && !skipNeigh {
            var dr : rank*whole.dim(0).type;
            for i in 0..rank-1 {
              const fa = fluff(i) * abstr(i);
              const cur = blockDims(i);

              // TODO: simplify?
              if to(i) < 0 then
                dr(i) = cur.first - fa .. cur.first-abstr(i);
              else if to(i) > 0 then
                dr(i) = cur.last+abstr(i)..cur.last+fa;
              else
                dr(i) = cur.first..cur.last;

              if stridable then
                dr(i) = dr(i) by cur.stride;
            }

            // destination indices in the fluff region
            recvD = {(...dr)};
            assert(wholeFluff.contains(recvD.first) && wholeFluff.contains(recvD.last), "StencilDist: Error computing destination slice.");

            if whole.contains(recvD.first) && whole.contains(recvD.last) {
              // sending to an adjacent locale
              recvS = recvD;
              N = chpl__tuplify(dist.targetLocsIdx(recvS.first));
            } else if periodic {
              // sending to a non-adjacent locale
              var offset : rank*idxType;
              for i in 0..rank-1 {
                const wd = whole.dim(i);
                const rd = recvD.dim(i);
                const mult = if rd.low > wd.high then -1
                             else if rd.high < wd.low then 1
                             else 0;
                offset(i) = mult * wd.size * abstr(i);
              }
              recvS = recvD.translate(offset);
              N = chpl__tuplify(dist.targetLocsIdx(recvS.first));

              assert(whole.contains(recvS.first) && whole.contains(recvS.last), "StencilDist: Failure to compute Src slice.");
              assert(dist.targetLocDom.contains(N), "StencilDist: Error computing neighbor index.");
            } else {
              // No communication needed in this direction
              assert(recvS.size == 0);
              N = chpl__tuplify(localeIdx);

              // zero out recvD so we can test its size later to determine if
              // communication needs to happen
              recvD = recvS;
            }

            if recvS.size != 0 {
              const offset = -1 * L * fluff * abstr;
              sendS = recvD.translate(offset);
              sendD = recvS.translate(offset);
            }

            bufLen = max(bufLen, recvD.size, recvS.size);

            if recvS.size != 0 && debugStencilDist {
              writeln("L = ", L, ": ");
              writeln("RECV: ", here, ".", recvD, " = ", dist.targetLocales[N], ".", recvS);
              writeln("SEND: ", here, ".", sendS, " = ", dist.targetLocales[N], ".", sendD);
            }
          }
        }
        myLocDom.bufDom = {1..bufLen};
      }
    }
  }

  // Validate the compute send/recv src/dest domains
  if debugStencilDist {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocales(localeIdx) {
        const myLocDom = locDoms(localeIdx);
        const ND = nearestDom(rank);

        forall (recvS, recvD, sendS, sendD, N, L) in zip(myLocDom.recvSrc, myLocDom.recvDest,
                                                         myLocDom.sendSrc, myLocDom.sendDest,
                                                         myLocDom.Neighs, ND) {
          if !isZeroTuple(L) && recvS.size != 0 {
            var other = -1 * L;

            proc checker(me, myDom, them, themDom) {
              writeln(dist.targetLocales(me), ".", myDom, " == ", dist.targetLocales(them), ".", themDom);
              assert(myDom == themDom, "StencilDist Error: failure to match recv and send domains.");
            }

            checker(localeIdx, recvS, N, locDoms[N].sendSrc[other]);
            checker(localeIdx, recvD, N, locDoms[N].sendDest[other]);
          }
        }
      }
    }
  }
}

override proc StencilDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom {
    on locDoms(localeIdx) do
      delete locDoms(localeIdx);
  }
}

proc StencilDom.dsiMember(i) {
  return wholeFluff.contains(i);
}

proc StencilDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocStencilDom.contains(i) return myBlock.contains(i);

override proc StencilArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
  }
}

override proc StencilArr.dsiGetBaseDom() return dom;

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
        myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
        for l in dom.dist.targetLocDom {
          if l != localeIdx {
            myLocArr.locRAD.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
          }
        }
      }
    }
  }
}

override proc StencilArr.dsiElementInitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      locArr(localeIdx).myElems.dsiElementInitializationComplete();
    }
  }
}


override proc StencilArr.dsiDestroyArr(param deinitElts:bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      var arr = locArr(localeIdx);
      if !ignoreFluff {
        if deinitElts {
          // only deinitialize non-fluff elements
          // fluff is always deinited in the LocArr deinit
          param needsDestroy = __primitive("needs auto destroy", eltType);
          if needsDestroy {
            if _deinitElementsIsParallel(eltType) {
              forall i in arr.locDom.myBlock {
                chpl__autoDestroy(arr.myElems[i]);
              }
            } else {
              for i in arr.locDom.myBlock {
                chpl__autoDestroy(arr.myElems[i]);
              }
            }
          }
        }
        delete arr;
      }
    }
  }
}

// Re-use _remoteAccessData.getDataIndex from BlockDist


inline proc StencilArr.dsiLocalAccess(i: rank*idxType) ref {
  return _to_nonnil(myLocArr).this(i);
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in targetLocsIdx?
//
inline
proc StencilArr.do_dsiAccess(param setter, const in idx: rank*idxType) ref {
  local {
    if myLocArr != nil {
      const myLocArr = _to_nonnil(this.myLocArr);
      if setter || this.ignoreFluff {
        // A write: return from actual data and not fluff
        if myLocArr.locDom.contains(idx) then return myLocArr.this(idx);
      } else {
        // A read: return from fluff if possible
        // If there is no fluff, then myFluff == myBlock
        if myLocArr.locDom.myFluff.contains(idx) then return myLocArr.this(idx);
      }
    }
  }

  return nonLocalAccess(idx);
}

proc StencilArr.nonLocalAccess(i: rank*idxType) ref {

  if doRADOpt {
    if myLocArr {
      const myLocArr = _to_nonnil(this.myLocArr);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableStencilLazyRAD {
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
      pragma "no copy" pragma "no auto destroy" var myLocRAD = _to_nonnil(myLocArr.locRAD);
      pragma "no copy" pragma "no auto destroy" var radata = myLocRAD.RAD;
      if radata(rlocIdx).shiftedData != nil {
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

inline proc StencilArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.wholeFluff.contains(i);
}

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

override proc StencilArr.dsiStaticFastFollowCheck(type leadType) param {
  if isSubtype(leadType, StencilArr) {
    var x : leadType?;
    return _to_borrowed(x!.dom.type) == _to_borrowed(this.dom.type);
  } else {
    return _to_borrowed(leadType) == _to_borrowed(this.dom.type);
  }
}

proc StencilArr.dsiDynamicFastFollowCheck(lead: [])
  return this.dsiDynamicFastFollowCheck(lead.domain);

proc StencilArr.dsiDynamicFastFollowCheck(lead: domain) {
  return lead.dist.dsiEqualDMaps(this.dom.dist) && lead._value.whole == this.dom.whole;
}

iter StencilArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
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
      const narrowArrSection =
        __primitive("_wide_get_addr", arrSection):(arrSection.type?);
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

//
// output array
//
proc StencilArr.dsiSerialWrite(f) {
  type strType = chpl__signedType(idxType);
  var binary = f.binary();
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 0..rank-1 do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f <~> do_dsiAccess(true, i);
    if i(rank-1) <= (dom.dsiDim(rank-1).high - dom.dsiDim(rank-1).stride:strType) {
      if ! binary then f <~> " ";
      i(rank-1) += dom.dsiDim(rank-1).stride:strType;
    } else {
      for dim in 0..rank-2 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:strType) {
          i(dim) += dom.dsiDim(dim).stride:strType;
          for dim2 in dim+1..rank-1 {
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
proc StencilArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 0..rank-1 {
    low(i) = ranges(i).low;
  }
  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 0;

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
  var j: int = 0;

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

private inline proc isZeroTuple(t) {
  if isTuple(t) {
    for param i in 0..t.size-1 do
      if t(i) != 0 then return false;
  } else if isIntegral(t) {
    return t == 0;
  }
  else compilerError("Incorrect usage of 'isZeroTuple' utility function.");
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
    for (D, N, L) in zip(myLocDom.recvDest, myLocDom.Neighs, myLocDom.NeighDom) {
      const target = i + L;
      const low = dom.dist.targetLocDom.low;
      const high = dom.dist.targetLocDom.high;

      if (!dom.dist.targetLocDom.contains(target)) {
        var translated : target.type;
        for param r in 0..LSA.rank-1 {
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
      forall (D, N, L) in zip(myLocDom.recvDest, myLocDom.Neighs, myLocDom.NeighDom) {
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
        if (!dom.dist.targetLocDom.contains(target)) {
          var translated : target.type;
          for param r in 0..LSA.rank-1 {
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
proc StencilArr.noFluffView() {
  var tempDist = new unmanaged Stencil(dom.dist.boundingBox, dom.dist.targetLocales,
                             dom.dist.dataParTasksPerLocale, dom.dist.dataParIgnoreRunningTasks,
                             dom.dist.dataParMinGranularity, ignoreFluff=true);
  pragma "no auto destroy" var newDist = new _distribution(tempDist);
  pragma "no auto destroy" var tempDom = new _domain(newDist, rank, idxType, dom.stridable, dom.whole.dims());
  newDist._value._free_when_no_doms = true;

  var newDom = tempDom._value;
  newDom._free_when_no_arrs = true;

  var alias = new unmanaged StencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=newDom.stridable, dom=newDom, ignoreFluff=true, locArr=locArr, myLocArr=myLocArr);

  newDom.add_arr(alias, locking=false);
  return _newArray(alias);
}

//
// On each locale, update the local cache by fetching data from neighbors.
//
// TODO: What's the best way to deal with the repeated `locArr[i]` expressions?
// Note that this is actually `this.locArr[i]`, where 'this' is on the locale
// where the coforall-on starts. What we really want is something like this:
//
// const curLocArr = chpl_getPrivatizedCopy(this.type, this.pid).locArr[i];
//
// Ideally the compiler could do something like this for us...
//
proc StencilArr.naiveUpdateFluff() {
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      ref myLocDom = locArr[i].locDom;
      forall (S, D, N, L) in zip(myLocDom.recvSrc, myLocDom.recvDest,
          myLocDom.Neighs, myLocDom.NeighDom) {
        // A source domain of size zero indicates that no communication
        // is necessary.
        //
        // if "L" is zero, that indicates we are at the center of the stencil
        // and do not need to update
        if !isZeroTuple(L) && S.size != 0 {
          locArr[i].myElems[D] = locArr[N].myElems[S];
        }
      }
    }
  }
}

//
// TODO: GET or PUT the data? Does it matter for performance?
//
// TODO: should we avoid doing the packed transfer for dense-ish regions?
// e.g. {1..5, 1..100} might only require 5 GETs/PUTs
//
// This is an optimized variant of the naiveUpdateFluff method. This variant
// packs elements into a buffer such that we only do one GET/PUT. This is
// most beneficial when the communicated region would require many GETs in
// the naive approach. For example, Let's say we have a 10x10 domain with
// a fluff of (1,1). Our wholeFluff domain would be {0..11, 0..11}. When we go
// to communicate the region {1..10,1..1}, the naive method requires 10 GETs or
// PUTs, whereas the packed method requires just one.
//
// On each locale:
// 1) Repeat the following for each region of data that other locales will
//    want to fetch:
//    a) Serialize/pack the region of data into a 1D buffer array. This is the
//       buffer that other locales will read from.
//    b) Write 'true' to an atomic flag on the local that intends to read from
//       the buffer that was just populated.
// 2) Once we have prepared our local data for reading, repeat the following
//    for each region of data that *our* cache wants to update:
//    a) Wait for the corresponding atomic flag to be 'true' so that we know
//       it is safe to fetch the data from the remote locale. Reset the flag to
//       false afterwards.
//    b) Bulk-copy the remote buffer into a local buffer
//    c) Copy elements from the local buffer into the cache
//
proc StencilArr._packedUpdate() {
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      var myLocDom = locArr[i].locDom;

      proc translateIdx(idx) {
        return -1 * chpl__tuplify(idx);
      }

      // BHARSH TODO: can we fuse these two foralls? My current concern is that
      // by waiting we might prevent another iteration running and possibly
      // find ourselves in a deadlock.
      forall (D, S, recvIdx, sendBufIdx) in zip(myLocDom.sendDest, myLocDom.sendSrc,
                                                myLocDom.Neighs,
                                                myLocDom.NeighDom) {
        // If S.size == 0, no communication is required
        if S.size != 0 {
          const chunkSize  = max(1, S.dim(rank-1).size); // avoid divide by zero
          const numChunks = S.size / chunkSize;
          if numChunks >= stencilDistPackedUpdateMinChunks {
            const recvBufIdx = translateIdx(sendBufIdx);

            // Pack the buffer
            //
            // TODO: Should we have a serialization helper for N-dimensional
            // DefaultRectangulars into 1-dimension arrays?
            ref src = locArr[i].myElems[S];
            ref buf = locArr[i].sendBufs[sendBufIdx];
            local do for (s, i) in zip(src, buf.domain.first..#src.size) do buf[i] = s;

            if debugStencilDist then
              writeln("Filled ", here, ".", S, " for ", dom.dist.targetLocales(recvIdx), "::", recvBufIdx);
            locArr[recvIdx].sendRecvFlag[recvBufIdx].write(true);
          } else {
            // 'naive' update
            locArr[recvIdx].myElems[D] = locArr[i].myElems[S];
          }
        }
      }
      forall (D, S, srcIdx, recvBufIdx) in zip(myLocDom.recvDest, myLocDom.recvSrc,
                                               myLocDom.Neighs,
                                               myLocDom.NeighDom) {
        const chunkSize  = max(1, S.dim(rank-1).size); // avoid divide by zero
        const numChunks = S.size / chunkSize;

        // If we did a naive update in the previous loop, this iteration does
        // not need to do anything.
        if S.size != 0 && numChunks >= stencilDistPackedUpdateMinChunks {
          const srcBufIdx = translateIdx(recvBufIdx);
          if debugStencilDist then
            writeln(here, "::", recvBufIdx, " WAITING");
          locArr[i].sendRecvFlag[recvBufIdx].waitFor(true); // Can we read yet?
          locArr[i].sendRecvFlag[recvBufIdx].write(false);  // reset for next call

          locArr[i].recvBufs[recvBufIdx][1..D.size] = locArr[srcIdx].sendBufs[srcBufIdx][1..D.size];

          ref dest = locArr[i].myElems[D];
          ref buf = locArr[i].recvBufs[recvBufIdx];
          local do for (d, i) in zip(dest, buf.domain.first..#dest.size) do d = buf[i];
        }
      }
    }
  }
}

proc StencilArr.shouldDoPackedUpdate() param : bool {
  return stencilDistAllowPackedUpdateFluff &&
         chpl__supportedDataTypeForBulkTransfer(eltType);
}

// Update caches
//
// TODO: allow a bool argument here?
//
// TODO: allow for some kind of user-defined packing/unpacking for complicated
// types?
//
// TODO: What if this is called from a locale not in targetLocales? Should
// we do an on-statement?
//
// TODO: Checking for 'dom.dist.targetLocales.size' isn't the most general
// approach. What we really want is to do a naive transfer if the periodic
// neighbor is the current locale.
//
proc StencilArr.updateFluff() {
  if isZeroTuple(dom.fluff) then return;

  if shouldDoPackedUpdate() && dom.dist.targetLocales.size > 1 {
    this._packedUpdate();
  } else {
    this.naiveUpdateFluff();
  }
}

override proc StencilArr.dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable))
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

// Call this *after* the domain has been reallocated
override proc StencilArr.dsiPostReallocate() {
  if doRADOpt then setupRADOpt();
}

proc StencilArr.setRADOpt(val=true) {
  doRADOpt = val;
  if doRADOpt then setupRADOpt();
}

//
// the accessor for the local array -- assumes the index is local
//
inline proc LocStencilArr.this(i) ref {
  return myElems(i);
}

//
// Privatization
//
proc Stencil.init(other: Stencil, privateData,
                  param rank = other.rank,
                  type idxType = other.idxType,
                  param ignoreFluff = other.ignoreFluff) {
  this.rank = rank;
  this.idxType = idxType;
  this.ignoreFluff = ignoreFluff;
  boundingBox = {(...privateData(0))};
  fluff = privateData(5);
  periodic = privateData(6);
  targetLocDom = {(...privateData(1))};
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = privateData(2);
  dataParIgnoreRunningTasks = privateData(3);
  dataParMinGranularity = privateData(4);
}

override proc Stencil.dsiSupportsPrivatization() param return true;

proc Stencil.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity,
          fluff, periodic);
}

proc Stencil.dsiPrivatize(privatizeData) {
  return new unmanaged Stencil(_to_unmanaged(this), privatizeData);
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

override proc StencilDom.dsiSupportsPrivatization() param return true;

proc StencilDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc StencilDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(0));

  var locDomsTemp: [privdist.targetLocDom]
                      unmanaged LocStencilDom(rank, idxType, stridable)
    = locDoms;

  var c = new unmanaged StencilDom(rank, idxType, stridable, ignoreFluff,
            privdist, locDomsTemp, {(...privatizeData(1))}, fluff, periodic);

  if c.whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * c.whole.dim(i).stride);
    }
    c.wholeFluff = c.whole.expand(absFluff);
  }

  return c;
}

proc StencilDom.dsiGetReprivatizeData() return whole.dims();

proc StencilDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = {(...reprivatizeData)};
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * whole.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
}

proc StencilDom.chpl__serialize() {
  return pid;
}

// TODO: What happens when we try to deserialize on a locale that doesn't
// own a copy of the privatized class?  (can that happen?)  Could this
// be a way to lazily privatize by also making the originating locale part
// of the 'data'?
proc type StencilDom.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged StencilDom(rank=this.rank,
                                idxType=this.idxType,
                                stridable=this.stridable,
                                ignoreFluff=this.ignoreFluff),
           data);
}

proc StencilArr.chpl__serialize() {
  return pid;
}

proc type StencilArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged StencilArr(rank=this.rank,
                                idxType=this.idxType,
                                stridable=this.stridable,
                                eltType=this.eltType,
                                ignoreFluff=this.ignoreFluff),
           data);
}

override proc StencilArr.dsiSupportsPrivatization() param return true;

proc StencilArr.dsiGetPrivatizeData() return dom.pid;

proc StencilArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);

  var locArrTemp: [privdom.dist.targetLocDom]
        unmanaged LocStencilArr(eltType, rank, idxType, stridable)
    = locArr;

  var myLocArrTemp: unmanaged LocStencilArr(eltType, rank, idxType, stridable)?;
  for localeIdx in privdom.dist.targetLocDom do
    if locArrTemp(localeIdx).locale.id == here.id then
      myLocArrTemp = locArrTemp(localeIdx);

  var c = new unmanaged StencilArr(eltType=eltType, rank=rank, idxType=idxType,
                       stridable=stridable, ignoreFluff=ignoreFluff,
                       dom=privdom, locArr=locArrTemp, myLocArr=myLocArrTemp);
  return c;
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

proc Stencil.chpl__locToLocIdx(loc: locale) {
  for locIdx in targetLocDom do
    if (targetLocales[locIdx] == loc) then
      return (true, locIdx);
  return (false, targetLocDom.first);
}

// Stencil subdomains are continuous

proc StencilArr.dsiHasSingleLocalSubdomain() param return true;
proc StencilDom.dsiHasSingleLocalSubdomain() param return true;

// returns the current locale's subdomain

proc StencilArr.dsiLocalSubdomain(loc: locale) {
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
proc StencilDom.dsiLocalSubdomain(loc: locale) {
  const (gotit, locid) = dist.chpl__locToLocIdx(loc);
  if (gotit) {
    var inds = chpl__computeBlock(locid, dist.targetLocDom, dist.boundingBox);
    return whole[(...inds)];
  } else {
    var d: domain(rank, idxType, stridable);
    return d;
  }
}

proc StencilDom.numRemoteElems(viewDom, rlo, rid) {
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
private proc canDoAnyToStencil(Dest, destDom, Src, srcDom) param : bool {
  if Src.doiCanBulkTransferRankChange() == false &&
     Dest.rank != Src.rank then return false;

  use Reflection;

  // Does 'Src' support bulk transfers *to* a DefaultRectangular?
  if !canResolveMethod(Src, "doiBulkTransferToKnown", srcDom,
                       Dest.locArr[Dest.locArr.domain.first].myElems._value, destDom) {
    return false;
  }

  return !disableStencilDistBulkTransfer;
}


proc StencilArr.doiBulkTransferToKnown(srcDom, destClass:StencilArr, destDom) : bool
where !disableStencilDistBulkTransfer {
  _doSimpleStencilTransfer(destClass, destDom, this, srcDom);
  return true;
}

proc StencilArr.doiBulkTransferFromKnown(destDom, srcClass:StencilArr, srcDom) : bool
where !disableStencilDistBulkTransfer {
  _doSimpleStencilTransfer(this, destDom, srcClass, srcDom);
  return true;
}

private proc _doSimpleStencilTransfer(Dest, destDom, Src, srcDom) {
  if debugStencilDistBulkTransfer then
    writeln("In Stencil=Stencil Bulk Transfer: Dest[", destDom, "] = Src[", srcDom, "]");

  // Cache to avoid GETs
  const DestPID = Dest.pid;
  const SrcPID = Src.pid;

  coforall i in Dest.dom.dist.activeTargetLocales(destDom) {
    on Dest.dom.dist.targetLocales[i] {
      // Relies on the fact that we privatize across all locales in the
      // program, not just the targetLocales of Dest/Src.
      const dst = if _privatization then chpl_getPrivatizedCopy(Dest.type, DestPID) else Dest;
      const src = if _privatization then chpl_getPrivatizedCopy(Src.type, SrcPID) else Src;

      // Note: Currently StencilDist does not support slicing into the
      // outermost fluff cells, so it is not possible for a BulkTransfer to
      // involve those indices. This means we can simply use 'myBlock' instead
      // of trying to compute something involving fluff.
      //
      // Compute the local portion of the destination domain, and find the
      // corresponding indices in the source's domain.
      const localDestBlock = dst.dom.locDoms[i].myBlock[destDom];
      assert(localDestBlock.size > 0);
      const corSrcBlock    = bulkCommTranslateDomain(localDestBlock, destDom, srcDom);
      if debugStencilDistBulkTransfer then
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

// Overload for any transfer *to* Stencil, if the RHS supports transfers to a
// DefaultRectangular
proc StencilArr.doiBulkTransferFromAny(destDom, Src, srcDom) : bool
where canDoAnyToStencil(this, destDom, Src, srcDom) {

  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferFromAny");

  coforall j in dom.dist.activeTargetLocales(destDom) {
    on dom.dist.targetLocales(j) {
      const Dest = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;

      const inters   = Dest.dom.locDoms(j).myBlock[destDom];
      const srcChunk = bulkCommTranslateDomain(inters, destDom, srcDom);

      if debugStencilDistBulkTransfer then
        writeln("Dest.locArr[", j, "][", inters, "] = Src[", srcDom, "]");

      chpl__bulkTransferArray(Dest.locArr[j].myElems._value, inters, Src, srcChunk);
    }
  }

  return true;
}

// For assignments of the form: DefaultRectangular = Stencil
proc StencilArr.doiBulkTransferToKnown(srcDom, Dest:DefaultRectangularArr, destDom) : bool
where !disableStencilDistBulkTransfer {

  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferToKnown(DefaultRectangular)");

  coforall j in dom.dist.activeTargetLocales(srcDom) {
    on dom.dist.targetLocales(j) {
      const Src = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;
      const inters = Src.dom.locDoms(j).myBlock[srcDom];

      const destChunk = bulkCommTranslateDomain(inters, srcDom, destDom);

      if debugStencilDistBulkTransfer then
        writeln("  A[",destChunk,"] = B[",inters,"]");

      const elemActual = Src.locArr[j].myElems._value;
      chpl__bulkTransferArray(Dest, destChunk, elemActual, inters);
    }
  }

  return true;
}

// For assignments of the form: Stencil = DefaultRectangular
proc StencilArr.doiBulkTransferFromKnown(destDom, Src:DefaultRectangularArr, srcDom) : bool
where !disableStencilDistBulkTransfer {
  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferFromKnown(DefaultRectangular)");

  coforall j in dom.dist.activeTargetLocales(destDom) {
    on dom.dist.targetLocales(j) {
      // Grab privatized copy of 'this' to avoid extra GETs
      const Dest = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      assert(inters.size > 0);

      const srcChunk = bulkCommTranslateDomain(inters, destDom, srcDom);

      if debugStencilDistBulkTransfer then
        writeln("  A[",inters,"] = B[",srcChunk,"]");

      const elemActual = Dest.locArr[j].myElems._value;
      chpl__bulkTransferArray(elemActual, inters, Src, srcChunk);
    }
  }

  return true;
}

override proc StencilArr.doiCanBulkTransferRankChange() param return true;

