// This test exercises inheriting from a generic class when the child does not
// use the generic field in its new field and the parent has a type field.
class Parent {
  type t;
  var x: t;

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

class Child : Parent {
  var y: int;

  proc init(yVal: int, xVal) {
    super.init(xVal);
    y = yVal;
  }
}

proc main() {
  var ownChild = new owned Child(10, 11);
  var child = ownChild.borrow();
  writeln(child.type:string);
  writeln(child);
}
