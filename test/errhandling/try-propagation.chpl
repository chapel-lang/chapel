proc throwAnError() throws {
  throw new Error();
}

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
