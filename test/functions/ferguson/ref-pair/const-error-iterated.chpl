const globalArray : [1..4] int;

proc writeit(ref x) {
  x = 1;
}

proc readit(const ref x) {
  writeln(x);
}

proc writeForA(ref A) {
  for a in A {
    writeit(a);
  }
}
proc writeForallA(ref A) {
  forall a in A {
    writeit(a);
  }
}
proc promotedAssign(ref A, B) {
  A = B;
}

proc writeCoforallA(ref A) {
  coforall a in A {
    writeit(a);
  }
}

writeln("WRITES");
writeForA((globalArray)); // error
writeForallA((globalArray)); // error
promotedAssign((globalArray), (globalArray)); // error
writeCoforallA((globalArray)); // error


