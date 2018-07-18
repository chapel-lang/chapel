/*
 * 'mink' reduction implementation. Returns vector of k elements of type
 * eltType.
 */

class mink : ReduceScanOp {
  type eltType;
  const k: int = 10;

  // Store minimum k items as vector in descending order.
  var v: [1..k] eltType = max(eltType);

  proc accumulate(value: eltType) {
    if value <= v[1] {
      v[1] = value;
      for i in 2..k do
        if v[i-1] < v[i] then
          v[i-1] <=> v[i];
    }
  }

  proc combine(state: mink(eltType)) {
    for stateValue in state.v {
      accumulate(stateValue);
    }
  }

  proc generate() {
    return v;
  }
}
