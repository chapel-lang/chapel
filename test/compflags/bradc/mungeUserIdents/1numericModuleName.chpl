record Box {
  var value: int;

  proc getValue() {
    return value;
  }
}

module Inner {
  record Nested {
    var value: int;
  }

  record Generic {
    type t;
    var value: t;
  }

  class GenericClass {
    type t;
    var value: t;
  }

  enum Choice { second }
}

proc increment(x: int) {
  return x + 1;
}

var box = new Box(41);
var nested = new Inner.Nested(1);
var generic = new Inner.Generic(int, 2);
var genericClass = new owned Inner.GenericClass(int, 4);
var choice = Inner.Choice.second;
writeln(increment(box.getValue()));
writeln(nested.value + generic.value + genericClass.value, " ", choice);
