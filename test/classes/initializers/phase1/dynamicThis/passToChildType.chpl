
class Parent {
  var x : int;

  proc init() {
    this.x = 5;
  }

  proc inheritedMethod() {
    writeln("Parent.inheritedMethod");
  }
}

class Child : Parent {
  var y : real;

  proc init() {
    this.y = x**2;
    foobar(this);
  }
}

proc foobar(c : Child) {
  writeln("foobar(Child)");
}

var c = new Child();
delete c;
