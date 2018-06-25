use ExampleErrors;
record Wrapper {

  var val: int;
  var err: unmanaged Error;

  proc init(v: int) {
    val = v;
    err = nil;
  }

  proc deinit() throws {
    if err then throw err;
  }

  proc oops() {
    err = new unmanaged StringError("called oops()");
  }
}

try {
  var w = new Wrapper(42);
  w.oops();
} catch err {
  writeln(err.message());
}
