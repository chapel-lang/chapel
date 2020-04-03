/*
 * 'mini' reduction implementation. Returns tuple of two ints with minimum
 * value and its location (i.e. `(value, location)`).
 */

// This is a workaround for #9605. Instead we'd like (max(eltType(1)), 0).
// This workaround may be costly when eltType(1) is an array.
proc miniIdentity(type eltType) {
  var result: eltType;
  result(0) = max(result(0).type);
  return result;
}

class mini : ReduceScanOp {
  type eltType;
  var value = miniIdentity(eltType);

  proc identity() {
    return miniIdentity(eltType);
  }

  proc accumulateOntoState(ref state: eltType, t: eltType) {
    if t(0) < state(0) then
      state = t;
  }

  proc accumulate(t: eltType) {
    accumulateOntoState(value, t);
  }

  proc combine(other: mini(eltType)) {
    accumulate(other.value);
  }

  proc generate() {
    return value;
  }

  proc clone() {
    return new unmanaged mini(eltType=eltType);
  }
}
