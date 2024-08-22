use BlockDist;
use CommDiagnostics;

config const printArray = true;

const D = {1..10, 1..10} dmapped new blockDist({1..10, 1..10});
var A: [D] real;
const DInner = D[3..8, 3..8];

forall a in A do
  a = here.id;

writeln("\nA is:\n", A);

ref B = A[DInner];

resetCommDiagnostics();
startCommDiagnostics();
forall ij in DInner do
  B[ij] += 0.1;
stopCommDiagnostics();
writeln(getCommDiagnostics());

if printArray then
  writeln("\nA is:\n", A);
