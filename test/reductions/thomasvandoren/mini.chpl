/*
 * 'mini' reduction implementation. Returns tuple of two ints with minimum
 * value and its location (i.e. `(value, location)`).
 */

class mini : ReduceScanOp {
  type eltType;
  var value: eltType = max(eltType);
  var location: int;

  proc accumulate(t: (eltType, int)) {
    if t(1) < value then
      (value, location) = t;
  }

  proc combine(state: mini(eltType)) {
    accumulate((state.value, state.location));
  }

  proc generate() {
    return (value, location);
  }
}
