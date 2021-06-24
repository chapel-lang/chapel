const ProgModels = {"C", "Chapel", "MPI", "OpenMP"};
const CoolLanguages = {"Chapel"};

var LinesOfCode: [ProgModels] int;

LinesOfCode["C"] = 10000;
LinesOfCode["Chapel"] = 1000;
LinesOfCode["MPI"] = 12000;
LinesOfCode["OpenMP"] = 11000;

for loc in LinesOfCode[CoolLanguages] do
  writeln(loc);

foo(LinesOfCode[CoolLanguages]);

proc foo(X: [] ) {
  for i in X.domain {
    writeln("X[", i, "] = ", X[i]);
  }
}
