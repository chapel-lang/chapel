iter myiter(ref n: int) {
  for i in 1..n {
    yield i;
    n -= 1;
  }
}

var i: int = 5;

for a in myiter(i) {
  writeln((i, a));
}
