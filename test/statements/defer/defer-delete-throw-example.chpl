
class C {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const doThrow = false;

proc canThrowError() throws {
  if doThrow then
    throw new owned Error();
}


proc test() throws {
  try {
    var a = new unmanaged C(1);
    defer {
      delete a;
    }
    canThrowError();
  }
}

try {
  test();
} catch e {
  writeln("caught error");
}
