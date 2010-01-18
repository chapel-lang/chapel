use DummyAssocDist;

var Dist = new dist(new MyDist());
var DAssoc: domain(int) distributed(Dist);
var DArith: domain(1) distributed(Dist);
