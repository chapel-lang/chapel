class Parent {
  proc x { return 0; }
}

class Child : Parent {
  override proc x { return 1; }
}

var c = new Child();
writeln(c.x);
