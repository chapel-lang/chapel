iter foo(param rank : int, n : int) : rank*int {
  var result : rank*int;
  for i in 1..n {
    for j in 0..#rank {
      result(j) = i;
    }
    yield result;
  }
}

writeln( foo( 2, 4));
writeln( foo( 4, 5));
