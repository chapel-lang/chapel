class Parent {
  var f1 = 4.27;

  proc init(val:real) {
    this.f1 = val;
  }
}

class Child : Parent {
  var f2: real;
  var f3 = f1 * 2; // 8.54

  proc init(parentFieldVal: real) {
    // Omitted field relies on parent field value.
    super.init(parentFieldVal);
    f2 = f1 - 3.2;
  }
}

proc main() {
  var c: borrowed Child = new borrowed Child(1.25);
  writeln(c);
}
