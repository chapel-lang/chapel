module M {
  var x: int;
}

module N {
  private use M;
  var x: int;

  proc main() {
    writeln(x);
  }
}
