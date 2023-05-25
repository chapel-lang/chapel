record TestUnstableOperator {
  var x: int;

  @unstable("Addition of TestUnstableOperator is unstable")
  operator +(arg1: TestUnstableOperator, arg2: TestUnstableOperator) {
    return new TestUnstableOperator(arg1.x + arg2.x);
  }
}

@unstable("Subtraction of TestUnstableOperator is deprecated")
operator TestUnstableOperator.-(arg1: TestUnstableOperator, arg2: TestUnstableOperator) {
  return new TestUnstableOperator(arg1.x - arg2.x);
}

proc main() {
  var a = new TestUnstableOperator(3);
  var b = new TestUnstableOperator(2);
  writeln(a + b);
  writeln(a - b);
}