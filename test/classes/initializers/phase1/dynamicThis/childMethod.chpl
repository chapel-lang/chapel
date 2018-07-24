
class Parent {
  var p : int;

  proc init() {
    this.p = 5;
  }

  proc inheritedMethod() {
    writeln("Parent.inheritedMethod()");
  }
}

class Child : Parent {
  var c : real;

  proc init() {
    this.c = p ** 2;
    inheritedMethod();
    childMethod();
  }

  proc inheritedMethod() {
    writeln("Child.inheritedMethod()");
  }

  proc childMethod() {
    writeln("Child.childMethod()");
  }
}

var c = new owned Child();
