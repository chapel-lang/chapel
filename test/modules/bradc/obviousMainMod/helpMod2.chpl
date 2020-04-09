record R {
  var x: int;

  proc foo() {
    writeln("In R.foo");
  }
}

class C {
  var x: int;

  proc foo() {
    writeln("In C.foo");
  }
}


union U {
  var x: int;

  proc foo() {
    writeln("In U.foo");
  }
}
