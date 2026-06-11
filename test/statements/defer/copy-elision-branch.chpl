record myRec {
  proc init=(rhs: myRec) {
    writeln("copy constructor");
  }
}

proc foo() {
  var aOrig = new myRec();
  var aCopy = aOrig;
  writeln("done with a");

  var bOrig = new myRec();
  var bCopy = bOrig;
  writeln("done with b");

  defer { writeln(aOrig); }

  var cond: bool;
  if cond {
    return;
  } else {
    return;
  }

  defer { writeln(bOrig); }
}

foo();
