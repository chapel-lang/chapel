use ExampleErrors;
proc oops(): int throws {
  throw new StringError("called oops()");
  return 99;
}

try {
  writeln("before");
  var y = oops();
  writeln("after");
} catch err {
  writeln(err.message());
}
