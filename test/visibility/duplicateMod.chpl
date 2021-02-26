module Library {
  module Sub {
    var x = 1;
  }
}

module Program {
  use Library.Sub.Sub;
  proc main() {
    writeln(x);
  }
}
