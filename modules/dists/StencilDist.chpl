/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

/* Draft support for stencil computations using a Block-style distribution. */

@unstable("StencilDist is unstable and may change in the future")
prototype module StencilDist {

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
config param disableStencilDistArrayViewElision = false;

private config param allowDuplicateTargetLocales = false;
// Instructs the _packedUpdate method to only perform the optimized buffer
// packing if the number of GETs/PUTs would be greater than or equal to the
// value in this config const.
//
// Currently disabled until we can find a way to determine a good value.
//
// TODO: find better heuristic through experimentation
config const stencilDistPackedUpdateMinChunks = 1;

// Re-uses these flags from BlockDist:
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
  The ``stencilDist`` distribution is an enhanced functionality variant of the
  :mod:`blockDist<BlockDist>` distribution. Both refer to an array partitioned
  into blocks, but ``stencilDist`` will help the user to improve
  performance for stencil computations by reducing the amount of communication
  necessary for array accesses across locales. A ``stencilDist`` array would
  most commonly be used in computations based on a numerical stencil formula.
  For example, solving banded linear equations by the Gauss-Seidel or Jacobi
  method.

  Like ``blockDist``, each block in a ``stencilDist`` array is owned by a
  (potentially) different locale. But when communication needs to occur between
  a given block and its set of immediately neighboring or adjacent blocks, the
  more feature-rich ``stencilDist`` is typically the better choice. This is
  because, for any given block, a ``stencilDist`` transparently creates a
  read-only cache of selected array elements from its adjacent blocks,
  specifically those array elements from the band of array elements just
  outside the edge of the locally-owned block. This band of array elements is
  sometimes referred to as a 'halo' or 'overlap region' in other languages
  and technologies, but in this documentation will be referred to as either
  'ghost cells' or more strictly, as 'fluff'.

  This approach can  avoid many instances of reading a remote array element
  when performing a stencil computation near the boundary of the current
  locale's chunk of array elements. Note that the user must manually refresh
  these caches after writes by calling the ``updateFluff`` method. The writing
  of array elements and partitioning of indices both behave the same as in a
  block-distributed array.

  The ``stencilDist`` initializer is defined as follows:

    .. code-block:: chapel

      proc stencilDist.init(
        boundingBox: domain(?),
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
  is not strided, you can consider indices for dimension ``i`` over a
  domain named ``boundingBox`` to be:

  .. literalinclude:: ../../../../test/distributions/doc-examples/StencilDistExamples.chpl
     :language: chapel
     :start-after: START_EXAMPLE_0
     :end-before: STOP_EXAMPLE_0

  If the domain is strided:

  .. literalinclude:: ../../../../test/distributions/doc-examples/StencilDistExamples.chpl
     :language: chapel
     :start-after: START_EXAMPLE_1
     :end-before: STOP_EXAMPLE_1

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

  Iterating directly over a stencil-distributed domain or array will only yield
  indices and elements within the ``boundingBox``.

  **Convenience Factory Methods**

  It is common for a ``stencilDist``-distributed domain or array to be
  declared using the same indices for both its ``boundingBox`` and its
  index set (as in the example using ``Space`` above).  It is also
  common to not override any of the other defaulted initializer
  arguments.  In such cases, factory procedures can be used for
  convenience and to avoid repetition.

  .. literalinclude:: ../../../../test/distributions/doc-examples/StencilDistExamples.chpl
     :language: chapel
     :start-after: START_EXAMPLE_2
     :end-before: STOP_EXAMPLE_2

  The helper methods on ``stencilDist`` have the following signatures:

  .. function:: proc type stencilDist.createDomain(dom: domain, targetLocales = Locales, fluff, periodic = false)

    Create a stencil-distributed domain. The provided domain is used as the
    ``boundingBox``.

  .. function:: proc type stencilDist.createDomain(rng: range(?)..., targetLocales = Locales, fluff, periodic = false)

    Create a stencil-distributed domain from a series of ranges. The ranges
    are also used to construct the ``boundingBox``.

  .. function:: proc type stencilDist.createArray(dom: domain, type eltType, targetLocales = Locales, fluff, periodic = false)

    Create a default-initialized, stencil-distributed array whose
    indices match those of the given domain.

  .. function:: proc type stencilDist.createArray(rng: range(?)..., type eltType, targetLocales = Locales, fluff, periodic = false)

    Create a default-initialized, stencil-distributed array using a
    domain constructed from the series of ranges.

  .. function:: proc type stencilDist.createArray(dom: domain, type eltType, initExpr, targetLocales = Locales, fluff, periodic = false)

    Create a stencil-distributed array whose indices match those of the
    given domain.

    The array's values are initialized using ``initExpr`` which can be any of
    the following:

    * a value coercible to ``eltType`` — all elements of the array will be
      assigned with this value
    * an iterator expression with compatible size and type — the array elements
      will be initialized with the values yielded by the iterator
    * an array of compatible size and type — the array will be assigned into
      the distributed array

  .. function:: proc type stencilDist.createArray(rng: range(?)..., type eltType, initExpr, targetLocales = Locales, fluff, periodic = false)

    Create a stencil-distributed array using a domain constructed from
    the series of ranges.

    The array's values are initialized using ``initExpr`` which can be any of
    the following:

    * a value coercible to ``eltType`` — all elements of the array will be
      assigned with this value
    * an iterator expression with compatible size and type — the array elements
      will be initialized with the values yielded by the iterator
    * an array of compatible size and type — the array will be assigned into
      the distributed array

  .. function:: proc stencilDist.createDomain(dom: domain(?))

    Create a stencil-distributed domain over an existing ``blockDist`` by copying
    the index space from the passed domain.

  .. function:: proc stencilDist.createDomain(rng: range(?)...)

    Create a stencil-distributed domain from a series of ranges over an existing
    ``blockDist``.


  Note that the ``fluff`` argument in the above methods defaults to a
  tuple of *n* zeros, where *n* is the domain's rank or the number of
  provided ranges.

  **Updating the Cached Elements**

  Once you have completed a series of writes to the array, you will need to
  call the ``updateFluff`` method to update the cached elements for each
  locale. Here is a simple example:

  .. literalinclude:: ../../../../test/distributions/doc-examples/StencilDistExamples.chpl
     :language: chapel
     :start-after: START_EXAMPLE_3
     :end-before: STOP_EXAMPLE_3

  After updating, any read from the array should be up-to-date. The
  ``updateFluff`` method does not currently accept any arguments.

  **Reading and Writing to Array Elements**

  The ``stencilDist`` distribution uses ghost cells as cached
  read-only values from other locales. When reading from a
  stencil-distributed array, the distribution will attempt to read
  from the local ghost cell cache first. If the index is not within the
  cached index set of the current locale, then we default to a remote
  read from the locale on which the element is located.

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
pragma "ignore noinit"
record stencilDist : writeSerializable {
  param rank: int;
  type idxType = int;
  param ignoreFluff = false;

  forwarding const chpl_distHelp: chpl_PrivatizedDistHelper(unmanaged StencilImpl(rank, idxType, ignoreFluff));

  proc init(boundingBox: domain,
            targetLocales: [] locale = Locales,
            dataParTasksPerLocale=getDataParTasksPerLocale(),
            dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
            dataParMinGranularity=getDataParMinGranularity(),
            param rank = boundingBox.rank,
            type idxType = boundingBox.idxType,
            fluff: rank*idxType = makeZero(rank, idxType),
            periodic: bool = false,
            param ignoreFluff = false) {
    const value = new unmanaged StencilImpl(boundingBox, targetLocales,
                                          dataParTasksPerLocale,
                                          dataParIgnoreRunningTasks,
                                          dataParMinGranularity,
                                          rank, idxType, fluff, periodic,
                                          ignoreFluff);
    this.rank = rank;
    this.idxType = idxType;
    this.ignoreFluff = ignoreFluff;
    this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                          if _isPrivatized(value)
                            then _newPrivatizedClass(value)
                            else nullPid,
                          value);
  }

    proc init(_pid : int, _instance, _unowned : bool) {
      this.rank = _instance.rank;
      this.idxType = _instance.idxType;
      this.ignoreFluff = _instance.ignoreFluff;
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(_pid,
                                                         _instance,
                                                         _unowned);
    }

    proc init(value) {
      this.rank = value.rank;
      this.idxType = value.idxType;
      this.ignoreFluff = value.ignoreFluff;
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                             if _isPrivatized(value)
                               then _newPrivatizedClass(value)
                               else nullPid,
                             _to_unmanaged(value));
    }

    // Note: This does not handle the case where the desired type of 'this'
    // does not match the type of 'other'. That case is handled by the compiler
    // via coercions.
    proc init=(const ref other : stencilDist(?)) {
      this.init(other._value.dsiClone());
    }

    proc clone() {
      return new stencilDist(this._value.dsiClone());
    }

  @chpldoc.nodoc
  inline operator ==(d1: stencilDist(?), d2: stencilDist(?)) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  @chpldoc.nodoc
  inline operator !=(d1: stencilDist(?), d2: stencilDist(?)) {
    return !(d1 == d2);
  }

  proc serialize(writer, ref serializer) throws {
    chpl_distHelp.serialize(writer, serializer);
  }
}


@chpldoc.nodoc
@unstable(category="experimental", reason="assignment between distributions is currently unstable due to lack of testing")
operator =(ref a: stencilDist(?), b: stencilDist(?)) {
  if a._value == nil {
    __primitive("move", a, chpl__autoCopy(b.clone(), definedConst=false));
  } else {
    if a._value.type != b._value.type then
      compilerError("type mismatch in distribution assignment");
    if a._value == b._value {
      // do nothing
    } else
        a._value.dsiAssign(b._value);
    if _isPrivatized(a._instance) then
      _reprivatize(a._value);
  }
}


class StencilImpl : BaseDist, writeSerializable {
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
// rank : generic rank that matches StencilImpl.rank
// idxType: generic index type that matches StencilImpl.idxType
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
// strides:   generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class StencilDom: BaseRectangularDom(?) {
  param ignoreFluff : bool;
  const dist: unmanaged StencilImpl(rank, idxType, ignoreFluff);
  var locDoms: [dist.targetLocDom] unmanaged LocStencilDom(rank, idxType,
                                                           strides);
  var whole: domain(rank=rank, idxType=idxType, strides=strides);
  var fluff: rank*idxType;
  var periodic: bool = false;
  var wholeFluff : domain(rank, idxType, strides);
}

//
// Local Stencil Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// strides: generic domain stridable parameter
// myBlock: a non-distributed domain that defines the local indices
//
// NeighDom will be a rectangular domain where each dimension is the range
// -1..1
//
class LocStencilDom {
  param rank: int;
  type idxType;
  param strides: strideKind;
  var myBlock, myFluff: domain(rank, idxType, strides);
  var NeighDom: domain(rank);
  var bufDom : domain(1);
  var recvDest, recvSrc,
      sendDest, sendSrc: [NeighDom] domain(rank, idxType, strides);
  var Neighs: [NeighDom] rank*int;
}

//
// Stencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// strides: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class StencilArr: BaseRectangularArr(?) {
  param ignoreFluff: bool;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: unmanaged StencilDom(rank, idxType, strides, ignoreFluff);
  var locArr: [dom.dist.targetLocDom] unmanaged LocStencilArr(eltType, rank,
                                                             idxType, strides);
  pragma "local field"
  var myLocArr: unmanaged LocStencilArr(eltType, rank, idxType, strides)?;
  const SENTINEL = max(rank*idxType);
}

//
// Local Stencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// strides: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocStencilArr : writeSerializable {
  type eltType;
  param rank: int;
  type idxType;
  param strides: strideKind;
  const locDom: unmanaged LocStencilDom(rank, idxType, strides);
  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, strides)?; // non-nil if doRADOpt=true
  pragma "local field" pragma "unsafe"
  // may be initialized separately
  var myElems: [locDom.myFluff] eltType;
  var locRADLock: chpl_LocalSpinlock;

  // TODO: use void type when packed update is disabled
  var recvBufs, sendBufs : [locDom.NeighDom] [locDom.bufDom] eltType;
  var sendRecvFlag : [locDom.NeighDom] atomic bool;

  proc init(type eltType,
            param rank: int,
            type idxType,
            param strides: strideKind,
            const locDom: unmanaged LocStencilDom(rank, idxType, strides),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.strides = strides;
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

    if locRAD != nil then
      delete locRAD;
  }

  // guard against dynamic dispatch resolution trying to resolve
  // write()ing out an array of sync vars and hitting the sync var
  // type's compilerError()
  override proc serialize(writer, ref serializer) throws {
    halt("LocStencilArr.serialize() is not implemented / should not be needed");
  }
}

private proc makeZero(param rank : int, type idxType) {
  var ret : rank*idxType;
  return ret;
}
//
// Stencil initializer for clients of the Stencil distribution
//
proc StencilImpl.init(boundingBox: domain,
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

  this.boundingBox = boundsBox(boundingBox);
  this.fluff = fluff;

  if !allowDuplicateTargetLocales {
    var checkArr: [LocaleSpace] bool;
    for loc in targetLocales {
      if checkArr[loc.id] then
        halt("StencilDist does not allow duplicate targetLocales");
      checkArr[loc.id] = true;
    }
  }

  // can't have periodic if there's no fluff
  this.periodic = periodic && !isZeroTuple(fluff);

  const ranges = setupTargetLocRanges(rank, targetLocales);
  this.targetLocDom = {(...ranges)};
  this.targetLocales = reshape(targetLocales, this.targetLocDom);

  // Instead of 'dummyLS', we could give 'locDistTemp' a nilable element type.
  const dummyLS = new unmanaged LocStencil(rank, idxType, dummy=true);
  var locDistTemp: [targetLocDom] unmanaged LocStencil(rank, idxType) = dummyLS;

  coforall locid in targetLocDom with (ref locDistTemp) do
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

  init this;

  if debugStencilDist {
    writeln("Creating new Stencil distribution:");
    dsiDisplayRepresentation();
  }
}

proc StencilImpl.dsiAssign(other: this.type) {
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
proc StencilImpl.dsiEqualDMaps(that: StencilImpl(?)) {
  return (this.boundingBox == that.boundingBox &&
          this.targetLocales.equals(that.targetLocales) &&
          this.fluff == that.fluff &&
          this.periodic == that.periodic);
}


proc StencilImpl.dsiEqualDMaps(that) param {
  return false;
}



proc StencilImpl.dsiClone() {
  return new unmanaged StencilImpl(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity, fluff=fluff, periodic=periodic,
                   ignoreFluff=this.ignoreFluff);
}

override proc StencilImpl.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

override proc StencilImpl.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

override proc StencilImpl.dsiNewRectangularDom(param rank: int, type idxType,
                                           param strides: strideKind, inds) {
  if idxType != this.idxType then
    compilerError("Stencil domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Stencil domain rank does not match distribution's");

  const dummyLSD = new unmanaged LocStencilDom(rank, idxType, strides);
  var locDomsTemp: [this.targetLocDom]
                  unmanaged LocStencilDom(rank, idxType, strides) = dummyLSD;
  coforall localeIdx in this.targetLocDom with (ref locDomsTemp) do
    on this.targetLocales(localeIdx) do
      locDomsTemp(localeIdx) =
        new unmanaged LocStencilDom(rank, idxType, strides,
                                    NeighDom=nearestDom(rank));
  delete dummyLSD;

  var dom = new unmanaged StencilDom(rank, idxType, strides, ignoreFluff,
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
override proc StencilImpl.serialize(writer, ref serializer) throws {
  writer.writeln("Stencil");
  writer.writeln("-------");
  writer.writeln("distributes: ", boundingBox);
  writer.writeln("across locales: ", targetLocales);
  writer.writeln("indexed via: ", targetLocDom);
  writer.writeln("resulting in: ");
  for locid in targetLocDom do
    writer.writeln("  [", locid, "] locale ", locDist(locid).locale.id,
      " owns chunk: ", locDist(locid).myChunk);
}

proc StencilImpl.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

proc StencilImpl.dsiIndexToLocale(ind: rank*idxType) where rank > 1 {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc StencilImpl.getChunk(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  // see Block.getChunk
  //
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  if sanityCheckDistribution then
    if chunk.sizeAs(int) > 0 {
      if targetLocsIdx(chunk.lowBound) != locid then
        writeln("[", here.id, "] ", chunk.lowBound, " is in my chunk but maps to ",
                targetLocsIdx(chunk.lowBound));
      if targetLocsIdx(chunk.highBound) != locid then
        writeln("[", here.id, "] ", chunk.highBound, " is in my chunk but maps to ",
                targetLocsIdx(chunk.highBound));
    }
  return chunk;
}

//
// get the index into the targetLocales array for a given distributed index
//
proc StencilImpl.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx((ind,));
}

proc StencilImpl.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 0..rank-1 do
    result(i) = max(0, min((targetLocDom.dim(i).sizeAs(int)-1):int,
                           (((ind(i) - boundingBox.dim(i).lowBound) *
                             targetLocDom.dim(i).sizeAs(int):idxType) /
                            boundingBox.dim(i).sizeAs(int)):int));
  return if rank == 1 then result(0) else result;
}

// TODO: This will not trigger the bounded-coforall optimization
iter StencilImpl.activeTargetLocales(const space : domain = boundingBox) {
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
  foreach i in {(...dims)} {
    const chunk = chpl__computeBlock(i, targetLocDom, boundingBox);
    // TODO: Want 'overlaps' for a domain. Slicing is a workaround.
    if locSpace[(...chunk)].sizeAs(int) > 0 then
      yield i;
  }
}

iter StencilImpl.activeTargetLocales(const space : range(?)) {
  compilerAssert(rank==1);
  const dims = targetLocsIdx(space.first)..targetLocsIdx(space.last);

  // In case 'space' is a strided domain we need to check that the locales
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
  foreach i in dims {
    const chunk = chpl__computeBlock(i, targetLocDom, boundingBox);
    // TODO: Want 'overlaps' for a domain. Slicing is a workaround.
    if space[chunk[0]].sizeAs(int) > 0 then
      yield i;
  }

}

// create a domain over an existing Stencil Distribution
proc stencilDist.createDomain(dom: domain(?)) {
  return dom dmapped this;
}

// create a domain over an existing Stencil Distribution constructed from a series of ranges
proc stencilDist.createDomain(rng: range(?)...) {
  return this.createDomain({(...rng)});
}

// create a domain over a Stencil Distribution
proc type stencilDist.createDomain(
  dom: domain(?),
  targetLocales: [] locale = Locales,
  fluff = makeZero(dom.rank, dom.idxType),
  periodic = false
) {
  return dom dmapped new stencilDist(dom, targetLocales, fluff=fluff, periodic=periodic);
}

// create a domain over a Stencil Distribution constructed from a series of ranges
pragma "last resort"
proc type stencilDist.createDomain(
  rng: range(?)...,
  targetLocales: [] locale = Locales,
  fluff: ?t = makeZero(rng.size, int),
  periodic = false
) where isHomogeneousTupleType(t)
{
  return createDomain({(...rng)}, targetLocales, fluff, periodic);
}

proc type stencilDist.createDomain(rng: range(?)...) {
  return createDomain({(...rng)}, fluff = makeZero(rng.size, rng[0].idxType));
}

// create an array over a Stencil Distribution, default initialized
pragma "no copy return"
proc type stencilDist.createArray(
  dom: domain(?),
  type eltType,
  targetLocales: [] locale = Locales,
  fluff = makeZero(dom.rank, dom.idxType),
  periodic = false
) {
  var D = createDomain(dom, targetLocales, fluff, periodic);
  var A: [D] eltType;
  return A;
}

// create an array over a Stencil Distribution, initialized with the given value or iterator
pragma "no copy return"
proc type stencilDist.createArray(
  dom: domain(?),
  type eltType,
  initExpr: ?t,
  targetLocales: [] locale = Locales,
  fluff = makeZero(dom.rank, dom.idxType),
  periodic = false
) where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)
{
  var D = createDomain(dom, targetLocales, fluff, periodic);
  var A: [D] eltType;
  A = initExpr;
  return A;
}

// create an array over a Stencil Distribution, initialized from the given array
pragma "no copy return"
proc type stencilDist.createArray(
  dom: domain(?),
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType,
  targetLocales: [] locale = Locales,
  fluff = makeZero(dom.rank, dom.idxType),
  periodic = false
) where dom.rank == arrayDom.rank && isCoercible(arrayEltType, eltType)
{
  if boundsChecking then
    for (d, ad, i) in zip(dom.dims(), arrayDom.dims(), 0..) do
      if d.size != ad.size then halt("Domain size mismatch in 'stencilDist.createArray' dimension " + i:string);
  var D = createDomain(dom, targetLocales, fluff, periodic);
  var A: [D] eltType;
  A = initExpr;
  return A;
}

// create an array over a Stencil Distribution constructed from a series of ranges, default initialized
pragma "no copy return"
pragma "last resort"
proc type stencilDist.createArray(
  rng: range(?)...,
  type eltType,
  targetLocales: [] locale = Locales,
  fluff: ?f = makeZero(rng.size, int),
  periodic = false
) where isHomogeneousTupleType(f) {
  return createArray({(...rng)}, eltType, targetLocales, fluff, periodic);
}


pragma "no copy return"
proc type stencilDist.createArray(rng: range(?)..., type eltType) {
  return createArray({(...rng)}, eltType, fluff = makeZero(rng.size, rng[0].idxType));
}

// create an array over a Stencil Distribution constructed from a series of ranges, initialized with the given value or iterator
pragma "no copy return"
proc type stencilDist.createArray(rng: range(?)..., type eltType, initExpr: ?t)
  where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)
{
  return createArray({(...rng)}, eltType, initExpr, fluff = makeZero(rng.size, rng[0].idxType));
}

pragma "no copy return"
pragma "last resort"
proc type stencilDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: ?t,
  targetLocales: [] locale = Locales,
  fluff: ?f = makeZero(rng.size, int),
  periodic = false
) where (isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)) && isHomogeneousTupleType(f)  {
  return createArray({(...rng)}, eltType, initExpr, targetLocales, fluff, periodic);
}

// create an array over a Cyclic Distribution constructed from a series of ranges, initialized from the given array
pragma "no copy return"
proc type stencilDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType
) where rng.size == arrayDom.rank && isCoercible(arrayEltType, eltType)
{
  return createArray({(...rng)}, eltType, initExpr);
}

pragma "no copy return"
pragma "last resort"
proc type stencilDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType,
  targetLocales: [] locale = Locales,
  fluff: ?f = makeZero(rng.size, int),
  periodic = false
) where rng.size == arrayDom.rank && isCoercible(arrayEltType, eltType) && isHomogeneousTupleType(f)
{
  return createArray({(...rng)}, eltType, initExpr, targetLocales, fluff, periodic);
}

proc chpl__computeBlock(locid, targetLocBox, boundingBox) {
  param rank = targetLocBox.rank;
  type idxType = chpl__tuplify(boundingBox)(0).idxType;
  var inds: rank*range(idxType);
  for param i in 0..rank-1 {
    const lo = boundingBox.dim(i).lowBound;
    const hi = boundingBox.dim(i).highBound;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox.dim(i).sizeAs(int);
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

proc StencilDom.dsiGetDist() {
  if _isPrivatized(dist) then
    return new stencilDist(dist.pid, dist, _unowned=true);
  else
    return new stencilDist(nullPid, dist, _unowned=true);
}

override proc StencilDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
}

// stopgap to avoid accessing locDoms field (and returning an array)
proc StencilDom.getLocDom(localeIdx) do return locDoms(localeIdx);


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
  const wholeLow = whole.lowBound;

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
    const tmpBlock = locDom.myBlock.chpl__unTranslate(wholeLow);
    var locOffset: rank*idxType;
    for param i in 0..tmpBlock.rank-1 {
      const dim = tmpBlock.dim(i);
      const aStr = if dim.hasPositiveStride() then dim.stride else -dim.stride;
      locOffset(i) = dim.low / aStr:idxType;
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
iter StencilDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  if chpl__testParFlag then
    chpl__testPar("Stencil domain follower invoked on ", followThis);

  var t: rank*range(idxType, strides=chpl_strideProduct(strides,
                                      chpl_strideUnion(followThis)));
  for param i in 0..rank-1 {
    const wholeDim  = whole.dim(i);
    const followDim = followThis(i);
    var low  = wholeDim.orderToIndex(followDim.low);
    var high = wholeDim.orderToIndex(followDim.high);
    if wholeDim.hasNegativeStride() then low <=> high;
    t(i) = (low..high by (wholeDim.stride*followDim.stride)) : t(i).type;
  }
  for i in {(...t)} {
    yield i;
  }
}

//
// how to allocate a new array over this domain
//
proc StencilDom.dsiBuildArray(type eltType, param initElts:bool) {
  if isOwnedClassType(eltType) {
    if isNonNilableClassType(eltType) {
      //
      // 'owned C' simply doesn't work today, so this creates a clean error
      //
      compilerError("'stencilDist'-distributed arrays of non-nilable 'owned' classes are not currently supported");
    } else {
      //
      // 'owned C?' works, but is probably surprising, so warn about it
      //
      compilerWarning("Creating a 'stencilDist'-distributed array of 'owned' classes is discouraged, since any calls to 'updateFluff()' will transfer the ownership of boundary values from one locale to the other, leaving the original with the value 'nil'");
    }
  }
  const dom = this;
  const creationLocale = here.id;
  const dummyLSD = new unmanaged LocStencilDom(rank, idxType, strides);
  const dummyLSA = new unmanaged LocStencilArr(eltType, rank, idxType,
                                               strides, dummyLSD, false);
  var locArrTemp: [dom.dist.targetLocDom]
        unmanaged LocStencilArr(eltType, rank, idxType, strides) = dummyLSA;
  var myLocArrTemp: unmanaged LocStencilArr(eltType, rank, idxType, strides)?;

  // formerly in StencilArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref locArrTemp, ref myLocArrTemp) {
    on dom.dist.targetLocales(localeIdx) {
      const LSA = new unmanaged LocStencilArr(eltType, rank, idxType, strides,
                                              dom.getLocDom(localeIdx),
                                              initElts=initElts);
      locArrTemp(localeIdx) = LSA;
      if here.id == creationLocale then
        myLocArrTemp = LSA;
    }
  }
  delete dummyLSA, dummyLSD;

  var arr = new unmanaged StencilArr(eltType=eltType, rank=rank,
        idxType=idxType, strides=strides, ignoreFluff=this.ignoreFluff,
        dom=_to_unmanaged(this), locArr=locArrTemp, myLocArr=myLocArrTemp);

  // formerly in StencilArr.setup()
  if arr.doRADOpt && disableStencilLazyRAD then arr.setupRADOpt();

  return arr;
}

// common redirects
override proc StencilDom.dsiLow do           return whole.lowBound;
override proc StencilDom.dsiHigh do          return whole.highBound;
override proc StencilDom.dsiAlignedLow do    return whole.low;
override proc StencilDom.dsiAlignedHigh do   return whole.high;
override proc StencilDom.dsiFirst do         return whole.first;
override proc StencilDom.dsiLast do          return whole.last;
override proc StencilDom.dsiStride do        return whole.stride;
override proc StencilDom.dsiAlignment do     return whole.alignment;
proc StencilDom.dsiNumIndices do    return whole.sizeAs(uint);
proc StencilDom.dsiDim(d) do        return whole.dim(d);
proc StencilDom.dsiDim(param d) do  return whole.dim(d);
proc StencilDom.dsiDims() do        return whole.dims();
proc StencilDom.dsiGetIndices() do  return whole.getIndices();
//proc StencilDom.dsiMember(i) do     return whole.contains(i);
proc StencilDom.doiToString() do    return whole:string;
proc StencilDom.dsiSerialWrite(x) { x.write(whole); }
proc StencilDom.dsiLocalSlice(param strides, ranges) do return whole((...ranges));
override proc StencilDom.dsiIndexOrder(i) do              return whole.indexOrder(i);
override proc StencilDom.dsiMyDist() do                   return dist;

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
  if whole.sizeAs(int) > 0 {
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
  if whole.sizeAs(int) > 0 {
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

      if !isZeroTuple(fluff) && myLocDom.myBlock.sizeAs(int) != 0 {
        myLocDom.myFluff = myLocDom.myBlock.expand(fluff*abstr);
      } else {
        myLocDom.myFluff = myLocDom.myBlock;
      }

      if !isZeroTuple(fluff) && myLocDom.myBlock.sizeAs(int) > 0 {
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
                dr(i).chpl_setFields(cur.first - fa, cur.first-abstr(i),
                                     cur.stride);
              else if to(i) > 0 then
                dr(i).chpl_setFields(cur.last+abstr(i), cur.last+fa,
                                     cur.stride);
              else
                dr(i).chpl_setFields(cur.first, cur.last,
                                     cur.stride);
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
                const mult = if rd.lowBound > wd.highBound then -1
                             else if rd.highBound < wd.lowBound then 1
                             else 0;
                // todo: what to do when idxType is unsigned and mult==-1 ?
                offset(i) = (mult * wd.sizeAs(int) * abstr(i)).safeCast(idxType);
              }
              recvS = recvD.translate(offset);
              N = chpl__tuplify(dist.targetLocsIdx(recvS.first));

              assert(whole.contains(recvS.first) && whole.contains(recvS.last), "StencilDist: Failure to compute Src slice.");
              assert(dist.targetLocDom.contains(N), "StencilDist: Error computing neighbor index.");
            } else {
              // No communication needed in this direction
              assert(recvS.sizeAs(int) == 0);
              N = chpl__tuplify(localeIdx);

              // zero out recvD so we can test its size later to determine if
              // communication needs to happen
              recvD = recvS;
            }

            if recvS.sizeAs(int) != 0 {
              const offset = -1 * L * fluff * abstr;
              sendS = recvD.translate(offset);
              sendD = recvS.translate(offset);
            }

            bufLen = max(bufLen, recvD.sizeAs(int), recvS.sizeAs(int));

            if recvS.sizeAs(int) != 0 && debugStencilDist {
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
          if !isZeroTuple(L) && recvS.sizeAs(int) != 0 {
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

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocStencilDom.contains(i) do return myBlock.contains(i);

override proc StencilArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
  }
}

override proc StencilArr.dsiGetBaseDom() do return dom;

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
        myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType,
                                             strides, dom.dist.targetLocDom);
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

override proc StencilArr.dsiElementDeinitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      locArr(localeIdx).myElems.dsiElementDeinitializationComplete();
    }
  }
}

override proc StencilArr.dsiDestroyArr(deinitElts:bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      var arr = locArr(localeIdx);
      if !ignoreFluff {
        if deinitElts {
          // only deinitialize non-fluff elements
          // fluff is always deinited in the LocArr deinit
          param needsDestroy = __primitive("needs auto destroy", eltType);
          if needsDestroy {
            if _deinitElementsIsParallel(eltType, arr.locDom.myBlock.size) {
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
        arr.myElems.dsiElementDeinitializationComplete();
        delete arr;
      }
    }
  }
}

// Re-use _remoteAccessData.getDataIndex from BlockDist


inline proc StencilArr.dsiLocalAccess(i: rank*idxType) ref {
  return if allowDuplicateTargetLocales then this.dsiAccess(i)
                                        else _to_nonnil(myLocArr)(i);
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
        if myLocArr.locDom.contains(idx) then return myLocArr(idx);
      } else {
        // A read: return from fluff if possible
        // If there is no fluff, then myFluff == myBlock
        if myLocArr.locDom.myFluff.contains(idx) then return myLocArr(idx);
      }
    }
  }

  return nonLocalAccess(idx);
}

pragma "not called from gpu"
proc StencilArr.nonLocalAccess(i: rank*idxType) ref {

  if doRADOpt {
    if myLocArr {
      const myLocArr = _to_nonnil(this.myLocArr);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableStencilLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.locRADLock.lock();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new unmanaged LocRADCache(eltType, rank, idxType,
                                              strides, dom.dist.targetLocDom);
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
inline proc StencilArr.dsiAccess(i: rank*idxType) const ref {
  return do_dsiAccess(false, i);
}


// ref version
inline proc StencilArr.dsiAccess(i: idxType...rank) ref do
  return dsiAccess(i);
// value version for POD types
inline proc StencilArr.dsiAccess(i: idxType...rank)
where shouldReturnRvalueByValue(eltType) do
  return dsiAccess(i);
// const ref version for types with copy-ctor
inline proc StencilArr.dsiAccess(i: idxType...rank) const ref do
  return dsiAccess(i);

inline proc StencilArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.wholeFluff.contains(i);
}

iter StencilArr.these() ref {
  foreach i in dom do
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

proc StencilArr.dsiDynamicFastFollowCheck(lead: []) do
  return this.dsiDynamicFastFollowCheck(lead.domain);

proc StencilArr.dsiDynamicFastFollowCheck(lead: domain) {
  return lead.distribution.dsiEqualDMaps(this.dom.dist) && lead._value.whole == this.dom.whole;
}

iter StencilArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  if chpl__testParFlag {
    if fast then
      chpl__testPar("Stencil array fast follower invoked on ", followThis);
    else
      chpl__testPar("Stencil array non-fast follower invoked on ", followThis);
  }

  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Stencil array");

  var myFollowThis: rank*range(idxType=idxType, strides=chpl_strideProduct(
                                     strides, chpl_strideUnion(followThis)));
  var lowIdx: rank*idxType;

  for param i in 0..rank-1 {
    const stride = dom.whole.dim(i).stride;
    // NOTE: Not bothering to check to see if these can fit into idxType
    var low = followThis(i).lowBound * abs(stride):idxType;
    var high = followThis(i).highBound * abs(stride):idxType;
    myFollowThis(i) = ((low..high by stride) + dom.whole.dim(i).low
                       by followThis(i).stride) : myFollowThis(i).type;
    lowIdx(i) = myFollowThis(i).lowBound;
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
      use CTypes; // Needed to cast from c_ptr(void) in the next line
      const narrowArrSection =
        __primitive("_wide_get_addr", arrSection):arrSection.type?;
      ref myElems = _to_nonnil(narrowArrSection).myElems;
      foreach i in myFollowThisDom do yield myElems[i];
    }
  } else {
    //
    // we don't necessarily own all the elements we're following
    //
    foreach i in myFollowThisDom {
      yield dsiAccess(i);
    }
  }
}

//
// output array
//
proc StencilArr.dsiSerialWrite(f) {
  type strType = chpl__signedType(idxType);
  var binary = f._binary();
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 0..rank-1 do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(do_dsiAccess(true, i));
    if i(rank-1) <= (dom.dsiDim(rank-1).high - dom.dsiDim(rank-1).stride:strType) {
      if ! binary then f.write(" ");
      i(rank-1) += dom.dsiDim(rank-1).stride:strType;
    } else {
      for dim in 0..rank-2 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:strType) {
          i(dim) += dom.dsiDim(dim).stride:strType;
          for dim2 in dim+1..rank-1 {
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
  for param i in 0..rank-1 {
    low(i) = ranges(i).lowBound;
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

pragma "do not unref for yields"
iter _array.boundaries() {
  for d in _value.dsiBoundaries() do yield d;
}

pragma "do not unref for yields"
iter _array.boundaries(param tag : iterKind) where tag == iterKind.standalone {
  forall d in _value.dsiBoundaries() do yield d;
}

pragma "do not unref for yields"
iter StencilArr.dsiBoundaries() {
  for i in dom.dist.targetLocDom {
    var LSA = locArr[i];
    ref myLocDom = LSA.locDom;
    for (D, N, L) in zip(myLocDom.recvDest, myLocDom.Neighs, myLocDom.NeighDom) {
      const target = chpl__tuplify(i + L);
      const low    = chpl__tuplify(dom.dist.targetLocDom.lowBound);
      const high   = chpl__tuplify(dom.dist.targetLocDom.highBound);

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

        foreach el in LSA.myElems[D] do yield (el, translated);
      }
    }
  }
}

//
// Yields any 'fluff' boundary chunks in the StencilArr along with a global coordinate of
// where the chunk lives relative to the core.
//
pragma "do not unref for yields"
iter StencilArr.dsiBoundaries(param tag : iterKind) where tag == iterKind.standalone {
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      var LSA = locArr[i];
      ref myLocDom = LSA.locDom;
      forall (D, N, L) in zip(myLocDom.recvDest, myLocDom.Neighs, myLocDom.NeighDom) {
        const target = chpl__tuplify(i + L);
        const low    = chpl__tuplify(dom.dist.targetLocDom.lowBound);
        const high   = chpl__tuplify(dom.dist.targetLocDom.highBound);
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
          foreach el in LSA.myElems[D] do yield (el, translated);
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
  var tempDist = new unmanaged StencilImpl(dom.dist.boundingBox, dom.dist.targetLocales,
                             dom.dist.dataParTasksPerLocale, dom.dist.dataParIgnoreRunningTasks,
                             dom.dist.dataParMinGranularity, ignoreFluff=true);
  pragma "no auto destroy" var newDist = new _distribution(tempDist);
  pragma "no auto destroy" var tempDom = new _domain(newDist, rank, idxType,
                                                dom.strides, dom.whole.dims());
  newDist._value._free_when_no_doms = true;

  var newDom = tempDom._value;
  newDom._free_when_no_arrs = true;

  var alias = new unmanaged StencilArr(eltType=eltType, rank=rank,
                         idxType=idxType, strides=newDom.strides, dom=newDom,
                         ignoreFluff=true, locArr=locArr, myLocArr=myLocArr);

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
        if !isZeroTuple(L) && S.sizeAs(int) != 0 {
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
  const mypid = this.pid;
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocales(i) {
      const privArr = if _privatization then chpl_getPrivatizedCopy(this.type, mypid) else this;
      var myLocDom = privArr.locArr[i].locDom;

      proc translateIdx(idx) {
        return -1 * chpl__tuplify(idx);
      }

      forall (D, S, recvIdx, sendBufIdx) in zip(myLocDom.sendDest, myLocDom.sendSrc,
                                                myLocDom.Neighs,
                                                myLocDom.NeighDom) {
        // If S.size == 0, no communication is required
        if S.sizeAs(int) != 0 {
          const chunkSize  = max(1, S.dim(rank-1).sizeAs(int)); // avoid divide by zero
          const numChunks = S.sizeAs(int) / chunkSize;
          if numChunks >= stencilDistPackedUpdateMinChunks {
            const recvBufIdx = translateIdx(sendBufIdx);

            // Pack the buffer
            ref src = privArr.locArr[i].myElems[S];
            ref buf = privArr.locArr[i].sendBufs[sendBufIdx];

            // TODO: parallelize for larger buffers? Other forms for this loop?
            local do for (s, i) in zip(src, buf.domain.first..#src.sizeAs(int)) do buf[i] = s;

            if debugStencilDist then
              writeln("Filled ", here, ".", S, " for ", dom.dist.targetLocales(recvIdx), "::", recvBufIdx);
            privArr.locArr[recvIdx].sendRecvFlag[recvBufIdx].write(true);
          } else {
            // 'naive' update
            privArr.locArr[recvIdx].myElems[D] = privArr.locArr[i].myElems[S];
          }
        }
      }
      forall (D, S, srcIdx, recvBufIdx) in zip(myLocDom.recvDest, myLocDom.recvSrc,
                                               myLocDom.Neighs,
                                               myLocDom.NeighDom) {
        const chunkSize  = max(1, S.dim(rank-1).sizeAs(int)); // avoid divide by zero
        const numChunks = S.sizeAs(int) / chunkSize;

        // If we did a naive update in the previous loop, this iteration does
        // not need to do anything.
        if S.sizeAs(int) != 0 && numChunks >= stencilDistPackedUpdateMinChunks {
          const srcBufIdx = translateIdx(recvBufIdx);
          if debugStencilDist then
            writeln(here, "::", recvBufIdx, " WAITING");

          privArr.locArr[i].sendRecvFlag[recvBufIdx].waitFor(true); // Can we read yet?
          privArr.locArr[i].sendRecvFlag[recvBufIdx].write(false);  // reset for next call

          privArr.locArr[i].recvBufs[recvBufIdx][1..D.sizeAs(int)] = privArr.locArr[srcIdx].sendBufs[srcBufIdx][1..D.sizeAs(int)];

          // unpack buffer
          ref dest = privArr.locArr[i].myElems[D];
          ref buf = privArr.locArr[i].recvBufs[recvBufIdx];

          local do for (d, i) in zip(dest, buf.domain.first..#dest.sizeAs(int)) do d = buf[i];
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

override proc StencilArr.dsiReallocate(bounds:rank*range(idxType,
                                                     boundKind.both, strides))
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

override proc StencilDom.dsiSupportsAutoLocalAccess() param { return true; }
override proc StencilDom.dsiSupportsOffsetAutoLocalAccess() param {
  return true;
}
// offsets is always a tuple
override proc StencilDom.dsiAutoLocalAccessOffsetCheck(offsets) {
  var ret = true;
  for param i in 0..<rank {
    ret &&= fluff[i] >= abs(offsets[i]);
  }
  return ret;
}

override proc StencilDom.dsiSupportsArrayViewElision() param {
  return !disableStencilDistArrayViewElision;
}

//
// Privatization
//
proc StencilImpl.init(other: StencilImpl, privateData,
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

override proc StencilImpl.dsiSupportsPrivatization() param do return true;

proc StencilImpl.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity,
          fluff, periodic);
}

proc StencilImpl.dsiPrivatize(privatizeData) {
  return new unmanaged StencilImpl(_to_unmanaged(this), privatizeData);
}

proc StencilImpl.dsiGetReprivatizeData() do return boundingBox.dims();

proc StencilImpl.dsiReprivatize(other, reprivatizeData) {
  boundingBox = {(...reprivatizeData)};
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

override proc StencilDom.dsiSupportsPrivatization() param do return true;

proc StencilDom.dsiGetPrivatizeData() do return (dist.pid, whole.dims());

proc StencilDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(0));

  var locDomsTemp: [privdist.targetLocDom]
                      unmanaged LocStencilDom(rank, idxType, strides)
    = locDoms;

  var c = new unmanaged StencilDom(rank, idxType, strides, ignoreFluff,
            privdist, locDomsTemp, {(...privatizeData(1))}, fluff, periodic);

  if c.whole.sizeAs(int) > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * c.whole.dim(i).stride);
    }
    c.wholeFluff = c.whole.expand(absFluff);
  }

  return c;
}

proc StencilDom.dsiGetReprivatizeData() do return whole.dims();

proc StencilDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = {(...reprivatizeData)};
  if whole.sizeAs(int) > 0 {
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
                                strides=this.strides,
                                ignoreFluff=this.ignoreFluff),
           data);
}

proc StencilArr.chpl__serialize()
    where !(isDomainType(eltType) || isArrayType(eltType)) {
  return pid;
}

proc type StencilArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(
           unmanaged StencilArr(rank=this.rank,
                                idxType=this.idxType,
                                strides=this.strides,
                                eltType=this.eltType,
                                ignoreFluff=this.ignoreFluff),
           data);
}

override proc StencilArr.dsiSupportsPrivatization() param do return true;

proc StencilArr.dsiGetPrivatizeData() do return dom.pid;

proc StencilArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);

  var locArrTemp: [privdom.dist.targetLocDom]
        unmanaged LocStencilArr(eltType, rank, idxType, strides)
    = locArr;

  var myLocArrTemp: unmanaged LocStencilArr(eltType, rank, idxType, strides)?;
  for localeIdx in privdom.dist.targetLocDom do
    if locArrTemp(localeIdx).locale.id == here.id then
      myLocArrTemp = locArrTemp(localeIdx);

  var c = new unmanaged StencilArr(eltType=eltType, rank=rank, idxType=idxType,
                       strides=strides, ignoreFluff=ignoreFluff,
                       dom=privdom, locArr=locArrTemp, myLocArr=myLocArrTemp);
  return c;
}


proc StencilArr.dsiTargetLocales() const ref {
  return dom.dist.targetLocales;
}

proc StencilDom.dsiTargetLocales() const ref {
  return dist.targetLocales;
}

proc StencilImpl.dsiTargetLocales() const ref {
  return targetLocales;
}

proc StencilImpl.chpl__locToLocIdx(loc: locale) {
  for locIdx in targetLocDom do
    if (targetLocales[locIdx] == loc) then
      return (true, locIdx);
  return (false, targetLocDom.first);
}

// Stencil subdomains are continuous

proc StencilArr.dsiHasSingleLocalSubdomain() param do return !allowDuplicateTargetLocales;
proc StencilDom.dsiHasSingleLocalSubdomain() param do return !allowDuplicateTargetLocales;

// returns the current locale's subdomain

proc StencilArr.dsiLocalSubdomain(loc: locale) {
  if (loc == here) {
    // quick solution if we have a local array
    if myLocArr != nil then
      return _to_nonnil(myLocArr).locDom.myBlock;
    // if not, we must not own anything
    var d: domain(rank, idxType, strides);
    return d;
  } else {
    return dom.dsiLocalSubdomain(loc);
  }
}
proc StencilDom.dsiLocalSubdomain(loc: locale) {
  const (gotit, locid) = dist.chpl__locToLocIdx(loc);
  if (gotit) {
    if loc == here {
      return locDoms[locid].myBlock;
    } else {
      var inds = chpl__computeBlock(locid, dist.targetLocDom, dist.boundingBox);
      return whole[(...inds)];
    }
  } else {
    var d: domain(rank, idxType, strides);
    return d;
  }
}

proc StencilDom.numRemoteElems(viewDom, rlo, rid) {
  // NOTE: Not bothering to check to see if rid+1, length, or rlo-1 used
  //  below can fit into idxType
  var blo, bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank-1).sizeAs(int) - 1) then
    bhi=viewDom.dim(rank-1).highBound;
  else {
      bhi = dist.boundingBox.dim(rank-1).lowBound +
        intCeilXDivByY((dist.boundingBox.dim(rank-1).highBound - dist.boundingBox.dim(rank-1).lowBound +1)*(rid+1):idxType,
                       dist.targetLocDom.dim(rank-1).sizeAs(int):idxType) - 1:idxType;

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
  return _doSimpleStencilTransfer(destClass, destDom, this, srcDom);
}

proc StencilArr.doiBulkTransferFromKnown(destDom, srcClass:StencilArr, srcDom) : bool
where !disableStencilDistBulkTransfer {
  return _doSimpleStencilTransfer(this, destDom, srcClass, srcDom);
}

private proc _doSimpleStencilTransfer(Dest, destDom, Src, srcDom) {
  if !chpl_allStridesArePositive(Dest, destDom, Src, srcDom) then return false;

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
      assert(localDestBlock.sizeAs(int) > 0);
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
  return true;
}

// Overload for any transfer *to* Stencil, if the RHS supports transfers to a
// DefaultRectangular
proc StencilArr.doiBulkTransferFromAny(destDom, Src, srcDom) : bool
where canDoAnyToStencil(this, destDom, Src, srcDom) {
  if !chpl_allStridesArePositive(this, destDom, Src, srcDom) then return false;

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
  if !chpl_allStridesArePositive(this, srcDom, Dest, destDom) then return false;


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
  if !chpl_allStridesArePositive(this, destDom, Src, srcDom) then return false;

  if debugStencilDistBulkTransfer then
    writeln("In StencilArr.doiBulkTransferFromKnown(DefaultRectangular)");

  coforall j in dom.dist.activeTargetLocales(destDom) {
    on dom.dist.targetLocales(j) {
      // Grab privatized copy of 'this' to avoid extra GETs
      const Dest = if _privatization then chpl_getPrivatizedCopy(this.type, pid) else this;
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      assert(inters.sizeAs(int) > 0);

      const srcChunk = bulkCommTranslateDomain(inters, destDom, srcDom);

      if debugStencilDistBulkTransfer then
        writeln("  A[",inters,"] = B[",srcChunk,"]");

      const elemActual = Dest.locArr[j].myElems._value;
      chpl__bulkTransferArray(elemActual, inters, Src, srcChunk);
    }
  }

  return true;
}

override proc StencilArr.doiCanBulkTransferRankChange() param do return true;


proc StencilArr.canDoOptimizedSwap(other) {
  var domsMatch = true;

  if this.dom != other.dom { // no need to check if this is true
    for param i in 0..this.dom.rank-1 {
      if this.dom.whole.dim(i) != other.dom.whole.dim(i) {
        domsMatch = false;
      }
    }
  }

  if domsMatch {
    // distributions must be equal, too
    return this.dom.dist.dsiEqualDMaps(other.dom.dist);
  }
  return false;
}

// A helper routine that will perform a pointer swap on an array
// instead of doing a deep copy of that array. Returns true
// if used the optimized swap, false otherwise
//
// TODO: stridability causes issues with RAD swap, and somehow isn't captured by
// the formal type when we check whether this resolves.
proc StencilArr.doiOptimizedSwap(other: this.type)
  where this.strides == other.strides {

  if(canDoOptimizedSwap(other)) {
    if debugOptimizedSwap {
      writeln("StencilArr doing optimized swap. Domains: ",
              this.dom.whole, " ", other.dom.whole, " Bounding boxes: ",
              this.dom.dist.boundingBox, " ", other.dom.dist.boundingBox);
    }
    coforall (locarr1, locarr2) in zip(this.locArr, other.locArr) {
      on locarr1 {
        locarr1.myElems <=> locarr2.myElems;
        locarr1.locRAD <=> locarr2.locRAD;
      }
    }
    return true;
  } else {
    if debugOptimizedSwap {
      writeln("StencilArr doing unoptimized swap. Domains: ",
              this.dom.whole, " ", other.dom.whole, " Bounding boxes: ",
              this.dom.dist.boundingBox, " ", other.dom.dist.boundingBox);
    }
    return false;
  }
}


// The purpose of this overload is to provide debugging output in the event that
// debugOptimizedSwap is on and the main routine doesn't resolve (e.g., due to a
// type, stridability, or rank mismatch in the other argument). When
// debugOptimizedSwap is off, this overload will be ignored due to its where
// clause.
pragma "last resort"
proc StencilArr.doiOptimizedSwap(other) where debugOptimizedSwap {
  writeln("StencilArr doing unoptimized swap. Type mismatch");
  return false;
}

} // StencilDist
