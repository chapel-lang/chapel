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


proc writeCaptureFor(A) {
  for i in 1..10 {
    writeit(A);
  }
}

proc writeCaptureForall(A) {
  forall i in 1..10 {
    writeit(A);
  }
}

proc writeCaptureCoforall(A) {
  coforall i in 1..10 {
    writeit(A);
  }
}

proc writeCaptureCobegin(A) {
  cobegin {
    writeit(A);
    writeit(A);
  }
}

proc writeCaptureBegin(A) {
  sync {
    begin {
      writeit(A);
    }
  }
}

proc writePromoted(A) {
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
