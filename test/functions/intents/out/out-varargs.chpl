proc test_out(out xxx...) {
  xxx = (1,2,3);
}

proc main() {
  var x, y, z;
  test_out(x, y, z);
  writeln(x, " ", y, " ", z);
}
