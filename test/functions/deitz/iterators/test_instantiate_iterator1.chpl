iterator foo(param rank : int) : int {
  for i in 1..5 do
    yield i + rank;
}

writeln( _to_seq( foo(2)));
writeln( _to_seq( foo(3)));
