
proc _tuple.withIdx(idx, mergeVal) where isHomogeneousTuple(this) {

  // FIXME this if doesn't seem to woerk as I expected
  if mergeVal.type != this[1].type then
    halt("Value to be merged doesn't match homogeneous tuple eltType");

  const defVal: mergeVal.type;
  var ret = createTuple(this.size+1, mergeVal.type, defVal);

  var partialIndexOffset = 0;
  for i in 1..ret.size {
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

  const defVal: this[1].type;
  var ret = createTuple(this.size-1, this[1].type, defVal);

  for i in 1..idx-1 do
    ret[i] = this[i];

  for i in idx+1..this.size do
    ret[i-1] = this[i];

  return ret;
}

proc __lineSliceMask(dom, param dim, idx) {

  if !isHomogeneousTuple(idx) then
    halt("Index to get line slice must be homogeneous");

  if idx[1].type != dom.idxType then 
    halt("Index to get line slice must match domains index type");

  param numIdxPre = dim - 1;
  param numIdxPost = dom.rank - dim;

  assert(numIdxPre + numIdxPost == dom.rank-1);

  var idxPre = createTuple(if numIdxPre>0 then numIdxPre else 1, 
      dom.idxType, 0:dom.idxType);
  for param i in 1..numIdxPre do
    idxPre[i] = idx[i];

  var idxPost = createTuple(if numIdxPost > 0 then numIdxPost else 1, 
      dom.idxType, 0:dom.idxType);
  for param i in 1..numIdxPost do
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
proc __faceSliceMask(dom, param exceptDim) {
  param numUbRangesPre = exceptDim - 1;
  param numUbRangesPost = dom.rank - exceptDim;

  assert(numUbRangesPre + numUbRangesPost == dom.rank-1);

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
