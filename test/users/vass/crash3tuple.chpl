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
inline proc _traceddc(param condition: bool, args...)
{
  if condition then writeln(traceDimensionalDistPrefix,(...args));
}
inline proc _traceddc(param cond, d:DimensionalDist, args...)
{ _traceddc(cond, "DimensionalDist(", d.name, ")", (...args)); }
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
type locIdT =  int(64);

// ... local storage size and indices (0-based)
type stoSzT  = uint(32);

param invalidLocID =
  // encode 'max(locIdT)' as a compile-time expression
  (2 ** (numBits(locIdT) - 1 - isIntType(locIdT):int)):int(64);


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

  // this is our index set; we store it here so we can get to it easily
  // although it is not necessary, strictly speaking
  var whole: domainT;

  // convenience - our instantiation of LocDimensionalDom
  proc lddTypeArg1 type  return domainT;
  proc lddTypeArg2 type  return (dom1.dsiNewLocalDom1d(0).type,
                                 dom2.dsiNewLocalDom1d(0).type);
  proc locDdescType type  return LocDimensionalDom(lddTypeArg1, lddTypeArg2);

  // local domain descriptors
  var localDdescs: [dist.targetIds] locDdescType; // not reprivatized

  // for privatization
  var pid: int = -1;
}

class LocDimensionalDom {

  // myBlock: which of the indices reside on this locale,
  // as reported by the subordinate 1-d descriptors.
  //
  // Btw it is really a subdomain(DimensionalDom.whole).
  // We do not declare it as such to avoid the link to the global descriptor
  // (although it might be doable to have such a link).
  var myBlock;

  // a tuple of subordinate 1-d local domain descriptors
  var local1dDdescs;

  // how much storage to allocate for an array on our locale
  var myStorageDom: domain(myBlock.rank, stoSzT, false);
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
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0 then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;
  checkInvariants();

  _passLocalLocIDsDist(di1, true, di2, true,
                     this.targetLocales, true, this.targetLocales.domain.low);
}

// Check all things that must be provided by the user
// when constructing a DimensionalDist.
proc DimensionalDist.checkInvariants(): void {
  assert(targetLocales.eltType == locale, "DimensionalDist-targetLocales.eltType");
  assert(targetIds.idxType == locIdT, "DimensionalDist-targetIdx.idxType");
  assert(targetIds == {0..#numLocs1, 0..#numLocs2},
         "DimensionalDist-targetIds");
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
  forall (lls, loc) in zip(targetLocales.domain, targetLocales)
    with (ref result)
  do
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
        if lg1 then locDdesc.local1dDdescs(1)
                      .dsiStoreLocalDescToPrivatizedGlobalDesc1d(dom1new);
        if lg2 then locDdesc.local1dDdescs(2)
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
  _traceddd(this, "dsiReprivatize on ", here.id);

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

  if idxType != this.idxType then
    compilerError("The domain index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the DimensionalDist used to map that domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDist used to map that domain");
  
  const dom1 = di1.dsiNewRectangularDom1d(idxType, stridable);
  _passLocalLocIDsDom1d(dom1, di1);

  const dom2 = di2.dsiNewRectangularDom1d(idxType, stridable);
  _passLocalLocIDsDom1d(dom2, di2);

  const result = new DimensionalDom(rank=rank, idxType=idxType,
                                  stridable=stridable, dist=this,
                                  dom1 = dom1, dom2 = dom2);
  // result.whole is initialized to the default value (empty domain)
  coforall (loc, locIds, locDdesc)
   in zip(targetLocales, targetIds, result.localDdescs) do
    on loc {
      const defaultVal1: result.lddTypeArg1;
      const locD1 = dom1.dsiNewLocalDom1d(locIds(1));
      const locD2 = dom2.dsiNewLocalDom1d(locIds(2));
      locDdesc = new LocDimensionalDom(defaultVal1, (locD1, locD2));
    }

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
  _traceddd(this, ".dsiSetIndices", newRanges);
  if rank != 2 then
    compilerError("DimensionalDist presently supports only 2 dimensions,",
                  " got a domain with ", rank, " dimensions");

  dom1.dsiSetIndices1d(newRanges(1));
  dom2.dsiSetIndices1d(newRanges(2));

  coforall (locId, locDD) in zip(dist.targetIds, localDdescs) do
    on locDD do
      locDD._dsiLocalSetIndicesHelper((dom1, dom2), locId);
}

// not part of DSI
// TODO: need to preserve the old contents
// in the intersection of the old and new domains' index sets
proc LocDimensionalDom._dsiLocalSetIndicesHelper(globDD, locId) {
  // we query the *global* subordinate 1-d domain descriptors
  var myRange1 = local1dDdescs(1).dsiSetLocalIndices1d(globDD(1),locId(1));
  var myRange2 = local1dDdescs(2).dsiSetLocalIndices1d(globDD(2),locId(2));

  myBlock = {myRange1, myRange2};
  myStorageDom = {0:stoSzT..#myRange1.length:stoSzT,
                  0:stoSzT..#myRange2.length:stoSzT};

  _traceddd("DimensionalDom.dsiSetIndices on ", here.id, " ", locId, " <- ",
           myBlock, "  storage ", myRange1.length, "*", myRange2.length);
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
   in zip(dist.targetLocales, localDdescs, result.localAdescs) do
    on loc do
      locAdesc = new LocDimensionalArr(eltType, locDdesc);
  return result;
}


//== dsiAccess

proc DimensionalArr.dsiAccess(indexx: dom.indexT) ref: eltType {
  const dom = this.dom;
  _traceddc(traceDimensionalDist || traceDimensionalDistDsiAccess,
            this, ".dsiAccess", indexx);
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist presently supports only indexing with",
                  " 2-tuples; got an array index of the type ",
                  typeToString(indexx.type));

  const (l1,i1):(locIdT, stoSzT) = dom.dom1.dsiAccess1d(indexx(1));
  const (l2,i2):(locIdT, stoSzT) = dom.dom2.dsiAccess1d(indexx(2));
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
          const elem = locAdesc.myStorageArr(i1:stoSzT,i2:stoSzT);
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


//== leader iterator - domain

iter DimensionalDom.these(param tag: iterKind) where tag == iterKind.leader {
  _traceddd(this, ".leader");
  assert(rank == 2);

  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;

  // A hook for the Replicated distribution.
  // If this is not enough, consult the subordinate 1-d descriptors
  // instead of this and 'myDims = locDdesc.myBlock.dims()' below.
  proc helpTargetIds(dom1d, param dd) {
    if dom1d.dsiIsReplicated() {
      if !dom1d.dsiUsesLocalLocID1d() then
        compilerError("DimensionalDist: currently, when a subordinate 1d distribution has dsiIsReplicated()==true, it also must have dsiUsesLocalLocID1d()==true");
      const (ix, legit) = dom1d.dsiGetLocalLocID1d();
      // we should only run this on target locales
      assert(legit);
      // not writing 'ix..#1' because it seems inefficient for this simple use
      return ix..ix;
    } else
      return dist.targetIds.dim(dd);
  }
  const overTargetIds = if dom1.dsiIsReplicated() || dom2.dsiIsReplicated()
    then {helpTargetIds(dom1,1), helpTargetIds(dom2,2)}
    else dist.targetIds; // in this case, avoid re-building the domain

  // todo: lls is needed only for debugging printing?
  //   may be needed by the subordinate 1-d distributions (esp. replicated)?
  //
  //   Bug note: if we change coforall as follows:
  //     coforall ((l1,l2), locDdesc) in (dist.targetIds, localDdescs) do
  //   presently it will crash the compiler on an assertion.
  //
  coforall (lls, locDdesc) in zip(overTargetIds, localDdescs[overTargetIds]) do
    on locDdesc {
      // mimic BlockDom leader

      // For now we handle almost everything ourselves. I.e. we consult the
      // subordinate 1-d distribution only for parDim only when numTasks>1.
      // We assume that myBlock already has most of what we need to know.
      // The only reason for doing so is simplicity (e.g. make the interface to
      // the subordinate 1-d distributions smaller). Change this if needed.

      const myDims = locDdesc.myBlock.dims();
writeln(lls(2), " dom whole ", whole, "  myDims ", myDims);
      const (numTasks, parDim) =
        // a debugging hook
        if fakeDimensionalDistParDim <= 0 then
          _computeChunkStuff(maxTasks, ignoreRunning, minSize, myDims)
        else
          ( min(_computeNumChunks(maxTasks, ignoreRunning, minSize,
                                  locDdesc.myBlock.numIndices),
                locDdesc.myBlock.dim(fakeDimensionalDistParDim).length):int,
            fakeDimensionalDistParDim:int);

      // parDim gotta point to one of the dimensions that we have
      assert(numTasks == 0 || (1 <= parDim && parDim <= rank));

      if numTasks == 0 then
        _traceddc(traceDimensionalDist || traceDimensionalDistIterators,
                  "  leader on ", here.id, " ", lls, " - no tasks");

      if numTasks == 1 then {
        const follow = densify(myDims, whole.dims());
        _traceddc(traceDimensionalDistIterators,
                  "  leader on ", lls, " single task -> ", follow);
        yield follow;

      } else {
        coforall taskid in 0..#numTasks {
          var follow: densify(myDims, whole.dims()).type;
          // follow(parDim) is this task's share of myDims(parDim);
          // follow(d!=parDim) is the entire myDims(d);
          // in both cases, they are densified w.r.t. 'whole'.

          // We have a param loop because need a param to index local1dDdescs.
          // TODO: make local1dDdescs homogenous (introduce a superclass
          // of subordinate 1-d local domain descriptors?).
          //
          for param dd in 1..rank do if dd == parDim {

            const dom1d = if dd==1 then dom1 else if dd==2 then dom2 else nil;
            const myPiece = locDdesc.local1dDdescs(dd).
              dsiMyDensifiedRangeForTaskID1d(dom1d, taskid, numTasks);

writeln(lls(2), "-", taskid,
        "  dens. myDims ", densify(myDims(dd), whole.dim(dd)),
        "  dens. myPiece ", myPiece);

            // ensure we got a subset
            if !(densify(myDims(dd), whole.dim(dd))(myPiece) == myPiece) then
              writeln("assert 1 failed");

            // Similar to the assert 'lo <= hi' in BlockDom leader.
            // Upon a second thought, if there is a legitimate reason
            // why dsiMyDensifiedRangeForTaskID1d() does not agree
            // with _computeChunkStuff (i.e. the latter returns more tasks
            // than the former wants to use) - fine. Then, replace assert with
            //   if myPiece.length == 0 then do not yield anything
            if !(myPiece.length > 0) then
              writeln("assert 2 failed");

            // apply myPiece
            follow(dd) = myPiece;
          } else { // dd != parDim
            follow(dd) = densify(myDims(dd), whole.dim(dd));
          } // for dd

writeln(lls(2), "-", taskid, "  follower ", follow);

          _traceddc(traceDimensionalDistIterators, "  leader on ", lls,
                    " task ", taskid, "/", numTasks, " -> ", follow);

          yield follow;
        }
      } // if numTasks
    } // coforall ... on locDdesc

} // leader iterator - domain


//== leader iterator - array

iter DimensionalArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}


//== follower iterator - domain

iter DimensionalDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  _traceddd(this, ".follower on ", here.id, "  got ", followThis);

  // This is pre-defined by DSI, so no need to consult
  // the subordinate 1-d distributions.

  for i in {(...unDensify(followThis, whole.dims()))} do
    yield i;
}


pragma "auto copy fn"
proc chpl__autoCopy(x: DimensionalDom) {
  x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: DimensionalDom) {
  var cnt = x.destroyDom();
  if cnt == 0 then
    delete x;
}


//== serial iterator - array

// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() ref {
  const dom = this.dom;
  _traceddd(this, ".serial iterator");
  assert(dom.rank == 2);

  // TODO: is this the right approach?
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d() do
    for (l2,r2) in dom.dom2.dsiSerialArrayIterator1d() do
      {
        const locAdesc = localAdescs[l1,l2];
        _traceddc(traceDimensionalDistIterators,
                  "  locAdesc", (l1,l2), " on ", locAdesc.locale);
        for i1 in r1 do
          for i2 in r2 do
            yield locAdesc.myStorageArr(i1:stoSzT, i2:stoSzT);
      }
}


//== follower iterator - array   (similar to the serial iterator)

iter DimensionalArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  const dom = this.dom;
  _traceddd(this, ".follower on ", here.id, "  got ", followThis);
  assert(dom.rank == 2);

  // TODO: is this the right approach? (similar to serial)
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  for (l1,r1) in dom.dom1.dsiFollowerArrayIterator1d(followThis(1)) do
    for (l2,r2) in dom.dom2.dsiFollowerArrayIterator1d(followThis(2)) do
      {
        const locAdesc = localAdescs[l1,l2];
        _traceddc(traceDimensionalDistIterators,
                  "  locAdesc", (l1,l2), " on ", locAdesc.locale);
        for i1 in r1 do
          for i2 in r2 do
            yield locAdesc.myStorageArr(i1:stoSzT, i2:stoSzT);
      }
}


pragma "auto copy fn"
proc chpl__autoCopy(x: DimensionalArr) {
  x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: DimensionalArr) {
  var cnt = x.destroyArr();
  if cnt == 0 then
    delete x;
}


//=============================================================================

/////////////////////////////////////////////////////////////////////////////
// Vass's test 1-d distribution - replicated

class vdist {
  // REQ over how many locales
  var numLocales: int;

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
  var localLocIDlegit = false;
}

class vdom {
  // REQ the parameters of our dimension of the domain being created
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);
//todo-remove?  proc domainT type return domain(1, idxType, stridable);

  // our range
  var wholeR: rangeT;

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
  var localLocIDlegit = false;
}

class vlocdom {
  // our copy of wholeR
  var locWholeR;
}


/////////// privatization - start

// REQ does this class support privatization?
proc vdist.dsiSupportsPrivatization1d() param return true;

// REQ if privatization is supported - same purpose as dsiGetPrivatizeData()
proc vdist.dsiGetPrivatizeData1d() {
  return (numLocales,);
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
proc vdist.dsiPrivatize1d(privatizeData) {
  return new vdist(numLocales = privatizeData(1));
}

// REQ does this class need -- and provide -- the localLocID?
// dsiStoreLocalLocID1d() will be invoked on privatized copies
// only when dsiSupportsPrivatization1d is true (obviously).
proc vdist.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
// If 'legit' is false, this is a privatized copy on a locale
// that's not among our Dimensional distribution's target locales.
proc vdist.dsiStoreLocalLocID1d(localLocID: locIdT, legit: bool) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
  this.localLocIDlegit = legit;
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID and its legitimacy
proc vdist.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ does this class support privatization?
proc vdom.dsiSupportsPrivatization1d() param return true;

// REQ if privatization is supported - same purpose as dsiGetPrivatizeData()
proc vdom.dsiGetPrivatizeData1d() {
  return 0; // no data
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
// 'privDist' is the corresponding 1-d distribution descriptor,
// privatized (if it supports privatization).
proc vdom.dsiPrivatize1d(privDist, privatizeData) {
  return new vdom(idxType   = this.idxType,
                  stridable = this.stridable);
}

// REQ if privatization is supported - same purpose as dsiGetReprivatizeData()
proc vdom.dsiGetReprivatizeData1d() {
  return (wholeR,);
}

// REQ if privatization is supported - same purpose as dsiReprivatize()
proc vdom.dsiReprivatize1d(other, reprivatizeData) {
  assert(other.idxType   == this.idxType,
         other.stridable == this.stridable);

  this.wholeR = reprivatizeData(1);
}

// REQ does this class need the localLocID?
// (See also vdist.dsiUsesLocalLocID1d.)
proc vdom.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
proc vdom.dsiStoreLocalLocID1d((localLocID, legit): (locIdT, bool)) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
  this.localLocIDlegit = legit;
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID
proc vdom.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ does a local domain descriptor use a pointer
// to the privatized global domain descriptor?
// Consulted only when dsiSupportsPrivatization1d is true.
proc vdom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

// REQ if dsiLocalDescUsesPrivatizedGlobalDesc1d: store the pointer to
// the privatized copy of the global descriptor that is on vlocdom's locale.
// Note: this is invoked only while privatizing the 1-d global descriptor.
// In particular, this is not invoked when the local descriptors are
// initially created.
// todo: we already pass globDD to all methods on vlocdom, so probably
// we do not need this
//
proc vlocdom.dsiStoreLocalDescToPrivatizedGlobalDesc1d(privGlobDesc) {
  compilerError("should not need dsiStoreLocalDescToPrivatizedGlobalDesc1d");
  // e.g.: this.privGlobDesc = privGlobDesc;
}

/////////// privatization - end


// REQ create a 1-d global domain descriptor for dsiNewRectangularDom()
// where our dimension is a range(idxType, bounded, stridable)
proc vdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  return new vdom(idxType, stridable);
}

// REQ is this a replicated distribution?
proc vdom.dsiIsReplicated() param return true;

// REQ produce a local domain descriptor for our dimension
// on a locale for the given locale index locId, for dsiNewRectangularDom().
// Note: may be invoked multiple times/locales with the same locId.
// - one for each point in the other dimension(s).
proc vdom.dsiNewLocalDom1d(locId: locIdT) {
  return new vlocdom(wholeR);
}

// REQ given our dimension of the array index, on which locale is it located?
proc vdist.dsiIndexToLocale1d(indexx): locIdT {
  return localLocID;
}

// REQ update our data structures as needed upon dsiSetIndices()
proc vdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// REQ update our local data structures as needed upon dsiSetIndices();
// return the subrange that will be handled locally.
// globDD is the global domain descriptor ('vdom' in our case)
proc vlocdom.dsiSetLocalIndices1d(globDD, locId: locIdT): globDD.rangeT {
  locWholeR = globDD.wholeR;
  return locWholeR;
}

// REQ given one dimension of the array index, return
// (locale id, storage index on that locale) for that index
proc vdom.dsiAccess1d(indexx: idxType): (locIdT, stoSzT) {
  assert(localLocIDlegit);
  return (localLocID, wholeR.indexOrder(indexx): stoSzT);
}

// REQ for iterating over array elements.
// Yield tuples (locId, iterable) s.t. the following results in
// traversing all array elements in order:
//   for (l,itr) in dsiSerialArrayIterator1d() do
//    for ix in itr do
//      yield (local storage on locale l)[ix]
iter vdom.dsiSerialArrayIterator1d() {
  assert(localLocIDlegit);
  yield (localLocID, 0..#wholeR.length);
}

// REQ generate the densified range to be used in the leader iterator
// on this descriptor's locale for task 'taskid' of 0..#numTasks.
// Currently the DSI framework computes numTasks based on dsiSetLocalIndices1d
// and handles the case numTasks <= 1.
// Note: should be densified w.r.t. the entire global range in this dimension.
proc vlocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int) {
  const (startIx, endIx) = _computeChunkStartEnd(locWholeR.length,
                                                 numTasks, taskid+1);
  return startIx - 1 .. endIx - 1;
}

// REQ for array follower iterator.
// Same as dsiSerialArrayIterator1d(), except only the elements in
//   unDensify(denseRange, wholeRangeInMyDimension)
// should be traversed, in the order of the unDensify-ed indices.
// The returned range's idxType must be stoSzT.
iter vdom.dsiFollowerArrayIterator1d(denseRange) {
  // The dense indices are just fine here.
  yield (localLocID, denseRange);
}


/////////////////////////////////////////////////////////////////////////////
// Vass's test 1-d distribution - block

class sdist {
  // REQ over how many locales
  const numLocales: int;

  // the type of bbStart, bbLength
  // (also (ab)used as the idxType of the domains created over this dist.)
  // (todo - straighten that out)
  type idxType;

  // the .low and .length of BlockDist's 'boundingBox' for our dimension
  const bbStart: idxType;
  const bbLength: idxType;
}

class sdom {
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range
  var wholeR: rangeT;

  // privatized distribution descriptor
  const pdist;
}

class slocdom {
  // TODO replace 0
  const myLocId: locIdT;
  var myRange;
}


/////////// privatization - start

proc sdist.dsiSupportsPrivatization1d() param return true;

proc sdist.dsiGetPrivatizeData1d() {
  return (numLocales, bbStart, bbLength);
}

proc sdist.dsiPrivatize1d(privatizeData) {
  return new sdist(privatizeData, this.idxType);
}

// constructor for privatization
proc sdist.sdist(privatizeData, type idxType) {
  numLocales = privatizeData(1);
  bbStart = privatizeData(2);
  bbLength = privatizeData(3);
}

proc sdist.dsiUsesLocalLocID1d() param return false;

proc sdom.dsiSupportsPrivatization1d() param return true;

proc sdom.dsiGetPrivatizeData1d() {
  return 0;  // no data
}

proc sdom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new sdom(idxType   = this.idxType,
                  stridable = this.stridable,
                  pdist     = privDist);
}

proc sdom.dsiGetReprivatizeData1d() {
  return (wholeR,);
}

proc sdom.dsiReprivatize1d(other, reprivatizeData) {
  assert(other.idxType   == this.idxType,
         other.stridable == this.stridable);

  this.wholeR = reprivatizeData(1);
}

proc sdom.dsiUsesLocalLocID1d() param return false;

proc sdom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

/////////// privatization - end


// Constructor. idxType is inferred from the 'bbLow' argument
// (alternative: default to 'int' instead).
proc sdist.sdist(nLocales, bbLow, bbHigh, type idxType = bbLow.type) {
  if !(bbLow <= bbHigh) then halt("'sdist' distribution must have a non-empty bounding box between bbLow and bbHigh; got ", bbLow, " .. ", bbHigh);
  this.numLocales = nLocales;
  this.bbStart = bbLow;
  this.bbLength = (bbHigh - bbLow + 1):idxType;
  assert(this.bbLength > 0);
}

proc sdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("The index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the 'sdist' 1-d distribution");
  return new sdom(idxType = idxType, stridable = stridable, pdist = this);
}

proc sdom.dsiIsReplicated() param return false;

proc sdom.dsiNewLocalDom1d(locId: locIdT) {
  var defaultVal: rangeT;
  return new slocdom(myLocId = locId, myRange = defaultVal);
}

proc sdist.dsiIndexToLocale1d(indexx): locIdT {
  const idTemp = (indexx - bbStart) * numLocales / bbLength;
  return max(0, min(numLocales-1, idTemp:int));
}

proc sdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// Helper: compute the chunk to be stored on locID
proc sdom._dsiComputeMyRange(locId): rangeT {
  const dom = this;
  const dist = dom.pdist;
  // see LocBlock.LocBlock()
  const (blo, bhi) = _computeBlock(dist.bbLength, dist.numLocales,
           locId, max(dom.idxType), min(dom.idxType), dist.bbStart);
  const myChunk = {blo..bhi};
  // see Block.getChunk()
  const chunk = myChunk(dom.wholeR);
  return chunk.dim(1);
}

proc slocdom.dsiSetLocalIndices1d(globDD, locId): globDD.rangeT {
  // todo: create local *distribution* descriptors for sdist, and
  // store myChunk there, like it's done in BlockDist?

  // TODO replace 0 DONE
  myRange = globDD._dsiComputeMyRange(locId);
  return myRange;
}

proc sdom.dsiAccess1d(indexx: idxType): (locIdT, stoSzT) {
  const locId = pdist.dsiIndexToLocale1d(indexx);

  // We do not go to slocdom for myRange because (a) recomputing it
  // is probably cheaper than going to a remote locale, and
  // (b) we do not store slocdom objects in sdom.

  const localRange = _dsiComputeMyRange(locId);
  const storageOffset = localRange.indexOrder(indexx);

  // This assert checks (a) that our computations are coherent, and
  // (b) that 'indexx' is a legitimate index.  Not sure about
  // who is responsible for (b) and what to do if it's not legitimate.
  assert(storageOffset >= 0);

  // TODO replace 0 DONE
  return (locId, storageOffset:stoSzT);
}

iter sdom.dsiSerialArrayIterator1d() {
  // By the nature of the Block distribution, the indices are assigned to
  // locales contiguously. But reverse the order if so does our range.
  // On a given locale, storage indices are always increasing -
  // because they are densified i.e. reflect indexOrder(), which always grows.

  for locId in (0..#pdist.numLocales) by sgn(wholeR.stride) {
    // We do not go to slocdom for myRange - see above for rationale.
    const localRange = _dsiComputeMyRange(locId);
    // TODO replace 0 DONE
    yield (locId, 0..#localRange.length);
  }
}

iter sdom.dsiFollowerArrayIterator1d(denseRange) {
  // The dense indices are just fine here.
  // TODO replace 0
  yield (0, denseRange);
}

proc slocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int) {
  const locRange = densify(myRange, globDD.wholeR, userErrors=false);
  // Copied straight from BlockDom leader - replace locBlock(parDim)->locRange.
  const (lo, hi) = _computeBlock(locRange.length, numTasks, taskid,
                                 locRange.high, locRange.low, locRange.low);

writeln(myLocId, "-", taskid, " -wholeR ", globDD.wholeR,
        "  myRange ", myRange, "  densified ", locRange,
        "  result ", lo, "..", hi);

  // It seems to have a reason why this will occasionally be an empty range.
  return lo..hi;
}

//=============================================================================

config const s2 = if numLocales < 3 then numLocales else 3;

var mylocs: [0..0, 0..#s2] locale;
mylocs[0, 0..#s2] = Locales[0..#s2];
writeln("mylocs = ", mylocs);
writeln();

var vdf = new vdist(1);
var sdf = new sdist(3, 1, 8);
var dm = new dmap(new DimensionalDist(mylocs, vdf, sdf, "dm"));

proc test(d) {
  writeln("test ", d);
  const vd = d._value.dom2;
  writeln("  vd = ", vd, "  : ", typeToString(vd.type));
  for i in d.dim(2) {
    writeln("  ", i, "  at ", vd.dsiAccess1d(i));
  }
  writeln();

  forall i in d do writeln(i);
  writeln();
}

var d1 = {1..1, 0..9} dmapped dm;
test(d1);
var d2 = {1..1, 0..9 by -1} dmapped dm;
test(d2);
var d3 = {1..1, 0..9 by -2} dmapped dm;
test(d3);
var d4 = {1..1, 0..9 by 3} dmapped dm;
test(d4);
