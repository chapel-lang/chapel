// This is a small test case for strings in tuples.

proc main {
  const ATUPLE = (12345, "hi");
  writeln("starting: ", ATUPLE(2));
  on Locales[1] {
    writeln(ATUPLE(2));
  }
}
