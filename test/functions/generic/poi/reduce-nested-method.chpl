/*
A simplification of 
  test/npb/cg/bradc/cg-sparse-partred.chpl
  test/reductions/partial/CSimpl.chpl
*/

module Application {
  use MyReduce;
  proc main {
    var A = [10,11], B = [2,3];
    record SimulatedForallExpr {
      proc this(idx) return A[idx]*B[idx];
    }
    writeln(plusReduce(new SimulatedForallExpr()));
  }
}

module MyReduce {
  proc plusReduce(arg) {
    return arg(0) + arg(1); // need visible functions from caller scope
  }
}
