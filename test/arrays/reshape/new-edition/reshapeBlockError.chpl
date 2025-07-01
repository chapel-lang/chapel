use BlockDist;

const D = blockDist.createDomain(1..8);
var A: [D] real;

ref B = reshape(A, blockDist.createDomain({1..2, 1..4}));
ref C = reshape(A, {1..2, 1..4});
ref E = reshape(A, 1..2, 1..4);

var F = [1, 2, 3, 4, 5, 6, 7, 8];
                var G = reshape(F, blockDist.createDomain({1..2, 1..4}));
