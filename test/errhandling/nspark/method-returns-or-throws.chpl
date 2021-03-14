use ExampleErrors;
record Wrapper {

  var val: int;
  var err: owned Error? = nil;

  proc get(): int throws {
    if err then throw err;
    return val;
  }

  proc oops() {
    err = new owned StringError("called oops()");
  }
}

try {
  var w = new Wrapper(42);
  writeln(w.get());
  w.oops();
  writeln(w.get());
} catch err {
  writeln(err.message());
}
