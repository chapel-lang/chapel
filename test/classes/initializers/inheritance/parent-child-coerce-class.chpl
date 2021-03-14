class Base { }
class Derived : Base { }

class Parent {
  proc init(x: borrowed Derived) {
    writeln("In Parent init(Derived)");
  }
  proc init() { }
}

class Child : Parent {
  proc init(x: borrowed Base) {
    writeln("In Child init(Base)");
  }
}

var obj = new borrowed Derived();
var x = new borrowed Child(obj);
writeln(x);
