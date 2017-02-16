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

proc writeCoforallA(A) {
  coforall a in A {
    writeit(a);
  }
}

writeForA(getter(globalArray));
writeForallA(getter(globalArray));
writeCoforallA(getter(globalArray));
