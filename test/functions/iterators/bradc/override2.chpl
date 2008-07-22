class C {
  def these() {
    yield 1;
    yield 2;
    yield 3;
  }
}

class D : C {
  def these() {
    yield 3;
    yield 2;
    yield 1;
  }
}

var c1: C = new C();
var c2: C = new D();
var d : D = new D();

test(c1);
test(c2);
test(d);

def test(x: C) {
  for i in x {
    writeln(i);
  }
  writeln();
}


