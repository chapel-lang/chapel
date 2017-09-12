pragma "error mode strict"
module TestModule {

  proc throwingFn() throws {
    forall i in 1..5 {
      throw new Error("iteration: " + i);
    }
    return "OK";
  }

  proc main() {
    writeln(throwingFn());
  }
}
