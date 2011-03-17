use BlockCycDist;

config const n = 25;

enum XO {O, X};

var MyBlkCyc = new dmap(new BlockCyclic(startIdx=tuple(1), blocksize=tuple(4)));

var D: domain(1) dmapped MyBlkCyc = [1..n];
var A: [D] XO;

proc helper(rng) {
 forall i in D do
   A(i) = XO.O;
 forall a in A[rng] do
   a = XO.X;
 writeln(A);
}

helper(1..5);
helper(5..7);
helper(2..6);
helper(6..12);
helper(15..19);
helper(2..24);

/*

forall i in D do
  A(i) = i;

writeln(A);

forall a in A[1..5] do
  a = -a;

writeln(A);

forall a in A[5..7] do
  a = -a;
writeln(A);

forall a in A[2..6] do
  a = -a;
writeln(A);

forall a in A[6..12] do
  a = -a;
writeln(A);

forall a in A[15..19] do
  a = -a;
writeln(A);

forall a in A[2..24] do
  a = -a;
writeln(A);

*/