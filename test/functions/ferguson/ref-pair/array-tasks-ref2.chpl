var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc readit(x) {
  var tmp = x;
}

proc readCaptureForall(A) {
  forall i in 1..10 with (ref A) {
    readit(A);
  }
}


writeln("READS");
readCaptureForall(getter(globalArray));
