module M {
  module N {
    var x = 100;
  }
}

module N {
  var x = 42;
}

module Application {
  use N only;
  use M;
  proc main() {
    writeln(N.x);
  }
}
