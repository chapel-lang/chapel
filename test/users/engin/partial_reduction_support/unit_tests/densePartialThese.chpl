use utilities;
use dsiMethods;
use templates;

config const N = 10;

const ParentDom = {0..#N, 0..#2*N};
var Mat: [ParentDom] int;

for (i,n) in zip(ParentDom, 1..) {
  Mat[i] = n;
}

for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    writef("%2i ", Mat[i, j]);
  }
  writeln();
}

writeln("Even rows:");
for r in ParentDom.dim(0) by 2 {
  write("Row ", r, ":\t");
  forall i in ParentDom._value.dsiPartialThese(1, r) {
    writef("%2i ", Mat[r, i]);
  }
  writeln();
}

writeln("Odd rows:");
for r in ParentDom.dim(0) by 2 align 1{
  write("Row ", r, ":\t");
  forall i in ParentDom._value.dsiPartialThese(1, r) {
    writef("%2i ", Mat[r, i]);
  }
  writeln();
}

writeln("Even cols:");
for c in ParentDom.dim(1) by 2 {
  write("Col ", c, ":\t");
  forall i in ParentDom._value.dsiPartialThese(0, c) {
    writef("%2i ", Mat[i, c]);
  }
  writeln();
}

writeln("Odd cols:");
for c in ParentDom.dim(1) by 2 align 1{
  write("Col ", c, ":\t");
  forall i in ParentDom._value.dsiPartialThese(0, c) {
    writef("%2i ", Mat[i, c]);
  }
  writeln();
}
