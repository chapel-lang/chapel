use BlockDist;



const m = 1000;
const ProblemSpace: domain(1) dmapped Block(boundingBox={1..m}) = {1..m};
writeln();
writeln();

var A: [ProblemSpace] int;
var B: [ProblemSpace] int;
var C: [ProblemSpace] int;

A = B + C;
writeln();
writeln();
allowTaskSpawnDebug = true;
forall (a, b, c) in zip(A, B, C) do 
  a = b + c;
writeln();
forall (a, b, c) in zip(A, B, C) do 
  a = b + c;

allowTaskSpawnDebug = false;

writeln();
writeln();


