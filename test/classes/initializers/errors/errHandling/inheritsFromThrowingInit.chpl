proc throwingFunc() throws {
  throw new Error("Catch me!");
}

class Parent {
  var x: int;

  proc init(xVal: int) throws {
    x = xVal;
    this.complete();
    throwingFunc();
  }

  // So it's at least possible to make valid instances of the
  // type
  proc init() {
    x = 5;
  }
}

class Child: Parent {
  var y: int;

  proc init(xVal: int, yVal: int) throws {
    // Verifies that we can't call throwing parent initializers
    // yet
    super.init(xVal);
    y = yVal;
    this.complete();
  }

  proc init(yVal: int) {
    super.init(); // Should be fine
    y = yVal;
    this.complete();
  }
}

proc main() {
  // Call good parent initializer
  var a = new Parent();
  writeln(a);

  // Call throwing parent initializer
  try {
    var b = new Parent(3);
    writeln(b);
  } catch e {
    writeln(e.message());
  }

  // Call good child initializer
  var c = new Child(3);
  writeln(c);

  // Call throwing child initializer
  try {
    var d = new Child(2, 6);
    writeln(d);
  } catch e {
    writeln(e.message());
  }
}
