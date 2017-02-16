proc throwAnError() throws {
  throw new Error();
}

write("should not continue");

try! throwAnError();

write("continued");
