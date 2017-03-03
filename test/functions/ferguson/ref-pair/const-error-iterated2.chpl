const globalArray : [1..4] int;

proc readit(const ref x) {
  writeln(x);
}

proc writeit(ref x) {
  x = 1;
}

proc readCoforallAZip(A, B) {
  coforall (a, b) in (A, B) {
    readit(a);
  }
}

proc writeForAZip(A, B) {
  for (a,b) in zip(A, B) {
    a = b;
  }
}
proc writeForallAZip(A, B) {
  forall (a,b) in zip(A, B) {
    a = b;
  }
}

proc writeCoforallAZip(A, B) {
  coforall (a, b) in (A, B) {
    writeit(a);
  }
}



readCoforallAZip(globalArray, globalArray);

writeForAZip((globalArray), (globalArray)); // should be error
writeForallAZip((globalArray), (globalArray)); // should be error
writeCoforallAZip((globalArray), (globalArray)); // should be error
