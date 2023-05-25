module M {
  var x: int;
}

module N {
  public use M;
  var x: int;

  proc main() {
    writeln(x);
  }
}
