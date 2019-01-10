use BlockDist;
use CommDiagnostics;

config const printArray = true;

  const D = {1..10, 1..10} dmapped Block({1..10, 1..10});
  var A: [D] real;
  const DInner = D[3..8, 3..8];

  forall a in A do
    a = here.id;

  writeln("\nA is:\n", A);

  writeln();
  writeln("Incrementing A[Dinner] via routine:");
  increment(A[DInner], DInner);

  writeln();
  writeln("Incrementing A[Dinner] via routine:");
  increment(A[DInner]);


  writeln();
  writeln("Creating B (a normal slice)");
  writeln("---------------------------");

  startTrial();
  ref B = A[DInner];
  stopTrial();


  writeln();
  writeln("Incrementing B via access");
  writeln("-------------------------");

  startTrial();
  forall ij in DInner do
    B[ij] += 0.1;
  stopTrial();


  writeln();
  writeln("Incrementing B via iteration");
  writeln("----------------------------");

  startTrial();
  forall b in B do
    b += 0.1;
  stopTrial();


  writeln();
  writeln("Incrementing B via routine:");
  increment(B, DInner);

  writeln();
  writeln("Incrementing B via routine:");
  increment(B);

inline proc startTrial() {
  resetCommDiagnostics();
  startCommDiagnostics();
}

inline proc stopTrial() {
  stopCommDiagnostics();
  // retrieve the counts and report the results
  writeln(getCommDiagnostics());
  if printArray then
    writeln("\nA is:\n", A);
}

proc increment(X, D) {
  writeln("Incrementing in routine by access");
  writeln("---------------------------------");

  startTrial();
  forall ij in D do
    X[ij] += 0.1;
  stopTrial();
}


proc increment(X) {
  writeln("Incrementing in routine by iteration");
  writeln("------------------------------------");

  startTrial();
  forall x in X do
    x += 0.1;
  stopTrial();
}
