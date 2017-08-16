use BlockDist;
use LayoutCS;

config const N = 16;


config type sparseLayoutType = DefaultDist;

const FullDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);

var FullSparseDom: sparse subdomain(FullDom);
var FullSparseArr: [FullSparseDom] int;

//define a hardcoded DefaultSparse subdomain for second quadrant
const SecondQuadrant = {0..#N/2, 0..#N/2};
var SparseSQ: sparse subdomain(SecondQuadrant);

//create diagonal indices
var diagIndArr1 : [{0..#N}] 2*int;
for i in SecondQuadrant.dim(1) {
  diagIndArr1[i*2] = (i, i);
  diagIndArr1[i*2+1] = (i, N/2-1-i);
}

SparseSQ += diagIndArr1;

//define a hardcoded CSR subdomain for second quadrant
const FourthQuadrant = {N/2..N-1, N/2..N-1};
var SparseFQ: sparse subdomain(FourthQuadrant) dmapped CS();

//create diagonal indices
var diagIndArr2 : [{0..#N}] 2*int;
for i in FourthQuadrant.dim(1) {
  diagIndArr2[(i-N/2)*2] = (i, i);
  diagIndArr2[(i-N/2)*2+1] = (i, N-1-(i-N/2));
}

SparseFQ += diagIndArr2;

FullSparseDom += SparseSQ;
FullSparseArr=1;

for i in FullDom.dim(1) {
  for j in FullDom.dim(2) {
    write(FullSparseArr[i, j]," ");
  }
  writeln();
}

FullSparseDom += SparseFQ;
FullSparseArr=2;

for i in FullDom.dim(1) {
  for j in FullDom.dim(2) {
    write(FullSparseArr[i, j]," ");
  }
  writeln();
}

