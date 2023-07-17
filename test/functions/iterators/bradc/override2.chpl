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

var ownC1 = new owned C();
var c1: borrowed C = ownC1.borrow();
var ownC2 = new owned D();
var c2: borrowed C = ownC2.borrow();
var ownD = new owned D();
var d : borrowed D = ownD.borrow();

test(c1);
test(c2);
test(d);

proc test(x: borrowed C) {
  for i in x {
    writeln(i);
  }
  writeln();
}
