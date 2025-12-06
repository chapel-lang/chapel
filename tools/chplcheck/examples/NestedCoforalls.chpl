/*
  Nested coforall loops can lead to performance issues.
*/

// This uses nested coforall loops which violates the rule.
coforall i in 1..10 {
  coforall j in 1..10 {
    writeln(i, j);
  }
}

// This uses a single coforall loop which is correct.
coforall i in 1..10 {
  for j in 1..10 {
    writeln(i, j);
  }
}
