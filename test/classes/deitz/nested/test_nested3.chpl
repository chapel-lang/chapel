class B {
  proc bar() { writeln("right bar"); }
}

class C {
  class D: B {
    proc foo() { bar(); }
  }
  proc bar() { writeln("wrong bar"); }
  var d = new unmanaged D();
}

var c = new unmanaged C();
c.d.foo();
delete c.d;
delete c;
