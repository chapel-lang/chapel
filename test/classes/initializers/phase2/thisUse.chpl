class ThisPhase2 {
  var someField = 5.0 - 3.0i;

  proc init(val: complex) {
    someField = val;
    this.complete();
    outsideFunc(this);
    // Verifies that we can utilize "this" in Phase 2
  }
}

proc outsideFunc(x: borrowed ThisPhase2) {
  writeln(x.someField);
}

proc main() {
  var c: borrowed ThisPhase2 = new borrowed ThisPhase2(2.5 + 1.3i);
}
