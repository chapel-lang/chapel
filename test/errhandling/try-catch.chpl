proc throwAnError() throws {
  throw new Error();
}

try {
  throwAnError();
} catch {
  writeln("caught an error");
}
