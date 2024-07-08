module M {
  var a : int = 3;
}

module N {
  use M only a.b as b;

  writeln("here");
  proc main() { }
}
