module Library {
  module Sub {
    var x = 1;
  }
}

module Program {
  import Library.Sub.Sub;
  proc main() {
    writeln(Sub.x);
  }
}
