use ThrowError;

proc propError() throws {
  throwAnError();
}

try {
  propError();
} catch {
  writeln("did not fail strict mode");
}
