use BlockDist;

config const n = 8;

const Elems = {0..#n} dmapped Block({0..#n});

var MatElems: sparse subdomain(Elems);

MatElems += 4;

var SparseArray: [MatElems] int;

SparseArray[7] = 22;

writeln(SparseArray[7]);
