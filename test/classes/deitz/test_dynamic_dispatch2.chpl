class C {
  var x: int;
  proc foo() {
    writeln("C: ", this);
  }
}

class D: C {
  type t;
  var y: t;
  proc foo() {
    writeln("D: ", this);
  }
}

proc bar(c: C) {
  c.foo();
  delete c;
}

bar(new C(x=3));
bar(new D(t=int, x=1, y=4));
bar(new D(t=real, x=2, y=4.0));
