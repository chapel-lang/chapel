class C {
  var x : int = 1;
  def print() {
    writeln(this);
  }
}

class D : C {
  var y : float = 2.0;
  def print() {
    writeln(this);
  }
}

def foo(c : C) {
  c.print();
}

var c : C;

c = C();
foo(c);
c = D();
foo(c);
