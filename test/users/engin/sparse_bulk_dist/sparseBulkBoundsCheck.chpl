use BlockDist;
use LayoutCS;

config const N=8;
config type sparseLayoutType = DefaultDist;

const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);
var SparseDom: sparse subdomain(ParentDom);

var SparseArr: [SparseDom] int;

const LargeDom = {0..#N*2, 0..#N*2};
var LargeSparseDom: sparse subdomain(LargeDom);

LargeSparseDom += (N-1, N-1);
SparseDom += LargeSparseDom; //this should work

LargeSparseDom += (N, N);
SparseDom += LargeSparseDom; //this should fail

