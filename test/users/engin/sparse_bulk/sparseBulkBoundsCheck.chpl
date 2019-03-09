use LayoutCS;

config const N=8;
config type layoutType = DefaultDist;
var layout = new unmanaged layoutType();

const ParentDom = {0..#N, 0..#N};
var SparseDom: sparse subdomain(ParentDom) dmapped new dmap(layout);

var SparseArr: [SparseDom] int;

const LargeDom = {0..#N*2, 0..#N*2};
var LargeSparseDom: sparse subdomain(LargeDom);

LargeSparseDom += (N-1, N-1);
SparseDom += LargeSparseDom; //this should work

LargeSparseDom += (N, N);
SparseDom += LargeSparseDom; //this should fail

