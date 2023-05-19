use BlockDist, Time, MemDiagnostics, Types, Random;
private use CommDiagnostics;

type elemType = real(64);
config const m: int(64) = 8, alpha = 3.0;
config const seed = 314159265;
config const tasksPerLocale = 1;

config const verbose: bool = false;

proc main() {
  const Dist = new dmap(new Block(rank=1,idxType=int(64),boundingBox={1..m},
                                  dataParTasksPerLocale=tasksPerLocale));
  const ProblemSpace: domain(1, int(64)) dmapped Dist = {1..m};
  var A, B, C: [ProblemSpace] elemType;

  var randlist = new owned NPBRandomStream(real, seed);
  randlist.fillRandom(B);
  randlist.fillRandom(C);
  startCommDiagnostics();
  forall (a, b, c) in zip(A, B, C) do
    a = b + alpha * c;
  stopCommDiagnostics();

  writeln("B: ", B);
  writeln("C: ", C);
  writeln("A: ", A);

  writeln(getCommDiagnostics());
}
