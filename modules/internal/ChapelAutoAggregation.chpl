module ChapelAutoAggregation {
  use CommAggregation;

  pragma "aggregator generator"
  proc chpl_srcAggregatorForArr(arr: []) {
    return newSrcAggregator(arr.eltType);
  }

  pragma "aggregator generator"
  proc chpl_srcAggregatorForArr(type t) {
    return newSrcAggregator(t);
  }

  pragma "aggregator generator"
  proc chpl_srcAggregatorForArr(arr) {
    return false;
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorForArr(arr: []) {
    return newDstAggregator(arr.eltType);
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorForArr(type t) {
    return newDstAggregator(t);
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorForArr(arr) {
    return false;
  }


  proc chpl__arrayIteratorYieldsLocalElements(a) param {
    if isArray(a) {
      if !isClass(a.eltType) { // I have no idea if we can do this for wide pointers
        return a.iteratorYieldsLocalElements();
      }
    }
    return false;
  }

  // make sure to resolve this, so that we can give a more meaningful error
  // message
  proc chpl__arrayIteratorYieldsLocalElements(type a) param {
    return false;
  }
}
