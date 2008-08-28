use DummyArithDist;

var Dist = new MyDist();
var DArith: domain(1) distributed(Dist);
var DAssoc: domain(int) distributed(Dist);
