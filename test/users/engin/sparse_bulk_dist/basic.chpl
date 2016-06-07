use BlockDist;

config const N = 8;

const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N});

var SparseDom: sparse subdomain(ParentDom);

var inds: [{0..#(N*N/2)}] 2*int;
var ii = 0;
for i in 0..#N {
  for j in 0..#N by 2 {
    inds[ii] = (i, j+i%2);
    ii += 1;
  }
}

SparseDom += inds;

for i in SparseDom do writeln(i);
