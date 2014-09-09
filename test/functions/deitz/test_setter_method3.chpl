class C {
  var x;
  proc foo() ref {
    writeln(setter);
    return x;
  }
}

class D {
  var c = new C(1);
  proc bar() {
    c.foo() = 2;
    writeln(c.foo());
  }
}

var d = new D();
d.bar();
delete d.c;
delete d;
