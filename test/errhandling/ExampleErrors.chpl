proc throwAnError() throws {
  throw new Error();
}


pragma "use default init"
class OtherError : Error { }

proc throwOtherError() throws {
  throw new OtherError();
}

class StringError : Error {
  var msg: string;

  proc init(msg:string) {
    this.msg = msg;
  }

  proc message() {
    return msg;
  }
}
