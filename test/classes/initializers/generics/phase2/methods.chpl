class ThisPhase2 {
  var someField = 5.0 - 3.0i;
  param genField: bool;

  proc init(val: complex, param genVal: bool) {
    someField = val;
    genField = genVal;
    this.initDone();
    myMethod(); // Verifies that we can call methods in Phase 2
  }

  proc myMethod() {
    writeln(someField);
  }
}

proc main() {
  var c: ThisPhase2(true) = new ThisPhase2(2.5 + 1.3i, true);
  delete c;
}
