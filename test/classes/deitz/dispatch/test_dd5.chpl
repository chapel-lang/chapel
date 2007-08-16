use List;

class C {
  var x: int;
  def foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  def foo() { writeln("D: ", this); }
}

var s: list of C = makeList( C(1), D(2,3), C(4), D(5,6) );

writeln(s);

for e in s do e.foo();
