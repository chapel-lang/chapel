iter I(): int {
  param x = 7;

  for i in 1..10 {
    yield x+i;
  }
}

for i in I() {
  writeln("i is: ", i);
}
