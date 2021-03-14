proc main {
  var sum = 0;

  for (i,j) in zip(1..10, 2..11) {
    sum += i + j;
  }

  writeln(sum);
}
