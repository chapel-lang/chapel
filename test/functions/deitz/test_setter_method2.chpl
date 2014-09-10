class C {
  var x: int;
  proc foo() ref {
    writeln(setter);
    return x;
  }
}

class D {
  var c = new C();
  proc bar() {
    c.foo() = 2;
    writeln(c.foo());
  }
}

var d = new D();
d.bar();
delete d.c;
delete d;
