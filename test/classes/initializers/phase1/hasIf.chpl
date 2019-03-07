class IfInit {
  var f1: bool;
  var f2: int;

  proc init(val: int) {
    f1 = val > 10;
    // Verifies that if statements can be used in Phase 1 of an initializer
    if (f1) {
      f2 = 10;
    } else {
      f2 = val;
    }
  }
}

proc main() {
  var c: borrowed IfInit = new borrowed IfInit(12);
  writeln(c);
}
