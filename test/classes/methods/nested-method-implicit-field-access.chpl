module DefinesX {
  record X {
    var field: real;
    var x: real;
  }
}

module DefinesY {
  record Y {
    var field: int;
    var y: int;
  }
}

module Main {
  use DefinesX;
  use DefinesY;

  proc X.outer() {
    proc Y.inner() {
      x;     // should refer to X.x, via the outer method
      field; // should refer to Y.field, via the inner method
    }
    (new Y()).inner(); // call inner() to make sure it is resolved

    x; // should refer to X.x
    //y; // should not resolve
    field; // should refer to X.field
  }

  proc main() {
    (new X()).outer(); // call outer() to make sure it is resolved
  }
}
