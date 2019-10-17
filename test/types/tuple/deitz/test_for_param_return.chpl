proc foo(param rank : int) {
  var x : rank*int;
  for param i in 0..rank-1 do
    x(i) = i+1;
  writeln(x);
  for param i in 0..rank-1 do
    if x(i) > 2 then
      return x(i);
  halt("unexpected case");
}

writeln(foo(4));
