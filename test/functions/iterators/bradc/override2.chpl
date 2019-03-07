class C {
  iter these() {
    yield 1;
    yield 2;
    yield 3;
  }
}

class D : C {
  override iter these() {
    yield 3;
    yield 2;
    yield 1;
  }
}

var c1: borrowed C = new borrowed C();
var c2: borrowed C = new borrowed D();
var d : borrowed D = new borrowed D();

test(c1);
test(c2);
test(d);

proc test(x: borrowed C) {
  for i in x {
    writeln(i);
  }
  writeln();
}
