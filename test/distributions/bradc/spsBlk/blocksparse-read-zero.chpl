use BlockDist;

config const n = 8;

const Elems = {0..#n} dmapped Block({0..#n});

var MatElems: sparse subdomain(Elems);

MatElems += 4;

var SparseArray: [MatElems] int;

var x = SparseArray[7];
writeln(x);
