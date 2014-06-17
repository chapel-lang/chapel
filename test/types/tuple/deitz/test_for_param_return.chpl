proc foo(param rank : int) {
  var x : rank*int;
  for param i in 1..rank do
    x(i) = i;
  writeln(x);
  for param i in 1..rank do
    if x(i) > 2 then
      return x(i);
  halt("unexpected case");
}

writeln(foo(4));
