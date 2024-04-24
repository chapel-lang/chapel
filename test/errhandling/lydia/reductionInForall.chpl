// This test doesn't do much, but it's intended to lock in that the compiler
// doesn't fall over at the mere presence of a reduction within a forall when
// the forall also calls throwing functions.
proc throwingFunc() throws {
  forall i in 0..2 {
    var tup = (0, 1, 2, 0, 1, 2);
    otherThrowingFunction();
    var reductionRes = + reduce tup;
    writeln(reductionRes);
  }
}

proc otherThrowingFunction() throws {
  var unimportant = 17;
  if (unimportant < 10) {
    throw new Error();
  }
}

proc main() {
  try! throwingFunc();
}
