use BlockDist, Time, Memory, Types, Random;

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

  var randlist = new RandomStream(seed);
  randlist.fillRandom(B);
  randlist.fillRandom(C);
  startCommDiagnostics();
  forall (a, b, c) in (A, B, C) do
    a = b + alpha * c;
  stopCommDiagnostics();

  writeln("B: ", B);
  writeln("C: ", C);
  writeln("A: ", A);

  var Diagnostics = getCommDiagnostics();
  var gets = + reduce Diagnostics.get;
  var nbgets = + reduce Diagnostics.get_nb;
  var testnbgets = + reduce Diagnostics.get_nb_test;
  var waitnbgets = + reduce Diagnostics.get_nb_wait;
  var puts = + reduce Diagnostics.put;
  var forks = + reduce Diagnostics.fork;
  var fforks = + reduce Diagnostics.fork_fast;
  var nbforks = + reduce Diagnostics.fork_nb;

  if verbose then
    writeln(Diagnostics);

  if gets != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of gets");
  }

  if nbgets != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of non-blocking gets");
  }

  if testnbgets != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of test non-blocking gets");
  }

  if waitnbgets != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of test non-blocking gets");
  }

  if puts != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of puts");
  }

  if forks != 1 {
    writeln(Diagnostics);
    halt("comm unexpected number of forks");
  }

  if fforks != 0 {
    writeln(Diagnostics);
    halt("comm unexpected number of fast forks");
  }

  if nbforks != 1 {
    writeln(Diagnostics);
    halt("comm unexpected number of non-blocking forks");
  }
}
