use arrayAPItest;
use BlockDist;

const X = {1..10, 1..10, 1..10};
const Y = X dmapped Block(X);
var B : [Y] real;
testArrayAPI2D("RankChange of Block-distributed array", B[1..7, 1..7, 1], {2..7, 2..3}, {2..#7, 1.. by 2 # 7});
