class C {}

def foo(A: C) {

}

use BlockDist;

var dist = distributionValue(new Block(rank=1, bbox=[1..5], idxType=int));
var dom: domain(1, int) distributed dist = [1..5];

foo(dom);
