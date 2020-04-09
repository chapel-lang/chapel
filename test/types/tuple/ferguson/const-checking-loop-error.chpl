proc modit(const tup) {
  for a in tup {
    a = 1;
  }
}

proc test() {
  var tup:2*int;
  modit(tup);
  writeln(tup);
}
test();
