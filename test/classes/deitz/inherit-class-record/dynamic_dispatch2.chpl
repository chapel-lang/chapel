class C {
  var x: int;
  proc foo() {
    writeln("C: ", this);
  }
}

class D: C {
  type t;
  var y: t;
  override proc foo() {
    writeln("D: ", this);
  }
}

proc bar(c: unmanaged C) {
  c.foo();
  delete c;
}

bar(new unmanaged C(x=3));
bar(new unmanaged D(t=int, x=1, y=4));
bar(new unmanaged D(t=real, x=2, y=4.0));
