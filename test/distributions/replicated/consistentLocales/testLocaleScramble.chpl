use ReplicatedDist;

const GridD = {0..#numLocales};

var Grid: [GridD] locale;

for (loc, i) in zip(Locales, 0..#numLocales by -1) {
  Grid[i] = loc;
}

writeln("Grid is: ", Grid);

const D = {1..3} dmapped Replicated(Grid);
var A: [D] real;

coforall loc in Locales do
  on loc do
    for i in 1..3 do
      A[i] = here.id + i/10.0;

writeln("A is: ", A);

for loc in Locales do on loc {
  writeln(here, ":");
  writeln(A);
}

coforall i in GridD do
  on Grid[i] do
    A = here.id + i/10.0;

writeln("A is: ", A);

for loc in Locales do on loc {
  writeln(here, ":");
  writeln(A);
}
