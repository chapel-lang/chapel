// This test is to lock in that you can call another type's operator method from
// within a method on a type that also defines an operator method for the same
// operator.
record A {
  var x: int;

  operator *(lhs: A, rhs: A) {
    return new A(lhs.x * rhs.x);
  }
}

record B {
  var a: int;

  operator *(lhs: B, rhs: B) {
    return new B(lhs.a * rhs.a);
  }

  proc otherMethod(one: A, two: A) {
    var combo = one * two;
    if (combo.x > 10) {
      writeln("multi-digit");
    } else if (combo.x > -1) {
      writeln("single digit");
    } else {
      halt("result was negative");
    }
    return new B (this.a * combo.x);
  }
}

proc main() {
  var a1 = new A(2);
  var a2 = new A(3);
  var b1 = new B(2);
  var b2 = b1.otherMethod(a1, a2);
  writeln(b2);
}
