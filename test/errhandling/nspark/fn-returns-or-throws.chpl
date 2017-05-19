proc oops(): int throws {
  throw new Error("called oops()");
  return 99;
}

try {
  writeln("before");
  var y = oops();
  writeln("after");
} catch err {
  writeln(err.msg);
}
