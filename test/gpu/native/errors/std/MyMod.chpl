module MyMod {
  pragma "no gpu codegen"
  proc doSomethingElse(x) {}

  proc doSomething(x) {
    doSomethingElse(x);
  }

  proc doSomethingElsePOI(x) {
    convertToString(x);
  }

  proc doSomethingPOI(x) {
    doSomethingElsePOI(x);
  }
}
