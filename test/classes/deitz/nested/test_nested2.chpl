class C {
  class D {
    var x: int = 0;
    proc bar() {
      writeln(d);
    }
  }
  var d: D = new D();
  proc foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = new C();
c.foo();
delete c.d;
delete c;
