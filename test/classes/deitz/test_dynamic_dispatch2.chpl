class C {
  var x: int;
  def foo() {
    writeln("C: ", this);
  }
}

class D: C {
  type t;
  var y: t;
  def foo() {
    writeln("D: ", this);
  }
}

def bar(c: C) {
  c.foo();
}

bar(new C(x=3));
bar(new D(t=int, x=1, y=4));
bar(new D(t=real, x=2, y=4.0));
