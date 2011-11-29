use Functions;

proc testme() {
  return 4;
}

var f:func(int) = testme;

writeln(testme());