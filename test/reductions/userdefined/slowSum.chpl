record stateType {
  var exps: domain(int);
  var vals: [exps] real;
}

class slowSum: ReduceScanOp {
  type eltType = real;
  var  state: stateType;

  proc identity {
    assert(eltType == real, "slowSum currently only supports reals");
    return new stateType();
  }

  proc accumulateOntoState(ref state, value) {
    use SysCTypes;

    var exp: c_int;
    extern proc frexp(value: real, ref exp: c_int): real;
    frexp(value, exp);
    if (!state.exps.contains(exp)) then
      state.exps += exp;
    state.vals[exp] += value;
  }

  proc accumulate(value: eltType) {
    accumulateOntoState(state, value);
  }

  proc accumulate(other: stateType) {
    for (exp, val) in zip(other.exps, other.vals) do {
      if (!this.state.exps.contains(exp)) then
        this.state.exps += exp;
      this.state.vals[exp] += val;
    }
  }

  proc combine(other) {
    accumulate(other.state);
  }

  proc generate() {
    var sum = 0.0;
    for val in state.vals do
      sum += val;
    return sum;
  }

  proc clone() {
    return new unmanaged slowSum(eltType = eltType);
  }
}

writeln(slowSum reduce [1.0, 10e100, 1.0, -10e100]);
