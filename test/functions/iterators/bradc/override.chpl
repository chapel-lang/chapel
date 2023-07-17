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

for i in c1 {
  writeln(i);
}
writeln();

for i in c2 {
  writeln(i);
}
writeln();

for i in d {
  writeln(i);
}
writeln();
