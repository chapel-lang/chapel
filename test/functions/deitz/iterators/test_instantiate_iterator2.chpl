iterator foo(param rank : integer, n : integer) : (rank*integer) {
  var result : (rank*integer);
  for i in 1..n {
    for j in 1..rank {
      result(j) = i;
    }
    yield result;
  }
}

writeln(foo(2, 4));
writeln(foo(4, 5));
