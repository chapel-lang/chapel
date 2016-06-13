class Parent {
  var f1 = 4.27;
}

class Child : Parent {
  var f2: real;
  var f3 = f1 * 2; // 8.54

  proc init(parentFieldVal: real) {
    f2 = f1 - 3.2; // Error, parent field f1 hasn't been initialized yet
    // Omitted field relies on parent field value.
    super.init(parentFieldVal);
    // The proper way to do this is to either use the value we know the parent
    // field will be updated to, or wait to assign the field until Phase 2
  }
}

proc main() {
  var c: Child = new Child(1.25);
  writeln(c);
  delete c;
}
