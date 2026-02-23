/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
proc createTuple(param rank, type t, val): rank*t {
  var tup: rank*t;
  for param i in 0..rank-1 do tup(i) = val;
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
  for param i in 0..rank-1 {
    numElems *= ranges(i).sizeAs(EC);
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
  for /* param */ i in 0..rank-1 {
    const curElems = ranges(i).sizeAs(EC);
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

  if myChunk <= rem {
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
proc intCeilXDivByY(x, y) do return 1 + (x - 1)/y;

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
  for param i in 0..rank-1 do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 0;
          for i in 1..rank-1 {
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
  for i in 0..rank-1 do
    for j in i+1..rank-1 do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}

//
// Returns a new default rectangular domain of the same rank, index
// type, and shape of 'dom' but for which the indices in each
// dimension start at zero and have unit stride.
//
proc computeZeroBasedDomain(dom: domain) do
  return {(...computeZeroBasedRanges(dom.dims()))};

proc computeZeroBasedRanges(ranges: _tuple) {
  proc helper(type idxType, first, rest...) {
    if rest.size > 1 then
      return (0:idxType..#first.sizeAs(idxType), (...helper(idxType, (...rest))));
    else
      return (0:idxType..#first.sizeAs(idxType), 0:idxType..#rest(0).sizeAs(idxType));
  }
  type idxType = ranges(0).idxType;
  if ranges.size > 1 then
    return helper(idxType, (...ranges));
  else
    return (0:idxType..#ranges(0).sizeAs(idxType),);
}

// densiResult() calculates the result type of densify() and unDensify()
private proc densiResult(arg: domain, whole: domain) type
  do return domain(whole.rank, whole.idxType,
                   chpl_strideProduct(arg.dim(0), whole.dim(0)));

// returns the type of one dimension
private proc densiResult(arg: _tuple, whole: _tuple) type
  do return range(whole(0).idxType, arg(0).bounds,
       chpl_strideProduct(chpl_strideUnion(arg), chpl_strideUnion(whole)));

private proc densiResult(arg: range(?), whole: range(?)) type
  do return range(whole.idxType, arg.bounds,
                  chpl_strideProduct(arg, whole));

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

proc densify(sub: domain, whole: domain, userErrors = true
             ) : densiResult(sub, whole)
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
  _densiCheck(isRange(subs(0)), argtypes);
  _densiCheck(isRange(wholes(0)), argtypes);
  _densiEnsureBounded(subs(0));
  _densiIdxCheck(subs(0).idxType, wholes(0).idxType, argtypes);

  param rank = wholes.size;
  type IT = wholes(0).idxType;
  var result: rank * densiResult(subs, wholes);

  for param d in 0..rank-1 {
    _densiCheck(isRange(subs(d)), argtypes);
    _densiCheck(isRange(wholes(d)), argtypes);
    _densiIdxCheck(wholes(d).idxType, IT, argtypes);
    _densiEnsureBounded(subs(d));
    _densiIdxCheck(subs(d).idxType, wholes(d).idxType, argtypes);

    result(d) = densify(subs(d), wholes(d), userErrors);
  }
  return result;
}

proc densify(s: range(?,bounds=?), w: range(?IT,?), userErrors=true
             ) : densiResult(s, w)
{
  _densiEnsureBounded(s);
  _densiIdxCheck(s.idxType, IT, (s,w).type);

  proc ensure(cond, args...) {
    if userErrors { if !cond then halt((...args)); }
    else                               assert(cond, (...args));
  }

  if s.sizeAs(int) == 0 {
    return new (densiResult(s, w))();

  } else {
    ensure(w.sizeAs(uint) > 0, "densify(s=", s, ", w=", w, "): w is empty while s is not");

    var low: IT = w.indexOrder(s.first);
    ensure(low >= 0, "densify(s=", s, ", w=", w, "): s.first is not in w");

    if s.sizeAs(int) == 1 {
      // The "several indices" case should produce the same answer. We still
      // include this special (albeit infrequent) case because it's so short.
      return densiResult(s,w).createWithSingleElement(low);

    } else {
      // several indices
      var high: IT = w.indexOrder(s.last);
      ensure(high >= 0, "densify(s=", s, ", w=", w, "): s.last is not in w");

      // ensure s is a subsequence of w
      ensure(s.stride % w.stride == 0, "densify(s=", s, ", w=", w, "): s.stride is not a multiple of w.stride");
      const stride = s.stride / w.stride;

      if stride < 0 then low <=> high;

      assert(low <= high, "densify(s=", s, ", w=", w, "): got low (", low, ") larger than high (", high, ")");
      return ( low .. high by stride ) :densiResult(s,w);
    }
  }
}

// w.strides==one
// in this case densiResult(sArg,w) == range(IT,B,S)
proc densify(sArg: range(?,bounds=?B,strides=?S), w: range(?IT,?,strides=strideKind.one), userErrors=true) : range(IT,B,S)
{
  _densiEnsureBounded(sArg);
  _densiIdxCheck(sArg.idxType, IT, (sArg,w).type);
  const s = sArg:range(IT,B,S);

  proc ensure(cond) {
    if userErrors { if !cond then halt(); }
    else                               assert(cond);
  }

  // todo: account for the case ! s.isAligned()
  ensure(s.isEmpty() ||
         // If idxType is unsigned, caller must ensure that s.lowBound is big enough
         // so it can be subtracted from.
         w.lowBound <= if isIntType(IT) then s.low else s.lowBound);
  ensure(s.isEmpty() || !w.hasHighBound() || s.high <= w.highBound);

  // gotta have a special case, e.g.: s=1..0 w=5..6 IT=uint
  if isUintType(IT) && s.isEmpty() then
    return new range(IT,B,S);

  return (s - w.lowBound): range(IT,B,S);
}

proc _densiEnsureBounded(arg: range(?)) {
  if arg.bounds != boundKind.both then
    compilerError("densify() currently requires that sub-ranges be bounded", 2);
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

proc unDensify(dense: domain, whole: domain, userErrors = true
               ) : densiResult(dense, whole)
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
  _undensCheck(isRange(denses(0)), argtypes);
  _undensCheck(isRange(wholes(0)), argtypes);
  _undensEnsureBounded(denses(0));

  param rank = wholes.size;
  type IT = wholes(0).idxType;
  var result: rank * densiResult(denses, wholes);

  for param d in 0..rank-1 {
    _undensCheck(isRange(denses(d)), argtypes);
    _undensCheck(isRange(wholes(d)), argtypes);
    _undensCheck(chpl__legalIntCoerce(wholes(d).idxType, IT), argtypes);
    _undensEnsureBounded(denses(d));

    result(d) = unDensify(denses(d), wholes(d));
  }
  return result;
}

proc unDensify(dense: range(?dIT,bounds=?), whole: range(?IT,?)
               ) : densiResult(dense, whole)
{
  _undensEnsureBounded(dense);
  if whole.bounds == boundKind.neither then
    compilerError("unDensify(): the 'whole' argument must have at least one bound");

  // ensure we can call dense.first below
  if dense.sizeAs(int) == 0 then
    return new (densiResult(dense, whole))();

  if ! whole.hasFirst() then
    halt("unDensify() is invoked with the 'whole' range that has no first index");

  var low :IT = whole.orderToIndex(dense.first);
  // should we special-case dense.sizeAs(dIT)==1?
  // if dense.sizeAs(dIT) == 1 then return low .. low;
  const stride = whole.stride * dense.stride;
  var high :IT = chpl__addRangeStrides(low, stride, dense.sizeAs(dIT) - 1);
  assert(high == whole.orderToIndex(dense.last));
  if stride < 0 then low <=> high;

  assert(low <= high, "unDensify(dense=", dense, ", whole=", whole, "): got low (", low, ") larger than high (", high, ")");
  return ( low .. high by stride ) :densiResult(dense, whole);
}

// whole.strides==one
proc unDensify(dense: range(?,bounds=?B,strides=?S), whole: range(?IT,?,strides=strideKind.one)) : range(IT,B,S)
{
  if !whole.hasLowBound() then
    compilerError("unDensify(): the 'whole' argument, when not stridable, must have a low bound");

  return (dense + whole.lowBound): range(IT,B,S);
}

proc _undensEnsureBounded(arg: range(?)) {
  if arg.bounds != boundKind.both then
    compilerError("unDensify() currently requires that the densified ranges be bounded", 2);
}

proc _undensCheck(param cond, type argtypes, param errlevel = 2) {
  if !cond then compilerError("unDensify() is defined only on matching domains, ranges, and quasi-homogeneous tuples of ranges, but is invoked on ", argtypes:string, errlevel);
}


//
// allStridesArePositive avoids run-time checks as much as possible
// args can be: array, domain, or their DSI classes; must be rectangular
//
// todo: when we switch from range.stridable to an enum,
// return param false when 1+ args have compile-time negative stride(s)
//

proc chpl_allStridesArePositive(arg1, arg2, arg3, arg4) param
  where asapP1(arg1) && asapP1(arg2) && asapP1(arg3) && asapP1(arg4)
  do return true;

proc chpl_allStridesArePositive(arg1, arg2, arg3, arg4)
  do return asap1(arg1) && asap1(arg2) && asap1(arg3) && asap1(arg4);

// helpers

// asap1 = All Strides Are Positive - 1 arg
// returns a param when possible
private proc asap1(arg) param where asapP1(arg) {
  return true;
}
private proc asap1(arg) {
  if isSubtype(arg.type, _domain) then return asapTuple(arg.dims());
  if isSubtype(arg.type, _array)  then return asapTuple(arg.dims());
  if isSubtype(arg.type, BaseDom) then return asapTuple(arg.dsiDims());
  if isSubtype(arg.type, BaseArr) then return asapTuple(arg.dom.dsiDims());
  compilerError("asap1: unsupported argument type ", arg.type:string);
  return false; // otherwise we get resolution errors before the compilerError
                // above
}

// asapP1 = All Strides Are Positive - Param - 1 arg
// returns true if all strides are known to be positive at compile time
private proc asapP1(arg) param {
  return arg.strides.isPositive();
}

private proc asapTuple(dims: _tuple) {
  for d in dims do if d.hasNegativeStride() then return false;
  return true;
}


//
// setupTargetLocalesArray
//
proc setupTargetLocalesArray(ref targetLocDom, targetLocArr, specifiedLocArr) {
  param rank = targetLocDom.rank;
  if rank != 1 && specifiedLocArr.rank == 1 {
    const factors = _factor(rank, specifiedLocArr.size);
    var ranges: rank*range;
    for param i in 0..rank-1 do
      ranges(i) = 0..#factors(i);
    targetLocDom = {(...ranges)};
    targetLocArr = reshape(specifiedLocArr, targetLocDom);
  } else {
    if specifiedLocArr.rank != rank then
      compilerError("specified target array of locales must equal 1 or distribution rank");
    var ranges: rank*range;
    for param i in 0..rank-1 do
      ranges(i) = 0..#specifiedLocArr.domain.dim(i).sizeAs(int);
    targetLocDom = {(...ranges)};
    targetLocArr = specifiedLocArr;
  }
}

proc setupTargetLocRanges(param rank, specifiedLocArr) {
  var ranges: rank*range;

  if rank != 1 && specifiedLocArr.rank == 1 {
    const factors = _factor(rank, specifiedLocArr.size);
    for param i in 0..rank-1 do
      ranges(i) = 0..#factors(i);

  } else {
    if specifiedLocArr.rank != rank then
      compilerError("specified target array of locales must equal 1 or distribution rank");
    for param i in 0..rank-1 do
      ranges(i) = 0..#specifiedLocArr.domain.dim(i).sizeAs(int);
  }

  return ranges;
}

proc createWholeDomainForInds(param rank, type idxType, param strides, inds) {
  if isDomain(inds) {
    return inds;
  } else {
    var result: domain(rank, idxType, strides);
    result.setIndices(inds);
    return result;
  }
}

// Compute the active dimensions of this assignment. For example, LeftDims
// could be (1..1, 1..10) and RightDims (1..10, 1..1). This indicates that
// a rank change occurred and that the inferredRank should be '1', the
// LeftActives = (1,), the RightActives = (0,)
proc bulkCommComputeActiveDims(LeftDims, RightDims) {
  param LeftRank  = LeftDims.size;
  param RightRank = RightDims.size;
  param minRank   = min(LeftRank, RightRank);

  var inferredRank = 0;

  // Tuple used instead of an array because returning an array would
  // recursively invoke array assignment (and therefore bulk-transfer).
  var LeftActives, RightActives : minRank * int;

  var li = 0, ri = 0;
  proc advance() {
    // Advance to positions in each domain where the sizes are equal.
    while LeftDims(li).sizeAs(int) == 1 && LeftDims(li).sizeAs(uint) != RightDims(ri).sizeAs(uint) do li += 1;
    while RightDims(ri).sizeAs(int) == 1 && RightDims(ri).sizeAs(uint) != LeftDims(li).sizeAs(uint) do ri += 1;

    assert(LeftDims(li).sizeAs(uint) == RightDims(ri).sizeAs(uint));
  }

  do {
    advance();

    LeftActives(inferredRank)  = li;
    RightActives(inferredRank) = ri;

    inferredRank += 1;
    li += 1;
    ri += 1;
  } while li < LeftRank && ri < RightRank;

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
  param strides = chpl_strideUnion(targetDom, srcSlice);
  var rngs = targetDom.dims() :
             (targetDom.rank*range(targetDom.idxType,strides=strides));

  for i in 0..inferredRank-1 {
    const SD    = SrcActives(i);
    const TD    = TargetActives(i);
    const dense = densify(srcSlice.dim(SD), srcDom.dim(SD));
    rngs(TD)    = unDensify(dense, targetDom.dim(TD));
  }

  return {(...rngs)};
}

// this is an overload for views that are ranges. This will be used by code
// paths that comes from elided array views. We could consider using this
// lighter weight implementation as a more general special case for 1D bulk
// transfers.
proc bulkCommTranslateDomain(srcSlice : domain, srcView : range(?),
                             targetView : range(?)) {
  if srcSlice.rank != 1 then
    compilerError("bulkCommTranslateDomain: source slice and source domain must have identical rank");

  param strides = chpl_strideUnion(targetView, srcSlice);

  const dense = densify(srcSlice.dim(0), srcView);
  const rng  = unDensify(dense, targetView);

  return {rng};
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
  for param i in 0..rank-1 {
    const ar = AD(i), br = BD(i);
    if boundsChecking then assert(br.contains(b(i)));
    result(i) = ar.orderToIndex(br.indexOrder(b(i)));
  }
  return result;
}

// this is a 1D, range-based version of the above. It is used by AVE, which
// tries to avoid creating domains for views.
proc bulkCommConvertCoordinate(ind, bView:range(?), aView:range(?))
{
  if boundsChecking then
    assert(bView.contains(ind));

  const result = aView.orderToIndex(bView.indexOrder(ind));
  return (result,);
}

record chpl_PrivatizedDistHelper : writeSerializable {
//  type instanceType;
  var _pid:int;  // only used when privatized
  pragma "owned"
  var _instance;
  var _unowned:bool; // 'true' for the result of 'getDistribution',
                     // in which case, the record destructor should
                     // not attempt to delete the _instance.

  inline proc _value {
    if _isPrivatized(_instance) {
      return chpl_getPrivatizedCopy(_instance.type, _pid);
    } else {
      return _instance;
    }
  }

  forwarding _value except targetLocales;

  inline proc _do_destroy() {
    if ! _unowned && ! _instance.singleton() {
      on _instance {
        // Count the number of domains that refer to this distribution.
        // and mark the distribution to be freed when that number reaches 0.
        // If the number is 0, .remove() returns the distribution
        // that should be freed.
        var distToFree = _instance.remove();
        if distToFree != nil {
          _delete_dist(distToFree!, _isPrivatized(_instance));
        }
      }
    }
  }

  proc deinit() {
    _do_destroy();
  }

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         ranges: rank*range(idxType, boundKind.both, strides),
                         definedConst: bool = false) {
    var x = _value.dsiNewRectangularDom(rank, idxType, strides, ranges);

    x.definedConst = definedConst;

    if x.linksDistribution() {
      _value.add_dom(x);
    }
    return x;
  }

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         definedConst: bool = false) {
    var ranges: rank*range(idxType, boundKind.both, strides);
    return newRectangularDom(rank, idxType, strides, ranges, definedConst);
  }

  proc newAssociativeDom(type idxType, param parSafe: bool=true) {
    var x = _value.dsiNewAssociativeDom(idxType, parSafe);
    if x.linksDistribution() {
      _value.add_dom(x);
    }
    return x;
  }

  proc newSparseDom(param rank: int, type idxType, dom: domain) {
    var x = _value.dsiNewSparseDom(rank, idxType, dom);
    if x.linksDistribution() {
      _value.add_dom(x);
    }
    return x;
  }

  proc idxToLocale(ind) do return _value.dsiIndexToLocale(ind);

  @chpldoc.nodoc
  proc serialize(writer, ref serializer) throws {
    writer.write(_value);
  }

  proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

  /*
    Return an array of locales over which this distribution was declared.
  */
  proc targetLocales() const ref {
    return _value.dsiTargetLocales();
  }
}

/*
   a utility wrapper record for turning old rectangular layout classes
   into values, similar to chpl_PrivatizedDistHelper for distributions
*/
record chpl__rectLayoutHelper {
  forwarding var _value;

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         ranges: rank*range(idxType, boundKind.both, strides),
                         definedConst: bool = false) {
    var x = _value.dsiNewRectangularDom(rank, idxType, strides, ranges);

    x.definedConst = definedConst;

    if x.linksDistribution() {
      _value.add_dom(x);
    }
    return x;
  }

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         definedConst: bool = false) {
    var ranges: rank*range(idxType, boundKind.both, strides);
    return newRectangularDom(rank, idxType, strides, ranges, definedConst);
  }

  proc newSparseDom(param rank: int, type idxType, dom: domain) {
    compilerError("sparse domains not supported by this distribution");
  }

  proc newAssociativeDom(type idxType, param parSafe: bool=true) {
    compilerError("associative domains not supported by this distribution");
  }

  proc deinit() {
    on _value {
      // Count the number of domains that refer to this distribution.
      // and mark the distribution to be freed when that number reaches 0.
      // If the number is 0, .remove() returns the distribution
      // that should be freed.
      var distToFree = _value.remove();
      if distToFree != nil {
        _delete_dist(distToFree!, false);
      }
    }
  }
}

/*
   a utility wrapper record for turning old associative layout classes
   into values, similar to chpl_PrivatizedDistHelper for distributions
*/
record chpl__assocLayoutHelper {
  forwarding var _value;

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         ranges: rank*range(idxType, boundKind.both, strides),
                         definedConst: bool = false) {
    compilerError("rectangular domains not supported by this distribution");
  }

  proc newRectangularDom(param rank: int, type idxType,
                         param strides: strideKind,
                         definedConst: bool = false) {
    compilerError("rectangular domains not supported by this distribution");
  }

  proc newSparseDom(param rank: int, type idxType, dom: domain) {
    compilerError("sparse domains not supported by this distribution");
  }

  proc newAssociativeDom(type idxType, param parSafe: bool=true) {
      var x = _value.dsiNewAssociativeDom(idxType, parSafe);
      if x.linksDistribution() {
        _value.add_dom(x);
      }
      return x;
  }

  proc deinit() {
    on _value {
      // Count the number of domains that refer to this distribution.
      // and mark the distribution to be freed when that number reaches 0.
      // If the number is 0, .remove() returns the distribution
      // that should be freed.
      var distToFree = _value.remove();
      if distToFree != nil {
        _delete_dist(distToFree!, false);
      }
    }
  }
}
