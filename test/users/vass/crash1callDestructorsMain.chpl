// This is a two-dimensional Dimensional distribution

//use DSIUtil;

// debugging/trace certain DSI methods as they are being invoked
config param traceDimensionalDist = true;
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


/// the types for ... ///////////////////////////////////////////////////////

type locCntT = uint(32); // ... locale counts
type locIdT =  int(64);  // ... locale ID, i.e., its index in targetLocales
type stoSzT  = uint(32); // ... local storage size and indices (0-based)

param invalidLocID =
  // encode 'max(t)' as a compile-time expression
  (2 ** (numBits(locIdT) - 1 - isIntType(locIdT):bool)):int(64);


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
  proc numLocs1: locCntT  return targetIds.dim(1).size: locCntT;
  proc numLocs2: locCntT  return targetIds.dim(2).size: locCntT;

  // parallelization knobs
  var dataParTasksPerLocale: int      = getDataParTasksPerLocale();
  var dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks();
  var dataParMinGranularity: int      = getDataParMinGranularity();

  // for privatization
  var pid: int = -1;
}

// class LocDimensionalDist - no local distribution descriptor - for now

private proc locDescTypeHelper(dom1, dom2) type {
  return (dom1.dsiNewLocalDom1d(0).type, dom2.dsiNewLocalDom1d(0).type);
}

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
  var whole: domain(rank, idxType, stridable);

  // convenience - our instantiation of LocDimensionalDom
  proc lddTypeArg1 type  return domainT;
  proc lddTypeArg2 type  return (dom1.dsiNewLocalDom1d(0).type,
                                 dom2.dsiNewLocalDom1d(0).type);
  proc locDdescType type  return LocDimensionalDom(lddTypeArg1, lddTypeArg2);

  // local domain descriptors
  var localDdescs : [dist.targetIdx] LocDimensionalDom(domain(rank, idxType, stridable),
                                                       locDescTypeHelper(dom1, dom2));

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
proc DimensionalDist.init(
  targetLocales,
  di1,
  di2,
  name: string = "dimensional distribution",
  type idxType = int,
  dataParTasksPerLocale: int      = getDataParTasksPerLocale(),
  dataParIgnoreRunningTasks: bool = getDataParIgnoreRunningTasks(),
  dataParMinGranularity: int      = getDataParMinGranularity()
) {
  this.targetLocales = targetLocales;
  this.di1 = di1;
  this.di2 = di2;
  this.name = name;
  this.idxType = idxType;
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0 then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  this.complete();

  checkInvariants();

  _passLocalLocIDsDist(di1, di2, this.targetLocales,
                       true, this.targetLocales.domain.low);
}

// Check all things that must be provided by the user
// when constructing a DimensionalDist.
proc DimensionalDist.checkInvariants(): void {
  assert(targetLocales.eltType == locale, "DimensionalDist-targetLocales.eltType");
  assert(targetIds.idxType == locIdT, "DimensionalDist-targetIdx.idxType");
  // todo: where do we rely on this?
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

  _passLocalLocIDsDist(di1new, di2new, privTargetLocales, false, 0);

  return new DimensionalDist(targetLocales = privTargetLocales,
                             name          = privatizeData(2),
                             idxType       = this.idxType,
                             dataParTasksPerLocale     = privatizeData(3),
                             dataParIgnoreRunningTasks = privatizeData(4),
                             dataParMinGranularity     = privatizeData(5),
                             di1   = di1new,
                             di2   = di2new,
                             dummy = 0);
}

// constructor of a privatized copy
// (currently almost same the user constructor; 'dummy' is to distinguish)
proc DimensionalDist.init(param dummy: int,
  targetLocales,
  di1,
  di2,
  name,
  type idxType,
  dataParTasksPerLocale,
  dataParIgnoreRunningTasks,
  dataParMinGranularity
) {
  this.targetLocales = targetLocales;
  this.di1 = di1;
  this.di2 = di2;
  this.name = name;
  this.idxType = idxType;
  this.dataParTasksPerLocale     = dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity     = dataParMinGranularity;

  this.complete();

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
                  indexx.type:string);

  return targetLocales(di1.dsiIndexToLocale1d(indexx(1), (this, 1)):int,
                       di2.dsiIndexToLocale1d(indexx(2), (this, 2)):int);
}

// Where, in the dimension 'dim', is 'here' located? (for dsiIndexLocale1d)
// todo: could implement using a hashtable/a replicated field/privatization?
// todo: questionable when over-subscribing the locales
//       (maybe in that case it's a "doesn't matter" ?)
proc dsiIndexCurrentLocale1d((dimMultiD, dim): (?, int)): locIdT {
  return _dsiIndexCurrentLocale1dHelper(dimMultiD, dim);
}

proc _dsiIndexCurrentLocale1dHelper(dd: DimensionalDist, dim: int) {
  for (ix, loc) in zip(dd.targetIds, dd.targetLocales) do
    if loc == here then
      return ix(dim);
  halt("DimensionalDist: the current locale ", here,
       " is not among the target locales ", dd.targetLocales);
  return -1;
}

proc _dsiIndexCurrentLocale1dHelper(dd: DimensionalDom, dim: int) {
  return _dsiIndexCurrentLocale1dHelper(dd.dist, dim);
}

proc _dsiIndexCurrentLocale1dHelper(dd: DimensionalArr, dim: int) {
  return _dsiIndexCurrentLocale1dHelper(dd.dom.dist, dim);
}

// Similar to dsiIndexCurrentLocale1d(), but receives as arguments
// (a) the locale array and (b) optional hint where to start looking.
//
// We are justified, sort-of, to run linear search because when it is invoked
// the targetLocales array is assigned, so this is at most a constant
// factor of extra time on top of that.
//
proc _CurrentLocaleToLocIDs(targetLocales): targetLocales.rank * locIdT
{
  var result: targetLocales.rank * locIdT;
  // gotta lock 'result' to ensure atomic update
  var gotresult$: sync bool = false;
  forall (lls, loc) in zip(targetLocales.domain, targetLocales)
    with (ref result)
  do
    if loc == here then
      // if we get multiple matches, we do not specify which is returned
      if !gotresult$.readXX() { // cheap pre-test
        gotresult$;
        result = lls;
        gotresult$ = true;
      }
  if !gotresult$.readXX() then
    halt("DimensionalDist: the current locale ", here,
         " is not among the target locales ", targetLocales);
  return result;
}

// How we usually invoke _CurrentLocaleToLocIDs().
proc _passLocalLocIDsDist(d1, d2, targetLocales, param gotHint, hint): void {
  if d1.dsiNeedLocalLocID1d() || d2.dsiNeedLocalLocID1d() {
    const locIds: targetLocales.rank * locIdT =
      if gotHint && targetLocales(hint) == here
        then hint
      else
        _CurrentLocaleToLocIDs(targetLocales);

    if d1.dsiNeedLocalLocID1d() then d1.dsiStoreLocalLocID1d(locIds(1));
    if d2.dsiNeedLocalLocID1d() then d2.dsiStoreLocalLocID1d(locIds(2));
  }
}

proc _passLocalLocIDsDom(dimDom): void
{
  proc helper(dom1d, dist1d) {
    if dom1d.dsiNeedLocalLocID1d() {
      if !dist1d.dsiNeedLocalLocID1d() then
        compilerError("DimensionalDist: currently, when a subordinate 1d distribution requires localLocID for *domain* descriptors, it must also require them for *distribution* descriptors");
      dom1d.dsiStoreLocalLocID1d(dist1d.dsiGetLocalLocID1d());
    }
  }
  helper(dimDom.dom1, dimDom.dist.di1);
  helper(dimDom.dom2, dimDom.dist.di2);
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
  proc dom1new   return if dom1.dsiSupportsPrivatization1d()
    then dom1orig.dsiPrivatize1d(dom1pd) else dom1orig;

  proc dom2orig  return privatizeData(4);
  proc dom2pd    return privatizeData(5);
  proc dom2new   return if dom2.dsiSupportsPrivatization1d()
    then dom2orig.dsiPrivatize1d(dom1pd) else dom2orig;

  const result = new DimensionalDom(rank      = this.rank,
                                    idxType   = this.idxType,
                                    stridable = this.stridable,
                                    dist = privdist,
                                    dom1 = dom1new,
                                    dom2 = dom2new);
  result.localDdescs = privatizeData(6);
  _passLocalLocIDsDom(result);

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

proc DimensionalDom.dsiDims()     return whole.dims();

proc DimensionalDom.dsiNumIndices return whole.size;

proc DimensionalDom.subordinate1dDist(param dim: int) {
  return dist.subordinate1dDist(dim);
}


//== writing

proc DimensionalDom.dsiSerialWrite(f): void {
  f.write(whole);
}


//== creation, SetIndices

// create a new domain mapped with this distribution
proc DimensionalDist.dsiNewRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool,
                                          inds)
//  : DimensionalDom(rank, idxType, stridable, this.type, ...)
{
  _traceddd(this, ".dsiNewRectangularDom ",
            (rank, idxType:string, stridable, inds));
  if rank != 2 then
    compilerError("DimensionalDist presently supports only 2 dimensions,",
                  " got ", rank, " dimensions");

  if idxType != this.idxType then
    compilerError("The domain index type ", idxType:string,
                  " does not match the index type ",this.idxType:string,
                  " of the DimensionalDist used to map that domain");
  if rank != this.rank then
    compilerError("The rank of the domain (", rank,
                  ") does not match the rank (", this.rank,
                  ") of the DimensionalDist used to map that domain");
  
  const dom1 = di1.dsiNewRectangularDom1d(idxType, stridable);
  const dom2 = di2.dsiNewRectangularDom1d(idxType, stridable);

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
  _passLocalLocIDsDom(result);
  result.dsiSetIndices(inds);

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
  myStorageDom = {0:stoSzT..#myRange1.size:stoSzT,
                  0:stoSzT..#myRange2.size:stoSzT};

  _traceddd("DimensionalDom.dsiSetIndices on ", here.id, " ", locId, " <- ",
           myBlock, "  storage ", myRange1.size, "*", myRange2.size);
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
                  indexx.type:string);

  const (l1,i1):(locIdT, stoSzT) = dom.dom1.dsiAccess1d(indexx(1), (this, 1));
  const (l2,i2):(locIdT, stoSzT) = dom.dom2.dsiAccess1d(indexx(2), (this, 2));
  const locAdesc = localAdescs[l1,l2];
  //writeln("  locales ", (l1,l2), "  storage ", (i1,i2));
  return locAdesc.myStorageArr(i1,i2);
}


//== writing

proc DimensionalArr.dsiSerialWrite(f): void {
  const dom = this.dom;
  _traceddd(this, ".dsiSerialWrite on ", here.id);
  assert(dom.rank == 2);

  // we largely follow DimensionalArr.these()
  // instead could just use BlockArr.dsiSerialWrite
  if dom.dsiNumIndices == 0 then return;

  var nextD1 = false;
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d((this, 1)) do
    for i1 in r1 {
      if nextD1 then f.writeln();
      nextD1 = true;

      var nextD2 = false;
      for (l2,r2) in dom.dom2.dsiSerialArrayIterator1d((this, 2)) do
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
  // instead of this plus 'myDims = locDdesc.myBlock.dims()' below.
  proc helpTargetIds(dom1d, param dd) {
    if dom1d.dsiIsReplicated() {
      // not writing 'ix..#1' because it seems inefficient for this simple use
      const ix = _dsiIndexCurrentLocale1dHelper(this, dd);
      return ix..ix;
    } else
      return dist.targetIds.dim(dd);
  }
  const overTargetIds = if dom1.dsiIsReplicated() || dom2.dsiIsReplicated()
    then {helpTargetIds(dom1,1), helpTargetIds(dom2,2)}
    else dist.targetIds; // this case is here for efficiency

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
      const (numTasks, parDim) =
        // a debugging hook
        if fakeDimensionalDistParDim <= 0 then
          _computeChunkStuff(maxTasks, ignoreRunning, minSize, myDims)
        else
          ( min(_computeNumChunks(maxTasks, ignoreRunning, minSize,
                                  locDdesc.myBlock.size),
                locDdesc.myBlock.dim(fakeDimensionalDistParDim).size):int,
            fakeDimensionalDistParDim:int);

      // parDim gotta point to one of the dimensions that we have
      assert(numTasks == 0 || (1 <= parDim && parDim <= rank));

      if numTasks == 0 then
        _traceddc(traceDimensionalDist || traceDimensionalDistIterators,
                  "  leader on ", here.id, " ", lls, " - no tasks");

      if numTasks == 1 then {
        const follow = computeZeroBasedRanges(myDims);
        // Actually myStorageDom.dims() is such a densification.
        assert(follow == locDdesc.myStorageDom.dims());

        _traceddc(traceDimensionalDistIterators,
                  "  leader on ", lls, " single task -> ", follow);

        // todo: change to 'yield locDdesc.myStorageDom.dims()'
        // to avoid computing 'follow'; challenge: typechecker
        yield follow;

      } else {
        coforall taskid in 0..#numTasks {
          var follow = computeZeroBasedRanges(myDims);

          // split the range along parDim-th dimension

          // gotta have a param index into local1dDdescs
          // TODO: make local1dDdescs homogenous (introduce a superclass
          // of subordinate 1-d local domain descriptors?)
          //
          for param dd in 1..rank do if dd == parDim {

            const myPiece = locDdesc.local1dDdescs(dd).
              dsiMyDensifiedRangeForTaskID1d(taskid, numTasks);

            // ensure we got a subset
            assert(follow(dd)(myPiece) == myPiece);

            // Similar to the assert 'lo <= hi' in BlockDom leader.
            // Upon a second thought, if there is a legitimate reason
            // why dsiMyDensifiedRangeForTaskID1d() does not agree
            // with _computeChunkStuff (i.e. the latter returns more tasks
            // than the former wants to use) - fine. Then, replace assert with
            //   if myPiece.size == 0 then do not yield anything
            assert(myPiece.size > 0);

            // apply myPiece
            follow(dd) = myPiece;
          } // for dd

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


//== serial iterator - array

// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() ref {
  const dom = this.dom;
  _traceddd(this, ".serial iterator");
  assert(dom.rank == 2);

  // TODO: is this the right approach?
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d((this, 1)) do
    for (l2,r2) in dom.dom2.dsiSerialArrayIterator1d((this, 2)) do
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
  for (l1,r1) in dom.dom1.dsiFollowerArrayIterator1d(followThis(1), (this, 1)) do
    for (l2,r2) in dom.dom2.dsiFollowerArrayIterator1d(followThis(2), (this, 2)) do
      {
        const locAdesc = localAdescs[l1,l2];
        _traceddc(traceDimensionalDistIterators,
                  "  locAdesc", (l1,l2), " on ", locAdesc.locale);
        for i1 in r1 do
          for i2 in r2 do
            yield locAdesc.myStorageArr(i1:stoSzT, i2:stoSzT);
      }
}

//=============================================================================

config const s1 = 2;
config const s2 = 2;

use crash1callDestructorsAddon;
var pfx = " 00  ";
var phase = 0;

/////////// locales

var mylocs: [0..#s1,0..#s2] locale;
var manylocs = true;

if numLocales >= s1 * s2 {
  var i = 0;
  for ml in mylocs { ml = Locales(i); i += 1; }
} else {
  msg("oversubscribing Locales(0)");
  mylocs = Locales(0);
  manylocs = false;
}
hd("mylocs");
writeln(mylocs);
tl();

/////////// helpers

// if oversubscribing a single locale, yield that one.
// Otherwise yield each of the distinct locales.
iter overLocales(): locale {
  if (manylocs) then
    for l in mylocs do yield l;
  else
    yield here;
}

// to preserve coarse ordering ("phases") when sorting the output
proc nextphase() {
  phase += 1;
  pfx = (if phase < 10 then " 0" else " ") + phase:string + "  ";
  traceDimensionalDistPrefix = pfx;
}

// heading/start new "phase"
type seena = [1..1] bool;
proc hd(args...) { nextphase(); writeln(pfx, (...args)); }
proc hhd(args...) { nextphase(); writeln(pfx, (...args), " on ", here.id); }
// additional messages
proc msg(args...) { writeln(pfx, (...args)); }
proc msgp() { write(pfx); }
proc msgNext(seen: seena, item) {
  if ! seen[1] { msgp(); seen[1] = true; }
  write(item);
}
// tail - finish a "phase"
proc tl()         { writeln(pfx); }

/////////// distribution

var vdf = new vdist(s1, "v1");
var sdf = new sdist(s2, "s2");
var ddf = new DimensionalDist(mylocs, vdf, sdf, "ddf");

var dilTest = (2,3);
hd(dilTest, " is on locale:");
for l in Locales do
  on l do
    msg(l, ": ", ddf.dsiIndexToLocale((2,3)));
tl();

/////////// domain

const dmbase = {1..3,1..4};

hd("dmdom - creating");
var dmdom: domain(2) dmapped new dmap(ddf);
tl();

hd("dmdom - assigning");
dmdom = dmbase;
msg("dmdom = ", dmdom);
tl();

for l in overLocales() do on l do showDom();

proc showDom() {
  hhd("dmdom - serial iteration");
  var seen: seena;
  for i in dmdom do msgNext(seen, i); writeln();
  tl();

  hhd("dmdom - parallel iteration");
  forall i in dmdom do msg(" ", i, "  on ", here.id);
  tl();
}

/////////// array

hd("dmarr - creating");
var dmarr: [dmdom] int;
tl();

var dmhelp: [dmbase] dmarr.eltType;

hd("dmarr - initializing");
// note this updates only *one* replicated copy
for (i,j) in dmdom {
  dmarr[i,j] = i*10 + j;
}
tl();

hd("dmarr - writeln()");
writeln(dmarr);
tl();

for l in overLocales() do on l do showArr();

proc showArr() {
  hhd("dmarr - serial iteration");
  var seen: seena;
  for elm in dmarr do msgNext(seen, elm); writeln();
  tl();

  hhd("dmarr - parallel iteration");
  forall elm in dmarr do
    msg(" ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr - assignment from");
  dmhelp = dmarr;
  tl();
  
  hd("assignment result");
  writeln(dmhelp);
  tl();

  //writeln(dmarr); -- on Locale 0
  //dmarr.writeThis(stdout); writeln(); -- works by chance

  hhd("dmdom, dmarr - zippered iteration");
  forall (ix,elm) in zip(dmdom,dmarr) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr, dmdom - zippered iteration");
  forall (elm,ix) in zip(dmarr,dmdom) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();
}

hd("dmarr - subordinate1dDist()");
msg(dmarr._value.subordinate1dDist(1));
msg(dmarr._value.subordinate1dDist(2));
tl();


/////////////////////////////////
// Vass's test 1-d distribution - replicated

class vdist {
  // REQ over how many locales
  var numLocales: int;

  // for debugging/tracing
  var name: string;

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
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
  return (numLocales, name);
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
proc vdist.dsiPrivatize1d(privatizeData) {
  return new vdist(numLocales = privatizeData(1),
                   name       = privatizeData(2));
}

// REQ does this class need the localLocID?
proc vdist.dsiNeedLocalLocID1d() param return true;

// REQ if dsiNeedLocalLocID1d: store the localLocID
proc vdist.dsiStoreLocalLocID1d(localLocID: locIdT) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
}

// REQ if dsiNeedLocalLocID1d: retrieve the localLocID
proc vdist.dsiGetLocalLocID1d(): locIdT {
  assert(this.localLocID != invalidLocID);
  return this.localLocID;
}

// REQ does this class support privatization?
proc vdom.dsiSupportsPrivatization1d() param return true;

// REQ if privatization is supported - same purpose as dsiGetPrivatizeData()
proc vdom.dsiGetPrivatizeData1d() {
  return 0; // no data
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
proc vdom.dsiPrivatize1d(privatizeData) {
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
proc vdom.dsiNeedLocalLocID1d() param return true;

// REQ if dsiNeedLocalLocID1d: store the localLocID
proc vdom.dsiStoreLocalLocID1d(localLocID: locIdT) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
}

// for internal use - to add assert() seamlessly
proc vdom.dsiGetLocalLocID1d(): locIdT {
  assert(this.localLocID != invalidLocID);
  return this.localLocID;
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
// CLargs is the arguments to dsiIndexCurrentLocale1d(), should it be needed
proc vdist.dsiIndexToLocale1d(indexx, CLargs): locIdT {
  return dsiIndexCurrentLocale1d(CLargs);
}

// REQ update our data structures as needed upon dsiSetIndices()
proc vdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// REQ update our local data structures as needed upon dsiSetIndices();
// return the subrange that will be handled locally
proc vlocdom.dsiSetLocalIndices1d(globDD, locId: locIdT): globDD.rangeT {
  locWholeR = globDD.wholeR;
  return locWholeR;
}

// REQ given one dimension of the array index, return
// (locale id, storage index on that locale) for that index
proc vdom.dsiAccess1d(indexx: idxType, CLargs): (locIdT, stoSzT) {
  return (dsiIndexCurrentLocale1d(CLargs),
          wholeR.indexOrder(indexx): stoSzT);
}

// REQ for iterating over array elements.
// Yield tuples (locId, iterable) s.t. the following results in
// traversing all array elements in order:
//   for (l,itr) in dsiSerialArrayIterator1d() do
//    for ix in itr do
//      yield (local storage on locale l)[ix]
// CLargs is the same as above
iter vdom.dsiSerialArrayIterator1d(CLargs) {
  yield (dsiIndexCurrentLocale1d(CLargs),
         0..#wholeR.size);
}

// REQ generate the densified range to be used in the leader iterator
// on this descriptor's locale for task 'taskid' of 0..#numTasks.
// Currently the DSI framework computes numTasks based on dsiSetLocalIndices1d
// and handles the case numTasks <= 1.
// Note: should be densified w.r.t. the entire global range in this dimension.
proc vlocdom.dsiMyDensifiedRangeForTaskID1d(taskid:int, numTasks:int) {
  const (startIx, endIx) = _computeChunkStartEnd(locWholeR.size,
                                                 numTasks, taskid+1);
  return startIx - 1 .. endIx - 1;
}

// REQ for array follower iterator.
// Same as dsiSerialArrayIterator1d(), except only the elements in
//   unDensify(denseRange, wholeRangeInMyDimension)
// should be traversed, in the order of the unDensify-ed indices.
// The returned range's idxType must be stoSzT.
iter vdom.dsiFollowerArrayIterator1d(denseRange, CLargs) {
  // The dense indices are just fine here.
  yield (dsiIndexCurrentLocale1d(CLargs), denseRange);
}


/////////////////////////////////
// Vass's test 1-d distribution - block

class sdist {
  // REQ over how many locales
  var numLocales: int;

  // for debugging/tracing
  var name: string;

  // the type of indices that we handle
  type idxType = int;
}

class sdom {
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range
  var wholeR: rangeT;
}

class slocdom {
  // for now, store everything on locale with id=0
  // TODO replace 0
  var myLocID: locIdT;
  var myRange;
}

/////////// privatization - start

proc sdist.dsiSupportsPrivatization1d() param return true;

proc sdist.dsiGetPrivatizeData1d() {
  return (numLocales, name);
}

proc sdist.dsiPrivatize1d(privatizeData) {
  return new sdist(numLocales = privatizeData(1),
                   name       = privatizeData(2),
                   idxType = this.idxType);
}

proc sdist.dsiNeedLocalLocID1d() param return false;

proc sdom.dsiSupportsPrivatization1d() param return true;

proc sdom.dsiGetPrivatizeData1d() {
  return 0;  // no data
}

proc sdom.dsiPrivatize1d(privatizeData) {
  return new sdom(idxType   = this.idxType,
                  stridable = this.stridable);
}

proc sdom.dsiGetReprivatizeData1d() {
  return (wholeR,);
}

proc sdom.dsiReprivatize1d(other, reprivatizeData) {
  assert(other.idxType   == this.idxType,
         other.stridable == this.stridable);

  this.wholeR = reprivatizeData(1);
}

proc sdom.dsiNeedLocalLocID1d() param return false;


/////////// privatization - end

proc sdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("The index type ", idxType:string,
                  " does not match the index type ",this.idxType:string,
                  " of the 'sdist' 1-d distribution");
  return new sdom(idxType = idxType, stridable = stridable);
}

proc sdom.dsiIsReplicated() param return false;

proc sdom.dsiNewLocalDom1d(locId: locIdT) {
  var defaultVal: rangeT;
  return new slocdom(locId, defaultVal);
}

proc sdist.dsiIndexToLocale1d(indexx: idxType, CLargs): locIdT {
  // TODO replace 0
  return 0;
}

proc sdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

proc slocdom.dsiSetLocalIndices1d(globDD, locId): globDD.rangeT {
  myLocID = locId;
  // TODO replace 0
  myRange = if locId == 0 then globDD.wholeR else 1..0;
  return myRange;
}

proc sdom.dsiAccess1d(indexx: idxType, CLargs): (locIdT, stoSzT) {
  // TODO replace 0
  return (0:locIdT,
          wholeR.indexOrder(indexx): stoSzT);
}

iter sdom.dsiSerialArrayIterator1d(CLargs) {
  // TODO replace 0
  yield (0, 0..#wholeR.size);
}

iter sdom.dsiFollowerArrayIterator1d(denseRange, CLargs) {
  // The dense indices are just fine here.
  // TODO replace 0
  yield (0, denseRange);
}

proc slocdom.dsiMyDensifiedRangeForTaskID1d(taskid:int, numTasks:int) {
  // TODO replace 0
  if myLocID == 0 {
    // NB for now we can densify w.r.t. myRange (i.e. self), but will need to
    // densify w.r.t. the entire global range once we distribute things.
    const (startIx, endIx) = _computeChunkStartEnd(myRange.size,
                                                   numTasks, taskid+1);
    return startIx - 1 .. endIx - 1;
  } else {
    return 1..0;
  }
}
