/*
This ReplicatedDist distribution causes a domain and its arrays
to be replicated across desired locales (currently across all the locales).
An array receives a distinct set of elements - a "replicand" -
allocated on each locale.

This replication is observable when iterating over such a domain or array.
When indexing into an array, only the replicand *on the current locale*
is accessed.

Features/limitations:
* Consistency/coherence among replicands is NOT maintained.
* Only rectangular domains are presently supported.

In other words, mapping a domain with ReplicatedDist makes it behave
as if there is one copy of its indices per locale.  When iterating,
the number of iterations will be (the number of locales involved)
times (the number of iterations over this domain if it were
distributed with the default distribution).

Examples:

const Dbase = [1..5];
const Drepl: domain(1) dmapped ReplicatedDist() = Dbase;
var Abase: [Dbase] int;
var Arepl: [Drepl] int;

// only the current locale's replicand is accessed
Arepl[3] = 4;

// these iterate over Dbase, so
// only the current locale's replicand is accessed
forall (b,r) in (Abase,Arepl) b = r;
Abase = Arepl;

// these iterate over Drepl;
// each replicand will be zippered against (and copied from) the entire Abase
forall (r,b) in (Arepl,Abase) r = b;
Arepl = Abase;

// sequential zippering will detect difference in sizes
// (if multiple locales)
for (b,r) in (Abase,Arepl) ... // error
for (r,b) in (Arepl,Abase) ... // error

TODO:
- implement privatization
- allow run-time change in locales (when it becomes needed)
- implement the rest of the DSI methods
- define dsiNumIndices
   - should it be Dbase.numIndices or that times num locales?
- support other kinds of domains
*/


// trace certain DSI methods as they are being invoked
config param traceReplicatedDist = false;


/////////////////////////////////////////////////////////////////////////////
// distribution

//
// (global) distribution class
//
class ReplicatedDist : BaseDist {
  // the desired locales (an array of locales)
  const targetLocales;
  // "IDs" are indices into targetLocales
  proc targetIds return targetLocales.domain;
}

// constructor: replicate over the given locales
// (by default, over all locales)
proc ReplicatedDist.ReplicatedDist(targetLocales: [] locale = Locales) {
  if traceReplicatedDist then
    writeln("ReplicatedDist constructor over ", targetLocales);
}


/////////////////////////////////////////////////////////////////////////////
// domains

//
// global domain class
//
class ReplicatedDom : BaseRectangularDom {
  // to support rectangular domains
  param rank : int;
  type idxType;
  param stridable: bool;
  // we need to be able to provide the domain map for our domain - to build its
  // runtime type (because the domain map is part of the type - for any domain)
  // (looks like it must be called exactly 'dist')
  var dist; // must be a ReplicatedDist

  // this is our index set; we store it here so we can get to it easily
  var domRep: domain(rank, idxType, stridable);

  // local domain objects
  // NOTE: 'dist' must be initialized prior to 'localDoms'
  // => currently have to use the default constructor
  var localDoms: [dist.targetIds] LocReplicatedDom(rank, idxType, stridable);
}

//
// local domain class
//
class LocReplicatedDom {
  // copy from the global domain
  param rank : int;
  type idxType;
  param stridable: bool;

  // our index set, copied from the global domain
  var domLocalRep: domain(rank, idxType, stridable);
}


// privatization is not implemented currently
proc ReplicatedDist.dsiSupportsPrivatization() param: bool return false;
proc ReplicatedDom.dsiSupportsPrivatization() param: bool return false;


proc ReplicatedDist.dsiClone(): this.type {
  if traceReplicatedDist then writeln("ReplicatedDist.dsiClone");
  return new ReplicatedDist(targetLocales);
}

// create a new domain mapped with this distribution
proc ReplicatedDist.dsiNewRectangularDom(param rank: int,
                                         type idxType,
                                         param stridable: bool)
  : ReplicatedDom(rank, idxType, stridable, this.type)
{
  if traceReplicatedDist then writeln("ReplicatedDist.dsiNewRectangularDom ",
                                      (rank, typeToString(idxType), stridable));

  // Have to call the default constructor because we need to initialize 'dist'
  // prior to initializing 'localDoms' (which needs a non-nil value for 'dist'.
  var result = new ReplicatedDom(rank=rank, idxType=idxType,
                                 stridable=stridable, dist=this);

  // create local domain objects
  coforall (loc, locDom) in (targetLocales, result.localDoms) do
    on loc do
      locDom = new LocReplicatedDom(rank, idxType, stridable);

  return result;
}

//Given an index, this should return the locale that owns that index.
proc ReplicatedDist.dsiIndexLocale(indexx): locale {
  // TODO: this is invoked from idxToLocale(),
  // but that does not seem to be used anywhere
  compilerError("not implemented");
  return this.locale;
}

/*
dsiSetIndices accepts ranges because it is invoked so from ChapelArray or so.
Seems like any dsiSetIndices() on a tuple of ranges can be like this one.
Or that one place can be converted following this example.
*/
proc ReplicatedDom.dsiSetIndices(rangesArg: rank * range(idxType)): void {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on ranges");
  dsiSetIndices([(...rangesArg)]);
}

proc ReplicatedDom.dsiSetIndices(domArg: domain(rank, idxType, stridable)): void {
  if traceReplicatedDist then
    writeln("ReplicatedDom.dsiSetIndices on domain ", domArg);
  domRep = domArg;
  coforall locDom in localDoms do
    on locDom do
      locDom.domLocalRep = domArg;
}

proc ReplicatedDom.dsiGetIndices(): domain(rank, idxType, stridable) {
  if traceReplicatedDist then writeln("ReplicatedDom.dsiGetIndices");
  return domRep;
}

// iterators over the domain's indices (serial, leader, follower)
// our semantics: yield each of the domain's indices once per each locale

// right now, completely serial
iter ReplicatedDom.these() {
  for locDom in localDoms do
//  on locDom do // compiler does not allow; see r16137 and nestedForall*
      for i in locDom.domLocalRep do
        yield i;
}

iter ReplicatedDom.these(param tag: iterator) where tag == iterator.leader {
  coforall locDom in localDoms do
    on locDom do
      // for simplicity, redirect to DefaultRectangular's leader/follow
      for follow in locDom.domLocalRep._value.these(tag) do
        yield follow;
}

iter ReplicatedDom.these(param tag: iterator, follower) where tag == iterator.follower {
  // redirect to DefaultRectangular
  // note: 'localDoms[here.id].domLocalRep' would not save any comm
  //       because have to go to ReplicatedDom's locale for 'localDoms' anyway
  for i in domRep._value.these(tag, follower) do
    yield i;
}

/* Write the domain out to the given Writer serially. */
proc ReplicatedDom.dsiSerialWrite(f: Writer): void {
  // redirect to DefaultRectangular
  domRep._value.dsiSerialWrite(f);
  f.write(" replicated over ");
  dist.targetLocales._value.dsiSerialWrite(f);
}


/////////////////////////////////////////////////////////////////////////////
// arrays

//
// global array class
//
class ReplicatedArr : BaseArr {
  // These two are hard-coded in the compiler - it computes the array's
  // type string as '[dom.type] eltType.type'
  type eltType;
  var dom; // must be a ReplicatedDom

  // the replicated arrays
  // NOTE: 'dom' must be initialized prior to initializing 'localArrs'
  var localArrs: [dom.dist.targetIds]
              LocReplicatedArr(eltType, dom.rank, dom.idxType, dom.stridable);
}

//
// local array class
//
class LocReplicatedArr {
  // these generic fields let us give types to the other fields easily
  type eltType;
  param rank : int;
  type idxType;
  param stridable: bool;

  var myDom: LocReplicatedDom(rank, idxType, stridable);
  var arrLocalRep: [myDom.domLocalRep] eltType;
}


// privitization is currently not implemented
proc ReplicatedArr.dsiSupportsPrivatization() param: bool return false;

// required by the arg type in _array.this(i: _value.idxType ...rank)
// could store as a field in ReplicatedArr
proc ReplicatedArr.idxType type return dom.idxType;


// create a new array over this domain
proc ReplicatedDom.dsiBuildArray(type eltType)
  : ReplicatedArr(eltType, this.type)
{
  if traceReplicatedDist then writeln("ReplicatedDom.dsiBuildArray");
  return new ReplicatedArr(eltType, this, localDoms);
}

/* A ReplicatedArr constructor, for ReplicatedDom.dsiBuildArray.
We declare it explicitly so that the fields in the parent class, BaseArr,
are initialized with their default initializers.
*/
proc ReplicatedArr.ReplicatedArr(type eltType, dom: ReplicatedDom, localDoms) {
  // initializes the fields 'eltType', 'dom' by name
  if traceReplicatedDist then writeln("ReplicatedArr constructor");
  coforall (loc, locDom, locArr)
   in (dom.dist.targetLocales, localDoms, localArrs) do
    on loc do
      locArr = new LocReplicatedArr(eltType,
                                    dom.rank, dom.idxType, dom.stridable,
                                    locDom);
}

// Seems to return the domain for an array
// used e.g. in BaseArr.destroyArr()
// This method is exactly the same across all existing distributions.
// TODO: maybe it should be hoisted into BaseArr, along with the 'dom' field.
proc ReplicatedArr.dsiGetBaseDom() return dom;

// Return the array element corresponding to the index - on the current locale
proc ReplicatedArr.dsiAccess(indexx) var: eltType {
  return localArrs[here.id].arrLocalRep[indexx];
}

// Write the array out to the given Writer serially.
proc ReplicatedArr.dsiSerialWrite(f: Writer): void {
  var neednl = false;
  for locArr in localArrs {
//  on locArr {  // may cause deadlock
      if neednl then f.write("\n"); neednl = true;
      f.write(locArr.locale, ":\n");
      locArr.arrLocalRep._value.dsiSerialWrite(f);
//  }
  }
}

// iterators

// completely serial
iter ReplicatedArr.these() var: eltType {
  for locArr in localArrs do
//  on locArr do // compiler does not allow; see r16137 and nestedForall*
      for a in locArr.arrLocalRep do
        yield a;
}

iter ReplicatedArr.these(param tag: iterator) where tag == iterator.leader {
  coforall locArr in localArrs do
    on locArr do
      // for simplicity, redirect to DefaultRectangular's leader/follow
      for follow in locArr.arrLocalRep._value.these(tag) do
        yield follow;
}

iter ReplicatedArr.these(param tag: iterator, follower) var where tag == iterator.follower {
  // redirect to DefaultRectangular
  for a in localArrs[here.id].arrLocalRep._value.these(tag, follower) do
    yield a;
}


/////////////////////////////////////////////////////////////////////////////
// slicing, reindexing

// This supports reassignment of the array's domain.
/*
This gets invoked upon reassignment of the array's domain,
prior to calling this.dom.dsiSetIndices().
So this needs to adjust anything in the array that won't be taken care of
in this.dom.dsiSetIndices().
*/
proc ReplicatedArr.dsiReallocate(d: domain): void {
  if traceReplicatedDist then
    writeln("ReplicatedArr.dsiReallocate ", dom.domRep, " -> ", d, " (no-op)");
}
