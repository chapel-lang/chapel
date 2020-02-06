/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
// Replicated dimension specifier -  for use with DimensionalDist2D.
//
// The code below also specifies the dimension specifier interface:
// The required methods are marked with 'REQ' followed by a brief description.
//

private use DimensionalDist2D;
private use RangeChunk only ;

/*
This Replicated dimension specifier is for use with the
:class:`DimensionalDist2D` distribution.

The dimension of a domain or array for which this specifier is used
has a *replicand* for each element of ``targetLocales``
in the same dimension. This is similar to the Replicated distribution
(``Replicated``). The dimension specifies differs
in that it always accesses the local replicand, whereas the Replicated
distribution accesses all replicands in certain cases, as specified there.

**Initializer Arguments**

The ``ReplicatedDim`` record initializer is available as follows:

  .. code-block:: chapel

    proc ReplicatedDim.init(numLocales:int)

It creates a dimension specifier for replication over ``numLocales`` locales.
*/
record ReplicatedDim {
  // REQ over how many locales
  // todo: can the Dimensional do without this one?
  const numLocales: int;

  // Locale ID in our dimension of the locale this instance is on.
  // Because of these, it may be incorrect to share the same ReplicatedDim
  // object between multiple DimensionalDist objects.
  var localLocID = invalidLocID;
  var localLocIDlegit = false;
}

record Replicated1dom {
  // REQ the parameters of our dimension of the domain being created
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);
//todo-remove?  proc domainT type return domain(1, idxType, stridable);

  // our range
  var wholeR: range(idxType, BoundedRangeType.bounded, stridable);

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
  var localLocIDlegit = false;

  // REQ return false if unused
  proc dsiSetIndicesUnimplementedCase param return false;
}

record Replicated1locdom {
  type stoIndexT;
  param stridable;
  // our copy of wholeR
  var locWholeR: range(stoIndexT, stridable=stridable);
}


/////////// privatization - start

// REQ - same purpose as dsiGetPrivatizeData(); can return 'this'
proc ReplicatedDim.dsiGetPrivatizeData1d() {
  return (numLocales,);
}

// REQ - same purpose as dsiPrivatize()
proc type ReplicatedDim.dsiPrivatize1d(privatizeData) {
  return new ReplicatedDim(numLocales = privatizeData(1));
}

// REQ does this class need -- and provide -- the localLocID?
// dsiStoreLocalLocID1d() will be invoked on privatized copies
proc ReplicatedDim.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
// If 'legit' is false, this is a privatized copy on a locale
// that's not among our Dimensional distribution's target locales.
proc ReplicatedDim.dsiStoreLocalLocID1d(localLocID: locIdT, legit: bool) {
  // This will get invoked multiple times if this ReplicatedDim object
  // is reused in another DimensionalDist object.
  // In which case the cache better be the same for both uses.
  if this.localLocID == invalidLocID {
    // This is the intended use.
    this.localLocID = localLocID;
    this.localLocIDlegit = legit;
  } else {
    if this.localLocID == localLocID &&
       this.localLocIDlegit == localLocIDlegit {
      // alright, let it be for now
    } else {
      halt("Inconsistent locale cache in a ReplicatedDim descriptor object. One cause can be a reuse of such an object for different DimensionalDist objects whose target locales differ and/or this object is reused in a different dimension.");
    }
  }
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID and its legitimacy
proc ReplicatedDim.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ - same purpose as dsiGetPrivatizeData(); can return 'this'
proc Replicated1dom.dsiGetPrivatizeData1d() {
  return (wholeR,);
}

// REQ - same purpose as dsiPrivatize()
// 'privDist' is the corresponding 1-d distribution descriptor, privatized
proc type Replicated1dom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new Replicated1dom(idxType   = this.idxType,
                            stridable = this.stridable,
                            wholeR    = privatizeData(1));
}

// REQ - same purpose as dsiGetReprivatizeData()
proc Replicated1dom.dsiGetReprivatizeData1d() {
  return (wholeR,);
}

// REQ - same purpose as dsiReprivatize()
proc Replicated1dom.dsiReprivatize1d(reprivatizeData) {
  this.wholeR = reprivatizeData(1);
}

// REQ does this class need the localLocID?
// (See also ReplicatedDim.dsiUsesLocalLocID1d.)
proc Replicated1dom.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
// This is invoked when a DimensionalDom is created, including
// from DimensionalDom.dsiPrivatize(), but not DimensionalDom.dsiReprivatize()
proc Replicated1dom.dsiStoreLocalLocID1d((localLocID, legit): (locIdT, bool)) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
  this.localLocIDlegit = legit;
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID
proc Replicated1dom.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ does a local domain descriptor use a pointer
// to the privatized global domain descriptor?
proc Replicated1dom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

// REQ if dsiLocalDescUsesPrivatizedGlobalDesc1d: store the pointer to
// the privatized copy of the global descriptor that is on Replicated1locdom's locale.
// Note: this is invoked only while privatizing the 1-d global descriptor.
// In particular, this is not invoked when the local descriptors are
// initially created.
// todo: we already pass globDD to all methods on Replicated1locdom, so probably
// we do not need this
//
proc Replicated1locdom.dsiStoreLocalDescToPrivatizedGlobalDesc1d(privGlobDesc) {
  compilerError("should not need dsiStoreLocalDescToPrivatizedGlobalDesc1d");
  // e.g.: this.privGlobDesc = privGlobDesc;
}

/////////// privatization - end


// REQ create a 1-d global domain descriptor for dsiNewRectangularDom()
// where our dimension is a range(idxType, bounded, stridable)
// stoIndexT is the same as in Replicated1dom.dsiNewLocalDom1d.
proc ReplicatedDim.dsiNewRectangularDom1d(type idxType, param stridable: bool,
                                          type stoIndexT)
{
  // ignore stoIndexT - all we need is for other places to work out
  return new Replicated1dom(idxType, stridable);
}

// A nicety: produce a string showing the parameters.
// This might get renamed in the future.
proc ReplicatedDim.toString()
  return "ReplicatedDim(" + numLocales:string + ")";

// REQ is this a replicated distribution?
proc Replicated1dom.dsiIsReplicated1d() param return true;

// REQ produce a local domain descriptor for our dimension
// on a locale for the given locale index locId, for dsiNewRectangularDom().
// Note: may be invoked multiple times/locales with the same locId.
// - one for each point in the other dimension(s).
// stoIndexT must be the index type of the range returned by
// dsiSetLocalIndices1d().
proc Replicated1dom.dsiNewLocalDom1d(type stoIndexT, locId: locIdT) {
  return new Replicated1locdom(stoIndexT, wholeR.stridable);
}

// REQ given our dimension of the array index, on which locale is it located?
proc ReplicatedDim.dsiIndexToLocale1d(indexx): locIdT {
  assert(localLocIDlegit);
  return localLocID;
}

// REQ update our data structures as needed upon dsiSetIndices()
proc Replicated1dom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// REQ update our local data structures as needed upon dsiSetIndices();
// return a range of "storage indices" to be stored locally.
//
// Each "storage index" corresponds to some "user index" (i.e.
// an index used to index into the array mapped with the Dimensional
// distribution) that is allocated on the receiver's locale.
//
// The returned range indicates *how many* of user indices are stored locally.
// It does NOT indicate *which* user indices those are. I.e. this range
// may contain exactly the user indices that are stored locally - or not
// (in which case dsiAccess1d() et al. will need to map each user index
// to an index in the range this returns).
//
// 'globDD' is the global domain descriptor (a 'Replicated1dom', in this case).
// The return type must be range(stoIndexT, stridable= true or false)
// where 'stoIndexT' is the arg of dsiNewLocalDom1d() that created 'this'.
//
proc Replicated1locdom.dsiSetLocalIndices1d(globDD, locId: locIdT) {
  locWholeR = globDD.wholeR;
  return locWholeR;
}



// REQ indicate whether "storage indices" returned by dsiSetLocalIndices1d()
// correspond to "user indices". (In which case dsiAccess1d() returns
// (as a part of a tuple) its argument without modification.)
proc Replicated1dom.dsiStorageUsesUserIndices() param return true;

// REQ given one dimension of the array index, return
// (locale id, storage index on that locale) for that index.
// The storage index must have the type DimensionalDom.stoIndexT.
proc Replicated1dom.dsiAccess1d(indexx: idxType): (locIdT, idxType) {
  // todo: provide access from non-"legit" locales;
  // maybe to locId=0?
  assert(localLocIDlegit);
  return (localLocID, indexx);
}

// REQ yield the densified range to be used in the leader iterator
// on this descriptor's locale, when there is only a single task.
iter Replicated1locdom.dsiMyDensifiedRangeForSingleTask1d(globDD)
  : dsiMyDensifiedRangeType1d(globDD)
{
  yield 0:globDD.idxType..#locWholeR.length;
}

// REQ whether this distribution can handle only a single task per locale.
proc Replicated1dom.dsiSingleTaskPerLocaleOnly1d() param return false;

// REQ unless dsiSingleTaskPerLocaleOnly1d() is true.
// Yield the densified range to be used in the leader iterator
// on this descriptor's locale for task 'taskid' of 0..#numTasks.
// Note: should be densified w.r.t. the entire global range in this dimension.
proc Replicated1locdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int)
  : dsiMyDensifiedRangeType1d(globDD)
{
  type IT = globDD.idxType;
  const (start, end) = RangeChunk.chunkOrder(locWholeR, numTasks:IT, taskid:IT);
  return (start:IT)..(end:IT);
}

// REQ the range type returned/yielded by dsiMyDensifiedRangeForSingleTask1d()
// and (if applicable) by dsiMyDensifiedRangeForTaskID1d().
proc Replicated1locdom.dsiMyDensifiedRangeType1d(globDD) type
  return range(globDD.idxType);

// REQ-2 if !dsiStorageUsesUserIndices
// Returns a range of storage indices corresponding to the user indices
// in 'sliceRange'. Can assume that all sliceRange indices are local.
proc Replicated1locdom.dsiLocalSliceStorageIndices1d(globDD, sliceRange) {
  compilerError("should not be invoked");
  return sliceRange; // but if needed, return this anyway
}

// REQ for iterating over array elements.
// Yield tuples (locId, iterable) s.t. the following results in
// traversing all array elements in order:
//   for (l,itr) in dsiSerialArrayIterator1d() do
//    for ix in itr do
//      yield (local storage on locale l)[ix]
iter Replicated1dom.dsiSerialArrayIterator1d() {
  assert(localLocIDlegit);
  yield (localLocID, wholeR);
}

// REQ for array follower iterator.
//
// Yield pairs (locId, storageOffset) that traverse storage for
// the array elements corresponding to the indices given by undensRange,
// preserving the order in which those indices occur.
//
// This is similar to dsiSerialArrayIterator1d(), except
// * only the indicated elements must be traversed, in the indicated order;
// * one storage offset is yielded each time.
//
// The latter is chosen to simplify dsiFollowerArrayIterator1d implementations.
// It also eliminates one loop nest per dimension in DimensionalArr follower.
//
iter Replicated1dom.dsiFollowerArrayIterator1d(undensRange): (locIdT, idxType) {
  assert(localLocIDlegit);
  for i in undensRange do
    yield (localLocID, i);
}
