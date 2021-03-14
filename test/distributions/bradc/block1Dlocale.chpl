use BlockDist;

config const m = 10;

const Dom = {1..m};
const ProblemSpace = Dom dmapped Block(Dom);

var A: [ProblemSpace] real;

// check location of un-assigned int elements
for i in ProblemSpace do
  writeln("A[i].locale = ", A[i].locale);

for i in ProblemSpace do
  A[i] = 1.2;

// check location of assigned int elements
writeln("--------------------");
for i in ProblemSpace do
  writeln("A[i].locale = ", A[i].locale);

var B: [ProblemSpace] string;

// check location of un-assigned string elements
writeln("--------------------");
for i in ProblemSpace do
  writeln("B[i].locale = ", B[i].locale);

for i in ProblemSpace do
  B[i] = "hi";

// check location of assigned string elements
writeln("--------------------");
for i in ProblemSpace do
  writeln("B[i].locale = ", B[i].locale);
