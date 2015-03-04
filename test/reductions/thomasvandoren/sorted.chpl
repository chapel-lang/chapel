/*
 * 'isSorted' reduction implementation. Determines whether values are sorted,
 * or not.
 */

class isSorted : ReduceScanOp {
  type eltType;
  param communicative = false;
  var status = true;
  var first, last: eltType;

  var _accumulatedFirst = false;

  proc accumulate(value: eltType) {
    if !_accumulatedFirst {
      first = value;
      last = value;
      _accumulatedFirst = true;
    }
    if last > value then
      status = false;
    last = value;
  }

  proc combine(state: sorted(eltType)) {
    status = status && state.status && last <= state.first;
    last = state.last;
  }

  proc generate() {
    return status;
  }
}
