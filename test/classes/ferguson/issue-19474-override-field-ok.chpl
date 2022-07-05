class Parent {
  var x: real;
}

class Child : Parent {
  override proc x { return 1; }
}

var c = new Child();
writeln(c.x);
