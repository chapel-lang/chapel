record R {
  var x: int;

  def foo() {
    writeln("In R.foo");
  }
}

class C {
  var x: int;

  def foo() {
    writeln("In C.foo");
  }
}


union U {
  var x: int;

  def foo() {
    writeln("In U.foo");
  }
}
