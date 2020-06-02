/*
A simplification of 
  test/library/standard/Random/RandomStreamInterface/choiceTestArray.chpl
*/

module Application {
  use MyRandom;

  proc main {
    var c = choice(5);
  }
}

module MyRandom {
  public use myPCGRandom;

  proc _choice(stream) {
    compilerError("success");
  }

  module myPCGRandom {
    proc choice(stream) {
      // need to import MyRandom._choice;
      _choice(stream);
    }
  }
}
