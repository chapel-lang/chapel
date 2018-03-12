class Parent {
  var x: int;

  proc init() {
    x = 10;
  }
}


class Child: Parent {
  var y: int;

  proc Child(yVal) {
    y = yVal;
    x = 13;
  }
}

var child = new Child(4);
writeln(child);
delete child;
