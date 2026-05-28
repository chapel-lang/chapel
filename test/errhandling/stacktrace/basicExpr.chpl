proc foo(): int throws {
  throw new Error("oops");
  return 2;
}
proc bar() throws {
  return foo();
}
proc foobar() throws {
  return bar();
}
config param doCatch = false;
proc main() {
  var x = try! foobar();
}
