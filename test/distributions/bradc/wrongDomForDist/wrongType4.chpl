use DummyAssocDist;

var Dist = new dmap(new MyDist());
var DAssoc: domain(int) dmapped(Dist);
var DArith: domain(1) dmapped(Dist);
