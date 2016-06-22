module M4 {
  use M1 except a;
  use M3;

  proc main() {
    writeln(b);
    writeln(c);
    writeln(a);
    // a should still be found, because M3 uses M1 without exception.
  }
}
