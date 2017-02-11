use SomeError;

proc propError() throws {
  throwAnError();
  writeln("fail: should have propagated");
}

writeln("calling propError");
propError();
