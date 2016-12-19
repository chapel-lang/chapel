class SomeError { }

proc throwAnError() throws {
  throw new SomeError();
}

writeln("should not continue");

try! throwAnError();

writeln("continued");
