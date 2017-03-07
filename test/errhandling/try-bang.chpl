proc throwAnError() throws {
  throw new Error();
}

writeln("should not continue");

try! throwAnError();

writeln("continued");
