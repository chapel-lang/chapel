use typeToExcept except foo;

// This test verifies the behavior when excepting a type, but calling a function
// that returns that type.
proc main() {
  var a = bar();
  // We may want a warning for the above assignment
  writeln(a.someMethod(3));
}
