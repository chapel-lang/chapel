class Parent {
  var f1 = 4.27;

  proc init(_f1 : real) {
    f1 = _f1;

    complete();
  }
}

class Child : Parent {
  var f2 : real;
  var f3 = 2 * f1;

  proc init(_f1 : real) {
    super.init(_f1);

    f2 = f1 - 3.2;

    complete();
  }
}

proc main() {
  var c: borrowed Child = new borrowed Child(1.25);

  writeln(c);
}
