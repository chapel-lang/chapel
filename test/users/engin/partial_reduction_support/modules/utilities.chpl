use DSIUtil;

proc _tuple.withIdx(idx, mergeVal) where isHomogeneousTuple(this) {

  // FIXME this if doesn't work as expected
  if mergeVal.type != this[0].type then
    compilerError("Value to be merged is not of the type of tuple components");

  const defVal: mergeVal.type;
  var ret = createTuple(this.size+1, mergeVal.type, defVal);

  var partialIndexOffset = 0;
  for i in 0..ret.size-1 {
    if i == idx {
      ret[i] = mergeVal;
      partialIndexOffset = 1;
    }
    else {
      ret[i] = this[i-partialIndexOffset];
    }
  }

  return ret;
}

proc _tuple.withoutIdx(idx) where isHomogeneousTuple(this) {

  const defVal: this[0].type;
  var ret = createTuple(this.size-1, this[0].type, defVal);

  for i in 0..idx-1 do
    ret[i] = this[i];

  for i in idx+1..this.size-1 do
    ret[i-1] = this[i];

  return ret;
}

proc lineSliceMask(dom, param dim, idx) {

  if !isTuple(idx) || !isHomogeneousTuple(idx) then
    compilerError("Index to get line slice must be a homogeneous tuple");

  if idx[0].type != dom.idxType then 
    compilerError("Index to get line slice is not of the domain's index type");

  param numIdxPre = dim - 1;
  param numIdxPost = dom.rank - dim;

  compilerAssert(numIdxPre + numIdxPost == dom.rank-1);

  var idxPre = createTuple(if numIdxPre>0 then numIdxPre else 1, 
      dom.idxType, 0:dom.idxType);
  for param i in 0..numIdxPre-1 do
    idxPre[i] = idx[i];

  var idxPost = createTuple(if numIdxPost > 0 then numIdxPost else 1, 
      dom.idxType, 0:dom.idxType);
  for param i in 0..numIdxPost-1 do
    idxPost[i] = idx[numIdxPre+1+i];

  if numIdxPre > 0 && numIdxPost > 0 {
    return ((...idxPre),..,(...idxPost));
  }
  if numIdxPre > 0 && numIdxPost <= 0 {
    return ((...idxPre),..);
  }
  if numIdxPre <= 0 && numIdxPost > 0 {
    return (..,(...idxPost));
  }
  if numIdxPre <= 0 && numIdxPost <= 0 {
    return (.., );
  }

}

// name is creepy
proc faceSliceMask(dom, param exceptDim) {
  param numUbRangesPre = exceptDim;
  param numUbRangesPost = dom.rank - (exceptDim + 1);

  compilerAssert(numUbRangesPre + numUbRangesPost == dom.rank-1);

  const ubRangesPre = createTuple(if numUbRangesPre > 0 then numUbRangesPre
      else 1, range(boundedType=BoundedRangeType.boundedNone), ..);
  const ubRangesPost = createTuple(if numUbRangesPost > 0 then numUbRangesPost
      else 1, range(boundedType=BoundedRangeType.boundedNone), ..);

  if numUbRangesPre > 0 && numUbRangesPost > 0 {
    return ((...ubRangesPre),0,(...ubRangesPost));
  }
  if numUbRangesPre > 0 && numUbRangesPost <= 0 {
    return ((...ubRangesPre),0);
  }
  if numUbRangesPre <= 0 && numUbRangesPost > 0 {
    return (0,(...ubRangesPost));
  }
  if numUbRangesPre <= 0 && numUbRangesPost <= 0 {
    return (0, );
  }
}
