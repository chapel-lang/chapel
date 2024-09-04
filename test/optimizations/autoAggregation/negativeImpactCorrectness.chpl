// this is the correctness-only analogue of perf/autoAggNegativeImpact
use BlockDist;

config const m = 10;

const ProblemSpace: domain(1) dmapped new blockDist(boundingBox={1..m}) = {1..m};

var A, B: [ProblemSpace] int;

for b in B do b = 1;

forall i in ProblemSpace with (ref A) do
  A[i] =
    B[i+0];

writeln(A);
