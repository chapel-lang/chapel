use DummyAssocDist;

var Dist = distributionValue(new MyDist());
var DAssoc: domain(int) distributed(Dist);
var DArith: domain(1) distributed(Dist);
