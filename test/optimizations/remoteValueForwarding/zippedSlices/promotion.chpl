use BlockDist;
use CommDiagnostics;

config const n = 10;
config param doForall = false;

proc main() {

  var A = newBlockArr(1..n, int);
  var B = newBlockArr(1..n, int);
  var C = newBlockArr(1..n, int);

  var alpha = 2;

  const sliceDom = A.domain[2..n-1];
  startCommDiagnostics();

  ref As = A[sliceDom];
  ref Bs = B[sliceDom];
  ref Cs = C[sliceDom];

  stopCommDiagnostics();
  writeln("3 slice creation:");
  printCommDiagnosticsTable();
  resetCommDiagnostics();

  writeln();

  startCommDiagnostics();

  if doForall {
    forall (a,b,c) in zip(As,Bs,Cs) {
      a = b + c * alpha;
    }
  }
  else {
    As = Bs + Cs * alpha;
  }

  stopCommDiagnostics();
  writeln("Stream:");
  printCommDiagnosticsTable();

  writeln(A[n-1]);
}

