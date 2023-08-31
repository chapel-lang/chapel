const globalArray : [1..4] int;

proc readit(const ref x) {
  writeln(x);
}

proc writeit(ref x) {
  x = 1;
}

proc writeForAZip(ref A, B) {
  for (a,b) in zip(A, B) {
    a = b;
  }
}
proc writeForallAZip(ref A, B) {
  forall (a,b) in zip(A, B) {
    a = b;
  }
}

proc writeCoforallAZip(ref A, B) {
  coforall (a, b) in zip(A, B) {
    writeit(a);
  }
}

writeForAZip((globalArray), (globalArray)); // should be error
writeForallAZip((globalArray), (globalArray)); // should be error
writeCoforallAZip((globalArray), (globalArray)); // should be error
