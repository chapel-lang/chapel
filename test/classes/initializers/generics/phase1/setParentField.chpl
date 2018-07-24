class Parent {
  var f1 = 4.27;
}

class Child : Parent {
  var f2: real;
  param f3: bool;

  proc init(parentFieldVal: real, param genVal: bool) {
    f1 = parentFieldVal; // Error, can't set the parentField in the Child's body
    f3 = genVal;
    super.init(parentFieldVal);
    // The proper way to do this is to assume the parent initializer will handle
    // it or to update the field in Phase 2
  }
}

proc main() {
  var c: shared Child(true) = new shared Child(1.25, true);
  writeln(c);
}
