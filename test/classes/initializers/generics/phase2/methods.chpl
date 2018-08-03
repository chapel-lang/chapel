class ThisPhase2 {
  var someField = 5.0 - 3.0i;
  param genField: bool;

  proc init(val: complex, param genVal: bool) {
    someField = val;
    genField = genVal;
    this.complete();
    myMethod(); // Verifies that we can call methods in Phase 2
  }

  proc myMethod() {
    writeln(someField);
  }
}

proc main() {
  var c: borrowed ThisPhase2(true) = new borrowed ThisPhase2(2.5 + 1.3i, true);
}
