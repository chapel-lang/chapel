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

bar(C(x=3));
bar(D(t=int, x=1, y=4));
bar(D(t=real, x=2, y=4.0));
