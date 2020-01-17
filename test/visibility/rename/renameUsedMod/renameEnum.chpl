module User {
  enum LongName { red, blue, yellow };

  use LongName as L;

  proc main() {
    writeln(L.red); // Should work, as L is now a symbol in context
    writeln(red); // Also should work, as we have used the enum
  }
}
