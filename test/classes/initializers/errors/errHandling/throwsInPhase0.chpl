class Parent {
  var x: int;
}

proc funcThatThrows() throws {
  throw new Error("Wheeeeee!");
}

class Child: Parent {
  var y: int;

  proc init(yVal: int) throws {
    funcThatThrows(); // Shouldn't work yet
    super.init();
    y = yVal;
  }
}

proc main() {
  try {
    var c = new Child(3);
    writeln(c);
  } catch e {
    writeln(e.message());
  }
}
