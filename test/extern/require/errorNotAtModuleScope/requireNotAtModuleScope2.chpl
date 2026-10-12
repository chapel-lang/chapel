record R {
  proc foo() {
    require "dummyFile.chpl"; // should error
  }
}
