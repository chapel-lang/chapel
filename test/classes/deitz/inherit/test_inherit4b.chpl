class C {
  var x : int = 1;
  def print() {
    writeln(this);
  }
}

class D : C {
  var y : real = 2.0;
  def print() {
    writeln(this);
  }
}

def foo(c : C) {
  c.print();
}

var c : C;

c = new C();
foo(c);
c = new D();
foo(c);
