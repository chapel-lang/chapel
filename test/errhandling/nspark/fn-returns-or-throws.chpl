use ExampleErrors;
proc oops(): int throws {
  throw new owned StringError("called oops()");
}

try {
  writeln("before");
  var y = oops();
  writeln("after");
} catch err {
  writeln(err.message());
}
