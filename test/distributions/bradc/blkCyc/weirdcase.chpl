use BlockCycDist;

var n = 100;

const BlkCycDist = new dmap(new BlockCyclic(startIdx=(1,1), blocksize=(5,3)));

var D: domain(2) dmapped BlkCycDist = {1..100, 1..100};

var A: [D] real;

forall (i,j) in D do
  A(i,j) = i + j/100.0;

writeln("A is: ", A);
