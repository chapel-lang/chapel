iter foo(param rank : int) : int {
  for i in 1..5 do
    yield i + rank;
}

writeln( foo(2));
writeln( foo(3));
