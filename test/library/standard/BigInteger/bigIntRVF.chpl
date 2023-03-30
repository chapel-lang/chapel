use BigInteger;
use BlockDist;
use CommDiagnostics;

proc main() {
  var D = Block.createDomain({0..#10});
  var A: [D] bigint = 10;
  var B: [D] bigint = 20;
  var C: [D] bigint;

  var alpha: bigint = 5;

  startCommDiagnostics();
  forall (aV, bV, cV) in zip(A, B, C) {
    cV = aV + alpha * bV;
  }
  stopCommDiagnostics();
  printCommDiagnosticsTable();
}
