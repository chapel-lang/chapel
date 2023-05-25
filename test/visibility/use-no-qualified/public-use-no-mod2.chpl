module M {
  proc f() { writeln("M.f"); }
}

module N {
  public use M;

  proc main() {
    M.f(); // nope, M was not brought in
  }
}
