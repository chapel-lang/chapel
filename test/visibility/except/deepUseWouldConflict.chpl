module M4 {
  use M1 except a;
  use M3;
  use M2;

  proc main() {
    writeln(b);
    writeln(c);
    writeln(a);
    // a used to evaluate to M2's version but now conflicts
  }
}
