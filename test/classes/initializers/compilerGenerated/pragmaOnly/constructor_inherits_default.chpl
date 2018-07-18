class Parent {
  var x: int;
}

class Child: Parent {
  var y: int;

  proc Child(yVal) {
    y = yVal;
  }
}

var c = new Child(3);
writeln(c);
delete c;
