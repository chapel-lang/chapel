use Functions;

proc testme() {
  return 4;
}

var f: proc(): int = testme;

writeln(testme());
