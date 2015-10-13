// This is a bigger test case for strings in tuples.

proc showme(myarg) {
  writeln(myarg(2));
}

proc main {
  const ATUPLE = (111, 222, (12345, "hi"));
  writeln("starting: ", ATUPLE(3)(2));
  on Locales[1] {
    showme(ATUPLE(3));
  }
}
