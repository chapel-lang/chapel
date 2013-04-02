/////////////////////////////////////////////////////////////////////////////
// === dimensional-distribution file ===

//use DSIUtil;

// trace certain DSI methods as they are being invoked
config param traceDimensionalDist = true;
config param traceDimensionalDistDsiAccess = false;


/// the types for ... ///////////////////////////////////////////////////////

type locCntT = uint(32); // ... locale counts
type locIdxT =  int(64); // ... indexing into targetLocales
type stoSzT  = uint(32); // ... local storage size and indices (0-based)


/// class declarations //////////////////////////////////////////////////////

class DimensionalDist : BaseDist {
  // the desired locales to distribute things over;
  // must be a [domain(2, locIdxT, false)] locale
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
  var dist;

  // convenience
  proc rangeT  type  return range(idxType, BoundedRangeType.bounded, stridable);
  proc domainT type  return domain(rank, idxType, stridable);
  proc indexT  type  return dist.indexT;

  // subordinate 1-d global domain descriptors
  var dom1, dom2;

  // this is our index set; we store it here so we can get to it easily
  // although it is not necessary, strictly speaking
  var whole: domainT;

  // convenience - our instantiation of LocDimensionalDom
  proc lddTypeArg1 type  return domainT;
  proc lddTypeArg2 type  return (dom1.dsiNewLocalDom1d(0).type,
                                 dom2.dsiNewLocalDom1d(0).type);
  proc locDdescType type  return LocDimensionalDom(lddTypeArg1, lddTypeArg2);

  // local domain descriptors
  var localDdescs: [dist.targetIds] locDdescType;
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
}

class LocDimensionalArr {
  type eltType;
  const locDom;  // a LocDimensionalDom
  var myStorageArr: [locDom.myStorageDom] eltType;
}


/// distribution ////////////////////////////////////////////////////////////


/////////// construction, cloning

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
}

proc DimensionalDist.checkInvariants(): void {
  assert(targetLocales.eltType == locale, "DimensionalDist-eltType");
  assert(targetIds._value.idxType == locIdxT, "DimensionalDist-idxType");
  // todo: where do we rely on this?
  assert(targetIds == {0..#numLocs1/*:locIdxT*/, 0..#numLocs2/*:locIdxT*/},
         "DimensionalDist-targetIds");
  assert(rank == targetLocales.rank, "DimensionalDist-rank");
  assert(dataParTasksPerLocale > 0, "DimensionalDist-dataParTasksPerLocale");
  assert(dataParMinGranularity > 0, "DimensionalDist-dataParMinGranularity");
}

proc DimensionalDist.dsiClone(): this.type {
  if traceDimensionalDist then writeln("DimensionalDist.dsiClone");
  checkInvariants();

  // do this until need something else
  return this;
}


/////////// privatization is not implemented currently

proc DimensionalDist.dsiSupportsPrivatization() param return false;


/////////// index to locale

//Given an index, this should return the locale that owns that index.
proc DimensionalDist.dsiIndexLocale(indexx: indexT): locale {
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist presently supports only indexing with",
                  " 2-tuples; got an index of the type ",
                  typeToString(indexx.type));

  return targetLocales(di1.dsiIndexLocale1d(indexx(1), (this, 1)):int,
                       di2.dsiIndexLocale1d(indexx(2), (this, 2)):int);
}

// Where, in the dimension 'dim', is 'here' located? (for dsiIndexLocale1d)
// todo: could implement using a hashtable/a replicated field/privatization?
// todo: questionable when over-subscribing the locales
//       (maybe in that case it's a "doesn't matter" ?)
proc dsiIndexCurrentLocale1d((dimMultiD, numDim): (?, int)): locIdxT {
  return _dsiIndexCurrentLocale1dHelper(dimMultiD, numDim);
}

proc _dsiIndexCurrentLocale1dHelper(dd: DimensionalDist, dim: int) {
  for (ix, loc) in zip(dd.targetIds, dd.targetLocales) do
    if loc == here then
      return ix(dim);
  halt("DimensionalDist: the current locale ", here,
       " is not among the target locales ", dd.targetLocales);
  return -1;
}

proc _dsiIndexCurrentLocale1dHelper(dd: DimensionalArr, dim: int) {
  return _dsiIndexCurrentLocale1dHelper(dd.dom.dist, dim);
}


/// domain //////////////////////////////////////////////////////////////////


/////////// privatization is not implemented currently

proc DimensionalDom.dsiSupportsPrivatization() param return false;


/////////// miscellanea

proc DimensionalDom.dsiMyDist() return dist;

proc DimensionalDom.dsiNumIndices return whole.numIndices;


/////////// writing

proc DimensionalDom.dsiSerialWrite(f: Writer): void {
  f.write(whole);
}


/////////// creation, SetIndices

// create a new domain mapped with this distribution
proc DimensionalDist.dsiNewRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool)
//  : DimensionalDom(rank, idxType, stridable, this.type, ...)
{
  if traceDimensionalDist then
    writeln("DimensionalDist(", name, ").dsiNewRectangularDom ",
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
  if traceDimensionalDist then
    writeln("DimensionalDom(", dist.name, ").dsiSetIndices ", newRanges);
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
  // we query the *global* subordinate 1-d domain descriptor
  var myRange1 = local1dDdescs(1).dsiSetLocalIndices1d(globDD(1),locId(1));
  var myRange2 = local1dDdescs(2).dsiSetLocalIndices1d(globDD(2),locId(2));

  myBlock = {myRange1, myRange2};
  myStorageDom = {0:stoSzT..#myRange1.length:stoSzT,
                  0:stoSzT..#myRange2.length:stoSzT};

  if traceDimensionalDist then
    writeln("DimensionalDom.dsiSetIndices ", here, " ", locId, " <- ", myBlock,
            "  storage ", myRange1.length, "*", myRange2.length);
}

proc DimensionalDom.dsiGetIndices(): domainT {
  if traceDimensionalDist then
    writeln("DimensionalDom(", dist.name, ").dsiGetIndices");
  return whole;
}


/////////// iterators - domain

// serial iterator
iter DimensionalDom.these() {
  if traceDimensionalDist then
    writeln("DimensionalDom(", dist.name, ").serial iterator");
  for ix in whole do
    yield ix;
}

/*
TODO - parallel iterators
(follower should be the same as everyone else's)

iter DimensionalDom.these(param tag: iterKind) where tag == iterKind.leader {
  coforall locDom in localDoms do
    on locDom do
      // for simplicity, redirect to DefaultRectangular's leader/follow
      for follow in locDom.domLocalRep._value.these(tag) do
        yield follow;
}

iter DimensionalDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  // redirect to DefaultRectangular
  // note: 'localDoms[here.id].domLocalRep' would not save any comm
  //       because have to go to DimensionalDom's locale for 'localDoms' anyway
  for i in whole._value.these(tag, followThis) do
    yield i;
}
*/


/// array ///////////////////////////////////////////////////////////////////


/////////// privatization is currently not implemented

proc DimensionalArr.dsiSupportsPrivatization() param return false;


/////////// miscellanea

proc DimensionalArr.idxType type return dom.idxType; // (could be a field)

proc DimensionalArr.dsiGetBaseDom() return dom;


/////////// creation

// create a new array over this domain
proc DimensionalDom.dsiBuildArray(type eltType)
  : DimensionalArr(eltType, this.type)
{
  if traceDimensionalDist then
    writeln("DimensionalDom(", dist.name, ").dsiBuildArray");
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


/////////// dsiAccess

proc DimensionalArr.dsiAccess(indexx: dom.indexT) var: eltType {
  const dom = this.dom;
  if traceDimensionalDist && traceDimensionalDistDsiAccess then
    writeln("DimensionalArr(", dom.dist.name, ").dsiAccess", indexx);
  if !isTuple(indexx) || indexx.size != 2 then
    compilerError("DimensionalDist presently supports only indexing with",
                  " 2-tuples; got an array index of the type ",
                  typeToString(indexx.type));

  const (l1,i1):(locIdxT, stoSzT) = dom.dom1.dsiAccess1d(indexx(1), (this, 1));
  const (l2,i2):(locIdxT, stoSzT) = dom.dom2.dsiAccess1d(indexx(2), (this, 2));
  const locAdesc = localAdescs[l1,l2];
  //writeln("  locales ", (l1,l2), "  storage ", (i1,i2));
  return locAdesc.myStorageArr(i1,i2);
}


/////////// iterators - array

// serial iterator
// note: no 'on' clauses - they not allowed by the compiler
iter DimensionalArr.these() var {
  const dom = this.dom;
  if traceDimensionalDist then
    writeln("DimensionalArr(", dom.dist.name, ").serial iterator");
  assert(dom.rank == 2);

  // REVIEW: is this the right approach?
  // e.g. is it right that the *global* subordinate 1-d descriptors are used?
  for (l1,r1) in dom.dom1.dsiSerialArrayIterator1d((this, 1)) do
    for (l2,r2) in dom.dom2.dsiSerialArrayIterator1d((this, 2)) do
      {
        const locAdesc = localAdescs[l1,l2];
        //writeln("  locAdesc", (l1,l2), " on ", locAdesc.locale);
        for i1 in r1 do
          for i2 in r2 do
            yield locAdesc.myStorageArr(i1,i2);
      }
}


/////////// writing

proc DimensionalArr.dsiSerialWrite(f: Writer): void {
  const dom = this.dom;
  if traceDimensionalDist then
    writeln("DimensionalArr(", dom.dist.name, ").dsiSerialWrite", " on ",here);
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
          const elem = locAdesc.myStorageArr(i1,i2);
          if nextD2 then f.write(" ");
          f.write(elem);
          nextD2 = true;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

// leader iterator - domain

iter DimensionalDom.these(param tag: iterKind) where tag == iterKind.leader {
  if traceDimensionalDist then
    writeln("DimensionalDom(", dist.name, ").leader iterator");
  assert(rank == 2);

  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;

  // Bug note: currently crashes: 'error: assertion error [iterator.cpp:120]'.
  // If the following coforall is replaced with
  //   coforall locDdesc in localDdescs do
  // then the crash goes away.
  //
  coforall ((l1,l2), locDdesc) in zip(dist.targetIds, localDdescs) do
    on locDdesc {
      // mimic BlockDom leader

      // For now we handle almost everything ourselves. I.e. we consult the
      // subordinate 1-d distribution only for parDim only when numTasks>1.
      // We assume that myBlock already has most of what we need to know.
      // The only reason for doing so is simplicity (e.g. make the interface to
      // the subordinate 1-d distributions smaller). Change this if needed.

      const myDims = locDdesc.myBlock.dims();
      const (numTasks, parDim) = _computeChunkStuff(maxTasks,
                 ignoreRunning, minSize, myDims);
      // parDim gotta point to one of the dimensions that we have
      assert(numTasks == 0 || (1 <= parDim && parDim <= rank));

      if numTasks == 1 then {
        const follow = computeZeroBasedRanges(myDims);
        // Actually myStorageDom.dims() is such a densification,
        // but I am not sure the index type will work.
        assert(follow == locDdesc.myStorageDom.dims());

        if traceDimensionalDist then
          writeln("  leader on ", (l1,l2), " single task -> ", follow);

        // change to 'yield follow;' if needed to make it work
        yield follow; //TODO: locDdesc.myStorageDom.dims();

      } else {
        coforall taskid in 0..#numTasks {
          var follow = computeZeroBasedRanges(myDims);

          // split the range along parDim-th dimension

          // gotta have a param index into local1dDdescs
          // TODO: make local1dDdescs homogenous (introduce a superclass
          // of subordinate 1-d local domain descriptors?)
          //
          for param ti in 1..rank do if ti == parDim {

            const myPiece = locDdesc.local1dDdescs(ti).
              dsiMyDensifiedRangeForTaskID1d(taskid, numTasks);

            // ensure we got a subset
            assert(follow(ti)(myPiece) == myPiece);

            // An assert to 'lo <= hi' in BlockDom leader.
            // Upon a second thought, if there is a legitimate reason
            // why dsiMyDensifiedRangeForTaskID1d() does not agree
            // with _computeChunkStuff (i.e. the latter returns more tasks
            // than the former wants to use) - fine. Then, convert this assert
            // to 'if myPiece.length == 0 then do not yield anything'.
            assert(myPiece.length > 0);

            // apply myPiece
            follow(ti) = myPiece;
          } // for ti

          if traceDimensionalDist then
            writeln("  leader on ", (l1,l2), " task ", taskid, "/", numTasks,
                    " -> ", follow);

          yield follow;
        }
      } // if numTasks
    } // coforall ... on locDdesc
} // DimensionalDom.these leader

iter DimensionalDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  writeln("DimensionalDom follower: got ", followThis);
  for i in {(...followThis)} do
    yield i;
}


/////////////////////////////////////////////////////////////////////////////
// === driver file ===

config const s1 = 2;
config const s2 = 2;

/////////// locales

var mylocs: [0..#s1,0..#s2] locale;
var manylocs = true;

if numLocales >= s1 * s2 {
  var i = 0;
  for ml in mylocs { ml = Locales(i); i += 1; }
} else {
  writeln("oversubscribing Locales(0)");
  mylocs = Locales(0);
  manylocs = false;
}
writeln("mylocs = ", mylocs);
writeln();

/////////// distribution

var vdf = new vdist(s1, "v1");
var sdf = new sdist(s2, "s2");
var ddf = new DimensionalDist(mylocs, vdf, sdf, "ddf");

var dilTest = (2,3);
writeln(dilTest, " is on locale:");
for l in Locales do
  on l do
    writeln(l, ": ", ddf.dsiIndexLocale((2,3)));
writeln();

/////////// domain

const dmbase = {1..3,1..4};
var dmdom: domain(2) dmapped new dmap(ddf);
dmdom = dmbase;
writeln("dmdom = ", dmdom);
writeln();

/////////// array

var dmarr: [dmdom] int;
var dmhelp: [dmbase] dmarr.eltType;
writeln("dmarr is created");
writeln();

// note this updates only one replicated copy
for (i,j) in dmdom {
  dmarr[i,j] = i*10 + j;
}

proc showArrLocales() {
  writeln("traversing dmarr on ", here);
  for elm in dmarr do write(" ", elm); writeln();

  dmhelp = dmarr;
  writeln(dmhelp);
  //writeln(dmarr); -- on Locale 0
  //dmarr.writeThis(stdout); writeln(); -- works by chance

  for (ix,elm) in zip(dmdom,dmarr) do
    writeln(" ", ix, " on ", elm.locale.id, ": ", elm);
  writeln();
}

if (manylocs) {
  for l in mylocs do
    on l do
      showArrLocales();
} else {
  showArrLocales();
}

writeln("parallel iterator over dmdom");
forall i in dmdom do
  writeln(i);
writeln();

/////////////////////////////////
// Vass's test 1-d distribution - replicated

class vdist {
  // REQ over how many locales
  var numLocales: int;

  // for debugging/tracing
  var name: string;
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
}

class vlocdom {
  // our copy of wholeR
  var locWholeR;
}

// REQ create a 1-d global domain descriptor for dsiNewRectangularDom()
// where our dimension is a range(idxType, bounded, stridable)
proc vdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  return new vdom(idxType, stridable);
}

// REQ produce a local domain descriptor for our dimension
// on a locale for the given locale index locId, for dsiNewRectangularDom().
// Note: may be invoked multiple times/locales with the same locId.
// - one for each point in the other dimension(s).
proc vdom.dsiNewLocalDom1d(locId: locIdxT) {
  return new vlocdom(wholeR);
}

// REQ given our dimension of the array index, on which locale is it located?
// CLargs is the arguments to dsiIndexCurrentLocale1d(), should it be needed
proc vdist.dsiIndexLocale1d(indexx, CLargs): locIdxT {
  return dsiIndexCurrentLocale1d(CLargs);
}

// REQ update our data structures as needed upon dsiSetIndices()
proc vdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// REQ update our local data structures as needed upon dsiSetIndices();
// return the subrange that will be handled locally
proc vlocdom.dsiSetLocalIndices1d(globDD, locId: locIdxT): globDD.rangeT {
  locWholeR = globDD.wholeR;
  return locWholeR;
}

// REQ given out dimension of the array index, return
// (locale id, storage index on that locale) for that index
proc vdom.dsiAccess1d(indexx: idxType, CLargs): (locIdxT, stoSzT) {
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
         0:stoSzT..#wholeR.length:stoSzT);
}

// REQ generate the densified range to be used in the leader iterator
// on this descriptor's locale for task 'taskid' of 0..#numTasks.
// Currently the DSI framework computes numTasks based on dsiSetLocalIndices1d
// and handles the case numTasks <= 1.
// Note: should be densified w.r.t. the entire global range in this dimension.
proc vlocdom.dsiMyDensifiedRangeForTaskID1d(taskid:int, numTasks:int) {
  const (startIx, endIx) = _computeChunkStartEnd(locWholeR.length,
                                                 numTasks, taskid);
  return startIx - 1 .. endIx - 1;
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
  const myLocID: locIdxT;
  var myRange;
}

proc sdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("The index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the 'sdist' 1-d distribution");
  return new sdom(idxType = idxType, stridable = stridable);
}

proc sdom.dsiNewLocalDom1d(indexx: locIdxT) {
  var defaultVal: rangeT;
  return new slocdom(indexx, defaultVal);
}

proc sdist.dsiIndexLocale1d(indexx: idxType, CLargs): locIdxT {
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

proc sdom.dsiAccess1d(indexx: idxType, CLargs): (locIdxT, stoSzT) {
  // TODO replace 0
  return (0:locIdxT,
          wholeR.indexOrder(indexx): stoSzT);
}

iter sdom.dsiSerialArrayIterator1d(CLargs) {
  // TODO replace 0
  yield (0, 0:stoSzT..#wholeR.length:stoSzT);
}

proc slocdom.dsiMyDensifiedRangeForTaskID1d(taskid:int, numTasks:int) {
  // TODO replace 0
  if myLocID == 0 {
    // NB for now we can densify w.r.t. myRange (i.e. self), but will need to
    // densify w.r.t. the entire global range once we distribute things.
    const (startIx, endIx) = _computeChunkStartEnd(myRange.length,
                                                   numTasks, taskid);
    return startIx - 1 .. endIx - 1;
  } else {
    return 1..0;
  }
}
