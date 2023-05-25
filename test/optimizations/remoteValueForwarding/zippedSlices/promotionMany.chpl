// same thing as the "promotion" test in the same dir, just more symbols
use BlockDist;
use CommDiagnostics;

config const n = 10;

proc main() {

  var A = Block.createArray(1..n, int);
  var B = Block.createArray(1..n, int);
  var C = Block.createArray(1..n, int);
  var D = Block.createArray(1..n, int);
  var E = Block.createArray(1..n, int);
  var F = Block.createArray(1..n, int);

  var alpha = 2;

  const sliceDom = A.domain[2..n-1];
  startCommDiagnostics();

  ref As = A[sliceDom];
  ref Bs = B[sliceDom];
  ref Cs = C[sliceDom];
  ref Ds = D[sliceDom];
  ref Es = E[sliceDom];
  ref Fs = F[sliceDom];

  stopCommDiagnostics();
  writeln("3 slice creation:");
  printCommDiagnosticsTable();
  resetCommDiagnostics();

  writeln();

  startCommDiagnostics();

  As = Bs + Cs * Ds + Es + Fs / alpha;

  stopCommDiagnostics();
  writeln("Stream:");
  printCommDiagnosticsTable();

  writeln(A[n-1]);
}

