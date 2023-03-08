module M {
  var a : int = 3;
}

module N {
  use M except a as b;

  proc main() {
    writeln("here");
  }
}
