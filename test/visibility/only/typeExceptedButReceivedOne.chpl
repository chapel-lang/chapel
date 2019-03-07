use typeToExcept only bar;

// This test verifies the behavior when excluding a type by not including it
// in your 'only' list, but calling a function that returns that type.
proc main() {
  var a = bar();
  // We may want a warning for the above assignment
  writeln(a.someMethod(3));
}
