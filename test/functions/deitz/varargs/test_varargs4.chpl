param n : integer = 2;

function foo(ys : string ...n) {
  for i in 1..n do
    writeln(ys(i));
}

foo("hello", "world!");
