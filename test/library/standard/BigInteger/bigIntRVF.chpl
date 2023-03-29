use BigInteger;
use BlockDist;
use CommDiagnostics;

proc main() {
  var D = Block.createDomain({0..#10});
  var A: [D] int = 10;
  var B: [D] int = 20;
  var C: [D] bigint;

  var alpha: bigint = 5;

  startCommDiagnostics();
  forall (aV, bV, cV) in zip(A, B, C) with (var locA = alpha) {
    cV = aV + locA * bV;
  }
  stopCommDiagnostics();
  printCommDiagnosticsTable();
}
