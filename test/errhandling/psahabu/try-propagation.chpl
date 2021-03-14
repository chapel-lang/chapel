use ExampleErrors;

proc throwAnErrorToo() throws {
  try {
    throwAnError();
  }
}

try {
  throwAnErrorToo();
} catch {
  writeln("caught an error");
}
