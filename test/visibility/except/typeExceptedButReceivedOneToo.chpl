use secondaryMethod except Foo;

// Similar to the other test by this name, verify that excluding a type by
// including it in your 'except' list, but calling a function that returns
// that type and then calling a secondary method on it behaves as expected.
proc main() {
  var a = returnAFoo(12);
  // We should error in the above assignment, since we're getting a variable of
  // the type we specifically didn't want to see?
  writeln(a.outerMethod(2)); // Would be 10 if we can see this method
}
