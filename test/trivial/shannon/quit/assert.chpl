module test_assert { // SJD: inserted to avoid name conflict with module
var x : integer = 1;

while (x) {
  assert(x != 10);
  x += 1;
}

writeln("This should never print.");
}
