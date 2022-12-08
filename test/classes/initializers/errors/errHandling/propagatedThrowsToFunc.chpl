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

proc foo() {
  try {
    var a = new Foo(15);
    writeln(a);
  } catch e {
    writeln("Hmm, first call threw");
    writeln(e.message());
  }

  try {
    var b = new Foo(7);
    writeln(b);
  } catch e {
    writeln("Good, second call threw");
    writeln(e.message());
  }
}

proc main() {
  foo();
}
