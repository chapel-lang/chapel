iterator basic() : integer {
  var i : integer;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}

for i in basic() {
  writeln(i);
}
