use ExampleErrors;
record Wrapper {

  type retType;
  var val: retType;
  var err: owned Error? = nil;

  proc get(): retType throws {
    if err then throw err;
    return val;
  }

  proc oops() {
    err = new owned StringError("called oops()");
  }
}

try {
  var w = new Wrapper(int, 42);
  writeln(w.get());
  w.oops();
  writeln(w.get());
} catch err {
  writeln(err.message());
}
