//// d.chpl

// This is a two-dimensional Dimensional distribution

//use DSIUtil;

// debugging/trace certain DSI methods as they are being invoked
config param traceDimensionalDist = false;
config param traceDimensionalDistDsiAccess = false;
config param traceDimensionalDistIterators = false;
config param fakeDimensionalDistParDim = 0;

// so user-specified phases can be retained while sorting verbose output
var traceDimensionalDistPrefix = "";

// private helpers ("trace DimensionalDist" Conditionally)
pragma "inline" proc _traceddc(param condition: bool, args...)
{
  if condition then writeln(traceDimensionalDistPrefix,(...args));
}
pragma "inline" proc _traceddc(param cond, d:DimensionalDist, args...)
{ _traceddc(cond, "DimensionalDist(", d.name, ")", (...args)); }
pragma "inline" proc _traceddc(param cond, d:DimensionalDom, args...)
{ _traceddc(cond, "DimensionalDom(", d.dist.name, ")", (...args)); }
pragma "inline" proc _traceddc(param cond, d:DimensionalArr, args...)
{ _traceddc(cond, "DimensionalArr(", d.dom.dist.name, ")", (...args)); }
// the Default condition
pragma "inline" proc _traceddd(args...)
{ _traceddc(traceDimensionalDist, (...args)); }


/// the type for ... ////////////////////////////////////////////////////////

// ... locale counts
type locCntT = uint(32);

// ... locale ID, i.e., its index in targetLocales in the given dimension
// convention: a locale ID is between 0 and (num. locales 1d - 1)
type locIdT =  int(32);

param invalidLocID =
  // encode 'max(locIdT)' as a compile-time expression
  2 ** (numBits(locIdT) - 1 - _isSignedType(locIdT):int);


/// class declarations //////////////////////////////////////////////////////

class DimensionalDist : BaseDist {
  // the desired locales to distribute things over;
  // must be a [domain(2, locIdT, false)] locale
  const targetLocales;
  // "IDs" are indices into targetLocales
  proc targetIds return targetLocales.domain;

  // the subordinate 1-d distributions - ones being combined
  var di1, di2;

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
  var dist; // not reprivatized

  // convenience
  proc rangeT  type  return range(idxType, BoundedRangeType.bounded, stridable);
  proc domainT type  return domain(rank, idxType, stridable);
  proc indexT  type  return dist.indexT;

  // subordinate 1-d global domain descriptors
  var dom1, dom2; // not reprivatized

  // This is our index set; we store it here so we can get to it easily.
  // Although strictly speaking it is not necessary.
  var whole: domainT;

  // This is the idxType of the "storage index ranges" to be produced
  // by dsiSetLocalIndices1d(). It needs to be uniform across dimensions,
  // and 'idxType' is the easiest choice (although not the most general).
  // NB it's also computed in DimensionalDist.dsiNewRectangularDom().
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
  var dom; // must be a DimensionalDom

  // no subordinate 1-d array descriptors - we handle storage ourselves

  // the local array descriptors
  // NOTE: 'dom' must be initialized prior to initializing 'localAdescs'
  var localAdescs: [dom.dist.targetIds]
                      LocDimensionalArr(eltType, dom.locDdescType);

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
proc DimensionalDist.DimensionalDist(
  targetLocales,
  di1,
  di2,
  name: string = "dimensional distribution",
  type idxType = int,
  dataParTasksPerLocale: int      = getDataParTasksPerLocale(),
  dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks(),
  dataParMinGranularity: int      = getDataParMinGranularity()
) {
  this.name = name;
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0 then here.numCores
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;
  checkInvariants();

  _passLocalLocIDsDist(di1, true, di2, true,
                     this.targetLocales, true, this.targetLocales.domain.low);
}

// Check all restrictions/assumptions that must be satisfied by the user
// when constructing a DimensionalDist.
proc DimensionalDist.checkInvariants(): void {
  assert(targetLocales.eltType == locale, "DimensionalDist-targetLocales.eltType");
  assert(targetIds.idxType == locIdT, "DimensionalDist-targetIdx.idxType");
  assert(targetIds == [0..#numLocs1, 0..#numLocs2], "DimensionalDist-targetIds");
  assert(di1.numLocales == numLocs1, "DimensionalDist-numLocales-1");
  assert(di2.numLocales == numLocs2, "DimensionalDist-numLocales-2");
  assert(rank == targetLocales.rank, "DimensionalDist-rank");
  assert(rank == 2, "DimensionalDist-rank==2");
  assert(dataParTasksPerLocale > 0, "DimensionalDist-dataParTasksPerLocale");
  assert(dataParMinGranularity > 0, "DimensionalDist-dataParMinGranularity");
}

proc DimensionalDist.dsiClone(): this.type {
  _traceddd("DimensionalDist.dsiClone");
  checkInvariants();

  // do this simple thing, until we find out that we need something else
  return this;
}


//== privatization

proc DimensionalDist.dsiSupportsPrivatization() param return true;

proc DimensionalDist.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  const di1pd = if di1.dsiSupportsPrivatization1d()
    then di1.dsiGetPrivatizeData1d()
    else 0;
  const di2pd = if di2.dsiSupportsPrivatization1d()
    then di2.dsiGetPrivatizeData1d()
    else 0;

  return (targetLocales, name, dataParTasksPerLocale,
          dataParIgnoreRunningTasks, dataParMinGranularity,
          di1, di1pd, di2, di2pd);
}

proc DimensionalDist.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

  // ensure we get a local copy of targetLocales
  // todo - provide the following as utilty functions (for domains, arrays)
  const pdTargetLocales = privatizeData(1);
  const privTargetIds: domain(pdTargetLocales.domain.rank,
                              pdTargetLocales.domain.idxType,
                              pdTargetLocales.domain.stridable
                              ) = pdTargetLocales.domain;
  const privTargetLocales: [privTargetIds] locale = pdTargetLocales;

  proc di1orig return privatizeData(6);
  proc di1pd   return privatizeData(7);
  const di1new = if di1.dsiSupportsPrivatization1d()
    then di1orig.dsiPrivatize1d(di1pd) else di1orig;

  proc di2orig  return privatizeData(8);
  proc di2pd    return privatizeData(9);
  const di2new = if di2.dsiSupportsPrivatization1d()
    then di2orig.dsiPrivatize1d(di2pd) else di2orig;

  const plliddDummy: privTargetLocales.domain.low.type;
  _passLocalLocIDsDist(di1new, di1.dsiSupportsPrivatization1d(),
                       di2new, di2.dsiSupportsPrivatization1d(),
                       privTargetLocales, false, plliddDummy);

  return new DimensionalDist(targetLocales = privTargetLocales,
                             name          = privatizeData(2),
                             idxType       = this.idxType,
                             di1           = di1new,
                             di2           = di2new,
                             dataParTasksPerLocale     = privatizeData(3),
                             dataParIgnoreRunningTasks = privatizeData(4),
                             dataParMinGranularity     = privatizeData(5),
                             dummy = 0);
}

// constructor of a privatized copy
// (currently almost same as user constructor; 'dummy' distinguishes)
proc DimensionalDist.DimensionalDist(param dummy: int,
  targetLocales,
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

proc DimensionalDist.subordinate1dDist(param dim: int) {
  if dim == 1 then
    return di1;
  else if dim == 2 then
    return di2;
  else
    compilerError("DimensionalDist presently supports subordinate1dDist()",
                  " only for dimension 1 or 2, got dim=", dim:string);
}


//== index to locale

//Given an index, this should return the locale that owns that index.
proc DimensionalDist.dsiIndexToLocale(indexx: indexT): locale {
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist presently supports only indexing with",
                  " 2-tuples; got an index of the type ",
                  typeToString(indexx.type));

  return targetLocales(di1.dsiIndexToLocale1d(indexx(1)):int,
                       di2.dsiIndexToLocale1d(indexx(2)):int);
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
  forall (lls, loc) in (targetLocales.domain, targetLocales) do
    if loc == here {
      // if we get multiple matches, we do not specify which is returned
      // could add a pre-test if it were cheap: if !gotresult$.readXX()
      gotresult$;
      result = lls;
      gotresult$ = true;
    }
  // instead of crashing right away, return a flag
  //if !gotresult$.readXX() then halt("DimensionalDist: the current locale ", here, " is not among the target locales ", targetLocales);

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

   // 'l' - "legitimate?"
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
    if !dist1d.dsiUsesLocalLocID1d() then compilerError("DimensionalDist: currently, when a subordinate 1d distribution requires localLocID for *domain* descriptors, it must also require them for *distribution* descriptors");

    // otherwise there is a mismatch: the local locID is copied
    // from a non-privatized object to a privatized one - or visa versa
    if dom1d.dsiSupportsPrivatization1d() != dist1d.dsiSupportsPrivatization1d() then compilerError("DimensionalDist: currently, when a subordinate 1d distribution requires localLocID for domain descriptors, it must support privatization for *domain* descriptors if and only if it supports privatization for *distribution* descriptors");

    dom1d.dsiStoreLocalLocID1d(dist1d.dsiGetLocalLocID1d());
  }
}


/// domain //////////////////////////////////////////////////////////////////


//== privatization

proc DimensionalDom.dsiSupportsPrivatization() param return true;

proc DimensionalDom.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  const dom1pd = if dom1.dsiSupportsPrivatization1d()
    then dom1.dsiGetPrivatizeData1d()
    else 0;
  const dom2pd = if dom2.dsiSupportsPrivatization1d()
    then dom2.dsiGetPrivatizeData1d()
    else 0;

  return (dist.pid, dom1, dom1pd, dom2, dom2pd, localDdescs);
}

proc DimensionalDom.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

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
                                    dom2 = dom2new);
  result.localDdescs = privatizeData(6);

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
  _traceddd(this, ".dsiGetReprivatizeData");

  const dom1rpd = if dom1.dsiSupportsPrivatization1d()
    then dom1.dsiGetReprivatizeData1d()
    else 0;
  const dom2rpd = if dom2.dsiSupportsPrivatization1d()
    then dom2.dsiGetReprivatizeData1d()
    else 0;

  return (dom1, dom1rpd, dom2, dom2rpd, whole);
}

proc DimensionalDom.dsiReprivatize(other, reprivatizeData) {
  _traceddd(this, ".dsiReprivatize on ", here.id);

  assert(this.rank == other.rank &&
         this.idxType == other.idxType &&
         this.stridable == other.stridable);

  // A natural thing to do is to pass 'other = other.dom1' below.
  // However, this *forces* communication to other.locale (I think).
  // So instead we package other.dom1 (or, rather, .dom1 of the original
  // object) into reprivatizeData.
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

proc DimensionalDom.dsiNumIndices return whole.numIndices;

proc DimensionalDom.dsiDims() return whole.dims();

proc DimensionalDom.dsiDim(d) return whole.dim(d);

proc DimensionalDom.subordinate1dDist(param dim: int) {
  return dist.subordinate1dDist(dim);
}


//== writing

proc DimensionalDom.dsiSerialWrite(f: Writer): void {
  f.write(whole);
}


//== creation, SetIndices

// create a new domain mapped with this distribution
proc DimensionalDist.dsiNewRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool)
//  : DimensionalDom(rank, idxType, stridable, this.type, ...)
{
  _traceddd(this, ".dsiNewRectangularDom ",
           (rank, typeToString(idxType), stridable));
  if rank != 2 then
    compilerError("DimensionalDist presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  // todo: ideally, this will not be required;
  // furthermore, DimensionalDist shouldn't be specific to idxType.
  if idxType != this.idxType then
    compilerError("The domain index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the DimensionalDist used to map that domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDist used to map that domain");

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
    compilerError("bug in DimensionalDist: inconsistent stoIndexT");

  //VASS coforall
  for (loc, locIds, locDdesc)
   in (targetLocales, targetIds, result.localDdescs) do
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
  whole = [(...newRanges)];
  _dsiSetIndicesHelper(newRanges);
}

// not part of DSI
proc DimensionalDom._dsiSetIndicesHelper(newRanges: rank * rangeT): void {
  _traceddd(this, ".dsiSetIndices", newRanges);
  if rank != 2 then
    compilerError("DimensionalDist presently supports only 2 dimensions,",
                  " got a domain with ", rank, " dimensions");

  dom1.dsiSetIndices1d(newRanges(1));
  dom2.dsiSetIndices1d(newRanges(2));

  //VASS coforall
  for (locId, locDD) in (dist.targetIds, localDdescs) do
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

  myStorageDom = [myRange1, myRange2];

  _traceddd("DimensionalDom.dsiSetIndices on ", here.id, " ", locId,
            "  storage ", myStorageDom);
}

proc DimensionalDom.dsiGetIndices(): domainT {
  _traceddd(this, ".dsiGetIndices");
  return whole;
}


/// array ///////////////////////////////////////////////////////////////////


//== privatization

proc DimensionalArr.dsiSupportsPrivatization() param return true;

proc DimensionalArr.dsiGetPrivatizeData() {
  _traceddd(this, ".dsiGetPrivatizeData");

  return (dom.pid, localAdescs);
}

proc DimensionalArr.dsiPrivatize(privatizeData) {
  _traceddd(this, ".dsiPrivatize on ", here.id);

  const privdom = chpl_getPrivatizedCopy(objectType = this.dom.type,
                                         objectPid  = privatizeData(1));
  const result = new DimensionalArr(eltType = this.eltType,
                                    dom     = privdom);
  result.localAdescs = privatizeData(2);
  return result;
}


//== miscellanea

proc DimensionalArr.idxType type return dom.idxType; // (could be a field)

proc DimensionalArr.dsiGetBaseDom() return dom;

proc DimensionalArr.subordinate1dDist(param dim: int) {
  return dom.subordinate1dDist(dim);
}


//== creation

// create a new array over this domain
proc DimensionalDom.dsiBuildArray(type eltType)
  : DimensionalArr(eltType, this.type)
{
  _traceddd(this, ".dsiBuildArray");
  if rank != 2 then
    compilerError("DimensionalDist presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  const result = new DimensionalArr(eltType = eltType, dom = this);
  coforall (loc, locDdesc, locAdesc)
   in (dist.targetLocales, localDdescs, result.localAdescs) do
    on loc do
      locAdesc = new LocDimensionalArr(eltType, locDdesc);
  return result;
}


//== dsiAccess

proc DimensionalArr.dsiAccess(indexx: dom.indexT) var: eltType {
  const dom = this.dom;
  _traceddc(traceDimensionalDist || traceDimensionalDistDsiAccess,
            this, ".dsiAccess", indexx);
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist presently supports only indexing with",
                  " 2-tuples; got an array index of the type ",
                  typeToString(indexx.type));

  const (l1,i1):(locIdT, dom.stoIndexT) = dom.dom1.dsiAccess1d(indexx(1));
  const (l2,i2):(locIdT, dom.stoIndexT) = dom.dom2.dsiAccess1d(indexx(2));
  const locAdesc = localAdescs[l1,l2];
//writeln("locAdesc.myStorageArr on ", locAdesc.myStorageArr.locale.id);
  return locAdesc.myStorageArr(i1,i2);
}

//== writing

proc DimensionalArr.dsiSerialWrite(f: Writer): void {
  const dom = this.dom;
  _traceddd(this, ".dsiSerialWrite on ", here.id);
  assert(dom.rank == 2);

  // we largely follow DimensionalArr.these()
  // instead could just use BlockArr.dsiSerialWrite
  if dom.dsiNumIndices == 0 then return;

  var nextD1 = false;
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d() do
    for i1 in r1 {
      if nextD1 then f.writeln();
      nextD1 = true;

      var nextD2 = false;
      for (l2,r2) in dom.dom2.dsiSerialArrayIterator1d() do
        for i2 in r2 {
          const locAdesc = localAdescs[l1,l2];
          const elem = locAdesc.myStorageArr(i1,i2);
          if nextD2 then f.write(" ");
          f.write(elem);
          nextD2 = true;
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

proc myden(a,b) {
  writeln("in myden");
  var dummy: 2*range(int,BoundedRangeType.bounded,true);
  return dummy;
}

//== leader iterator - domain

iter DimensionalDom.these(param tag: iterator) where tag == iterator.leader {
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
        compilerError("DimensionalDist: currently, when a subordinate 1d distribution has dsiIsReplicated1d()==true, it also must have dsiUsesLocalLocID1d()==true");
      const (ix, legit) = dom1d.dsiGetLocalLocID1d();
      // we should only run this on target locales
      assert(legit);
      return ix..ix;
    } else
      return dist.targetIds.dim(dd);
  }
  const overTargetIds = if dom1.dsiIsReplicated1d() || dom2.dsiIsReplicated1d()
    then [helpTargetIds(dom1,1), helpTargetIds(dom2,2)]
    else dist.targetIds; // in this case, avoid re-building the domain

  // todo: lls is needed only for debugging printing?
  //   may be needed by the subordinate 1-d distributions (esp. replicated)?
  //
  //   Bug note: if we change coforall as follows:
  //     coforall ((l1,l2), locDdesc) in (dist.targetIds, localDdescs) do
  //   presently it will crash the compiler on an assertion.
  //
  //VASS coforall
  for (lls, locDdesc) in (overTargetIds, localDdescs[overTargetIds]) do
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

      if numTasks == 0 then
        _traceddc(traceDimensionalDist || traceDimensionalDistIterators,
                  "  leader on ", here.id, " ", lls, " - no tasks");

      type followT = myden(myDims, whole.dims()).type; // was: densify(..).type

      if numTasks == 1 then {

        for r1 in locDdesc.doml1.dsiMyDensifiedRangeForSingleTask1d(dom1) do
          for r2 in locDdesc.doml2.dsiMyDensifiedRangeForSingleTask1d(dom2) do
          {
            const follow: followT = (r1, r2);
            _traceddc(traceDimensionalDistIterators,
                      "  leader on ", lls, " single task -> ", follow);
            yield follow;
          }

      } else {
        //VASS coforall
        for taskid in 0..#numTasks {

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

              // ensure we got a subset
              assert(densify(myDim, whole.dim(dd))(myPiece) == myPiece);

              // Similar to the assert 'lo <= hi' in BlockDom leader.
              // Upon a second thought, if there is a legitimate reason
              // why dsiMyDensifiedRangeForTaskID1d() does not agree
              // with _computeChunkStuff (i.e. the latter returns more tasks
              // than the former wants to use) - fine. Then, replace assert with
              //   if myPiece.length == 0 then do not yield anything
              assert(myPiece.length > 0);

              yield myPiece;
            }
          }

          for r1 in iter1dCheck(1, dom1, locDdesc.doml1, myDims(1)) do
            for r2 in iter1dCheck(2, dom2, locDdesc.doml2, myDims(2)) do
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

iter DimensionalDom.these(param tag: iterator, follower) where tag == iterator.follower {
  _traceddd(this, ".follower on ", here.id, "  got ", follower);

  // This is pre-defined by DSI, so no need to consult
  // the subordinate 1-d distributions.

  for i in [(...unDensify(follower, whole.dims()))] do
    yield i;
}


//== serial iterator - array

// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() var {
  const dom = this.dom;
  _traceddd(this, ".serial iterator");
  assert(dom.rank == 2);

  // Cache the set of tuples for the inner loop.
  // todo: Make this conditional on the size of the outer loop,
  // to reduce overhead in some cases?
  //
  const dim2tuples = dom.dom2.dsiSerialArrayIterator1d();

  // TODO: is this the right approach (ditto for 2nd dimension)?
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d() do
    for i1 in r1 do
      for (l2,r2) in dim2tuples do
        {
          // Could cache locAdesc like for the follower iterator,
          // but that's less needed here.
          const locAdesc = localAdescs[l1,l2];
          _traceddc(traceDimensionalDistIterators,
                    "  locAdesc", (l1,l2), " on ", locAdesc.locale);
          for i2 in r2 do
            yield locAdesc.myStorageArr(i1, i2);
        }
}


//== leader iterator - array

iter DimensionalArr.these(param tag: iterator) where tag == iterator.leader {
  for follower in dom.these(tag) do
    yield follower;
}


//== follower iterator - array   (somewhat similar to the serial iterator)

iter DimensionalArr.these(param tag: iterator, follower) var where tag == iterator.follower {
  const dom = this.dom;
  _traceddd(this, ".follower on ", here.id, "  got ", follower);
  assert(dom.rank == 2);

  // single-element cache of localAdescs[l1,l2]
  var lastl1 = invalidLocID, lastl2 = invalidLocID;
  var lastLocAdesc: localAdescs.eltType;

  // TODO: is this the right approach? (similar to serial)
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  //
  // TODO: can we avoid calling the inner dsiFollowerArrayIterator1d()
  // more than once? Can we create a more efficient interface?
  //
  for (l1,i1) in dom.dom1.dsiFollowerArrayIterator1d(follower(1)) do
    for (l2,i2) in dom.dom2.dsiFollowerArrayIterator1d(follower(2)) do
      {
        // reuse the cache or index into the array?
        if l1 != lastl1 || l2 != lastl2 {
          lastl1 = l1;
          lastl2 = l2;
          lastLocAdesc = localAdescs[l1,l2];
          _traceddc(traceDimensionalDistIterators,
                    "  locAdesc", (l1,l2), " on ", lastLocAdesc.locale);
        }

//writeln("DimensionalArr follower on ", here.id, "  l=(", l1, ",", l2, ")  i=(", i1, ",", i2, ")");

        yield lastLocAdesc.myStorageArr(i1, i2);
      }
}


//// f.chpl
// One-dimensional block-cyclic distribution for use with Dimensional.

//use d;

// the types to use for blockSzie and numLocales
type cycSizeT = uint(32);     // unsigned - for optimization
type cycSizeTuser = int(32);  // for versatility

class idist {
  // distribution parameters
  const lowIdx: int(64); // ensure we can always subtract it
  const blockSize: cycSizeTuser;
  const numLocales: cycSizeTuser;

  // for debugging
  const name:string;

  // tell the compiler these are positive
  proc blockSizePos   return blockSize: cycSizeT;
  proc numLocalesPos  return numLocales: cycSizeT;
  const cycleSizePos: cycSizeT = blockSizePos * numLocalesPos;
}

class idom {
  type idxType;
  type stoIndexT;
  param stridable: bool;

  // for debugging
  const name:string;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range, normalized; its absolute stride
  var wholeR: rangeT;
  var wholeRstrideAbs: idxType;

  // a copy of idist constants
  const lowIdxAdj: idxType;
  const blockSizePos, numLocalesPos, cycleSizePos: cycSizeT;

  // amount of storage per locale per cycle - depends on wholeR.stride
  var storagePerCycle: cycSizeT = 0;
}

class ilocdom {
  type idxType;
  type stoIndexT;
  const locId: locIdT;
}


/////////// privatization - start
proc idist.dsiSupportsPrivatization1d() param return false;
proc idom.dsiSupportsPrivatization1d() param return false;
proc idist.dsiUsesLocalLocID1d() param return false;
proc idom.dsiUsesLocalLocID1d() param return false;
/////////// privatization - end


// Constructor: use the default one for now.
//proc idist.idist(...)

// Check all restrictions/assumptions that must be satisfied by the user
// when constructing a 1-d BlockCyclic distribution.
inline proc idist.checkInvariants() {
  assert(blockSize > 0, "BlockCyclic1d-blockSize");
  assert(numLocales > 0, "BlockCyclic1d-numLocales");
}

// Assert that the value of 'src' is preserved when casting it to 'destT'.
inline proc _checkFitsWithin(src: integral, type destT)
  where _isIntegralType(destT)
{
  inline proc ensure(arg:bool) { assert(arg); }
  type maxuT = uint(64); // the largest unsigned type
  type srcT = src.type;
  proc numMantBits(type T) param
    return numBits(T) - if _isSignedType(T) then 1 else 0;

  if _isUnsignedType(destT) {
    ensure(isNonnegative(src));
    if numMantBits(srcT) > numMantBits(destT) then
      ensure(src:maxuT <= max(destT):maxuT);

  } else {
    // destT is signed
    if _isUnsignedType(srcT) && numMantBits(srcT) > numMantBits(destT) then
      ensure(src:maxuT <= max(destT):maxuT);
    if _isSignedType(srcT) && numBits(destT) < numBits(srcT) {
      ensure(src <= max(destT):srcT);
      ensure(src >= min(destT):srcT);
    }
  }
}

proc idist.dsiNewRectangularDom1d(type idxType, param stridable: bool,
                                  type stoIndexT)
{
  checkInvariants();
  _checkFitsWithin(this.lowIdx, idxType);
  const lowIdxDom = this.lowIdx: idxType;

  // Allow for idxType and/or stoIndexT to be unsigned, by replacing
  //  ind0 = ind -lowIdx  -->  ind0 = ind + lowIdxAdj
  //
  // where lowIdxAdj is
  //  -lowIdx whenever possible (more natural for debugging), otherwise
  //  -lowIdx shifted up by a multiple of cycleSize until it is >=0.
  //
  proc adjustLowIdx()  return cycleSizePos - mod(lowIdxDom, cycleSizePos);
  //
  const lowIdxAdj: idxType =
    if _isSignedType(idxType)
    then
      if _isSignedType(stoIndexT)
      then
        -lowIdxDom
      else
        if lowIdxDom <= 0 then -lowIdxDom else adjustLowIdx()
    else
      if lowIdxDom == 0 then lowIdxDom else adjustLowIdx()
    ;

    const negate = _isSignedType(idxType) && lowIdxDom <= 0;

  const result = new idom(idxType = idxType,
                  stoIndexT = stoIndexT,
                  stridable = stridable,
                  lowIdxAdj = lowIdxAdj,
                  blockSizePos  = this.blockSizePos,
                  numLocalesPos = this.numLocalesPos,
                  cycleSizePos  = this.cycleSizePos,
                  name = this.name);
//writeln();
//writeln("dsiNewRectangularDom1d ", result);

  return result;
}

proc idom.dsiIsReplicated1d() param return false;

proc idom.dsiNewLocalDom1d(type stoIndexT, locId: locIdT) {
  const result = new ilocdom(idxType = this.idxType,
                             stoIndexT = stoIndexT,
                             locId = locId);
  return result;
}


/////////////////////////////////

// #include notes.VASS

inline proc idom._dsiInd0(ind: idxType): idxType
  return ind + lowIdxAdj;

inline proc idom._dsiCycNo(ind: idxType)
  return divfloor(_dsiInd0(ind), cycleSizePos): idxType;

inline proc idom._dsiCycOff(ind: idxType)
  return mod(_dsiInd0(ind), cycleSizePos): cycSizeT;

// "formula" in the name emphasizes no sanity checking
inline proc idom._dsiLocNo_formula(ind: idxType): locIdT {
  // keep in sync with idist.dsiIndexToLocale1d()
  const ind0 = _dsiInd0(ind);
  return
    if isNonnegative(ind0) then ( (ind0/blockSizePos) % numLocalesPos ): locIdT
    else  mod(divfloor(ind0, blockSizePos), numLocalesPos): locIdT
    ;
}

inline proc idom._dsiLocOff(ind: idxType)
  return ( _dsiCycOff(ind) % blockSizePos ): stoIndexT;

// hoist some common code
inline proc idom._dsiStorageIdx2(cycNo, locOff)
  return cycNo * storagePerCycle + _divByStride(locOff);

// "formula" in the name implies no sanity checking
// in particular at the moment its type may not be stoIndexT
inline proc idom._dsiStorageIdx_formula(ind: idxType)
  return _dsiStorageIdx2(_dsiCycNo(ind), _dsiLocOff(ind));

inline proc idom._dsiStorageIdx(ind: idxType)
  return _dsiStorageIdx_formula(ind): stoIndexT;

// oblivious of 'wholeR'
inline proc idom._dsiIndicesOnCycLoc(cycNo: idxType, locNo: locIdT)
  : range(idxType)
{
  const startCycle = (cycNo * cycleSizePos): idxType - lowIdxAdj;
  const startLoc = startCycle:idxType + locNo:idxType * blockSizePos:idxType;
  return startLoc..#blockSizePos:idxType;
}

/////////////////////////////////

proc idist.dsiIndexToLocale1d(ind): locIdT {
  // keep in sync with idom._dsiLocNo_formula
  const ind0 = ind - lowIdx;
  const locNo =
    if ind0 >= 0 then ( (ind0 / blockSize) % numLocales )//: locIdT
    else  mod(divfloor(ind0, blockSizePos), numLocalesPos)//: locIdT
    ;

  assert(0 <= locNo && locNo < numLocales);
  // todo: the following assert should not be needed - it can be proven
  assert(locNo == mod(ind0, cycleSizePos) / blockSize);

  return locNo;
}

// allow uint(64) indices, but assert that they fit in int(64)
inline proc idist.dsiIndexToLocale1d(ind: uint(64)): locIdT {
  type convT = int(64);
  assert(ind <= max(convT));
  return dsiIndexToLocale1d(ind:convT);
}

//var debugD1Shown = false;
//proc idom.debugIsD1() return name == "D1";

proc idom.dsiSetIndices1d(rangeArg: rangeT): void {
  // For now, require the user to provide unambiguous ranges only.
  // This requirement could potentially be avoided (as long as no arrays
  // are declared over the domain), but it simplifies/speeds up our code.
  //
  // todo: document this in the spec for this distribution.
  // see also an assert is dsiSetLocalIndices1d()
  assert(!rangeArg.isAmbiguous());

  const prevStoragePerCycle = storagePerCycle;

  // As of this writing, alignedLow/High are valid even for empty ranges
  if stridable {
    wholeR = rangeArg.alignedLow..rangeArg.alignedHigh by rangeArg.stride;
    wholeRstrideAbs = abs(rangeArg.stride): idxType;
    storagePerCycle = (1 + (blockSizePos - 1) / wholeRstrideAbs): cycSizeT;
  } else {
    assert(rangeArg.stride == 1); // ensures we can simplify things
    wholeR = rangeArg.alignedLow..rangeArg.alignedHigh;
    wholeRstrideAbs = 0; // be sure nobody ever reads this
    storagePerCycle = blockSizePos;
  }

//proc writei(i) {
//  write("(", _dsiCycNo(i), ",", _dsiLocNo_formula(i),
//        ",", _dsiCycOff(i) % blockSizePos, ")");
//}
//writeln();
//write  ("dsiSetIndices1d(", name, ", ", rangeArg, ") --> ");
//  write(wholeR, " | ");
//  writei(wholeR.low); write(" .. "); writei(wholeR.high);
//  writeln();
//if debugIsD1() then debugD1Shown = false;

  if prevStoragePerCycle != 0 && storagePerCycle != prevStoragePerCycle then
    stderr.writeln("warning: array resizing is not implemented upon change in dimension stride with 1-d BlockCyclic distribution");
}

inline proc idom._divByStride(locOff)  return
  if stridable then ( locOff / wholeRstrideAbs ): stoIndexT
  else              locOff: stoIndexT;

// _dsiStorageLow(), _dsiStorageHigh(): save a few mods and divisions
// at the cost of potentially allocating more storage
inline proc idom._dsiStorageLow(locId: locIdT): stoIndexT {
  const lowW = wholeR.low;

  // smallest cycNo(i) for i in wholeR
  var   lowCycNo  = _dsiCycNo(lowW): stoIndexT;
  const lowLocNo  = _dsiLocNo_formula(lowW);
  var   lowIdxAdj = 0: stoIndexT;

  // (Optional) tighten the storage if wholeR
  // does not fall on this locale in the lowest cycle.
  if lowLocNo > locId then
    lowCycNo += 1;
  else
    // (Optional) tighten the storage if wholeR
    // starts on this locale, but not at the beginning of it.
    if lowLocNo == locId then
      lowIdxAdj = _divByStride(_dsiLocOff(lowW));

  return lowCycNo * storagePerCycle:stoIndexT + lowIdxAdj;
}

inline proc idom._dsiStorageHigh(locId: locIdT): stoIndexT {
  const hiW = wholeR.high;

  // biggest cycNo(i) for i in wholeR
  var   hiCycNo  = _dsiCycNo(hiW): stoIndexT;
  const hiLocNo  = _dsiLocNo_formula(hiW);
  var   hiIdxAdj = (storagePerCycle - 1): stoIndexT;

  // (Optional) tighten the storage if wholeR
  // does not fall on this locale in the highest cycle.
  if hiLocNo < locId then
    hiCycNo -= 1;
  else
    // (Optional) tighten the storage if wholeR
    // ends on this locale, but not at the end of it.
    if hiLocNo == locId then
      hiIdxAdj = _divByStride(_dsiLocOff(hiW));

  return hiCycNo * storagePerCycle:stoIndexT + hiIdxAdj;
}

proc ilocdom.dsiSetLocalIndices1d(globDD, locId: locIdT): range(stoIndexT) {
  const stoLow = globDD._dsiStorageLow(locId);
  const stoHigh = globDD._dsiStorageHigh(locId);

//proc debugShowD1() {
// if debugD1Shown then return false; debugD1Shown = true; return true;
//}
//if globDD.debugIsD1() && !debugD1Shown then writeln();
//if !globDD.debugIsD1() || debugShowD1() {
//writeln("dsiSetLocalIndices1d ", globDD.name, "  l ", locId,
//        " -> ", stoLow, "..", stoHigh,
//        if stoLow <= stoHigh then "" else "  empty");
//}

  return stoLow:stoIndexT .. stoHigh:stoIndexT;
}

proc idom.dsiAccess1d(ind: idxType): (locIdT, stoIndexT) {
  return (_dsiLocNo_formula(ind), _dsiStorageIdx(ind));
}

iter ilocdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
  const locNo = this.locId;
  const wholeR = globDD.wholeR;
  const lowIdx = wholeR.low;
  const highIdx = wholeR.high;
  param stridable = globDD.stridable;
  assert(stridable == wholeR.stridable); // sanity

  if lowIdx > highIdx then
    return;

  const lowCycNo = globDD._dsiCycNo(lowIdx);
  const highCycNo = globDD._dsiCycNo(highIdx);
  const up = wholeR.stride > 0;
  assert(lowIdx <= highIdx);

  proc mydensify(densifyee) {
    const temp = densify(densifyee, wholeR);
    assert(temp.stride == wholeR.stride);
    return temp:wholeR.type;
  }

  var curIndices = globDD._dsiIndicesOnCycLoc(
                            if up then lowCycNo else highCycNo, locNo);

  const firstRange = wholeR[curIndices];
  if firstRange.low <= firstRange.high then
    yield mydensify(firstRange);
  // else nothing to yield on this locale

  if lowCycNo == highCycNo then
    // we have covered all cycles
    return;

  proc advance() {
    curIndices = curIndices.translate(
      if !stridable || up then globDD.cycleSizePos else -globDD.cycleSizePos);
  }

  for cycNo in (lowCycNo + 1) .. (highCycNo - 1) {
    advance();
    const curRange =
      if stridable then curIndices by wholeR.stride align wholeR.alignment
      else              curIndices;
    yield mydensify(curRange);
  }

  advance();
  const lastRange = wholeR[curIndices];
  if lastRange.low <= lastRange.high then
    yield mydensify(lastRange);
}

proc idom.dsiSingleTaskPerLocaleOnly1d() param return true;

// not supported at the moment:
//proc ilocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int)

iter idom.dsiSerialArrayIterator1d() {
  // dispatch here, for code clarity
  if stridable then
    for result in dsiSerialArrayIterator1dStridable() do
      yield result;
  else
    for result in dsiSerialArrayIterator1dUnitstride() do
      yield result;
}

iter idom.dsiSerialArrayIterator1dUnitstride() {
  assert(!stridable);

  const firstIdx = wholeR.low;
  const lastIdx = wholeR.high;

  // This rarely fires, but if so it gets rid of lots of computations.
  // In the common case it adds just 1 branch to at least 2 branches.
  if firstIdx > lastIdx then return;

  // the current point, initialized to the starting point
  var cycNo = _dsiCycNo(firstIdx);
  var locNo = _dsiLocNo_formula(firstIdx);
  var locOff = _dsiLocOff(firstIdx);

  // the final point
  const lastCycNo = _dsiCycNo(lastIdx);
  const lastLocNo = _dsiLocNo_formula(lastIdx);
  const lastLocOff = _dsiLocOff(lastIdx);

  // shortcut
  proc spec(start, end)  return
    (locNo, _dsiStorageIdx2(cycNo, start): stoIndexT ..
             _dsiStorageIdx2(cycNo, end): stoIndexT);

  assert(cycNo <= lastCycNo);
  assert(locNo < numLocalesPos);
  while cycNo < lastCycNo {
    while locNo < numLocalesPos {
      yield spec(locOff, blockSizePos - 1);
      locNo += 1;
      locOff = 0;
    }
    cycNo += 1;
    locNo = 0;
  }

  assert(cycNo == lastCycNo);
  assert(locNo <= lastLocNo);
  while locNo < lastLocNo {
    yield spec(locOff, blockSizePos - 1);
    locNo += 1;
    locOff = 0;
  }

  assert(cycNo == lastCycNo);
  assert(locNo == lastLocNo);
  assert(locOff <= lastLocOff);
  yield spec(locOff, lastLocOff);
}

iter idom.dsiSerialArrayIterator1dStridable() {
  assert(stridable);
  // the simplest way out
  for ind in wholeR do
    yield (_dsiLocNo_formula(ind), _dsiStorageIdx(ind)..#(1:stoIndexT));
}



//// u.chpl
// various utilites for testing

//use d;


/// label the output with "phases" //////////////////////////////////////////

var pfx = "# ";
var phase = 0;
var no_pfx = false;

// to preserve coarse ordering ("phases") when sorting the output
proc nextphase() {
  phase += 1;
  pfx = if no_pfx then "" else format("####", phase) + "  ";
  traceDimensionalDistPrefix = pfx;
}

// Force the next phase to be 'ph' - so that when phases are added earlier,
// downstream output does not change.
// It is OK to advance to (the current phase number + 1), or later.
proc fphase(ph) {
  if phase >= ph {
    hd("###### advancing phase to a duplicate phase number: ",
       phase, " -> ", ph);
    tl();
  }
  phase = ph - 1;
}

// heading/start new "phase"
proc hd(args...) { nextphase(); writeln(pfx, (...args)); }
proc hhd(args...) { nextphase(); writeln(pfx, (...args), " on ", here.id); }
// additional messages
proc msg(args...) { writeln(pfx, (...args)); }
proc msgp() { write(pfx); }
proc msgserial(arg) { write(pfx); for e in arg do write(" ", e); writeln(); }
// tail - finish a "phase"
proc tl()         { writeln(pfx); }


/// locales for testing /////////////////////////////////////////////////////

var mylocdom: domain(2);
var mylocs: [mylocdom] locale;
var manylocs: bool;

// Initialize 'mylocs' and 'manylocs'.
//
proc setupLocales(s1:int, s2:int, ensureManyLocs: bool = false) {
  hd("setupLocales ", s1, "*", s2);
  mylocdom = [0..#s1,0..#s2];
  manylocs = (numLocales >= mylocs.numElements);

  if manylocs {
    var i = 0;
    for ml in mylocs { ml = Locales(i); i += 1; }
  } else {
    msg("oversubscribing Locales(0)");
    mylocs = Locales(0);
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    mylocs.numElements, ", got ", numLocales);

  msg("mylocs");
  msgserial(mylocs);
  tl();
}

// If oversubscribing a single locale, yield that one.
// Otherwise yield each of the distinct locales.
//
iter overLocales(): locale {
  if (manylocs) then
    for l in mylocs do yield l;
  else
    yield here;
}

/// testing functions ///////////////////////////////////////////////////////
/// these are really a part of testing, so may change frequently

proc i2lTest(dm, ix) {
  hd(ix, " is on locale:");
  for l in overLocales() do
    on l do
      msg(l, ": ", dm.idxToLocale(ix));
  tl();
}

proc showDom(dmdom) {
  for l in overLocales() do
    on l do
      doShowDom(dmdom);
}

proc doShowDom(dmdom) {
  hhd("dmdom - serial iteration");
  msgserial(dmdom);
  tl();

  hhd("dmdom - parallel iteration");
  forall i in dmdom do msg(" ", i, "  on ", here.id);
  tl();
}

proc showArr(dmdom, dmarr, dmhelp) {
  for l in overLocales() do
    on l do
      doShowArr(dmdom, dmarr, dmhelp);
}

proc doShowArr(dmdom, dmarr, dmhelp) {
  hhd("dmarr - serial iteration");
  msgserial(dmarr);
  tl();

  hhd("dmarr - parallel iteration");
  forall elm in dmarr do
    msg(" ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr - assignment to dmhelp");
  resetDmhelp(dmhelp);
  dmhelp = dmarr;
  tl();
  
  hhd("dmhelp after assignment");
  msgserial(dmhelp);
  tl();

  //writeln(dmarr); -- on Locale 0
  //dmarr.writeThis(stdout); writeln(); -- works by chance

  hhd("dmdom, dmarr - zippered iteration");
  forall (ix,elm) in (dmdom,dmarr) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr, dmdom - zippered iteration");
  forall (elm,ix) in (dmarr,dmdom) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr, dmhelp - zippered iteration");
  resetDmhelp(dmhelp);
  forall (elm,hlp) in (dmarr,dmhelp) do
    msg(" ", elm, " ", hlp, "  on ", here.id);
  tl();
}

proc resetDmhelp(dmhelp: []) {
  if dmhelp.rank != 2 then
    compilerError("resetDmhelp is implemented only for 2-d arrays");

  for (ix, elm) in (dmhelp.domain, dmhelp) do
    elm = ix(1)*100 + ix(2);
}

var privTestWarningShown:bool = false;

// NOTE: this does not work because 'dmarr' is not treated as local
// in the 'on' clause
//
proc privTest(dmarr, locIds, arrIdx, expVal) {
  proc warn(msgtext) {
    if privTestWarningShown then return;
    privTestWarningShown = true;
    msg(msgtext);
  }

  if manylocs {
    if traceDimensionalDist {
      warn("skipped because traceDimensionalDist = true");

    } else {
      on mylocs(locIds) {
        msg(" mylocs", locIds, "  dmarr", arrIdx, "  expecting ", expVal,
            "  on ", here.id);
        var temp = 999999;
        const idx = arrIdx;
        assert(temp != expVal);
        local {
          temp = dmarr(idx);
        }
        if temp != expVal then
          msg("*** error: expected ", expVal, ", got ", temp);

      }
    }
  } else {
    warn("skipped because of oversubscribing Locales(0)");
  }
}


//// t8.chpl
// Testing various aspects of 1-d BlockCyclic.

//use d, f, u;

config const verbose = false;
config const nopfx = false;
no_pfx = nopfx;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);

var myphase = 1;
proc leapphase() { myphase += 10; fphase(myphase); }

proc test(d) {
  leapphase();

  hd("domain = ", d);
  tl();

  const vd = d._value.dom2;
  if verbose {
    hd("vd = ", vd, "  : ", typeToString(vd.type));
    tl();
  } else {
    nextphase();
  }

  hd("storage");
  for (ix, locDdesc) in (d._value.dist.targetIds, d._value.localDdescs) do
    msg(" ", ix, "   ", locDdesc.myStorageDom);
  tl();

  proc t1(param k) {
    hd("dsiAccess1d dim", k);
    const subdom = if k==1 then d._value.dom1 else d._value.dom2;
    for i in d.dim(k) do
      msg(" ", i, "   ", subdom.dsiAccess1d(i));
    tl();
  }

  t1(1);
  t1(2);

  hd("serial iterator over the domain");
  msgserial(d);
  tl();

  hd("parallel iterator over the domain");
  forall i in d do msg(i);
  tl();

  hd("creating an array");
  var a: [d] int;
  tl();

  hd("initializing the array with explicit indexing");
  for ix in d do
    a[ix] = ( ix(1)*1000 + ix(2) ): a.eltType;
  tl();

  hd("serial iterator over the array");
  msgserial(a);
  tl();

} // test()

proc testsuite(type T) {
//compilerWarning("testsuite -- ", typeToString(T), 0);
  myphase = 1; leapphase();
  hd("testsuite(", typeToString(T), ")");
  tl();

  const df8 = new idist(lowIdx=-100, blockSize=7, numLocales=1, name="D1");
  const df9 = new idist(lowIdx=-10, blockSize=5, numLocales=3, name="D2");
  const dm = new dmap(new DimensionalDist(mylocs, df8, df9, "dm", idxType=T));

  proc tw(a, b, c, d) { test([a:T..b:T, c:T..d:T] dmapped dm); }
  inline
  proc t2(a,b) { tw(5,5,a,b); }
  proc t22(a,b,st,al) { test([5:T..5:T, a:T..b:T by st align al:T] dmapped dm); }

  t2(7,7);   // 1,0
  t2(12,12); // 1,1
  t2(18,18); // 1,2

  t2(5,9);   // 1,0
  t2(10,14); // 1,1
  t2(15,19); // 1,2

  t2(3,7);   // 0,2 - 1,0

  t2(3,12);  // 0,2 - 1,1
  t2(3,15);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(3,17);  // 0,2 - 1,2
  t2(3,18);  // 0,2 - 1,2
  t2(3,19);  // 0,2 - 1,2

  t2(0,16);  // 0,2 - 1,2
  t2(1,16);  // 0,2 - 1,2
  t2(2,16);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(4,16);  // 0,2 - 1,2

  t22(0,19,3,0);
  t22(0,19,3,1);
  t22(0,19,3,2);

  t22(0,19,5,1);
  t22(0,19,7,1);
  t22(0,19,8,1);
  t22(0,19,9,1);
  t22(0,19,10,1);
  t22(0,19,15,1);
  t22(0,19,18,1);
  t22(0,19,19,1);
  t22(0,19,20,1);

  tw(1,1,  7,7);  // 14,0 | 1,0
  tw(1,1, 12,12); // 14,0 | 1,1
  tw(1,1, 18,18); // 14,0 | 1,2

  tw(5,11, 5,9);
  tw(5,11, 10,14);
  tw(5,11, 16,19);

  test([5:T..11:T, 12:T..12:T] dmapped dm);

  test([1:T..1:T, 0:T..9:T       ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by  1 ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by -1 ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by -2 ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by  3 ] dmapped dm);

  tw(5,11, 47,42);
  tw(11,5, 42,47);
  tw(11,5, 47,42);
}

testsuite(int);
