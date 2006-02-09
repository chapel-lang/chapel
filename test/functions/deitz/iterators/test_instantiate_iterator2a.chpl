iterator foo(param rank : integer, n : integer) : (rank*integer) {
  var result : (rank*integer);
  if rank == 1 then
    for i in 1..n do
      yield i;
  else
    return result;
}

writeln(foo(1, 4));
writeln(foo(2, 4));
