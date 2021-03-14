use BlockCycDist;

config const n = 25;

enum XO {O, X};

var MyBlkCyc = new dmap(new BlockCyclic(startIdx=(1,), blocksize=(4,)));

var D: domain(1) dmapped MyBlkCyc = {1..n};
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
