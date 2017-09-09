proc throwingFn() throws {
  forall i in 1..5 {
    throw new Error("iteration: " + i);
  }
  return "OK";
}

writeln(throwingFn());
