use BlockDist;
use LayoutCS;

config const N = 9;

config type sparseLayoutType = DefaultDist;

const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);

var SparseDom: sparse subdomain(ParentDom);

var SparseArr: [SparseDom] int;

//create diagonal indices
var diagIndArr : [{0..#2*N}] 2*int;
for i in ParentDom.dim(0) {
  /*(i, i) (i, N-1-i)*/
  diagIndArr[i*2] = (i, i);
  diagIndArr[i*2+1] = (i, N-1-i);
}

SparseDom += diagIndArr;
SparseArr = 1;

writeln("Diagonals");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseArr[i,j], " ");
  }
  writeln();
}

//create a column of indices
var colIndArr: [{0..#N}] 2*int;
for i in ParentDom.dim(0) {
  colIndArr[i] = (i, N/2); 
}

SparseDom += colIndArr;
SparseArr = 2;

writeln("Diagonals + Column");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseArr[i,j], " ");
  }
  writeln();
}

//create row of indices
var rowIndArr: [{0..#N}] 2*int;
for i in ParentDom.dim(1) {
  rowIndArr[i] = (N/2, i);
}

SparseDom += rowIndArr;
SparseArr = 3;

writeln("Diagonals + Column + Row");
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseArr[i,j], " ");
  }
  writeln();
}
