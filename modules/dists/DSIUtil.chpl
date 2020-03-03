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

// Useful functions for implementing distributions

inline proc getDataParTasksPerLocale() {
  return dataParTasksPerLocale;
}

inline proc getDataParIgnoreRunningTasks() {
  return dataParIgnoreRunningTasks;
}

inline proc getDataParMinGranularity() {
  return dataParMinGranularity;
}

//
// return a rank*t tuple initialized to val
//
proc createTuple(param rank, type t, val) {
  var tup: rank*t;
  for param i in 1..rank do tup(i) = val;
  return tup;
}

//
// helper functions for determining the number of chunks and the
//   dimension to chunk over
//
proc _computeChunkStuff(maxTasks, ignoreRunning, minSize, ranges,
                        param adjustToOneDim = true): (int,int)
{
  param rank=ranges.size;
  type EC = uint; // type for element counts
  var numElems = 1:EC;
  for param i in 1..rank do {
    numElems *= ranges(i).size:EC;
  }

  var numChunks = _computeNumChunks(maxTasks, ignoreRunning, minSize, numElems);
  if numChunks == 0 then
    return (0,-1);
  assert(numChunks > 0);

  // Dimension to parallelize (eventually should "block" the space)
  var parDim = -1;
  var maxDim = -1;
  var maxElems = min(EC);
  // break/continue don't work with param loops (known future)
  for /* param */ i in 1..rank do {
    const curElems = ranges(i).size:EC;
    if curElems >= numChunks:EC {
      parDim = i;
      break;
    }
    if curElems > maxElems {
      maxElems = curElems;
      maxDim = i;
    }
  }

  if parDim == -1 {
    parDim = maxDim;

    // In those cases where parallelization is done over a single dimension
    // (which will be parDim), ensure these are no extraneous chunks.
    if adjustToOneDim && maxElems < numChunks:EC then
      numChunks = maxElems:int;
  }

  return (numChunks, parDim);
}

// returns 0 if no numElems <= 0
proc _computeNumChunks(maxTasks, ignoreRunning, minSize, numElems): int {
  if numElems <= 0 then
    return 0;

  type EC = uint; // type for element counts
  const unumElems = numElems:EC;
  var numChunks = maxTasks:int;
  if !ignoreRunning {
    const otherTasks = here.runningTasks() - 1; // don't include self
    numChunks = if otherTasks < maxTasks
      then (maxTasks-otherTasks):int
      else 1;
  }

  if minSize > 0 then
    // This is approximate
    while (unumElems < (minSize*numChunks):EC) && (numChunks > 1) {
        numChunks -= 1;
    }

  if numChunks:EC > unumElems then numChunks = unumElems:int;

  return numChunks;
}

// How many tasks should be spawned to service numElems elements.
proc _computeNumChunks(numElems): int {
  // copy some machinery from DefaultRectangularDom
  var numTasks = if dataParTasksPerLocale==0
                 then here.maxTaskPar
                 else dataParTasksPerLocale;
  var ignoreRunning = dataParIgnoreRunningTasks;
  var minIndicesPerTask = dataParMinGranularity;
  var numChunks = _computeNumChunks(numTasks, ignoreRunning,
                                    minIndicesPerTask, numElems);
  return numChunks;
}

// Divide 1..numElems into (almost) equal numChunk pieces
// and return myChunk-th piece.
proc _computeChunkStartEnd(nElems, nChunks, myCnk): 2*nElems.type {
  // the type for intermediate computations
  type IT = if nElems.type == uint then uint else int;
  const (numElems, numChunks, myChunk) = (nElems:IT, nChunks:IT, myCnk:IT);
  // the result type
  type RT = nElems.type;

  var div = numElems / numChunks;
  var rem = numElems % numChunks;

  // Caller's responsibility.
  assert(1 <= myChunk && myChunk <= numChunks);

  if myChunk <= rem then {
    // (div+1) elements per chunk
    var endIx = myChunk * (div + 1);
    //writeln("_computeChunkStartEnd", (numElems, numChunks, myChunk),
    // " = ", endIx - div, "..", endIx);
    return ((endIx - div):RT, (endIx):RT);
  } else {
    // (div) elements per chunk
    var startIx1 = numElems - (numChunks - myChunk + 1) * div;
    //writeln("_computeChunkStartEnd", (numElems, numChunks, myChunk),
    // " = ", startIx1 + 1, "..", startIx1 + div);
    return ((startIx1 + 1):RT, (startIx1 + div):RT);
  }
}

//
// helper function for blocking index ranges
//
proc intCeilXDivByY(x, y) return 1 + (x - 1)/y;

proc _computeBlock(numelems, numblocks, blocknum, wayhi,
                  waylo=0:wayhi.type, lo=0:wayhi.type) {
  if numelems == 0 then
    return (1:lo.type, 0:lo.type);

  const blo =
    if blocknum == 0 then waylo
    else lo + intCeilXDivByY(numelems:uint * blocknum:uint, numblocks:uint):lo.type;
  const bhi =
    if blocknum == numblocks - 1 then wayhi
    else lo + intCeilXDivByY(numelems:uint * (blocknum+1):uint, numblocks:uint):lo.type - 1;

  return (blo, bhi);
}

//
// naive routine for dividing numLocales into rank factors
//
proc _factor(param rank: int, value) {
  var factors: rank*int;
  for param i in 1..rank do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 1;
          for i in 2..rank {
            if factors(i) < factors(j) then
              j = i;
          }
          factors(j) *= i;
          iv = iv / i;
          break;
        }
      }
    }
  }
  for i in 1..rank do
    for j in i+1..rank do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}

//
// Returns a new default rectangular domain of the same rank, index
// type, and shape of 'dom' but for which the indices in each
// dimension start at zero and have unit stride.
//
proc computeZeroBasedDomain(dom: domain)
  return {(...computeZeroBasedRanges(dom.dims()))};

proc computeZeroBasedRanges(ranges: _tuple) {
  proc helper(type idxType, first, rest...) {
    if rest.size > 1 then
      return (0:idxType..#first.size:idxType, (...helper(idxType, (...rest))));
    else
      return (0:idxType..#first.size:idxType, 0:idxType..#rest(1).size:idxType);
  }
  type idxType = ranges(1).idxType;
  if ranges.size > 1 then
    return helper(idxType, (...ranges));
  else
    return (0:idxType..#ranges(1).size:idxType,);
}

//
// densify(): returns a DSI densification of a
// sub-domain / tuple of ranges / a single range w.r.t.
// the "whole" (of same kind).
//
// The caller must ensure the sub's indices are a subset
// of the whole's. The sub ranges presently must be bounded.
//
// The returned domains/ranges are always stridable
// (because we cannot discern non-stridability at compile time).
//
// densify(dom,dom) should return the same result as
// computeZeroBasedDomain(dom).
//
// userErrors indicates whether errors (such as 'sub' is not a
// subset of 'whole') should be reported with halt() (if true)
// or with assert() (if false).
//

// would like 'whole: domain(?IT,?r,?)'
proc densify(sub: domain, whole: domain, userErrors = true) : domain(whole.rank, whole.idxType, true)
{

  type argtypes = (sub, whole).type;
  _densiCheck(sub.rank == whole.rank, argtypes);
  _densiIdxCheck(sub.idxType, whole.idxType,  argtypes);
  return {(...densify(sub.dims(), whole.dims(), userErrors))};
}

// the desired type of 'wholes': ?rank * range(?IT,?,?)
proc densify(subs, wholes, userErrors = true)
  where isTuple(subs) && isTuple(wholes)
{
  type argtypes = (subs, wholes).type;
  _densiCheck(wholes.size == subs.size, argtypes);
  _densiCheck(isRange(subs(1)), argtypes);
  _densiCheck(isRange(wholes(1)), argtypes);
  _densiEnsureBounded(subs(1));
  _densiIdxCheck(subs(1).idxType, wholes(1).idxType, argtypes);

  param rank = wholes.size;
  type IT = wholes(1).idxType;
  var result: rank * range(IT, BoundedRangeType.bounded, true);

  for param d in 1..rank {
    _densiCheck(isRange(subs(d)), argtypes);
    _densiCheck(isRange(wholes(d)), argtypes);
    _densiIdxCheck(wholes(d).idxType, IT, argtypes);
    _densiEnsureBounded(subs(d));
    _densiIdxCheck(subs(d).idxType, wholes(d).idxType, argtypes);

    result(d) = densify(subs(d), wholes(d), userErrors);
  }
  return result;
}

proc densify(s: range(?,boundedType=?B), w: range(?IT,?,stridable=true), userErrors=true) : range(IT,B,true)
{
  _densiEnsureBounded(s);
  _densiIdxCheck(s.idxType, IT, (s,w).type);

  proc ensure(cond, args...) {
    if userErrors then { if !cond then halt((...args)); }
    else                               assert(cond, (...args));
  }

  if s.size == 0 {
    return 1:IT .. 0:IT;

  } else {
    ensure(w.size > 0, "densify(s=", s, ", w=", w, "): w is empty while s is not");

    var low: IT = w.indexOrder(s.first);
    ensure(low >= 0, "densify(s=", s, ", w=", w, "): s.first is not in w");

    if s.size == 1 {
      // The "several indices" case should produce the same answer. We still
      // include this special (albeit infrequent) case because it's so short.
      return low .. low;

    } else {
      // several indices
      var high: IT = w.indexOrder(s.last);
      ensure(high >= 0, "densify(s=", s, ", w=", w, "): s.last is not in w");

      // ensure s is a subsequence of w
      ensure(s.stride % w.stride == 0, "densify(s=", s, ", w=", w, "): s.stride is not a multiple of w.stride");
      const stride = s.stride / w.stride;

      if stride < 0 then low <=> high;

      assert(low <= high, "densify(s=", s, ", w=", w, "): got low (", low, ") larger than high (", high, ")");
      return low .. high by stride;
    }
  }
}

proc densify(sArg: range(?,boundedType=?B,stridable=?S), w: range(?IT,?,stridable=false), userErrors=true) : range(IT,B,S)
{
  _densiEnsureBounded(sArg);
  _densiIdxCheck(sArg.idxType, IT, (sArg,w).type);
  const s = sArg:range(IT,B,S);

  proc ensure(cond) {
    if userErrors then { if !cond then halt(); }
    else                               assert(cond);
  }

  // todo: account for the case s.isAmbiguous()
  ensure(s.isEmpty() ||
         // If idxType is unsigned, caller must ensure that s.low is big enough
         // so it can be subtracted from.
         w.low <= if isIntType(IT) then s.alignedLow else s.low);
  ensure(s.isEmpty() || !w.hasHighBound() || s.alignedHigh <= w.high);

  // gotta have a special case, e.g.: s=1..0 w=5..6 IT=uint
  if isUintType(IT) && s.isEmpty() then
    return 1:IT..0:IT;
    
  return (s - w.low): range(IT,B,S);
}

proc _densiEnsureBounded(arg) {
  if !isBoundedRange(arg) then compilerError("densify() currently requires that sub-ranges be bounded", 2);
}

// not sure what kind of relationship we want to enforce
proc _densiIdxCheck(type subIdxType, type wholeIdxType, type argtypes) {
  _densiCheck(chpl__legalIntCoerce(subIdxType, wholeIdxType), argtypes, errlevel=3);
}

proc _densiCheck(param cond, type argtypes, param errlevel = 2) {
  if !cond then compilerError("densify() is defined only on matching domains, ranges, and quasi-homogeneous tuples of ranges (except stridability and range boundedness do not need to match), but is invoked on ", argtypes:string, errlevel);
}

//
// unDensify: reverse the densification of a range:
//   unDensify( densify(sub,whole), whole) == sub
//
// We have to declare the results stridable because we cannot assure
// at compile time that they won't be.
//

proc unDensify(dense: domain, whole: domain, userErrors = true) : domain(whole.rank, whole.idxType, true)
{
  type argtypes = (dense, whole).type;
  _undensCheck(dense.rank == whole.rank, argtypes);
  return {(...unDensify(dense.dims(), whole.dims(), userErrors))};
}

// the desired type of 'wholes': ?rank * range(?IT,?,?)
proc unDensify(denses, wholes, userErrors = true)
  where isTuple(denses) && isTuple(wholes)
{
  type argtypes = (denses, wholes).type;
  _undensCheck(wholes.size == denses.size, argtypes);
  _undensCheck(isRange(denses(1)), argtypes);
  _undensCheck(isRange(wholes(1)), argtypes);
  _undensEnsureBounded(denses(1));

  param rank = wholes.size;
  type IT = wholes(1).idxType;
  var result: rank * range(IT, BoundedRangeType.bounded, true);

  for param d in 1..rank {
    _undensCheck(isRange(denses(d)), argtypes);
    _undensCheck(isRange(wholes(d)), argtypes);
    _undensCheck(chpl__legalIntCoerce(wholes(d).idxType, IT), argtypes);
    _undensEnsureBounded(denses(d));

    result(d) = unDensify(denses(d), wholes(d));
  }
  return result;
}

proc unDensify(dense: range(?,boundedType=?B), whole: range(?IT,?,stridable=true)) : range(IT,B,true)
{
  _undensEnsureBounded(dense);
  if whole.boundedType == BoundedRangeType.boundedNone then
    compilerError("unDensify(): the 'whole' argument must have at least one bound");

  // ensure we can call dense.first below
  if dense.size == 0 then
    return 1:IT .. 0:IT;

  if ! whole.hasFirst() then
    halt("unDensify() is invoked with the 'whole' range that has no first index");

  var low :IT = whole.orderToIndex(dense.first);
  // should we special-case dense.size==1?
  // if dense.size == 1 then return low .. low;
  const stride = whole.stride * dense.stride;
  var high :IT = chpl__addRangeStrides(low, stride, dense.size - 1);
  assert(high == whole.orderToIndex(dense.last));
  if stride < 0 then low <=> high;

  assert(low <= high, "unDensify(dense=", dense, ", whole=", whole, "): got low (", low, ") larger than high (", high, ")");
  return low .. high by stride;
}

proc unDensify(dense: range(?,boundedType=?B,stridable=?S), whole: range(?IT,?,stridable=false)) : range(IT,B,S)
{
  if !whole.hasLowBound() then
    compilerError("unDensify(): the 'whole' argument, when not stridable, must have a low bound");

  return (dense + whole.low): range(IT,B,S);
}

proc _undensEnsureBounded(arg) {
  if !isBoundedRange(arg) then compilerError("unDensify() currently requires that the densified ranges be bounded", 2);
}

proc _undensCheck(param cond, type argtypes, param errlevel = 2) {
  if !cond then compilerError("unDensify() is defined only on matching domains, ranges, and quasi-homogeneous tuples of ranges, but is invoked on ", argtypes:string, errlevel);
}


//
// setupTargetLocalesArray
//
proc setupTargetLocalesArray(ref targetLocDom, targetLocArr, specifiedLocArr) {
  param rank = targetLocDom.rank;
  if rank != 1 && specifiedLocArr.rank == 1 {
    const factors = _factor(rank, specifiedLocArr.numElements);
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#factors(i);
    targetLocDom = {(...ranges)};
    targetLocArr = reshape(specifiedLocArr, targetLocDom);
  } else {
    if specifiedLocArr.rank != rank then
      compilerError("specified target array of locales must equal 1 or distribution rank");
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#specifiedLocArr.domain.dim(i).size;
    targetLocDom = {(...ranges)};
    targetLocArr = specifiedLocArr;
  }
}

// Compute the active dimensions of this assignment. For example, LeftDims
// could be (1..1, 1..10) and RightDims (1..10, 1..1). This indicates that
// a rank change occurred and that the inferredRank should be '1', the
// LeftActives = (2,), the RightActives = (1,)
proc bulkCommComputeActiveDims(LeftDims, RightDims) {
  param LeftRank  = LeftDims.size;
  param RightRank = RightDims.size;
  param minRank   = min(LeftRank, RightRank);

  var inferredRank = 0;

  // Tuple used instead of an array because returning an array would
  // recursively invoke array assignment (and therefore bulk-transfer).
  var LeftActives, RightActives : minRank * int;

  var li = 1, ri = 1;
  proc advance() {
    // Advance to positions in each domain where the sizes are equal.
    while LeftDims(li).size == 1 && LeftDims(li).size != RightDims(ri).size do li += 1;
    while RightDims(ri).size == 1 && RightDims(ri).size != LeftDims(li).size do ri += 1;

    assert(LeftDims(li).size == RightDims(ri).size);
  }

  do {
    advance();
    inferredRank += 1;

    LeftActives(inferredRank)  = li;
    RightActives(inferredRank) = ri;

    li += 1;
    ri += 1;
  } while li <= LeftRank && ri <= RightRank;

  return (LeftActives, RightActives, inferredRank);
}

// Translates the indices of 'srcSlice' from the context of 'srcDom' to the
// context of 'targetDom'.
//
// The rank of 'srcDom' may differ from that of 'targetDom' if a rank change is
// involved.
//
// Note: Assumes that the total number of indices is the same in both 'srcDom'
// and 'targetDom'
proc bulkCommTranslateDomain(srcSlice : domain, srcDom : domain, targetDom : domain) {
  if srcSlice.rank != srcDom.rank then
    compilerError("bulkCommTranslateDomain: source slice and source domain must have identical rank");

  const (SrcActives, TargetActives, inferredRank) = bulkCommComputeActiveDims(srcDom.dims(), targetDom.dims());

  // Consider assignment between two rank changes:
  //   var A, B : [1..10, 1..10] int;
  //   A[1, ..] = B[.., 1];
  //
  // Here 'srcDom' will be {1..1, 1..10} and 'targetDom' will be {1..10, 1..1}.
  // 'bulkCommComputeActiveDims' will tell us which dimensions of 'srcDom' need
  // to be translated in to specific dimensions of 'targetDom'. In this case,
  // we need to translate from srcDom's second dimension into targetDom's first
  // dimension.
  //
  // The remaining dimensions in 'targetDom' are left untouched in the
  // assumption they were 'squashed' by the rank change.
  //
  // If the given slice is stridable but its context is not, then the result
  // will need to be stridable as well. For example:
  // {1..20 by 4} in {1..20} to {101..120} = {101..120 by 4}
  param needsStridable = targetDom.stridable || srcSlice.stridable;
  var rngs : targetDom.rank*range(targetDom.idxType, stridable=needsStridable);
  rngs = targetDom.dims();

  for i in 1..inferredRank {
    const SD    = SrcActives(i);
    const TD    = TargetActives(i);
    const dense = densify(srcSlice.dim(SD), srcDom.dim(SD));
    rngs(TD)    = unDensify(dense, targetDom.dim(TD));
  }

  return {(...rngs)};
}

//
// bulkCommConvertCoordinate() converts
//   point 'ind' within 'bView'
// to
//   a point within 'aView'
// that has the same indexOrder in each dimension.
//
// This function was contributed by Juan Lopez and later improved by Alberto.
// In the SBAC'12 paper it is called m(). Later modified by Ben Harshbarger
// in support of ArrayViews.
//
// TODO: assert that bView and aView are rectangular?
// TODO: If we wanted to be more general, the domain args could turn into
//       tuples of ranges
//
proc bulkCommConvertCoordinate(ind, bView:domain, aView:domain)
{
  if bView.rank != aView.rank {
    compilerError("Invalid arguments passed to bulkCommConvertCoordinate - domain ranks must match: bView.rank = ", bView.rank:string, ", aView.rank = ", aView.rank:string);
  }
  param rank = aView.rank;
  const b = chpl__tuplify(ind);
  if b.size != rank {
    param plural = if b.size == 1 then " element" else " elements";
    compilerError("Invalid arguments passed to bulkCommConvertCoordinate - expecting index with ", rank:string, " elements, got ", b.size:string, plural);
  }
  type idxType = aView.idxType;
  const AD = aView.dims();
  const BD = bView.dims();
  var result: rank * idxType;
  for param i in 1..rank {
    const ar = AD(i), br = BD(i);
    if boundsChecking then assert(br.contains(b(i)));
    result(i) = ar.orderToIndex(br.indexOrder(b(i)));
  }
  return result;
}
