module M {
  var x: int;
}

module N {
  public import M.x;
  var x: int;

  proc main() {
    writeln(x);
  }
}
