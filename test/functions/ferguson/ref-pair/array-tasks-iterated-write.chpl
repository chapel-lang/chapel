var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc writeit(ref x) {
  x = 1;
}

proc writeCoforallZip(A, B) {
  coforall (a,b) in zip(A,B) {
    a = b;
  }
}

writeCoforallZip(getter(globalArray), getter(globalArray));
