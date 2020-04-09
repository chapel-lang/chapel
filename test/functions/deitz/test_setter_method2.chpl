class C {
  var x: int;
  proc foo() ref {
    writeln(true);
    return x;
  }
  proc foo() {
    writeln(false);
    return x;
  }

}

class D {
  var c = new unmanaged C();
  proc bar() {
    c.foo() = 2;
    writeln(c.foo());
  }
}

var d = new unmanaged D();
d.bar();
delete d.c;
delete d;
