use ReplicatedDist;

const GridD = {0..#2, 0..#(numLocales/2)};

var Grid = reshape(Locales, GridD);





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

coforall (i,j) in GridD do
  on Grid[i,j] do
    A = here.id + i/10.0 + j/100.0;

writeln("A is: ", A);

for loc in Locales do on loc {
  writeln(here, ":");
  writeln(A);
}
