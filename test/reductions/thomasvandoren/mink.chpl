/*
 * 'mink' reduction implementation. Returns vector of k elements of type
 * eltType.
 */

class mink : ReduceScanOp {
  type eltType;
  const k: int = 10;

  // Store minimum k items as vector in descending order.
  var v: [1..k] eltType = max(eltType);

  proc identity {
    var v: [1..k] eltType = max(eltType); return v;
  }

  proc accumulateOntoState(ref v, value: eltType) {
    if value <= v[1] {
      v[1] = value;
      for i in 2..k do
        if v[i-1] < v[i] then
          v[i-1] <=> v[i];
    }
  }

  proc accumulate(value: eltType) {
    accumulateOntoState(v, value);
  }

  proc accumulate(accumState: []) {
    for stateValue in accumState {
      accumulate(stateValue);
    }
  }

  proc combine(state: borrowed mink(eltType)) {
    accumulate(state.v);
  }

  proc generate() {
    return v;
  }

  proc clone() {
    return new unmanaged mink(eltType=eltType, k=k);
  }
}
