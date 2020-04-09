class Parent {
  var f1 = 4.27;
}

class Child : Parent {
  var f2: real;

  proc init(parentFieldVal: real) {
    f1 = parentFieldVal; // Error, can't set the parentField in the Child's body
    super.init(parentFieldVal);
    // The proper way to do this is to assume the parent initializer will handle
    // it or to update the field in Phase 2
  }
}

proc main() {
  var c: unmanaged Child = new unmanaged Child(1.25);
  writeln(c);
  delete c;
}
