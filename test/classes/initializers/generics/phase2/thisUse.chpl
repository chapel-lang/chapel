class ThisPhase2 {
  var someField = 5.0 - 3.0i;
  param genField: bool;

  proc init(val: complex, param genVal: bool) {
    someField = val;
    genField = genVal;
    this.complete();
    outsideFunc(this);
    // Verifies that we can utilize "this" in Phase 2
  }
}

proc outsideFunc(x: ThisPhase2) {
  writeln(x.someField);
}

proc main() {
  var c: ThisPhase2(true) = new ThisPhase2(2.5 + 1.3i, true);
  delete c;
}
