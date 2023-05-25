module M {
  var X: int;
}

module N {
  public use M;

  proc main() {
    writeln(M.X); // nope, M was not brought in
  }
}
