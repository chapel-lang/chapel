/*
This code is a simplification of another scenario where we need to
search also the scope of the caller for visible functions.

It is simplified from:
  test/arrays/ferguson/array-initialization-patterns/private-record.chpl

specifically:
  use PrivateDist;
  record R {...}
  var A: [PrivateSpace] R;
*/

module Application {
  use MyPrivateDist;

  record R {
    var x: int = 0;
    proc init() {
      compilerError("success");
    }
  }

  proc main {
    var A = new MyPrivateArray(R);
  }
}

module MyPrivateDist {
  class MyPrivateArray {
    proc init(type eltType) {
      var default: eltType; // needs functions visible to the caller
    }
  }
}
