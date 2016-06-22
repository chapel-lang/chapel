use secondaryMethod only returnAFoo;

// Similar to the other test by this name, verify that excluding a type by
// not including it in your 'only' list, but calling a function that returns
// that type and then calling a secondary method on it behaves as expected.
proc main() {
  var a = returnAFoo(12);
  // We should error in the above assignment, since we're getting a variable of
  // the type we specifically didn't want to see?
  writeln(a.outerMethod(2));
  // The only prevents us from seeing this method, but we want to fail sooner?
}
