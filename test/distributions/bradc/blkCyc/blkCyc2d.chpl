use BlockCycDist;

config const m = 25, n = 35;

var MyBlkCyc = new dmap(new BlockCyclic(startIdx=(1,0), blocksize=(4, 8)));

writeln("Declaring D:");
var D: domain(2) dmapped MyBlkCyc = {1..m, 0..n};
writeln();

writeln("Declaring D2:");
var D2: domain(2) dmapped MyBlkCyc = {1..7, 0..0};
writeln();


writeln("D is: ", D);
writeln("D2 is: ", D2);

var A: [D] real;
var A2: [D2] real;

forall (i,j) in D do
  A(i,j) = i + j/100.0;
         
forall (i,j) in D2 do
  A2(i,j) = i + j/100.0;

writeln("A is: ", A);
writeln("A2 is: ", A2);

{
  // check copy/move initialization of 2d array
  var AA = A;
  var AAA = AA;
  assert(A.equals(AAA));

  var AA2 = A2;
  var AAA2 = AA2;
  assert(A2.equals(AAA2));
}

forall ((i,j), a) in zip(D, A) do
  a = j + i/100.0;

forall ((i,j), a) in zip(D2, A2) do
  a = j + i/100.0;

writeln("A is: ", A);
writeln("A2 is: ", A2);

forall (a, (i,j)) in zip(A, D) do
  a = i + j/100.0;

forall (a, (i,j)) in zip(A2, D2) do
  a = i + j/100.0;

writeln("A is: ", A);
writeln("A2 is: ", A2);

for i in D do
  A(i) = 1;

for i in D2 do
  A2(i) = 1;

writeln("A is: ", A);
writeln("A2 is: ", A2);
