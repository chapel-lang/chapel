record Wrapper {

  var val: int;
  var err: Error = nil;

  proc action() throws {
    if err then throw err;
    writeln(val);
  }

  proc oops() {
    err = new Error("called oops()");
  }
}

try {
  var w = new Wrapper(42);
  w.action();
  w.oops();
  w.action();
} catch err {
  writeln(err.msg);
}
