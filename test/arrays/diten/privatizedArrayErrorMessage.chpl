class C {}

proc foo(A: borrowed C) {

}

use BlockDist;

var Dist = new dmap(new Block(rank=1, boundingBox={1..5}));
var dom: domain(1, int) dmapped Dist = {1..5};
var B: [dom] int;
foo(B);
