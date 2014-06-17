iter foo() : (int, int) {
  for i in 1..10 {
    for j in 1..10 {
      yield (i, j);
    }
  }
}

writeln( foo());
