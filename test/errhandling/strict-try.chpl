use ThrowError;

proc propError() throws {
  throwAnError();
}

try {
  propError();
} catch {
  writeln("fail: did not fail strict mode");
}
