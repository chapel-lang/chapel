module M {
  var x: int;
}

module N {
  private import M.x;
  var x: int;

  proc main() {
    writeln(x);
  }
}
