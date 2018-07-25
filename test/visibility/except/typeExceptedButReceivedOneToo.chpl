use secondaryMethod except Foo;

// Similar to the other test by this name, verify that excluding a type by
// including it in your 'except' list, but calling a function that returns
// that type and then calling a secondary method on it behaves as expected.
proc main() {
  var a = returnAFoo(12);
  // We may want a warning for the above assignment
  writeln(a.outerMethod(2));
}
