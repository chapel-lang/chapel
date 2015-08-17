// This test exercises global variables that are tuples with strings.

const ATUPLE = (12345, "hi");
const BTUPLE = (111, 222, ATUPLE);
const CTUPLE = (111, 222, (12345, "bye"));

proc showme(myarg) {
  writeln(myarg(2));
}

proc main {
  writeln("starting: ", ATUPLE(2), BTUPLE(3)(2), CTUPLE(3)(2));
  on Locales[1] {
    showme(ATUPLE);
    showme(BTUPLE(3));
    showme(CTUPLE(3));
  }
}
