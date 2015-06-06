// This test is intended to ensure that the access of some of a module's
// contents does not cause its contents to be visible without its name as a
// predicate.

module M {
  proc a(x: bool) {
    return !x;
  }

  proc b(y: int) {
    return 4 + y;
  }
}

module M2 {
  proc main() {
    writeln(M.a(true));
    writeln(b(3));
  }
}
