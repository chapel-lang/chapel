use BlockDist, RARandomStream;

config const numItems = 1000:randType;

const ProbSpace = {0:randType..#numItems};

var D = ProbSpace dmapped Block(ProbSpace);
var A: [D] randType;

forall (i, r) in zip(D, RAStream()) do
  A[i] = r;

writeln("RAStream is: ");
for a in A do
  writeln(a);

