
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

  proc init(cond:bool) {
    if cond {
      this.y = -1.0;
      inheritedMethod();
    } else {
      this.y = 1.0;
      inheritedMethod();
    }
    inheritedMethod();
  }
}

var c = new owned Child(true);
writeln("c = ", c);
