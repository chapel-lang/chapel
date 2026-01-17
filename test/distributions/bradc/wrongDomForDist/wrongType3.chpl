use DummyArithDist;

enum color { red, blue, green };

var Dist = new myDist();
var DArith: domain(1) dmapped(Dist);
var DAssoc: domain(color) dmapped(Dist);
