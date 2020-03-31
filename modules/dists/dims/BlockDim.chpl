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
// Block dimension specifier - for use with DimensionalDist2D.
//

private use DimensionalDist2D;


/*
This Block dimension specifier is for use with the
:class:`DimensionalDist2D` distribution.

It specifies the mapping of indices in its dimension
that would be produced by a 1D :class:`~BlockDist.Block` distribution.

**Initializer Arguments**

The following ``BlockDim`` record initializers are available:

  .. code-block:: chapel

    proc BlockDim.init(
      numLocales,
      boundingBoxLow,
      boundingBoxHigh,
      type idxType = boundingBoxLow.type)

    proc BlockDim.init(
      numLocales: int,
      boundingBox: range(?),
      type idxType = boundingBox.idxType)

The ``numLocales`` argument specifies the number of locales
that this dimension's bounding box is to be distributed over.

The ``boundingBoxLow`` and ``boundingBoxHigh`` arguments are
a convenient replacement for the ``boundingBox`` argument,
which specifies the bounding box in this dimension.

The ``idxType``, whether provided or inferred, must match
the index type of the domains "dmapped" using the corresponding
``DimensionalDist2D`` distribution.
*/
record BlockDim {
  // the type of bbStart, bbLength
  // (also (ab)used as the idxType of the domains created over this dist.)
  // (todo - straighten that out)
  type idxType;

  const numLocales: int;

  // the .low and .size of BlockDist's 'boundingBox' for our dimension
  const bbStart: idxType;
  const bbLength: idxType;

  proc boundingBox return bbStart .. (bbStart + bbLength - 1);
}

record Block1dom {
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range
  var wholeR: range(idxType, BoundedRangeType.bounded, stridable);

  // privatized distribution descriptor
  const pdist;

  proc dsiSetIndicesUnimplementedCase param return false;
}

record Block1locdom {
  var myRange;
}


/////////// user initializer, for convenience

proc BlockDim.init(numLocales: int, boundingBox: range(?),
                   type idxType = boundingBox.idxType)
{
  if !isBoundedRange(boundingBox) then
    compilerError("The 1-d block descriptor initializer was passed an unbounded range as the boundingBox");
  this.idxType = idxType;

  this.numLocales = numLocales;
  this.bbStart = boundingBox.low;
  this.bbLength = boundingBox.size;
}

/////////// privatization - start

proc BlockDim.dsiGetPrivatizeData1d() {
  return (numLocales, bbStart, bbLength);
}

proc type BlockDim.dsiPrivatize1d(privatizeData) {
  return new BlockDim(privatizeData, this.idxType);
}

// initializer for privatization
proc BlockDim.init(privatizeData, type idxType) {
  this.idxType = idxType;
  numLocales = privatizeData(0);
  bbStart = privatizeData(1);
  bbLength = privatizeData(2);
}

proc BlockDim.dsiUsesLocalLocID1d() param return false;

proc Block1dom.dsiGetPrivatizeData1d() {
  return (wholeR,);
}

proc type Block1dom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new Block1dom(idxType   = this.idxType,
                  stridable = this.stridable,
                  wholeR    = privatizeData(0),
                  pdist     = privDist);
}

proc Block1dom.dsiGetReprivatizeData1d() {
  return (wholeR,);
}

proc Block1dom.dsiReprivatize1d(reprivatizeData) {
  this.wholeR = reprivatizeData(0);
}

proc Block1dom.dsiUsesLocalLocID1d() param return false;

proc Block1dom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

/////////// privatization - end


// Initializer. idxType is inferred from the 'bbLow' argument
// (alternative: default to 'int' instead).
proc BlockDim.init(numLocales, boundingBoxLow, boundingBoxHigh, type idxType = boundingBoxLow.type) {
  if !(boundingBoxLow <= boundingBoxHigh) then halt("'BlockDim' distribution must have a non-empty bounding box between boundingBoxLow and boundingBoxHigh; got ", boundingBoxLow, " .. ", boundingBoxHigh);
  assert(numLocales > 0); // so we can cast it to any int type
  this.idxType = idxType;
  this.numLocales = numLocales;
  this.bbStart = boundingBoxLow;
  this.bbLength = (boundingBoxHigh - boundingBoxLow + 1):idxType;
  assert(this.bbLength > 0);
}

proc BlockDim.toString()
  return "BlockDim(" + numLocales:string + ", " + boundingBox:string + ")";

proc BlockDim.dsiNewRectangularDom1d(type idxType, param stridable: bool,
                                     type stoIndexT)
{
  // ignore stoIndexT - all we need is for other places to work out
  if idxType != this.idxType then
    compilerError("The index type ", idxType:string,
                  " does not match the index type ",this.idxType:string,
                  " of the 'BlockDim' 1-d distribution");
  return new Block1dom(idxType = idxType, stridable = stridable, pdist = this);
}

proc Block1dom.dsiIsReplicated1d() param return false;

proc Block1dom.dsiNewLocalDom1d(type stoIndexT, locId: locIdT) {
  var defaultVal: range(stoIndexT, stridable=this.stridable);
  return new Block1locdom(myRange = defaultVal);
}

proc BlockDim.dsiIndexToLocale1d(indexx): locIdT {
  if indexx <= bbStart then
    return 0;

  const index0 = indexx - bbStart;  // always > 0

  // (numLocales-1) is the answer when
  //   index0 >= (numLocales-1) * (bbLength+1) / numLocales
  // whose r.h.s. could be stored in 'BlockDim', trading memory for computation.
  // We simplify the condition to 'index0 >= bbLength'.
  // (Note: if index0 == bbLength - 1, the answer may not be (numLocales - 1)).
  // If the condition doesn't hold, the later formula gives the answer.
  // (Compared to the formula in Block.targetLocsIdx, we run comparisons
  // *before* multiplying and dividing.)
  //
  if index0 >= bbLength then
    return numLocales - 1;

  // need to think what to do if numLocales is not int
  type adjT =
    if numLocales.type == int && idxType == uint(64)
    then uint(64)
    else numLocales.type;

  const result = index0 * numLocales:adjT / bbLength;
  assert(0 <= result && result < numLocales:adjT);
  return result:locIdT;
}

proc Block1dom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// Helper: compute the chunk to be stored on locID
proc Block1dom._dsiComputeMyRange(locId): rangeT {
  const dom = this;
  const dist = dom.pdist;
  // see LocBlock.LocBlock()
  const (blo, bhi) = _computeBlock(dist.bbLength, dist.numLocales,
           locId, max(dom.idxType), min(dom.idxType), dist.bbStart);
  const myChunk = blo..bhi;
  // see Block.getChunk()
  const chunk = dom.wholeR(myChunk);
  return chunk;
}

proc Block1locdom.dsiSetLocalIndices1d(globDD, locId: locIdT) {
  myRange = globDD._dsiComputeMyRange(locId);
  return myRange;
}

/////////////////////////////////

proc Block1dom.dsiStorageUsesUserIndices() param return true;

proc Block1dom.dsiAccess1d(indexx: idxType): (locIdT, idxType) {
  return (pdist.dsiIndexToLocale1d(indexx), indexx);
}

iter Block1locdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
  const locRange = densify(myRange, globDD.wholeR, userErrors=false);
  yield locRange: range(globDD.idxType);
}

proc Block1dom.dsiSingleTaskPerLocaleOnly1d() param return false;

proc Block1locdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int) {
  const locRange = densify(myRange, globDD.wholeR, userErrors=false);
  // Copied straight from BlockDom leader - replace locBlock(parDim)->locRange.
  const (lo, hi) = _computeBlock(locRange.size, numTasks, taskid,
                                 locRange.high, locRange.low, locRange.low);

  // If this can occasionally be an empty range, add a check to Dimensional
  // to not yield anything in such a case.
  return lo..hi;
}

proc Block1locdom.dsiMyDensifiedRangeType1d(globDD) type
  return range(globDD.idxType);

iter Block1dom.dsiSerialArrayIterator1d() {
  // The Block distribution assigns indices to locales contiguously and
  // so that (i1<i2) => (locId1<=locId2). This is defined by the domain map.
  // As we traverse the range (i.e. this.wholeR), locIds will increase
  // if the stride is positive, and decrease otherwise.
  // On a single locale, Block1locdom.myRange (aka this._dsiComputeMyRange())
  // already happens to reflect the desired direction.

  for locId in (0..#pdist.numLocales) by sgn(wholeR.stride) {
    // We do not go to Block1locdom for myRange because (a) recomputing it
    // is probably cheaper than going to a remote locale, and
    // (b) we do not store Block1locdom objects in Block1dom.
    const localRange = _dsiComputeMyRange(locId);
//writeln("Block1dom.dsiSerialArrayIterator1d  ", locId, ", ", localRange);
    yield (locId, localRange);
  }
}

iter Block1dom.dsiFollowerArrayIterator1d(undensRange): (locIdT, idxType) {
//writeln("Block1dom.dsiFollowerArrayIterator1d  undensRange ", undensRange);

  const heuristic_weight_rangepoint = 2; // measure of work in (a) below
  const heuristic_weight_locale = 3;     // measure of work in (b)

  if heuristic_weight_locale * undensRange.stride >
      heuristic_weight_rangepoint * pdist.bbLength / pdist.numLocales
  {
//writeln("Block1dom.dsiFollowerArrayIterator1d case A");
    // (a) no benefit to factor out computations for a particular locale
    for ix in undensRange do
      yield dsiAccess1d(ix);

  } else {
    // (b) per-locale computation is amortized over enough undensRange indices
    const lowLocId = pdist.dsiIndexToLocale1d(undensRange.alignedLow);
    const highLocId = pdist.dsiIndexToLocale1d(undensRange.alignedHigh);

    // check for a supposedly more common case of following our own leader
    if lowLocId == highLocId {
//writeln("Block1dom.dsiFollowerArrayIterator1d case B1  lowLocId ", lowLocId);
      for i in undensRange do
        yield (lowLocId, i);

    } else {
      assert(lowLocId < highLocId);
      for locId in lowLocId .. highLocId by sgn(undensRange.stride) {
        // We do not go to Block1locdom for myRange - same rationale as above.
        const thisLocaleRange = _dsiComputeMyRange(locId);
        const localRangeToFollow = undensRange(thisLocaleRange);
//writeln("Block1dom.dsiFollowerArrayIterator1d case B2  locId ", locId, "  localRangeToFollow ", localRangeToFollow);
        for i in localRangeToFollow do
          yield (locId, i);
      }
    }
  }
}
