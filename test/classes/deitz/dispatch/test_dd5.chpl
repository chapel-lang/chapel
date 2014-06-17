use List;

class C {
  var x: int;
  proc foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  proc foo() { writeln("D: ", this); }
}

var s: list(C) = makeList( new C(1), new D(2,3), new C(4), new D(5,6) );

writeln(s);

for e in s do e.foo();

for e in s do
  delete e;
