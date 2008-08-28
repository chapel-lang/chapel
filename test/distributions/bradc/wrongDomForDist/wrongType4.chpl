use DummyAssocDist;

var Dist = new MyDist();
var DAssoc: domain(int) distributed(Dist);
var DArith: domain(1) distributed(Dist);
