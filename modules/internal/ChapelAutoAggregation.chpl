module ChapelAutoAggregation {
  use CommAggregation;

  proc chpl_srcAggregatorForArr(arr) {
    return newSrcAggregator(arr.eltType);
  }
}
