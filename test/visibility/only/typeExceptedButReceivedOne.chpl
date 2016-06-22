use typeToExcept only bar;

// This test verifies the behavior when excluding a type by not including it
// in your 'only' list, but calling a function that returns that type.
proc main() {
  var a = bar();
  // We should error in the above assignment, since we're getting a variable of
  // the type we specifically didn't want to see?
  writeln(a.someMethod(3)); // The only prevents us from seeing this method, but we want to fail sooner?
}
