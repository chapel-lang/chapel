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

var obj = (new owned Derived()).borrow();
var x = (new owned Child(obj)).borrow();
writeln(x);
