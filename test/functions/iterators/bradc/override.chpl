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
