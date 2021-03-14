
class Parent {
  var x : int;

  proc init() {
    this.x = 5;
  }

  proc inheritedMethod() {
    writeln("Parent.inheritedMethod()");
  }
}

class Child : Parent {
  var y : real;

  proc init() {
    this.y = x ** 2;
    inheritedMethod();
    (this:Child).inheritedMethod();
  }

  proc inheritedMethod() {
    writeln("Child.inheritedMethod()");
  }
}
