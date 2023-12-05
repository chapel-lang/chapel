module M {
  var x: int = 1;
}

module N {
  var x: int = 2;
}

module App {
  public use M;
  private use N;

  proc main() {
    writeln(x);
  }
}
