class C {
  class D {
    var x = 0;
    proc bar() {
      writeln(d);
    }
  }
  var d = new D();
  proc foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = new C();
c.foo();
delete c.d;
delete c;
