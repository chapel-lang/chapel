use MultiBlockDist;
config const N = 500 : int(64);

var Dist = new Block(rank=1, bbox=[0..#N]);
var Dom : domain(1, int(64)) distributed Dist = [0..#N];

var U,Au : [Dom] real;

var vv = + reduce (U + U);
writeln(vv);
