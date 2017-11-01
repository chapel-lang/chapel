
class C {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const doThrow = false;

proc canThrowError() throws {
  if doThrow then
    throw new Error();
}


proc test() throws {
  try {
    var a = new C(1);
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
