use BlockDist;

var Dist = new dmap(new Block(boundingBox={1..9}));

var D1: domain(1) dmapped (Dist) = {1..9};
var D2: domain(1) dmapped (Dist) = {2..10};

forall (i,j) in zip(D1, D2) do
  writeln("on ", here.id, ", (i,j) is: ", (i,j));

