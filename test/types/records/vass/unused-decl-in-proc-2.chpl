module M {
  use N1, N2;
  proc main() { writeln("hello");  if false then q(); }
}

module N1 {
  proc p {
    record R {}
  }
}

module N2 {
  proc q() {
    record Q {}
    return new Q();
  }
}

module N3 {
  proc s() {
    record S {}
    return new S();
  }
}
