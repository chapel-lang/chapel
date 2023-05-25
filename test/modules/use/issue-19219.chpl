module M {
  var M: int;
}

module N {
  use M;

  proc main() {
    writeln(M);
  }
}
