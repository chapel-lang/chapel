iter myiter() {
  for i in 1..10 {
    yield i;
  }

  for i in 20..30 {
    yield i;
  }
  yield 0;
}

proc main {
  var sum = 0;

  for i in myiter() {
    sum += i;
  }

  writeln(sum);
}
