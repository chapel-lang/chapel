module M {
  var x: int = 1;
}

module N {
  private use M;
  var x: int = 2;

  proc main() {
    writeln(x);
  }
}
