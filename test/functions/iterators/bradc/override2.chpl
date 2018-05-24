class C {
  iter these() {
    yield 1;
    yield 2;
    yield 3;
  }
}

class D : C {
  iter these() {
    yield 3;
    yield 2;
    yield 1;
  }
}

var c1: C = new borrowed C();
var c2: C = new borrowed D();
var d : D = new borrowed D();

test(c1);
test(c2);
test(d);

proc test(x: C) {
  for i in x {
    writeln(i);
  }
  writeln();
}
