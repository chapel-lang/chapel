const globalArray : [1..4] int;

proc readit(const ref x) {
  writeln(x);
}

proc readCoforallAZip(A, B) {
  coforall (a, b) in (A, B) {
    readit(a);
  }
}


readCoforallAZip(globalArray, globalArray);
