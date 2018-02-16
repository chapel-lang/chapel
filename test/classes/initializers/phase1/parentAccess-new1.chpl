class Parent {
  var f1 = 4.27;

  proc init(_f1 : real) {
    f1 = _f1;

    initDone();
  }
}

class Child : Parent {
  var f2 : real;

  proc init(_f1 : real) {
    super.init(_f1);

    f2 = f1 - 3.2;

    initDone();
  }
}

proc main() {
  var c: Child = new Child(1.25);

  writeln(c);

  delete c;
}
