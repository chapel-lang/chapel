// Test to exercise printing of operators in call stack when failure occurs
record bar {
  var x: int;
}

// Must be generic to turn up in call stack
operator bar.+(lhs, rhs: bar) {
  someBrokenFunc(lhs.x); // Expected to fail
  return new bar(lhs.x + rhs.x);
}

// Intentionally not a match for the call above
proc someBrokenFunc(y: string) {
  writeln(y);
}

proc main() {
  var a = new bar(3);
  var b = new bar(17);
  writeln(a + b);
}
