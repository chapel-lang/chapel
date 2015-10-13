use BlockDist;

var Dist = new dmap(new Block(boundingBox={1..9}));

// hilde sez: Parallel iteration of "nude" domains can lead to MM errors.
// One must either adjust the ref count manually, to move it away from zero, or
// manipulate those domains through record-wrappers, to have the reference
// counting done automatically.  The latter is the recommended coding style.
// var D1 = Dist.newRectangularDom(1, int(64), false);
// var D2 = Dist.newRectangularDom(1, int(64), false);
var D1 = _newDomain(Dist.newRectangularDom(1, int(64), false));
var D2 = _newDomain(Dist.newRectangularDom(1, int(64), false));
D1._value.dsiSetIndices({1..9});
D2._value.dsiSetIndices({2..10});

forall (i,j) in zip(D1, D2) do
  writeln("on ", here.id, ", (i,j) is: ", (i,j));
