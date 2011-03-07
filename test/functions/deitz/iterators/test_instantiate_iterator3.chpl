iter foo(param rank : int, n : int) : rank*int {
  if rank > 2 {
    for i in 0..n-1 do
      for x in foo(rank-1, n) {
        var result : rank*int;
        for j in 1..rank-1 do
          result(j) = x(j);
        result(rank) = i;
        yield result;
      }
  } else if rank == 2 {
    for i in 0..n-1 do
      for x in foo(1, n) {
        var result : rank*int;
        result(1) = x(1);
        result(2) = i;
        yield result;
      }
  } else if rank == 1 {
    for i in 0..n-1 {
      var result : rank*int;
      result(1) = i;
      yield result;
    }
  }
}

writeln( foo( 1, 2));
writeln( foo( 2, 2));
writeln( foo( 3, 2));
