module M {
  var a : int = 3;
}

module N {
  import M.b;

  writeln("here");
  proc main() { }
}
