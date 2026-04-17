module MyMod {

  config param usePoi = true;

  pragma "no gpu codegen"
  proc doSomethingElse(x) { return 1; }

  proc doSomething(x) {
    doSomethingElse(x);
  }

  proc doSomethingElsePOI(x) {
    convertToString(x);
  }

  proc doSomethingPOI(x) {
    doSomethingElsePOI(x);
  }

  proc badLoop(type t) {
    @assertOnGpu
    foreach 1..10 {
      if usePoi {
        doSomethingPOI(new t());
      } else {
        doSomething(new t());
      }
    }
  }
}
