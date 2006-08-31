class C {
  var x: int;
  def foo() { writeln("C: ", this); }
}

class D: C {
  var y: int;
  def foo() { writeln("D: ", this); }
}

var s: seq of C = (/ C(1), D(2,3), C(4), D(5,6) /);

writeln(s);

s.foo();
