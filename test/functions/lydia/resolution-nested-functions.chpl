// Note: there is a similar function in classes/lydia/resolution-nested-method.chpl

proc foo() {
  writeln("Original foo");
}

proc modifyIt() {
  // Modifying foo here affects all calls to it within this
  // scope.
  foo();
  proc foo() {
    writeln("Modified foo");
  }
  foo();
}

proc main() {
  // Calling foo here should not be affected by modifyIt
  // shadowing foo within its scope
  foo();
  modifyIt();
  foo();
}