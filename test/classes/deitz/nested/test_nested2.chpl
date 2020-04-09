class C {
  class D {
    var x: int = 0;
    proc bar() {
      writeln(d);
    }
  }
  var d: unmanaged D = new unmanaged D();
  proc foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = new unmanaged C();
c.foo();
delete c.d;
delete c;
