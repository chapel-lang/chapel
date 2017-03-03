const globalArray : [1..4] int;

proc writeit(ref x) {
  x = 1;
}

proc readit(const ref x) {
  writeln(x);
}

proc writeForA(A) {
  for a in A {
    writeit(a);
  }
}
proc writeForallA(A) {
  forall a in A {
    writeit(a);
  }
}
proc promotedAssign(A, B) {
  A = B;
}

proc writeCoforallA(A) {
  coforall a in A {
    writeit(a);
  }
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

writeln("READS");
readForA(globalArray);
readForallA(globalArray);
readForAZip(globalArray, globalArray);
readForallAZip(globalArray, globalArray);
readCoforallA(globalArray);

writeln("WRITES");
writeForA((globalArray)); // error
writeForallA((globalArray)); // error
promotedAssign((globalArray), (globalArray)); // error
writeCoforallA((globalArray)); // error


