iterator foo(param rank : int, n : int) : rank*int {
  var result : rank*int;
  for i in 1..n {
    for j in 1..rank {
      result(j) = i;
    }
    yield result;
  }
}

writeln( _to_seq( foo( 2, 4)));
writeln( _to_seq( foo( 4, 5)));
