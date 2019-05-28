use BlockDist;

var Dist = new dmap(new Block(boundingBox={1..9}));

var D1 = new _domain(Dist.newRectangularDom(1, int(64), false));
var D2 = new _domain(Dist.newRectangularDom(1, int(64), false));
D1.setIndices({1..9});
D2.setIndices({2..10});

forall (i,j) in zip(D1, D2) do
  writeln("(i,j) is: ", (i,j));
