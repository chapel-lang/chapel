module M1 {
  var x: int = 2;
}

module M2 {
  use M1;
  var x: int = 3;
  proc main {
    writeln(x);
  }
}
