use arrayAPItest;
use BlockDist;

const D = {1..10, 1..10};
const Space = D dmapped Block(D);

var A: [Space] real;

testArrayAPI2D("Block-distributed array", A, {2..7, 5..10}, {0..9, 1.. by 2 # 10});
testArrayAPI2D("Slice of Block-distributed array", A[2..8, 2..8], {2..5, 4..7}, {0..6, 1.. by 3 # 7});
