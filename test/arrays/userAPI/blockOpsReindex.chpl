use arrayAPItest;
use BlockDist;

const D = {1..10, 1..10};
const Space = D dmapped Block(D);

var A: [Space] real;

testArrayAPI2D("Reindex of Block-distributed array", A.reindex(0..9, 1..20 by 2), {2..7, 7..17 by 2}, {1.. by 4 # 10, 1..10});
