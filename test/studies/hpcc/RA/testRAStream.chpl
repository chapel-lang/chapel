use BlockDist, RARandomStream;

config const numItems = 1000:randType;

const ProbSpace = {0:randType..#numItems};

var D = ProbSpace dmapped new blockDist(ProbSpace);
var A: [D] randType;

forall (i, r) in zip(D, RAStream()) with (ref A) do
  A[i] = r;

writeln("RAStream is: ");
for a in A do
  writeln(a);

