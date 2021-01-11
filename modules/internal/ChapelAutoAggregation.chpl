module ChapelAutoAggregation {
  use CommAggregation;

  proc chpl_srcAggregatorForArr(arr) {
    return newSrcAggregator(arr.eltType);
  }

  proc chpl_dstAggregatorForArr(arr) {
    return newDstAggregator(arr.eltType);
  }
}
