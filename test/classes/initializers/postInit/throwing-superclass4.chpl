interface dummy { }
interface dummy2 { }

class Foo {
  var x: int;

  proc postinit() throws {
    writeln("In Foo's postinit");
  }
}

class Bar : dummy, Foo, dummy2 {
  proc init(x: int) {
    super.init(x=x);
  }

  proc postinit() throws {
    if (x > 5) {
      throw new Error("x too large");
    }
  }
}

proc main() {
  try {
    var f1 = new Bar(3);
    writeln(f1);
    var f2 = new Bar(7);
    writeln(f2);
  } catch e {
    writeln(e.message());
  }
}
