// This test exercises inheriting from a generic class when the parent has a
// param field and the child also has a param field.
class Parent {
  param p: int;
  var x: int;

  proc init(param pVal: int) {
    p = pVal;
    x = pVal + 4;
    super.init();
  }
}

class Child : Parent {
  param p2: bool;
  var y: int;

  proc init(yVal: int, param pVal: int) {
    p2 = pVal > 10;
    y = yVal;
    super.init(pVal);
  }
}

proc main() {
  var child = new Child(10, 11);
  writeln(child.type:string);
  writeln(child);
  delete child;
}
