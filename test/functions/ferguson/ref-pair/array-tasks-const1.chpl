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
  forall i in 1..10 with (const A) {
    readit(A);
  }
}

proc readCaptureCoforall(A) {
  coforall i in 1..10 with (const A) {
    readit(A);
  }
}

proc readCaptureCobegin(A) {
  cobegin with (const A) {
    readit(A);
    readit(A);
  }
}

proc readCaptureBegin(A) {
  sync {
    begin with (const A) {
      readit(A);
    }
  }
}


writeln("READS");
readCaptureForall(getter(globalArray));
readCaptureCoforall(getter(globalArray));
readCaptureCobegin(getter(globalArray));
readCaptureBegin(getter(globalArray));

proc writeit(x) {
  x = 1;
}


proc writeCaptureForall(A) { // error
  forall i in 1..10 with (const A) {
    writeit(A);
  }
}

proc writeCaptureCoforall(A) { // error
  coforall i in 1..10 with (const A) {
    writeit(A);
  }
}

proc writeCaptureCobegin(A) { // error
  cobegin with (const A) {
    writeit(A);
    writeit(A);
  }
}

proc writeCaptureBegin(A) { // error
  sync {
    begin with (const A) {
      writeit(A);
    }
  }
}

writeln("WRITES");
writeCaptureForall(getter(globalArray));
writeCaptureCoforall(getter(globalArray));
writeCaptureCobegin(getter(globalArray));
writeCaptureBegin(getter(globalArray));
