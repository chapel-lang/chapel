class ThisPhase2 {
  var someField = 5.0 - 3.0i;

  proc init(val: complex) {
    someField = val;
    this.complete();
    myMethod(); // Verifies that we can call methods in Phase 2
  }

  proc myMethod() {
    writeln(someField);
  }
}

proc main() {
  var ownC = new owned ThisPhase2(2.5 + 1.3i);
  var c: borrowed ThisPhase2 = ownC.borrow();
}
