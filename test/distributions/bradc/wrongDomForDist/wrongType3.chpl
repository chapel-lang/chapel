use DummyArithDist;

enum color { red, blue, green };

var Dist = new dmap(new MyDist());
var DArith: domain(1) dmapped(Dist);
var DAssoc: domain(color) dmapped(Dist);
