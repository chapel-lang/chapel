module ChapelAutoAggregation {
  use CommAggregation;

  pragma "aggregator generator"
  proc chpl_srcAggregatorForArr(arr) {
    return newSrcAggregator(arr.eltType);
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorForArr(arr) {
    return newDstAggregator(arr.eltType);
  }
}
