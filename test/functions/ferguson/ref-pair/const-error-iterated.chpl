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

writeln("WRITES");
writeForA((globalArray)); // error
writeForallA((globalArray)); // error
promotedAssign((globalArray), (globalArray)); // error
writeCoforallA((globalArray)); // error


