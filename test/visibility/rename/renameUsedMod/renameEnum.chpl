module User {
  enum LongName { red, blue, yellow };

  use LongName as L;

  proc main() {
    writeln(L.red);
    writeln(red);
  }
}
