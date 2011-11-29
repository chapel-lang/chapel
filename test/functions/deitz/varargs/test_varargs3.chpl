proc foo(param n : int, ys : string ...n) {
  for i in 1..n do
    writeln(ys(i));
}

foo(2, "hello", "world!");
