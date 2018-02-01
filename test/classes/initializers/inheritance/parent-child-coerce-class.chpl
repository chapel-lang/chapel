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

var obj = new Derived();
var x = new Child(obj);
writeln(x);
delete x;
delete obj;
