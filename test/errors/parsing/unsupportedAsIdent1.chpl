module M {
  var a : int = 3;
}

module N {
  use M as b.a;

  writeln("here");
  proc main() { }
}
