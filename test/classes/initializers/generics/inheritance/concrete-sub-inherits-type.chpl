// This test exercises inheriting from a generic class when the child uses the
// parent's generic type field as the type of one of its fields, but is
// otherwise concrete.
class Parent {
  type t;
  var x: t;

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

class Child : Parent {
  var y: t;

  proc init(yVal, xVal) where yVal.type == xVal.type {
    super.init(xVal);
    y = yVal;
  }
}

proc main() {
  var child = new borrowed Child(10, 11);
  writeln(child.type:string);
  writeln(child);
}
