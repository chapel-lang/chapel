proc throwAnError() throws {
  throw new Error();
}


class OtherError : Error { }

proc throwOtherError() throws {
  throw new OtherError();
}
