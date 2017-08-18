use utilities;
use dsiMethods;
use templates;

use LayoutCS;

config const N = 10;
config type layoutType = DefaultDist;

const layout = new layoutType;
const ParentDom = {0..#N, 0..#2*N};
var SparseDom: sparse subdomain(ParentDom) dmapped new dmap(layout);
var SparseMat: [SparseDom] int;

// create cool set of sparse indices
for i in ParentDom.dim(1) {
  for j in ParentDom.dim(2) by 2 align i%2 {
    SparseDom += (i, j);
  }
} 

for (i,n) in zip(SparseDom, 1..) {
  SparseMat[i] = n;
}

for i in ParentDom.dim(1) {
  for j in ParentDom.dim(2) {
    writef("%2i ", SparseMat[i, j]);
  }
  writeln();
}

writeln("Even rows:");
for r in ParentDom.dim(1) by 2 {
  write("Row ", r, ":\t");
  forall i in SparseDom._value.dsiPartialThese(2, r) {
    writef("%2i ", SparseMat[r, i]);
  }
  writeln();
}

writeln("Odd rows:");
for r in ParentDom.dim(1) by 2 align 1{
  write("Row ", r, ":\t");
  forall i in SparseDom._value.dsiPartialThese(2, r) {
    writef("%2i ", SparseMat[r, i]);
  }
  writeln();
}

writeln("Even cols:");
for c in ParentDom.dim(2) by 2 {
  write("Col ", c, ":\t");
  forall i in SparseDom._value.dsiPartialThese(1, c) {
    writef("%2i ", SparseMat[i, c]);
  }
  writeln();
}

writeln("Odd cols:");
for c in ParentDom.dim(2) by 2 align 1{
  write("Col ", c, ":\t");
  forall i in SparseDom._value.dsiPartialThese(1, c) {
    writef("%2i ", SparseMat[i, c]);
  }
  writeln();
}
