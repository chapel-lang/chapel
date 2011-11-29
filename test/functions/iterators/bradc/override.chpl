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

var c1: C = new C();
var c2: C = new D();
var d : D = new D();

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

delete c1;
delete c2;
delete d;
