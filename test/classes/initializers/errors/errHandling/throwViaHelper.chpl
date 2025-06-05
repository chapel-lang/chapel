class Foo {
  var x: int;

  proc helpThrow(x: int) throws {
    if (x > 5) {
      throw new Error("x too large");
    }
  }
  
  proc init(x: int) throws {
    this.x = x;
    init this;
    helpThrow(x);
  }
}

proc main() {
  try {
    var f1 = new Foo(3);
    writeln(f1);
    var f2 = new Foo(7);
    writeln(f2);
  } catch e {
    writeln(e.message());
  }
}
