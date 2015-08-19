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
// A 2D dimensional distribution.
//

// TODO
//
// * Implement reindexing and rank change directly within Dimensional.
//  That will probably be more efficient than going through WrapperDist.
//  For that, will need allow Dimensional to have smaller rank.
//  Can use "reindexing" dimension specifiers - will only
//  need two - one for replicated and one for any non-replicated.
// * Ensure that reallocation works with block-cyclic 1-d distribution
//  when the domain's stride changes.

//use DSIUtil;
//use WrapperDist;

// debugging/trace certain DSI methods as they are being invoked
config param traceDimensionalDist = false;
config param traceDimensionalDistDsiAccess = false;
config param traceDimensionalDistIterators = false;
config param fakeDimensionalDistParDim = 0;

// so user-specified phases can be retained while sorting verbose output
var traceDimensionalDistPrefix = "";

// private helpers ("trace DimensionalDist" Conditionally)
inline proc _traceddc(param condition: bool, args...)
{
  if condition then writeln(traceDimensionalDistPrefix,(...args));
}
inline proc _traceddc(param cond, d:DimensionalDist2D, args...)
{ _traceddc(cond, "DimensionalDist2D(", d.name, ")", (...args)); }
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

class DimensionalDist2D : BaseDist {
  // the desired locales to distribute things over;
  // must be a [domain(2, locIdT, false)] locale
  const targetLocales;
  // "IDs" are indices into targetLocales
  proc targetIds return targetLocales.domain;

  // the dimension specifiers - ones being combined
  const di1, di2;

  // for debugging/tracing (remove later)
  var name: string;

  // the domains' idxType that we support
  type idxType = int;
  // the type of the corresponding domain/array indices
  // todo: it does not really apply to dsiIndexToLocale()
  proc indexT type  return if rank == 1 then idxType else rank * idxType;

  // the count and size of each dimension of targetLocales
  // implementation note: 'rank' is not a real param; it's just that having
  // 'proc rank param return targetLocales.rank' did not work
  param rank: int = targetLocales.rank;
  proc numLocs1: locCntT  return targetIds.dim(1).length: locCntT;
  proc numLocs2: locCntT  return targetIds.dim(2).length: locCntT;

  // parallelization knobs
  var dataParTasksPerLocale: int      = getDataParTasksPerLocale();
  var dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks();
  var dataParMinGranularity: int      = getDataParMinGranularity();

  // for privatization
  var pid: int = -1;
}

// class LocDimensionalDist - no local distribution descriptor - for now

class DimensionalDom : BaseRectangularDom {
  // required
  param rank: int;
  type idxType;
  param stridable: bool;
  const dist; // not reprivatized

  // convenience
  proc targetIds return localDdescs.domain;
  proc rangeT  type  return range(idxType, BoundedRangeType.bounded, stridable);
  proc domainT type  return domain(rank, idxType, stridable);
  proc indexT  type  return dist.indexT;

  // subordinate 1-d global domain descriptors
  const dom1, dom2; // not reprivatized

  // This is our index set; we store it here so we can get to it easily.
  // Although strictly speaking it is not necessary.
  var whole: domainT;

  // This is the idxType of the "storage index ranges" to be produced
  // by dsiSetLocalIndices1d(). It needs to be uniform across dimensions,
  // and 'idxType' is the easiest choice (although not the most general).
  // NB it's also computed in DimensionalDist2D.dsiNewRectangularDom().
  proc stoIndexT type  return idxType;

  // helper for locDdescType: any of storage index ranges can be stridable
  proc stoStridable param {
    proc stoStridable1d(dom1d) param {
      const dummy = dom1d.dsiNewLocalDom1d(stoIndexT, 0:locIdT)
        .dsiSetLocalIndices1d(dom1d, 0:locIdT);
      return dummy.stridable;
    }
    return stoStridable1d(dom1) || stoStridable1d(dom2);
  }

  proc stoRangeT type  return range(stoIndexT, stridable = stoStridable);
  proc stoDomainT type  return domain(rank, stoIndexT, stoStridable);

  // convenience - our instantiation of LocDimensionalDom
  proc locDdescType type  return LocDimensionalDom(stoDomainT,
                                         dom1.dsiNewLocalDom1d(stoIndexT, 0:locIdT).type,
                                         dom2.dsiNewLocalDom1d(stoIndexT, 0:locIdT).type);

  // local domain descriptors
  var localDdescs: [dist.targetIds] locDdescType; // not reprivatized

  // for privatization
  var pid: int = -1;
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

class DimensionalArr : BaseArr {
  // required
  type eltType;
  const dom; // must be a DimensionalDom

  // same as 'dom'; for an alias (e.g. a slice), 'dom' of the original array
  const allocDom; // must be a DimensionalDom

  proc rank param return dom.rank;
  proc targetIds return localAdescs.domain;

  // no subordinate 1-d array descriptors - we handle storage ourselves

  // local array descriptors (from the original array, if this is an alias)
  // NOTE: 'dom' must be initialized prior to initializing 'localAdescs'
  var localAdescs: [dom.targetIds]
                      LocDimensionalArr(eltType, allocDom.locDdescType);

  // for privatization
  var pid: int = -1;
}

class LocDimensionalArr {
  type eltType;
  const locDom;  // a LocDimensionalDom
  var myStorageArr: [locDom.myStorageDom] eltType;
}


/// distribution ////////////////////////////////////////////////////////////


//== construction, cloning

// constructor
// gotta list all the things we let the user set
proc DimensionalDist2D.DimensionalDist2D(
  targetLocales: [] locale,
  di1,
  di2,
  name: string = "dimensional distribution",
  type idxType = int,
  dataParTasksPerLocale: int      = getDataParTasksPerLocale(),
  dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks(),
  dataParMinGranularity: int      = getDataParMinGranularity()
) {
  this.name = name;
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;
  checkInvariants();

  _passLocalLocIDsDist(di1, true, di2, true,
                     this.targetLocales, true, this.targetLocales.domain.low);
}

//
// Having targetLocales be a constant means we have to reshape Locales
// before invoking the constructor. This method does that.
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
  var reshapedLocales => reshape(targetLocales[0..#nl1*nl2],{0..#nl1,0..#nl2});

  return new DimensionalDist2D(reshapedLocales, di1, di2, name, idxType,
   dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}


// Check all restrictions/assumptions that must be satisfied by the user
// when constructing a DimensionalDist2D.
proc DimensionalDist2D.checkInvariants(): void {
  proc ensure(param cond:bool, param msg:c_string) {
    if !cond then compilerError(msg, 3);
  }
  ensure(targetLocales.rank == 2, "DimensionalDist2D requires 'targetLocales' to be a 2-dimensional array, got " + targetLocales.rank:c_string + " dimension(s)");
  ensure(rank == targetLocales.rank, "DimensionalDist2D bug: inconsistent rank");
  ensure(targetLocales.eltType == locale, "DimensionalDist2D requires 'targetLocales' to be an array of locales, got an array of " + typeToString(targetLocales.eltType));
  ensure(targetIds.idxType == locIdT, "DimensionalDist2D currently requires 'idxType' of 'targetLocales.domain' to be " + typeToString(locIdT) + ", got " + typeToString(targetIds.idxType));

  assert(targetIds == {0..#numLocs1, 0..#numLocs2}, "DimensionalDist2D-targetIds");
  assert(di1.numLocales == numLocs1, "DimensionalDist2D-numLocales-1");
  assert(di2.numLocales == numLocs2, "DimensionalDist2D-numLocales-2");
  assert(dataParTasksPerLocale > 0, "DimensionalDist2D-dataParTasksPerLocale");
  assert(dataParMinGranularity > 0, "DimensionalDist2D-dataParMinGranularity");
}

proc DimensionalDist2D.dsiClone(): this.type {
  //_traceddd("DimensionalDist2D.dsiClone");
  checkInvariants();

  // do this simple thing, until we find out that we need something else
  return this;
}


//== privatization

proc DimensionalDist2D.dsiSupportsPrivatization() param return true;

proc DimensionalDist2D.dsiGetPrivatizeData() {
  //_traceddd(this, ".dsiGetPrivatizeData");

  const di1pd = if di1.dsiSupportsPrivatization1d()
    then di1.dsiGetPrivatizeData1d()
    else 0;
  const di2pd = if di2.dsiSupportsPrivatization1d()
    then di2.dsiGetPrivatizeData1d()
    else 0;

  return (targetLocales, dataParTasksPerLocale,
          dataParIgnoreRunningTasks, dataParMinGranularity,
          di1, di1pd, di2, di2pd);
}

proc DimensionalDist2D.dsiPrivatize(privatizeData) {
  //_traceddd(this, ".dsiPrivatize on ", here.id);

  // ensure we get a local copy of targetLocales
  // todo - provide the following as utilty functions (for domains, arrays)
  const pdTargetLocales = privatizeData(1);
  const privTargetIds: domain(pdTargetLocales.domain.rank,
                              pdTargetLocales.domain.idxType,
                              pdTargetLocales.domain.stridable
                              ) = pdTargetLocales.domain;
  const privTargetLocales: [privTargetIds] locale = pdTargetLocales;

  proc di1orig return privatizeData(5);
  proc di1pd   return privatizeData(6);
  const di1new = if di1.dsiSupportsPrivatization1d()
    then di1orig.dsiPrivatize1d(di1pd) else di1orig;

  proc di2orig  return privatizeData(7);
  proc di2pd    return privatizeData(8);
  const di2new = if di2.dsiSupportsPrivatization1d()
    then di2orig.dsiPrivatize1d(di2pd) else di2orig;

  const plliddDummy: privTargetLocales.domain.low.type;
  _passLocalLocIDsDist(di1new, di1.dsiSupportsPrivatization1d(),
                       di2new, di2.dsiSupportsPrivatization1d(),
                       privTargetLocales, false, plliddDummy);

  return new DimensionalDist2D(targetLocales = privTargetLocales,
                             name          = "",
                             idxType       = this.idxType,
                             di1           = di1new,
                             di2           = di2new,
                             dataParTasksPerLocale     = privatizeData(2),
                             dataParIgnoreRunningTasks = privatizeData(3),
                             dataParMinGranularity     = privatizeData(4),
                             dummy = 0);
}

// constructor of a privatized copy
// ('dummy' distinguishes it from the user constructor)
proc DimensionalDist2D.DimensionalDist2D(param dummy: int,
  targetLocales: [] locale,
  name,
  type idxType,
  di1,
  di2,
  dataParTasksPerLocale,
  dataParIgnoreRunningTasks,
  dataParMinGranularity
) {
  this.name = name;
  this.dataParTasksPerLocale     = dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity     = dataParMinGranularity;
  // should not need it, but run it for now just in case
  checkInvariants();
}


//== miscellanea

proc DimensionalDist2D.dimSpecifier(param dim: int) {
  if dim == 1 then
    return di1;
  else if dim == 2 then
    return di2;
  else
    compilerError("DimensionalDist2D presently supports dimSpecifier()",
                  " only for dimension 1 or 2, got dim=", dim:c_string);
}


//== index to locale

//Given an index, this should return the locale that owns that index.
proc DimensionalDist2D.dsiIndexToLocale(indexx: indexT): locale {
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist2D presently supports only indexing with",
                  " 2-tuples; got an index of the type ",
                  typeToString(indexx.type));

  return targetLocales(di1.dsiIndexToLocale1d(indexx(1)):int,
                       di2.dsiIndexToLocale1d(indexx(2)):int);
}

pragma "auto copy fn"
proc chpl__autoCopy(x: DimensionalDist2D) {
  if ! noRefCount && ! _isPrivatized(x) then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: DimensionalDist2D) {
  if !noRefCount && ! _isPrivatized(x) {
    var cnt = x.destroyDist();
    if cnt == 0 then
      delete x;
  }
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
proc _CurrentLocaleToLocIDs(targetLocales): (targetLocales.rank*locIdT, bool)
{
  var result: targetLocales.rank * locIdT;
  // guard updates to 'result' to ensure atomicity of updates
  var gotresult$: sync bool = false;
  forall (lls, loc) in zip(targetLocales.domain, targetLocales) with (ref result) do
    if loc == here {
      // if we get multiple matches, we do not specify which is returned
      // could add a pre-test if it were cheap: if !gotresult$.readXX()
      gotresult$;
      result = lls;
      gotresult$ = true;
    }
  // instead of crashing right away, return a flag
  //if !gotresult$.readXX() then halt("DimensionalDist2D: the current locale ", here, " is not among the target locales ", targetLocales);

  return (result, gotresult$.readXX());
}

// How we usually invoke _CurrentLocaleToLocIDs().
proc _passLocalLocIDsDist(d1, doD1:bool, d2, doD2:bool,
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
       _CurrentLocaleToLocIDs(targetLocales);

   if d1.dsiUsesLocalLocID1d() && doD1 then d1.dsiStoreLocalLocID1d(lIds(1),l);
   if d2.dsiUsesLocalLocID1d() && doD2 then d2.dsiStoreLocalLocID1d(lIds(2),l);
  }
 }
}

// Subordinate 1-d domains copy the local locId from their distributions.
proc _passLocalLocIDsDom1d(dom1d, dist1d) {
  if dom1d.dsiUsesLocalLocID1d() {

    // ensure dist1d.dsiGetLocalLocID1d() is available
    if !dist1d.dsiUsesLocalLocID1d() then compilerError("DimensionalDist2D: currently, when a subordinate 1d distribution requires localLocID for *domain* descriptors, it must also require them for *distribution* descriptors");

    // otherwise there is a mismatch: the local locID is copied
    // from a non-privatized object to a privatized one - or visa versa
    if dom1d.dsiSupportsPrivatization1d() != dist1d.dsiSupportsPrivatization1d() then compilerError("DimensionalDist2D: currently, when a subordinate 1d distribution requires localLocID for domain descriptors, it must support privatization for *domain* descriptors if and only if it supports privatization for *distribution* descriptors");

    dom1d.dsiStoreLocalLocID1d(dist1d.dsiGetLocalLocID1d());
  }
}


/// domain //////////////////////////////////////////////////////////////////


pragma "auto copy fn"
proc chpl__autoCopy(x: DimensionalDom) {
  if ! noRefCount && ! _isPrivatized(x) then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: DimensionalDom) {
  if !noRefCount && ! _isPrivatized(x) {
    var cnt = x.destroyDom();
    if cnt == 0 then
      delete x;
  }
}

//== privatization

proc DimensionalDom.dsiSupportsPrivatization() param return true;

proc DimensionalDom.dsiGetPrivatizeData() {
  //_traceddd(this, ".dsiGetPrivatizeData");

  const dom1pd = if dom1.dsiSupportsPrivatization1d()
    then dom1.dsiGetPrivatizeData1d()
    else 0;
  const dom2pd = if dom2.dsiSupportsPrivatization1d()
    then dom2.dsiGetPrivatizeData1d()
    else 0;

  return (dist.pid, dom1, dom1pd, dom2, dom2pd, whole.dims(), localDdescs);
}

proc DimensionalDom.dsiPrivatize(privatizeData) {
  //_traceddd(this, ".dsiPrivatize on ", here.id);

  var privdist = chpl_getPrivatizedCopy(objectType = this.dist.type,
                                        objectPid  = privatizeData(1));

  proc dom1orig  return privatizeData(2);
  proc dom1pd    return privatizeData(3);
  const dom1new = if dom1orig.dsiSupportsPrivatization1d()
    then dom1orig.dsiPrivatize1d(privdist.di1, dom1pd) else dom1orig;

  if dom1orig.dsiSupportsPrivatization1d() then
    _passLocalLocIDsDom1d(dom1new, privdist.di1);

  proc dom2orig  return privatizeData(4);
  proc dom2pd    return privatizeData(5);
  const dom2new = if dom2orig.dsiSupportsPrivatization1d()
    then dom2orig.dsiPrivatize1d(privdist.di2, dom2pd) else dom2orig;

  if dom2orig.dsiSupportsPrivatization1d() then
    _passLocalLocIDsDom1d(dom2new, privdist.di2);

  const result = new DimensionalDom(rank      = this.rank,
                                    idxType   = this.idxType,
                                    stridable = this.stridable,
                                    dist = privdist,
                                    dom1 = dom1new,
                                    dom2 = dom2new,
                                    whole       = {(...privatizeData(6))},
                                    localDdescs = privatizeData(7));

  // update local-to-global pointers as needed
  param lg1 = dom1orig.dsiSupportsPrivatization1d() &&
              dom1orig.dsiLocalDescUsesPrivatizedGlobalDesc1d();
  param lg2 = dom2orig.dsiSupportsPrivatization1d() &&
              dom2orig.dsiLocalDescUsesPrivatizedGlobalDesc1d();
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
  //_traceddd(this, ".dsiGetReprivatizeData");

  const dom1rpd = if dom1.dsiSupportsPrivatization1d()
    then dom1.dsiGetReprivatizeData1d()
    else 0;
  const dom2rpd = if dom2.dsiSupportsPrivatization1d()
    then dom2.dsiGetReprivatizeData1d()
    else 0;

  return (dom1, dom1rpd, dom2, dom2rpd, whole);
}

proc DimensionalDom.dsiReprivatize(other, reprivatizeData) {
  //_traceddd(this, ".dsiReprivatize on ", here.id);

  compilerAssert(this.rank == other.rank &&
                 this.idxType == other.idxType &&
                 this.stridable == other.stridable);

  if dom1.dsiSupportsPrivatization1d() then
    dom1.dsiReprivatize1d(other           = reprivatizeData(1),
                          reprivatizeData = reprivatizeData(2));
  if dom2.dsiSupportsPrivatization1d() then
    dom2.dsiReprivatize1d(other           = reprivatizeData(3),
                          reprivatizeData = reprivatizeData(4));
  this.whole = reprivatizeData(5);
}


//== miscellanea

proc DimensionalDom.dsiMyDist() return dist;

proc DimensionalDom.dsiDims()             return whole.dims();
proc DimensionalDom.dsiDim(d)             return whole.dim(d);
proc DimensionalDom.dsiDim(param d)       return whole.dim(d);
proc DimensionalDom.dsiLow                return whole.low;
proc DimensionalDom.dsiHigh               return whole.high;
proc DimensionalDom.dsiStride             return whole.stride;
proc DimensionalDom.dsiNumIndices         return whole.numIndices;
proc DimensionalDom.dsiMember(indexx)     return whole.member(indexx);
proc DimensionalDom.dsiIndexOrder(indexx) return whole.indexOrder(indexx);

proc DimensionalDom.dimSpecifier(param dim: int) {
  return dist.dimSpecifier(dim);
}


//== writing

proc DimensionalDom.dsiSerialWrite(f: Writer): void {
  f.write(whole);
}


//== creation, SetIndices

// create a new domain mapped with this distribution
proc DimensionalDist2D.dsiNewRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool)
//  : DimensionalDom(rank, idxType, stridable, this.type, ...)
{
  //_traceddd(this, ".dsiNewRectangularDom ",
  //         (rank, typeToString(idxType), stridable));
  if rank != 2 then
    compilerError("DimensionalDist2D presently supports only 2 dimensions,",
                  " got ", rank:c_string, " dimensions");

  // todo: ideally, this will not be required;
  // furthermore, DimensionalDist2D shouldn't be specific to idxType.
  if idxType != this.idxType then
    compilerError("The domain index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the DimensionalDist2D used to map that domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDist2D used to map that domain");

  // need this for dsiNewRectangularDom1d()
  type stoIndexT = this.idxType;

  const dom1 = di1.dsiNewRectangularDom1d(idxType, stridable, stoIndexT);
  _passLocalLocIDsDom1d(dom1, di1);

  const dom2 = di2.dsiNewRectangularDom1d(idxType, stridable, stoIndexT);
  _passLocalLocIDsDom1d(dom2, di2);

  const result = new DimensionalDom(rank=rank, idxType=idxType,
                                    stridable=stridable, dist=this,
                                    dom1 = dom1, dom2 = dom2);
  // result.whole is initialized to the default value (empty domain)

  if stoIndexT != result.stoIndexT then
    compilerError("bug in DimensionalDist2D: inconsistent stoIndexT");

  coforall (loc, locIds, locDdesc)
   in zip(targetLocales, targetIds, result.localDdescs) do
    on loc do
      locDdesc = new LocDimensionalDom(result.stoDomainT,
                       doml1 = dom1.dsiNewLocalDom1d(stoIndexT, locIds(1)),
                       doml2 = dom2.dsiNewLocalDom1d(stoIndexT, locIds(2)));
  return result;
}

proc DimensionalDom.dsiSetIndices(newIndices: domainT): void {
  whole = newIndices;
  _dsiSetIndicesHelper(newIndices.dims());
}

proc DimensionalDom.dsiSetIndices(newRanges: rank * rangeT): void {
  whole = {(...newRanges)};
  _dsiSetIndicesHelper(newRanges);
}

// not part of DSI
proc DimensionalDom._dsiSetIndicesHelper(newRanges: rank * rangeT): void {
  //_traceddd(this, ".dsiSetIndices", newRanges);
  if rank != 2 then
    compilerError("DimensionalDist2D presently supports only 2 dimensions,",
                  " got a domain with ", rank, " dimensions");

  dom1.dsiSetIndices1d(newRanges(1));
  dom2.dsiSetIndices1d(newRanges(2));

  // could omit this warning if the intersection between the old and the new
  // domains is empty; could change it to halt("unimplemented")
  if dom1.dsiSetIndicesUnimplementedCase||dom2.dsiSetIndicesUnimplementedCase
    then if _arrs.length > 0 then
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
  var myRange1: stoRangeT = doml1.dsiSetLocalIndices1d(globDD(1),locId(1));
  var myRange2: stoRangeT = doml2.dsiSetLocalIndices1d(globDD(2),locId(2));

  myStorageDom = {myRange1, myRange2};

  //_traceddd("DimensionalDom.dsiSetIndices on ", here.id, " ", locId,
  //          "  storage ", myStorageDom);
}

proc DimensionalDom.dsiGetIndices(): rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                 stridable) {
  //_traceddd(this, ".dsiGetIndices");
  return whole.dims();
}

// create a new domain mapped with this's distribution + initialize to 'ranges'
//
// dsiBuildRectangularDom   = dsiNewRectangularDom   + dsiSetIndices
// dsiBuildRectangularDom1d = dsiNewRectangularDom1d + dsiSetIndices1d
// dsiBuildLocalDom1d       = dsiNewLocalDom1d       + dsiSetLocalIndices1d
//
proc DimensionalDom.dsiBuildRectangularDom(param rank: int,
                                           type idxType,
                                           param stridable: bool,
                                           ranges: rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                                stridable))
{
  //_traceddd(this, ".dsiBuildRectangularDom ",
  //         (rank, typeToString(idxType), stridable), ranges);
  if rank != 2 then
    compilerError("DimensionalDist2D presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  // todo: ideally, this will not be required;
  // furthermore, DimensionalDist2D shouldn't be specific to idxType.
  if idxType != this.idxType then
    compilerError("The domain index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the DimensionalDom used to create this domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDom used to create this domain");

  const dist = this.dist;

  const dom1 = this.dom1.dsiBuildRectangularDom1d(dist.di1,
                                                  stridable, ranges(1));
  _passLocalLocIDsDom1d(dom1, dist.di1);

  const dom2 = this.dom2.dsiBuildRectangularDom1d(dist.di2,
                                                  stridable, ranges(2));
  _passLocalLocIDsDom1d(dom2, dist.di2);

  const result = new DimensionalDom(rank=rank, idxType=idxType,
                                    stridable=stridable, dist=dist,
                                    dom1 = dom1, dom2 = dom2,
                                    whole = {(...ranges)});

  // Not including 'targetLocales' in zippering for now -
  // obtain the locale/locId from 'this' and its components instead.
  // (Is that more efficient?)
  coforall (oldLocDdesc, newLocDdesc, locIds)
   in zip(this.localDdescs, result.localDdescs, this.targetIds) do
    on oldLocDdesc {
      var (doml1, myRange1) = oldLocDdesc.doml1.dsiBuildLocalDom1d(dom1, locIds(1));
      var (doml2, myRange2) = oldLocDdesc.doml2.dsiBuildLocalDom1d(dom2, locIds(2));

      newLocDdesc = new LocDimensionalDom(result.stoDomainT,
                                          myStorageDom = {myRange1, myRange2},
                                          doml1 = doml1, doml2 = doml2);
    }

  return result;
}


/// array ///////////////////////////////////////////////////////////////////


pragma "auto copy fn"
proc chpl__autoCopy(x: DimensionalArr) {
  if !noRefCount && ! _isPrivatized(x) then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: DimensionalArr) {
  if !noRefCount && ! _isPrivatized(x) {
    var cnt = x.destroyArr();
    if cnt == 0 then
      delete x;
  }
}

//== privatization

proc DimensionalArr.dsiSupportsPrivatization() param return true;

proc DimensionalArr.dsiGetPrivatizeData() {
  //_traceddd(this, ".dsiGetPrivatizeData");

  return (dom.pid, allocDom.pid, localAdescs);
}

proc DimensionalArr.dsiPrivatize(privatizeData) {
  //_traceddd(this, ".dsiPrivatize on ", here.id);

  const idDom = privatizeData(1);
  const idAllocDom = privatizeData(2);

  const privDom = chpl_getPrivatizedCopy(objectType = this.dom.type,
                                         objectPid  = idDom);

  const privAllocDom =
    if !this.mustbeAlias && idAllocDom == idDom
    then privDom
    else chpl_getPrivatizedCopy(objectType = this.allocDom.type,
                                objectPid  = idAllocDom);

  const result = new DimensionalArr(eltType  = this.eltType,
                                    dom      = privDom,
                                    allocDom = privAllocDom);

  result.localAdescs = privatizeData(3);

  assert(result.isAlias == this.isAlias);
  return result;
}


//== miscellanea

proc DimensionalArr.idxType type return dom.idxType; // (could be a field)

proc DimensionalArr.dsiGetBaseDom() return dom;

proc DimensionalArr.dimSpecifier(param dim: int) {
  return dom.dimSpecifier(dim);
}

proc DimensionalArr.mustbeAlias param
  return this.dom.type != this.allocDom.type;

proc DimensionalArr.isAlias
  return this.dom != this.allocDom;


//== creation

// create a new array over this domain
proc DimensionalDom.dsiBuildArray(type eltType)
  : DimensionalArr(eltType, this.type, this.type)
{
  //_traceddd(this, ".dsiBuildArray");
  if rank != 2 then
    compilerError("DimensionalDist2D presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  const result = new DimensionalArr(eltType  = eltType,
                                    dom      = this,
                                    allocDom = this);
  coforall (loc, locDdesc, locAdesc)
   in zip(dist.targetLocales, localDdescs, result.localAdescs) do
    on loc do
      locAdesc = new LocDimensionalArr(eltType, locDdesc);

  assert(!result.isAlias);
  return result;
}


//== dsiAccess

proc DimensionalArr.dsiAccess(indexx: dom.indexT) ref: eltType {
  //_traceddc(traceDimensionalDist || traceDimensionalDistDsiAccess,
  //          this, ".dsiAccess", indexx);

  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist2D presently supports only indexing with",
                  " 2-tuples; got an array index of the type ",
                  typeToString(indexx.type));

  const alDom = this.allocDom;
  const (l1,i1):(locIdT, alDom.stoIndexT) = alDom.dom1.dsiAccess1d(indexx(1));
  const (l2,i2):(locIdT, alDom.stoIndexT) = alDom.dom2.dsiAccess1d(indexx(2));
  const locAdesc = this.localAdescs[l1,l2];
//writeln("locAdesc.myStorageArr on ", locAdesc.myStorageArr.locale.id);
  return locAdesc.myStorageArr(i1,i2);
}

//== writing

proc DimensionalArr.dsiSerialWrite(f: Writer): void {
  //_traceddd(this, ".dsiSerialWrite on ", here.id,
  //          if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  iter iHelp(param d) {
    if this.isAlias {
       // Go to the original array and invoke the follower iterator on it,
       // giving the alias's entire domain as the index set to follow.
       // (NB dsiFollowerArrayIterator1d's argument is not densified.)
      const dom1d = if d == 1 then this.allocDom.dom1 else this.allocDom.dom2;
      for l_i in dom1d.dsiFollowerArrayIterator1d(this.dom.whole.dim(d)) do
        yield l_i;

    } else {
      const alDom = this.dom;
      const dom1d = if d == 1 then alDom.dom1 else alDom.dom2;
      for (l,r) in dom1d.dsiSerialArrayIterator1d() do
        for i in r do
          yield (l,i);
    }
  }

  // we largely follow DimensionalArr.these()
  // instead could just use BlockArr.dsiSerialWrite
  if this.dom.dsiNumIndices == 0 then return;

  var nextD1 = false;
  for (l1,i1) in iHelp(1) {
      if nextD1 then f.writeln();
      nextD1 = true;

      var nextD2 = false;
      for (l2,i2) in iHelp(2) {
          const locAdesc = this.localAdescs[l1,l2];
          const elem = locAdesc.myStorageArr(i1,i2);
          if nextD2 then f.write(" ");
          f.write(elem);
          nextD2 = true;
        }
    }
}


/// slicing, reindexing, rank change, reallocation //////////////////////////

proc DimensionalArr.dsiSlice(sliceDef: DimensionalDom) {
  //_traceddd(this, ".dsiSlice of ", this.dom.whole, " with ", sliceDef.whole);

//writeln("slicing DimensionalArr ", this.dom.dsiDims(), "  with DimensionalDom ", sliceDef.dsiDims());

// started with dsiBuildArray, modified like ReplicatedArr.dsiSlice
  const slicee = this;
  if slicee.rank != sliceDef.rank then
    compilerError("slicing with a different rank");

  const result = new DimensionalArr(eltType  = slicee.eltType,
                                    dom      = sliceDef,
                                    allocDom = slicee.allocDom);

  // reuse the original array's local descriptors,
  // ensuring sliceDef and slicee are over the same set of locales/targetIds
  assert(sliceDef.localDdescs.domain == slicee.localAdescs.domain);
  result.localAdescs = slicee.localAdescs;

  assert(result.isAlias);
  return result;
}

proc DimensionalArr.dsiLocalSlice((sliceDim1, sliceDim2)) {
  const dom = this.dom;
  const dist = dom.dist;
  // todo: cache (l1, l2) in privatized copies when possible
  // (i.e. if privatization is supported and there is no oversubscription)
  // Assuming dsiLocalSlice is guaranteed to be local to 'here'.
  const l1 = dist.di1.dsiIndexToLocale1d(sliceDim1.low),
        l2 = dist.di2.dsiIndexToLocale1d(sliceDim2.low),
        locAdesc = this.localAdescs[l1, l2],
        r1 = if dom.dom1.dsiStorageUsesUserIndices()
             then dom.whole.dim(1)(sliceDim1)
             else locAdesc.locDom.doml1.dsiLocalSliceStorageIndices1d(dom.dom1, sliceDim1),
        r2 = if dom.dom2.dsiStorageUsesUserIndices()
             then dom.whole.dim(2)(sliceDim2)
             else locAdesc.locDom.doml2.dsiLocalSliceStorageIndices1d(dom.dom2, sliceDim2);

  return locAdesc.myStorageArr[r1, r2];
}

/* The following are using the standalone WrapperDist, currently not provided.

proc DimensionalDist2D.dsiCreateReindexDist(newSpace, oldSpace) {
  return genericDsiCreateReindexDist(this, this.rank, newSpace, oldSpace);
}

proc DimensionalArr.dsiReindex(reindexDef: DimensionalDom) {
  const reindexee = this;
  if reindexee.dom == reindexDef then
    return reindexee;

  halt("DimensionalArr.dsiReindex: unexpected invocation on ",
       typeToString(this.type), "  and  ", typeToString(reindexDef.type));
}

proc DimensionalArr.dsiReindex(reindexDef: WrapperRectDom) {
  return genericDsiReindex(this, reindexDef);
}

proc DimensionalDist2D.dsiCreateRankChangeDist(param newRank: int, args) {
  return genericDsiCreateRankChangeDist(this, newRank, args);
}

proc DimensionalArr.dsiRankChange(sliceDefDom: WrapperRectDom,
                                  param newRank: int,
                                  param newStridable: bool,
                                  sliceDefIndsRanges) {
  return genericDsiRankChange(this, sliceDefDom, newRank, newStridable,
                              sliceDefIndsRanges);
}

*/

proc DimensionalArr.dsiReallocate(d: domain) {
  // nothing
  // TODO: handle block-cyclic 1d when the stride changes
}

proc DimensionalArr.dsiPostReallocate() {
  // nothing for now
}


/// iterators ///////////////////////////////////////////////////////////////


//== serial iterator - domain

iter DimensionalDom.these() {
  //_traceddd(this, ".serial iterator");
  for ix in whole do
    yield ix;
}


//== leader iterator - domain

iter DimensionalDom.these(param tag: iterKind) where tag == iterKind.leader {
  //_traceddd(this, ".leader");
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
        compilerError("DimensionalDist2D: currently, when a subordinate 1d distribution has dsiIsReplicated1d()==true, it also must have dsiUsesLocalLocID1d()==true");
      const (ix, legit) = dom1d.dsiGetLocalLocID1d();
      // we should only run this on target locales
      assert(legit);
      return ix..ix;
    } else
      return targetIds.dim(dd);
  }
  const overTargetIds = if dom1.dsiIsReplicated1d() || dom2.dsiIsReplicated1d()
    then {helpTargetIds(dom1,1), helpTargetIds(dom2,2)}
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
        const myNumIndices = myDims(1).length * myDims(2).length;
        const cnc:int =
          _computeNumChunks(maxTasks, ignoreRunning, minSize, myNumIndices);
        return ( min(cnc, myDims(parDim).length:int), parDim );
      }

      const (numTasks, parDim) =
        if fakeDimensionalDistParDim > 0 then
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
              compute1dNTPD(2)
          else
            if dom2.dsiSingleTaskPerLocaleOnly1d() then
              compute1dNTPD(1)
            else
              _computeChunkStuff(maxTasks, ignoreRunning, minSize, myDims);


      // parDim gotta point to one of the dimensions that we have
      assert(numTasks == 0 || (1 <= parDim && parDim <= rank));

      // parDim cannot point to a single-task-only dimension
      assert(numTasks <= 1 ||
             ( (parDim !=1 || !dom1.dsiSingleTaskPerLocaleOnly1d()) &&
               (parDim !=2 || !dom2.dsiSingleTaskPerLocaleOnly1d()) ));

      //if numTasks == 0 then
      //  _traceddc(traceDimensionalDist || traceDimensionalDistIterators,
      //            "  leader on ", here.id, " ", lls, " - no tasks");

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
            //_traceddc(traceDimensionalDistIterators,
            //          "  leader on ", lls, " single task -> ", follow);
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
              for r in loc1d.dsiMyDensifiedRangeForSingleTask1d(dom1d) do
                yield r: resultT;
            }
          }

          // Bug note: computing 'myDims(dd)' instead of passing 'myDim'
          // would trip an assertion in the compiler.
          iter iter1dCheck(param dd, dom1d, loc1d, myDim) {
            for myPiece in iter1d(dd, dom1d, loc1d) {

              // ensure we got a subset, if applicable
              if dom1d.dsiStorageUsesUserIndices() then
                assert(densify(myDim, whole.dim(dd))(myPiece) == myPiece);

              // Similar to the assert 'lo <= hi' in BlockDom leader.
              // Upon a second thought, if there is a legitimate reason
              // why dsiMyDensifiedRangeForTaskID1d() does not agree
              // with _computeChunkStuff (i.e. the latter returns more tasks
              // than the former wants to use) - fine. Then, replace assert with
              //   if myPiece.length == 0 then do not yield anything
// TODO: can it be enabled for test_strided_slice1.chpl with 1d block-cyclic?
//              assert(myPiece.length > 0);
              if myPiece.length > 0 then
                yield myPiece;
            }
          }

          for r1 in iter1dCheck(1, dom1, locDdesc.doml1, myDims(1)) do
            for r2 in iter1dCheck(2, dom2, locDdesc.doml2, myDims(2)) do
            {
              const follow: followT = (r1, r2);
              //_traceddc(traceDimensionalDistIterators, "  leader on ", lls,
              //          " task ", taskid, "/", numTasks, " -> ", follow);

              yield follow;
            }
        }
      } // if numTasks
    } // coforall ... on locDdesc

} // leader iterator - domain


//== follower iterator - domain

iter DimensionalDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  //_traceddd(this, ".follower on ", here.id, "  got ", followThis);

  // This is pre-defined by DSI, so no need to consult
  // the dimension specifiers.

  for i in {(...unDensify(followThis, whole.dims()))} do
    yield i;
}


//== serial iterator - array

// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() ref {
  //_traceddd(this, ".serial iterator",
  //          if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  if this.isAlias {
     // Go to the original array and invoke the follower iterator on it,
     // giving the alias's entire domain as the index set to follow.
     // (NB dsiFollowerArrayIterator1d's argument is not densified.)
    for v in this._dsiIteratorHelper(this.allocDom, this.dom.whole.dims()) do
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
          //_traceddc(traceDimensionalDistIterators,
          //          "  locAdesc", (l1,l2), " on ", locAdesc.locale);
          for i2 in r2 do
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
  //_traceddd(this, ".follower on ", here.id, "  got ", followThis,
  //          if this.isAlias then "  (alias)" else "");
  assert(this.rank == 2);

  // Convert the followThis ranges to user index space.
  const f1 = unDensify(followThis(1), this.dom.whole.dim(1));
  const f2 = unDensify(followThis(2), this.dom.whole.dim(2));

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
  var lastLocAdesc: this.localAdescs.eltType;

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
          //_traceddc(traceDimensionalDistIterators,
          //          "  locAdesc", (l1,l2), " on ", lastLocAdesc.locale);
        }

//writeln("DimensionalArr follower on ", here.id, "  l=(", l1, ",", l2, ")  i=(", i1, ",", i2, ")");

        yield lastLocAdesc.myStorageArr(i1, i2);
      }
}
