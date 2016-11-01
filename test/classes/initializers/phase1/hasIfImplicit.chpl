class IfInit {
  var f1: bool;
  var f2: int;

  proc init(val: int) {
    f1 = val < 10;
    // Verifies that if statements can be used in Phase 1 of an initializer
    if (!f1) {
      f2 = val;
    }
    // f2 set implicitly to 0 in else branch
    super.init();
  }
}

proc main() {
  var c: IfInit = new IfInit(3);
  writeln(c);
  delete c;
}
