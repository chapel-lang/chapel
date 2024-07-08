module M {
  var a : int = 3;
}

module N {
  use M except M.a;

  writeln("here");
  proc main() { }
}
