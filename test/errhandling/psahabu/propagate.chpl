use ExampleErrors;

proc propError() throws {
  try throwAnError();
  writeln("fail: should have propagated");
}

writeln("calling propError");
propError();
