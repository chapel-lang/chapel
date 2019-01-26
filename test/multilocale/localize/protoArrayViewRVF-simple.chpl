use BlockDist;
use CommDiagnostics;
//use myArrayStuff;

config const printArray = true;

proc main() {
  const D = {1..10, 1..10} dmapped Block({1..10, 1..10});
  var A: [D] real;
  const DInner = D[3..8, 3..8];

  forall a in A do
    a = here.id;

  startTrial();
  ref Slice = A[DInner];
  stopTrial("created slice: ");
  writeln("Slice is: ", Slice);

  on Locales[numLocales-1] {
    startTrial();
    forall s in Slice do
      s += 0.1;
    stopTrial("incremented slice");
    writeln("Slice is: ", Slice);
  }

inline proc startTrial() {
  resetCommDiagnostics();
  startCommDiagnostics();
}

inline proc stopTrial(lbl) {
  stopCommDiagnostics();
  // retrieve the counts and report the results
  writeln(lbl, "\n", getCommDiagnostics());
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


}

