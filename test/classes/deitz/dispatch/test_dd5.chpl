use List;

class C {
  var x: int;
  proc foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  proc foo() { writeln("D: ", this); }
}

var s: list(C) = makeList( new unmanaged C(1), new unmanaged D(2,3), new unmanaged C(4), new unmanaged D(5,6) );

writeln(s);

for e in s do e.foo();

for e in s do
  delete e;

s.destroy();
