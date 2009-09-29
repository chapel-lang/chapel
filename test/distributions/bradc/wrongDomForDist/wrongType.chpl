use DummyArithDist;

var Dist = distributionValue(new MyDist());
var DArith: domain(1) distributed(Dist);
var DAssoc: domain(int) distributed(Dist);
