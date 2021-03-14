use BlockCycDist;

config const n = 25;

enum XO {O, X};

var MyBlkCyc = new dmap(new BlockCyclic(startIdx=(1,), blocksize=(4,)));

var D: domain(1) dmapped MyBlkCyc = {1..n};

for i in D do
  writeln(i, "(A)");

forall i in D do
  writeln(i + n, "(B)");

for i in D by 2 do
  writeln(i + 2*n, "(C)");

forall i in D by 2 do
  writeln(i + 3*n, "(D)");

var A: [D] int;

A = D;

for a in A do
  writeln(a + 4*n, "(E)");

forall a in A do
  writeln(a + 5*n, "(F)");

/* Can't yet handle: need to separate stridability of allocDom from idxDom?
for a in A[D by 2] do
  writeln(a + 6*n, "(G)");

forall a in A[D by 2] do
  writeln(a + 7*n, "(H)");
*/
