config const N = 32;

const ParentDom = {0..#N};

var SparseDom: sparse subdomain(ParentDom);

var SparseVec: [SparseDom] int;

//create a small dense chunk somewhere in vector
var denseChunk = {7..14}; 

SparseDom += denseChunk; //not sure if this would work

//create a strided chunk
var stridedChunk = {18..28 by 2};

SparseDom += stridedChunk;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

//create some more interesting indices
var cnt = N/3;
var inds: [{0..#cnt}] int;
for (n,ind) in zip(0..#cnt,inds) do ind=n*3;
SparseDom += inds;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

//create even indices
var cnt2 = N/2-1;
var inds2: [{0..#cnt2}] int;
for (n, ind) in zip(0..#cnt2, inds2) do ind=n*2;
SparseDom += inds2;

for (d,a) in zip(SparseDom, SparseVec) do a=d;

for i in ParentDom do writeln(SparseVec[i]);

