//
// This test was initially created to track a bug where the presence of a
// ``try!`` would result in malformed AST for arguments with default values
// that relied on other arguments. See issue #14465
//

proc foo(a:int, b=a*2) throws {
  return a+b;
}

proc defaultHelper(arg) {
  return arg;
}

proc theFunction(firstArg = 1234, secondArg = defaultHelper(firstArg)) throws {
  // Dummy error so that this function throws
  if firstArg == 123456789 then
    throw new Error("dummy error");

  return secondArg;
}

proc main() {
  var x = try! foo(1);
  writeln(x);

  var thing = try! theFunction();
  writeln(thing);
}
