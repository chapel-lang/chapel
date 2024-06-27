use BlockDist, LayoutCS;

config const n = 10;

const D = {0..<n, 0..<n} dmapped new blockDist({0..<n, 0..<n},
                                               sparseLayoutType=CS(compressRows=true));
const DS: sparse subdomain(D);

var A: [DS] real;

var SpsD: sparse subdomain(D);
DS.setLocalSubdomain(SpsD);

var CSC: sparse subdomain(D) dmapped new dmap(new CS(compressRows=false));
DS.setLocalSubdomain(CSC);

var B: [D] real;
A.setLocalSubarray(B);
