use secondaryMethod only returnAFoo;

// Similar to the other test by this name, verify that excluding a type by
// not including it in your 'only' list, but calling a function that returns
// that type and then calling a secondary method on it behaves as expected.
proc main() {
  var a = returnAFoo(12);
  // We may want a warning in the above assignment.
  writeln(a.outerMethod(2));
}
