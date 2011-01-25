iter foo(param rank : int, n : int) {
  var result : rank*int;
  if rank == 1 then
    for i in 1..n do
      yield i;
  else {
    yield result;
    return;
  }
}

writeln(foo(1, 4));
writeln(foo(2, 4));
