use LayoutCS;

config const N = 8;

const ParentDom = {0..#N, 0..#N};

enum layoutTypes {coo, csr, csc};
config param layoutType = layoutTypes.coo;

var csrDom: sparse subdomain(ParentDom) dmapped CS(compressRows=true);
var cscDom: sparse subdomain(ParentDom) dmapped CS(compressRows=false);
var cooDom: sparse subdomain(ParentDom);

var SparseDom = if layoutType == layoutTypes.csr then 
                  csrDom
               else if layoutType == layoutTypes.csc then
                  cscDom
               else
                  cooDom;

var SparseMat: [SparseDom] int;

//create a small dense chunk somewhere in vector
const denseChunk = {1..3, 5..7}; 

SparseDom += denseChunk; //not sure if this would work

for i in denseChunk do SparseMat[i]=i[0]+i[1];

writeln("Dense chunk:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}

//create a strided chunk
var stridedChunk = {1..6 by 2, 1..3};

SparseDom += stridedChunk;

for i in stridedChunk do SparseMat[i]=i[0]+i[1];

writeln("Dense + strided chunk:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}

//create diagonal indices
var diagIndArr : [{17..#2*N }] 2*int;
for i in ParentDom.dim(0) {
  diagIndArr[diagIndArr.domain.low+i*2] = (i, i);
  diagIndArr[diagIndArr.domain.low+i*2+1] = (i, N-1-i);
}

SparseDom += diagIndArr;

for i in diagIndArr do SparseMat[i]=i[0]+i[1];

writeln("Chunks + Diagonals:");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i,j], " ");
  }
  writeln();
}
