use BlockDist;

var Dist = new dmap(new Block(boundingBox={1..9}));

var D1 = Dist.newRectangularDom(1, int(64), false);
var D2 = Dist.newRectangularDom(1, int(64), false);
D1.dsiSetIndices({1..9});
D2.dsiSetIndices({2..10});

// Parallel iteration over "nude" domains is dangerous.
// This is the other way to avoid the problem.
D1.incRefCount();
D2.incRefCount();

forall (i,j) in zip(D1, D2) do
  writeln("(i,j) is: ", (i,j));

var cnt2 = D2.decRefCount();
if cnt2 == 0 then
  delete D2;

var cnt1 = D1.decRefCount();
if cnt1 == 0 then
  delete D1;
