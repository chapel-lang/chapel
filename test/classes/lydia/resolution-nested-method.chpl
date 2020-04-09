// Note: there is a similar function in functions/lydia/resolution-nested-functions.chpl
record foo {
  var t: int = 6;

  proc bar() {
    writeln("foo unmodified");
  }
}

proc modifyIt() {
  // Modifying foo here affects all calls to it within this
  // scope.
  var baz: foo;
  baz.bar();
  proc foo.bar() {
    writeln("Modified foo");
  }
  baz.bar();
}


proc main() {
  // Calling foo.bar here should not be affected by modifyIt
  // shadowing foo.bar within its scope
  var baz: foo;
  baz.bar();
  modifyIt();
  baz.bar();
}
