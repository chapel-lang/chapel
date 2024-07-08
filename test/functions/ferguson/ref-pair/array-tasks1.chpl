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

proc readCoforallA(A) {
  coforall a in A {
    readit(a);
  }
}


proc readCaptureFor(A) {
  for i in 1..10 {
    readit(A);
  }
}

proc readCaptureForall(A) {
  forall i in 1..10 {
    readit(A);
  }
}

proc readCaptureCoforall(A) {
  coforall i in 1..10 {
    readit(A);
  }
}

proc readCaptureCobegin(A) {
  cobegin {
    readit(A);
    readit(A);
  }
}

proc readCaptureBegin(A) {
  sync {
    begin {
      readit(A);
    }
  }
}


proc readPromoted(A) {
  var B = A;
}


writeln("READS");
readForA(getter(globalArray));
readForallA(getter(globalArray));
readCoforallA(getter(globalArray));
readCaptureFor(getter(globalArray));
readCaptureForall(getter(globalArray));
readCaptureCoforall(getter(globalArray));
readCaptureCobegin(getter(globalArray));
readCaptureBegin(getter(globalArray));
readPromoted(getter(globalArray));

proc writeit(ref x) {
  x = 1;
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

proc writeCoforallA(ref A) {
  coforall a in A {
    writeit(a);
  }
}


proc writeCaptureFor(ref A) {
  for i in 1..10 {
    writeit(A);
  }
}

proc writeCaptureForall(ref A) {
  forall i in 1..10 with (ref A) {
    writeit(A);
  }
}

proc writeCaptureCoforall(ref A) {
  coforall i in 1..10 {
    writeit(A);
  }
}

proc writeCaptureCobegin(ref A) {
  cobegin {
    writeit(A);
    writeit(A);
  }
}

proc writeCaptureBegin(ref A) {
  sync {
    begin {
      writeit(A);
    }
  }
}

proc writePromoted(ref A) {
  A = 1;
}

writeln("WRITES");
writeForA(getter(globalArray));
writeForallA(getter(globalArray));
writeCoforallA(getter(globalArray));
writeCaptureFor(getter(globalArray));
writeCaptureForall(getter(globalArray));
writeCaptureCoforall(getter(globalArray));
writeCaptureCobegin(getter(globalArray));
writeCaptureBegin(getter(globalArray));
writePromoted(getter(globalArray));
