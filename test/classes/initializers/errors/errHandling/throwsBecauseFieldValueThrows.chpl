class Foo {
  var x: int;

  proc init(val: int) throws {
    x = funcThatThrows(val); // Should not be allowed yet
  }
}

proc funcThatThrows(val: int) throws {
  if (val < 10) {
    throw new Error("too small!");
  } else {
    return val;
  }
}

proc main() {
  var a = new Foo(15);
  writeln(a);
  var b = new Foo(7);
  writeln(b);
}
