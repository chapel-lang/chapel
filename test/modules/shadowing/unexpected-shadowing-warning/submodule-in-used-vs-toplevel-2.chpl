module M {
  module N {
    var x = 100;
  }
}

module N {
  var x = 42;
}

module Application {
  use M;
  use N only;
  proc main() {
    writeln(N.x);
  }
}
