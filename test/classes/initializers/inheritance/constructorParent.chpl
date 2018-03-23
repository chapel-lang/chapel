class Parent {
  var x: int;

  proc Parent() {
    x = 10;
  }
}

class Child: Parent {
  var y: int;

  proc init(yVal) {
    super.init(13);
    y = yVal;
  }
}

var child = new Child(4);
writeln(child);
delete child;
