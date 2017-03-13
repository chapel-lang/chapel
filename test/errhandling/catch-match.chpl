use ThrowError;

class OtherError: Error {

}

proc throwOtherError() throws {
  throw new OtherError();
}

try {
  throwAnError();
} catch e: OtherError {
  write("should not catch here");
} catch {
  write("caught Error");
}

try {
  throwOtherError();
} catch e: OtherError {
  write("caught OtherError");
} catch {
  write("should not catch here");
}
