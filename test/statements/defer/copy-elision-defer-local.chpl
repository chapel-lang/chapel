record myRec {
  proc init=(rhs: myRec) {
    writeln("copy constructor");
  }
}

proc foo() {
  var a = new myRec();
  defer {
    var b = a; // ought to copy
    writeln("b: ", b);
    var c = new myRec();
    var d = c; // ought to elide
    writeln("d: ", d);
  }
  return;
}

foo();
