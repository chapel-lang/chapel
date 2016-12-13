proc throwAnError() throws {
  throw new Error();
}

try {
  throwAnError();
} catch {
  write("caught an error");
}
