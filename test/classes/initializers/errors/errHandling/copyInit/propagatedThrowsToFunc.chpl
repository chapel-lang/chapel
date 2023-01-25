record Foo {
  var x: int;

  proc init=(val: int) throws {
    this.complete();
    x = funcThatThrows(val);
  }

  operator :(x: int, type t: Foo) {
    var result = new Foo(x);
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

proc foo() {
  try {
    var a: Foo = 15;
    writeln(a);
  } catch e {
    writeln("Hmm, first call threw");
    writeln(e.message());
  }

  try {
    var b: Foo = 7;
    writeln(b);
  } catch e {
    writeln("Good, second call threw");
    writeln(e.message());
  }
}

proc main() {
  foo();
}
