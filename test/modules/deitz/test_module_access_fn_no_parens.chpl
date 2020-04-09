module M1 {
  proc f return 3;
}

module M2 {
  use M1 only;
  proc main {
    var f = 4;
    writeln(f);
    writeln(M1.f);
  }
}
