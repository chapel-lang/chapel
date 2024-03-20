class C {}

proc foo(A: borrowed C) {

}

use BlockDist;

var Dist = new blockDist(rank=1, boundingBox={1..5});
var dom: domain(1, int) dmapped Dist = {1..5};

foo(dom);
