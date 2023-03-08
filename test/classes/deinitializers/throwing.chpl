class Foo {
  var x: int;
}

class Bar {
  var foo: unmanaged Foo;

  proc deinit() throws {
    delete foo;
  }
}

proc createInFunc() throws {
  var bar = new Bar(new unmanaged Foo(14));
  writeln(bar);
}

proc main() {
  try {
    createInFunc();
  } catch e {
    writeln(e.message());
  }
}
