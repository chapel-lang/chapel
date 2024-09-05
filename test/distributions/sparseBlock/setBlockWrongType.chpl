use BlockDist, CompressedSparseLayout;

config const n = 10;

const D = {0..<n, 0..<n} dmapped new blockDist({0..<n, 0..<n},
                                               sparseLayoutType=cs(compressRows=true));
const DS: sparse subdomain(D);

var A: [DS] real;

var SpsD: sparse subdomain(D);
DS.setLocalSubdomain(SpsD);

var CSC: sparse subdomain(D) dmapped new cs(compressRows=false);
DS.setLocalSubdomain(CSC);

var B: [D] real;
A.setLocalSubarray(B);
