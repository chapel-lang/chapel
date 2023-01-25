proc funcThatThrows1() throws {
  throw new Error("Greater than 5");
}

proc funcThatThrows2() throws {
  throw new Error("Less than or equal to 5");
}

class Foo {
  var x: int;

  proc init(xVal: int) throws {
    if (xVal > 5) {
      x = xVal;
      funcThatThrows1();
      this.complete();
    } else {
      x = xVal + 5;
      this.complete();
      funcThatThrows2();
    }
  }
}

proc main() {
  try {
    var f = new Foo(3);
    writeln(f);
  } catch e {
    writeln(e.message());
  }

  try {
    var f2 = new Foo(7);
    writeln(f2);
  } catch e {
    writeln(e.message());
  }
}
