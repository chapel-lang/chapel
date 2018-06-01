class Base { }
class Derived : Base { }

class Parent {
  proc init(x:Derived) {
    writeln("In Parent init(Derived)");
  }
  proc init() { }
}

class Child : Parent {
  proc init(x:Base) {
    writeln("In Child init(Base)");
  }
}

var obj = new owned Derived();
var x = new owned Child(obj);
writeln(x);
