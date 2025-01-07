/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
// A 2D dimensional distribution.
//

// TODO
//
// * Ensure that reallocation works with block-cyclic 1-d distribution
//  when the domain's stride changes.

@unstable("DimensionalDist2D is unstable and may change in the future")
prototype module DimensionalDist2D {

use DSIUtil;
//use WrapperDist;

// debugging/trace certain DSI methods as they are being invoked
config param traceDimensionalDist = false;
config param traceDimensionalDistDsiAccess = false;
config param traceDimensionalDistIterators = false;
config param fakeDimensionalDistParDim = -1;

// so user-specified phases can be retained while sorting verbose output
var traceDimensionalDistPrefix = "";

// private helpers ("trace DimensionalDist" Conditionally)
inline proc _traceddc(param condition: bool, args...)
{
  if condition then writeln(traceDimensionalDistPrefix,(...args));
}
inline proc _traceddc(param cond, d:DimensionalDist2DImpl, args...)
{ _traceddc(cond, "DimensionalDist2DImpl(", d.name, ")", (...args)); }
inline proc _traceddc(param cond, d:DimensionalDom, args...)
{ _traceddc(cond, "DimensionalDom(", d.dist.name, ")", (...args)); }
inline proc _traceddc(param cond, d:DimensionalArr, args...)
{ _traceddc(cond, "DimensionalArr(", d.dom.dist.name, ")", (...args)); }
// the Default condition
inline proc _traceddd(args...)
{ _traceddc(traceDimensionalDist, (...args)); }


/// the type for ... ////////////////////////////////////////////////////////

// ... locale counts
type locCntT = uint(32);

// ... locale ID, i.e., its index in targetLocales in the given dimension
// convention: a locale ID is between 0 and (num. locales 1d - 1)
type locIdT = int;

param invalidLocID =
  // encode 'max(locIdT)' as a compile-time expression
  2 ** (numBits(locIdT) - 1 - isIntType(locIdT):int);


/// class declarations //////////////////////////////////////////////////////
// chpldoc TODO
//  link to dimension specifiers - ideally an inlined list of those
//    otherwise manual list
//
/*
This Dimensional distribution allows the mapping from indices
to locales to be specified as a composition of independent mappings
along each dimension. For example, when implementing the HPL
benchmark, the user may wish to use 2D arrays that are Block-distributed
along one dimension and replicated along the other dimension.
Currently only 2D domains and arrays are supported.

Formally, consider a Dimensional distribution with:

  ====================  ====================================================
  rank                  ``d``
  dimension specifiers  ``dim_1``, ...., ``dim_d``
  over locales          ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
  ====================  ====================================================

It maps an index ``(i_1, ...., i_d)``
to the locale ``targetLocales[j_1, ...., j_d]``
where, for each ``k`` in ``1..d``,
the mapping is obtained using the dimension specifier:

  ``j_k = dim_k(i_k, N_k)``

**Example**

The following code declares a domain ``D`` distributed
using a 2D Dimensional distribution that
replicates over 2 locales (when available) in the first dimension
and distributes using block-cyclic distribution in the second dimension.

  .. code-block:: chapel

    use DimensionalDist2D, ReplicatedDim, BlockCycDim;

    const Space = {1..3, 1..8};

    // Compute N_1 and N_2 and reshapes Locales correspondingly.

    var (N_1, N_2) =
      if numLocales == 1
        then (1, 1)
        else (2, numLocales/2);

    var MyLocaleView = {0..N_1-1, 0..N_2-1};
    var MyLocales = reshape(Locales[0..N_1*N_2-1], MyLocaleView);

    const D = Space
      dmapped new DimensionalDist2D(MyLocales,
                                new ReplicatedDim(numLocales = N_1),
                                new BlockCyclicDim(numLocales = N_2,
                                                   lowIdx     = 1,
                                                   blockSize  = 2));
    var A: [D] int;

    for loc in MyLocales do on loc {

      // The ReplicatedDim specifier always accesses the local replicand.
      //
      // Therefore, 'forall a in A' when executed on MyLocales[loc1,loc2]
      // visits only the replicands on MyLocales[loc1,0..N_2-1].

      forall a in A do
        a = here.id;

      // Technicality: 'writeln(A)' would read A always on Locale 0.
      // Since we want to see what A contains on the current locale,
      // we use default-distributed 'Helper'.
      // 'Helper = A' captures the view of A on the current locale,
      // which we then print out.

      writeln("On ", here, ":");
      const Helper: [Space] int = A;
      writeln(Helper);
      writeln();
    }

When run on 6 locales, the output is:

  ::

    On LOCALE0:
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0

    On LOCALE1:
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0

    On LOCALE2:
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0
    0 0 1 1 2 2 0 0

    On LOCALE3:
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3

    On LOCALE4:
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3

    On LOCALE5:
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3
    3 3 4 4 5 5 3 3


**Initializer Arguments**

The ``DimensionalDist2D`` class initializer is defined as follows:

  .. code-block:: chapel

    proc DimensionalDist2D.init(
      targetLocales: [] locale,
      di1,
      di2,
      name: string = "dimensional distribution",
      type idxType = int,
      dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
      dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
      dataParMinGranularity     = // value of  dataParMinGranularity      config const )

The argument ``targetLocales`` must be a 2D array indicating
the locales to distribute over.

The arguments ``di1`` and ``di2`` are the desired dimension specifiers
for the first and second dimension, respectively.

The ``name`` argument may be useful for debugging.
It is stored and otherwise ignored by the implementation.

The ``idxType`` argument must match the index type of the domains
"dmapped" using that DimensionalDist2D instance.

The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
and ``dataParMinGranularity`` set the knobs that are used to
control intra-locale data parallelism for Block-distributed domains
and arrays in the same way that the like-named config constants
control data parallelism for ranges and default-distributed domains
and arrays.


**Dimension Specifiers**

Presently, the following dimension specifiers are available
(shown here with their initializer arguments):

* :class:`ReplicatedDim(numLocales) <ReplicatedDim>`
* :class:`BlockDim(numLocales, boundingBox, idxType=boundingBox.idxType) <BlockDim>`
* :class:`BlockCyclicDim(numLocales, lowIdx, blockSize) <BlockCycDim>`


**Limitations**

Only 2D domains and arrays are supported.

There may be performance issues when scaling to a large number of locales.
*/
record dimensionalDist2D: writeSerializable {
  type targLocType;
  type di1t;
  type di2t;
  type idxType;
  forwarding const chpl_distHelp: chpl_PrivatizedDistHelper(unmanaged DimensionalDist2DImpl(targLocType, di1t, di2t, idxType));

  proc init(targetLocales: [] locale,
            di1,
            di2,
            name: string = "dimensional distribution",
            type idxType = int,
            dataParTasksPerLocale=getDataParTasksPerLocale(),
            dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
            dataParMinGranularity=getDataParMinGranularity()) {
    this.targLocType = targetLocales.type;
    this.di1t = di1.type;
    this.di2t = di2.type;
    this.idxType = idxType;
    const value = new unmanaged DimensionalDist2DImpl(targetLocales, di1, di2,
                                                      name,
                                                      idxType,
                                                      dataParTasksPerLocale,
                                                      dataParIgnoreRunningTasks,
                                                      dataParMinGranularity);
    this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                          if _isPrivatized(value)
                            then _newPrivatizedClass(value)
                            else nullPid,
                          value);
  }

    proc init(_pid : int, _instance, _unowned : bool) {
      this.targLocType = _instance.targLocType;
      this.di1t = _instance.di1t;
      this.di2t = _instance.di2t;
      this.idxType = _instance.idxType;
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(_pid,
                                                         _instance,
                                                         _unowned);
    }

    proc init(value) {
      this.targLocType = value.targLocType;
      this.di1t = value.di1t;
      this.di2t = value.di2t;
      this.idxType = value.idxType;
      this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                             if _isPrivatized(value)
                               then _newPrivatizedClass(value)
                               else nullPid,
                             _to_unmanaged(value));
    }

    // Note: This does not handle the case where the desired type of 'this'
    // does not match the type of 'other'. That case is handled by the compiler
    // via coercions.
    proc init=(const ref other : dimensionalDist2D(?)) {
      this.init(other._value.dsiClone());
    }

    proc clone() {
      return new dimensionalDist2D(this._value.dsiClone());
    }

  @chpldoc.nodoc
  inline operator ==(d1: dimensionalDist2D(?), d2: dimensionalDist2D(?)) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  @chpldoc.nodoc
  inline operator !=(d1: dimensionalDist2D(?), d2: dimensionalDist2D(?)) {
    return !(d1 == d2);
  }

  proc serialize(writer, ref serializer) throws {
    chpl_distHelp.serialize(writer, serializer);
  }
}


@chpldoc.nodoc
@unstable(category="experimental", reason="assignment between distributions is currently unstable due to lack of testing")
operator =(ref a: dimensionalDist2D(?), b: dimensionalDist2D(?)) {
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


@deprecated("'DimensionalDist2D' is deprecated, please use 'dimensionalDist2D' instead")
type DimensionalDist2D = dimensionalDist2D;


class DimensionalDist2DImpl : BaseDist, writeSerializable {
  type targLocType;
  type di1t;
  type di2t;
  // the desired locales to distribute things over;
  // must be a [domain(2, locIdT, false)] locale
  const targetLocales: targLocType;
  // "IDs" are indices into targetLocales
  proc targetIds do return targetLocales.domain;

  // the dimension specifiers - ones being combined
  var di1: di1t, di2: di2t;

  // for debugging/tracing (remove later)
  var name: string;

  // the domains' idxType that we support
  type idxType = int;
  // the type of the corresponding domain/array indices
  // todo: it does not really apply to dsiIndexToLocale()
  proc indexT type do  return if rank == 1 then idxType else rank * idxType;

  // the count and size of each dimension of targetLocales
  // implementation note: 'rank' is not a real param; it's just that having
  // 'proc rank param do return targetLocales.rank' did not work
  param rank: int = targetLocales.rank;
  proc numLocs1: locCntT do  return targetIds.dim(0).sizeAs(locCntT);
  proc numLocs2: locCntT do  return targetIds.dim(1).sizeAs(locCntT);

  // parallelization knobs
  var dataParTasksPerLocale: int      = getDataParTasksPerLocale();
  var dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks();
  var dataParMinGranularity: int      = getDataParMinGranularity();
}

// class LocDimensionalDist - no local distribution descriptor - for now

// helper for locDdescType: any of storage index ranges can be stridable
private proc stoStridableDom(type stoIndexT, dom1, dom2) param {
  proc stoStridable1d(dom1d) param {
    var dummy1 = dom1d.dsiNewLocalDom1d(stoIndexT, 0:locIdT);
    const dummy = dummy1.dsiSetLocalIndices1d(dom1d, 0:locIdT);
    return dummy.strides;
  }
  return chpl_strideUnion(stoStridable1d(dom1), stoStridable1d(dom2));
}

private proc locDescTypeHelper(param rank : int, type idxType, dom1, dom2) type {
  type d1type = dom1.dsiNewLocalDom1d(idxType, 0).type;
  type d2type = dom2.dsiNewLocalDom1d(idxType, 0).type;

  proc strideHelper(dom1d) param {
    var dummy1 = dom1d.dsiNewLocalDom1d(idxType, 0);
    const dummy = dummy1.dsiSetLocalIndices1d(dom1d, 0);
    return dummy.strides;
  }

  param str = chpl_strideUnion(strideHelper(dom1), strideHelper(dom2));

  return unmanaged LocDimensionalDom(domain(rank, idxType, str), d1type, d2type);
}

class DimensionalDom : BaseRectangularDom(?) {
  // required
  const dist; // not reprivatized

  // convenience
  proc targetIds do return localDdescs.domain;
  proc rangeT  type do  return range(idxType, boundKind.both, strides);
  proc domainT type do  return domain(rank, idxType, strides);
  proc indexT  type do  return dist.indexT;

  // subordinate 1-d global domain descriptors
  var dom1, dom2;

  // This is our index set; we store it here so we can get to it easily.
  // Although strictly speaking it is not necessary.
  var whole: domain(rank, idxType, strides);

  // This is the idxType of the "storage index ranges" to be produced
  // by dsiSetLocalIndices1d(). It needs to be uniform across dimensions,
  // and 'idxType' is the easiest choice (although not the most general).
  // NB it's also computed in DimensionalDist2DImpl.dsiNewRectangularDom().
  proc stoIndexT type do  return idxType;

  // replace this with 'this.stridable' for simplicity?
  proc stoStridable param do  return stoStridableDom(stoIndexT, dom1, dom2);
  proc stoRangeT type do  return range(stoIndexT, strides = stoStridable);
  proc stoDomainT type do  return domain(rank, stoIndexT, stoStridable);

  // convenience - our instantiation of LocDimensionalDom
  proc locDdescType type do  return unmanaged LocDimensionalDom(stoDomainT,
                                         dom1.dsiNewLocalDom1d(stoIndexT, 0:locIdT).type,
                                         dom2.dsiNewLocalDom1d(stoIndexT, 0:locIdT).type);

  // local domain descriptors, not reprivatized
  // INIT TODO: Used to use 'locDdescType' instead of 'locDescTypeHelper'. Can
  // we clean this up?
  var localDdescs: [dist.targetIds] locDescTypeHelper(rank, idxType, dom1, dom2); // locDdescType
}

class LocDimensionalDom {
  type myStorageDomT;

  // myStorageDom: what storage to allocate for an array on our locale.
  // Its indices are not necessarily user indices. However,
  // we use it to learn *how many* indices there are on our locale.
  //
  // Currently there are no direct constraints on how many indices
  // should go on each locale. An indirect constraint is that dsiAccess1d()
  // should work (as desired for the given distribution).
  //
  var myStorageDom: myStorageDomT;

  // subordinate 1-d local domain descriptors
  var doml1, doml2;
}

class DimensionalArr : BaseRectangularArr(?) {
  // required
  const dom; // must be a DimensionalDom

  // same as 'dom'; for an alias (e.g. a slice), 'dom' of the original array
  const allocDom: dom.type; // must be a DimensionalDom

  override proc rank param do return dom.rank;

  proc targetIds do return localAdescs.domain;

  // no subordinate 1-d array descriptors - we handle storage ourselves

  // local array descriptors (from the original array, if this is an alias)
  // NOTE: 'dom' must be initialized prior to initializing 'localAdescs'
  var localAdescs: [dom.targetIds]
                      unmanaged LocDimensionalArr(eltType, allocDom.locDdescType);
}

class LocDimensionalArr : writeSerializable {
  type eltType;
  const locDom;  // a LocDimensionalDom
  pragma "local field" pragma "unsafe"
  // may be initialized separately
  var myStorageArr: [locDom.myStorageDom] eltType;

  proc init(type eltType,
            const locDom,
            param initElts: bool) {
    this.eltType = eltType;
    this.locDom = locDom;
    this.myStorageArr = this.locDom.myStorageDom.buildArray(eltType,
                                                            initElts=initElts);
  }

  proc deinit() {
    // Elements in myStorageArr are deinited in dsiDestroyArr if necessary.
  }

  // guard against dynamic dispatch resolution trying to resolve
  // write()ing out an array of sync vars and hitting the sync var
  // type's compilerError()
  override proc serialize(writer, ref serializer) throws {
    halt("LocDimensionalArr.serialize() is not implemented / should not be needed");
  }
}


/// distribution ////////////////////////////////////////////////////////////


//== initialization, cloning

// initializer
// gotta list all the things we let the user set
proc DimensionalDist2DImpl.init(
  targetLocales: [] locale,
  di1,
  di2,
  name: string = "dimensional distribution",
  type idxType = int,
  dataParTasksPerLocale: int      = getDataParTasksPerLocale(),
  dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks(),
  dataParMinGranularity: int      = getDataParMinGranularity()
) {
  this.targLocType = targetLocales.type;
  this.di1t = di1.type;
  this.di2t = di2.type;
  this.targetLocales = targetLocales;
  this.di1 = di1;
  this.di2 = di2;
  this.name = name;
  this.idxType = idxType;
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  init this;

  checkInvariants();

  _passLocalLocIDsDist(this.di1, true, this.di2, true,
                     this.targetLocales, true, this.targetLocales.domain.lowBound);
}

//
// Having targetLocales be a constant means we have to reshape Locales
// before invoking the initializer. This method does that.
// It also offers the convenience of passing just di1 and di2 and
// using 'Locales' for targetLocales by default.
// Recall that di1 and di2 determine the number of locales in each dimension.
//
proc newDimensionalDist2D(
  di1,
  di2,
  targetLocales: [] locale = Locales,
  name: string = "dimensional distribution",
  type idxType = int,
  dataParTasksPerLocale: int      = getDataParTasksPerLocale(),
  dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks(),
  dataParMinGranularity: int      = getDataParMinGranularity()
) {
  if targetLocales.rank != 1 then compilerError("newDimensionalDist2D() is provided only for 1D targetLocales arrays");
  const (nl1, nl2) = (di1.numLocales, di2.numLocales);
  var reshapedLocales = reshape(targetLocales[0..#nl1*nl2],{0..#nl1,0..#nl2});

  return new dimensionalDist2D(reshapedLocales, di1, di2, name, idxType,
   dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}


// Check all restrictions/assumptions that must be satisfied by the user
// when initializing a DimensionalDist2D.
proc DimensionalDist2DImpl.checkInvariants(): void {
  proc ensure(param cond:bool, param msg:string) {
    if !cond then compilerError(msg, 3);
  }
  ensure(targetLocales.rank == 2, "DimensionalDist2D requires 'targetLocales' to be a 2-dimensional array, got " + targetLocales.rank:string + " dimension(s)");
  ensure(rank == targetLocales.rank, "DimensionalDist2D bug: inconsistent rank");
  ensure(targetLocales.eltType == locale, "DimensionalDist2D requires 'targetLocales' to be an array of locales, got an array of " + targetLocales.eltType:string);
  ensure(targetIds.idxType == locIdT, "DimensionalDist2D currently requires 'idxType' of 'targetLocales.domain' to be " + locIdT:string + ", got " + targetIds.idxType:string);

  assert(targetIds == {0..#numLocs1, 0..#numLocs2}, "DimensionalDist2D-targetIds");
  assert(di1.numLocales == numLocs1, "DimensionalDist2D-numLocales-1");
  assert(di2.numLocales == numLocs2, "DimensionalDist2D-numLocales-2");
  assert(dataParTasksPerLocale > 0, "DimensionalDist2D-dataParTasksPerLocale");
  assert(dataParMinGranularity > 0, "DimensionalDist2D-dataParMinGranularity");

  ensure(!(isClass(di1) || isClass(di2)), "DimensionalDist2D does not support dimension specifiers that are classes");
  /* If these are classes, the deinitializer should delete them if we own them.
     Note: a privatized copy does not own them if they are not privatized. */
}

proc DimensionalDist2DImpl.dsiClone(): _to_unmanaged(this.type) {
  _traceddd("DimensionalDist2DImpl.dsiClone");
  checkInvariants();

  // do this simple thing, until we find out that we need something else
  return _to_unmanaged(this);
}

//== targetLocales, localSubdomain

proc DimensionalDist2DImpl.dsiTargetLocales() const ref do return targetLocales;
proc DimensionalDom.dsiTargetLocales()    const ref do return dist.targetLocales;
proc DimensionalArr.dsiTargetLocales()    const ref do return dom.dist.targetLocales;

proc DimensionalDom.dsiHasSingleLocalSubdomain() param do return true;
proc DimensionalArr.dsiHasSingleLocalSubdomain() param do return true;

proc DimensionalDom.dsiLocalSubdomain(loc: locale) {
  import ChplConfig;
  if ChplConfig.CHPL_COMM != "none" then
    compilerError("DimensionalDist2DImpl.dsiLocalSubdomain() is currently not implemented for non-local compilation");
  return whole;
  // for multilocale, start here:
  // const (locIds, foundIt) = _CurrentLocaleToLocIDs(dist.targetLocales, loc);
  // var result: whole.type;
  // if !foundIt then return empty result;
}

proc DimensionalArr.dsiLocalSubdomain(loc: locale) {
  return dom.dsiLocalSubdomain(loc);
}


//== privatization

override proc DimensionalDist2DImpl.dsiSupportsPrivatization() param do return true;

proc DimensionalDist2DImpl.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  return (targetLocales, name, dataParTasksPerLocale,
          dataParIgnoreRunningTasks, dataParMinGranularity,
          di1.dsiGetPrivatizeData1d(), di2.dsiGetPrivatizeData1d());
}

proc DimensionalDist2DImpl.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

  // ensure we get a local copy of targetLocales
  // todo - provide the following as utility functions (for domains, arrays)
  const pdTargetLocales = privatizeData(0);
  const privTargetIds: domain(pdTargetLocales.domain.rank,
                              pdTargetLocales.domain.idxType,
                              pdTargetLocales.domain.strides
                              ) = pdTargetLocales.domain;
  const privTargetLocales: [privTargetIds] locale = pdTargetLocales;

  var di1new = di1.type.dsiPrivatize1d(privatizeData(5));
  var di2new = di2.type.dsiPrivatize1d(privatizeData(6));
  const plliddDummy: privTargetLocales.domain.lowBound.type;
  _passLocalLocIDsDist(di1new, true, di2new, true,
                       privTargetLocales, false, plliddDummy);

  return new unmanaged DimensionalDist2DImpl(targetLocales = privTargetLocales,
                             name          = privatizeData(1),
                             idxType       = this.idxType,
                             di1           = di1new,
                             di2           = di2new,
                             dataParTasksPerLocale     = privatizeData(2),
                             dataParIgnoreRunningTasks = privatizeData(3),
                             dataParMinGranularity     = privatizeData(4),
                             dummy = 0);
}

// initializer of a privatized copy
// ('dummy' distinguishes it from the user initializer)
proc DimensionalDist2DImpl.init(param dummy: int,
  targetLocales: [] locale,
  name,
  type idxType,
  di1,
  di2,
  dataParTasksPerLocale,
  dataParIgnoreRunningTasks,
  dataParMinGranularity
) {
  this.targLocType = targetLocales.type;
  this.di1t = di1.type;
  this.di2t = di2.type;
  this.targetLocales = targetLocales;
  this.di1 = di1;
  this.di2 = di2;
  this.name = name;
  this.idxType = idxType;
  this.dataParTasksPerLocale     = dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity     = dataParMinGranularity;

  init this;

  // should not need it, but run it for now just in case
  checkInvariants();
}


//== miscellanea

proc DimensionalDist2DImpl.dsiEqualDMaps(that: DimensionalDist2DImpl) {
  return this.di1 == that.di1   &&
         this.di2 == that.di2   &&
         this.targetLocales.equals(that.targetLocales);
}

proc DimensionalDist2DImpl.dsiEqualDMaps(that) param {
  return false;
}

proc DimensionalDist2DImpl.dimSpecifier(param dim: int) {
  if dim == 1 then
    return di1;
  else if dim == 2 then
    return di2;
  else
    compilerError("DimensionalDist2DImpl presently supports dimSpecifier()",
                  " only for dimension 1 or 2, got dim=", dim:string);
}


//== index to locale

//Given an index, this should return the locale that owns that index.
proc DimensionalDist2DImpl.dsiIndexToLocale(indexx: indexT): locale {
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist2DImpl presently supports only indexing with",
                  " 2-tuples; got an index of the type ",
                  indexx.type:string);

  return targetLocales(di1.dsiIndexToLocale1d(indexx(0)):int,
                       di2.dsiIndexToLocale1d(indexx(1)):int);
}

// Find ix such that targetLocales(ix) == here.
// If there is more than one answer, return one of them.
// 'targetLocales' is passed explicitly because the caller may have
// a local copy of it to use.
//
// We are justified, sort-of, to run linear search because it is invoked
// when the targetLocales array is assigned, so this is at most a constant
// factor of extra time on top of that.
//
private proc _CurrentLocaleToLocIDs(targetLocales, desiredLocale)
                                     : (targetLocales.rank*locIdT, bool)
{
  var result: targetLocales.rank * locIdT;
  // guard updates to 'result' to ensure atomicity of updates
  var gotresult: sync bool = false;
  forall (lls, loc) in zip(targetLocales.domain, targetLocales) with (ref result) do
    if loc == desiredLocale {
      // if we get multiple matches, we do not specify which is returned
      // could add a pre-test if it were cheap: if !gotresult.readXX()
      gotresult.readFE();
      result = lls;
      gotresult.writeEF(true);
    }
  // instead of crashing right away, return a flag
  //if !gotresult.readXX() then halt("DimensionalDist2DImpl: the current locale ", desiredLocale, " is not among the target locales ", targetLocales);

  return (result, gotresult.readXX());
}

// How we usually invoke _CurrentLocaleToLocIDs().
proc _passLocalLocIDsDist(ref d1, param doD1:bool, ref d2, param doD2:bool,
                          targetLocales, gotHint:bool, hint): void
{
 // otherwise don't bother generating any code
 if d1.dsiUsesLocalLocID1d() || d2.dsiUsesLocalLocID1d() {
  // now do the runtime checks, too
  if (d1.dsiUsesLocalLocID1d() && doD1) || (d2.dsiUsesLocalLocID1d() && doD2) {

   // 'l' stands for "legitimate?"
   const (lIds, l): (targetLocales.rank * locIdT, bool) =
     if gotHint && targetLocales(hint) == here
       then (hint, true)
     else
       _CurrentLocaleToLocIDs(targetLocales, here);

   if d1.dsiUsesLocalLocID1d() && doD1 then d1.dsiStoreLocalLocID1d(lIds(0),l);
   if d2.dsiUsesLocalLocID1d() && doD2 then d2.dsiStoreLocalLocID1d(lIds(1),l);
  }
 }
}

// Subordinate 1-d domains copy the local locId from their distributions.
proc _passLocalLocIDsDom1d(ref dom1d, dist1d) {
  if dom1d.dsiUsesLocalLocID1d() {

    // ensure dist1d.dsiGetLocalLocID1d() is available
    if !dist1d.dsiUsesLocalLocID1d() then compilerError("DimensionalDist2DImpl: currently, when a subordinate 1d distribution requires localLocID for *domain* descriptors, it must also require them for *distribution* descriptors");

    dom1d.dsiStoreLocalLocID1d(dist1d.dsiGetLocalLocID1d());
  }
}


/// domain //////////////////////////////////////////////////////////////////

proc DimensionalDom.deinit() {
  if isClass(dom1) || isClass(dom2) then
    compilerError("1-d domain descriptors cannot be classes");
  /* If these are classes, the deinitializer should delete them if we own them.
     Note: a privatized copy does not own them if they are not privatized. */
}

proc LocDimensionalDom.deinit() {
  if isClass(doml1) | isClass(doml2) then
    compilerError("1-d local domain descriptors cannot be classes");
  /* If these are classes, they should probably be 'owned'
     so they are deleted properly. */
}


//== privatization

override proc DimensionalDom.dsiSupportsPrivatization() param do return true;

proc DimensionalDom.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  return (dist.pid, dom1.dsiGetPrivatizeData1d(), dom2.dsiGetPrivatizeData1d(),
          whole.dims(), localDdescs);
}

proc DimensionalDom.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

  var privdist = chpl_getPrivatizedCopy(objectType = this.dist.type,
                                        objectPid  = privatizeData(0));

    var dom1new = dom1.type.dsiPrivatize1d(privdist.di1, privatizeData(1));
    _passLocalLocIDsDom1d(dom1new, privdist.di1);

    var dom2new = dom2.type.dsiPrivatize1d(privdist.di2, privatizeData(2));
    _passLocalLocIDsDom1d(dom2new, privdist.di2);

  const result = new unmanaged DimensionalDom(rank      = this.rank,
                                    idxType   = this.idxType,
                                    strides   = this.strides,
                                    dist = privdist,
                                    dom1 = dom1new,
                                    dom2 = dom2new,
                                    whole       = {(...privatizeData(3))},
                                    localDdescs = privatizeData(4));

  // update local-to-global pointers as needed
  param lg1 = dom1new.dsiLocalDescUsesPrivatizedGlobalDesc1d();
  param lg2 = dom2new.dsiLocalDescUsesPrivatizedGlobalDesc1d();
  if lg1 || lg2 then
    // We are justified, sort-of, to go over the entire localDdescs
    // because we have just gone over them above, so it's only
    // a constant factor of extra time on top of that.
    forall locDdesc in result.localDdescs do
      if locDdesc.locale == here {
        if lg1 then locDdesc.doml1
                      .dsiStoreLocalDescToPrivatizedGlobalDesc1d(dom1new);
        if lg2 then locDdesc.doml2
                      .dsiStoreLocalDescToPrivatizedGlobalDesc1d(dom2new);
      }

  return result;
}

proc DimensionalDom.dsiGetReprivatizeData() {
  _traceddd(this, ".dsiGetReprivatizeData");

  return (dom1.dsiGetReprivatizeData1d(),
          dom2.dsiGetReprivatizeData1d(),
          whole);
}

proc DimensionalDom.dsiReprivatize(other, reprivatizeData) {
  _traceddd(this, ".dsiReprivatize on ", here.id);

  compilerAssert(this.rank == other.rank &&
                 this.idxType == other.idxType &&
                 this.strides == other.strides);

  dom1.dsiReprivatize1d(reprivatizeData(0));
  dom2.dsiReprivatize1d(reprivatizeData(1));

  this.whole = reprivatizeData(2);
}


//== miscellanea

proc DimensionalDom.dimSpecifier(param dim: int) {
  return dist.dimSpecifier(dim);
}


//== creation, SetIndices

// create a new domain mapped with this distribution
override proc DimensionalDist2DImpl.dsiNewRectangularDom(param rank: int,
                                            type idxType,
                                            param strides: strideKind,
                                            inds)
//  : DimensionalDom(rank, idxType, strides, this.type, ...)
{
  _traceddd(this, ".dsiNewRectangularDom ",
            (rank, idxType:string, strides, inds));
  if rank != 2 then
    compilerError("DimensionalDist2DImpl presently supports only 2 dimensions,",
                  " got ", rank:string, " dimensions");

  // todo: ideally, this will not be required;
  // furthermore, DimensionalDist2DImpl shouldn't be specific to idxType.
  if idxType != this.idxType then
    compilerError("The domain index type ", idxType:string,
                  " does not match the index type ",this.idxType:string,
                  " of the DimensionalDist2DImpl used to map that domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDist2DImpl used to map that domain");

  // need this for dsiNewRectangularDom1d()
  type stoIndexT = this.idxType;

  var dom1 = di1.dsiNewRectangularDom1d(idxType, strides, stoIndexT);
  _passLocalLocIDsDom1d(dom1, di1);

  var dom2 = di2.dsiNewRectangularDom1d(idxType, strides, stoIndexT);
  _passLocalLocIDsDom1d(dom2, di2);

  // We could try the dummyLB trick from BlockDist instead of the '?'/'!',
  // although that would be more complex here because of 'dom1' and 'dom2'.
  // Ditto for localAdescsTemp.
  var localDdescsTemp: [this.targetIds]
                         locDescTypeHelper(rank, idxType, dom1, dom2)?;
  for (loc, locIds, locDdesc)
   in zip(targetLocales, targetIds, localDdescsTemp) do
    on loc do
      locDdesc = new unmanaged LocDimensionalDom(
              // stoDomainT -- see compilerAssert below; must be local
              domain(rank, idxType, stoStridableDom(stoIndexT, dom1, dom2)),
                       doml1 = dom1.dsiNewLocalDom1d(stoIndexT, locIds(0)),
                       doml2 = dom2.dsiNewLocalDom1d(stoIndexT, locIds(1)));

  var localDdescsNN = localDdescsTemp!; //#15080
  const result = new unmanaged DimensionalDom(rank=rank, idxType=idxType,
                                    strides=strides, dist=_to_unmanaged(this),
                                    localDdescs = localDdescsNN,
                                    dom1 = dom1, dom2 = dom2);
  // result.whole is initialized to the default value (empty domain)

  compilerAssert(stoIndexT == result.stoIndexT, "bug in DimensionalDist2DImpl: inconsistent stoIndexT");
  // stoDomainT -- keep in sync with the above
  compilerAssert(domain(rank, idxType, stoStridableDom(stoIndexT, dom1, dom2)) == result.stoDomainT, "bug in DimensionalDist2DImpl: inconsistent stoDomainT");

  result.dsiSetIndices(inds);
  return result;
}

// common redirects
override proc DimensionalDom.dsiLow do           return whole.lowBound;
override proc DimensionalDom.dsiHigh do          return whole.highBound;
override proc DimensionalDom.dsiAlignedLow do    return whole.low;
override proc DimensionalDom.dsiAlignedHigh do   return whole.high;
override proc DimensionalDom.dsiFirst do         return whole.first;
override proc DimensionalDom.dsiLast do          return whole.last;
override proc DimensionalDom.dsiStride do        return whole.stride;
override proc DimensionalDom.dsiAlignment do     return whole.alignment;
proc DimensionalDom.dsiNumIndices do    return whole.sizeAs(uint);
proc DimensionalDom.dsiDim(d) do        return whole.dim(d);
proc DimensionalDom.dsiDim(param d) do  return whole.dim(d);
proc DimensionalDom.dsiDims() do        return whole.dims();
//proc DimensionalDom.dsiGetIndices() do  return whole.getIndices();
proc DimensionalDom.dsiMember(i) do     return whole.contains(i);
proc DimensionalDom.doiToString() do    return whole:string;
proc DimensionalDom.dsiSerialWrite(x) do { x.write(whole); }
proc DimensionalDom.dsiLocalSlice(param strides, ranges) do return whole((...ranges));
override proc DimensionalDom.dsiIndexOrder(i) do              return whole.indexOrder(i);
override proc DimensionalDom.dsiMyDist() do                   return dist;

proc DimensionalDom.dsiGetDist() {
  if _isPrivatized(dist) then
    return new dimensionalDist2D(dist.pid, dist, _unowned=true);
  else
    return new dimensionalDist2D(nullPid, dist, _unowned=true);
}

proc DimensionalDom.dsiSetIndices(newIndices: domainT): void {
  whole = newIndices;
  _dsiSetIndicesHelper(newIndices.dims());
}

proc DimensionalDom.dsiSetIndices(newRanges: rank * rangeT): void {
  whole = {(...newRanges)};
  _dsiSetIndicesHelper(newRanges);
}

proc DimensionalDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

// not part of DSI
proc DimensionalDom._dsiSetIndicesHelper(newRanges: rank * rangeT): void {
  use IO;

  _traceddd(this, ".dsiSetIndices", newRanges);
  if rank != 2 then
    compilerError("DimensionalDist2DImpl presently supports only 2 dimensions,",
                  " got a domain with ", rank, " dimensions");

  dom1.dsiSetIndices1d(newRanges(0));
  dom2.dsiSetIndices1d(newRanges(1));

  // could omit this warning if the intersection between the old and the new
  // domains is empty; could change it to halt("unimplemented")
  if dom1.dsiSetIndicesUnimplementedCase||dom2.dsiSetIndicesUnimplementedCase
    then if _arrs_containing_dom > 0 then
      stderr.writeln("warning: array resizing will not preserve array contents upon change in dimension stride with 1-d BlockCyclic distribution");

  coforall (locId, locDD) in zip(targetIds, localDdescs) do
    on locDD do
     locDD._dsiLocalSetIndicesHelper(stoRangeT, (dom1,dom2), locId);
}

// not part of DSI
// TODO: need to preserve the old contents
// in the intersection of the old and new domains' index sets
proc LocDimensionalDom._dsiLocalSetIndicesHelper(type stoRangeT, globDD, locId)
{
  var myRange1: stoRangeT = doml1.dsiSetLocalIndices1d(globDD(0),locId(0));
  var myRange2: stoRangeT = doml2.dsiSetLocalIndices1d(globDD(1),locId(1));

  myStorageDom = {myRange1, myRange2};

  _traceddd("DimensionalDom.dsiSetIndices on ", here.id, " ", locId,
            "  storage ", myStorageDom);
}

proc DimensionalDom.dsiGetIndices(): rank * range(idxType,
                                                 boundKind.both,
                                                 strides) {
  _traceddd(this, ".dsiGetIndices");
  return whole.dims();
}


/// array ///////////////////////////////////////////////////////////////////


//== privatization

override proc DimensionalArr.dsiSupportsPrivatization() param do return true;

proc DimensionalArr.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  return (dom.pid, allocDom.pid, localAdescs);
}

proc DimensionalArr.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

  const idDom = privatizeData(0);
  const idAllocDom = privatizeData(1);

  const privDom = chpl_getPrivatizedCopy(objectType = this.dom.type,
                                         objectPid  = idDom);

  const privAllocDom =
    if !this.mustbeAlias && idAllocDom == idDom
    then privDom
    else chpl_getPrivatizedCopy(objectType = this.allocDom.type,
                                objectPid  = idAllocDom);

  const result = new unmanaged DimensionalArr(rank     = this.rank,
                                    idxType  = this.idxType,
                                    strides  = this.strides,
                                    eltType  = this.eltType,
                                    localAdescs = privatizeData(2),
                                    dom      = privDom,
                                    allocDom = privAllocDom);

  assert(result.isAlias == this.isAlias);
  return result;
}


//== miscellanea

override proc DimensionalArr.idxType type do return dom.idxType; // (could be a field)

override proc DimensionalArr.dsiGetBaseDom() do return dom;

proc DimensionalArr.dimSpecifier(param dim: int) {
  return dom.dimSpecifier(dim);
}

proc DimensionalArr.mustbeAlias param do
  return this.dom.type != this.allocDom.type;

proc DimensionalArr.isAlias do
  return this.dom != this.allocDom;


//== creation and destruction

// create a new array over this domain
proc DimensionalDom.dsiBuildArray(type eltType, param initElts:bool)
{
  _traceddd(this, ".dsiBuildArray");
  if rank != 2 then
    compilerError("DimensionalDist2DImpl presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  var localAdescsTemp: [this.targetIds]
        unmanaged LocDimensionalArr(eltType, this.locDdescType)?;

  coforall (loc, locDdesc, locAdesc)
   in zip(dist.targetLocales, localDdescs, localAdescsTemp) do
    on loc do
      locAdesc = new unmanaged LocDimensionalArr(eltType, locDdesc,
                                                 initElts=initElts);

  var localAdescsNN = localAdescsTemp!; //#15080
  const result = new unmanaged DimensionalArr(rank = rank,
                                    idxType = idxType,
                                    strides = strides,
                                    eltType  = eltType,
                                    localAdescs = localAdescsNN,
                                    dom      = _to_unmanaged(this),
                                    allocDom = _to_unmanaged(this));
  assert(!result.isAlias);
  return result;
}


override proc DimensionalDom.dsiDestroyDom() {
  coforall desc in localDdescs do
    on desc do
      delete desc;

  if isClass(dom2) then delete dom2;
  if isClass(dom1) then delete dom1;
}


//== dsiAccess

proc DimensionalArr.dsiAccess(indexx: dom.indexT) ref: eltType {
  if traceDimensionalDist || traceDimensionalDistDsiAccess {
    _traceddc(traceDimensionalDist || traceDimensionalDistDsiAccess,
              this, ".dsiAccess", indexx);
  }

  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist2DImpl presently supports only indexing with",
                  " 2-tuples; got an array index of the type ",
                  indexx.type:string);

  const alDom = this.allocDom;
  const (l1,i1):(locIdT, alDom.stoIndexT) = alDom.dom1.dsiAccess1d(indexx(0));
  const (l2,i2):(locIdT, alDom.stoIndexT) = alDom.dom2.dsiAccess1d(indexx(1));
  const locAdesc = this.localAdescs[l1,l2];
//writeln("locAdesc.myStorageArr on ", locAdesc.myStorageArr.locale.id);
  return locAdesc.myStorageArr(i1,i2);
}

proc DimensionalArr.dsiBoundsCheck(i: dom.indexT) {
  return allocDom.dsiMember(i);
}

//== writing

proc DimensionalArr.dsiSerialWrite(f): void {
  _traceddd(this, ".dsiSerialWrite on ", here.id,
            if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  iter iHelp(param d) {
    if this.isAlias {
       // Go to the original array and invoke the follower iterator on it,
       // giving the alias's entire domain as the index set to follow.
       // (NB dsiFollowerArrayIterator1d's argument is not densified.)
      const dom1d = if d == 0 then this.allocDom.dom1 else this.allocDom.dom2;
      foreach l_i in dom1d.dsiFollowerArrayIterator1d(this.dom.whole.dim(d)) do
        yield l_i;

    } else {
      const alDom = this.dom;
      const dom1d = if d == 0 then alDom.dom1 else alDom.dom2;
      foreach (l,r) in dom1d.dsiSerialArrayIterator1d() do
        foreach i in r do
          yield (l,i);
    }
  }

  // we largely follow DimensionalArr.these()
  // instead could just use BlockArr.dsiSerialWrite
  if this.dom.dsiNumIndices == 0 then return;

  var nextD1 = false;
  for (l1,i1) in iHelp(0) {
      if nextD1 then f.writeln();
      nextD1 = true;

      var nextD2 = false;
      for (l2,i2) in iHelp(1) {
          const locAdesc = this.localAdescs[l1,l2];
          const elem = locAdesc.myStorageArr(i1,i2);
          if nextD2 then f.write(" ");
          f.write(elem);
          nextD2 = true;
        }
    }
}


/// local slicing, reallocation //////////////////////////

pragma "no copy return"
proc DimensionalArr.dsiLocalSlice((sliceDim1, sliceDim2)) {
  const dom = this.dom;
  const dist = dom.dist;
  // todo: cache (l1, l2) in privatized copies when possible
  // (i.e. if privatization is supported and there is no oversubscription)
  // Assuming dsiLocalSlice is guaranteed to be local to 'here'.
  const l1 = dist.di1.dsiIndexToLocale1d(if sliceDim1.hasLowBound() then sliceDim1.lowBound else chpl__intToIdx(sliceDim1.idxType, 1)),
        l2 = dist.di2.dsiIndexToLocale1d(if sliceDim2.hasLowBound() then sliceDim2.lowBound else chpl__intToIdx(sliceDim2.idxType, 1)),
        locAdesc = this.localAdescs[l1, l2],
        r1 = if dom.dom1.dsiStorageUsesUserIndices()
             then dom.whole.dim(0)(sliceDim1)
             else locAdesc.locDom.doml1.dsiLocalSliceStorageIndices1d(dom.dom1, sliceDim1),
        r2 = if dom.dom2.dsiStorageUsesUserIndices()
             then dom.whole.dim(1)(sliceDim2)
             else locAdesc.locDom.doml2.dsiLocalSliceStorageIndices1d(dom.dom2, sliceDim2);

  return locAdesc.myStorageArr[r1, r2];
}

proc DimensionalArr.dsiReallocate(d: domain) {
  // nothing
  // TODO: handle block-cyclic 1d when the stride changes
}

override proc DimensionalArr.dsiPostReallocate() {
  // nothing for now
}

override proc DimensionalArr.dsiElementInitializationComplete() {
  coforall desc in localAdescs {
    on desc {
      desc.myStorageArr.dsiElementInitializationComplete();
    }
  }
}

override proc DimensionalArr.dsiElementDeinitializationComplete() {
  coforall desc in localAdescs {
    on desc {
      desc.myStorageArr.dsiElementDeinitializationComplete();
    }
  }
}

override proc DimensionalArr.dsiDestroyArr(deinitElts:bool) {
  coforall desc in localAdescs {
    on desc {
      if deinitElts then
        _deinitElements(desc.myStorageArr);
      desc.myStorageArr.dsiElementDeinitializationComplete();
      delete desc;
    }
  }
}


/// iterators ///////////////////////////////////////////////////////////////


//== serial iterator - domain

iter DimensionalDom.these() {
  _traceddd(this, ".serial iterator");
  for ix in whole do
    yield ix;
}


//== leader iterator - domain

iter DimensionalDom.these(param tag: iterKind) where tag == iterKind.leader {
  _traceddd(this, ".leader");
  assert(rank == 2);

  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;

  // A hook for the Replicated distribution.
  // If this is not enough, consult the subordinate 1-d descriptors
  // instead of this and 'myDims = locDdesc.myStorageDom.dims()' below.
  proc helpTargetIds(dom1d, param dd) {
    if dom1d.dsiIsReplicated1d() {
      if !dom1d.dsiUsesLocalLocID1d() then
        compilerError("DimensionalDist2DImpl: currently, when a subordinate 1d distribution has dsiIsReplicated1d()==true, it also must have dsiUsesLocalLocID1d()==true");
      const (ix, legit) = dom1d.dsiGetLocalLocID1d();
      // we should only run this on target locales
      assert(legit);
      return ix..ix;
    } else
      return targetIds.dim(dd);
  }
  const overTargetIds = if dom1.dsiIsReplicated1d() || dom2.dsiIsReplicated1d()
    then {helpTargetIds(dom1,0), helpTargetIds(dom2,1)}
    else targetIds; // in this case, avoid re-building the domain

  // todo: lls is needed only for debugging printing?
  //   may be needed by the dimension specifiers (esp. replicated)?
  //
  //   Bug note: if we change coforall as follows:
  //     coforall ((l1,l2), locDdesc) in (targetIds, localDdescs) do
  //   presently it will crash the compiler on an assertion.
  //
  coforall (lls, locDdesc) in zip(overTargetIds, localDdescs[overTargetIds]) do
    on locDdesc {
      // mimic BlockDom leader, except computing parDim is more involved here

      // Note: we consult myStorageDom (via myDims), not the 1-d descriptors,
      // to learn how many indices each dimension has.
      const myDims = locDdesc.myStorageDom.dims();
      assert(rank == 2); // relied upon in a few places below

      // when we know which dimension should be the parallel one
      proc compute1dNTPD(param parDim): (int,int) {
        const myNumIndices = myDims(0).sizeAs(int) * myDims(1).sizeAs(int);
        const cnc:int =
          _computeNumChunks(maxTasks, ignoreRunning, minSize, myNumIndices);
        return ( min(cnc, myDims(parDim).sizeAs(int)), parDim );
      }

      const (numTasks, parDim) =
        if fakeDimensionalDistParDim >= 0 then
          // a debugging hook
          compute1dNTPD(fakeDimensionalDistParDim)
        else
          // For now, handle all four combinations explicitly.
          // Will need to generalize this to arbitrary no. of dimensions.
          //
          if dom1.dsiSingleTaskPerLocaleOnly1d() then
            if dom2.dsiSingleTaskPerLocaleOnly1d() then
              (1, 1)
            else
              compute1dNTPD(1)
          else
            if dom2.dsiSingleTaskPerLocaleOnly1d() then
              compute1dNTPD(0)
            else
              _computeChunkStuff(maxTasks, ignoreRunning, minSize, myDims);


      // parDim gotta point to one of the dimensions that we have
      assert(numTasks == 0 || (0 <= parDim && parDim < rank));

      // parDim cannot point to a single-task-only dimension
      assert(numTasks <= 1 ||
             ( (parDim !=0 || !dom1.dsiSingleTaskPerLocaleOnly1d()) &&
               (parDim !=1 || !dom2.dsiSingleTaskPerLocaleOnly1d()) ));

      if numTasks == 0 then
        _traceddc(traceDimensionalDist || traceDimensionalDistIterators,
                  "  leader on ", here.id, " ", lls, " - no tasks");

      const followDummy: (locDdesc.doml1.dsiMyDensifiedRangeType1d(dom1),
                          locDdesc.doml2.dsiMyDensifiedRangeType1d(dom2));
      type followT = followDummy.type;

      if numTasks == 1 then {

        // TODO: pass locId to dsiMyDensifiedRangeType1d,
        // then ilocdom will not need to keep it locally!!!
        for r1 in locDdesc.doml1.dsiMyDensifiedRangeForSingleTask1d(dom1) do
          for r2 in locDdesc.doml2.dsiMyDensifiedRangeForSingleTask1d(dom2) do
          {
            const follow: followT = (r1, r2);
            _traceddc(traceDimensionalDistIterators,
                      "  leader on ", lls, " single task -> ", follow);
            yield follow;
          }

      } else {
        coforall taskid in 0..#numTasks {

          // For a dimension other than 'parDim', all yields should
          // produce, collectively, all the indices in this dimension.
          // For 'parDim' - only the 'taskid'-th share of all indices.
          //
          iter iter1d(param dd, dom1d, loc1d) {
            const dummy: followT;
            type resultT = dummy(dd).type;
            if !dom1d.dsiSingleTaskPerLocaleOnly1d() && dd == parDim {
              // no iterators here, so far
              yield loc1d.dsiMyDensifiedRangeForTaskID1d
                (dom1d, taskid, numTasks) : resultT;
            } else {
              foreach r in loc1d.dsiMyDensifiedRangeForSingleTask1d(dom1d) do
                yield r: resultT;
            }
          }

          // Bug note: computing 'myDims(dd)' instead of passing 'myDim'
          // would trip an assertion in the compiler.
          iter iter1dCheck(param dd, dom1d, loc1d, myDim) {
            foreach myPiece in iter1d(dd, dom1d, loc1d) {

              // ensure we got a subset, if applicable
              if dom1d.dsiStorageUsesUserIndices() then
                assert(densify(myDim, whole.dim(dd))(myPiece) == myPiece);

              // Similar to the assert 'lo <= hi' in BlockDom leader.
              // Upon a second thought, if there is a legitimate reason
              // why dsiMyDensifiedRangeForTaskID1d() does not agree
              // with _computeChunkStuff (i.e. the latter returns more tasks
              // than the former wants to use) - fine. Then, replace assert with
              //   if myPiece.size == 0 then do not yield anything
// TODO: can it be enabled for test_strided_slice1.chpl with 1d block-cyclic?
//              assert(myPiece.size > 0);
              if myPiece.sizeAs(uint) > 0 then
                yield myPiece;
            }
          }

          for r1 in iter1dCheck(0, dom1, locDdesc.doml1, myDims(0)) do
            for r2 in iter1dCheck(1, dom2, locDdesc.doml2, myDims(1)) do
            {
              const follow: followT = (r1, r2);
              _traceddc(traceDimensionalDistIterators, "  leader on ", lls,
                        " task ", taskid, "/", numTasks, " -> ", follow);

              yield follow;
            }
        }
      } // if numTasks
    } // coforall ... on locDdesc

} // leader iterator - domain


//== follower iterator - domain

iter DimensionalDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  _traceddd(this, ".follower on ", here.id, "  got ", followThis);

  // This is pre-defined by DSI, so no need to consult
  // the dimension specifiers.

  for i in {(...unDensify(followThis, whole.dims()))} do
    yield i;
}


//== serial iterator - array

// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() ref {
  _traceddd(this, ".serial iterator",
            if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  if this.isAlias {
     // Go to the original array and invoke the follower iterator on it,
     // giving the alias's entire domain as the index set to follow.
     // (NB dsiFollowerArrayIterator1d's argument is not densified.)
    foreach v in this._dsiIteratorHelper(this.allocDom, this.dom.whole.dims()) do
      yield v;

    return;
  }

  // Could invoke the follower or _dsiIteratorHelper in this case, too,
  // but maybe invoking dsiSerialArrayIterator1d() is more efficient?

  const alDom = this.dom;

  // Cache the set of tuples for the inner loop.
  // todo: Make this conditional on the size of the outer loop,
  // to reduce overhead in some cases?
  //
  const dim2tuples = alDom.dom2.dsiSerialArrayIterator1d();

  // TODO: is this the right approach (ditto for 2nd dimension)?
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  // TODO: cf. the follower iterator has 1 fewer loop nest per dimension.
  // Which one do we want?
  for (l1,r1) in alDom.dom1.dsiSerialArrayIterator1d() do
    for i1 in r1 do
      for (l2,r2) in dim2tuples do
        {
          // Could cache locAdesc like for the follower iterator,
          // but that's less needed here.
          const locAdesc = this.localAdescs[l1,l2];
          _traceddc(traceDimensionalDistIterators,
                    "  locAdesc", (l1,l2), " on ", locAdesc.locale);
          foreach i2 in r2 do
            yield locAdesc.myStorageArr(i1, i2);
        }
}


//== leader iterator - array

iter DimensionalArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}


//== follower iterator - array   (somewhat similar to the serial iterator)

iter DimensionalArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  _traceddd(this, ".follower on ", here.id, "  got ", followThis,
            if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  // Convert the followThis ranges to user index space.
  const f1 = unDensify(followThis(0), this.dom.whole.dim(0));
  const f2 = unDensify(followThis(1), this.dom.whole.dim(1));

  // If this is an alias, we will invoke dsiFollowerArrayIterator1d
  // on the original array's domain descriptors.
  const alDom = if this.mustbeAlias || this.isAlias
                then this.allocDom
                else this.dom;

  for v in this._dsiIteratorHelper(alDom, (f1, f2)) do
    yield v;
}

// factor our some common code
iter DimensionalArr._dsiIteratorHelper(alDom, (f1, f2)) ref {
  // single-element cache of localAdescs[l1,l2]
  var lastl1 = invalidLocID, lastl2 = invalidLocID;
  var lastLocAdesc: this.localAdescs.eltType?;

  // TODO: is this the right approach? (similar to serial)
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  //
  // TODO: can we avoid calling the inner dsiFollowerArrayIterator1d()
  // more than once? Can we create a more efficient interface?
  //
  for (l1,i1) in alDom.dom1.dsiFollowerArrayIterator1d(f1) do
    for (l2,i2) in alDom.dom2.dsiFollowerArrayIterator1d(f2) do
      {
        // reuse the cache or index into the array?
        if l1 != lastl1 || l2 != lastl2 {
          lastl1 = l1;
          lastl2 = l2;
          lastLocAdesc = this.localAdescs[l1,l2];
          _traceddc(traceDimensionalDistIterators,
                    "  locAdesc", (l1,l2), " on ", lastLocAdesc.locale);
        }

//writeln("DimensionalArr follower on ", here.id, "  l=(", l1, ",", l2, ")  i=(", i1, ",", i2, ")");

        yield lastLocAdesc!.myStorageArr(i1, i2);
      }
}

} // DimensionalDist2DImpl
