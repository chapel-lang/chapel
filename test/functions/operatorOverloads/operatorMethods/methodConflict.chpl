record A {
  var x: int;

  operator +(lhs: A, rhs: B) {
    writeln("In A.+");
    return new A(lhs.x + rhs.x);
  }
}

record B {
  var x: int;

  operator +(lhs: A, rhs: B) {
    writeln("In B.+");
    return new B(lhs.x + rhs.x);
  }
}

proc main() {
  var r1 = new A(3);
  var r2 = new B(5);
  var r3 = r1 + r2; // Uh oh!  Both apply
  writeln(r3);
}
