class MyError : Error {
  proc message() {
    return "custom message";
  }
}

proc throwit() throws {
  var e = new MyError("test error");
  writeln(e);
  throw e;
}

proc test() {
  try! throwit();
}

test();
