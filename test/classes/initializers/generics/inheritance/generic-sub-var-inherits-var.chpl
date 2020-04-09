// This test exercises inheriting from a generic class when the parent has a
// generic var field and the child is concrete.
class Parent {
  var gen;
  var x: int;

  proc init(xVal: int) {
    gen = -xVal;
    x = xVal;
  }
}

class Child : Parent {
  var gen2;
  var y: int;

  proc init(yVal: int, xVal: int) {
    super.init(xVal);
    gen2 = -yVal;
    y = yVal;
  }
}

proc main() {
  var child = new borrowed Child(10, 11);
  writeln(child.type:string);
  writeln(child);
}
