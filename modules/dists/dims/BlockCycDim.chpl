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
// Block-cyclic dimension specifier - for use with DimensionalDist2D.
//

private use DimensionalDist2D;
private use RangeChunk only ;

config const BlockCyclicDim_allowParLeader = true;
config param BlockCyclicDim_enableArrayIterWarning = false;  // 'false' for testing
config const BlockCyclicDim_printAdjustedLowIdx = false;

// the values of this type must always be positive
type bcdPosInt = int;

// chpldoc TODO
// * Get the BlockCyclic distribution references to be presented
//   as links. At some point in the past they were not perhaps because
//   chpldoc did not find that module while creating this documentation.
//   Cf. it found ReplicatedDist while processing ReplicatedDim.
//   That may have been because the name of the class and the name of the file
//   used to match in the ReplicatedDist case.
//
/*
This Block-Cyclic dimension specifier is for use with the
:class:`DimensionalDist2D` distribution.

It specifies the mapping of indices in its dimension
that would be produced by a 1D :class:`~BlockCycDist.BlockCyclic` distribution.

**Initializer Arguments**

The ``BlockCyclicDim`` record initializer is defined as follows:

  .. code-block:: chapel

    proc init(
      numLocales:   int,
      lowIdx:       int,
      blockSize:    int,
      cycleSizePos: int = // computed by the implementation )

The arguments are as follows:

  ``numLocales``
      the number of locales that this dimension's indices are
      to be distributed over
  ``lowIdx``, ``blockSize``
      are the counterparts to ``startIdx`` and ``blocksize``
      in the :class:`~BlockCycDist.BlockCyclic` distribution
  ``cycleSizePos``
      is used internally by the implementation and
      should not be specified by the user code
*/
record BlockCyclicDim {
  // distribution parameters
  const numLocales: int;
  const lowIdx:     int;  // we want the most general type here
  const blockSize:  int;

  // tell the compiler these are positive
  proc blockSizePos   return blockSize: bcdPosInt;
  proc numLocalesPos  return numLocales: bcdPosInt;
  const cycleSizePos: bcdPosInt = (blockSize:bcdPosInt) * (numLocales:bcdPosInt);
}

record BlockCyclic1dom {
  type idxType;
  type stoIndexT;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range, normalized; its absolute stride
  var wholeR: range(idxType, BoundedRangeType.bounded, stridable);
  var wholeRstrideAbs: idxType;

  // a copy of BlockCyclicDim constants
  const adjLowIdx: idxType;
  const blockSizePos, numLocalesPos, cycleSizePos: bcdPosInt;

  // amount of storage per locale per cycle - depends on wholeR.stride
  var storagePerCycle: bcdPosInt = 0;

  // whether dsiSetIndices1d() detected an unimplemented case
  var dsiSetIndicesUnimplementedCase: bool;
}

record BlockCyclic1locdom {
  type idxType;
  type stoIndexT;
  const locId: locIdT;
}


/////////// privatization - start

proc BlockCyclicDim.dsiGetPrivatizeData1d() {
  return (lowIdx, blockSize, numLocales);
}

proc type BlockCyclicDim.dsiPrivatize1d(privatizeData) {
  return new BlockCyclicDim(lowIdx = privatizeData(1),
                            blockSize = privatizeData(2),
                            numLocales = privatizeData(3));
}

proc BlockCyclicDim.dsiUsesLocalLocID1d() param return false;

proc BlockCyclic1dom.dsiGetPrivatizeData1d() {
  return (wholeR, wholeRstrideAbs, storagePerCycle, adjLowIdx);
}

proc type BlockCyclic1dom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new BlockCyclic1dom(
                  idxType   = this.idxType,
                  stoIndexT = this.stoIndexT,
                  stridable = this.stridable,
                  wholeR          = privatizeData(1),
                  wholeRstrideAbs = privatizeData(2),
                  storagePerCycle = privatizeData(3),
                  adjLowIdx       = privatizeData(4),
                  // could include these in privatizeData
                  blockSizePos  = privDist.blockSizePos,
                  numLocalesPos = privDist.numLocalesPos,
                  cycleSizePos  = privDist.cycleSizePos);
}

proc BlockCyclic1dom.dsiGetReprivatizeData1d() {
  return (wholeR, wholeRstrideAbs, storagePerCycle);
}

proc BlockCyclic1dom.dsiReprivatize1d(reprivatizeData) {
  this.wholeR          = reprivatizeData(1);
  this.wholeRstrideAbs = reprivatizeData(2);
  this.storagePerCycle = reprivatizeData(3);
}

proc BlockCyclic1dom.dsiUsesLocalLocID1d() param return false;

proc BlockCyclic1dom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

/////////// privatization - end


// Check all restrictions/assumptions that must be satisfied by the user
// when initializing a 1-d BlockCyclic distribution.
inline proc BlockCyclicDim.checkInvariants() {
  assert(blockSize > 0, "BlockCyclic1d-blockSize");
  assert(numLocales > 0, "BlockCyclic1d-numLocales");
}

proc BlockCyclicDim.toString()
  return "BlockCyclicDim(" + numLocales:string + ", " +
         lowIdx:string + ", " + blockSize:string + ")";

// Assert that the value of 'src' is preserved when casting it to 'destT'.
// Note: customized error message for the use in dsiNewRectangularDom1d().
inline proc _checkFitsWithin(src: integral, type destT)
  where isIntegralType(destT)
{
  inline proc ensure(arg:bool) {
    if !arg then halt("When creating a domain mapped using DimensionalDist2D with a BlockCyclicDim specifier, could not fit BlockCyclicDim's adjusted lowIdx of ", src, " in the domain's idxType ", destT:string);
  }
  type maxuT = uint(64); // the largest unsigned type
  type srcT = src.type;
  proc numMantBits(type T) param
    return numBits(T) - if isIntType(T) then 1 else 0;

  if isUintType(destT) {
    ensure(isNonnegative(src));
    if numMantBits(srcT) > numMantBits(destT) then
      ensure(src:maxuT <= max(destT):maxuT);

  } else {
    // destT is signed
    if isUintType(srcT) && numMantBits(srcT) > numMantBits(destT) then
      ensure(src:maxuT <= max(destT):maxuT);
    if isIntType(srcT) && numBits(destT) < numBits(srcT) {
      ensure(src <= max(destT):srcT);
      ensure(src >= min(destT):srcT);
    }
  }
}

proc BlockCyclicDim.dsiNewRectangularDom1d(type idxType, param stridable: bool,
                                           type stoIndexT)
{
  checkInvariants();
  const lowIdxDom = this.lowIdx;

  // Allow for idxType and/or stoIndexT to be unsigned, by replacing
  //  ind0 = ind -lowIdx  -->  ind0 = ind + adjLowIdx
  //
  // where adjLowIdx is
  //  -lowIdx whenever possible (more natural for debugging), otherwise
  //  -lowIdx shifted up by a multiple of cycleSize until it is >=0.
  //
  // Motivation: we want _dsiInd0 to return idxType.
  // Alternatively we could switch all arithmetic in BlockCyclicDim
  // to int(64) and avoid the adjustment.
  //
  proc adjustLowIdx() {
    const offset = modP2(lowIdxDom, cycleSizePos);
    return
      if offset.type == uint then (cycleSizePos:uint) - offset
      else                         cycleSizePos - offset;
  }

  // Do we need to care about isIntType(stoIndexT) ?
  const adjLowIdx =
    if isIntType(idxType)
    then
      -lowIdxDom
    else
      if lowIdxDom <= 0 then -lowIdxDom else adjustLowIdx()
    ;

  if BlockCyclicDim_printAdjustedLowIdx then writeln("BlockCyclicDim(numLocales=", numLocales, ", lowIdx=", lowIdx, " blockSize=", blockSize, ").dsiNewRectangularDom1d(idxType=", idxType:string, "): adjusted lowIdx = ", adjLowIdx);

  _checkFitsWithin(adjLowIdx, idxType);

  const result = new BlockCyclic1dom(idxType = idxType,
                  stoIndexT = stoIndexT,
                  stridable = stridable,
                  adjLowIdx = adjLowIdx: idxType,
                  blockSizePos  = this.blockSizePos,
                  numLocalesPos = this.numLocalesPos,
                  cycleSizePos  = this.cycleSizePos);

  return result;
}

proc BlockCyclic1dom.dsiIsReplicated1d() param return false;

proc BlockCyclic1dom.dsiNewLocalDom1d(type stoIndexT, locId: locIdT) {
  const result = new BlockCyclic1locdom(idxType = this.idxType,
                             stoIndexT = stoIndexT,
                             locId = locId);
  return result;
}

/////////////////////////////////

/* The following comment:
- reviews the math for BlockCyclic, for a single dimension;
- motivates some functions used in the implementation;
- defines the mapping from user to storage index space.

*** Given:

distribution parameters:
 lowIdx
 blockSize >= 1
 numLocales >= 1

a range 'whole' with parameters (aligned):
 wLo
 wHi
 wSt

user's domain index (a member of 'whole'):
 i
 i0 = (i-lowIdx)  - "zero-based" index
 (the formulas assume 1:1 correspondence between i and i0, for brevity)

 note:  wLo <= i <= wHi
        advanced: i = wLo + iSt * |wSt| where iSt - a non-negative integer

*** Notation:

 floor(a,b) = floor((real)a/(real)b)
 a div b = { assert a>=0 && b>=0; return floor(a,b); }
 a mod b = { assert b >= 0; return a - b*floor(a,b); }
 "advanced" = "skip upon first reading"

*** Define the "cycle" of indices that starts at lowIdx, traverses
each locale 0..#numLocales, while traversing offsets 0..#blockSize
on each locale, then starts over:

 cycSize = blockSize * numLocales
 cycNo(i) = floor(i0,cycSize)
 cycOff(i) = i0 mod cycSize

 note:  cycNo(wLo) <= cycNo(i) <= cycNo(wHi)
        0 <= cycOff(i) < cycSize

 // the locale number that hosts 'i', aka locId or "block number" blkNum
 locNo(i) = cycOff(i) div blockSize

 note:  0 <= locNo(i) < numLocales

 // position of 'i' within the locale, aka "block offset" blkOff
 locOff(i) = cycOff(i) mod blockSize

 note: 0 <= locOff(i) < blockSize

 advanced property:
   If i1 and i2 are members of 'whole'
     and fall on the same cycle and on the same locale
     (i.e. cycNo and locNo are the same),
   then
     (locOff(i1) div |wSt|) vs. (locOff(i2) div |wSt|) are distinct
       IFF
     i1 vs. i2 are distinct.

 advanced proof:
   In general,
     i1 == lowIdx + cycNo(i1)*cycSize + locNo(i1)*blockSize + locOff(i1)
     i1 == wLo + i1St * |wSt| for some integer i1St
   then
     locOff(i1) div |wSt| == (i1A + i1St * |wSt|) div |wSt| == i1B + i1St
   where
     i1A = wLo - (lowIdx + cycNo(i1)*cycSize + locNo(i1)*blockSize)
     i1B = i1A div |wSt|

   likewise
     locOff(i2) div |wSt| = i2B + i2St

   Note i1B==i2B - because cycNo and locNo are the same for i1 and i2.
   Note i1==i2 IFF i1St==i2St - because of the definition of i1St, i2St.
   The property, then, follows.

*** Assign each index of 'whole' to "storage" on its locale as follows:

 // the pair (locNo(j), storageOff(j)) is unique for each integer j
 storageOff(i) = cycNo(i) * blockSize + locOff(i)

Advanced: compress the storage based on the above advanced property,
which implies that:
 the pair (locNo(i), storageOff(i) div |wSt|) is unique
 for each 'i' - member of 'whole'.

 storageIdx(i) = cycNo(i) * storagePerCycle + (locOff(i) div |wSt|)

where storagePerCycle is determined to ensure uniqueness of storageIdx(i)

 storagePerCycle
   = 1 + max(locOff(i) div |wSt|) for any i s.t.
                                  whole.contains(i)==true and locNo(i) is fixed
   approximated as: 1 + ( (max locOff(i) for any i) div |wSt| )
   = 1 + ((blockSize-1) div |wSt|)

*** Advanced: replacing mod with Chapel's %.

Calculate i0 using the following:

 i0 = i - lowIdx + cycSize * cycAdj
   choosing any fixed integer cycAdj >= ceil( (lowIdx-wLo), cycSize )

This guarantees i0>=0, but modifies cycNo(i) by cycAdj.
The latter is acceptable for our purposes.
Having i0>=0 ensures that (i0 mod cycSize) == (i0 % cycSize).

Additional consideration: for any given i, we want cycNo(i)
to stay the same throughout the life of a domain descriptor.
(This is so that our storage indices remain consistent - which is
useful to implement Chapel's preservation of array contents upon
domain assignments.)
This implies that the same cycAdj should accommodate wLo for any
domain bounds that can be assigned to our domain descriptor.
That may not be convenient in practice.
*/

/* do not use the above comment for chpldoc */

inline proc BlockCyclic1dom._dsiInd0(ind: idxType): idxType
  return ind + adjLowIdx;

inline proc BlockCyclic1dom._dsiCycNo(ind: idxType)
  return divfloorP2(_dsiInd0(ind), cycleSizePos): idxType;

inline proc BlockCyclic1dom._dsiCycOff(ind: idxType)
  return modP2(_dsiInd0(ind), cycleSizePos): bcdPosInt;

// "formula" in the name emphasizes no sanity checking
inline proc BlockCyclic1dom._dsiLocNo_formula(ind: idxType): locIdT {
  // keep in sync with BlockCyclicDim.dsiIndexToLocale1d()
  const ind0 = _dsiInd0(ind);
  return
    if isNonnegative(ind0) then remP2(divP2(ind0, blockSizePos), numLocalesPos): locIdT
    else  modP2(divfloorP2(ind0, blockSizePos), numLocalesPos): locIdT
    ;
}

inline proc BlockCyclic1dom._dsiLocOff(ind: idxType)
  return ( _dsiCycOff(ind) % blockSizePos ): stoIndexT;

// hoist some common code
inline proc BlockCyclic1dom._dsiStorageIdx2(cycNo, locOff)
  return mulP2(cycNo, storagePerCycle) + _divByStride(locOff);

// "formula" in the name implies no sanity checking
// in particular at the moment its type may not be stoIndexT
inline proc BlockCyclic1dom._dsiStorageIdx_formula(ind: idxType)
  return _dsiStorageIdx2(_dsiCycNo(ind), _dsiLocOff(ind));

inline proc BlockCyclic1dom._dsiStorageIdx(ind: idxType)
  return _dsiStorageIdx_formula(ind): stoIndexT;

// oblivious of 'wholeR'
inline proc BlockCyclic1dom._dsiIndicesOnCycLoc(cycNo: idxType, locNo: locIdT,
                                                param zerobased)
  : range(idxType)
{
  const startCycle = mulP2(cycNo, cycleSizePos) -
    // cycNo*cycleSize may be <adjLowIdx, subtraction would wrap for unsigned
    if zerobased then 0 else adjLowIdx;
  const startLoc = startCycle:idxType + locNo:idxType * blockSizePos:idxType;
  return startLoc..#blockSizePos:idxType;
}

// Support mixing uint and int.
inline proc mulP2(m: integral, n: bcdPosInt)
  return if m.type == uint then m * (n:uint)
  else                          m * n;
inline proc divP2(m: integral, n: bcdPosInt): m.type
  return if m.type == uint then m / (n:uint)
  else                         (m / n): m.type;
inline proc remP2(m: integral, n: bcdPosInt): m.type
  return if m.type == uint then m % (n:uint)
  else                         (m % n): m.type;

// from Math, but assuming the second arg is positive
inline proc modP2(m: integral, n: bcdPosInt) {
  const temp = remP2(m, n);

  // eliminate some run-time tests if input(s) is(are) unsigned
  return
    if true then //if isNonnegative(n) then
      if isUintType(m.type)
      then temp
      else ( if temp >= 0 then temp else temp + n )
    else
      // n < 0
      ( if temp <= 0 then temp else temp + n );
}
inline proc divfloorP2(m: integral, n: bcdPosInt) return
  if isNonnegative(m) then
    if true             then divP2(m, n)
    else                     (m - n - 1) / n
  else
    if true             then (m - n + 1) / n
    else                     m / n;

/////////////////////////////////

proc BlockCyclicDim.dsiIndexToLocale1d(ind): locIdT {
  // keep in sync with BlockCyclic1dom._dsiLocNo_formula
  const ind0 = ind - lowIdx;
  const locNo =
    if ind0 >= 0 then ( (ind0 / blockSize) % numLocales ): locIdT
    else  modP2(divfloorP2(ind0, blockSizePos), numLocalesPos): locIdT
    ;

  assert(0 <= locNo && locNo < numLocales);
  // todo: the following assert should not be needed - it can be proven
  assert(locNo == modP2(ind0, cycleSizePos) / blockSize);

  return locNo;
}

// allow uint(64) indices, but assert that they fit in int(64)
inline proc BlockCyclicDim.dsiIndexToLocale1d(ind: uint(64)): locIdT {
  type convT = int(64);
  assert(ind <= max(convT));
  return dsiIndexToLocale1d(ind:convT);
}

proc BlockCyclic1dom.dsiSetIndices1d(rangeArg: rangeT): void {
  // For now, require the user to provide unambiguous ranges only.
  // This requirement could potentially be avoided (as long as no arrays
  // are declared over the domain), but it simplifies/speeds up our code.
  //
  // todo: document this in the spec for this distribution.
  // see also an assert is dsiSetLocalIndices1d()
  assert(!rangeArg.isAmbiguous());

  const prevStoragePerCycle = storagePerCycle;
  dsiSetIndicesUnimplementedCase = false;

  // As of this writing, alignedLow/High are valid even for empty ranges
  if stridable {
    wholeR = rangeArg.alignedLow..rangeArg.alignedHigh by rangeArg.stride;
    wholeRstrideAbs = abs(rangeArg.stride): idxType;
    storagePerCycle = (1 + divP2(blockSizePos-1, wholeRstrideAbs:int)): bcdPosInt;
  } else {
    assert(rangeArg.stride == 1); // ensures we can simplify things
    wholeR = rangeArg.alignedLow..rangeArg.alignedHigh;
    wholeRstrideAbs = 0; // be sure nobody ever reads this
    storagePerCycle = blockSizePos;
  }

  if prevStoragePerCycle != 0 && storagePerCycle != prevStoragePerCycle then
    dsiSetIndicesUnimplementedCase = true;
}

inline proc BlockCyclic1dom._divByStride(locOff)  return
  if stridable then ( locOff / wholeRstrideAbs ): stoIndexT
  else              locOff: stoIndexT;

// _dsiStorageLow(), _dsiStorageHigh(): save a few mods and divisions
// at the cost of potentially allocating more storage
inline proc BlockCyclic1dom._dsiStorageLow(locId: locIdT): stoIndexT {
  const lowW = wholeR.low;

  // smallest cycNo(i) for i in wholeR
  var   lowCycNo  = _dsiCycNo(lowW): stoIndexT;
  const lowLocNo  = _dsiLocNo_formula(lowW);
  var   adjLowIdx = 0: stoIndexT;

  // (Optional) tighten the storage if wholeR
  // does not fall on this locale in the lowest cycle.
  if lowLocNo > locId then
    lowCycNo += 1;
  else
    // (Optional) tighten the storage if wholeR
    // starts on this locale, but not at the beginning of it.
    if lowLocNo == locId then
      adjLowIdx = _divByStride(_dsiLocOff(lowW));

  return lowCycNo * storagePerCycle:stoIndexT + adjLowIdx;
}

inline proc BlockCyclic1dom._dsiStorageHigh(locId: locIdT): stoIndexT {
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

proc BlockCyclic1locdom.dsiSetLocalIndices1d(globDD, locId: locIdT): range(stoIndexT) {
  const stoLow = globDD._dsiStorageLow(locId);
  const stoHigh = globDD._dsiStorageHigh(locId);

  return stoLow:stoIndexT .. stoHigh:stoIndexT;
}

proc BlockCyclic1dom.dsiStorageUsesUserIndices() param return false;

proc BlockCyclic1dom.dsiAccess1d(ind: idxType): (locIdT, stoIndexT) {
  return (_dsiLocNo_formula(ind), _dsiStorageIdx(ind));
}

proc _bcddb(args...) { /* writeln((...args)); */ }

iter BlockCyclic1locdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
  param zbased = isUintType(idxType);
// todo: for the special case handled in dsiMyDensifiedRangeForTaskID1d,
// maybe handling it here will be beneficial, too?
  const locNo = this.locId;
  const wholeROrig = globDD.wholeR;
  const wholeR  = globDD.wholeR +
    if zbased then globDD.adjLowIdx else 0:idxType;
  const lowIdx  = wholeROrig.low;
  const highIdx = wholeROrig.high;
  type retT = dsiMyDensifiedRangeType1d(globDD);
  param stridable = globDD.stridable;
  compilerAssert(stridable == wholeR.stridable); // sanity

  _bcddb("\n", "dsiMyDensifiedRangeForSingleTask1d ",
         "{ wholeR ",    globDD.wholeR,
         "  adjLowIdx ", globDD.adjLowIdx,
         "  cycleSize ", globDD.cycleSizePos,
         " }\n",
         "  locNo ", locNo,
         "  wholeR ", wholeR,
         "  lowIdx ", lowIdx,
         "  highIdx ", highIdx,
         "");

  if lowIdx > highIdx then
    return;

  const lowCycNo = globDD._dsiCycNo(lowIdx);
  const highCycNo = globDD._dsiCycNo(highIdx);
  const up = wholeR.stride > 0;
  _bcddb("  lowCycNo ", lowCycNo, "  highCycNo ", highCycNo);
  assert(lowIdx <= highIdx);

  // Right now explicit cast range(64) to range(32) is not implemented.
  // We are doing it by hand here. Cf. proc =(range, range).
  proc rangecast(out r1: range(?), r2: range(?)): void {
    compilerAssert(r1.boundedType == r2.boundedType);
    if !r1.stridable && r2.stridable && r2._stride != 1 then
      halt("range with non-unit stride is cast to non-stridable range");
    r1._low       = r2._low: r1.idxType;
    r1._high      = r2._high: r1.idxType;
    if r1.stridable {
      r1._stride  = r2.stride: r1.strType;
      r1._alignment = r2._alignment: r1.idxType;
      r1._aligned = r2._aligned;
    }
  }

  // todo: make a cheaper densify() for this case, where
  // always densifyee==wholeR[smth..smthelse]
  proc mydensify(densifyee): retT {
    const temp = densify(densifyee, wholeR);
    _bcddb("  mydensify(", densifyee, ") = ", temp);
    var result: retT;
    rangecast(result, temp);
    return result;
  }

  // Cf. wholeR above. We rely on this:
  //   densify(range-adjLowIdx, wholeR) == densify(range, wholeR+adjLowIdx)
  // Need zbased==true when idxType is unsigned (see _dsiIndicesOnCycLoc).
  // I retained zbased==false, too, because it makes things more natural
  // and perhaps avoids overflow in some cases.
  var curIndices = globDD._dsiIndicesOnCycLoc(
                     if up then lowCycNo else highCycNo, locNo, zbased);
  const firstRange = wholeR[curIndices];
  _bcddb("  curIndices ", curIndices, "  firstRange ", firstRange);
  if firstRange.low <= firstRange.high then
    yield mydensify(firstRange);
  // else nothing to yield on this locale

  if lowCycNo == highCycNo then
    // we have covered all cycles
    return;

  proc advance() {
    curIndices = curIndices.translate(
      if !stridable || up then globDD.cycleSizePos else -globDD.cycleSizePos);
    _bcddb("  advance curIndices ", curIndices);
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
proc BlockCyclic1dom.dsiSingleTaskPerLocaleOnly1d()
  return !BlockCyclicDim_allowParLeader ||
         !((blockSizePos:wholeR.stride.type) == wholeR.stride);

// only works when BlockCyclic1dom.dsiSingleTaskPerLocaleOnly1d()
proc BlockCyclic1locdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int)
{
  // We better fit within this type. An explicit cast at the callsite
  // would presently fail if we don't fit.
  type resultIdxType = globDD.idxType;
  // Ensure it is the same as dsiMyDensifiedRangeType1d(globDD).idxType.
  // Have to do it a bit indirectly.
  compilerAssert(range(idxType=resultIdxType, stridable=globDD.stridable)
                 == dsiMyDensifiedRangeType1d(globDD));

  // Assume 2*numLocales always fits in 31 bits, so we can skip this check
  // for u/int(32) and larger.
  if numBits(resultIdxType) < 32 &&
     // currently need to fit 2*numLocales for computing 'AL' below
     2 * globDD.numLocalesPos:int > max(resultIdxType):int
  then halt("The BlockCycDim specifier is not implemented when twice numLocales in its dimension does not fit in the domain's idxType");

  const wholeR = globDD.wholeR;
  const nLocs  = globDD.numLocalesPos :resultIdxType;
  assert((globDD.blockSizePos:wholeR.stride.type) == wholeR.stride);
  assert(globDD.storagePerCycle == 1); // should follow from the previous

  // In this case, the densified range for *all* indices on this locale is:
  //   0..#wholeR.length by numLocales align AL
  // where
  //   (_dsiLocNo(wholeR.low) + AL) % numLocales == this.locId

  const firstLoc = globDD._dsiLocNo_formula(wholeR.low) :resultIdxType;
  const AL = this.locId :resultIdxType + (nLocs - firstLoc);

  // Here is the densified range for all indices on this locale.
  const hereDenseInds = 0:resultIdxType..#wholeR.length by nLocs align AL;

  // This is our chunk of hereDenseInds
  return RangeChunk.chunk(hereDenseInds, numTasks, taskid);
}

proc BlockCyclic1locdom.dsiMyDensifiedRangeType1d(globDD) type
  return range(idxType=globDD.idxType, stridable=globDD.stridable);

proc BlockCyclic1locdom.dsiLocalSliceStorageIndices1d(globDD, sliceRange)
  : range(stoIndexT, sliceRange.boundedType, false)
{
  if sliceRange.stridable {
    // to be done: figure out sliceRange's stride vs. globDD.wholeR.stride
    compilerError("localSlice is not implemented for the Dimensional distribution with a block-cyclic dimension specifier when the slice is stridable");
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

iter BlockCyclic1dom.dsiSerialArrayIterator1d() {
  // dispatch here, for code clarity
  if stridable then
    for result in _dsiSerialArrayIterator1dStridable() do
      yield result;
  else
    for result in _dsiSerialArrayIterator1dUnitstride(wholeR) do
      yield result;
}

iter BlockCyclic1dom._dsiSerialArrayIterator1dUnitstride(rangeToIterateOver) {
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

iter BlockCyclic1dom._dsiSerialArrayIterator1dStridable() {
  assert(stridable);
 if BlockCyclicDim_enableArrayIterWarning then
  compilerWarning("array iterator over stridable block-cyclic-dim arrays is presently not efficient", 4);

  // the simplest way out
  for ind in wholeR do
    yield (_dsiLocNo_formula(ind), _dsiStorageIdx(ind)..#(1:stoIndexT));
}

iter BlockCyclic1dom.dsiFollowerArrayIterator1d(undensRange): (locIdT, idxType) {
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
