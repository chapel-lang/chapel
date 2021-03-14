module M {
  var x: int;
  var y = "blah";
}

module N {
  public import M.{x, y};
}

module User {
  use N;

  proc main() {
    writeln(x);
    writeln(y);
    writeln(N.x);
    writeln(N.y);
  }
}
