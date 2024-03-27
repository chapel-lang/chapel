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
