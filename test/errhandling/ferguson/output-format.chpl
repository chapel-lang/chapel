class MyError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;

  }
  override proc message() {
    return "custom message";
  }
}
proc throwit() throws {
  var e = new owned MyError("test error");
  writeln(e);
  throw e;
}

proc test() {
  try! throwit();
}

test();
