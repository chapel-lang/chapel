iter myiter(end:int(64)) {
  end = 10;
  for i in 1..end {
    yield i;
  }
}

for i in myiter(20) {
  writeln(i);
}

