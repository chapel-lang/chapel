proc throwAnError() throws {
  throw new Error();
}


class OtherError : Error { }

proc throwOtherError() throws {
  throw new OtherError();
}

class StringError : Error {
  var msg: string;

  proc init(msg:string) {
    this.msg = msg;
  }

  override proc message() {
    return msg;
  }
}
