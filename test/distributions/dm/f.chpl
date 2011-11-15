// One-dimensional block-cyclic distribution for use with Dimensional.

use d;

config const BlockCyclicDim_allowParLeader = true;

// the types to use for blockSzie and numLocales
type cycSizeT = uint(32);     // unsigned - for optimization
type cycSizeTuser = int(32);  // for versatility

class idist {
  // distribution parameters
  const numLocales: cycSizeTuser;
  const lowIdx: int(64); // ensure we can always subtract it
  const blockSize: cycSizeTuser;

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

proc idist.dsiSupportsPrivatization1d() param return true;

proc idist.dsiGetPrivatizeData1d() {
  return (lowIdx, blockSize, numLocales, name);
}

proc idist.dsiPrivatize1d(privatizeData) {
  return new idist(lowIdx = privatizeData(1),
                   blockSize = privatizeData(2),
                   numLocales = privatizeData(3),
                   name = privatizeData(4));
}

proc idist.dsiUsesLocalLocID1d() param return false;

proc idom.dsiSupportsPrivatization1d() param return true;

proc idom.dsiGetPrivatizeData1d() {
  return tuple(wholeR, wholeRstrideAbs, storagePerCycle, lowIdxAdj, name);
}

proc idom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new idom(idxType   = this.idxType,
                  stoIndexT = this.stoIndexT,
                  stridable = this.stridable,
                  name            = privatizeData(5),
                  wholeR          = privatizeData(1),
                  wholeRstrideAbs = privatizeData(2),
                  storagePerCycle = privatizeData(3),
                  lowIdxAdj       = privatizeData(4),
                  // could include these in privatizeData
                  blockSizePos  = privDist.blockSizePos,
                  numLocalesPos = privDist.numLocalesPos,
                  cycleSizePos  = privDist.cycleSizePos);
}

proc idom.dsiGetReprivatizeData1d() {
  return tuple(wholeR, wholeRstrideAbs, storagePerCycle);
}

proc idom.dsiReprivatize1d(other, reprivatizeData) {
  if other.idxType   != this.idxType ||
     other.stoIndexT != this.stoIndexT ||
     other.stridable != this.stridable then
    compilerError("inconsistent types in privatization");

  this.wholeR          = reprivatizeData(1);
  this.wholeRstrideAbs = reprivatizeData(2);
  this.storagePerCycle = reprivatizeData(3);
}

proc idom.dsiUsesLocalLocID1d() param return false;

proc idom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

/////////// privatization - end


// Check all restrictions/assumptions that must be satisfied by the user
// when constructing a 1-d BlockCyclic distribution.
inline proc idist.checkInvariants() {
  assert(blockSize > 0, "BlockCyclic1d-blockSize");
  assert(numLocales > 0, "BlockCyclic1d-numLocales");
}

proc idist.toString()
  return "BlockCyclicDim(" + numLocales:string + ", " +
         lowIdx:string + ", " + blockSize:string + ")";

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

  return result;
}

proc idom.dsiIsReplicated1d() param return false;

proc idom.dsiNewLocalDom1d(type stoIndexT, locId: locIdT) {
  const result = new ilocdom(idxType = this.idxType,
                             stoIndexT = stoIndexT,
                             locId = locId);
  return result;
}

proc idom.dsiBuildRectangularDom1d(DD,
                                   param stridable:bool,
                                   rangeArg: range(idxType,
                                                   BoundedRangeType.bounded,
                                                   stridable))
{
  // There does not seem to be any optimizations from merging the two calls.
  const result = DD.dsiNewRectangularDom1d(idxType, stridable, stoIndexT);
  result.dsiSetIndices1d(rangeArg);
  return result;
}

proc ilocdom.dsiBuildLocalDom1d(newGlobDD, locId: locIdT) {
  assert(locId == this.locId);
  // There does not seem to be any optimizations from merging the two calls.
  const newLocDD = newGlobDD.dsiNewLocalDom1d(this.stoIndexT, locId);
  const newStoRng = newLocDD.dsiSetLocalIndices1d(newGlobDD, locId);
  return (newLocDD, newStoRng);
}


/////////////////////////////////

// #include f.chpl.formulas.txt

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
    if ind0 >= 0 then ( (ind0 / blockSize) % numLocales ): locIdT
    else  mod(divfloor(ind0, blockSizePos), numLocalesPos): locIdT
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

proc idom.dsiStorageUsesUserIndices() param return false;

proc idom.dsiAccess1d(ind: idxType): (locIdT, stoIndexT) {
  return (_dsiLocNo_formula(ind), _dsiStorageIdx(ind));
}

iter ilocdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
// todo: for the special case handled in dsiMyDensifiedRangeForTaskID1d,
// maybe handling it here will be beneficial, too?
  const locNo = this.locId;
  const wholeR = globDD.wholeR;
  const lowIdx = wholeR.low;
  const highIdx = wholeR.high;
  type retT = dsiMyDensifiedRangeType1d(globDD);
  param stridable = globDD.stridable;
  assert(stridable == wholeR.stridable); // sanity

  if lowIdx > highIdx then
    return;

  const lowCycNo = globDD._dsiCycNo(lowIdx);
  const highCycNo = globDD._dsiCycNo(highIdx);
  const up = wholeR.stride > 0;
  assert(lowIdx <= highIdx);

  proc mydensify(densifyee): dsiMyDensifiedRangeType1d(globDD) {
    const temp = densify(densifyee, wholeR);
    return temp:retT;
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

// available in a special case only, for now
proc idom.dsiSingleTaskPerLocaleOnly1d()
  return !BlockCyclicDim_allowParLeader ||
         !((blockSizePos:wholeR.stride.type) == wholeR.stride);

// only works when idom.dsiSingleTaskPerLocaleOnly1d()
proc ilocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int)
{
  const wholeR = globDD.wholeR;
  const nLocs  = globDD.numLocalesPos :globDD.idxType;
  assert((globDD.blockSizePos:wholeR.stride.type) == wholeR.stride);
  assert(globDD.storagePerCycle == 1); // should follow from the previous

  // In this case, the densified range for *all* indices on this locale is:
  //   0..#wholeR.length by numLocales align AL
  // where
  //   (_dsiLocNo(wholeR.low) + AL) % numLocales == this.locId

  const firstLoc = globDD._dsiLocNo_formula(wholeR.low);
  const AL = this.locId + nLocs - firstLoc;

  // Here is the densified range for all indices on this locale.
  const hereDenseInds = 0..#wholeR.length by nLocs align AL;
  const hereNumInds   = hereDenseInds.length;
  const hereFirstInd  = hereDenseInds.first;

  // This is our piece of hereNumInds
  const (begNo,endNo) = _computeChunkStartEnd(hereNumInds, numTasks, taskid+1);

  // Pick the corresponding part of hereDenseInds
  const begIx = hereFirstInd + (begNo - 1) * nLocs;
  const endIx = hereFirstInd + (endNo - 1) * nLocs;
  assert(hereDenseInds.member(begIx));
  assert(hereDenseInds.member(endIx));

//writeln("MyDensifiedRangeForTaskID(", globDD.name, ") on ", locId,
//        "  taskid ", taskid, " of ", numTasks, "  ", begIx, "...", endIx,
//        "   fl=", firstLoc, " al=", AL,
//        "  fullR ", hereDenseInds, " myR ", begIx .. endIx by nLocs,
//        "");

  return begIx .. endIx by nLocs;
}

proc ilocdom.dsiMyDensifiedRangeType1d(globDD) type
  return range(idxType=globDD.idxType, stridable=globDD.stridable);

proc ilocdom.dsiLocalSliceStorageIndices1d(globDD, sliceRange)
  : range(stoIndexT, sliceRange.boundedType, false)
{
  if sliceRange.stridable {
    // to be done: figure out sliceRange's stride vs. globDD.wholeR.stride
    compilerError("localSlice is not implemented for the Dimensional distribution with a block-cyclic dimension descriptor when the slice is stridable");
  } else {
    const lowSid = if sliceRange.hasLowBound()
      then globDD._dsiStorageIdx(sliceRange.low)
      else 0: stoIndexT;
    const highSid = if sliceRange.hasHighBound()
      then globDD._dsiStorageIdx(sliceRange.high)
      else 0: stoIndexT;
    return new range(stoIndexT, sliceRange.boundedType, false, lowSid, highSid);
  }
}

iter idom.dsiSerialArrayIterator1d() {
  // dispatch here, for code clarity
  if stridable then
    for result in _dsiSerialArrayIterator1dStridable() do
      yield result;
  else
    for result in _dsiSerialArrayIterator1dUnitstride(wholeR) do
      yield result;
}

iter idom._dsiSerialArrayIterator1dUnitstride(rangeToIterateOver) {
  assert(!rangeToIterateOver.stridable);

  const firstIdx = rangeToIterateOver.low;
  const lastIdx = rangeToIterateOver.high;

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

iter idom._dsiSerialArrayIterator1dStridable() {
  assert(stridable);
  compilerWarning("array iterator over stridable block-cyclic-dim arrays is presently not efficient", 4);

  // the simplest way out
  for ind in wholeR do
    yield (_dsiLocNo_formula(ind), _dsiStorageIdx(ind)..#(1:stoIndexT));
}

iter idom.dsiFollowerArrayIterator1d(undensRange): (locIdT, idxType) {
  if undensRange.stridable {
    // the simplest way out
    for ix in undensRange do
      yield dsiAccess1d(ix);

  } else {
    for (locNo, stoIxs) in _dsiSerialArrayIterator1dUnitstride(undensRange) do
      for stoIdx in stoIxs do
        yield (locNo, stoIdx);
  }
}
