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

var ownObj = new owned Derived();
var obj = ownObj.borrow();
var ownX = new owned Child(obj);
var x = ownX.borrow();
writeln(x);
