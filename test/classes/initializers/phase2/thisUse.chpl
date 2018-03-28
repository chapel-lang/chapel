class ThisPhase2 {
  var someField = 5.0 - 3.0i;

  proc init(val: complex) {
    someField = val;
    this.complete();
    outsideFunc(this);
    // Verifies that we can utilize "this" in Phase 2
  }
}

proc outsideFunc(x: ThisPhase2) {
  writeln(x.someField);
}

proc main() {
  var c: ThisPhase2 = new ThisPhase2(2.5 + 1.3i);
  delete c;
}
