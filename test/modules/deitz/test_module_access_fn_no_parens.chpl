module M1 {
  proc f return 3;
}

module M2 {
  proc main {
    var f = 4;
    writeln(f);
    writeln(M1.f);
  }
}
