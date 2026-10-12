record myRec {
  proc init=(rhs: myRec) {
    writeln("copy constructor");
  }
}

proc foo1() {
  var a = new myRec();
  defer {
    writeln(a);
  }
  var b = a; // not copy elided; defer uses it.
  return;
}
proc foo2() {
  var a = new myRec();
  defer {
    writeln("not using a");
  }
  var b = a; // should be eligible for copy elision, not print 'copy constructor'
  return;
}

foo1();
writeln("---");
foo2();
