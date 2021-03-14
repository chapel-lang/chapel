use BlockCycDist;

config const n = 25;

var MyBlkCyc = new dmap(new BlockCyclic(startIdx=(1,), blocksize=(4,)));

writeln("Declaring D:");
var D: domain(1) dmapped MyBlkCyc = {1..n};
writeln();

writeln("Declaring D2:");
var D2: domain(1) dmapped MyBlkCyc = {1..7};
writeln();

writeln("D is: ", D);
writeln("D2 is: ", D2);

var A: [D] real;
var A2: [D2] real;

forall i in D do
  A(i) = i;
         
forall i in D2 do
  A2(i) = i;

writeln("A is: ", A);
writeln("A2 is: ", A2);

forall (i, a) in zip(D, A) do
  a = i/10.0;

forall (i, a) in zip(D2, A2) do
  a = i/10.0;

writeln("A is: ", A);
writeln("A2 is: ", A2);

        A = 0;
        A2 = 0;

forall (a, i) in zip(A, D) do
  a = i;

forall (a, i) in zip(A2, D2) do
  a = i;

writeln("A is: ", A);
writeln("A2 is: ", A2);

for i in D do
  A(i) = 1;

for i in D2 do
  A2(i) = 1;

writeln("A is: ", A);
writeln("A2 is: ", A2);
