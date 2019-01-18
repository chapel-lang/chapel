// This test exercises inheriting from a generic class when the parent has a
// param field and the child also has a param field.
class Parent {
  param p: int;
  var x: int;

  proc init(param pVal: int) {
    p = pVal;
    x = pVal + 4;
  }
}

class Child : Parent {
  param p2: bool;
  var y: int;

  proc init(yVal: int, param pVal: int) {
    super.init(pVal);
    p2 = pVal > 10;
    y = yVal;
  }
}

proc main() {
  var child = new borrowed Child(10, 11);
  writeln(child.type:string);
  writeln(child);
}
