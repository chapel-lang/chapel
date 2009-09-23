use BlockCycDist;

config const n = 25;

var MyBlkCyc = new BlockCyclic(rank=1, idxType=int(32), low=tuple(1), blk=tuple(4));

writeln("Declaring D:");
var D: domain(1) distributed MyBlkCyc = [1..n];
writeln();

writeln("Declaring D2:");
var D2: domain(1) distributed MyBlkCyc = [1..7];
writeln();

writeln("D is: ", D);
writeln("D2 is: ", D2);

var A: [1..n] real;
var A2: [1..7] real;

forall i in D do
  A(i) = i;
         
forall i in D2 do
  A2(i) = i;

writeln("A is: ", A);
writeln("A2 is: ", A2);

forall (i, a) in (D, A) do
  a = i/10.0;

forall (i, a) in (D2, A2) do
  a = i/10.0;

writeln("A is: ", A);
writeln("A2 is: ", A2);

forall (a, i) in (A, D) do
  a = i;

forall (a, i) in (A2, D2) do
  a = i;

writeln("A is: ", A);
writeln("A2 is: ", A2);

for i in D do
  A(i) = 1;

for i in D2 do
  A2(i) = 1;

writeln("A is: ", A);
writeln("A2 is: ", A2);
