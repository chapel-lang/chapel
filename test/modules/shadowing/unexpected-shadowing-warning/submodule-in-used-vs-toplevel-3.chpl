module M {
  module N {
    var x: int;
  }
}

module N {
  var x: real;
}

module Main {
  proc main() {
    use M;
    use N;
    x;
  }
}
