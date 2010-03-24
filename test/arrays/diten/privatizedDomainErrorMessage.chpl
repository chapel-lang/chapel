class C {}

def foo(A: C) {

}

use BlockDist;

var Dist = new dist(new Block(rank=1, boundingBox=[1..5]));
var dom: domain(1, int) distributed Dist = [1..5];

foo(dom);
