class AnyEvenReduceScanOp: ReduceScanOp {
  type eltType;
  var value: bool;

  // Rely on the default value of the desired type.
  // Todo: is this efficient when that is an array?
  inline proc identity {
    return false;
  }
  inline proc accumulate(x) {
    value = value || (x % 2 == 0);
  }
  inline proc accumulateOntoState(ref state, x) {
    state = state || (x % 2 == 0);
  }
  inline proc combine(x) {
    value = value || x.value;
  }
  inline proc generate() do return value;
  inline proc clone() do return new unmanaged AnyEvenReduceScanOp(eltType=eltType);
}

class WeirdReduceScanOp: ReduceScanOp {
  type eltType;
  var value: bool;

  // Rely on the default value of the desired type.
  // Todo: is this efficient when that is an array?
  inline proc identity {
    return false;
  }
  inline proc accumulate(x) {
    value = value || (x % 2 == 0);
  }
  inline proc accumulateOntoState(ref state, x) {
    state = state || (x % 2 == 0);
  }
  inline proc combine(x) {
    value = value || x.value;
  }
  inline proc generate() do return value;
  inline proc clone() do return new unmanaged AnyEvenReduceScanOp(eltType=eltType);
}

var result = WeirdReduceScanOp reduce [1,2,3];
writeln(result);
