


class Foo {
  proc foobar() throws {
    throw new owned Error();
  }
}

proc main() {
  var f = new owned Foo();
  try {
    f.foobar();
  } catch e : Error {
    writeln("SUCCESS");
  }
}
