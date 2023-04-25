record Foo {
  var x: int;

  proc init=(val: int) throws {
    x = funcThatThrows(val); // Should not be allowed yet
  }

  operator :(x: int, type t: Foo) {
    var result: Foo = new Foo(x);
    return result;
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
  var a: Foo = 15;
  writeln(a);
  var b: Foo = 7;
  writeln(b);
}
