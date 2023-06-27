// This test exercises inheriting from a generic class with a type field when
// the child class also has a type field.
class Parent {
  type t;
  var x: t;

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

class Child : Parent {
  type t2;
  var y: t2;

  proc init(yVal, xVal) {
    super.init(xVal);
    t2 = yVal.type;
    y = yVal;
  }
}

proc main() {
  var ownChild = new owned Child(10, 11);
  var child = ownChild.borrow();
  writeln(child.type:string);
  writeln(child);
}
