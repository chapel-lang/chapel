var s : [1..5] int = (1, 2, 3, 4, 5);

proc foo(i : int) do
  return "hello world";

writeln(foo(s));
