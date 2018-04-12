class slowSum: ReduceScanOp {
  type eltType = real;
  var exps: domain(int);
  var vals: [exps] real;

  proc identity {
    assert(eltType == real, "slowSum currently only supports reals");
    return 0.0;
  }

  proc accumulate(value) {
    var exp: c_int;
    extern proc frexp(value: real, ref exp: c_int): real;
    frexp(value, exp);
    if (!exps.member(exp)) then
      exps += exp;
    vals[exp] += value;
  }

  proc combine(other) {
    for (exp, val) in zip(other.exps, other.vals) do {
      if (!exps.member(exp)) then
        exps += exp;
      vals[exp] += val;
    }
  }

  proc generate() {
    var sum = 0.0;
    for val in vals do
      sum += val;
    return sum;
  }
}

writeln(slowSum reduce [1.0, 10e100, 1.0, -10e100]);
