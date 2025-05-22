use BlockDist;

const D = blockDist.createDomain(1..8);
var A: [D] real;

ref B = A.reshape(blockDist.createDomain({1..2, 1..4}));
ref C = A.reshape({1..2, 1..4});
ref E = A.reshape(1..2, 1..4);

var F = [1, 2, 3, 4, 5, 6, 7, 8];
var G = F.reshape(blockDist.createDomain({1..2, 1..4}));
