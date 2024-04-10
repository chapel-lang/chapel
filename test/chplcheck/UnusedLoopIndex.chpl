module UnusedLoopIndex {
  for i in 1..10 {}
  for (i, j) in zip(1..10, 1..10) {
    writeln(i);
  }
  for (i, j) in zip(1..10, 1..10) {
    writeln(j);
  }
  for (i, j) in zip(1..10, 1..10) {
  }
}
