use typeToExcept except foo;

// This test verifies the behavior when excepting a type, but calling a function
// that returns that type.
proc main() {
  var a = bar();
  // We should error in the above assignment, since we're getting a variable of
  // the type we specifically didn't want to see?
  writeln(a.someMethod(3)); // Would be 6 if we can see this method.
}
