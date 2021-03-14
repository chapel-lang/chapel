use utilities;
use dsiMethods;
use templates;

use LayoutCS;

config const N = 10;

const ParentDom = {0..#N, 0..#2*N};

enum layoutTypes {coo, csr, csc};
config param layoutType = layoutTypes.coo;

var csrDom: sparse subdomain(ParentDom) dmapped CS(compressRows=true);
var cooDom: sparse subdomain(ParentDom);

var SparseDom = if layoutType == layoutTypes.csr then 
                  csrDom
                else
                  cooDom;

var SparseMat: [SparseDom] int;

// create cool set of sparse indices
for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) by 2 align i%2 {
    SparseDom += (i, j);
  }
} 

for (i,n) in zip(SparseDom, 1..) {
  SparseMat[i] = n;
}

for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    writef("%2i ", SparseMat[i, j]);
  }
  writeln();
}

writeln("Even rows:");
for r in ParentDom.dim(0) by 2 {
  write("Row ", r, ":\t");
  forall i in SparseDom._value.dsiPartialThese(1, r) {
    writef("%2i ", SparseMat[r, i]);
  }
  writeln();
}

writeln("Odd rows:");
for r in ParentDom.dim(0) by 2 align 1{
  write("Row ", r, ":\t");
  forall i in SparseDom._value.dsiPartialThese(1, r) {
    writef("%2i ", SparseMat[r, i]);
  }
  writeln();
}

writeln("Even cols:");
for c in ParentDom.dim(1) by 2 {
  write("Col ", c, ":\t");
  forall i in SparseDom._value.dsiPartialThese(0, c) {
    writef("%2i ", SparseMat[i, c]);
  }
  writeln();
}

writeln("Odd cols:");
for c in ParentDom.dim(1) by 2 align 1{
  write("Col ", c, ":\t");
  forall i in SparseDom._value.dsiPartialThese(0, c) {
    writef("%2i ", SparseMat[i, c]);
  }
  writeln();
}
