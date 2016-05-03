class Parent {
  var f1 = 4.27;
}

class Child : Parent {
  var f2: real;
  var f3 = f1 * 2; // 8.54

  proc init(parentFieldVal: real) {
    // Omitted field relies on parent field value.
    super.init(parentFieldVal);
    f2 = f1 - 3.2;
    // overwrites the default value of f2.  If that was costly, we could noinit
    // the field in Phase 1
  }
}

proc main() {
  var c: Child = new Child(1.25);
  writeln(c);
  delete c;
}
