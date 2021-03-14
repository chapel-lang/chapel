const globalArray : [1..1] int;

proc readit(const ref x) {
  writeln(x);
}

proc readForA(A) {
  for a in A {
    readit(a);
  }
}
proc readForallA(A) {
  forall a in A {
    readit(a);
  }
}
proc readForAZip(A, B) {
  for (a,b) in zip(A, B) {
    readit(a);
    readit(b);
  }
}
proc readForallAZip(A, B) {
  forall (a,b) in zip(A, B) {
    readit(a);
    readit(b);
  }
}
proc readCoforallA(A) {
  coforall a in A {
    readit(a);
  }
}
proc readCoforallAZip(A, B) {
  coforall (a, b) in zip(A, B) {
    readit(a);
    readit(b);
  }
}

writeln("READS");
readForA(globalArray);
readForallA(globalArray);
readForAZip(globalArray, globalArray);
readForallAZip(globalArray, globalArray);
readCoforallA(globalArray);
readCoforallAZip(globalArray, globalArray);
