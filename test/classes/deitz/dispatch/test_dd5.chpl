use List;

class C {
  var x: int;
  def foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  def foo() { writeln("D: ", this); }
}

var s: list(C) = makeList( new C(1), new D(2,3), new C(4), new D(5,6) );

writeln(s);

for e in s do e.foo();
