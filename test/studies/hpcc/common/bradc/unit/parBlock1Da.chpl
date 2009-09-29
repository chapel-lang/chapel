use BlockDist;

var Dist = distributionValue(new Block(rank=1, int(32), bbox=[1..9]));

var D1: domain(1) distributed (Dist) = [1..9];
var D2: domain(1) distributed (Dist) = [2..10];

forall (i,j) in (D1, D2) do
  writeln("on ", here.id, ", (i,j) is: ", (i,j));

